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

#include <e32base.h>


#ifndef CTIMER_H
#define CTIMER_H

    
enum TTimerEventType
	{
	ETimerExpired,
	ETimerCancled
	};
 
class MTimerObserver
	{
	
	public :
		virtual TBool HandleTimerEventL(TTimerEventType aEventType)=0;
		
	};


class CTimerOut : public CActive
	{
	public:
	    static CTimerOut* NewL(MTimerObserver& aObserver);
    	~CTimerOut();
	    void Start(TTimeIntervalMicroSeconds32 aInterval);
	    void Stop();

	protected:
		CTimerOut(MTimerObserver& aObserver);
		void ConstructL();
	    void DoCancel();
    	void RunL();
	private:
		RTimer			 	iTimer;
 		MTimerObserver& 	iObserver;

	};

#endif
