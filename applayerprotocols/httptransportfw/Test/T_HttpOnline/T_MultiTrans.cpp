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
// defines test class: CTextModeTestMultiTrans
// 
//

#include "T_MultiTrans.h"

// Constants used in CTextModeTestMultiTrans test
_LIT8(KHttpMultiTransUrlFormat, "http://%S/http_tests/prothnd/");
_LIT8(KHttpMultiTransUrlExtn, ".txt");
const TInt KMultiTransFailed = -7399;

// CTextModeTestMultiTrans-----------------------------------

CTextModeTestMultiTrans* CTextModeTestMultiTrans::NewL(const TDesC& aHostName, TInt aMaxNumTrans)
	{
	CTextModeTestMultiTrans* me = new(ELeave)CTextModeTestMultiTrans(aMaxNumTrans);
	CleanupStack::PushL(me);
	me->ConstructL(aHostName);
	CleanupStack::Pop();
	return me;
	}

CTextModeTestMultiTrans::~CTextModeTestMultiTrans()
	{
	delete iHostName;
	iTransArray.Close();
	}

CTextModeTestMultiTrans::CTextModeTestMultiTrans(TInt aMaxNumTrans)
	: iMaxNumTransactions(aMaxNumTrans)
	{
	// does nothing
	}

void CTextModeTestMultiTrans::ConstructL(const TDesC& aHostName)
	{
	iHostName = HBufC8::NewL(aHostName.Length());
	iHostName->Des().Copy(aHostName);
	}

void CTextModeTestMultiTrans::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	// Set the protocol to use
/*	if (iUseWsp)
		{
		RHTTPConnectionInfo ci = iSession.ConnectionInfo();
		ci.SetPropertyL(iSession.StringPool().StringF(HTTP::EProtocol,RHTTPSession::GetTable()), THTTPHdrVal(iSession.StringPool().StringF(HTTP::EWSP,RHTTPSession::GetTable())));
		}*/

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
	
TInt CTextModeTestMultiTrans::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CTextModeTestMultiTrans::DoCancel()
	{
	}

const TDesC& CTextModeTestMultiTrans::TestName()
	{
	_LIT(KHeaderTestName,"CTextModeTestMultiTrans");
	return KHeaderTestName;
	}


TInt CTextModeTestMultiTrans::MHFRunError(TInt aError, 
										RHTTPTransaction aTransaction,
										const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d on transaction ID=%d\n"), aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTextModeTestMultiTrans::MHFRunL(RHTTPTransaction aTransaction,
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
		if (aEvent.iStatus < 0)
			{
			iFailureError = aEvent.iStatus;
			CActiveScheduler::Stop();
			}
		} 
		break;
		}
	}

void CTextModeTestMultiTrans::ValidateBodyL(RHTTPTransaction aTransaction)
	{
	// In this test, the test URLs point to resources that contain just characters from the test number
	// i.e. http:/xxx/1.txt will contain only the char 1, CR or LF.
	// These numbers should also align with the transaction ID, since they were created in order
	RHTTPResponse resp = aTransaction.Response();
	MHTTPDataSupplier* body = resp.Body();
	TPtrC8 data;
	body->GetNextDataPart(data);
	TChar reqdCh = (TChar)(aTransaction.Id() + 48);
	for (TInt ii = 0; ii < data.Length(); ii++)
		{
		TChar ch = data[ii];
		if ((ch != reqdCh) && (ch != (TChar)0x0d) && (ch != (TChar)0x0a))
			User::Leave(KMultiTransFailed);
		}

	// OK - release the block
	body->ReleaseData();
	}

void CTextModeTestMultiTrans::CreateTransArrayL()
	{
	// Open several transactions, all with a URL of the form http://xxx/y.txt where y is from 1 to iMaxNumTransactions
	TBuf8<50> url;
	RStringF get = iSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	iOpenTransactionCount = 0;
	for (TInt ii = 1; ii <= iMaxNumTransactions; ii++)
		{
		// create the transaction and submit
		url.Zero();
		url.Format(KHttpMultiTransUrlFormat(), iHostName);
		url.Append(TChar(ii + 48));
		url.Append(KHttpMultiTransUrlExtn());
		TUriParser8 urlParser;
		urlParser.Parse(url);
		RHTTPTransaction tr = iSession.OpenTransactionL(urlParser, *this, get);
		CleanupClosePushL(tr);
		User::LeaveIfError(iTransArray.Append(tr));
		CleanupStack::Pop(&tr);
		tr.SubmitL();
		iOpenTransactionCount++;
		}
	}

