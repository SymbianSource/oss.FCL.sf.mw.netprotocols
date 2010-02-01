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

#include "T_performancetrans.h"
#include  <http/rhttpheaders.h>


CTestPerformanceTrans* CTestPerformanceTrans::NewLC(const TDesC& aHostName, TInt aMaxNumTrans, 
												   THttpPerformanceTestMode aTestMode)
	{
	CTestPerformanceTrans* me = new(ELeave)CTestPerformanceTrans(aMaxNumTrans,aTestMode);
	CleanupStack::PushL(me);
	me->ConstructL(aHostName);
	return me;
	}

CTestPerformanceTrans::~CTestPerformanceTrans()
	{
	delete iHostName;
	iTransArray.Close();
	}

CTestPerformanceTrans::CTestPerformanceTrans(TInt aNumTrans,THttpPerformanceTestMode aTestMode)
	: iMaxNumTransactions(aNumTrans), iTestMode(aTestMode)
	{
	// does nothing
	}

void CTestPerformanceTrans::ConstructL(const TDesC& aHostName)
	{
	iHostName = HBufC8::NewL(aHostName.Length());
	iHostName->Des().Copy(aHostName);
	}

void CTestPerformanceTrans::DoRunL()
	{
	iSession.OpenL();
	CleanupClosePushL(iSession);
	CreateTransArrayL();
	iFailureError = KErrNone;
	CActiveScheduler::Start();
	iExpectedStatusCode = 0; // No particular status code is required for success in this test
	// active scheduler will be stopped in the MHFRunL after all transactions have been completed or one fails
	// Close anything opened
	CleanupStack::PopAndDestroy(&iSession);
	// If any transaction failed then fail the test
	User::LeaveIfError(iFailureError);
	}

	
TInt CTestPerformanceTrans::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}


void CTestPerformanceTrans::DoCancel()
	{
	}


const TDesC& CTestPerformanceTrans::TestName()
	{
	_LIT(KHeaderTestName,"CTestPerformanceTrans");
	return KHeaderTestName;
	}


TInt CTestPerformanceTrans::MHFRunError(TInt aError, RHTTPTransaction aTransaction,
										const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d on transaction ID=%d\n"), aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}


void CTestPerformanceTrans::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
	case THTTPEvent::EGotResponseHeaders:
		// HTTP response headers have been received
		iEngine->Utils().LogIt(_L("<Got response headers>"));
		 break;
	case THTTPEvent::ESubmit:
		break;
	case THTTPEvent::EGotResponseBodyData:
		// Some (more) body data has been received (in the HTTP response)
		iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
		aTransaction.Response().Body()->ReleaseData();
		break;
	case THTTPEvent::EResponseComplete:
		// The transaction's response is complete
		iEngine->Utils().LogIt(_L("<Transaction Complete>")); 
		break;
	case THTTPEvent::ESucceeded:
		HandleCompletedTransactionL();
		break;
	case THTTPEvent::EFailed:
		{
		iEngine->Utils().LogIt(_L("<Transaction Failed>"));
		CActiveScheduler::Stop();
		iFailureError = KErrTestFailed;
		}
		break;
	default:
		{
		iEngine->Utils().LogIt(_L("<unrecognised event> %d"),aEvent.iStatus);
		if (aEvent.iStatus < 0)
			{
			iFailureError = aEvent.iStatus;
			CActiveScheduler::Stop();
			}
		} 
		break;
		}
	}

void CTestPerformanceTrans::HandleCompletedTransactionL()
	{
	iEngine->Utils().LogIt(_L("<Transaction Succeeded >"));
	
	if (--iOpenTransactionCount == 0)
			CActiveScheduler::Stop();
	else if (iTestMode == EGetSequential || iTestMode == EPostSequential)
		{
		iTransArray[iOpenTransactionCount - 1].SubmitL();
		}
	}


void CTestPerformanceTrans::CreateTransArrayL()
	{
	for (TInt ii = 0; ii < iMaxNumTransactions; ii++)
		{
		RStringPool strP = iSession.StringPool();
		RStringF method;
		if (iTestMode == EGetParallel || iTestMode == EGetSequential)
			method = strP.StringF(HTTP::EGET,RHTTPSession::GetTable());
		else
			method = strP.StringF(HTTP::EPOST,RHTTPSession::GetTable());
		
		TUriParser8 urlParser;
		urlParser.Parse(*iHostName);
		RHTTPTransaction tr = iSession.OpenTransactionL(urlParser, *this, method);
		CleanupClosePushL(tr);

		RHTTPRequest request = tr.Request();
		if (request.Method().Index(RHTTPSession::GetTable()) == HTTP::EPOST)
			{
			request.SetBody(*this);
			RHTTPHeaders hdr = request.GetHeaderCollection();
			THTTPHdrVal contentType(strP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable()));
			hdr.SetFieldL(strP.StringF(HTTP::EContentType,RHTTPSession::GetTable()),contentType); 
			}

		User::LeaveIfError(iTransArray.Append(tr));
		CleanupStack::Pop(&tr);

		// if this is the end transaction or we are in parallel mode then submit the transaction
		if (iTestMode == EGetParallel || iTestMode == EPostParallel || ii== iMaxNumTransactions -1)
			tr.SubmitL();
	
		iOpenTransactionCount++;
		}
	}

