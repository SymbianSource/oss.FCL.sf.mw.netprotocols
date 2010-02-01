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

#include "customproperty.h"

CCustomProperty* CCustomProperty::NewL(CRepository& aRepository)
	{
	CCustomProperty* self = new (ELeave) CCustomProperty(aRepository);
	return self;
	}
	
CCustomProperty::CCustomProperty(CRepository& aRepository)
	: CRepositoryAccessor()
	{
	SetRepository(aRepository);
	}

CCustomProperty::~CCustomProperty()
	{
	}

TUint32 CCustomProperty::IndexBase()
	{
	return iIndexBase;
	}
	
void CCustomProperty::SetIdFromIndexBase(TUint32 aIndexBase)
	{
	iIndexBase = aIndexBase;
	iEntryId = aIndexBase & KRepIndexMask;
	}

TUid CCustomProperty::Uid()
	{
	return iProperty.iId;
	}
	
void CCustomProperty::SetUid(TUid aId)
	{
	iProperty.iId = aId;
	}

Bookmark::TPropertyType CCustomProperty::Type()
	{
	return iProperty.iDataType;
	}

void CCustomProperty::SetType(Bookmark::TPropertyType aType)
	{
	iProperty.iDataType = aType;
	}
	
void CCustomProperty::TransNewL()
	{
	TPckgBuf<TProperty> pckg(iProperty);
	// create entry in the repository
	iRepository->Create(iIndexBase, pckg);
	}
	
void CCustomProperty::TransSaveL()
	{
	TPckgBuf<TProperty> pckg(iProperty);
	iRepository->Set(iIndexBase, pckg);
	}

void CCustomProperty::TransLoadL()
	{
	TPckgBuf<TProperty> pckg;
	User::LeaveIfError(iRepository->Get(iIndexBase, pckg));
	iProperty = pckg();
	
	SetClean();
	}

void CCustomProperty::TransRemoveL()
	{
	iRepository->Delete(iIndexBase);
	}
	
