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

#ifndef __BKMRK_H__
#define __BKMRK_H__

#include "bkmrkbase.h"
#include "bkmrkproperties.h"

/**
Internal representation of a bookmark. It contains the bookmark specific properties and holds
a pointer to an extended properties object. This object is only loaded when needed.
*/
class CBookmark: public CBookmarkBase
	{
public:
	static CBookmark* NewL(CBookmarkFolder& aParent, CBookmarkDb& aDb);
	~CBookmark();
	void DeleteL();

	// From CBookmarkBase
	Bookmark::TType Type() const;

	RBkBookmark OpenBookmark();

	TBool IsHomePage();

	const TTime& LastVisited() const;
	void SetLastVisited(const TTime& aTime);
	void UpdateVisited();

	const TDesC8& Uri() const;
	void SetUriL(const TDesC8& aUri);

	CBkmrkExtendedProperties& BkmrkExtendedPropertiesL();

	// From MRepositoryAccessor
	TUint32 IndexBase();
	void SetIdFromIndexBase(TUint32 aIndexBase);
	void TransSaveL();
	void TransNewL();
	void TransLoadL();
	void TransRemoveL();
	
private:
	CBookmark(CBookmarkDb& aDb);
	
	void ConstructL(CBookmarkFolder& aParent);

private:
	TTime						iLastVisited;
	CUri8*						iUri;
	CBkmrkExtendedProperties*	iProperties;
	};

#endif //__BKMRK_H__
