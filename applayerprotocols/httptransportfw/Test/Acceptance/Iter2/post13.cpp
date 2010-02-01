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
//

#include "post13.h"
#include <http.h>

#include "csrvaddrval.h"

CHttpTestCasePost13::CHttpTestCasePost13(CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile)
	{
	iExpectedStatusCode=401;
	}

const TDesC& CHttpTestCasePost13::TestName()
	{
	_LIT(KHeaderTestName,"Iteration 2 Post Test 13 ");
	return KHeaderTestName;
	}

void CHttpTestCasePost13::OpenTestSessionL()
	{
	//open  a Session
	
	iSession.OpenL();
	iEngine->Utils().LogIt(_L("Session Created(Iteration 2 Post Test 13)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	
	InstallAuthenticationL(iSession);

	//Get the iSession'string pool handle;
	iMyStrP = iSession.StringPool();
	//open string this testcase needs

	// Literals used in the function
	_LIT8(KWapTestUrl,		"http://WapTestIP/perl/protected/dumpform.pl");
	
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);

	iTransaction = iSession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));
	iOpenTransactionCount++;

	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	//provide  some headers 

	THTTPHdrVal contentTypeHdr(iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable()));
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType, RHTTPSession::GetTable()),contentTypeHdr);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Content-Type:application/x-www-form-urlencoded "));
	myRequest.SetBody(*this);

	// for now we need to set the content length
	THTTPHdrVal contentLengthHdr(OverallDataSize());
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentLength, RHTTPSession::GetTable()), contentLengthHdr);
	CleanupStack::PopAndDestroy(newUrl8);
	} 

void CHttpTestCasePost13::CloseTestSession()
	{
	// Close strings used in this session before closing the session
	//close Transaction and session
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}


TBool CHttpTestCasePost13::GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword)
	{
	iAttempt++;
	if(iAttempt<=2)
		{
		iEngine->Utils().LogIt(_L("Supplying credentials"));
		// Can't log 8 bit data, so dump it.
		iEngine->Utils().DumpData(aURI.UriDes());
		iEngine->Utils().DumpData(aRealm.DesC());	
		iEngine->Utils().DumpData(aAuthenticationType.DesC());	
		_LIT8(KUser, "invalid");
		_LIT8(KPass, "forgot");
		RStringPool p = aRealm.Pool();
		aUsername = p.OpenStringL(KUser);
		aPassword = p.OpenStringL(KPass);
		return ETrue;
		}
	else return EFalse;
	}

void CHttpTestCasePost13::MHFRunL(RHTTPTransaction aTransaction,
								  THTTPEvent aEvent)
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
		iEngine->Utils().LogIt(_L("<ESucceeded received from the VF>\n"));
		CActiveScheduler::Stop();
		} break;
	case THTTPEvent::EFailed:
		{
		iEngine->Utils().LogIt(_L("<EFailed received from the VF( becouse status code is 4xx) but not a failure for the purpose ofthis test>\n"));
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
