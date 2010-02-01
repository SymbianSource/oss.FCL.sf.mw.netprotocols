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
// Internal class for the bookmark item base class.
// @internalComponent
// 
//

//#include <bookmarkdef.h>
#include "bkmrk.h"
#include "bkmrkfolder.h"

CBookmarkBase::CBookmarkBase(CBookmarkDb& aDb, CRepository& aRepository)
	: CRepositoryAccessor(), iDatabase(aDb), iRefCount(0), iId(Bookmark::KNullItemID), iParent(NULL), iFlags(EFlagPublic|EFlagWritable)
	{
	SetRepository(aRepository);
	iStatus = EStatusCreating;
	}

CBookmarkBase::~CBookmarkBase()
	{
	if (iParent)
		{
		iParent->Remove(*this);
		}
	}

Bookmark::TItemId CBookmarkBase::Id()
	{
	return iId;
	}

void CBookmarkBase::AssignIdL()
	{
	iId = NextIndexL();
	if (Type() == Bookmark::ETypeFolder)
		{
		iId |= KFolderIdMaskID;
		}
	}

void CBookmarkBase::SetId(Bookmark::TItemId aId)
	{
	iId = aId;
	}

RBkNode CBookmarkBase::OpenItemL()
	{
	RBkNode item;
	item.SetItem(*this);
	return item;
	}

void CBookmarkBase::SetDirty()
	{
	if (!iDatabase.IsLoading())
		{
		CRepositoryAccessor::SetDirty();
		}
	}

Bookmark::TItemId CBookmarkBase::ParentId() const
	{
	return iParentId;
	}

CBookmarkFolder* CBookmarkBase::Parent() const
	{
	return iParent;
	}

// Should not be used. Use CBookmarkFolder::AppendL or InsertL
// to an item to another folders.
void CBookmarkBase::SetParentL(CBookmarkFolder& aNewParent)
	{
	// No item should set it's parent to itself
	__ASSERT_ALWAYS(&aNewParent != this, User::Panic(Bookmark::KBookmarkErrParent, Bookmark::KErrCyclicLoop));
	LeaveIfNotWritableL();

	// Ensure we're not trying to set the parent of an item to a folder it owns.
	if (this->Type() == Bookmark::ETypeFolder &&
		aNewParent.IsAChild(static_cast<CBookmarkFolder*>(this)))
		{
		User::Leave(Bookmark::KErrCyclicLoop);
		}

	iParent = &aNewParent;
	iParentId = aNewParent.Id();
	SetDirty();
	}

void CBookmarkBase::SetTitleL(const TDesC& aTitle)
	{
	// The aTitle must be smaller that the maximim descriptor storage size
	__ASSERT_ALWAYS(aTitle.Length() <= Bookmark::KMaxDescriptorLength, User::Panic(Bookmark::KBookmarkErrTooLong, KErrArgument));
	
	if (Type() == Bookmark::ETypeFolder && 
		(iDatabase.FindFolder(aTitle) != NULL || 
		iDatabase.FindUnloadedFolder(aTitle) != NULL))
		{
		User::Leave(Bookmark::KErrTitleAlreadyUsed);
		}

	LeaveIfNotWritableL();
	if (iTitle)
		{
		delete iTitle;
		}

	iTitle = NULL;
	iTitle = aTitle.AllocL();
	SetDirty();
	}

const TDesC& CBookmarkBase::Title() const
	{
	if ( !iTitle )
		{
		return KNullDesC;
		}
 	return *iTitle;
	}

void CBookmarkBase::IncRefCount()
	{
	++iRefCount;
	}

void CBookmarkBase::DecRefCount()
	{
	__ASSERT_ALWAYS(iRefCount > 0, User::Panic(Bookmark::KBookmarkErrNegativeRefCount, KErrGeneral));
	--iRefCount;
	}

TUint CBookmarkBase::RefCount()
	{
	return iRefCount;
	}

TBool CBookmarkBase::IsPublic()
	{
	return iFlags & EFlagPublic;
	}

void CBookmarkBase::SetPublicL(TBool aPublic)
	{
	LeaveIfNotWritableL();
	
	// The home page cannot be set as private
	if (!aPublic &&
		Type() == Bookmark::ETypeBookmark &&		
		static_cast<CBookmark*>(this)->IsHomePage())
		{
		User::Leave(Bookmark::KErrOperationDenied);
		}
	
	// You can't change the public-ness of an item in public only and private only modes.
	if (iDatabase.Visibility() == Bookmark::EVisibilityPublic || iDatabase.Visibility() == Bookmark::EVisibilityPrivate)
		{
		User::Leave(KErrPermissionDenied);
		}
		
	if (aPublic)
		{
		iFlags = iFlags | EFlagPublic;
		}
	else
		{
		RThread thread;
		TSecurityInfo secInfo(thread);
		SetOwnerL(secInfo.iSecureId);
		}
	SetDirty();
	}

TBool CBookmarkBase::IsWritable()
	{
	return iFlags & EFlagWritable;
	}

void CBookmarkBase::SetWritableL(TBool aWritable)
	{
	if (aWritable)
		{
		LeaveIfNotInManagerModeL ();
		iFlags = iFlags | EFlagWritable;
		}
	else
		{
		iFlags = iFlags & ~EFlagWritable;
		}
	SetDirty();
	}

TBool CBookmarkBase::IsOrphaned()
	{
	return iFlags & EFlagOrphaned;
	}

void CBookmarkBase::SetOrphaned(TBool aOrphaned)
	{
	if (aOrphaned)
		{
		iFlags = iFlags | EFlagOrphaned;
		}
	else
		{
		iFlags = iFlags & ~EFlagOrphaned;
		}
	}

void CBookmarkBase::GetOwnerL(TSecureId& aOwner)
	{
	TInt id = Bookmark::KNoOwnerId;
	TInt err = iRepository->Get(IndexBase() + KCmnOwnerIndex, id);
	if (err != KErrNone && err != KErrNotFound)
		{
		User::Leave(err);
		}
	aOwner.iId = static_cast<TUint32>(id);
	}

void CBookmarkBase::SetOwnerL(TSecureId aOwner)
	{
	if ( iDatabase.Visibility() != Bookmark::EVisibilityManager )
		{
		RThread thread;
		TSecurityInfo secInfo ( thread );
		
		if ( aOwner != secInfo.iSecureId )
			{
			User::Leave ( KErrPermissionDenied );
			}
		}
	
	// The owner is never stored in the class as a member variable
	if (iStatus == EStatusCreating)
		{
		// the owner id is not stored in the class so it can't be set until the bookmark 
		// had been committed at least once
		User::Leave(Bookmark::KErrNotCommitted);
		}
	else
		{
		TInt id = Bookmark::KNoOwnerId;
		if (aOwner != Bookmark::KNoOwnerId)
			{
			id = static_cast<TInt>(aOwner.iId);
			}
		User::LeaveIfError(iRepository->Set(IndexBase() + KCmnOwnerIndex, id));
		}
	iFlags = iFlags & ~EFlagPublic;
	SetDirty();
	}

TInt CBookmarkBase::Rank()
	{
	return iRank;
	}

void CBookmarkBase::LeaveIfNotWritableL()
	{
	if ( iDatabase.Visibility() != Bookmark::EVisibilityManager && 
		!iDatabase.IsLoading() && 
		!IsWritable())
 		{
 		User::Leave(Bookmark::KErrReadOnly);
 		}	
 	}

void CBookmarkBase::LeaveIfNotInManagerModeL ()
	{
	if ( iDatabase.Visibility() != Bookmark::EVisibilityManager )
 		{
		User::Leave ( KErrPermissionDenied );
 		}
 	}

TBool CBookmarkBase::IsAChild(CBookmarkFolder* aAncestor)
	{
	CBookmarkFolder* parent = iParent;
	for (TInt folderDepth = 0; parent != NULL && folderDepth < Bookmark::KMaxFolderDepth; ++folderDepth)
		{
		if (parent == aAncestor)
			{
			return ETrue;
			}
		parent = parent->Parent();
		}

	return EFalse;
	}

CBkmrkProperties& CBookmarkBase::ExtendedPropertiesL()
	{
	if (Type() == Bookmark::ETypeFolder)
		{
		return static_cast<CBookmarkFolder*>(this)->BkmrkPropertiesL();
		}
	else
		{
		return static_cast<CBookmark*>(this)->BkmrkExtendedPropertiesL();
		}		
	}

void CBookmarkBase::TransNewL()
	{
	TUint32 indexBase = IndexBase();
	// create entries in the repository
	iRepository->Create(indexBase + KCmnTitleIndex, Title());
	iRepository->Create(indexBase + KCmnParentIndex, static_cast<TInt>(iParent->Id()));
	iRepository->Create(indexBase + KCmnFlagsIndex, static_cast<TInt>(iFlags));
	TInt rank = iParent->Index(*this);
	iRepository->Create(indexBase + KCmnRankIndex, rank);

	TInt id = Bookmark::KNoOwnerId;
	if (!(iFlags & EFlagPublic))
		{
		RThread thread;
		TSecurityInfo secInfo(thread);
		id = static_cast<TInt>(secInfo.iSecureId);
		}
	iRepository->Create(indexBase + KCmnOwnerIndex, id);
	}

void CBookmarkBase::TransSaveL()
	{
	TUint32 indexBase = IndexBase();
	// create entries in the repository
	iRepository->Set(indexBase + KCmnTitleIndex, Title());
	iRepository->Set(indexBase + KCmnParentIndex, static_cast<TInt>(iParent->Id()));
	iRepository->Set(indexBase + KCmnFlagsIndex, static_cast<TInt>(iFlags));
	TInt rank = iParent->Index(*this);
	iRepository->Set(indexBase + KCmnRankIndex, rank);
	}

void CBookmarkBase::TransLoadL()
	{
	TUint32 indexBase = IndexBase();
	HBufC* titleBuffer = HBufC::NewLC(Bookmark::KMaxDescriptorLength);
	TPtr titlePtr = titleBuffer->Des();
	User::LeaveIfError(iRepository->Get(indexBase + KCmnTitleIndex, titlePtr));
	delete iTitle;
	iTitle = NULL;
	iTitle = titlePtr.AllocL();
	CleanupStack::PopAndDestroy(titleBuffer);

	TInt retInt = 0;
	User::LeaveIfError(iRepository->Get(indexBase + KCmnParentIndex, retInt));
	iParentId = static_cast<Bookmark::TItemId>(retInt);
	User::LeaveIfError(iRepository->Get(indexBase + KCmnFlagsIndex, retInt));
	iFlags = static_cast<TUint32>(retInt);
	User::LeaveIfError(iRepository->Get(indexBase + KCmnRankIndex, iRank))	;
	SetClean();
	}

void CBookmarkBase::TransRemoveL()
	{
	TUint32 indexBase = IndexBase();
	iRepository->Delete(indexBase + KCmnTitleIndex);
	iRepository->Delete(indexBase + KCmnParentIndex);
	iRepository->Delete(indexBase + KCmnFlagsIndex);
	iRepository->Delete(indexBase + KCmnRankIndex);
	iRepository->Delete(indexBase + KCmnOwnerIndex);
	}
	
