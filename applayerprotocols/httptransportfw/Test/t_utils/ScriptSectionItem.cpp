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

CScriptSectionItem::CScriptSectionItem(CScriptSection& aParent)
: iParent(aParent)
	{
	}

void CScriptSectionItem::ConstructL(const TDesC& aItem, const TDesC& aValue)
	{
	iItem = aItem.AllocL();
	iValue = aValue.AllocL();
	}

EXPORT_C CScriptSectionItem::~CScriptSectionItem()
	{
	delete iItem;
	delete iValue;
	}

EXPORT_C CScriptSectionItem* CScriptSection::Item(const TDesC& aItem)
	{
	CScriptSectionItem* sectionItem = iLastItem;

	if (!iLastItem || iLastItem->Item().CompareF(aItem) != 0)
		{
		const TInt count = iItems->Count();

		for (TInt i = 0; i < count; i++)
			{
			sectionItem = iItems->At(i);
			iLastItem = sectionItem;

			if (sectionItem->Item().CompareF(aItem) == 0)
				{
				break;
				}
			else
				{
				sectionItem = NULL;
				}
			}
		}

	return sectionItem;
	}

EXPORT_C CScriptSectionItem* CScriptSectionItem::NewLC(CScriptSection& aParent, const TDesC& aItem, const TDesC& aValue)
	{
	CScriptSectionItem* self = new (ELeave) CScriptSectionItem(aParent);
	CleanupStack::PushL(self);
	self->ConstructL(aItem, aValue);
	return self;
	}
