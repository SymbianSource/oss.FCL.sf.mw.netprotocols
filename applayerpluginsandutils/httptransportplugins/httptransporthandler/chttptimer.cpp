// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "chttptimer.h"
#include "mhttptimerobserver.h"


CHttpTimer* CHttpTimer::NewL(MHttpTimerObserver& aTimerObserver)
	{
	return new (ELeave) CHttpTimer(aTimerObserver);
	}


CHttpTimer::CHttpTimer(MHttpTimerObserver& aTimerObserver)
:CActive(EPriorityStandard),iTimerObserver(aTimerObserver)
	{
	CActiveScheduler::Add(this);
	iTimer.CreateLocal();
	}

CHttpTimer::~CHttpTimer()
	{
	Cancel();
	iTimer.Close();	
	}
	
void CHttpTimer::After(TTimeIntervalMicroSeconds32 anInterval)
	{
	if(!IsActive())
		{
		iTimer.After(iStatus, anInterval);	
		SetActive();
		}
	}

void CHttpTimer::DoCancel()
	{
	iTimer.Cancel();
	}

void CHttpTimer::RunL()
	{
	iTimerObserver.TimeOut();
	}


