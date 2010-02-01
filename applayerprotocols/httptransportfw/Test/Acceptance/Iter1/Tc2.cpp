// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Author: Franco.Bellu@symbian.com
// 
//


#include "Tc2.h"
#include <http.h>
#include "testservers.h"

#include "csrvaddrval.h"


CHttpTestCase2::CHttpTestCase2(CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile)
	{
	iExpectedStatusCode=0;
	}

// methods derived from MHTTPDataSuplier
void CHttpTestCase2::MHFRunL(RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
			{
	case THTTPEvent::EGotResponseHeaders:
		{
		iEngine->Utils().LogIt(_L("<EGotResponseHeaders received>\n"));
		DumpRespHeaders(aTransaction);
		iEngine->SetCurrentStatusCode(aTransaction.Response().StatusCode());
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		iEngine->Utils().LogIt(_L("<EGotResponseBodyData received>\n"));
		DumpResponseBody(aTransaction);
		} break;
	case THTTPEvent::EResponseComplete:
		{
		// The transaction's response is complete
		iEngine->Utils().LogIt(_L("<EResponseComplete received>\n"));
		} break;
	default:
		{
		iTestFail=1;
		iEngine->Utils().LogIt(_L("<unrecognised event>\n %d"),aEvent.iStatus);
		iEngine->PressAnyKey();
		CActiveScheduler::Stop();
		} 
		break;
		}
	}

TInt CHttpTestCase2::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
								 const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

void CHttpTestCase2::DoRunL()
	{
	// Literals used in the function
	_LIT8(KWapTestUrl,			"http://WapTestIP/perl/dumpform.pl");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);

	//open a Session
	RHTTPSession mySession;
	mySession.OpenL();
	CleanupClosePushL(mySession);
	iEngine->Utils().LogIt(_L("Session Created (TC2)"));
	iEngine->Utils().LogIt(_L("Session (Default) parameters: RHTTPProxy aProxy = RHTTPProxy(), MHTTPSessionCallback* aCallback = NULL"));

	// Get the mySession'string pool handle;
	iMyStrP = mySession.StringPool();

	//get strings used in this test
	RStringF textPlain = iMyStrP.StringF(HTTP::ETextPlain, RHTTPSession::GetTable());
	RStringF textHtml = iMyStrP.StringF(HTTP::ETextHtml, RHTTPSession::GetTable());
	RStringF mimeType = iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded, RHTTPSession::GetTable());

	// Create , open and push in the CS a Transaction in mySession 
	RHTTPTransaction myTransaction;
	myTransaction = mySession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	CleanupClosePushL(myTransaction);
	iEngine->Utils().LogIt(_L("Transaction Created in mySession"));

	// Get a handle of the request in myTransaction
	RHTTPRequest myRequest = myTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	// provide  some headers 
	THTTPHdrVal v2(textPlain);
	THTTPHdrVal v3(textHtml);
	THTTPHdrVal v4(6);
	THTTPHdrVal v5(mimeType);

	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()),v2 );
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()),v3);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentLength, RHTTPSession::GetTable()), v4);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType, RHTTPSession::GetTable()), v5);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Accept:test/plain,text/html"));
	iEngine->Utils().LogIt(_L("Content-Type: application/x-www-form-urlencoded"));
	iEngine->Utils().LogIt(_L("Content-Length: 4"));
	myRequest.SetBody(*this);
	iEngine->Utils().LogIt(_L("Transaction terminated"));

	// Close strings used in this session before closing the session
	//close transaction and session
	myTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated"));
	mySession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	CleanupStack::Pop(2); // mySession,myTansaction
	CleanupStack::PopAndDestroy(newUrl8);
	}
	
TInt CHttpTestCase2::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}
void CHttpTestCase2::DoCancel()
	{
	}

CHttpTestCase2::~CHttpTestCase2()
	{
	}	

/** set this object active */
void CHttpTestCase2::CompleteOwnRequest()
	{
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);	
	if (!IsActive())
		SetActive();
	}

/** sets base test name.
	@param void
	@return string - the test name 
*/

const TDesC& CHttpTestCase2::TestName()
	{
	_LIT(KHeaderTestName,"Test Case 2 ");
	return KHeaderTestName;
	}
