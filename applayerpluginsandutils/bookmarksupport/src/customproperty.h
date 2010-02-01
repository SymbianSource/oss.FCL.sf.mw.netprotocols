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
// Class representing a single custom property.
// @internalComponent
// 
//

#ifndef __CUSTOMPROPERTY_H__
#define __CUSTOMPROPERTY_H__

#include <centralrepository.h>
#include "repository.h"

/**
This class represents registry entry for a custom property. Custom properties are registered centrally 
in the database. A registered custom property consists of a UID for identification and a data type.
*/
class CCustomProperty: public CRepositoryAccessor
	{
private:
	class TProperty
		{
	public:
		TUid iId;
		Bookmark::TPropertyType iDataType;
		};

public:
	static CCustomProperty* NewL(CRepository& aRepository);
	~CCustomProperty();

	TUid Uid();
	void SetUid(TUid aId);

	Bookmark::TPropertyType Type();
	void SetType(Bookmark::TPropertyType aType);

	// From CRepositoryAccessor
	TUint32 IndexBase();
	void SetIdFromIndexBase(TUint32 aIndexBase);
	void TransSaveL();
	void TransNewL();
	void TransLoadL();
	void TransRemoveL();

private:
	CCustomProperty(CRepository& aRepository);

private:
	TUint32		iEntryId;
	TUint32		iIndexBase;
	TProperty	iProperty;
	};

typedef RArray<CCustomProperty*> TPropertyList;

#endif //__CUSTOMPROPERTY_H__
