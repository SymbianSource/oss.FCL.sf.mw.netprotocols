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
// defines test class: CTextModeTestCancelMultiTrans
// 
//

#include "T_CancelMultiTrans.h"


// Constants used in test
const TInt KCancelTestEventWhenCancelled = -7398;


// CTextModeTestCancelMultiTrans-----------------------------------

CTextModeTestCancelMultiTrans* CTextModeTestCancelMultiTrans::NewL(const TDesC& aHostName, TInt aNumTrans, TInt aTestNum, TInt aDelay, TInt aIdxToCancel)
	{
	CTextModeTestCancelMultiTrans* me = new(ELeave)CTextModeTestCancelMultiTrans(aNumTrans, aTestNum, aDelay, aIdxToCancel);
	CleanupStack::PushL(me);
	me->ConstructL(aHostName);
	CleanupStack::Pop();
	return me;
	}

CTextModeTestCancelMultiTrans::~CTextModeTestCancelMultiTrans()
	{
	delete iCancelTimer;
	iTransStatus.Close();
	iTransIds.Close();
	}

const TDesC& CTextModeTestCancelMultiTrans::TestName()
	{
	_LIT(KHeaderTestNameFmt,"CTextModeTestCancelMultiTrans:%d");
	iTestName.Format(KHeaderTestNameFmt(), iTestNum);
	return iTestName;
	}


TInt CTextModeTestCancelMultiTrans::MHFRunError(TInt aError, 
										RHTTPTransaction aTransaction,
										const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("TestCancelMultiTrans: failed with error code %d on transaction ID=%d\n"), aError, aTransaction.Id());
	// Use the response status to convey the error code
	aTransaction.Response().SetStatusCode(aError);
	return KErrNone;
	}

void CTextModeTestCancelMultiTrans::MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent)
	{
	// look up the transaction ID in our list of launched transactions
	TInt transIdx = KErrNotFound;
	TInt ii = 0;
	while ((ii < iMaxNumTransactions) && (transIdx == KErrNotFound))
		{
		if (iTransIds[ii] == aTransaction.Id())
			transIdx = ii;
		else
			ii++;
		}

	// Leave with an error if the event is for the transaction which has been cancelled
	if (iCancelTimer->HasFired() && (aTransaction.Id() == iTransIds[iIdxToCancel]))
		{
		iTransStatus[ii] = KCancelTestEventWhenCancelled;
		User::Leave(KCancelTestEventWhenCancelled);
		}

	switch (aEvent.iStatus)
		{
	case THTTPEvent::EGotResponseHeaders:
		{
		// HTTP response headers have been received
		iEngine->Utils().LogIt(_L("<Got response headers>"));
		RHTTPResponse resp = aTransaction.Response();
		iTransStatus[ii] = resp.StatusCode();
		} break;
	case THTTPEvent::ESubmit:
		{
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
		ValidateBodyL(aTransaction);
		} break;
	case THTTPEvent::EResponseComplete:
		{
		// The transaction's response is complete
		iEngine->Utils().LogIt(_L("<Transaction Complete>"));
		} break;
	case THTTPEvent::ESucceeded:
		{
		iEngine->Utils().LogIt(_L("<Transaction validated successfully>"));
		if (--iOpenTransactionCount == 0)
			CActiveScheduler::Stop();
		} break;
	case THTTPEvent::EFailed:
		{
		iEngine->Utils().LogIt(_L("<Transaction NOT validated successfully>"));
		if (--iOpenTransactionCount == 0)
			CActiveScheduler::Stop();
		iFailureError = KErrTestFailed;
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

void CTextModeTestCancelMultiTrans::DoRunL()
	{
	iSession.OpenL();
	CleanupClosePushL(iSession);

	// Open the HTTP session
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
	RStringPool strP = iSession.StringPool();
	connInfo.SetPropertyL(strP.StringF(HTTP::EMaxNumTransportHandlers,RHTTPSession::GetTable()), THTTPHdrVal(1));

	// Create the array of transactions
	CreateTransArrayL();

	// Populate some other arrays used for event checking in MHFRunL
	TInt ii;
	for (ii = 0; ii < iMaxNumTransactions; ii++)
		{
		iTransIds.Append(iTransArray[ii].Id());
		iTransStatus.Append(0);
		}

	// Create the delayed notifier
	iCancelTimer = CDelayedRequestCanceller::NewL(iTransArray[iIdxToCancel], *this);
	CActiveScheduler::Add(iCancelTimer);

	// Set timer to fire when we want to cancel the transaction (50ms interval added per test)
	iCancelTimer->After(iDelay);

	// Go!
	iFailureError = 0;
	CActiveScheduler::Start();

	// Close anything opened, and fail straight away on any failure error caught in MHFRunL
	CleanupStack::PopAndDestroy(&iSession);
	User::LeaveIfError(iFailureError);

	// Anything other than 0 (no response received yet) or HTTP/200 status in any of the
	// transactions is a failure for this test
	// Difficult to be precise about which tests should run to completion (i.e. 200) and which won't, so
	// set the test engine not to check status codes
	iExpectedStatusCode = 0;
	TInt status = 0, st = 0, id = 0;
	for (ii = 0; ii < iTransStatus.Count(); ii++)
		{
		st = iTransStatus[ii];
		if (st < 0)
			{
			status = st;
			id = iTransIds[ii];
			}
		else if ((st != 0) && (st != 200))
			{
			status = KErrGeneral;
			id = iTransIds[ii];
			}
		}

	// Log the failure we've picked on to report
	if (status < 0)
		iEngine->Utils().LogIt(_L("TestCancelMultiTrans : transaction ID=%d failed with code %d"), id, status);
	User::LeaveIfError(status);
	}

TInt CTextModeTestCancelMultiTrans::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}
	
void CTextModeTestCancelMultiTrans::DoCancel()
	{
	iCancelTimer->Cancel();
	}

CTextModeTestCancelMultiTrans::CTextModeTestCancelMultiTrans(TInt aNumTrans, TInt aTestNum, TInt aDelay, TInt aIdxToCancel)
	:  CTextModeTestMultiTrans(aNumTrans), iTestNum(aTestNum), iDelay(aDelay), iIdxToCancel(aIdxToCancel)
	{
	}

void CTextModeTestCancelMultiTrans::CancelFired(RHTTPTransaction aTrans)
	{
	aTrans.Cancel();
	if (--iOpenTransactionCount == 0)
		CActiveScheduler::Stop();
	}

void CTextModeTestCancelMultiTrans::ConstructL(const TDesC& aHostName)
	{
	// call Base ConstructL
	CTextModeTestMultiTrans::ConstructL(aHostName);
	}

CTextModeTestCancelMultiTrans::CDelayedRequestCanceller*
CTextModeTestCancelMultiTrans::CDelayedRequestCanceller::NewL(RHTTPTransaction aTrans, CTextModeTestCancelMultiTrans& aOwningTest)
	{
	CDelayedRequestCanceller* me = new(ELeave)CDelayedRequestCanceller(aTrans, aOwningTest);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

CTextModeTestCancelMultiTrans::CDelayedRequestCanceller::CDelayedRequestCanceller(RHTTPTransaction aTrans, CTextModeTestCancelMultiTrans& aOwningTest)
	: 	CTimer(EPriorityStandard), iTrans(aTrans), iOwningTest(&aOwningTest)
	{
	}

void CTextModeTestCancelMultiTrans::CDelayedRequestCanceller::RunL()
	{
	iHasFired = ETrue;
	iOwningTest->CancelFired(iTrans);
	}
