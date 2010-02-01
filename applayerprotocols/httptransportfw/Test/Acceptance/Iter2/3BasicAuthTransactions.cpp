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

#include "3BasicAuthTransactions.h"
#include <http.h>
#include "csrvaddrval.h"

// Literals used in the file
_LIT8(KWapTestUrl,			"http://WapTestName");

/**
	This function returns the name of the test.
*/
const TDesC& C3BasicAuthTransactions::TestName()
	{
	_LIT(KHeaderTestName, "Basic Authentication Test, sending three transactions");
	return KHeaderTestName;
	}

C3BasicAuthTransactions::C3BasicAuthTransactions(CScriptFile* aIniSettingsFile)
	:	iIniSettingsFile(aIniSettingsFile)
	{
	iTransNum=0;
	iTransSucceeded=0;
	iExpectedStatusCode=200;
	}

/**
	This function is opens a session for the test being executed.

	@leave		Leaves with a standard error.
*/
void C3BasicAuthTransactions::OpenTestSessionL()
	{
	//open a default Session
	iSession.OpenL();
	iSetCredentials=ETrue;
	InstallAuthenticationL(iSession);

	//Logs 
	iEngine->Utils().LogIt(_L("Session Created(Iteration 2 Get Test 17)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	
	//Get the iSession'string pool handle;
	iMyStrP = iSession.StringPool();
	}

/**
	This function closes all the transactions and sessions opened.
*/
void C3BasicAuthTransactions::CloseTestSession()
	{
	//Close all the 8 Transactions opened in the DoRunL()
	iTransaction1.Close();
	iEngine->Utils().LogIt(_L("Transaction1 terminated\n"));
	iTransaction2.Close();
	iEngine->Utils().LogIt(_L("Transaction2 terminated\n"));
	iTransaction3.Close();
	iEngine->Utils().LogIt(_L("Transaction3 terminated\n"));
	//and close the session
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}

/**
	3 basic authentication transactions are executed from this function.

	@leave		Leaves with a standard error.
*/
void C3BasicAuthTransactions::DoRunL()
	{
	OpenTestSessionL(); 

	//Trans1
	OpenTestTrans1L();
	iTransaction1.SubmitL();
	iTransNum++;
	//Trans2
	OpenTestTrans2L();
	iTransaction2.SubmitL();
	iTransNum++;

	//Trans3
	OpenTestTrans3L();
	iTransaction3.SubmitL();
	iTransNum++;

	CActiveScheduler::Start();
	
	CloseTestSession();
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	}

void C3BasicAuthTransactions::OpenTestTrans1L()
	{
	// Literals used in the function
	_LIT8(KUrlPath,				"/perl/protected/dumpform.pl");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI1; 
	testURI1.Parse(newUrlPtr8);

	//open a GET transaction and log it
	iTransaction1 = iSession.OpenTransactionL(testURI1, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	// Log the Request parameters
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void C3BasicAuthTransactions::OpenTestTrans2L()
	{
	// Literals used in the function
	_LIT8(KUrlPath,				"/http_tests/rc/401/private/index.html");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI1; 
	testURI1.Parse(newUrlPtr8);

	//open a GET transaction and log it
	iTransaction2 = iSession.OpenTransactionL(testURI1, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	// Log the Request parameters
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void C3BasicAuthTransactions::OpenTestTrans3L()
	{
	// Literals used in the function
	_LIT8(KUrlPath,				"/perl/protected/dumpform.pl");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI1; 
	testURI1.Parse(newUrlPtr8);

	//open a GET transaction and log it
	iTransaction3 = iSession.OpenTransactionL(testURI1, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	// Log the Request parameters
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}


TInt C3BasicAuthTransactions::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
										  const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}

void C3BasicAuthTransactions::MHFRunL(RHTTPTransaction aTransaction,
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
		iEngine->Utils().LogIt(_L("<EGotResponseBodyData received>\n"));
		// for each chunk of data received we have to empty the buffer before to be able to receive 
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
		transCount++;
		iTransSucceeded++;
		iEngine->Utils().LogIt(_L("<ESucceeded received >\n"));
		if (transCount<=iTransNum)
			{
			if(iTransSucceeded==iTransNum)
				{
				iEngine->Utils().LogIt(_L("The test has PASSED"));
				CActiveScheduler::Stop();
				} 
			}
		else 
			{
			iTestFail=1;
			iEngine->Utils().LogIt(_L("<The Test will fail:less than %d ESucceeded have been received>\n"),iTransNum);		
			}
		} break;
	case THTTPEvent::EFailed:
		{
		TInt  statusCode = aTransaction.Response().StatusCode();
		iEngine->Utils().LogIt(_L("Status Code: %d \n"),statusCode);
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


TBool C3BasicAuthTransactions::GetCredentialsL(const TUriC8& /*aURI*/, RString aRealm, RStringF /*aAuthenticationType*/, RString& aUsername, RString& aPassword)
	{
	if (!iSetCredentials)
		return EFalse;

	iSetCredentials =EFalse; //resetting

	_LIT8(KUsername, "scooby");
	_LIT8(KPassword, "doo");

	RStringPool stringPool = aRealm.Pool();
	aUsername = stringPool.OpenStringL(KUsername);
	aPassword = stringPool.OpenStringL(KPassword);
	return ETrue;
	}	
