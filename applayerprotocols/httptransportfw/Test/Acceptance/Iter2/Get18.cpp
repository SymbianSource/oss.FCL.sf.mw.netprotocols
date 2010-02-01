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

#include "Get18.h"
#include <http.h>

#include "csrvaddrval.h"

_LIT8(KWapTestUrl,			"http://WapTestIP/");

const TDesC& CHttpTestCaseGet18::TestName()
	{
	_LIT(KHeaderTestName,"Iteration 2 Get Test 18 ");
	return KHeaderTestName;
	}

CHttpTestCaseGet18::CHttpTestCaseGet18(CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile)
	{
	iTransNum=0;
	iTransSucceeded=0;
	}

void CHttpTestCaseGet18::OpenTestSessionL()
	{
	//open a default Session
	iSession.OpenL();

	//Logs 
	iEngine->Utils().LogIt(_L("Session Created(Iteration 2 Get Test 18)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	
	//Get the iSession'string pool handle;
	iMyStrP = iSession.StringPool();
	
	// for the purpose of this test set the http version to  HTTP/1.0 
	//	(non persistent connectionas default)
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
	THTTPHdrVal httpver(iMyStrP.StringF(HTTP::EHTTPVersion, RHTTPSession::GetTable()));
	connInfo.SetPropertyL(httpver, iMyStrP.StringF(HTTP::EHttp10, RHTTPSession::GetTable()));
	}

void CHttpTestCaseGet18::CloseTestSession()
	{
	//Close all the 8 Transactions opened in the DoRunL()
	iTransaction1.Close();
	iEngine->Utils().LogIt(_L("Transaction1 terminated\n"));
	iTransaction2.Close();
	iEngine->Utils().LogIt(_L("Transaction2 terminated\n"));
	iTransaction3.Close();
	iEngine->Utils().LogIt(_L("Transaction3 terminated\n"));
	iTransaction4.Close();
	iEngine->Utils().LogIt(_L("Transaction4 terminated\n"));
	iTransaction5.Close();
	iEngine->Utils().LogIt(_L("Transaction5 terminated\n"));
	iTransaction6.Close();
	iEngine->Utils().LogIt(_L("Transaction6 terminated\n"));
	iTransaction7.Close();
	iEngine->Utils().LogIt(_L("Transaction7 terminated\n"));
	iTransaction8.Close();
	iEngine->Utils().LogIt(_L("Transaction8 terminated\n"));
	//and close the session
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}

void CHttpTestCaseGet18::DoRunL()
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

	//Trans4
	OpenTestTrans4L();
	iTransaction4.SubmitL();
	iTransNum++;
	//Trans5
	OpenTestTrans5L();
	iTransaction5.SubmitL();
	iTransNum++;

	//Trans6
	OpenTestTrans6L();
	iTransaction6.SubmitL();
	iTransNum++;

	//Trans7
	OpenTestTrans7L();
	iTransaction7.SubmitL();
	iTransNum++;

	//Trans8
	OpenTestTrans8L();
	iTransaction8.SubmitL();
	iTransNum++;

	CActiveScheduler::Start();
	
	CloseTestSession();
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	}

void CHttpTestCaseGet18::OpenTestTrans1L()
	{
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

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

void CHttpTestCaseGet18::OpenTestTrans2L()
	{
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

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

void CHttpTestCaseGet18::OpenTestTrans3L()
	{
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

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

void CHttpTestCaseGet18::OpenTestTrans4L()
	{
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	TUriParser8 testURI1; 
	testURI1.Parse(newUrlPtr8);
	
	//open a GET transaction and log it
	iTransaction4 = iSession.OpenTransactionL(testURI1, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	// Log the Request parameters
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCaseGet18::OpenTestTrans5L()
	{
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	TUriParser8 testURI1; 
	testURI1.Parse(newUrlPtr8);
	
	//open a GET transaction and log it
	iTransaction5 = iSession.OpenTransactionL(testURI1, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	// Log the Request parameters
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCaseGet18::OpenTestTrans6L()
	{
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	TUriParser8 testURI1; 
	testURI1.Parse(newUrlPtr8);
	
	//open a GET transaction and log it
	iTransaction6 = iSession.OpenTransactionL(testURI1, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	// Log the Request parameters
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCaseGet18::OpenTestTrans7L()
	{
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	TUriParser8 testURI1; 
	testURI1.Parse(newUrlPtr8);
	
	//open a GET transaction and log it
	iTransaction7 = iSession.OpenTransactionL(testURI1, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	// Log the Request parameters
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCaseGet18::OpenTestTrans8L()
	{
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	TUriParser8 testURI1; 
	testURI1.Parse(newUrlPtr8);
	
	//open a GET transaction and log it
	iTransaction8 = iSession.OpenTransactionL(testURI1, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	// Log the Request parameters
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

TInt CHttpTestCaseGet18::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
									 const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}

void CHttpTestCaseGet18::MHFRunL(RHTTPTransaction aTransaction,
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
		if(statusCode!=200)
			{
			iTestFail=1;
			iEngine->Utils().LogIt(_L("<The test willl fail: not a 200 status code received>\n"));
			}
		}break;
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
		transCount++;
		iTestFail=1;
		iEngine->Utils().LogIt(_L("<The Test will fail:1 Transaction  has failed"));
		iEngine->Utils().LogIt(_L("<EFailed received from the VF>\nThe Test has FAILED"));
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
