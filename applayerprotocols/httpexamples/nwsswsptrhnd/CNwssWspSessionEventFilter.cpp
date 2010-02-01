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
#include "mnwsstransactioneventhandler.h"
#include "testoom.h"

#include "cnwsswspsessioneventfilter.h"

CNwssWspSessionEventFilter* CNwssWspSessionEventFilter::NewL(
															CNwssTransLookUpTable&			aTransTable,
															MNwssSessionEventHandler&		aSessionEventHandler,
															MNwssTransactionEventHandler&	aTransEventHandler
															)
	{
	return new (ELeave) CNwssWspSessionEventFilter(aTransTable, aSessionEventHandler, aTransEventHandler);
	}

CNwssWspSessionEventFilter::~CNwssWspSessionEventFilter()
	{
	}

CNwssWspSessionEventFilter::CNwssWspSessionEventFilter(
													  CNwssTransLookUpTable&		aTransTable,
													  MNwssSessionEventHandler&		aSessionEventHandler,
													  MNwssTransactionEventHandler&	aTransEventHandler
													  )
: CActive(CActive::EPriorityHigh), iTransTable(aTransTable), 
								   iSessionEventHandler(aSessionEventHandler),
								   iTransEventHandler(aTransEventHandler)
	{
	CActiveScheduler::Add(this);
	}

void CNwssWspSessionEventFilter::CompleteSelf()
	{
	if (!IsActive())
		{
		TRequestStatus* pStat = &iStatus;
		User::RequestComplete(pStat, KErrNone);
		SetActive();
		}
	}

/*
 * Methods from CActive
 */

void CNwssWspSessionEventFilter::RunL()
	{
	__ASSERT_DEBUG( !iTransTable.IsEmpty(), User::Invariant() );

	// Get the first entry in the table
	TBool found;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iTransTable.Head(found);

	__ASSERT_DEBUG( found, User::Invariant() );

	// Get the transaction ID for aborted method
	RWSPCOTrans::TTransID transId = lutEntry.iStackTransID;

	// Send the T-MethodAbort.ind event
	__DEBUGTESTLEAVE
	iTransEventHandler.HandleAbortIndL(transId);

	if( iTransTable.IsEmpty() )
		{
		// No more 'live' transactions - send S-Disconnect.ind
		__DEBUGTESTLEAVE
		iSessionEventHandler.HandleDisconnectIndL();
		}
	else
		{
		// More 'live' transactions - complete-self to deal with them.
		CompleteSelf();
		}
	}

TInt CNwssWspSessionEventFilter::RunError(TInt /*aError*/)
	{
	//@todo - what to do? Abort event left...
	return KErrNone;
	}

void CNwssWspSessionEventFilter::DoCancel()
	{
	}

/*
 * Methods from MNwssSessionEventHandler
 */

void CNwssWspSessionEventFilter::HandleDisconnectIndL()
	{
	// Have received the S-Disconnect.ind from the server. This means that there
	// will no more events sent from the server until the next S-Connect.req is 
	// sent. Are there any 'live' transactions?
	if( !iTransTable.IsEmpty() )
		{
		// There are 'live' transactions - this means that the T-MethodResult.ind
		// for these transactions was received and so the Nwss Stack treats them
		// as complete and therefore did not abort them when the S-Disconnet.req
		// was sent. Complete this object - RunL() will send the appropriate 
		// primitives.
		CompleteSelf();
		}
	else
		{
		// Pass event on.
		__DEBUGTESTLEAVE
		iSessionEventHandler.HandleDisconnectIndL();
		}
	}

void CNwssWspSessionEventFilter::HandleSuspendIndL()
	{
	// Pass event on.
	__DEBUGTESTLEAVE
	iSessionEventHandler.HandleSuspendIndL();
	}

void CNwssWspSessionEventFilter::HandleResumeCnfL()
	{
	// Pass event on.
	__DEBUGTESTLEAVE
	iSessionEventHandler.HandleResumeCnfL();
	}

void CNwssWspSessionEventFilter::HandleConnectCnfL()
	{
	// Pass event on.
	__DEBUGTESTLEAVE
	iSessionEventHandler.HandleConnectCnfL();
	}

void CNwssWspSessionEventFilter::HandleRedirectIndL()
	{
	// Pass event on.
	__DEBUGTESTLEAVE
	iSessionEventHandler.HandleRedirectIndL();
	}

void CNwssWspSessionEventFilter::HandleExceptionIndL()
	{
	// Pass event on.
	__DEBUGTESTLEAVE
	iSessionEventHandler.HandleExceptionIndL();
	}
