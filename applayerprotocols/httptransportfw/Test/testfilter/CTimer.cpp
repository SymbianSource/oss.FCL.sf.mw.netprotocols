// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "Timer.h"


CTimerOut* CTimerOut::NewL(MTimerObserver& aObserver)
{
	
	CTimerOut* iSelf = new (ELeave) CTimerOut(aObserver);
	CleanupStack::PushL(iSelf);
	iSelf->ConstructL();
	CleanupStack::Pop();
	
	return iSelf;
}


CTimerOut::CTimerOut(MTimerObserver& aObserver) : CActive(EPriorityStandard), iObserver(aObserver)
{
	
}


void CTimerOut::ConstructL()
{
	User::LeaveIfError(iTimer.CreateLocal());
	CActiveScheduler::Add(this);
	
}


CTimerOut::~CTimerOut()
{
	Deque();
	iTimer.Close();
	
}


void CTimerOut::Start(TTimeIntervalMicroSeconds32 aInterval)
{
	
	iTimer.After(iStatus,aInterval);
	SetActive();

}


void CTimerOut::Stop()
{

	Cancel();

}


void CTimerOut::DoCancel()
{
	
	iTimer.Cancel();
	
}


void CTimerOut::RunL()
{
	if(iStatus == KErrNone)
		iObserver.HandleTimerEventL(ETimerExpired);
	else
		iObserver.HandleTimerEventL(ETimerCancled);

}

