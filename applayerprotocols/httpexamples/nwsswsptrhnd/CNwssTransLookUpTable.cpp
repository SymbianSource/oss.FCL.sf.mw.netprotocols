// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// System includes
// 
//

// Local includes
#include "cnwsswsptrhnddatasupplier.h"
#include "tnwsswsptrhndpanic.h"

// Class signature
#include "cnwsstranslookuptable.h"



CNwssTransLookUpTable::CNwssTransLUTEntry* CNwssTransLookUpTable::CNwssTransLUTEntry::NewL()
	{
	return new(ELeave) CNwssTransLookUpTable::CNwssTransLUTEntry;
	}

CNwssTransLookUpTable::CNwssTransLUTEntry::CNwssTransLUTEntry()
	{
	}

CNwssTransLookUpTable::CNwssTransLUTEntry::~CNwssTransLUTEntry()
	{
	delete iResponseBodyHandler;
	}

CNwssTransLookUpTable* CNwssTransLookUpTable::NewL(TInt aTableSize)
	{
	CNwssTransLookUpTable* me = new(ELeave)CNwssTransLookUpTable();
	CleanupStack::PushL(me);
	me->ConstructL(aTableSize);
	CleanupStack::Pop(me);
	return me;
	}

CNwssTransLookUpTable::CNwssTransLookUpTable()
	{
	}

CNwssTransLookUpTable::~CNwssTransLookUpTable()
	{
	// Flush the look-up table.  Entries which are unused will delete OK since
	// when removed, their iResponseBodyHandler pointer is reset to NULL.
	iEntries.ResetAndDestroy();
	}

CNwssTransLookUpTable::CNwssTransLUTEntry& CNwssTransLookUpTable::NewEntry()
	{
	TInt numTableEntries = iEntries.Count();
#ifdef __UNIT_TESTING__
	if (numTableEntries == 0)
		// dummy return for testing only
		return *(REINTERPRET_CAST(CNwssTransLookUpTable::CNwssTransLUTEntry*, 1));
#endif
	__ASSERT_DEBUG(numTableEntries > 0, User::Invariant());

	CNwssTransLookUpTable::CNwssTransLUTEntry* retVal = NULL;
	if (iNumActiveLUTEntries < numTableEntries)
		retVal = iEntries[iNumActiveLUTEntries++];
#ifdef __UNIT_TESTING__
	else
		// dummy return for testing only
		return *(REINTERPRET_CAST(CNwssTransLookUpTable::CNwssTransLUTEntry*, 1));
#else
	else
		TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ETransLUTOverflow);
#endif
	return *retVal;
	}

CNwssTransLookUpTable::CNwssTransLUTEntry&
CNwssTransLookUpTable::LookUpByTransId(RWSPCOTrans::TTransID aTransId, TBool& aFound)
	{
	// Iterate the look-up-table until we find the entry whose transaction ID matches
	// the supplied value
	aFound = EFalse;
	TInt idx = 0;
	while (!aFound && (idx < iNumActiveLUTEntries))
		{
		if (iEntries[idx]->iStackTransID == aTransId)
			aFound = ETrue;
		else
			++idx;
		}
	if (aFound)
		return *(iEntries[idx]);

	// dummy return
	CNwssTransLookUpTable::CNwssTransLUTEntry* retVal = NULL;
	return *retVal;
	}

CNwssTransLookUpTable::CNwssTransLUTEntry&
CNwssTransLookUpTable::LookUpByCallback(MWspCOMethodCallback& aCallback, TBool& aFound)
	{
	// Locate the item in the table
	aFound = EFalse;
	TInt unused = KErrNotFound;
	CNwssTransLookUpTable::CNwssTransLUTEntry* lutEntry = LookUp(aCallback, unused);
	if (lutEntry)
		{
		aFound = ETrue;
		return *lutEntry;
		}

	// dummy return
	CNwssTransLookUpTable::CNwssTransLUTEntry* retVal = NULL;
	return *retVal;
	}

void CNwssTransLookUpTable::RemoveEntry(MWspCOMethodCallback& aCallback)
	{
	// Locate the item in the table
	TInt idx = KErrNotFound;
	CNwssTransLookUpTable::CNwssTransLUTEntry* lutEntry = LookUp(aCallback, idx);
	if (lutEntry)
		{
		iEntries.Remove(idx);
		--iNumActiveLUTEntries;

		// Reset the entry, so it can be recycled
		lutEntry->iCallback = NULL;
		delete lutEntry->iResponseBodyHandler;
		lutEntry->iResponseBodyHandler = NULL;
		lutEntry->iStackTrans = RWSPCOTrans();
		lutEntry->iStackTransID = 0;
		lutEntry->iAborted = EFalse;

		// Move the removed entry to the end of the table, to ensure the table remains
		// of constant size.  Note, this append will not leave, since the table size
		// was preallocated in ConstructL.
#if defined (_DEBUG) && defined(__UNIT_TESTING__)
		TInt err =
#endif
			iEntries.Append(lutEntry);
#ifdef __UNIT_TESTING__
		__ASSERT_DEBUG(err == KErrNone, 
					TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ETransLUTOverflow));
#endif
		}
	}

TBool CNwssTransLookUpTable::IsEmpty() const
	{
	return iNumActiveLUTEntries == 0;
	}

CNwssTransLookUpTable::CNwssTransLUTEntry& CNwssTransLookUpTable::Head(TBool& aFound)
	{
	// Are there any entries
	aFound = iNumActiveLUTEntries > 0;

	// Return the first entry
	if (aFound)
		return *(iEntries[0]);

	// dummy return
	CNwssTransLookUpTable::CNwssTransLUTEntry* retVal = NULL;
	return *retVal;
	}

void CNwssTransLookUpTable::ConstructL(TInt aTableSize)
	{
	CNwssTransLookUpTable::CNwssTransLUTEntry* entry;

	// Pre-allocate the look-up table
	for (TInt ii = 0; ii < aTableSize; ++ii)
		{
		entry = CNwssTransLookUpTable::CNwssTransLUTEntry::NewL();
		CleanupStack::PushL(entry);
		User::LeaveIfError(	iEntries.Append(entry) );
		CleanupStack::Pop(entry);
		}
	}

void CNwssTransLookUpTable::ResizeTableL(TInt aNewSize)
	{
	// Bail out now if the size doesn't change!
	const TInt oldSize = iEntries.Count();
	if (aNewSize == oldSize)
		return;
	CNwssTransLookUpTable::CNwssTransLUTEntry* entry = NULL;
	if (aNewSize > oldSize)
		{
		// Table is being enlarged - pre-allocate new entries
		for (TInt ii = 0; ii < (aNewSize - oldSize); ++ii)
			{
			entry = CNwssTransLookUpTable::CNwssTransLUTEntry::NewL();
			CleanupStack::PushL(entry);
			User::LeaveIfError(	iEntries.Append(entry) );
			CleanupStack::Pop(entry);
			}

		// High-water mark doesn't change in this situation
		}
	else
		{
		// Table is being shrunk - remove surplus entries
		for (TInt ii = oldSize - 1; ii >= aNewSize; --ii)
			{
			delete iEntries[ii];
			iEntries.Remove(ii);
			}

		// Adjust high-water mark
		if (iNumActiveLUTEntries > aNewSize)
			iNumActiveLUTEntries = aNewSize;
		}
	}

CNwssTransLookUpTable::CNwssTransLUTEntry*
CNwssTransLookUpTable::LookUp(MWspCOMethodCallback& aCallback, TInt& aIndex)
	{
	// Iterate the look-up-table until we find the entry whose transaction callback
	// matches the supplied value
	TBool found = EFalse;
	aIndex = 0;
	CNwssTransLookUpTable::CNwssTransLUTEntry* lutEntry = NULL;
	while (!found && (aIndex < iNumActiveLUTEntries))
		{
		lutEntry = iEntries[aIndex];
		if (lutEntry->iCallback == &aCallback)
			found = ETrue;
		else
			++aIndex;
		}
	if (!found)
		{
		aIndex = KErrNotFound;
		lutEntry = NULL;
		}
	return lutEntry;
	}
