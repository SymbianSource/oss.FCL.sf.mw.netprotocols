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


#include "Tc3.h"
#include <http.h>
#include "testservers.h"

#include "csrvaddrval.h"

void CHttpTestCase3::DoRunL()
	{
	// Literals used in the function
	_LIT(KWapTestUrl,			"http://WapTestIP/perl/dumpform.pl");

	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl());
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	TUriParser8 testURI; 
	testURI.Parse(newUrlPtr8);
	
	//open a Session
	RHTTPSession mySession;
	mySession.OpenL();
	CleanupClosePushL(mySession);
	iEngine->Utils().LogIt(_L("Session Created(TC3)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));

	//Get the mySession'string pool handle;
	iMyStrP = mySession.StringPool();

	//get strings this testcase needs
	RStringF textPlain = iMyStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());
	RStringF textHtml = iMyStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF mimeType = iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable());

	//Open a Transaction in mySession 
	RHTTPTransaction myTransaction;
	myTransaction = mySession.OpenTransactionL(testURI, *this,iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	CleanupClosePushL(myTransaction);
	iEngine->Utils().LogIt(_L("Transaction Created in mySession"));
	
	//Get a handle of the request in myTransaction
	RHTTPRequest myRequest = myTransaction.GetRequest();
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

	iEngine->Utils().LogIt(KWapTestUrl());
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Accept:test/plain,text/html"));
	iEngine->Utils().LogIt(_L("Content-Type: application/x-www-form-urlencoded"));
	iEngine->Utils().LogIt(_L("Content-Length: 6"));
		
	myRequest.SetBody(*this);
	myTransaction.SubmitL();
	CActiveScheduler::Start();

	//close transaction and session
	myTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	mySession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	CleanupStack::Pop(3, newUrl8); // mySession,myTansaction
	}
	
TInt CHttpTestCase3::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CHttpTestCase3::DoCancel()
	{
	}

CHttpTestCase3::~CHttpTestCase3()
	{
	}	

/** set this object active */
void CHttpTestCase3::CompleteOwnRequest()
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
const TDesC& CHttpTestCase3::TestName()
	{
	_LIT(KHeaderTestName,"Test Case 3 ");
	return KHeaderTestName;
	}

TInt CHttpTestCase3::MHFRunError(TInt aError, 
										RHTTPTransaction /*aTransaction*/,
										const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}

void CHttpTestCase3::MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
			{
	case THTTPEvent::EGotResponseHeaders:
		{
		// HTTP response headers have been received
		iEngine->Utils().LogIt(_L("<EGotResponseHeaders received>\n"));
		DumpRespHeaders(aTransaction);
		iEngine->SetCurrentStatusCode(aTransaction.GetResponse().Status());
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		iEngine->Utils().LogIt(_L("<EGotResponseBodyData received>\n"));
		// for each chunk of data received we have to empty the buffer before to be able to receive 
		MHTTPDataSupplier& body = aTransaction.GetResponse().GetBody();
		body.ReleaseData();
		} break;
	case THTTPEvent::EResponseComplete:
		{
		// The transaction's response is complete
		iEngine->Utils().LogIt(_L("<EResponseComplete received>\n"));
		} break;
	case THTTPEvent::ESucceeded:
		{
		iEngine->Utils().LogIt(_L("<ESucceeded received>\n"));
		CActiveScheduler::Stop();
		} break;
	default:
		{
		iEngine->Utils().LogIt(_L("<unrecognised event>\n %d"),aEvent.iStatus);
		iEngine->Utils().LogIt(_L("Test Failed\n"));
		iEngine->PressAnyKey();
		CActiveScheduler::Stop();
		} 
		break;
		}
	}

