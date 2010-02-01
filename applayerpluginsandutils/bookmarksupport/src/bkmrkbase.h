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
// Classes for manipulating bookmarks and bookmark folders.
// @internalComponent
// 
//

#ifndef __BKMRKBASE_H__
#define __BKMRKBASE_H__

#include <uri8.h>
#include <centralrepository.h>
#include <bookmarkdef.h>
#include <bookmark.h>
#include "bkmrkdb.h"
#include "repository.h"

/**
The base class for bookmark items. This is an abstract class. All concrete bookmark objects derive
from this and are either bookmarks or folders. It is responsible for managing common properties
in both bookmarks and folders
*/
class CBookmarkBase: public CRepositoryAccessor
	{
protected:
	enum TFlags
		{
		EFlagNull		= 0x0,
		EFlagPublic		= 0x1,
		EFlagWritable	= 0x2,
		EFlagOrphaned	= 0x4
		};

public:
	virtual ~CBookmarkBase();

	virtual Bookmark::TType Type() const = 0;

	Bookmark::TItemId Id();
	void AssignIdL();
	void SetId(Bookmark::TItemId);

	RBkNode OpenItemL();

	void IncRefCount();
	void DecRefCount();
	TUint RefCount();

	Bookmark::TItemId ParentId() const;
	CBookmarkFolder* Parent() const;
	void SetParentL(CBookmarkFolder& aNewParent);

	const TDesC& Title() const;
	void SetTitleL(const TDesC& aTitle);

	TBool IsPublic();
	void SetPublicL(TBool aPublic);

	TBool IsWritable();
	void SetWritableL(TBool aWritable);

	TBool IsOrphaned();
	void SetOrphaned(TBool aOrphaned);

	void GetOwnerL(TSecureId& aOwner);
	void SetOwnerL(TSecureId aOwner = Bookmark::KNoOwnerId);

	TInt Rank();
	TBool IsAChild(CBookmarkFolder* aParent);

	CBkmrkProperties& ExtendedPropertiesL();
	
	void LeaveIfNotWritableL();
	
	// From CRepositoryAccessor
	void SetDirty();
	
	void LeaveIfNotInManagerModeL ();
protected:
	// From CRepositoryAccessor
	virtual void TransSaveL();
	virtual void TransNewL();
	virtual void TransLoadL();
	virtual void TransRemoveL();
	CBookmarkBase(CBookmarkDb& aDb, CRepository& aRepository);

protected:
	CBookmarkDb&		iDatabase;
	TUint 				iRefCount;
	Bookmark::TItemId	iId;
	CBookmarkFolder*	iParent;
	Bookmark::TItemId	iParentId;
	TInt				iRank;
	TUint32				iFlags;
	HBufC* 				iTitle;
	};


#endif //__BKMRKBASE_H__
