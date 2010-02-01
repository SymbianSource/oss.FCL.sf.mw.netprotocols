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


#include "Tc12.h"
#include <http.h>
#include "testservers.h"

#include "csrvaddrval.h"

_LIT8(KWapTestUrl,			"http://WapTestIP");

CHttpTestCase12::CHttpTestCase12(CScriptFile* aIniSettingsFile) : 
	CHttpTestTransBase(),
	iIniSettingsFile(aIniSettingsFile)
	{
	}

void CHttpTestCase12::DoRunL()
	{	
	//open  a Session
	iSession.OpenL();
	iEngine->Utils().LogIt(_L("Session Created(TC(12)(Default parameters)"));

	//Get the iSession'string pool handle;
	iMyStrP = iSession.StringPool();
	//First test transaction
	//Prepare anything needed for a correct POST Request based on TestCase5 
	//and submit it
	OpenTestTransaction1L();
	iTransaction.SubmitL();
	CActiveScheduler::Start();
	CloseTestTransaction1();

	//Second test transaction
	OpenTestTransaction2L();
	iTransaction.SubmitL();
	CActiveScheduler::Start();
	CloseTestTransaction2();

	//Third test transaction
	OpenTestTransaction3L();
	iTransaction.SubmitL();
	CActiveScheduler::Start();
	CloseTestTransaction3();

	iSession.Close();
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	iEngine->Utils().LogIt(_L("Session terminated"));
	}
	
TInt CHttpTestCase12::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CHttpTestCase12::DoCancel()
	{
	}

CHttpTestCase12::~CHttpTestCase12()
	{
	}	

/** increment the value of iState and set active to create call to RunL
	with new state value 
*/

/** set this object active */
void CHttpTestCase12::CompleteOwnRequest()
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
const TDesC& CHttpTestCase12::TestName()
	{
	_LIT(KHeaderTestName,"Test Case 12 ");
	return KHeaderTestName;
	}


TInt CHttpTestCase12::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
								  const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}

void CHttpTestCase12::MHFRunL(RHTTPTransaction aTransaction,
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
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Some (more) body data has been received (in the HTTP response)
			iEngine->Utils().LogIt(_L("<EGotResponseBodyData received>\n"));
			DumpRespHeaders(aTransaction);
			// for each chunk of data received we have to empty the buffer 
			// before to be able to receive 
			MHTTPDataSupplier* body = aTransaction.Response().Body();
			body->ReleaseData();

	//		DumpResponseBody(aTransaction);
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			iEngine->Utils().LogIt(_L("<EResponseComplete received>\n"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			iEngine->Utils().LogIt(_L("<ESucceeded Received from the VF>\n"));
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::EFailed:
			{
			iEngine->Utils().LogIt(_L("<EFailed Received from the VF>\n"));
			CActiveScheduler::Stop();
			} break;
		case KErrHttpRequestHasBody:
			{
			iEngine->Utils().LogIt(_L("<KErrHttpGetReqWithBody  received from the VF>"));
			} break;
		case KErrHttpEntityHeaderMissingContentType:
			{
			iEngine->Utils().LogIt(_L("<KErrHttpNoEntityHeaderWithBodyContentType  received from the VF>"));
			} break;
		case KErrHttpRequestBodyMissing:
			{
			iEngine->Utils().LogIt(_L("<KErrHttpPostReqWithoutBody  received from the VF>"));
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

void CHttpTestCase12::OpenTestTransaction2L()
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

	//open string this testcase needs
	RStringF textPlain = iMyStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());
	RStringF textHtml = iMyStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF mimeType = iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable());

	//open a Transaction in iSession 
	iTransaction = iSession.OpenTransactionL(testURI, *this,iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction (%d)Created in Session"),iTransaction.Id());

	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	
	//provide  some headers 
	THTTPHdrVal v2(textPlain);
	THTTPHdrVal v3(textHtml);
	THTTPHdrVal v4(6);
	THTTPHdrVal v5(mimeType);

	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),v2);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),v3);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentLength,RHTTPSession::GetTable()),v4);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable()), v5);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Accept:test/plain,text/html"));
	iEngine->Utils().LogIt(_L("Content-Type: application/x-www-form-urlencoded"));
	iEngine->Utils().LogIt(_L("Content-Length: 6"));
	CleanupStack::PopAndDestroy(newUrl8);
	//NB: No body posted this  whill generate the EFailed message we want to test
	}

void CHttpTestCase12::CloseTestTransaction2()
	{
	//close Transaction and session
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	}

void CHttpTestCase12::OpenTestTransaction1L()
	{
	// Literals used in the function
	_LIT8(KUrlPath,				"/");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);

	//get string handles this testcase needs
	RStringF textPlain = iMyStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());
	RStringF textHtml = iMyStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());

	//open a Transaction in iSession 
	iTransaction = iSession.OpenTransactionL(testURI, *this,iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction (%d)Created in Session"),iTransaction.Id());

	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	//provide  some headers 
	THTTPHdrVal v2(textPlain);
	THTTPHdrVal v3(textHtml);
	THTTPHdrVal v4(6);

	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),v2);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),v3);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentLength,RHTTPSession::GetTable()),v4);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	iEngine->Utils().LogIt(_L("Accept:test/plain,text/html"));
	iEngine->Utils().LogIt(_L("Content-Length: 6"));
	myRequest.SetBody(*this);
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCase12::CloseTestTransaction1()
	{
	//close Transaction and session
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	}


void CHttpTestCase12::OpenTestTransaction3L()
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

	//get string handles this testcase needs
	RStringF textPlain = iMyStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());
	RStringF textHtml = iMyStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF mimeType = iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable());

	//open a Transaction in iSession 
	iTransaction = iSession.OpenTransactionL(testURI, *this,iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction (%d)Created in Session"),iTransaction.Id());

	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	//provide  some headers 
	THTTPHdrVal v2(textPlain);
	THTTPHdrVal v3(textHtml);
	THTTPHdrVal v5(mimeType);
	
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),v2);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),v3);
	//No Content-Length mandatory entity header for POST: this generates the 
	//failure;
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable()), v5);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	iEngine->Utils().LogIt(_L("Accept:test/plain,text/html"));
	iEngine->Utils().LogIt(_L("Content-Type : application/x-www-form-urlencoded"));
	myRequest.SetBody(*this);
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCase12::CloseTestTransaction3()
	{
	//close Transaction and session
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	}
