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

#include "propertyreg.h"

CPropertyReg* CPropertyReg::NewL()
	{
	CPropertyReg* self = new (ELeave) CPropertyReg();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CPropertyReg::CPropertyReg()
	{
	}

CPropertyReg::~CPropertyReg()
	{
	Reset();
	delete iDatabaseRepository;
	}
	
void CPropertyReg::Reset()
	{
	TInt index = iDatabaseProperties.Count() - 1;
	for (;index >= 0; --index)
		{
		delete iDatabaseProperties[index];
		}
	iDatabaseProperties.Reset();
	
	index = iFolderProperties.Count() - 1;
	for (;index >= 0; --index)
		{
		delete iFolderProperties[index];
		}
	iFolderProperties.Reset();
	
	index = iBookmarkProperties.Count() - 1;
	for (;index >= 0; --index)
		{
		delete iBookmarkProperties[index];
		}
	iBookmarkProperties.Reset();

	index = iDeletedList.Count() - 1;
	for (;index >= 0; --index)
		{
		delete iDeletedList[index];
		}
	iDeletedList.Reset();
	}
	
void CPropertyReg::ConstructL()
	{
	iDatabaseRepository = CRepository::NewL(KUidBookmarkDatabaseRepository);
	LoadPropertiesL();
	}
	
void CPropertyReg::RefreshL()
	{
	Reset();
	LoadPropertiesL();
	}
	
void CPropertyReg::CommitL()
	{
	TInt index = iDatabaseProperties.Count() - 1;
	for (;index >= 0; --index)
		{
		if (iDatabaseProperties[index])
			{
			iDatabaseProperties[index]->CommitL();
			}
		}
		
	index = iFolderProperties.Count() - 1;
	for (;index >= 0; --index)
		{
		if (iFolderProperties[index])
			{
			iFolderProperties[index]->CommitL();
			}
		}
		
	index = iBookmarkProperties.Count() - 1;
	for (;index >= 0; --index)
		{
		if (iBookmarkProperties[index])
			{
			iBookmarkProperties[index]->CommitL();
			}
		}
		
	index = iDeletedList.Count() - 1;
	for (;index >= 0; --index)
		{
		iDeletedList[index]->CommitL();
		delete iDeletedList[index];
		}
	iDeletedList.Reset();
	}

TUint32 CPropertyReg::AssignIdL(TPropertyGroup aGroup)
	{
	TUint32 nextEntryId = 0;
	switch(aGroup)
		{
	case EGroupDatabase:
		nextEntryId = KRepDbPropertyStart;
		break;
	case EGroupFolder:
		nextEntryId = KRepFldrPropertyStart;
		break;
	case EGroupBookmark:
		nextEntryId = KRepBkmrkPropertyStart;
		break;
	default:
		// not a TPropertyGroup value so should not get to here
		User::Invariant();	
		break;
		}
		
	TInt nextIndex = 0;
	User::LeaveIfError(iDatabaseRepository->Get(nextEntryId, nextIndex));
		if (nextIndex >= Bookmark::KMaxCustomProperties)
		{
		User::Leave(Bookmark::KErrNoMoreSpace);
		}
	
	// The repository index for this property will be the property type start position
	// plus 1 plus the newly assigned index value. The plus one is because the the start
	// position index holds the next index that can be assigned.
	TUint32 entryIndex = (nextEntryId + 1) + nextIndex;
	++nextIndex;
	User::LeaveIfError(iDatabaseRepository->Set(nextEntryId, nextIndex));
	
	return entryIndex;
	}

void CPropertyReg::RegisterPropertyL(TPropertyGroup aGroup, TUid aCustomId, Bookmark::TPropertyType aDataType)
	{
	TPropertyList& list = List(aGroup);
	if (FindProperty(list, aCustomId) != KErrNotFound)
		{
		User::Leave(Bookmark::KErrUidAlreadyUsed);
		}

	CCustomProperty* property = CCustomProperty::NewL(*iDatabaseRepository);
	CleanupStack::PushL(property);
	TUint32 newEntryId = AssignIdL(aGroup);		
	property->SetIdFromIndexBase(newEntryId);
	property->SetUid(aCustomId);
	property->SetType(aDataType);
	list.AppendL(property);
	CleanupStack::Pop(property);
	}

TInt CPropertyReg::DeregisterPropertyL(TPropertyGroup aGroup, TUid aCustomId)
	{
	TPropertyList& list = List(aGroup);
	TInt index = FindProperty(list, aCustomId);
	if (index == KErrNotFound)
		{
		return KErrNotFound;
		}
	
	list[index]->DeleteL();
	iDeletedList.AppendL(list[index]);
	list.Remove(index);
	
	return KErrNone;
	}

TInt CPropertyReg::PropertyIndex(TPropertyGroup aGroup, TUid aPropertyId, Bookmark::TPropertyType aType)
	{
	TPropertyList& list = List(aGroup);
	TInt index = FindProperty(list, aPropertyId);
	if (index != KErrNotFound)
		{
		if (list[index]->Type() != aType)
			{
			index = KErrNotFound;
			}
		}
	return index;
	}

TInt CPropertyReg::GetPropertyType(TPropertyGroup aGroup, TInt aPropertyIndex, Bookmark::TPropertyType& aType)
	{
	TInt error = KErrNone;
	TPropertyList& list = List(aGroup);
	if (aPropertyIndex >= list.Count() || !list[aPropertyIndex])
		{
		error = Bookmark::KErrNotRegistered;
		}
	else
		{
		aType = list[aPropertyIndex]->Type();
		}
		
	return error;
	}
	
TInt CPropertyReg::FindProperty(TPropertyList& aList, TUid aPropertyId)
	{
	TInt index = aList.Count() -1;
	for (;index >= 0; --index)
		{
		if (aList[index] && aList[index]->Uid() == aPropertyId)
			{
			break;
			}
		}
		
	return index;
	}
	
TPropertyList& CPropertyReg::List(TPropertyGroup aGroup)
	{
	TPropertyList* list = NULL;

	switch(aGroup)
		{
	case EGroupDatabase:
		list = &iDatabaseProperties;
		break;
	case EGroupFolder:
		list = &iFolderProperties;
		break;
	case EGroupBookmark:
		list = &iBookmarkProperties;
		break;
	default:
		// not a TPropertyGroup value so should not get to here
		User::Invariant();	
		break;
		}
		
	return *list;
	}


void CPropertyReg::LoadPropertyListL(TPropertyList& aList, TUint32 aPropertyStart)
	{
	RArray<TUint32> entryList;
	CleanupClosePushL(entryList);
	iDatabaseRepository->FindL(aPropertyStart, KRepPropertyMask, entryList);	
	
	TInt count = 0;
	iDatabaseRepository->Get(aPropertyStart, count);
	count++;
	
	CCustomProperty* newProperty;
	
	TInt listEntry = 1;
	for (TInt index = 1; index < count; ++index)
		{
		if (entryList.Count() > 1 && entryList[listEntry] == aPropertyStart + index)
			{
			newProperty = CCustomProperty::NewL(*iDatabaseRepository);
			CleanupStack::PushL(newProperty);
			newProperty->SetIdFromIndexBase(entryList[listEntry]);
			newProperty->TransactionL(CRepositoryAccessor::ETransLoad);
			aList.AppendL(newProperty);
			CleanupStack::Pop(newProperty);
			listEntry++;
			}
		else
			{
			aList.AppendL(NULL);
			}
		}
	CleanupStack::PopAndDestroy(&entryList);
	}


void CPropertyReg::LoadPropertiesL()
	{
	LoadPropertyListL(iDatabaseProperties, KRepDbPropertyStart);
	LoadPropertyListL(iFolderProperties, KRepFldrPropertyStart);
	LoadPropertyListL(iBookmarkProperties, KRepBkmrkPropertyStart);
	}


