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
//

#include "tnwsswsptrhndpanic.h"
#include "cnwsstranslookuptable.h"
#include "testoom.h"

#include "cnwsswsptransactioneventfilter.h"

CNwssWspTransactionEventFilter* CNwssWspTransactionEventFilter::NewL(
																	CNwssTransLookUpTable&			aTransTable,
																	MNwssTransactionEventHandler&	aTransEventHandler,
																	TInt							aTableSize
																	)
	{
	CNwssWspTransactionEventFilter* self = new (ELeave) CNwssWspTransactionEventFilter(aTransTable, aTransEventHandler);
	CleanupStack::PushL(self);
	self->ConstructL(aTableSize);
	CleanupStack::Pop(self);
	return self;
	}

CNwssWspTransactionEventFilter::~CNwssWspTransactionEventFilter()
	{
	Cancel();
	iEntries.Reset();
	}

CNwssWspTransactionEventFilter::CNwssWspTransactionEventFilter(
															  CNwssTransLookUpTable&			aTransTable,
															  MNwssTransactionEventHandler&		aTransEventHandler
															  )
: CActive(CActive::EPriorityHigh), iTransTable(aTransTable), iTransEventHandler(aTransEventHandler)
	{
	CActiveScheduler::Add(this);
	}

void CNwssWspTransactionEventFilter::ConstructL(TInt aTableSize)
	{
	// Pre-allocate the look-up table
	for (TInt ii = 0; ii < aTableSize; ++ii)
		User::LeaveIfError(	iEntries.Append(RWSPCOTrans::TTransID()) );
	}

void CNwssWspTransactionEventFilter::SendMethodAbortIndEvent(RWSPCOTrans::TTransID aTransId)
	{
	// Find this transaction in the LUT
	TBool found = EFalse;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iTransTable.LookUpByTransId(aTransId, found);

	// Check to see that this hasn't already been aborted...
	if( found && !lutEntry.iAborted )
		{
		__ASSERT_ALWAYS( iHighWaterMark < iEntries.Count(), TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ETransLUTOverflow));

		// Mark as aborted
		lutEntry.iAborted = ETrue;

		// Add to the table of aborted transaction
		iEntries[iHighWaterMark++] = aTransId;

		// Complete-self
		CompleteSelf();
		}
	}

void CNwssWspTransactionEventFilter::SessionDisconnectRequested()
	{
	iSessionClosing = ETrue;
	}

void CNwssWspTransactionEventFilter::SessionDisconnected()
	{
	iSessionClosing = EFalse;
	}

void CNwssWspTransactionEventFilter::ResizeTableL(TInt aNewSize)
	{
	// Bail out now if the size doesn't change!
	const TInt oldSize = iEntries.Count();
	if (aNewSize == oldSize)
		return;
	if (aNewSize > oldSize)
		{
		// Table is being enlarged - pre-allocate new entries
		for (TInt ii = 0; ii < (aNewSize - oldSize); ++ii)
			{
			User::LeaveIfError(	iEntries.Append(RWSPCOTrans::TTransID()) );
			}

		// High-water mark doesn't change in this situation
		}
	else
		{
		// Table is being shrunk - remove surplus entries
		for (TInt ii = oldSize - 1; ii >= aNewSize; --ii)
			iEntries.Remove(ii);

		// Adjust high-water mark
		if (iHighWaterMark > aNewSize)
			iHighWaterMark = aNewSize;
		}
	}

void CNwssWspTransactionEventFilter::CompleteSelf()
	{
	if (!IsActive())
		{
		TRequestStatus* pStat = &iStatus;
		User::RequestComplete(pStat, KErrNone);
		SetActive();
		}
	}

/*
 * Methods from  Active
 */

void CNwssWspTransactionEventFilter::RunL()
	{
	__ASSERT_DEBUG( iHighWaterMark > 0, User::Invariant() );

	// Ok, get the transaction ID for aborted method
	RWSPCOTrans::TTransID transId = iEntries[--iHighWaterMark];

	// Anymore to send aborts to send?
	if( iHighWaterMark > 0 )
		{
		// Yep - complete self
		CompleteSelf();
		}
	// Send the T-MethodAbort.ind event
	__DEBUGTESTLEAVE
	iTransEventHandler.HandleAbortIndL(transId);
	}

TInt CNwssWspTransactionEventFilter::RunError(TInt /*aError*/)
	{
	//@todo - what to do? Abort event left...
	return KErrNone;
	}

void CNwssWspTransactionEventFilter::DoCancel()
	{
	}

/*
 * Methods from MNwssTransactionEventHandler
 */

void CNwssWspTransactionEventFilter::HandleAbortIndL(RWSPCOTrans::TTransID aTransId)
	{
	// Check to see if this transaction is aborted
	TBool found = EFalse;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iTransTable.LookUpByTransId(aTransId, found);

	if( found && !lutEntry.iAborted )
		{
		// Not aborted by client - pass the event on.
		__DEBUGTESTLEAVE
		iTransEventHandler.HandleAbortIndL(aTransId);
		}
	// Ignore event if the transaction has been aborted
	}

void CNwssWspTransactionEventFilter::HandleMethodInvokeCnfL(RWSPCOTrans::TTransID aTransId)
	{
	// Check to see if this transaction is aborted
	TBool found = EFalse;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iTransTable.LookUpByTransId(aTransId, found);

	if( found && !lutEntry.iAborted && !iSessionClosing )
		{
		// Not aborted by client and session not in Closing state - pass the event on.
		__DEBUGTESTLEAVE
		iTransEventHandler.HandleMethodInvokeCnfL(aTransId);
		}
	// Ignore event if the transaction has been aborted
	}

void CNwssWspTransactionEventFilter::HandleMethodResultIndL(RWSPCOTrans::TTransID aTransId)
	{
	// Check to see if this transaction is aborted
	TBool found = EFalse;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iTransTable.LookUpByTransId(aTransId, found);

	if( found && !lutEntry.iAborted && !iSessionClosing )
		{
		// Not aborted by client and session not in Closing state - pass the event on.
		__DEBUGTESTLEAVE
		iTransEventHandler.HandleMethodResultIndL(aTransId);
		}
	// Ignore event if the transaction has been aborted
	}

