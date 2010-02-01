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
// Class for managing custom property lists
// @internalComponent
// 
//

#ifndef __PROPERTYLIST_H__
#define __PROPERTYLIST_H__

#include "propertyreg.h"

/**
The database holds a set of registered custom properties. Each folder, bookmark and the database itself
has a list of custom property values. This class represents one of these lists of values. 
*/
class CPropertyList: public CRepositoryAccessor
	{
public:
	static CPropertyList* NewL(CRepository& aRepository, CPropertyReg& aPropertyRegister, CPropertyReg::TPropertyGroup aGroup, TUint32 aIndexBase);
	~CPropertyList();

	void Reset();
	
	TInt GetCustomProperty(TUid aPropertyId, TInt& aValue) const;
	TInt GetCustomProperty(TUid aPropertyId, TReal& aValue) const;
	TInt GetCustomProperty(TUid aPropertyId, TDes& aValue) const;
	TInt GetCustomProperty(TUid aPropertyId, TDes8& aValue) const;
	void SetCustomPropertyL(TUid aPropertyId, TInt aValue);
	void SetCustomPropertyL(TUid aPropertyId, TReal aValue);
	void SetCustomPropertyL(TUid aPropertyId, const TDesC& aValue);
	void SetCustomPropertyL(TUid aPropertyId, const TDesC8& aValue);

	// from CRepositoryAccessor
	TUint32 IndexBase();
	void SetIdFromIndexBase(TUint32 aIndexBase);
	void TransSaveL();
	void TransNewL();
	void TransLoadL();
	void TransRemoveL();

private:
	CPropertyList(CRepository& aRepository, CPropertyReg& aPropertyRegister, CPropertyReg::TPropertyGroup aGroup, TUint32 aIndexBase);
	void ConstructL();

	TInt PropertyIndex(TUid aPropertyId, Bookmark::TPropertyType aType)  const;
	void UpdateValueListItemL(TInt aIndex, Bookmark::TPropertyType aType, const TAny* aValue);

private:
	CPropertyReg&					iPropertyRegister;
	CPropertyReg::TPropertyGroup	iGroup;
	TUint32 						iIndexBase;
	TCustomPropertyList				iPropertyValues;
	};

#endif //__PROPERTYLIST_H__
