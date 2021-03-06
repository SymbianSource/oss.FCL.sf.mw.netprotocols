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


#include "Tc10.h"
#include <http.h>
#include "testservers.h"

_LIT8(KWapTestUrl,			"http://WapTestIP");

CHttpTestCase10::CHttpTestCase10(CScriptFile* aIniSettingsFile) : 
	CHttpTestTransBase(),
	iIniSettingsFile(aIniSettingsFile)
	{
	}

void CHttpTestCase10::DoRunL()
	{
	iSession.OpenL();
	iEngine->Utils().LogIt(_L("Session Created(10) (Default parameters)"));	
	iMyStrP = iSession.StringPool();
	
	OpenTestTransaction1L();
	iTransaction.SubmitL();
	CActiveScheduler::Start();
	CloseTestTransaction1();

	OpenTestTransaction2L();
	iTransaction.SubmitL();
	CActiveScheduler::Start();
	CloseTestTransaction2();

	iSession.Close();
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	iEngine->Utils().LogIt(_L("Session terminated"));
	}
	
TInt CHttpTestCase10::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CHttpTestCase10::DoCancel()
	{
	}

CHttpTestCase10::~CHttpTestCase10()
	{
	}	

/** increment the value of iState and set active to create call to RunL
	with new state value 
*/

/** set this object active */
void CHttpTestCase10::CompleteOwnRequest()
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
const TDesC& CHttpTestCase10::TestName()
	{
	_LIT(KHeaderTestName,"Test Case 10 ");
	return KHeaderTestName;
	}


TInt CHttpTestCase10::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
								  const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}

void CHttpTestCase10::MHFRunL(RHTTPTransaction aTransaction,
							  const THTTPEvent& aEvent)
	{
	if(aEvent.iUID!=KHTTPUid)
		{
		iEngine->Utils().LogIt(_L("\nTHTTPEvent::iUID =%d"),aEvent.iUID);
		}

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
		} break;

	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		iEngine->Utils().LogIt(_L("<EGotResponseBodyData received>\n"));
		// for each chunk of data received we have to empty the buffer before 
		// to be able to receive 
		MHTTPDataSupplier* body = aTransaction.Response().Body();
		body->ReleaseData();
		} break;
	case THTTPEvent::EResponseComplete:
		{
		// The transaction's response is complete
		iEngine->Utils().LogIt(_L("<EResponseComplete received >\n"));
		} break;
	case THTTPEvent::ESucceeded:
		{
		iEngine->Utils().LogIt(_L("<ESucceeded received>\n"));
		CActiveScheduler::Stop();
		} break;
	case THTTPEvent::EFailed:
		{
		iEngine->Utils().LogIt(_L("<EFailed received>\n"));
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

void CHttpTestCase10::OpenTestTransaction1L()
	{
	// Literals used in the function
	_LIT8(KUrlPath,				"/perl/dumpform.pl");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);

	//get the string handles this testcase needs
	RStringF textPlain = iMyStrP.StringF(HTTP::ETextPlain, RHTTPSession::GetTable());
	RStringF textHtml = iMyStrP.StringF(HTTP::ETextHtml, RHTTPSession::GetTable());
	RStringF mimeType = iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded, RHTTPSession::GetTable());
	iUnknownHeader = iMyStrP.OpenFStringL(_L8("UnknownHeader:"));

	//open  a Transaction in iSession 
	iTransaction = iSession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	//provide  some headers 
	THTTPHdrVal v2(textPlain);
	THTTPHdrVal v3(textHtml);
	THTTPHdrVal v4(6);
	THTTPHdrVal v5(mimeType);
	THTTPHdrVal v6(iUnknownHeader);

	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()),v2);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()),v3);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentLength, RHTTPSession::GetTable()),v4);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType, RHTTPSession::GetTable()), v5);
	myHeaders.SetFieldL(iMyStrP.StringF(iUnknownHeader), v6);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Accept:test/plain,text/html"));
	iEngine->Utils().LogIt(_L("Content-Type: application/x-www-form-urlencoded"));
	iEngine->Utils().LogIt(_L("Content-Length: 6"));
	iEngine->Utils().LogIt(_L("UnknownHeader:UnknownHeader:"));
	myRequest.SetBody(*this);
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCase10::CloseTestTransaction1()
	{
	iUnknownHeader.Close();
	//close Transaction 
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	}

void CHttpTestCase10::OpenTestTransaction2L()
	{
	// Literals used in the function
	_LIT8(KUrlPath,				"/perl/dumpform.pl");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);

	//get the string handles this testcase needs
	RStringF textPlain = iMyStrP.StringF(HTTP::ETextPlain, RHTTPSession::GetTable());
	RStringF textHtml = iMyStrP.StringF(HTTP::ETextHtml, RHTTPSession::GetTable());
	RStringF mimeType = iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded, RHTTPSession::GetTable());

	//open  a Transaction in iSession 
	iTransaction = iSession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();

	//provide  some headers 
	THTTPHdrVal v2(textPlain);
	THTTPHdrVal v3(textHtml);
	THTTPHdrVal v4(6);
	THTTPHdrVal v5(mimeType);

	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()),v2);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()),v3);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentLength, RHTTPSession::GetTable()),v4);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType, RHTTPSession::GetTable()), v5);

 	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Accept:test/plain,text/html"));
	iEngine->Utils().LogIt(_L("Content-Type: application/x-www-form-urlencoded"));
	iEngine->Utils().LogIt(_L("Content-Length: 6"));
	myRequest.SetBody(*this);
	CleanupStack::PopAndDestroy(newUrl8);

	//this event should be ignored by the VF
	iTransaction.SendEventL(THTTPEvent(0,0x1000A440),THTTPEvent::EIncoming, THTTPFilterHandle::EClient );
	}

void CHttpTestCase10::CloseTestTransaction2()
	{
	//close Transaction 
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	}
