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

#include "EofTransactionResubmission.h"
#include <http.h>
#include "csrvaddrval.h"

// Literals used in the file
_LIT8(KWapTestUrl,			"http://WapTestName");

/**
	This function returns the name of the test.
*/
const TDesC& CEofTransactionResubmission::TestName()
	{
	_LIT(KHeaderTestName,"Resubmission due to EOF error returned from socket");
	return KHeaderTestName;
	}

CEofTransactionResubmission::CEofTransactionResubmission(CScriptFile* aIniSettingsFile) 
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
void CEofTransactionResubmission::OpenTestSessionL()
	{
	//open a default Session
	iSession.OpenL();
	iSetCredentials=ETrue;

	//Logs 
	iEngine->Utils().LogIt(_L("Session Created(Iteration 2 Get Test 17)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	
	//Get the iSession'string pool handle;
	iMyStrP = iSession.StringPool();
	}

/**
	This function closes all the transactions and sessions opened.
*/
void CEofTransactionResubmission::CloseTestSession()
	{
	//Close all the 8 Transactions opened in the DoRunL()
	iTransaction1.Close();
	iEngine->Utils().LogIt(_L("Transaction1 terminated\n"));
	iTransaction2.Close();
	iEngine->Utils().LogIt(_L("Transaction2 terminated\n"));
	//and close the session
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}

/**
	3 Digest authentication transactions are executed from this function.

	@leave		Leaves with a standard error.
*/
void CEofTransactionResubmission::DoRunL()
	{
	OpenTestSessionL(); 

	//Trans1
	OpenTestTrans1L();
	iTransaction1.SubmitL();
	iTransNum++;
	i2ndTrans=EFalse;

	CActiveScheduler::Start();

	CloseTestSession();
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	}

void CEofTransactionResubmission::OpenTestTrans1L()
	{
	// Literals used in the function
	_LIT8(KUrlPath,				"/stats.html");

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
	DisablePipeliningL(iTransaction1);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CEofTransactionResubmission::OpenTestTrans2L()
	{
	// Literals used in the function
	_LIT8(KUrlPath,				"/manual/windows.html");

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
	DisablePipeliningL(iTransaction2);

	// Log the Request parameters
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

TInt CEofTransactionResubmission::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
											  const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}

void CEofTransactionResubmission::MHFRunL(RHTTPTransaction aTransaction,
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
		// for each chunk of data received we have to empty the buffer 
		// before to be able to receive 
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

		if (!i2ndTrans)
			{
			User::After(20*1000000);
			OpenTestTrans2L();
			iTransaction2.SubmitL();
			iTransNum++;
			i2ndTrans=ETrue;
			}

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


void CEofTransactionResubmission::DisablePipeliningL(RHTTPTransaction trans)
	{
	RStringPool strPool = iSession.StringPool();

	RHTTPTransactionPropertySet transactionPropertySet = trans.PropertySet();
	RStringF name = strPool.StringF(HTTP::EHttpPipelining, RHTTPSession::GetTable());
	RStringF value = strPool.StringF(HTTP::EDisablePipelining, RHTTPSession::GetTable());
	transactionPropertySet.SetPropertyL(name,value);
	}
