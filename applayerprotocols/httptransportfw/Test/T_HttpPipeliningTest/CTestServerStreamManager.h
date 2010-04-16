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

#ifndef __CTESTSERVERSTREAMMANAGER_H__
#define __CTESTSERVERSTREAMMANAGER_H__

#include <e32base.h>
#include <minputstreamobserver.h>
#include <moutputstreamobserver.h>

class CHTTPTestUtils;
class MPipeliningTestCase;

class MTimerClient
	{
public:
	virtual void TimeOut() = 0;		
	};

class CHttpTimer: public CActive
	{
	public:
		CHttpTimer(MTimerClient& aClient);
		~CHttpTimer();
		void After(TTimeIntervalMicroSeconds32 anInterval);
		void DoCancel();
		void RunL();
	private:
		RTimer iTimer;
		MTimerClient& iClient;
	};	

class CTestServerStreamManager : public CTimer,
								 public MInputStreamObserver,
								 public MOutputStreamObserver,
								 public MTimerClient
	{
public:
	static CTestServerStreamManager* NewL(CHTTPTestUtils& aTestUtils, TInt aConnectionIndex, MPipeliningTestCase* aTestCase, MInputStream* aInputStream, MOutputStream* aOutputStream);
	virtual ~CTestServerStreamManager();

private:
	CTestServerStreamManager(CHTTPTestUtils& aTestUtils, TInt aConnectionIndex, MPipeliningTestCase* aTestCase, MInputStream* aInputStream, MOutputStream* aOutputStream);
	void ConstructL();
	TBool ProcessRequestL();
	TBool IsConnectionCloseInData(const TDesC8& aRequest, const TDesC8& aResponse) const;
	void SendDataL();

	// From MInputStreamObserver
	void ReceivedDataIndL(const TDesC8& aBuffer);
	void SecureServerCnf();
	void InputStreamCloseInd(TInt aError);
	void MInputStreamObserver_Reserved();
	MHttpResponse*  CurrentResponse();
	virtual void OnReceiveTimeOut();
	
	// From MOutputStreamObserver
	void SendDataCnfL();
	void SecureClientCnf();
	void OutputStreamCloseInd(TInt aError);
	inline TBool SecureRetry(){ return EFalse;}
	void MOutputStreamObserver_Reserved();
    virtual void OnSendTimeOut();
	virtual TInt SendTimeOutVal();
	// From CTimer
	void RunL();
	
private:
	void TimeOut();	

private:
	CHTTPTestUtils&				iTestUtils;
	MInputStream*				iInputStream;	// Not owned
	MOutputStream*				iOutputStream;	// Not owned
	HBufC8*						iDataStore;
	HBufC8*						iDataToSend;
	MPipeliningTestCase*		iTestCase;		// Not owned
	TInt						iTransCount;
	TInt						iCurrentTrans;
	TInt						iDataPos;
	TBool						iCloseConnection;
	TInt						iConnectionIndex;
	TBool						iMoreResponseBatches;
	CHttpTimer*					iHttpTimer;
	CActiveSchedulerWait*		iASW;
	};
	


#endif // __CTESTSERVERSTREAMMANAGER_H__
