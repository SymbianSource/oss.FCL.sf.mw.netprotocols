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


// Local includes
#include "cnwsstranslookuptable.h"
#include "mnwsssessioneventhandler.h"
#include "mnwsstransactioneventhandler.h"
#include "mnwssoomhandler.h"
#include "tnwsswsptrhndpanic.h"
#include "testoom.h"

// Class signature
#include "cnwsswspcoeventdispatcher.h"

	

CNwssWspCOEventDispatcher* CNwssWspCOEventDispatcher::NewL(
											RWSPCOConn& aWspSession,
											MNwssSessionEventHandler& aSessEventHnd,
											MNwssTransactionEventHandler& aTransEventHnd,
										    MNwssOomHandler& aOutOfMemoryHnd
											)
	{
	return new(ELeave)CNwssWspCOEventDispatcher(aWspSession, aSessEventHnd,
												aTransEventHnd, aOutOfMemoryHnd);
	}

CNwssWspCOEventDispatcher::~CNwssWspCOEventDispatcher()
	{
	Cancel();
	}

CNwssWspCOEventDispatcher::CNwssWspCOEventDispatcher(
										RWSPCOConn& aWspSession,
		 								MNwssSessionEventHandler& aSessEventHnd,
										MNwssTransactionEventHandler& aTransEventHnd,
										MNwssOomHandler& aOutOfMemoryHnd
										)
	: CActive(CActive::EPriorityHigh),
	  iWspSession(aWspSession), iSessEventHnd(aSessEventHnd),
	  iTransEventHnd(aTransEventHnd), iOutOfMemoryHnd(aOutOfMemoryHnd)
	{
	CActiveScheduler::Add(this);
	}

void CNwssWspCOEventDispatcher::Start()
	{
	if (!IsActive())
		{
		iWspSession.GetEvent(iWspEvent, iTransaction, iStatus);
		SetActive();
		}
	}

void CNwssWspCOEventDispatcher::RunL()
	{
	RWSPCOConn::TEventType event = (RWSPCOConn::TEventType)iWspEvent();

	// Obtain the transaction ID for transaction events
	iTrId = KErrNotFound;
	if ( (event == RWSPCOConn::EMethodInvoke_cnf_t) ||
		 (event == RWSPCOConn::EMethodResult_ind_t) ||
		 (event == RWSPCOConn::EAbort_ind_t) )
		{
		__TESTOOMD(stkErr, iTransaction.Id(iTrId));
		User::LeaveIfError(stkErr);
		}

	// Testing only - simulate a WAP Stack OOM which would cause this AO to complete with KErrNoMemory
	TInt status = iStatus.Int();
#if defined (_DEBUG) && defined(__UNIT_TESTING__)
	TAny* _a = User::Alloc(sizeof(TInt));
	User::Free(_a);
	if (_a == NULL)
		status = KErrNoMemory;
#endif
	User::LeaveIfError(status);

	// Request the next event before processing this one.  It is important to do this here,
	// and not after the following event handling - since the handler may decide to cancel this
	// dispatcher as a result of the event, or even close iWspSession.
	Start();

	// Act according to the event type
	switch (event)
		{
		// dispatch session events to the session event handler
		case RWSPCOConn::EDisconnect_ind_s:
			{
			__DEBUGTESTLEAVE
			iSessEventHnd.HandleDisconnectIndL();
			} break;
		case RWSPCOConn::ESuspend_ind_s:
			{
			__DEBUGTESTLEAVE
			iSessEventHnd.HandleSuspendIndL();
			} break;
		case RWSPCOConn::EResume_cnf_s:
			{
			__DEBUGTESTLEAVE
			iSessEventHnd.HandleResumeCnfL();
			} break;
		case RWSPCOConn::EConnect_cnf_s:
			{
			__DEBUGTESTLEAVE
			iSessEventHnd.HandleConnectCnfL();
			} break;
		case RWSPCOConn::ERedirect_ind_s:
			{
			__DEBUGTESTLEAVE
			iSessEventHnd.HandleRedirectIndL();
			} break;
		// dispatch transaction events to the transaction event handler
		case RWSPCOConn::EMethodInvoke_cnf_t:
			{
			__DEBUGTESTLEAVE
			iTransEventHnd.HandleMethodInvokeCnfL(iTrId);
			} break;
		case RWSPCOConn::EMethodResult_ind_t:
			{
			__DEBUGTESTLEAVE
			iTransEventHnd.HandleMethodResultIndL(iTrId);
			} break;
		case RWSPCOConn::EAbort_ind_t:
			{
			__DEBUGTESTLEAVE
			iTransEventHnd.HandleAbortIndL(iTrId);
			} break;
		// push events are not supported, and should indeed never arise if the
		// capability negotiations with the WAP proxy have been successful
		case RWSPCOConn::EPush_ind_t:
		case RWSPCOConn::EConfirmedPush_ind_t:
			{
			TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ECOPushNotSupported);
			} break;
		// exception event is treated like a session event
		case RWSPCOConn::EException_ind_e:
			{
			__DEBUGTESTLEAVE
			iSessEventHnd.HandleExceptionIndL();
			} break;
		default: 
			{
			TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EUnknownWspEvent);
			} break;
		};
	}

TInt CNwssWspCOEventDispatcher::RunError(TInt aError)
	{
	// Handle errors according to event type: if it's a transaction event, then send an AbortInd
	// for it. If it's a session event, then send DisconnectInd.
	RWSPCOConn::TEventType event = (RWSPCOConn::TEventType)iWspEvent();
	TInt err = KErrNone;

	// Act according to the event type
	switch (event)
		{
		case RWSPCOConn::ESuspend_ind_s:
		case RWSPCOConn::EResume_cnf_s:
		case RWSPCOConn::EConnect_cnf_s:
		case RWSPCOConn::ERedirect_ind_s:
		case RWSPCOConn::EDisconnect_ind_s:
			{
			if (aError != KErrNoMemory)
				TRAP(err, __DEBUGTESTLEAVE \
						  iSessEventHnd.HandleDisconnectIndL());
			if ((aError == KErrNoMemory) || (err == KErrNoMemory))
				{
				// We need to disconnect the WAP stack if there is any risk of
				// the client and WAP stack's session state becoming different
				TBool disconStack = ( (event != RWSPCOConn::EDisconnect_ind_s) &&
									  (event != RWSPCOConn::ERedirect_ind_s) );
				iOutOfMemoryHnd.SendOomDisconnect(disconStack);
				}
			} break;
		case RWSPCOConn::EMethodInvoke_cnf_t:
		case RWSPCOConn::EMethodResult_ind_t:
		case RWSPCOConn::EAbort_ind_t:
			{
			if (iTrId != KErrNotFound) // if we don't have a transaction ID we can't do anything
				{
				if (aError != KErrNoMemory)
					TRAP(err, __DEBUGTESTLEAVE \
							  iTransEventHnd.HandleAbortIndL(iTrId));
				if ((aError == KErrNoMemory) || (err == KErrNoMemory))
					iOutOfMemoryHnd.SendOomMethodAbort(iTrId, EFalse);
				}
			else
				{
				// We had an error whilst processing a transaction event, but we couldn't obtain
				// the transaction ID - so we'll have to disconnect the session instead.
				iOutOfMemoryHnd.SendOomDisconnect(ETrue);
				}
			} break;
		default:
			; // ignore - nothing else we can do
		};
	return KErrNone;
	}

void CNwssWspCOEventDispatcher::DoCancel()
	{
	iWspSession.CancelGetEvent();
	}
