// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Internal class for opening and manipulating the bookmark database.
// @internalComponent
// 
//

#include <e32test.h>

#include "bkmrk.h"
#include "bkmrkfolder.h"
#include "bkmrkdb.h"
#include "repository.h"

_LIT8(KEmptyIconData, "");

const TInt KVersionMajor = 1;
const TInt KVersionMinor = 0;
const TInt KVersionBuild = 1;

const TInt 	KFirstIndex	= 1;

CBookmarkDb* CBookmarkDb::NewL(Bookmark::TVisibility aVisibility, MBookmarkObserver* aObserver)
	{
	CBookmarkDb* self = new (ELeave) CBookmarkDb(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aVisibility);
	CleanupStack::Pop(self);
	return self;
	}

CBookmarkDb::~CBookmarkDb()
	{
	delete iHomeText;
	delete iSearchUri;
	delete iDatabaseWatcher;
	delete iDatabaseRepository;
	delete iFolderWatcher;
	delete iFolderRepository;
	delete iBookmarkWatcher;
	delete iBookmarkRepository;
	delete iIconWatcher;
	delete iIconRepository;
	Reset();
	delete iCustomProperties;
	delete iPropertyRegister;

	delete iUnloadedFolderList;
	}

CBookmarkDb::CBookmarkDb(MBookmarkObserver* aObserver)
	: iObserver(aObserver)
	{
	}

void CBookmarkDb::ConstructL(Bookmark::TVisibility aVisibility)
	{
	RThread thread;
	TSecurityInfo secInfo(thread);
	TBool hasWriteUserData = secInfo.iCaps.HasCapability(ECapabilityWriteUserData);
	if (aVisibility == Bookmark::EVisibilityManager && !hasWriteUserData)
		{
		User::Leave(KErrPermissionDenied);
		}

	iVisibility = (aVisibility == Bookmark::EVisibilityDefault) ? Bookmark::EVisibilityAll : aVisibility;

	iDatabaseRepository = CRepository::NewL(KUidBookmarkDatabaseRepository);
	SetRepository(*iDatabaseRepository);
	iFolderRepository = CRepository::NewL(KUidFolderRepository);
	iBookmarkRepository = CRepository::NewL(KUidBookmarkRepository);
	iIconRepository = CRepository::NewL(KUidIconRepository);

	iPropertyRegister = CPropertyReg::NewL();
	iCustomProperties = CPropertyList::NewL(*iDatabaseRepository, *iPropertyRegister, CPropertyReg::EGroupDatabase, KDbCusPropertyStart);

	// Create the repositories if they don't exist
	TPckgBuf<TVersion> pckg;
	TInt err = iDatabaseRepository->Get(KVersionIndex, pckg);
	if (err == KErrNone)
		{
		TransactionL(ETransLoad);
		iBkmrkDbInitialized = ETrue;
		}
	else if (err == KErrNotFound)
		{
		// Set all as default empty values
		iHomeId 		= Bookmark::KNullItemID;
		iHomeText 		= KNullDesC().AllocL();
		iSearchUri 		= KNullDesC8().AllocL();
		iDefaultProxy 	= 0;
		iDefaultNap 	= 0;
		}
	else
		{
		User::Leave(err);
		}

	if (iObserver)
		{
		iDatabaseWatcher = new (ELeave) CBkmrkWatcher(EBkmrkWatcherDatabase, *this, *iDatabaseRepository);
		iDatabaseWatcher->WatchL();
		iFolderWatcher = new (ELeave) CBkmrkWatcher(EBkmrkWatcherFolder, *this, *iFolderRepository);
		iFolderWatcher->WatchL();
		iBookmarkWatcher = new (ELeave) CBkmrkWatcher(EBkmrkWatcherBookmark, *this, *iBookmarkRepository);
		iBookmarkWatcher->WatchL();
		iIconWatcher = new (ELeave) CBkmrkWatcher(EBkmrkWatcherIcon, *this, *iIconRepository);
		iIconWatcher->WatchL();
		}

	iTreeSync = EFalse;
	iConfigSync = EFalse;
	iIconSync = EFalse;

	iUnloadedFolderList = new (ELeave) CDesCArrayFlat(10);
	SetClean();
	}

void CBookmarkDb::Reset()
	{
	ResetTree();
	ResetIcons();
	if (iCustomProperties)
		{
		iCustomProperties->Reset();
		}
	}

void CBookmarkDb::ResetTree()
	{
	if (iRoot)
		{
		iRoot->Reset();
		delete iRoot;
		iRoot = NULL;
		iTreeConstructed = EFalse;
		}

	if (iUnloadedFolderList)
		{
		iUnloadedFolderList->Reset();
		}

	iFolderList.Reset();
	iBookmarkList.Reset();

	for (TInt index = iDeletedList.Count() - 1; index >= 0; --index)
		{
		delete iDeletedList[index];
		}
	iDeletedList.Reset();
	}

void CBookmarkDb::ResetIcons()
	{
	for (TInt index = iIconList.Count() - 1; index >= 0; --index)
		{
		delete iIconList[index];
		}
	iIconList.Reset();

	iIconSync = EFalse;
	}

void CBookmarkDb::RefreshL()
	{
	// If no repository files( no bookmark data ) exists, it is not required to
	// refresh the bookmark view. Check whether the repository has data or not
	// 
	if ( HasDataInRepositoryL () )
		{
		TransactionL(ETransLoad);
		ResetTree();
		ConstructTreeL();
		ResetIcons();
		LoadIconsL();				
		}		
	}

void CBookmarkDb::InitialiseRepL(CRepository& aRepository)
	{
	TPckgBuf<TVersion> pckg;
	TInt err = aRepository.Get(KVersionIndex, pckg);
	if (err == KErrNotFound)
		{
		TInt result = KErrLocked;
		do
			{
			aRepository.StartTransaction(CRepository::EConcurrentReadWriteTransaction);

			// create entries in the repository
			TVersion version(KVersionMajor, KVersionMinor, KVersionBuild);
			TPckgBuf<TVersion> pckg(version);
			aRepository.Create(KVersionIndex, pckg);
			aRepository.Create(KRepNextIndex, KFirstIndex);
		
			// commit the transaction
			TUint32 failedIndex;
			result = aRepository.CommitTransaction(failedIndex);
			} while (result == KErrLocked);
		User::LeaveIfError(result);
		}
	else if (err != KErrNone)
		{
		User::Leave(err);
		}
	}


void CBookmarkDb::CommitL()
	{
	CreateDefaultRepositoriesL ();
	
	CRepositoryAccessor::CommitL();

	iPropertyRegister->CommitL();
	iCustomProperties->CommitL();

	TInt index = iFolderList.Count() - 1;
	for (;index >= 0; --index)
		{
		iFolderList[index]->CommitL();
		}

	index = iBookmarkList.Count() - 1;
	for (;index >= 0; --index)
		{
		iBookmarkList[index]->CommitL();
		}

	index = iIconList.Count() - 1;
	for (;index >= 0; --index)
		{
		iIconList[index]->CommitL();
		}

	index = iDeletedList.Count() - 1;
	for (;index >= 0; --index)
		{
		iDeletedList[index]->CommitL();
		delete iDeletedList[index];
		iDeletedList[index] = NULL;
		}
	iDeletedList.Reset();
	}

CBookmarkFolder& CBookmarkDb::RootL()
	{
	ConstructTreeL();
	return *iRoot;
	}

CBookmarkFolder* CBookmarkDb::FindFolder(const TDesC& aFolderName)
	{
	TInt index = iFolderList.Count() - 1;
	for (;index >=0; --index)
		{
		if (iFolderList[index] != NULL &&
			aFolderName.Compare(iFolderList[index]->Title()) == 0)
			{
			return static_cast<CBookmarkFolder*>(iFolderList[index]);
			}
		}

	return NULL;
	}

CBookmark& CBookmarkDb::CreateBookmarkL(CBookmarkFolder& aParent)
	{
	CreateDefaultRepositoriesL ();
	ConstructTreeL();
	CBookmarkFolder* parent = &aParent;
	CBookmark* bookmark = CBookmark::NewL(*parent, *this);
	CleanupStack::PushL(bookmark);
	bookmark->AssignIdL();
	bookmark->SetTitleL(KNullDesC);
	iBookmarkList.AppendL(bookmark);
	CleanupStack::Pop(bookmark);

	return *bookmark;
	}

CBookmarkFolder& CBookmarkDb::CreateFolderL(const TDesC& aFolderName, CBookmarkFolder& aParent)
	{
	CreateDefaultRepositoriesL ();
	if (FindFolder(aFolderName) || FindUnloadedFolder(aFolderName))
		{
		User::Leave(Bookmark::KErrTitleAlreadyUsed);
		}

	ConstructTreeL();
	CBookmarkFolder* parent = &aParent;
	CBookmarkFolder* folder = CBookmarkFolder::NewL(parent, *this);
	CleanupStack::PushL(folder);
	folder->AssignIdL();
	folder->SetTitleL(aFolderName);
	iFolderList.AppendL(folder);
	CleanupStack::Pop(folder);

	return *folder;
	}

void CBookmarkDb::DeleteItemL(Bookmark::TItemId aItem, TBool aRecursive)
	{
	if (aItem == Bookmark::KRootItemID)
		{
		User::Leave(Bookmark::KErrOperationDenied);
		}

	CBookmarkBase* item = FindItem(aItem);
	if (!item)
		{
		User::Leave(KErrNotFound);
		}

  	if (item->Type() == Bookmark::ETypeFolder && !aRecursive)
		{
		CBookmarkFolder* folder = static_cast<CBookmarkFolder*>(item);
		folder->SendChildrenToFolderL(*iRoot);
		}

	// Bookmark items are not deleted until the database is commited
	// Deleted items are removed from the folder tree and go into a
	// 'deleted' list. On a commit the deleted list is deleted from memory
	// and from the repository.
  	item->DeleteL();
	}

void CBookmarkDb::RemoveBookmarkL(CBookmark* aBookmark)
	{
	TInt index = iBookmarkList.Find(aBookmark);
	if (index != KErrNotFound)
		{
		if (Home() == aBookmark)
			{
			SetHome(NULL);
			}
		iBookmarkList.Remove(index);
		iDeletedList.AppendL(aBookmark);
		}
	}

void CBookmarkDb::RemoveFolderL(CBookmarkFolder* aFolder)
	{
	TInt index = iFolderList.Find(aFolder);
	if (index != KErrNotFound)
		{
		iFolderList.Remove(index);
		iDeletedList.AppendL(aFolder);
		}
	}

const TDesC8& CBookmarkDb::IconL(Bookmark::TAttachmentId aIconId)
	{
	LoadIconsL();
	TInt index = FindIcon(aIconId);
	if (index == KErrNotFound)
		{
		User::Leave(KErrNotFound);
		}
	return iIconList[index]->Data();
	}

Bookmark::TAttachmentId CBookmarkDb::CreateIconL(const TDesC8& aData)
	{
	CreateDefaultRepositoriesL ();
	
	LoadIconsL();
	CBkmrkAttachment* icon = CBkmrkAttachment::NewL(*iIconRepository, aData);
	CleanupStack::PushL(icon);
	icon->SetIdL();
	iIconList.AppendL(icon);
	CleanupStack::Pop(icon);
	return icon->Id();
	}

TInt CBookmarkDb::DeleteIconL(Bookmark::TAttachmentId aIconId)
	{
	LoadIconsL();
	TInt index = FindIcon(aIconId);
	if (index == KErrNotFound)
		{
		return KErrNotFound;
		}

	iDeletedList.AppendL(iIconList[index]);
	iIconList[index]->DeleteL();
	iIconList.Remove(index);

	return KErrNone;
	}

TVersion CBookmarkDb::Version()
	{
	TVersion version(KVersionMajor, KVersionMinor, KVersionBuild);
	return version;
	}

Bookmark::TVisibility CBookmarkDb::Visibility()
	{
	return iVisibility;
	}

void CBookmarkDb::SetHome(CBookmark* aHome)
	{
	iHome = aHome;
	if (iHome)
		{
		iHomeId = iHome->Id();
		}
	else
		{
		iHomeId = Bookmark::KNullItemID;
		}
	SetDirty();
	}

CBookmark* CBookmarkDb::Home()
	{
	return iHome;
	}

const TDesC& CBookmarkDb::HomePageText() const
	{
	return *iHomeText;
	}

void CBookmarkDb::SetHomePageTextL(const TDesC& aHomePageText)
	{
	// The aHomePageText must be smaller that the maximim descriptor storage size
	__ASSERT_ALWAYS(aHomePageText.Length() <= Bookmark::KMaxDescriptorLength, User::Panic(Bookmark::KBookmarkErrTooLong, KErrArgument));

	delete iHomeText;
	iHomeText = NULL;
	iHomeText = aHomePageText.AllocL();
	SetDirty();
	}

const TDesC8& CBookmarkDb::SearchUri() const
	{
	return *iSearchUri;
	}

void CBookmarkDb::SetSearchUriL(const TDesC8& aUri)
	{
	// The aUri must be smaller that the maximim uri storage size
	__ASSERT_ALWAYS(aUri.Length() <= Bookmark::KMaxUriLength, User::Panic(Bookmark::KBookmarkErrTooLong, KErrArgument));

	delete iSearchUri;
	iSearchUri = NULL;
	iSearchUri = aUri.AllocL();
	SetDirty();
	}

TUint32 CBookmarkDb::DefaultProxy() const
	{
	return iDefaultProxy;
	}

void CBookmarkDb::SetDefaultProxy(TUint32 aServiceId)
	{
	iDefaultProxy = aServiceId;
	SetDirty();
	}

TUint32 CBookmarkDb::DefaultNap() const
	{
	return iDefaultNap;
	}

void CBookmarkDb::SetDefaultNap(TUint32 aNetworkId)
	{
	iDefaultNap = aNetworkId;
	SetDirty();
	}

TBool CBookmarkDb::IsTreeConstructed()
	{
	return (iRoot != NULL && iTreeConstructed);
	}

void CBookmarkDb::ConstructTreeL()
	{
	if (!iRoot)
		{
		iRoot = CBookmarkFolder::NewL(NULL, *this);
		iRoot->SetTitleL(Bookmark::KTxtRootTitle);
		iRoot->SetId(Bookmark::KRootItemID);
		}
	if ( !IsTreeConstructed() && HasDataInRepositoryL () )
		{
		// Load the tree only if the bookmark db is initialized
		// and the tree is not already constructed.
		LoadTreeL ();			
 		}
	}

CBookmarkBase* CBookmarkDb::FindItem(Bookmark::TItemId aItemId)
	{
	if (aItemId == Bookmark::KRootItemID)
		{
		return iRoot;
		}

	CBookmarkBase* item = NULL;

	TInt index = 0;
	TInt count = 0;
	// the top bit of the id indicates whether the id is a folder or bookmark
	TInt topBitSet = aItemId & KFolderIdMaskID;
	if (topBitSet == 0)
		{
		// item is a bookmark
		count = iBookmarkList.Count();
		for (index = 0; index < count; ++index)
			{
			if (iBookmarkList[index]->Id() == aItemId)
				{
				// bookmark item found
				item = iBookmarkList[index];
				break;
				}
			}
		}
	else
		{
		// item is a folder
		Bookmark::TItemId folderId;
		count = iFolderList.Count();
		for (index = 0; index < count; ++index)
			{
			folderId = iFolderList[index]->Id();
			if (folderId == aItemId)
				{
				// folder item found
				item = iFolderList[index];
				break;
				}
			}
		}

	return item;
	}

CRepository& CBookmarkDb::FolderRepository()
	{
	return *iFolderRepository;
	}

CRepository& CBookmarkDb::BookmarkRepository()
	{
	return *iBookmarkRepository;
	}

CPropertyList& CBookmarkDb::CustomProperties()
	{
	return *iCustomProperties;
	}

CPropertyReg& CBookmarkDb::PropertyRegister()
	{
	return *iPropertyRegister;
	}

void CBookmarkDb::BWONotify(TBkmrkWatcherType aType)
	{
	switch(aType)
		{
	case EBkmrkWatcherFolder:
	case EBkmrkWatcherBookmark:
		iObserver->MBONotify(MBookmarkObserver::EEventUpdatedBookmarkTree);
		iTreeSync = EFalse;
		break;
	case EBkmrkWatcherDatabase:
		iObserver->MBONotify(MBookmarkObserver::EEventUpdatedConfig);
		iConfigSync = EFalse;
		break;
	case EBkmrkWatcherIcon:
		iObserver->MBONotify(MBookmarkObserver::EEventUpdatedIcons);
		iIconSync = EFalse;
		break;
	default:
		break;
		}
	}

void CBookmarkDb::LoadBookmarkListL(CRepository& aRepository, TBkmrkList& aList, Bookmark::TType aType, TBool aLoadAll)
	{
	RArray<TUint32> entryList;
	CleanupClosePushL(entryList);
	aRepository.FindL(KRepIndexFind, KRepIndexMask, entryList);
	CBookmarkBase* newItem;
	TInt count = entryList.Count();
	for (TInt index = 0;index < count; ++index)
		{
		if (aType == Bookmark::ETypeFolder)
			{
			newItem = CBookmarkFolder::NewL(NULL, *this);
			}
		else
			{
			newItem = CBookmark::NewL(*iRoot, *this);
			}
		CleanupStack::PushL(newItem);
		newItem->SetIdFromIndexBase(entryList[index]);
		newItem->TransactionL(ETransLoad);
		if (aLoadAll || CheckAccessL(*newItem))
			{
			aList.AppendL(newItem);
			CleanupStack::Pop(newItem);
			}
		else
			{
			if (aType == Bookmark::ETypeFolder)
				{
				iUnloadedFolderList->AppendL(newItem->Title());
				}
			CleanupStack::PopAndDestroy(newItem);
			}
		}
	CleanupStack::PopAndDestroy(&entryList);
	}

void CBookmarkDb::AddItemsToFolderTreeL(TBkmrkList& aList)
	{
	TInt rank = 0;
	CBookmarkFolder* parent;
	CBookmarkBase* item;
	TInt count = aList.Count();
	for (TInt index = 0; index < count; ++index)
		{
		item = aList[index];
		parent = static_cast<CBookmarkFolder*>(FindItem(item->ParentId()));
		if (!parent)
			{
			item->SetOrphaned(ETrue);
			iRoot->AppendL(*item);
			}
		else
			{
			rank = item->Rank();
			if (rank < parent->Count())
				{
				parent->InsertL(*item, rank);
				}
			else
				{
				parent->AppendL(*item);
				}
			}
		item->SetClean();
		}
	}

TBool CBookmarkDb::RemoveFolderIfPublicL(CBookmarkFolder& aFolder)
	{
	// This method removes public folders in private-only mode.
	// If not in this mode this method should not be called
	__ASSERT_DEBUG(iVisibility == Bookmark::EVisibilityPrivate, User::Invariant());

	TBool containsPrivate = EFalse;
	for (TInt index = aFolder.Count() - 1; index >= 0; --index)
		{
		if (aFolder.At(index).Type() == Bookmark::ETypeFolder)
			{
			if (RemoveFolderIfPublicL(static_cast<CBookmarkFolder&>(aFolder.At(index))))
				{
				containsPrivate = ETrue;
				}
			}
		else
			{
			// child is a bookmark which will only exist if it is private
			containsPrivate = ETrue;
			}
		}

	if (&aFolder != iRoot && !containsPrivate && !CheckAccessL(aFolder))
		{
		// The folder is not the root, does not contain any private items and is not private itself
		// we can remove the folder
		TInt index = iFolderList.Find(&aFolder);
		iFolderList.Remove(index);
		iUnloadedFolderList->AppendL(aFolder.Title());
		delete &aFolder;
		}

	return containsPrivate;
	}

void CBookmarkDb::LoadTreeL()
	{
	if (HasOpenHandles())
		{
		User::Leave(Bookmark::KErrOpenHandle);
		}

	iLoading = ETrue;

	// If we are in private-only mode then we need to open all the folders initially.
	// We need to include public folders that contain private bookmarks
	TBool privateMode = (Visibility() == Bookmark::EVisibilityPrivate);

	// The pointers inside the folder list will not get deleted if LoadBookmarkListL or
	// AddItemsToFolderTreeL functions leave. Attach the folder list into TCleanupItem
	CleanupStack::PushL ( TCleanupItem ( CBookmarkDb::ResetAndDestroyBkmrkList, &iFolderList ) );
	// Load folders into folder list
	LoadBookmarkListL(*iFolderRepository, iFolderList, Bookmark::ETypeFolder, privateMode);
	AddItemsToFolderTreeL(iFolderList);
	CleanupStack::Pop (); // Pop the cleanup item object
	
	// Load bookmarks into bookmark list
	CleanupStack::PushL ( TCleanupItem ( CBookmarkDb::ResetAndDestroyBkmrkList, &iBookmarkList ) );
	LoadBookmarkListL(*iBookmarkRepository, iBookmarkList, Bookmark::ETypeBookmark, EFalse);
	AddItemsToFolderTreeL(iBookmarkList);
	CleanupStack::Pop (); // Pop the cleanup item object	
	if (privateMode)
		{
		// remove folders that are not private and contain no private items.
		RemoveFolderIfPublicL(*iRoot);
		}

	SetHome(static_cast<CBookmark*>(FindItem(iHomeId)));

	iLoading = EFalse;
	iTreeSync = ETrue;
	iTreeConstructed = ETrue;
	}

TBool CBookmarkDb::IsLoading()
	{
	return iLoading;
	}

void CBookmarkDb::LoadIconsL()
	{
	if (!iIconSync)
		{
		RArray<TUint32> entryList;
		CleanupClosePushL ( entryList );
		iIconRepository->FindL(KRepIndexFind, KRepIconIndexMask, entryList);
		CBkmrkAttachment* newIcon;
		TInt index = 0;
		TInt count = entryList.Count();
		for (;index < count; ++index)
			{
			newIcon = CBkmrkAttachment::NewL(*iIconRepository, KEmptyIconData);
			CleanupStack::PushL(newIcon);
			newIcon->SetIdFromIndexBase(entryList[index]);
			newIcon->TransactionL(ETransLoad);
			iIconList.AppendL(newIcon);
			CleanupStack::Pop(newIcon);
			newIcon->SetClean();
			}
		CleanupStack::PopAndDestroy ( &entryList );

		iIconSync = ETrue;
		}

	}

TBool CBookmarkDb::HasOpenHandles()
	{
	if (!iRoot)
		{
		return EFalse;
		}

	TInt index = iFolderList.Count() -1;
	for (; index >= 0; --index)
		{
		if (iFolderList[index]->RefCount() != 0)
			{
			return ETrue;
			}
		}

	index = iBookmarkList.Count() -1;
	for (; index >= 0; --index)
		{
		if (iBookmarkList[index]->RefCount() != 0)
			{
			return ETrue;
			}
		}

	return EFalse;
	}

TInt CBookmarkDb::FindIcon(Bookmark::TAttachmentId aIconId)
	{
	TInt index = iIconList.Count() - 1;
	for (;index >= 0; --index)
		{
		if (iIconList[index]->Id() == aIconId)
			{
			break;
			}
		}
	return index;
	}

TBool CBookmarkDb::FindUnloadedFolder(const TDesC& aFolderName)
	{
	TInt index = iUnloadedFolderList->Count() - 1;
	for (;index >=0; --index)
		{
		if (aFolderName.Compare((*iUnloadedFolderList)[index]) == 0)
			{
			return ETrue;
			}
		}

	return EFalse;
	}

TBool CBookmarkDb::CheckAccessL(CBookmarkBase& aItem)
	{
	TBool accessible = EFalse;

	// Always accessible if the item is the home page
	if (aItem.Type() == Bookmark::ETypeBookmark && aItem.Id() == iHomeId)
	{
		return ETrue;
	}

	TBool owned = EFalse;
	if (!aItem.IsPublic())
		{
		RThread thread;
		TSecurityInfo secInfo(thread);
		TSecureId id;
		aItem.GetOwnerL(id);
		if (id == secInfo.iSecureId)
			{
			owned = ETrue;
			}
		}

	switch(iVisibility)
		{
		case Bookmark::EVisibilityPublic:
			accessible = aItem.IsPublic();
			break;
		case Bookmark::EVisibilityPrivate:
			accessible = owned;
			break;
		case Bookmark::EVisibilityAll:
			accessible = (owned || aItem.IsPublic());
			break;
		case Bookmark::EVisibilityManager:
			accessible = ETrue;
			break;
		case Bookmark::EVisibilityDefault:
		default:
			// should have been assigned to one of the values above
			// during construction so shouldn't get here
			User::Invariant();
			break;
		}

	return accessible;
	}


void CBookmarkDb::TransNewL()
	{
	TInt customPropertyCount = 0;
	// create entries in the repository
	TVersion version(KVersionMajor, KVersionMinor, KVersionBuild);
	TPckgBuf<TVersion> pckg(version);
	iRepository->Create(KVersionIndex, pckg);
	iRepository->Create(KDbHomeIdIndex, iHomeId);
	iRepository->Create(KDbHomeTextIndex, *iHomeText);
	iRepository->Create(KDbSearchUriIndex, *iSearchUri);
	iRepository->Create(KDbDefaultProxyIndex, iDefaultProxy);
	iRepository->Create(KDbDefaultNapIndex, iDefaultNap);
	iRepository->Create(KRepBkmrkPropertyStart, customPropertyCount);
	iRepository->Create(KRepFldrPropertyStart, customPropertyCount);
	iRepository->Create(KRepDbPropertyStart, customPropertyCount);
	}

void CBookmarkDb::TransSaveL()
	{
	// create entries in the repository
	TVersion version(KVersionMajor, KVersionMinor, KVersionBuild);
	TPckgBuf<TVersion> pckg(version);
	iRepository->Set(KVersionIndex, pckg);
	iRepository->Set(KDbHomeIdIndex, iHomeId);
	iRepository->Set(KDbHomeTextIndex, *iHomeText);
	iRepository->Set(KDbSearchUriIndex, *iSearchUri);
	iRepository->Set(KDbDefaultProxyIndex, iDefaultProxy);
	iRepository->Set(KDbDefaultNapIndex, iDefaultNap);
	}

void CBookmarkDb::TransLoadL()
	{
	TPckgBuf<TVersion> pckg;
	User::LeaveIfError(iRepository->Get(KVersionIndex, pckg));
	TVersion version = pckg();
	if (version.iMajor != KVersionMajor ||
		version.iMinor != KVersionMinor )		
		{
		User::Leave(Bookmark::KErrBadVersion);
		}

	User::LeaveIfError(iRepository->Get(KDbHomeIdIndex, iHomeId));
	HBufC* homeText = HBufC::NewLC(Bookmark::KMaxDescriptorLength);
	TPtr16 ptr = homeText->Des();
	User::LeaveIfError(iRepository->Get(KDbHomeTextIndex, ptr));
	SetHomePageTextL(*homeText);
	CleanupStack::PopAndDestroy(homeText);


	HBufC8* searchUri = HBufC8::NewLC(Bookmark::KMaxDataLength);
	TPtr8 ptr8 = searchUri->Des();
	User::LeaveIfError(iRepository->Get(KDbSearchUriIndex, ptr8));
	SetSearchUriL(*searchUri);
	CleanupStack::PopAndDestroy(searchUri);

	User::LeaveIfError(iRepository->Get(KDbDefaultProxyIndex, iDefaultProxy));
	User::LeaveIfError(iRepository->Get(KDbDefaultNapIndex, iDefaultNap));

	iCustomProperties->TransLoadL();
	SetClean();
	iTreeSync = ETrue;
	}

void CBookmarkDb::TransRemoveL()
	{
	iDatabaseRepository->Delete(KVersionIndex);
	iDatabaseRepository->Delete(KDbHomeIdIndex);
	iDatabaseRepository->Delete(KDbHomeTextIndex);
	iDatabaseRepository->Delete(KDbSearchUriIndex);
	iDatabaseRepository->Delete(KDbDefaultProxyIndex);
	iDatabaseRepository->Delete(KDbDefaultNapIndex);
	iDatabaseRepository->Delete(KRepBkmrkPropertyStart);
	iDatabaseRepository->Delete(KRepFldrPropertyStart);
	iDatabaseRepository->Delete(KRepDbPropertyStart);
	}

TUint32 CBookmarkDb::IndexBase()
	{
	return KIndexBaseZero;
	}

void CBookmarkDb::SetIdFromIndexBase(TUint32 /*aIndexBase*/)
	{
	// the id and index base is always 0 so do nothing
	}

void CBookmarkDb::ResetAndDestroyBkmrkList ( TAny* aPtr )
	{
	TBkmrkList& bkList = *( TBkmrkList* )aPtr;
 	
	// Delete all items added. Otherwise these items will not get deleted and there will
	// be a memory leak.
	for ( TInt index = bkList.Count() - 1; index >= 0; --index )
		{
		delete bkList[ index ];			
		}
	bkList.Reset ();	
	}

void CBookmarkDb::CreateDefaultRepositoriesL ()
	{
	if ( !iBkmrkDbInitialized )
		{		
		TransactionL ( ETransNew );		
		InitialiseRepL ( *iFolderRepository );
		InitialiseRepL ( *iBookmarkRepository );
		InitialiseRepL ( *iIconRepository );		
		iBkmrkDbInitialized = ETrue;		
		}
	}

TBool CBookmarkDb::HasDataInRepositoryL ()
	{	
	if ( !iBkmrkDbInitialized )
		{
		const TUint32 KNullKey = 0x00000000;
		RArray<TUint32> foundKeys;
		CleanupClosePushL ( foundKeys );	
		iDatabaseRepository->FindL ( KNullKey, KNullKey, foundKeys );	
		if ( foundKeys.Count () > 0 )
			{
			// Repository has data.
			iBkmrkDbInitialized = ETrue;			
			}	
		CleanupStack::PopAndDestroy (); // foundKeys		
		}	
	return iBkmrkDbInitialized;
	}
	
#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}
#endif // EKA2
