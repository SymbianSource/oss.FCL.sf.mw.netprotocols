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
// $Header$
// This source file implements the basic event management control (active
// objects) for our domain command supprt.
// based loosely on the WSP version...
// Note: you must regenerate from this for the domain you require, providing the
// appropriate callbacks - this way reuse and oo is maintained.
// 
//

#include "CEventDispatcher.h"
#include "CTimeoutTimer.h"
#include "MHTTPEventCallback.h"

//-----------------------------------------------------------------------------

const TInt KSecondsToMicroSecondsFactor	= 1000000;

//-----------------------------------------------------------------------------

CEventDispatcher *CEventDispatcher::NewL(MHTTPEventCallback& aCallback)
{
CEventDispatcher *self = new (ELeave) CEventDispatcher(aCallback);
CleanupStack::PushL(self);
self->ConstructL();
CleanupStack::Pop(self);
return self;
}

//-----------------------------------------------------------------------------

CEventDispatcher::~CEventDispatcher()
{
Cancel();

delete iTimer;
}

//-----------------------------------------------------------------------------

CEventDispatcher::CEventDispatcher(MHTTPEventCallback &aCallback)
	: 
	CActive(EPriorityStandard), 
	iCallback(aCallback),
	iSession(NULL),
	iTransaction(NULL)
{
CActiveScheduler::Add(this);
}

//-----------------------------------------------------------------------------

void CEventDispatcher::ConstructL()
{
iTimer = CTimeoutTimer::NewL(*this);
}

//-----------------------------------------------------------------------------

void CEventDispatcher::WaitForHTTPEvent(RHTTPSession *aSess, TInt aTimeoutValueSeconds)
{
iSession = aSess;

// Request the get event from the co session
//iTrans = NULL;
//iConn->GetEvent(iEvent, iTrans, iStatus);

// Set ourselves active
SetActive();

// Start the timeout timer
iTimer->After(aTimeoutValueSeconds * KSecondsToMicroSecondsFactor);
}

//-----------------------------------------------------------------------------

void CEventDispatcher::CancelWait()
{
Cancel();
}

//-----------------------------------------------------------------------------
//	Methods from CActive

void CEventDispatcher::RunL()
{
// Check to see if this event is for a transaction in the aborting state
if (iTransaction != NULL)
	{
	/*
	// Check the state
	TWspMethodState state;
	iTrans->GetState(state);

	if (state == EAborting && iEvent != EMethodAbortInd)
		{
		// Transaction is in Aborting state and this not the S-MethodAbort.ind
		// event - need to ignore this event. Ask the session for next event.
		iTrans = NULL;
		iConn->GetEvent(iEvent, iTrans, iStatus);

		// Set ourselves active
		SetActive();

		// NOTE - don't cancel timeout timer, nor start it again.
		return;
		}
	*/
	}

//	Record the iStatus
TRequestStatus status = iStatus;

//	Cancel the timer
iTimer->Cancel();

//	We are waiting for HTTP event	
iCallback.HandleHTTPEvent(iEvent, iSession, status);
}

//-----------------------------------------------------------------------------

void CEventDispatcher::DoCancel()
{
if (iTransaction)
	iTransaction->Cancel();
iTimer->Cancel();
}

//-----------------------------------------------------------------------------
//	Methods from MTimeoutCallback

void CEventDispatcher::Timeout()
{
// Cancel the get event request
Cancel();

// Inform the call back
iCallback.HandleHTTPEvent(iEvent, iSession, KRequestPending);
}

//-----------------------------------------------------------------------------
