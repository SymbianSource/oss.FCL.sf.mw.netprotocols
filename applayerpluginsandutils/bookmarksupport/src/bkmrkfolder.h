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

#ifndef __BKMRKFOLDER_H__
#define __BKMRKFOLDER_H__

#include "bkmrkbase.h"
#include "bkmrkproperties.h"

const Bookmark::TItemId KFolderIdMaskID = 0x80000000;

/**
Internal representation of a bookmark folder. It contains the folder specific properties and holds
a pointer to an extended properties object. This object is only loaded when needed. 
A folder holds a list of child bookmark items. These can be either bookmarks or further folders.
CBookmarkFolder provides methods for inserting, removing and moving these child bookmark items.
*/
class CBookmarkFolder: public CBookmarkBase
	{
public:
	static CBookmarkFolder* NewL(CBookmarkFolder* aParent, CBookmarkDb& aDb);
	~CBookmarkFolder();
	void DeleteL();

	// From CBookmarkBase
	Bookmark::TType Type() const;

	RBkFolder OpenFolder();

	void SendChildrenToFolderL(CBookmarkFolder& aFolder);

	void AppendL(CBookmarkBase& aBookmarkItem);
	void InsertL(CBookmarkBase& aBookmarkItem, TInt aIndexPosition);

	TInt Move(TInt aOldPosition, TInt aNewPosition);
	TInt Count() const;

	TInt Index(CBookmarkBase& aBookmarkItem) const;
	TInt Find(Bookmark::TItemId aId) const;

	CBookmarkBase& At(TInt aIndex);

	void Remove(CBookmarkBase& aBookmarkItem);
	void Reset();

	CBkmrkProperties& BkmrkPropertiesL();

	// From CRepositoryAccessor
	TUint32 IndexBase();
	void SetIdFromIndexBase(TUint32 aIndexBase);
	void TransSaveL();
	void TransNewL();
	void TransLoadL();
	void TransRemoveL();
	
private:
	CBookmarkFolder(CBookmarkDb& aDb);
	void ConstructL(CBookmarkFolder* aParent);

private:
	RArray<CBookmarkBase*>			iChildren;
	CBkmrkProperties*				iProperties;
	};

#endif //__BKMRKFOLDER_H__
