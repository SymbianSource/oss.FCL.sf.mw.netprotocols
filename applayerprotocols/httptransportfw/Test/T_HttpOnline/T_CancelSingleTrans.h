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

#if !defined (__CTEXTMODETESTCANCELSINGLETRANS_H__)
#define __CTEXTMODETESTCANCELSINGLETRANS_H__

#include <http.h>
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

/**
 *  Derived test case class : Test  cancellation of single transaction after a specified time delay
 */
class CUri8;
class CTextModeTestCancelSingleTrans: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
public:
	static CTextModeTestCancelSingleTrans* NewL(const TDesC& aHostName, TInt aTestNum, TInt aDelay, TBool aSecure);
	virtual ~CTextModeTestCancelSingleTrans();
public: 
	// methods inherited from MHTTPTransactionCallback
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	// methods inherited from MHTTPDataSupplier
	virtual TBool GetNextDataPart(TPtrC8& aDataChunk);
	void ReleaseData();
	TInt OverallDataSize();
protected:
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
private:
	class CSchedulerStopper : public CTimer
		{
	public:
		static CSchedulerStopper* NewL();
		virtual void RunL();
	protected:
		CSchedulerStopper();
	private:
		};
	class CDelayedRequestCanceller : public CTimer
		{
	public:
		static CDelayedRequestCanceller* NewL(RHTTPTransaction aTrans, CSchedulerStopper* aStopper);
		virtual void RunL();
		inline TBool HasFired() const {return iHasFired;};
	protected:
		CDelayedRequestCanceller(RHTTPTransaction aTrans, CSchedulerStopper* aStopper);
	private:
		RHTTPTransaction iTrans;
		CSchedulerStopper* iStopSchedulerTimer;
		TBool iHasFired;
		};
private:
	CTextModeTestCancelSingleTrans(TInt aTestNum, TInt aDelay);
	void ConstructL(const TDesC& aHostName, TBool aSecure);
private:
	CDelayedRequestCanceller* iCancelTimer;
	TInt iTestNum;
	TInt iDelay;
	TBuf<40> iTestName;
	CUri8* iTestUrl;
	TBool iReceivedRespComplete;
	TBool iReceivedFinalStatus;
	TBool iFailureError;
	};


#endif // !defined __CTEXTMODETESTCANCELSINGLETRANS_H__
