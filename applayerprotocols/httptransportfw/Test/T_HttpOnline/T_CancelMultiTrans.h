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

#if !defined (__CTEXTMODETESTCANCELMULTITRANS_H__)
#define __CTEXTMODETESTCANCELMULTITRANS_H__

#if !defined(__T_MULTITRANS_H__)
#include "T_MultiTrans.h"
#endif


/**
 *  Derived test case class : Test cancellation of a single transaction when multiple transactions are started
 */
class CTextModeTestCancelMultiTrans : public CTextModeTestMultiTrans
	{
public:
	static CTextModeTestCancelMultiTrans* NewL(const TDesC& aHostName, TInt aNumTrans, TInt aTestNum, TInt aDelay, TInt aIdxToCancel);
	virtual ~CTextModeTestCancelMultiTrans();
	void CancelFired(RHTTPTransaction aTrans);
protected:
	// methods inherited from MHTTPTransactionCallback
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
protected:
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
private:
	class CDelayedRequestCanceller : public CTimer
		{
	public:
		static CDelayedRequestCanceller* NewL(RHTTPTransaction aTrans, CTextModeTestCancelMultiTrans& aOwningTest);
		virtual void RunL();
		inline TBool HasFired() const {return iHasFired;};
	protected:
		CDelayedRequestCanceller(RHTTPTransaction aTrans, CTextModeTestCancelMultiTrans& aOwningTest);
	private:
		RHTTPTransaction iTrans;
		CTextModeTestCancelMultiTrans* iOwningTest;
		TBool iHasFired;
		};
private:
	CTextModeTestCancelMultiTrans(TInt aNumTrans, TInt aTestNum, TInt aDelay, TInt aIdxToCancel);
	void ConstructL(const TDesC& aHostName);
private:
	CDelayedRequestCanceller* iCancelTimer;
	TInt iTestNum;
	TInt iDelay;
	TInt iIdxToCancel;
	TBuf<40> iTestName;
	RArray<TInt> iTransIds;
	RArray<TInt> iTransStatus;
	TInt iFailureError;
	};

#endif // !defined __CTEXTMODETESTCANCELMULTITRANS_H__
