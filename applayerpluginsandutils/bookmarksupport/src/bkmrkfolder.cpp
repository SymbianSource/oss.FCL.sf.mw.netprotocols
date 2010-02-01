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
// Internal class for manipulating the bookmark folders.
// @internalComponent
// 
//

#include "bkmrk.h"
#include "bkmrkfolder.h"
#include "bkmrkdb.h"
#include "repository.h"

CBookmarkFolder* CBookmarkFolder::NewL(CBookmarkFolder* aParent, CBookmarkDb& aDb)
	{
	CBookmarkFolder* self = new (ELeave) CBookmarkFolder(aDb);
	CleanupStack::PushL(self);
	self->ConstructL(aParent);
	CleanupStack::Pop(self);
	return self;
	}

CBookmarkFolder::CBookmarkFolder(CBookmarkDb& aDb)
	: CBookmarkBase(aDb, aDb.FolderRepository())
	{
	}

CBookmarkFolder::~CBookmarkFolder()
	{
	Reset();

	delete iTitle;
	delete iProperties;
	}

void CBookmarkFolder::DeleteL()
	{
	TInt index = iChildren.Count() - 1;
	for (;index >= 0; --index)
		{
		// there should be no open handles to any bookmarks
		__ASSERT_ALWAYS(iChildren[index]->RefCount() == 0,
						User::Panic(Bookmark::KBookmarkErrOpenHandles, KErrGeneral));
		iChildren[index]->DeleteL();
		}
	iChildren.Reset();

	iDatabase.RemoveFolderL(this);
  	iParent->Remove(*this);
  	iParent = NULL;

	iStatus = EStatusDeleted;
	}

void CBookmarkFolder::Reset()
	{
	TInt index = iChildren.Count() - 1;
	for (;index >= 0; --index)
		{
		// there should be no open handles to any bookmarks
		__ASSERT_ALWAYS(iChildren[index]->RefCount() == 0,
						User::Panic(Bookmark::KBookmarkErrOpenHandles, KErrGeneral));
		delete iChildren[index];
		}
	iChildren.Reset();
	}

void CBookmarkFolder::ConstructL(CBookmarkFolder* aParent)
	{
	if (aParent)
		{
		aParent->AppendL(*this);
		}
	}

Bookmark::TType CBookmarkFolder::Type() const
	{
	return Bookmark::ETypeFolder;
	}

RBkFolder CBookmarkFolder::OpenFolder()
	{
	RBkFolder folder;
	folder.SetItem(*this);
	return folder;
	}

CBkmrkProperties& CBookmarkFolder::BkmrkPropertiesL()
	{
	if (!iProperties)
		{
		iProperties = CBkmrkProperties::NewL(*this, iDatabase, *iRepository);
		if (iStatus != EStatusCreating)
			{
			iProperties->TransactionL(ETransLoad);
			}
		}

	return *iProperties;
	}

void CBookmarkFolder::SendChildrenToFolderL(CBookmarkFolder& aFolder)
	{
	TInt index = iChildren.Count() - 1;
	for (;index >= 0; --index)
		{
		aFolder.AppendL(*(iChildren[index]));
		}
	iChildren.Reset();
	}

void CBookmarkFolder::AppendL(CBookmarkBase& aBookmarkItem)
	{
	LeaveIfNotWritableL();
	CBookmarkFolder* parent = aBookmarkItem.Parent();
	aBookmarkItem.SetParentL(*this);
	if (parent)
		{
		parent->Remove(aBookmarkItem);
		}
	iChildren.AppendL(&aBookmarkItem);
	aBookmarkItem.SetDirty();
	}

void CBookmarkFolder::InsertL(CBookmarkBase& aBookmarkItem, TInt aPosition)
	{
	LeaveIfNotWritableL();
	CBookmarkFolder* parent = aBookmarkItem.Parent();

	if (parent == this)
		{
		if (Index(aBookmarkItem) == aPosition)
			{
			// If the item is already in the right position in this folder
			// then there is nothing to do.
			return;
			}
		}
	else
		{
		aBookmarkItem.SetParentL(*this);
		}

	if (parent)
		{
		parent->Remove(aBookmarkItem);
		}
	iChildren.InsertL(&aBookmarkItem, aPosition);
	aBookmarkItem.SetDirty();

	// This changes the rank of all children after the insertion. Touch each
	// child so that this is saved on a commit
	TInt index = iChildren.Count() - 1;
	for (;index >= aPosition; --index)
		{
		if (!iChildren[index]->IsOrphaned())
			{
			iChildren[index]->SetDirty();
			}
		}
	}

TInt CBookmarkFolder::Move(TInt aOldPosition, TInt aNewPosition)
	{
	TRAPD( err, LeaveIfNotWritableL() );
	if (err != KErrNone)
		{
		return err;
		}
	TInt maxIndex = iChildren.Count() -1;
	if (aOldPosition < 0 || aOldPosition > maxIndex ||
		aNewPosition < 0 || aNewPosition > maxIndex)
		{
		return KErrArgument;
		}
	if (iChildren[aOldPosition]->IsOrphaned())
		{
		return Bookmark::KErrOrphanedItem;
		}
	CBookmarkBase* item = iChildren[aOldPosition];
	iChildren.Remove(aOldPosition);
	iChildren.Insert(item, aNewPosition);
	return KErrNone;
	}

TInt CBookmarkFolder::Index(CBookmarkBase& aItem) const
	{
	return iChildren.Find(&aItem);
	}

TInt CBookmarkFolder::Find(Bookmark::TItemId aId) const
	{
	TInt index = iChildren.Count() - 1;
	for (; index >= 0; --index)
		{
		if (iChildren[index]->Id() == aId)
			{
			break;
			}
		}
	return index;
	}

TInt CBookmarkFolder::Count() const
	{
	return iChildren.Count();
	}

CBookmarkBase& CBookmarkFolder::At(TInt aIndex)
	{
	return *(iChildren[aIndex]);
	}

void CBookmarkFolder::Remove(CBookmarkBase& aBookmarkItem)
	{
	TInt index = iChildren.Find(&aBookmarkItem);
	if (index != KErrNotFound)
		{
		iChildren.Remove(index);
		}
	}

TUint32 CBookmarkFolder::IndexBase()
	{
	// Folder ids have the top bit set. This ensures that bookmark ids and folder ids don't
	// clash. This bit needs to be removed to get the actual index value 
	TUint32 indexBase = iId & ~KFolderIdMaskID;
	return (indexBase << KBookmarkIndexShift);
	}

void CBookmarkFolder::SetIdFromIndexBase(TUint32 aIndexBase)
	{
	iId = aIndexBase >> KBookmarkIndexShift;
	iId |= KFolderIdMaskID;
	}

void CBookmarkFolder::TransNewL()
	{
	CBookmarkBase::TransNewL();
	BkmrkPropertiesL().TransNewL();
	}

void CBookmarkFolder::TransSaveL()
	{
	CBookmarkBase::TransSaveL();
	if (iProperties)
		{
		BkmrkPropertiesL().TransSaveL();
		}
	}

void CBookmarkFolder::TransLoadL()
	{
	CBookmarkBase::TransLoadL();
	if (iProperties)
		{
		BkmrkPropertiesL().TransLoadL();
		}
	SetClean();
	}

void CBookmarkFolder::TransRemoveL()
	{
	CBookmarkBase::TransRemoveL();

	// ensure extended properties are loaded
	BkmrkPropertiesL();
	iProperties->TransRemoveL();
	}
	
