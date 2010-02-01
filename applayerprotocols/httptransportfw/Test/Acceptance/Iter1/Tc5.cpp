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


#include "Tc5.h"
#include <http.h>
#include "testservers.h"

CHttpTestCase5::CHttpTestCase5(CScriptFile* aIniSettingsFile) :
	CHttpTestTransBase(),
	iIniSettingsFile(aIniSettingsFile)
	{
	}

void CHttpTestCase5::DoRunL()
	{
	// Literals used in the function
	_LIT8(KWapTestUrl,			"http://WapTestIP/cgi-bin/testcase5.cgi");

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
	iEngine->Utils().LogIt(_L("Session Created(TC5)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	//Get the mySession'string pool handle;
	iMyStrP = mySession.StringPool();
	//open string this testcase needs
	RStringF mimeType = iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded, RHTTPSession::GetTable());
	RStringF textPlain = iMyStrP.StringF(HTTP::ETextPlain, RHTTPSession::GetTable());
	RStringF textHtml = iMyStrP.StringF(HTTP::ETextHtml, RHTTPSession::GetTable());
	
	//Create , open and push in the CS a Transaction in mySession 
	RHTTPTransaction myTransaction;
	myTransaction = mySession.OpenTransactionL(testURI, *this, mySession.StringPool().StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	CleanupClosePushL(myTransaction);
	iEngine->Utils().LogIt(_L("Transaction Created in mySession"));
	//Get a handle of the request in myTransaction
	RHTTPRequest myRequest = myTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	// provide  some headers 
	
	THTTPHdrVal v2(textPlain);
	THTTPHdrVal v3(textHtml);
	THTTPHdrVal v4(6);
	THTTPHdrVal v6(mimeType);

	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()), v2);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()),v3);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentLength, RHTTPSession::GetTable()), v4);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType, RHTTPSession::GetTable()), v6);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Accept:test/plain,text/html"));
	iEngine->Utils().LogIt(_L("Content-Type: application/vnd.syncml-xml"));
	iEngine->Utils().LogIt(_L("Content-Length: 6"));	
	myRequest.SetBody(*this);
	myTransaction.SubmitL();
	CActiveScheduler::Start();

	//close Transaction and session
	myTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	mySession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	CleanupStack::Pop(2); // mySession,myTansaction
	CleanupStack::PopAndDestroy(newUrl8);
	}
	
TInt CHttpTestCase5::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CHttpTestCase5::DoCancel()
	{
	}

CHttpTestCase5::~CHttpTestCase5()
	{
	}	

/** set this object active */
void CHttpTestCase5::CompleteOwnRequest()
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
const TDesC& CHttpTestCase5::TestName()
	{
	_LIT(KHeaderTestName,"Test Case 5 ");
	return KHeaderTestName;
	}

TInt CHttpTestCase5::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
								 const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}

void CHttpTestCase5::MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
			{
	case THTTPEvent::EGotResponseHeaders:
		{
		// HTTP response headers have been received
		iEngine->Utils().LogIt(_L("<EGotResponseHeaders>\n"));
		TInt  statusCode = aTransaction.Response().StatusCode();
		iEngine->Utils().LogIt(_L("Status Code: %d \n"),statusCode);
		DumpRespHeaders(aTransaction);
		iEngine->SetCurrentStatusCode(statusCode);
		//define 2 RStringF : I want to find a header-field header-vaulue match in the response headers
		_LIT8(KContentTypeValSyncMl, "application/vnd.syncml-xml"); // ota content type header
  		RStringF contTypeStr = iMyStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable());
		RStringF syncmlStr = iMyStrP.OpenFStringL(KContentTypeValSyncMl);
		// get  a handle for the headers
		RHTTPHeaders hdr =aTransaction.Response().GetHeaderCollection();
		//parse the headers and look for application/vnd.syncml-xml header
		THTTPHdrVal hVal;
		if(hdr.GetField(contTypeStr,0,hVal)== KErrNone) 
			{// Content-Type header is present
			if (hVal.StrF()==syncmlStr)
				iEngine->Utils().LogIt(_L("\nOk header application/vnd.syncml-xml found"));
			else 
				iEngine->Utils().LogIt(_L("\nThe server doesn't support the application/vnd.syncml-xml header"));
			}
		syncmlStr.Close();
		if (iExpectedStatusCode != statusCode)
			{
			iTestFail=1;
			iEngine->Utils().LogIt(_L("<The Test will fail :The expected status code doesn't match the actual status code received>\n"));
			}
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
		iEngine->Utils().LogIt(_L("<EResponseComplete received >\n"));
		} break;
	case THTTPEvent::ESucceeded:
		{
		iEngine->Utils().LogIt(_L("<ESucceeded received >\n"));
		CActiveScheduler::Stop();
		} break;
	default:
		{
		iEngine->Utils().LogIt(_L("<unrecognised event>\n %d"),aEvent.iStatus);
		if (aEvent.iStatus < THTTPEvent::ERedirectedPermanently)
			{
			iTestFail=1;
			iEngine->Utils().LogIt(_L("Test Failed: The event received was not expected and it's not just a warning\n"));
			iEngine->PressAnyKey();
			CActiveScheduler::Stop();
			}
		} 
		break;
		}
	}
