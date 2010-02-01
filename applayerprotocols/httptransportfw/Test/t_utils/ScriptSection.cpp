// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include "TestScripts.h"

//
// CScriptSection
//

CScriptSection::CScriptSection()
	: CBase()
	{
	}

EXPORT_C CScriptSection* CScriptSection::NewLC(const TDesC& aSectionName, CScriptSection& aDefaults)
	{
	CScriptSection* self = new (ELeave) CScriptSection();
	CleanupStack::PushL(self);
	self->iDefaults = aDefaults.CopyLC();
	CleanupStack::Pop(self->iDefaults);
	self->ConstructL(aSectionName);
	return self;
	}

EXPORT_C CScriptSection* CScriptSection::NewLC(const TDesC& aSectionName)
	{
	CScriptSection* self = new (ELeave) CScriptSection();
	CleanupStack::PushL(self);
	self->ConstructL(aSectionName);
	return self;
	}

void CScriptSection::ConstructL(const TDesC& aSectionName)
	{
	iSectionName = aSectionName.AllocL();
	iItems = new(ELeave) CArrayPtrFlat<CScriptSectionItem>(1);
	}

EXPORT_C const TDesC& CScriptSection::ItemValue(const TDesC& aItem, const TDesC& aDefault)
	{
	CScriptSectionItem* sectionItem = Item(aItem);

	if (sectionItem != NULL)
		{
		TPtrC value(sectionItem->Value());

		if (iDefaults != NULL && (value.CompareF(KDefault1) == 0 || value.CompareF(KDefault2) == 0))
			{
			return iDefaults->ItemValue(aItem, aDefault);
			}
		else
			{
			return sectionItem->Value();
			}
		}
	else if (iDefaults != NULL)
		{
		return iDefaults->ItemValue(aItem, aDefault);
		}
	else
		{
		return aDefault;
		}
	}

EXPORT_C TInt CScriptSection::ItemValue(const TDesC& aItem, const TInt aDefault)
	{
	const TDesC& value = ItemValue(aItem, KNullDesC);

	if (value.Length() == 0)
		{
		if (iDefaults != NULL)
			return iDefaults->ItemValue(aItem, aDefault);
		else
			return aDefault;
		}

	TLex input(value);
	TInt ret = aDefault;
	TInt err = input.Val(ret);
	
	if (err)
		{
		if (iDefaults != NULL)
			return iDefaults->ItemValue(aItem, aDefault);
		else
			return aDefault;
		}

	return ret;
	}

EXPORT_C CScriptSectionItem& CScriptSection::AddItemL(const TDesC& aItem, const TDesC& aValue)
	{
	CScriptSectionItem* sectionItem = CScriptSectionItem::NewLC(*this, aItem, aValue);
	iItems->AppendL(sectionItem);
	CleanupStack::Pop(sectionItem);
	iLastItem = sectionItem;
	return *sectionItem;
	}

EXPORT_C void CScriptSection::AddItemIfNotExistL(const TDesC& aItem, const TDesC& aValue)
	{
	CScriptSectionItem* sectionItem = Item(aItem);

	if (sectionItem == NULL)
		{
		AddItemL(aItem, aValue);
		}
	}

EXPORT_C CScriptSectionItem& CScriptSection::ReplaceItemL(const TDesC& aItem, const TDesC& aValue)
	{
	DeleteItemsL(aItem);
	return AddItemL(aItem, aValue);
	}

EXPORT_C void CScriptSection::DeleteItemsL(const TDesC& aItem)
	{
	iLastItem = NULL;
	TInt count = iItems->Count();

	//Remove existing CScriptSectionItems that have the same item name
	while (count--)
		{
		CScriptSectionItem* sectionItem = iItems->At(count);

		if (sectionItem->Item().CompareF(aItem) == 0)
			{
			delete sectionItem;
			iItems->Delete(count);
			}
		}	
	}


EXPORT_C CScriptSection::~CScriptSection()
	{
	if (iItems)
		iItems->ResetAndDestroy();

	delete iItems;
	delete iSectionName;
	delete iDefaults;
	}

EXPORT_C CScriptSection* CScriptSection::CopyLC()
	{
	TInt count = iItems->Count();
	CScriptSection* copy = NULL;

	if (iDefaults != NULL)
		copy = CScriptSection::NewLC(SectionName(), *iDefaults);
	else
		copy = CScriptSection::NewLC(SectionName());
		
	while (count--)
		{
		copy->iItems->AppendL(Item(count).CopyLC());
		CleanupStack::Pop();
		}

	return copy;
	}
