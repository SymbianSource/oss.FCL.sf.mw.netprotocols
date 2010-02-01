// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ByteList.h"

//
// class CByteList - FIFO for bytes
//

CByteList* CByteList::NewLC()
{
	CByteList* self = CByteList::NewL();
	CleanupStack::PushL(self);
	return self;
}

CByteList* CByteList::NewL()
{
	CByteList* self = new (ELeave) CByteList;
	return self;
}

CByteList::CByteList()
	: iList(CItem::iOffset),iListIter(iList),iListCommit(iList)
{}

CByteList::~CByteList()
{
	CItem*  item;

	iListIter.SetToFirst(); 
	while ((item = iListIter++) != NULL)
	{
		iList.Remove(*item);
		delete item;
	};
}

void CByteList::AddToListL(HBufC8& aData)
{
	// NULL buffers accepted, but not stored
	if (&aData == NULL)
		return;

	TBool first = FALSE;
	if (iList.IsEmpty())
	{
		first = TRUE;
	}

	CItem* item = CItem::NewL(CONST_CAST(HBufC8&,aData));
	iList.AddLast(*item);
	
	if (first)
	{
		iListIter.SetToFirst();
		iByteOffset = 0;
		iListCommit.SetToFirst();
		iByteCommit = 0;
	}
}

TInt CByteList::Byte(TUint8* aByte, TBool aIncrement)
{
	if (iList.IsEmpty())
		return KErrNotFound;

	// We have data, but it's all used
	if (iListIter == NULL)
		return KErrNotFound;

	if (iByteOffset >= ((*iListIter).Data())->Length())
		return KErrNotFound;

	*aByte = (((*iListIter).Data())->Des())[iByteOffset];
	Inc(aIncrement ? 1 : 0);

	return KErrNone;
}

TInt CByteList::Inc(TInt aCount)
{
	// Note: Inc(0) is ok
	if (!aCount)
		return KErrNone;

	const HBufC8* data = (*iListIter).Data();
	for (TInt i=0; i<aCount; i++)
	{
		iByteOffset++;
		if (iByteOffset >= data->Length())
		{
			// Need next buffer
			iListIter++;
			if (iListIter == NULL)
			{
				// User should call RollBack()
				return KErrNotFound;
			}

			data = (*iListIter).Data();
			iByteOffset = 0;
		}
	}

	return KErrNone;
}

void CByteList::Commit()
{
	CItem* current = iListIter;
	iListIter.SetToFirst();
	CItem* item = iListIter;

	// Remove used buffers
	if (item != current && item != NULL)
	{
		// Note: Increment iListIter to keep it on the list during loop
		while ((item = iListIter++) != NULL)
		{
			if (item == current)
				break;

			iList.Remove(*item);
			delete item;
		};
		iListIter.SetToFirst();
		current = iListIter;
	}

	if (current != NULL)
	{
		// Is current buffer fully used
		if (iByteOffset >= (*iListIter).Data()->Length())
		{
			item = iListIter;
			iList.Remove(*item);
			delete item;
			iByteOffset = 0;
			// BUG: is iListIter NULL after Remove? Manual didn't say anything
		}
	}

	iListCommit = iListIter;
	iByteCommit = iByteOffset;
}

void CByteList::RollBack()
{
	iListIter = iListCommit;
	iByteOffset = iByteCommit;
}

//
// class CItem - to be used with CList
//

const TInt CItem::iOffset = _FOFF(CItem,iLink);

CItem* CItem::NewLC (const HBufC8& aData)
{
	CItem* self = new (ELeave) CItem();
	CleanupStack::PushL(self);
	self->ConstructL(aData);
	return self;
}

CItem* CItem::NewL(const HBufC8& aData)
{
	CItem* self = CItem::NewLC(aData);
	CleanupStack::Pop();
	return self;
}

void CItem::ConstructL(const HBufC8& aData)
{
	// NOTE: Make sure aData != NULL
	iData = HBufC8::NewL(aData.Length());
	(*iData) = aData;
}

CItem::CItem()
{}

CItem::~CItem()
{
	delete iData;
}

HBufC8* CItem::Data() const
{
	return (iData);
}

//
// End of file
//
