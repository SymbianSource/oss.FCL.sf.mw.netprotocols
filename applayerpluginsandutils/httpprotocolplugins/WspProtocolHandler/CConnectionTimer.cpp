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

#include "CConnectionTimer.h"

#include "MConnectionTimerCallback.h"

CConnectionTimer* CConnectionTimer::NewL(MConnectionTimerCallback& aCallback)
	{
	CConnectionTimer* self = new (ELeave) CConnectionTimer(aCallback);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CConnectionTimer::~CConnectionTimer()
	{
	}

CConnectionTimer::CConnectionTimer(MConnectionTimerCallback& aCallback)
: CTimer(EPriorityHigh), iCallback(aCallback)
	{
	CActiveScheduler::Add(this);
	}

void CConnectionTimer::ConstructL()
	{
	CTimer::ConstructL();
	}

void CConnectionTimer::Start(TInt aDelayTime)
	{
	if( aDelayTime > 0 )
		{
		// No delay timeout if delay time is zero
		CTimer::After(aDelayTime);
		}
	}

void CConnectionTimer::RunL()
	{
	// Timer has completed - tell the callback object
	iCallback.HandleConnectionTimedOut();
	}

TInt CConnectionTimer::RunError(TInt /*aError*/)
	{
	// The RunL() cannot leave - nothing to do here.
	return KErrNone;
	}
