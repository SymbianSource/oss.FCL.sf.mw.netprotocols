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

#ifndef __CHTTPTIMER_H__
#define __CHTTPTIMER_H__

#include <e32base.h>

const TInt KMicrovalue = 1000000;
const TInt KDefTimeoutValue = 60000000;
const TInt KMinTimeoutValue = 60;	//In Seconds.

class MHttpTimerObserver;

class CHttpTimer: public CActive
	{
	public:
		static CHttpTimer* NewL(MHttpTimerObserver& aTimerObserver);
		~CHttpTimer();
		void After(TTimeIntervalMicroSeconds32 anInterval);
		void DoCancel();
		void RunL();
	private:
		CHttpTimer(MHttpTimerObserver& aTimerObserver);
		
		RTimer iTimer;
		MHttpTimerObserver&			iTimerObserver;
	};


#endif /*__CHTTPTIMER_H__*/
