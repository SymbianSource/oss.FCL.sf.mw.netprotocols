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

class CFrmwrkSession;

#include "MWspEventCallback.h"
#include "CWspEventDispatcher.h"
#include "CTimeoutTimer.h"

const TInt KSecondsToMicroSecondsFactor	= 1000000;

CWspEventDispatcher*  CWspEventDispatcher::NewL(MWspEventCallback& aCallback)
	{
	CWspEventDispatcher* self = new (ELeave) CWspEventDispatcher(aCallback);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CWspEventDispatcher::~CWspEventDispatcher()
	{
	Cancel();

	delete iTimer;
	}

CWspEventDispatcher::CWspEventDispatcher(MWspEventCallback& aCallback)
: CActive(EPriorityStandard), iCallback(aCallback)
	{
	CActiveScheduler::Add(this);
	}

void CWspEventDispatcher::ConstructL()
	{
	iTimer = CTimeoutTimer::NewL(*this);
	}

void CWspEventDispatcher::WaitForWspEvent(TInt aTimeoutValueSeconds)
	{

	if (!IsActive())
		SetActive();

	// Start the timeout timer

	iTimer->After(aTimeoutValueSeconds*KSecondsToMicroSecondsFactor);
	}

void CWspEventDispatcher::WaitForWspEvent()
	{
	if (!IsActive())
		{
		iStatus = KRequestPending;
		SetActive();
		}
	}

void CWspEventDispatcher::CancelWait()
	{
	Cancel();
	}

/*
 * Methods from CActive
 */

void CWspEventDispatcher::RunL()
	{
	// Record the iStatus
	TRequestStatus status = iStatus;

	// Cancel the timer
	iTimer->Cancel();

	// Were waiting for WSP event
	iCallback.HandleWspEvent(status);
	}

void CWspEventDispatcher::DoCancel()
	{
	TRequestStatus* pStat = &iStatus;

	User::RequestComplete(pStat, KErrCancel);
	iTimer->Cancel();
	}

/*
 * Methods from MTimeoutCallback
 */

void CWspEventDispatcher::Timeout()
	{
	TRequestStatus status;
	status = KRequestPending;
	iCallback.HandleWspEvent(status);
	}

