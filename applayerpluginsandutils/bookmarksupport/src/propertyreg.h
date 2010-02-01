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
// Classes for registering custom properties.
// @internalComponent
// 
//

#ifndef __PROPERTYREG_H__
#define __PROPERTYREG_H__

#include "customproperty.h"

typedef RArray<TAny*> TCustomPropertyList;

/**
The client can register custom properties for bookmark items and the database. There are three sets of 
registered properties: database properties, folder properties and bookmark properties. This class is 
responsible for handling the registration and checking the registration of these properties.
*/
class CPropertyReg: public CBase
	{
public:
	enum TPropertyGroup
		{
		EGroupDatabase,
		EGroupFolder,
		EGroupBookmark
		};
public:
	static CPropertyReg* NewL();
	~CPropertyReg();
	
	void RefreshL();
	void CommitL();
	
	void RegisterPropertyL(TPropertyGroup aGroup, TUid aCustomId, Bookmark::TPropertyType aDataType);
	TInt DeregisterPropertyL(TPropertyGroup aGroup, TUid aCustomId);
	TInt PropertyIndex(TPropertyGroup aGroup, TUid aPropertyId, Bookmark::TPropertyType aType);
	TInt GetPropertyType(TPropertyGroup aGroup, TInt aPropertyIndex, Bookmark::TPropertyType& aType);
	
private:
	CPropertyReg();
	void ConstructL();
	void Reset();
	
	TUint32 AssignIdL(TPropertyGroup aGroup);

	TInt FindProperty(TPropertyList& aList, TUid aPropertyId);
	
	void UpdateValueListItemL(TCustomPropertyList& aValueList, TInt aIndex, Bookmark::TPropertyType aType, const TAny* aValue);
	
	TPropertyList& List(TPropertyGroup aGroup);
	
	void LoadPropertyListL(TPropertyList& aList, TUint32 aPropertyStart);
	void LoadPropertiesL();
	
private:
	CRepository*	iDatabaseRepository;
	
	TPropertyList	iDatabaseProperties;
	TPropertyList	iFolderProperties;
	TPropertyList	iBookmarkProperties;
	
	TBkmrkAccessorList iDeletedList;
	};

#endif //__PROPERTYREG_H__
