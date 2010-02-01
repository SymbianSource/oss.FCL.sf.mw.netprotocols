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

#include "propertylist.h"
#include "repository.h"

CPropertyList* CPropertyList::NewL(CRepository& iRepository, CPropertyReg& aPropertyRegister, CPropertyReg::TPropertyGroup aGroup, TUint32 aIndexBase)
	{
	CPropertyList* self = new (ELeave) CPropertyList(iRepository, aPropertyRegister, aGroup, aIndexBase);
	return self;
	}

CPropertyList::CPropertyList(CRepository& iRepository, CPropertyReg& aPropertyRegister, CPropertyReg::TPropertyGroup aGroup, TUint32 aIndexBase)
	: CRepositoryAccessor(), iPropertyRegister(aPropertyRegister), iGroup(aGroup), iIndexBase(aIndexBase)
	{
	SetRepository(iRepository);
	}

CPropertyList::~CPropertyList()
	{
	Reset();
	}

void CPropertyList::Reset()
	{
	TInt index = iPropertyValues.Count() - 1;
	for (;index >= 0; --index)
		{
		delete iPropertyValues[index];
		}
	iPropertyValues.Reset();
	}
	
TInt CPropertyList::PropertyIndex(TUid aPropertyId, Bookmark::TPropertyType aType) const
	{
	TInt index = iPropertyRegister.PropertyIndex(iGroup, aPropertyId, aType);
	TInt count = iPropertyValues.Count();
	if (index == KErrNotFound || index >= count || !iPropertyValues[index])
		{
		index = KErrNotFound;
		}
		
	return index;
	}

TInt CPropertyList::GetCustomProperty(TUid aPropertyId, TInt& aValue) const
	{
	TInt index = PropertyIndex(aPropertyId, Bookmark::EDataTypeInt);
	if (index == KErrNotFound)
		{
		return KErrNotFound;
		}

	TInt* intPtr = static_cast<TInt*>(iPropertyValues[index]);
	aValue = *intPtr;
	return KErrNone;
	}

TInt CPropertyList::GetCustomProperty(TUid aPropertyId, TReal& aValue) const
	{
	TInt index = PropertyIndex(aPropertyId, Bookmark::EDataTypeReal);
	if (index == KErrNotFound)
		{
		return KErrNotFound;
		}

	TReal* realPtr = static_cast<TReal*>(iPropertyValues[index]);
	aValue = *realPtr;
	return KErrNone;
	}

TInt CPropertyList::GetCustomProperty(TUid aPropertyId, TDes& aValue) const
	{
	TInt index = PropertyIndex(aPropertyId, Bookmark::EDataTypeDes16);
	if (index == KErrNotFound)
		{
		return KErrNotFound;
		}

	TDes* descPtr = static_cast<TDes*>(iPropertyValues[index]);
	if (aValue.MaxLength() < descPtr->Length())
		{
		return KErrArgument;
		}
	aValue = *descPtr;
	return KErrNone;
	}
	
TInt CPropertyList::GetCustomProperty(TUid aPropertyId, TDes8& aValue) const
	{
	TInt index = PropertyIndex(aPropertyId, Bookmark::EDataTypeDes8);
	if (index == KErrNotFound)
		{
		return KErrNotFound;
		}

	TDes8* desc8Ptr = static_cast<TDes8*>(iPropertyValues[index]);
	if (aValue.MaxLength() < desc8Ptr->Length())
		{
		return KErrArgument;
		}
	aValue = *desc8Ptr;
	return KErrNone;
	}
	
void CPropertyList::SetCustomPropertyL(TUid aPropertyId, TInt aValue)
	{
	TInt index = iPropertyRegister.PropertyIndex(iGroup, aPropertyId, Bookmark::EDataTypeInt);
	if (index == KErrNotFound)
		{
		User::Leave(Bookmark::KErrNotRegistered);
		}
	UpdateValueListItemL(index, Bookmark::EDataTypeInt, &aValue);
	SetDirty();
	}

void CPropertyList::SetCustomPropertyL(TUid aPropertyId, TReal aValue)
	{
	TInt index = iPropertyRegister.PropertyIndex(iGroup, aPropertyId, Bookmark::EDataTypeReal);
	if (index == KErrNotFound)
		{
		User::Leave(Bookmark::KErrNotRegistered);
		}
	UpdateValueListItemL(index, Bookmark::EDataTypeReal, &aValue);
	SetDirty();
	}

void CPropertyList::SetCustomPropertyL(TUid aPropertyId, const TDesC& aValue)
	{
	// The aValue must be smaller that the maximim descriptor storage size
	__ASSERT_ALWAYS(aValue.Length() <= Bookmark::KMaxDescriptorLength, User::Panic(Bookmark::KBookmarkErrTooLong, KErrArgument));

	TInt index = iPropertyRegister.PropertyIndex(iGroup, aPropertyId, Bookmark::EDataTypeDes16);
	if (index == KErrNotFound)
		{
		User::Leave(Bookmark::KErrNotRegistered);
		}
	UpdateValueListItemL(index, Bookmark::EDataTypeDes16, &aValue);
	SetDirty();
	}

void CPropertyList::SetCustomPropertyL(TUid aPropertyId, const TDesC8& aValue)
	{
	// The aValue must be smaller that the maximim data storage size
	__ASSERT_ALWAYS(aValue.Length() <= Bookmark::KMaxDataLength, User::Panic(Bookmark::KBookmarkErrTooLong, KErrArgument));

	TInt index = iPropertyRegister.PropertyIndex(iGroup, aPropertyId, Bookmark::EDataTypeDes8);
	if (index == KErrNotFound)
		{
		User::Leave(Bookmark::KErrNotRegistered);
		}
	UpdateValueListItemL(index, Bookmark::EDataTypeDes8, &aValue);
	SetDirty();
	}

void CPropertyList::UpdateValueListItemL(TInt aIndex, Bookmark::TPropertyType aType, const TAny* aValue)
	{
	TInt lastIndex = iPropertyValues.Count() - 1;
	if (aIndex > lastIndex)
		{
		// create Null values in the property list.
		for (; lastIndex < aIndex; ++lastIndex)
			{
			iPropertyValues.AppendL(NULL);
			}
		}
		
	//Delete any previously allocated memory
	delete iPropertyValues[aIndex];
	iPropertyValues[aIndex] = NULL;	

	switch (aType)
		{
		case Bookmark::EDataTypeInt:
			{
			const TInt* intPtr = static_cast<const TInt*>(aValue);
			TInt* newInt = new (ELeave) TInt;
			*newInt = *intPtr;
			iPropertyValues[aIndex] = newInt;
			break;
			}
		case Bookmark::EDataTypeReal:
			{
			const TReal* realPtr = static_cast<const TReal*>(aValue);
			TReal* newReal = new (ELeave) TReal;
			*newReal = *realPtr;
			iPropertyValues[aIndex] = newReal;
			break;
			}
		case Bookmark::EDataTypeDes16:
			{
			const TDes* buffer16 = static_cast<const TDes*>(aValue);
			iPropertyValues[aIndex] = buffer16->AllocL();
			break;
			}
		case Bookmark::EDataTypeDes8:
			{
			const TDes8* buffer8 = static_cast<const TDes8*>(aValue);
			iPropertyValues[aIndex] = buffer8->AllocL();
			break;
			}
		default:
			// Should not get to here
			User::Leave(Bookmark::KErrCorrupt);
			break;
		}
	}

void CPropertyList::TransNewL()
	{
	TransSaveL();
	}

void CPropertyList::TransSaveL()
	{
	RArray<TUint32> entryList;
	TBool creating;
	TInt* intPtr = NULL;
	TReal* realPtr = NULL;
	TDesC* desc16Ptr = NULL;
	TDesC8* desc8Ptr = NULL;
	TInt index;
	Bookmark::TPropertyType type;
	
	for (index = iPropertyValues.Count() - 1; index >= 0; --index)
		{
		if (iPropertyValues[index])
			{
			if (iPropertyRegister.GetPropertyType(iGroup, index, type) != KErrNone)
				{
				User::Leave(Bookmark::KErrNotRegistered);
				}
				
			// if the index does not yet exist in the database we need to create it.
			creating = EFalse;
			if (KErrNotFound == iRepository->FindL(iIndexBase + index, KRepExactMask, entryList))
				{
				creating = ETrue;
				}
			entryList.Reset();
				
			switch (type)
				{
			case Bookmark::EDataTypeInt:
				intPtr = static_cast<TInt*>(iPropertyValues[index]);
				if (creating)
					{
					iRepository->Create(iIndexBase + index, *intPtr);
					}
				else
					{
					iRepository->Set(iIndexBase + index, *intPtr);
					}
				break;
			case Bookmark::EDataTypeReal:
				realPtr = static_cast<TReal*>(iPropertyValues[index]);
				if (creating)
					{
					iRepository->Create(iIndexBase + index, *realPtr);
					}
				else
					{
					iRepository->Set(iIndexBase + index, *realPtr);
					}
				break;
			case Bookmark::EDataTypeDes16:
				desc16Ptr = static_cast<TDes*>(iPropertyValues[index]);
				if (creating)
					{
					iRepository->Create(iIndexBase + index, *desc16Ptr);
					}
				else
					{
					iRepository->Set(iIndexBase + index, *desc16Ptr);
					}
				break;
			case Bookmark::EDataTypeDes8:
				desc8Ptr = static_cast<TDes8*>(iPropertyValues[index]);
				if (creating)
					{
					iRepository->Create(iIndexBase + index, *desc8Ptr);
					}
				else
					{
					iRepository->Set(iIndexBase + index, *desc8Ptr);
					}
				break;
			default:
				// Should not get to here
				User::Leave(Bookmark::KErrCorrupt);
				break;
				}
			}
		}
	}

void CPropertyList::TransLoadL()
	{
	RArray<TUint32> entryList;
	CleanupClosePushL ( entryList );
	iRepository->FindL(iIndexBase, KCusPropertyMask, entryList);

	TInt intValue = 0;
	TReal realValue = 0.0;
	HBufC* desc16Buffer = HBufC::NewLC(Bookmark::KMaxDescriptorLength);
	TPtr desc16Ptr = desc16Buffer->Des();
	HBufC8* desc8Buffer = HBufC8::NewLC(Bookmark::KMaxDataLength);
	TPtr8 desc8Ptr = desc8Buffer->Des();

	TInt valueIndex = 0;
	TInt index = 0;
	Bookmark::TPropertyType type;

	TInt count = entryList.Count();
	for (;index < count; ++index)
		{
		valueIndex = entryList[index] - iIndexBase;
		if (iPropertyRegister.GetPropertyType(iGroup, valueIndex, type) != KErrNone)
			{
			// remove redundant entries
			User::LeaveIfError(iRepository->Delete(entryList[index]));
			}
		else
			{
			switch (type)
				{
			case Bookmark::EDataTypeInt:
				User::LeaveIfError(iRepository->Get(entryList[index], intValue));
				UpdateValueListItemL(valueIndex, type, &intValue);
				break;
			case Bookmark::EDataTypeReal:
				User::LeaveIfError(iRepository->Get(entryList[index], realValue));
				UpdateValueListItemL(valueIndex, type, &realValue);
				break;
			case Bookmark::EDataTypeDes16:
				User::LeaveIfError(iRepository->Get(entryList[index], desc16Ptr));
				UpdateValueListItemL(valueIndex, type, &desc16Ptr);
				break;
			case Bookmark::EDataTypeDes8:
				User::LeaveIfError(iRepository->Get(entryList[index], desc8Ptr));
				UpdateValueListItemL(valueIndex, type, &desc8Ptr);
				break;
			default:
				// Should not get to here
				User::Leave(Bookmark::KErrCorrupt);
				break;
				}
			}
		}	
	
	CleanupStack::PopAndDestroy(desc8Buffer);
	CleanupStack::PopAndDestroy(desc16Buffer);
	CleanupStack::PopAndDestroy ( &entryList );
	
	SetClean();
	}
	
void CPropertyList::TransRemoveL()
	{
	TInt index = iPropertyValues.Count() - 1;
	for (;index >= 0; --index)
		{
		if (iPropertyValues[index])
			{
			iRepository->Delete(iIndexBase + index);
			}
		}
	}

TUint32 CPropertyList::IndexBase()
	{
	return iIndexBase;
	}

void CPropertyList::SetIdFromIndexBase(TUint32 aIndexBase)
	{
	iIndexBase = aIndexBase;
	}
