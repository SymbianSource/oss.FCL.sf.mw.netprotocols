// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "get21.h"
#include <http.h>
#include <commdbconnpref.h>

const TDesC& CHttpTestCaseGet21::TestName()
	{
	_LIT(KHeaderTestName,"Iteration 2 Get Test 21 ");
	return KHeaderTestName;
	}

CHttpTestCaseGet21::CHttpTestCaseGet21()
	{
	iTransNum=0;
	iTransSucceeded=0;
	}

void CHttpTestCaseGet21::OpenTestSessionL()
	{
	//open sessions
	iSession1.OpenL();
	CleanupClosePushL(iSession1);
	iSession2.OpenL();
	CleanupClosePushL(iSession2);

	iEngine->Utils().LogIt(_L("Sessions Created(Iteration 2 Get Test 21)"));
	iEngine->Utils().LogIt(_L("Session1 parameters: Default"));

	//Connect to Socket server 
	User::LeaveIfError(iSockServ.Connect());

	//Define preferences for connection1
	TCommDbConnPref prefs1;
	prefs1.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
	//Start Connection1
	User::LeaveIfError(iConnection1.Open(iSockServ));
	User::LeaveIfError(iConnection1.Start(prefs1));
	//Set properties for session1
	RStringPool strP1 = iSession1.StringPool();
	RHTTPConnectionInfo connInfo1 = iSession1.ConnectionInfo();
	connInfo1.SetPropertyL ( strP1.StringF(HTTP::EHttpSocketServ, RHTTPSession::GetTable() ), THTTPHdrVal (iSockServ.Handle()) );
	TInt connPtr1 = REINTERPRET_CAST(TInt, &iConnection1);
	connInfo1.SetPropertyL ( strP1.StringF(HTTP::EHttpSocketConnection, RHTTPSession::GetTable() ), THTTPHdrVal (connPtr1) );


	//Define preferences for connection2
	TCommDbConnPref prefs2;
	prefs2.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
	//Start Connection2
	User::LeaveIfError(iConnection2.Open(iSockServ));
	User::LeaveIfError(iConnection2.Start(prefs2));
	//Set properties for session2
	RStringPool strP2 = iSession2.StringPool();
	RHTTPConnectionInfo connInfo2 = iSession2.ConnectionInfo();
	connInfo2.SetPropertyL ( strP2.StringF(HTTP::EHttpSocketServ, RHTTPSession::GetTable() ), THTTPHdrVal (iSockServ.Handle()) );
	TInt connPtr2 = REINTERPRET_CAST(TInt, &iConnection2);
	connInfo2.SetPropertyL ( strP2.StringF(HTTP::EHttpSocketConnection, RHTTPSession::GetTable() ), THTTPHdrVal (connPtr2) );
	
	iEngine->Utils().LogIt(_L("Session2 parameters: Specified"));
	
	}

void CHttpTestCaseGet21::CloseTestSession()
	{
	iTransaction1.Close();
	iEngine->Utils().LogIt(_L("Transaction1 terminated\n"));
	iTransaction2.Close();
	iEngine->Utils().LogIt(_L("Transaction2 terminated\n"));
	iTransaction3.Close();
	iEngine->Utils().LogIt(_L("Transaction3 terminated\n"));
	iSession1.Close();
	iEngine->Utils().LogIt(_L("Session1 terminated"));
	iSession2.Close();
	iEngine->Utils().LogIt(_L("Session2 terminated"));
	}

void CHttpTestCaseGet21::DoRunL()
	{
	OpenTestSessionL(); 

	//Trans1 in Session1
	OpenTestTrans1L();
	iTransaction1.SubmitL();
	iTransNum++;
	//Trans2 in Session2
	OpenTestTrans2L();
	iTransaction2.SubmitL();
	iTransNum++;
	//Trans3 in Session1
	OpenTestTrans3L();
	iTransaction3.SubmitL();
	iTransNum++;

	CActiveScheduler::Start();

	CloseTestSession();
	if (iTestFail==1)
		{
		User::Leave(-1);
		}

	CleanupStack::PopAndDestroy(2); //session1 & session2
	}

void CHttpTestCaseGet21::OpenTestTrans1L()
	{
	_LIT8(KTestURIDes,"http://www.google.com/");
	TUriParser8 testURI1;
	testURI1.Parse(KTestURIDes);

	//Get the session's string pool handle;
	RStringPool strP = iSession1.StringPool();
	
	//open a GET transaction and log it
	iTransaction1 = iSession1.OpenTransactionL(testURI1, *this,strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	//Get a handle of each request and of its header 
	RHTTPRequest iMyRequest1 = iTransaction1.Request();
	RHTTPHeaders iMyHeaders1 = iMyRequest1.GetHeaderCollection(); 

	//get string from the string pool
	RStringF accept= strP.StringF(HTTP::EAnyAny,RHTTPSession::GetTable()); 

	//provide  some headers 
	THTTPHdrVal acceptHdr(accept);

	iMyHeaders1.SetFieldL(strP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),acceptHdr);

	// Log the Request parameters
	iEngine->Utils().LogIt(_L("URI: http://www.google.com/"));
	iEngine->Utils().LogIt(_L("Method:GET"));
	iEngine->Utils().LogIt(_L("Accept: */*"));
	}

void CHttpTestCaseGet21::OpenTestTrans2L()
	{
	_LIT8(KTestURIDes,"http://www.lonelyplanet.com//");  
	TUriParser8 testURI2;
	testURI2.Parse(KTestURIDes);

	//Get the session's string pool handle
	RStringPool strP = iSession2.StringPool();

	//open a GET transaction and log it
	iTransaction2 = iSession2.OpenTransactionL(testURI2, *this,strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	//Get a handle of each request and of its header 
	RHTTPRequest iMyRequest2 = iTransaction2.Request();
	RHTTPHeaders iMyHeaders2 = iMyRequest2.GetHeaderCollection(); 

	 //get string from the string pool
	RStringF accept= strP.StringF(HTTP::EAnyAny,RHTTPSession::GetTable());

	//provide  some headers 
	THTTPHdrVal acceptHdr(accept);
	iMyHeaders2.SetFieldL(strP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),acceptHdr);

	// Log the Request parameters
	iEngine->Utils().LogIt(_L("URI: http://www.lonelyplanet.com//"));
	iEngine->Utils().LogIt(_L("Method:GET"));
	iEngine->Utils().LogIt(_L("Accept: */*"));
	}

void CHttpTestCaseGet21::OpenTestTrans3L()
	{
	_LIT8(KTestURIDes,"http://www.yahoo.com/");  
	TUriParser8 testURI3;
	testURI3.Parse(KTestURIDes);

	//Get the session's string pool handle
	RStringPool strP = iSession1.StringPool();

	//open a GET transaction and log it
	iTransaction3 = iSession1.OpenTransactionL(testURI3, *this,strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

    //Get a handle of each request and of its header 
	RHTTPRequest iMyRequest3 = iTransaction3.Request();
	RHTTPHeaders iMyHeaders3 = iMyRequest3.GetHeaderCollection();

	//get string from the string pool
	RStringF accept= strP.StringF(HTTP::EAnyAny,RHTTPSession::GetTable());
	
	//provide  some headers 	
	THTTPHdrVal acceptHdr(accept);
	iMyHeaders3.SetFieldL(strP.StringF(HTTP::EAccept,RHTTPSession::GetTable()),acceptHdr);

	// Log the Request parameters
	iEngine->Utils().LogIt(_L("URI: http://www.yahoo.com/"));
	iEngine->Utils().LogIt(_L("Method:GET"));
	iEngine->Utils().LogIt(_L("Accept: */*"));

	}



TInt CHttpTestCaseGet21::MHFRunError(TInt aError, 
										RHTTPTransaction /*aTransaction*/,
										const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}


void CHttpTestCaseGet21::MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
	case THTTPEvent::EGotResponseHeaders:
		{
		// HTTP response headers have been received
		iEngine->Utils().LogIt(_L("<EGotResponseHeaders for TR %d>\n"),transCount+1);
		TInt  statusCode = aTransaction.Response().StatusCode();
		iEngine->Utils().LogIt(_L("Status Code: %d \n"),statusCode);
		DumpRespHeaders(aTransaction);
		iEngine->SetCurrentStatusCode(statusCode);
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		iEngine->Utils().LogIt(_L("<EGotResponseBodyData received for TR %d>\n"),transCount+1);
		// for each chunk of data received we have to empty the buffer before to be able to receive 
		MHTTPDataSupplier* body = aTransaction.Response().Body();
		body->ReleaseData();
		} break;
	case THTTPEvent::EResponseComplete:
		{
		// The transaction's response is complete
		iEngine->Utils().LogIt(_L("<EResponseComplete received for TR %d>\n"),transCount+1);
		
		} break;
	case THTTPEvent::ESucceeded:
		{
		transCount++;
		iTransSucceeded++;
		iEngine->Utils().LogIt(_L("<ESucceeded received %d>\n"),transCount);
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
		iEngine->PressAnyKey();
		CActiveScheduler::Stop();
		} 
		break;
		}
	}
