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
// System includes
// 
//

#include <uri8.h>

//
// Class signature
//
#include "T_Proxies.h"

//
// Global declarations for all tests
//
_LIT8(KTestUserAgentVal, "HttpTester");
_LIT8(KTestContentTypeVal, "application/x-www-form-urlencoded");
const TInt KTestContentLenVal = 15;

// Request Body
_LIT8(KTestRqBody, "abc=def&ghi=jkl");
_LIT8(KEmptyHostVal, "");
_LIT8(KIncorrectHostVal, "wrong.host.name");
_LIT8(KTestRelURL, "/perl/dumpform.pl");
_LIT8(KTestScheme, "http");

// Test 1 - Absolute URL; compatible Host header; proxy on
TBool KTest1UseProxy  = ETrue;
TBool KTest1UseRelURL = EFalse;

// Test 2 - Absolute URL; incorrect Host header; proxy on
TBool KTest2UseProxy  = ETrue;
TBool KTest2UseRelURL = EFalse;

// Test 3 - Absolute URL; missing Host header; proxy on
TBool KTest3UseProxy  = ETrue;
TBool KTest3UseRelURL = EFalse;

// Test 4 - Relative URL; correct Host header; proxy on
TBool KTest4UseProxy  = ETrue;
TBool KTest4UseRelURL = ETrue;

// Test 5 - Relative URL; missing Host header; proxy on
TBool KTest5UseProxy  = ETrue;
TBool KTest5UseRelURL = ETrue;

// Test 6 - Absolute URL; compatible Host header; proxy off
TBool KTest6UseProxy  = EFalse;
TBool KTest6UseRelURL = EFalse;

// Test 7 - Absolute URL; incorrect Host header; proxy off
TBool KTest7UseProxy  = EFalse;
TBool KTest7UseRelURL = EFalse;

// Test 8 - Absolute URL; missing Host header; proxy off
TBool KTest8UseProxy  = EFalse;
TBool KTest8UseRelURL = EFalse;

// Test 9 - Relative URL; correct Host header; proxy off
TBool KTest9UseProxy  = EFalse;
TBool KTest9UseRelURL = ETrue;

// Test 10 - Relative URL; missing Host header; proxy off
TBool KTest10UseProxy = EFalse;
TBool KTest10UseRelURL = ETrue;


// CTextModeTestProxies-----------------------------------

CTextModeTestProxies* CTextModeTestProxies::NewL(TInt aTestNum, const TDesC8& aProxyAddr, const TDesC8& aHostName)
	{
	CTextModeTestProxies* me = new(ELeave)CTextModeTestProxies(aTestNum);
	CleanupStack::PushL(me);
	me->ConstructL(aProxyAddr, aHostName);
	CleanupStack::Pop();
	return me;
	}

CTextModeTestProxies::~CTextModeTestProxies()
	{
	delete iProxyAddr;
	delete iHostName;
	delete iAbsUri;
	}

void CTextModeTestProxies::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
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
		} break;
	case THTTPEvent::ESucceeded:
		{
		iEngine->Utils().LogIt(_L("<Transaction validated successfully>"));
		CActiveScheduler::Stop();
		} break;

	case THTTPEvent::EFailed:
		{
		iEngine->Utils().LogIt(_L("<Transaction NOT validated successfully>"));
		iFailureError = KErrTestFailed;
		CActiveScheduler::Stop();
		} break;
	default:
		{
		iEngine->Utils().LogIt(_L("<unrecognised event> %d"),aEvent.iStatus);
		if (aEvent.iStatus < 0)
			{
			iFailureError = aEvent.iStatus;

			// Tests 5 and 10 expect a missing Host error KErrHttpGeneralHeaderMissingHost,
			// so ignore that one.
			if ( (aEvent.iStatus == KErrHttpGeneralHeaderMissingHost) &&
				 ( (iTestNum == 5) || (iTestNum == 10) ) )
				iFailureError = KErrNone;
			CActiveScheduler::Stop();
			}
		} 
		break;
		}
	}

TInt CTextModeTestProxies::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	iEngine->Utils().LogIt(_L("\nMHFRunL left: error code %d on transaction ID=%d for event %d\n"), aError, aTransaction.Id(), aEvent.iStatus);
	return KErrNone;
	}

TBool CTextModeTestProxies::GetNextDataPart(TPtrC8& aDataChunk)
	{
	aDataChunk.Set(KTestRqBody());
	return ETrue;
	} 

void CTextModeTestProxies::ReleaseData()
	{
	// Only a single chunk of data - do nothing as no more data to send!
	}

TInt CTextModeTestProxies::OverallDataSize()
	{
	return KTestRqBody().Length();
	}

const TDesC& CTextModeTestProxies::TestName()
	{
	_LIT(KHeaderTestName,"CTextModeTestProxies");
	return KHeaderTestName;
	}

void CTextModeTestProxies::DoRunL()
	{
	// Run appropriate test
	switch (iTestNum)
		{
	case 1:
		Test1L(); 
		break;
	case 2:
		Test2L(); 
		break;
	case 3:
		Test3L(); 
		break;
	case 4:
		Test4L(); 
		break;
	case 5:
		Test5L(); 
		break;
	case 6:
		Test6L(); 
		break;
	case 7:
		Test7L(); 
		break;
	case 8:
		Test8L(); 
		break;
	case 9:
		Test9L(); 
		break;
	case 10:
		Test10L(); 
		break;
	default:
		;
		}
	}

TInt CTextModeTestProxies::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}
	
void CTextModeTestProxies::DoCancel()
	{
	}

CTextModeTestProxies::CTextModeTestProxies(TInt aTestNum)
	: CHttpTestTransBase(), iTestNum(aTestNum)
	{
	}

void CTextModeTestProxies::ConstructL(const TDesC8& aProxyAddr, const TDesC8& aHostName)
	{
	iProxyAddr = aProxyAddr.AllocL();
	iHostName = aHostName.AllocL();

	// Form the correct absolute URL
	iAbsUri = CUri8::NewLC();
	iAbsUri->SetComponentL(KTestScheme(), EUriScheme);
	iAbsUri->SetComponentL(aHostName, EUriHost);
	iAbsUri->SetComponentL(KTestRelURL(), EUriPath);
	CleanupStack::Pop(iAbsUri);
	}

void CTextModeTestProxies::Test1L()
	{
	iEngine->Utils().LogIt(_L("Test 1 - Absolute URL; compatible Host header; proxy on"));
	iTestNum = 1;
	DoTestMethodL(KTest1UseRelURL, *iHostName, KTest1UseProxy);
	}

void CTextModeTestProxies::Test2L()
	{
	iEngine->Utils().LogIt(_L("Test 2 - Absolute URL; incorrect Host header; proxy on"));
	iTestNum = 2;
	DoTestMethodL(KTest2UseRelURL, KIncorrectHostVal(), KTest2UseProxy);
	}

void CTextModeTestProxies::Test3L()
	{
	iEngine->Utils().LogIt(_L("Test 3 - Absolute URL; missing Host header; proxy on"));
	iTestNum = 3;
	DoTestMethodL(KTest3UseRelURL, KEmptyHostVal, KTest3UseProxy);
	}

void CTextModeTestProxies::Test4L()
	{
	iEngine->Utils().LogIt(_L("Test 4 - Relative URL; correct Host header; proxy on"));
	iTestNum = 4;
	DoTestMethodL(KTest4UseRelURL, *iHostName, KTest4UseProxy);
	}

void CTextModeTestProxies::Test5L()
	{
	iEngine->Utils().LogIt(_L("Test 5 - Relative URL; missing Host header; proxy on"));
	iTestNum = 5;
	DoTestMethodL(KTest5UseRelURL, KEmptyHostVal, KTest5UseProxy);
	}

void CTextModeTestProxies::Test6L()
	{
	iEngine->Utils().LogIt(_L("Test 6 - Absolute URL; compatible Host header; proxy off"));
	iTestNum = 6;
	DoTestMethodL(KTest6UseRelURL, *iHostName, KTest6UseProxy);
	}

void CTextModeTestProxies::Test7L()
	{
	iEngine->Utils().LogIt(_L("Test 7 - Absolute URL; incorrect Host header; proxy off"));
	iTestNum = 7;
	DoTestMethodL(KTest7UseRelURL, KIncorrectHostVal(), KTest7UseProxy);
	}

void CTextModeTestProxies::Test8L()
	{
	iEngine->Utils().LogIt(_L("Test 8 - Absolute URL; missing Host header; proxy off"));
	iTestNum = 8;
	DoTestMethodL(KTest8UseRelURL, KEmptyHostVal, KTest8UseProxy);
	}

void CTextModeTestProxies::Test9L()
	{
	iEngine->Utils().LogIt(_L("Test 9 - Relative URL; correct Host header; proxy off"));
	iTestNum = 9;
	DoTestMethodL(KTest9UseRelURL, *iHostName, KTest9UseProxy);
	}

void CTextModeTestProxies::Test10L()
	{
	iEngine->Utils().LogIt(_L("Test 10 - Relative URL; missing Host header; proxy off"));
	iTestNum = 10;
	DoTestMethodL(KTest10UseRelURL, KEmptyHostVal, KTest10UseProxy);
	}

void CTextModeTestProxies::DoTestMethodL(TBool aUseRelativeURL, const TDesC8& aHostVal, TBool aUseProxy)
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);
	RStringPool strP = iSession.StringPool();

	// Specify a proxy address, if required
	if (aUseProxy)
		{
		RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
		THTTPHdrVal proxyUsage(strP.StringF(HTTP::EUseProxy,RHTTPSession::GetTable()));
		connInfo.SetPropertyL(strP.StringF(HTTP::EProxyUsage,RHTTPSession::GetTable()), proxyUsage);
		RStringF proxy = strP.OpenFStringL(*iProxyAddr);
		CleanupClosePushL(proxy);
		THTTPHdrVal proxyAddr(proxy);
		connInfo.SetPropertyL(strP.StringF(HTTP::EProxyAddress,RHTTPSession::GetTable()), proxyAddr);
		CleanupStack::PopAndDestroy(); // proxy
		}

	// Form the URI for this test
	TUriParser8 up;
	if (aUseRelativeURL)
		up.Parse(KTestRelURL());
	else
		up.Parse(iAbsUri->Uri().UriDes());	

	// Open a POST transactions, specifying this object as the request body data supplier
	iTransaction = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	CleanupClosePushL(iTransaction);
	RHTTPRequest rq = iTransaction.Request();
	rq.SetBody(*this);
	RHTTPHeaders hdr = rq.GetHeaderCollection();

	// Add a host header if it isn't empty
	RStringF host, contType, userAgent;
	TInt numStringsOpened = 0;
	if (aHostVal.Compare(KNullDesC8()))
		{
		host = strP.OpenFStringL(aHostVal);
		CleanupClosePushL(host);
		numStringsOpened++;
		hdr.SetFieldL(strP.StringF(HTTP::EHost,RHTTPSession::GetTable()), THTTPHdrVal(host));
		}

	// Content-Length header
	THTTPHdrVal length(KTestContentLenVal);
	hdr.SetFieldL(strP.StringF(HTTP::EContentLength,RHTTPSession::GetTable()), length);

	// Content-Type header
	contType = strP.OpenFStringL(KTestContentTypeVal());
	CleanupClosePushL(contType);
	numStringsOpened++;
	hdr.SetFieldL(strP.StringF(HTTP::EContentType,RHTTPSession::GetTable()), THTTPHdrVal(contType));

	// User-Agent header
	userAgent = strP.OpenFStringL(KTestUserAgentVal());
	CleanupClosePushL(userAgent);
	numStringsOpened++;
	hdr.SetFieldL(strP.StringF(HTTP::EUserAgent,RHTTPSession::GetTable()), THTTPHdrVal(userAgent));

	// Submit the transaction and go
	iTransaction.SubmitL();
	iFailureError = KErrNone;
	CActiveScheduler::Start();

	// Check for failure error codes caught in MHFRunL
	User::LeaveIfError(iFailureError);

	// Get the completion code and inform the engine.
	iEngine->SetCurrentStatusCode(iTransaction.Response().StatusCode());

	// Set expected status HTTP/200 for tests 1-4 and 6-9; expected error -7362 (missing Host) for 5 and 10
	if (((iTestNum >= 1) && (iTestNum <= 4)) || ((iTestNum >= 6) && (iTestNum <= 9)))
		iExpectedStatusCode = 200;
	if ((iTestNum == 5) || (iTestNum == 10))
		iExpectedStatusCode = KErrHttpGeneralHeaderMissingHost;

	// Close the opened strings, transaction and session
	CleanupStack::PopAndDestroy(numStringsOpened + 2);
	}





	

