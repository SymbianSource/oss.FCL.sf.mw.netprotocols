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
// Author: Leon.Clarke@symbian.com
// 
//


#include "Tc8.h"
#include <http.h>
#include "testservers.h"

CHttpTestCase8::CHttpTestCase8(CScriptFile* aIniSettingsFile) : 
	CHttpTestTransBase(),
	iIniSettingsFile(aIniSettingsFile)
	{
	}

void CHttpTestCase8::DoRunL()
	{
	// Literals used in the function
	_LIT8(KWapTestUrl,			"http://WapTestIP/perl/protected/dumpform.pl");

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
	iEngine->Utils().LogIt(_L("Session Created(TC8)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	// Install the authorisation plugin.
	InstallAuthenticationL(mySession);
	//Get the mySession'string pool handle;
	iMyStrP = mySession.StringPool();
	//Create , open and push in the CS a Transaction in mySession 
	RHTTPTransaction myTransaction;
	myTransaction = mySession.OpenTransactionL(testURI, *this, mySession.StringPool().StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	CleanupClosePushL(myTransaction);
	iEngine->Utils().LogIt(_L("Transaction Created in mySession"));	
	//Get a handle of the request in myTransaction
	RHTTPRequest myRequest = myTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	//  provide  some headers 
	THTTPHdrVal v2(iMyStrP.OpenFStringL(_L8("text/plain")));
	THTTPHdrVal v3(iMyStrP.OpenFStringL(_L8("text/html")));
	THTTPHdrVal v4(6);
	myHeaders.SetFieldL(iMyStrP.OpenFStringL(_L8("Accept")),v2 );
	myHeaders.SetFieldL(iMyStrP.OpenFStringL(_L8("Content-Type")), v3);
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentLength, RHTTPSession::GetTable()),v4);
	iEngine->Utils().LogIt(_L("Testing Authentication. Method:Post"));
	myRequest.SetBody(*this);
	myTransaction.SubmitL();
	CActiveScheduler::Start();
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
	
TInt CHttpTestCase8::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CHttpTestCase8::DoCancel()
	{
	}

CHttpTestCase8::~CHttpTestCase8()
	{
	}	

/** set this object active */
void CHttpTestCase8::CompleteOwnRequest()
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
const TDesC& CHttpTestCase8::TestName()
	{
	_LIT(KHeaderTestName,"Test Case 8 ");
	return KHeaderTestName;
	}

TInt CHttpTestCase8::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
								 const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}

void CHttpTestCase8::MHFRunL(RHTTPTransaction aTransaction,
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
		if (iExpectedStatusCode != statusCode)
			{
			iTestFail=1;
			iEngine->Utils().LogIt(_L("<The Test will fail :The expected status code doesn't match the actual status code received>\n"));
			}
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		iEngine->Utils().LogIt(_L("<Got some body data>\n"));
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
		iEngine->Utils().LogIt(_L("<ESucceeded received from the VF>\n"));
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

TBool CHttpTestCase8::GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword)
	{
	iEngine->Utils().LogIt(_L("Supplying credentials"));
	iEngine->Utils().DumpData(aURI.UriDes());
	iEngine->Utils().DumpData(aRealm.DesC());
	iEngine->Utils().DumpData(aAuthenticationType.DesC());
	_LIT8(KScooby, "scooby");
	_LIT8(KDoo, "doo");
	RStringPool p = aRealm.Pool();
	aUsername = p.OpenStringL(KScooby);
	aPassword = p.OpenStringL(KDoo);
	return ETrue;
	}
