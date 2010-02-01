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

#if !defined (__CTestPerformanceTrans_H__)
#define __CTestPerformanceTrans_H__

#include <http.h>
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

/**
 *  Derived test case class : test parallel transactions
 */
class CTestPerformanceTrans: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
public:

	enum THttpPerformanceTestMode
		{
		EGetParallel,
		EPostParallel,
		EGetSequential,
		EPostSequential
		};

	static CTestPerformanceTrans* NewLC(const TDesC& aHostName, TInt aNumTrans, THttpPerformanceTestMode aTestMode);
	virtual ~CTestPerformanceTrans();

// from MHTTPFilterBase
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

private:
	CTestPerformanceTrans(TInt aNumTrans, THttpPerformanceTestMode aTestMode);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void CreateTransArrayL();
	void ConstructL(const TDesC& aHostName);
	void HandleCompletedTransactionL();

private:
	TInt iMaxNumTransactions;
	RArray<RHTTPTransaction> iTransArray;
	TInt iOpenTransactionCount;
	HBufC8* iHostName;
	TInt iFailureError;
	THttpPerformanceTestMode iTestMode;
	};

#endif // !defined __CTestPerformanceTrans_H__
