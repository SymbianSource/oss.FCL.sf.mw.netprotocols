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

#ifndef __CPIPELININGTESTCLIENT_H__
#define __CPIPELININGTESTCLIENT_H__

#include <e32base.h>
#include <http.h>

class CHTTPTestUtils;
class MPipeliningTestCase;
class MPipeliningTestObserver;

class MTimerClient1
	{
public:
	virtual void TimeOut() = 0;		
	};

class CHttpTimer1: public CActive
	{
	public:
		CHttpTimer1(MTimerClient1& aClient);
		~CHttpTimer1();
		void After(TTimeIntervalMicroSeconds32 anInterval);
		void DoCancel();
		void RunL();
	private:
		RTimer iTimer;
		MTimerClient1& iClient;
	};	

class CPipeliningTestClient : public CBase,
							  public MHTTPTransactionCallback, public MTimerClient1 
	{
public:
	static CPipeliningTestClient* NewL(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver);
	~CPipeliningTestClient();
	
	RHTTPSession& GetSession();
	void AddTransactionL(RHTTPTransaction aTransaction);
	void StartClientL();
	void SetTestCase(MPipeliningTestCase* aTestCase);

private:
	CPipeliningTestClient(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver);
	void ConstructL();
	
	// From MHTTPTransactionCallback
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	
	void EndTestL(TInt aErrorCode);
	void TimeOut();	
	
private:
	CHTTPTestUtils&				iTestUtils;
	MPipeliningTestObserver&	iObserver;
	RHTTPSession				iSession;
	RArray<RHTTPTransaction>	iTransArray;
	MPipeliningTestCase*		iTestCase;		// Not owned
	TInt						iCurrentTrans;
	TInt						iLastError;
	CHttpTimer1*				iHttpTimer;
	CActiveSchedulerWait*		iASW;
	};
	
#endif // __CPIPELININGTESTCLIENT_H__
