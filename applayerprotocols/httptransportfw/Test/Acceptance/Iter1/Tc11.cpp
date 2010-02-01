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


#include "Tc11.h"
#include <http.h>
#include "testservers.h"

#include "csrvaddrval.h"

_LIT8(KWapTestUrl,			"http://WapTestIP");

CHttpTestCase11::CHttpTestCase11(CScriptFile* aIniSettingsFile) : 
	CHttpTestTransBase(),
	iIniSettingsFile(aIniSettingsFile)
	{
	}

void CHttpTestCase11::DoRunL()
	{
	//open sessions
	OpenTestSession1L();
	OpenTestSession2L();
	
	// submit a request with a Response Header ( Server: )
	iTransaction1.SubmitL();
	CActiveScheduler::Start();
	// submit a Wrong Request: GET  with a Entity Header  Contrent-Length: 6
	iTransaction2.SubmitL();
	CActiveScheduler::Start();
	//close anything opend 
	CloseTestSession1();
	CloseTestSession2();
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	}
	
TInt CHttpTestCase11::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CHttpTestCase11::DoCancel()
	{
	}

CHttpTestCase11::~CHttpTestCase11()
	{
	}	

/** increment the value of iState and set active to create call to RunL
	with new state value 
*/

/** set this object active */
void CHttpTestCase11::CompleteOwnRequest()
	{
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);	
	if (!IsActive())
		{
		SetActive();
		}
	}

/** sets base test name.
	@param void
	@return string - the test name 
*/
const TDesC& CHttpTestCase11::TestName()
	{
	_LIT(KHeaderTestName,"Test Case 11 ");
	return KHeaderTestName;
	}


TInt CHttpTestCase11::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
								  const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}

void CHttpTestCase11::MHFRunL(RHTTPTransaction aTransaction,
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
			// for each chunk of data received we have to empty the buffer before to be able to receive 
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
		case KErrHttpInvalidHeaderInRequest:
			{
			iEngine->Utils().LogIt(_L("<KErrHttpWrongRespHeaderInReqServer  received from the VF>"));
			THTTPHdrVal hVal;

			//check the wrong header has been removed
			if(aTransaction.Request().GetHeaderCollection().GetField(iMyStrP.StringF(HTTP::EServer,RHTTPSession::GetTable()),0,hVal) == KErrNone)
				{
				iEngine->Utils().LogIt(_L("***TEST FAILED***The erroneous header has NOT been removed"));
				}
			else
				{
				iEngine->Utils().LogIt(_L("<The erroneous header has  been removed>"));
				}
			} break;
		case KErrHttpEntityHeaderMissingContentType:
			{
			iEngine->Utils().LogIt(_L("<KErrHttpNoEntityHeaderWithBodyContentType  received from the VF>"));
			THTTPHdrVal hVal;
			if(aTransaction.Request().GetHeaderCollection().GetField(iMyStrP.StringF(HTTP::EContentLength,RHTTPSession::GetTable()),0,hVal) == KErrNone)
				{
				iEngine->Utils().LogIt(_L("***TEST FAILED***The erroneous header has NOT been removed"));
				}
			else
				{
				iEngine->Utils().LogIt(_L("<The erroneous header has  been removed>"));
				}
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

void CHttpTestCase11::OpenTestSession1L()
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

	//open  a Session
	iSession1.OpenL();
	iEngine->Utils().LogIt(_L("Session Created(TC11.1)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	//Get the iSession'string pool handle;
	iMyStrP = iSession1.StringPool();
	//open string this testcase needs
	RStringF textPlain = iMyStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());
	RStringF textHtml = iMyStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	iServer = iMyStrP.OpenFStringL(_L8("11.111.1.1")); //any string
	RStringF mimeType = iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable());
	//Create , open a Transaction in iSession 
	iTransaction1 = iSession1.OpenTransactionL(testURI, *this,iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));
	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction1.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	//provide  some headers 
	THTTPHdrVal v2(textPlain);
	THTTPHdrVal v3(textHtml);
	THTTPHdrVal v4(6);
	THTTPHdrVal v5(mimeType);
	THTTPHdrVal v6(iServer);

	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),v2);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),v3);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentLength,RHTTPSession::GetTable()),v4);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable()), v5);
	// add a response header to test the VF behavior

	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EServer,RHTTPSession::GetTable()), v6);
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Accept:test/plain,text/html"));
	iEngine->Utils().LogIt(_L("Content-Type: application/x-www-form-urlencoded"));
	iEngine->Utils().LogIt(_L("Content-Length: 6"));
	iEngine->Utils().LogIt(_L("Server: 11.111.1.1"));
	myRequest.SetBody(*this);
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCase11::CloseTestSession1()
	{
	// Close strings used in this session before closing the session
	iServer.Close();
	//close Transaction and session
	iTransaction1.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	iSession1.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}

void CHttpTestCase11::OpenTestSession2L()
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

	//open  a Session
	iSession2.OpenL();
	iEngine->Utils().LogIt(_L("Session Created(TC(11.2)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	//Get the iSession'string pool handle;
	iMyStrP = iSession2.StringPool();
	//open string this testcase needs
	RStringF textPlain = iMyStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());
	RStringF textHtml = iMyStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	//Create , open a Transaction in iSession 
	iTransaction2 = iSession2.OpenTransactionL(testURI, *this,iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));
	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction2.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	//provide  some headers 
	THTTPHdrVal v2(textPlain);
	THTTPHdrVal v3(textHtml);
	THTTPHdrVal v4(6);

	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),v2);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),v3);
	// this erroneous header is to test the VF
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentLength,RHTTPSession::GetTable()),v4);

 	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	iEngine->Utils().LogIt(_L("Accept:test/plain,text/html"));
	iEngine->Utils().LogIt(_L("Content-Length: 6"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCase11::CloseTestSession2()
	{
	// Close strings used in this session before closing the session
	//close Transaction and session
	iTransaction2.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	iSession2.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}
