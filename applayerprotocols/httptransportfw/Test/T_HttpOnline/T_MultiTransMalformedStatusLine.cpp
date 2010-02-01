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
// defines test class: CTextModeTestMultiTransMalformedStatusLine
// 
//

#include "T_MultiTransMalformedStatusLine.h"


// CTextModeTestMultiTrans-----------------------------------

CTextModeTestMultiTransMalformedStatusLine* CTextModeTestMultiTransMalformedStatusLine::NewL(const TDesC& aHostName, TInt aMaxNumTrans)
	{
	CTextModeTestMultiTransMalformedStatusLine* me = new(ELeave)CTextModeTestMultiTransMalformedStatusLine(aMaxNumTrans);
	CleanupStack::PushL(me);
	me->ConstructL(aHostName);
	CleanupStack::Pop();
	return me;
	}

CTextModeTestMultiTransMalformedStatusLine::~CTextModeTestMultiTransMalformedStatusLine()
	{
	delete iHostName;
	iTransArray.Close();
	}

CTextModeTestMultiTransMalformedStatusLine::CTextModeTestMultiTransMalformedStatusLine(TInt aMaxNumTrans)
	: iMaxNumTransactions(aMaxNumTrans)
	{
	// does nothing
	}

void CTextModeTestMultiTransMalformedStatusLine::ConstructL(const TDesC& aHostName)
	{
	iHostName = HBufC8::NewL(aHostName.Length());
	iHostName->Des().Copy(aHostName);
	}

void CTextModeTestMultiTransMalformedStatusLine::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	// Create the array of transactions
	CreateTransArrayL();

	// Go!
	iFailureError = KErrNone;
	CActiveScheduler::Start();

	// No particular status code is required for success in this test
	iExpectedStatusCode = 0;

	// Close anything opened
	CleanupStack::PopAndDestroy(); //close iSession

	// If any transaction failed then fail the test
	User::LeaveIfError(iFailureError);
	}
	
TInt CTextModeTestMultiTransMalformedStatusLine::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CTextModeTestMultiTransMalformedStatusLine::DoCancel()
	{
	}

const TDesC& CTextModeTestMultiTransMalformedStatusLine::TestName()
	{
	_LIT(KHeaderTestName,"CTextModeTestMultiTransMalformedStatusLine");
	return KHeaderTestName;
	}


TInt CTextModeTestMultiTransMalformedStatusLine::MHFRunError(TInt aError, 
										RHTTPTransaction aTransaction,
										const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d on transaction ID=%d\n"), aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTextModeTestMultiTransMalformedStatusLine::MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
	case THTTPEvent::EGotResponseHeaders:
		{
		// HTTP response headers have been received
		iEngine->Utils().LogIt(_L("<Got response headers>"));
		} break;
	case THTTPEvent::ESubmit:
		{
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
		} break;
	case THTTPEvent::EResponseComplete:
		{
		// The transaction's response is complete
		iEngine->Utils().LogIt(_L("<Transaction Complete>"));
		} break;
	case THTTPEvent::ESucceeded:
		{
		iEngine->Utils().LogIt(_L("<Transaction validated successfully>"));
		aTransaction.Close();
		if (--iOpenTransactionCount == 0)
			CActiveScheduler::Stop();
		} break;
	case THTTPEvent::EFailed:
		{
		iEngine->Utils().LogIt(_L("<Transaction NOT validated successfully>"));
		aTransaction.Close();
		if (--iOpenTransactionCount == 0)
			{
			CActiveScheduler::Stop();
			}
		iFailureError = KErrTestFailed;
		} break;
	default:
		{
		iEngine->Utils().LogIt(_L("<unrecognised event> %d"),aEvent.iStatus);
		aTransaction.Close();
		TInt status = aEvent.iStatus;
		if ( status < 0)
			{
			if( status == KErrCouldNotConnect )
				{
				++iConnectFailureCount;
				// If we get KErrCouldNotConnect create another transation to replace (it means the
				// test server is too slow and was not ready for the next request)
				RStringF get = iSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
				CreateSingleTransactionL(get);
				}
			else if( status == KErrCorrupt )
				iConnectFailureCount = 0;
            else
				iFailureError = aEvent.iStatus;

			if( --iOpenTransactionCount == 0 )
				CActiveScheduler::Stop();

            if ( iConnectFailureCount == 10 )
				{
				iFailureError = KErrCouldNotConnect;
				CActiveScheduler::Stop();
				}
			}
		} 
		break;
		}
	}

void CTextModeTestMultiTransMalformedStatusLine::CreateTransArrayL()
	{
	// Open several transactions, all with a URL of the form http://xxx/y.txt where y is from 1 to iMaxNumTransactions
	RStringF get = iSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	iOpenTransactionCount = 0;
	for (TInt ii = 1; ii <= 6; ii++)
		{
		CreateSingleTransactionL(get);
		}
	}

void CTextModeTestMultiTransMalformedStatusLine::CreateSingleTransactionL(RStringF aGet)
	{
	// create the transaction and submit
	TUriParser8 urlParser;
	urlParser.Parse(*iHostName);
	RHTTPTransaction tr = iSession.OpenTransactionL(urlParser, *this, aGet);
	CleanupClosePushL(tr);
	User::LeaveIfError(iTransArray.Append(tr));
	CleanupStack::Pop(&tr);
	tr.SubmitL();
	iOpenTransactionCount++;
	}
