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
// TEngine.cpp
// 
//

#include "CTimeoutTimer.h"

//-----------------------------------------------------------------------------

CTimeoutTimer* CTimeoutTimer::NewL(MTimeoutCallback& aCallback)
{
CTimeoutTimer* self = new (ELeave) CTimeoutTimer(aCallback);
CleanupStack::PushL(self);
self->ConstructL();
CleanupStack::Pop(self);
return self;
}

//-----------------------------------------------------------------------------

CTimeoutTimer::~CTimeoutTimer()
{
}

//-----------------------------------------------------------------------------

CTimeoutTimer::CTimeoutTimer(MTimeoutCallback& aCallback)
	: 
	CTimer(EPriorityHigh), 
	iCallback(aCallback)
{
CActiveScheduler::Add(this);
}

//-----------------------------------------------------------------------------

void CTimeoutTimer::ConstructL()
{
CTimer::ConstructL();
}

//-----------------------------------------------------------------------------

void CTimeoutTimer::RunL()
{
iCallback.Timeout();
}

//-----------------------------------------------------------------------------
//	End of File
//-----------------------------------------------------------------------------
