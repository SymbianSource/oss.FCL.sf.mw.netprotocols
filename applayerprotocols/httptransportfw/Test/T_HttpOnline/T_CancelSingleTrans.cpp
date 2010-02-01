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
// defines test class: CTextModeTestCancelSingleTrans
// 
//

#include "T_CancelSingleTrans.h"

// Constants used in CTextModeTestCancelSingleTrans test
const TInt KCancelTestEventWhenCancelled = -7398;
_LIT8(KTestUrlFormat, "http://%S/http_tests/prothnd/rfc2616.txt");
_LIT8(KSecureTestUrlFormat, "https://%S/http_tests/tcphnd/test1.html");


// CTextModeTestCancelSingleTrans-----------------------------------

CTextModeTestCancelSingleTrans* CTextModeTestCancelSingleTrans::NewL(const TDesC& aHostName, 
																	 TInt aTestNum, 
																	 TInt aDelay, 
																	 TBool aSecure)
	{
	CTextModeTestCancelSingleTrans* me = new(ELeave)CTextModeTestCancelSingleTrans(aTestNum, aDelay);
	CleanupStack::PushL(me);
	me->ConstructL(aHostName, aSecure);
	CleanupStack::Pop();
	return me;
	}

CTextModeTestCancelSingleTrans::~CTextModeTestCancelSingleTrans()
	{
	delete iTestUrl;
	}

void CTextModeTestCancelSingleTrans::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	if (iCancelTimer->HasFired())
		{
		iEngine->Utils().LogIt(_L("Single transaction cancellation test: transaction %d received event %d after being cancelled!"), aTransaction.Id(), aEvent.iStatus);
		User::Leave(KCancelTestEventWhenCancelled);
		}

	switch (aEvent.iStatus)
		{
	case THTTPEvent::EGotResponseHeaders:
		{
		// HTTP response headers have been received
		//iEngine->Utils().LogIt(_L("<Got response headers>"));
		DumpRespHeaders(aTransaction);
		} break;
	case THTTPEvent::ESubmit:
		{
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		//iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
		DumpResponseBody(aTransaction);
		} break;
	case THTTPEvent::EResponseComplete:
		{
		// The transaction's response is complete
		iEngine->Utils().LogIt(_L("<Transaction Complete>"));
		iReceivedRespComplete = ETrue;
		} break;
	case THTTPEvent::ESucceeded:
		{
		iEngine->Utils().LogIt(_L("<Transaction validated successfully>"));
		iReceivedFinalStatus = ETrue;
		CActiveScheduler::Stop();
		} break;
	case THTTPEvent::EFailed:
		{
		iEngine->Utils().LogIt(_L("<Transaction NOT validated successfully>"));
		iReceivedFinalStatus = ETrue;
		iFailureError = KErrTestFailed;
		CActiveScheduler::Stop();
		} 
		break;
	case THTTPEvent::EUnrecoverableError:
		{
		iEngine->Utils().LogIt(_L("<Test failed unrecoverably>"));
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

TInt CTextModeTestCancelSingleTrans::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	iEngine->Utils().LogIt(_L("\nMHFRunL left: error code %d on transaction ID=%d for event %d\n"), aError, aTransaction.Id(), aEvent.iStatus);
	// Place the error into the response status to get it out to the DoRunL()
	aTransaction.Response().SetStatusCode(aError);
	return KErrNone;
	}

TBool CTextModeTestCancelSingleTrans::GetNextDataPart(TPtrC8& /*aDataChunk*/)
	{
	TBool retVal = EFalse;
	return retVal;
	}

void CTextModeTestCancelSingleTrans::ReleaseData()
	{
	}

TInt CTextModeTestCancelSingleTrans::OverallDataSize()
	{
	return 0;
	}

const TDesC& CTextModeTestCancelSingleTrans::TestName()
	{
	_LIT(KHeaderTestNameFmt,"CTextModeTestCancelSingleTrans:%d");
	iTestName.Format(KHeaderTestNameFmt(), iTestNum);
	return iTestName;
	}

void CTextModeTestCancelSingleTrans::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);
	RStringPool strP = iSession.StringPool();

	// Open a GET transactions, specifying this object as the request body data supplier
	iTransaction = iSession.OpenTransactionL(iTestUrl->Uri(), *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	
	// Create the delayed notifier
	CSchedulerStopper* stopper = CSchedulerStopper::NewL();
	CActiveScheduler::Add(stopper);
	CleanupStack::PushL(stopper);
	iCancelTimer = CDelayedRequestCanceller::NewL(iTransaction, stopper);
	CActiveScheduler::Add(iCancelTimer);
	CleanupStack::PushL(iCancelTimer);

	// Set timer to fire when we want to cancel the transaction (50ms interval added per test)
	iCancelTimer->After(iDelay);

	// Submit the transaction
	iTransaction.SubmitL();
	iFailureError = KErrNone;
	CActiveScheduler::Start();

	if (iFailureError != KErrNone)
		{
		// Close anything opened
		CleanupStack::PopAndDestroy(3, &iSession); // timer, stopper then session
		
		// Check for failure error codes caught in MHFRunL
		User::LeaveIfError(iFailureError);
		}

	// Get the completion code
	TInt status = iTransaction.Response().StatusCode();

	// Anything other than 0 (no response received yet) or HTTP/200 status is a failure for this test.
	// Difficult to be precise about which tests should run to completion (i.e. 200) and which won't, so
	// set the test engine not to check status codes
	iExpectedStatusCode = 0;
	if (status < 0)
		User::Leave(status);
	else if ((status != 0) && (status != 200))
		User::Leave(KErrGeneral);
	
	// Close anything opened
	CleanupStack::PopAndDestroy(3, &iSession); // timer, stopper then session
	}

TInt CTextModeTestCancelSingleTrans::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}
	
void CTextModeTestCancelSingleTrans::DoCancel()
	{
	iCancelTimer->Cancel();
	}

CTextModeTestCancelSingleTrans::CTextModeTestCancelSingleTrans(TInt aTestNum, TInt aDelay)
	: iTestNum(aTestNum), iDelay(aDelay)
	{
	}

void CTextModeTestCancelSingleTrans::ConstructL(const TDesC& aHostName, TBool aSecure)
	{
	HBufC8* tmp1 = NULL;
	if(aSecure)
		tmp1 = HBufC8::NewL(KSecureTestUrlFormat().Length() + aHostName.Length());
	else
		tmp1 = HBufC8::NewL(KTestUrlFormat().Length() + aHostName.Length());

	CleanupStack::PushL(tmp1);

	HBufC8* tmp2 = HBufC8::NewL(aHostName.Length());
	CleanupStack::PushL(tmp2);
	tmp2->Des().Copy(aHostName);
	if(aSecure)
		tmp1->Des().Format(KSecureTestUrlFormat, tmp2);
	else
		tmp1->Des().Format(KTestUrlFormat, tmp2);
	
	TUriParser8 up;
	up.Parse(*tmp1);
	iTestUrl = CUri8::NewL(up);
	CleanupStack::PopAndDestroy(2, tmp1);
	}

CTextModeTestCancelSingleTrans::CDelayedRequestCanceller*
CTextModeTestCancelSingleTrans::CDelayedRequestCanceller::NewL(RHTTPTransaction aTrans, CSchedulerStopper* aStopper)
	{
	CDelayedRequestCanceller* me = new(ELeave)CDelayedRequestCanceller(aTrans, aStopper);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

CTextModeTestCancelSingleTrans::CDelayedRequestCanceller::CDelayedRequestCanceller(RHTTPTransaction aTrans, CSchedulerStopper* aStopper)
	: 	CTimer(EPriorityStandard), iTrans(aTrans), iStopSchedulerTimer(aStopper)
	{
	}

void CTextModeTestCancelSingleTrans::CDelayedRequestCanceller::RunL()
	{
	iHasFired = ETrue;
	iTrans.Cancel();
	iStopSchedulerTimer->After(5000000);
	}

CTextModeTestCancelSingleTrans::CSchedulerStopper*
CTextModeTestCancelSingleTrans::CSchedulerStopper::NewL()
	{
	CSchedulerStopper* me = new(ELeave)CSchedulerStopper();
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

CTextModeTestCancelSingleTrans::CSchedulerStopper::CSchedulerStopper()
	: CTimer(EPriorityStandard)
	{
	}

void CTextModeTestCancelSingleTrans::CSchedulerStopper::RunL()
	{
	CActiveScheduler::Stop();
	}
