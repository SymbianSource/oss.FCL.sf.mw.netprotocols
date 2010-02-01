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

#include "T_SocketShutdownMode.h"

// User include
#include "csrvaddrval.h"

_LIT8(KTxtUri1,				"http://www.symbian.com");
_LIT8(KTxtUri2,				"http://www.nokia.com");
_LIT8(KTxtUri3,				"http://WapTestName");
_LIT8(KTxtUri4,				"http://SecureServerName");

const TInt KNumOfTransLoops = 2;

// CSocketShutdownModeTest-----------------------------------

/**
	This function is called to create a new instance of the class 
	CTextModeTestCookies

	@param aImmediateShutdown	This indicates to close the socket immediately
	@param aInterrupt			The interrupt used to close the socket.
	@param aIniSettingsFile		The script file that holds the sections
							    from which the value has to be retrieved
	@leave 						KErrNoMemory if there is insufficient memory
*/
CSocketShutdownModeTest* CSocketShutdownModeTest::NewL(TBool aImmediateShutdown, TBool aInterrupt, CScriptFile* aIniSettingsFile)
	{
	CSocketShutdownModeTest* self = new(ELeave)CSocketShutdownModeTest(aImmediateShutdown, aInterrupt, aIniSettingsFile);
	return self;
	}

/**
	Destructor
*/
CSocketShutdownModeTest::~CSocketShutdownModeTest()
	{
	iTransArray.Close();
	}

CSocketShutdownModeTest::CSocketShutdownModeTest(TBool aImmediateShutdown, TBool aInterrupt, CScriptFile* aIniSettingsFile)
	: iIniSettingsFile(aIniSettingsFile), 
	  iImmediateShutdown(aImmediateShutdown), 
	  iInterruptClose(aInterrupt)
	{
	// does nothing
	}

/**
	This function is called to initiate the tests.
	It may leave with one of the system-wide error codes.
*/
void CSocketShutdownModeTest::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	// If immediate socket shutdown has been specified in the test script, 
	// set the appropriate session property
	if(iImmediateShutdown)
		{
		RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
		RStringPool stringPool = iSession.StringPool();
		THTTPHdrVal immediateShutdown = stringPool.StringF(HTTP::ESocketShutdownImmediate, RHTTPSession::GetTable() );
		connInfo.SetPropertyL ( stringPool.StringF(HTTP::ESocketShutdownMode, RHTTPSession::GetTable() ), immediateShutdown );
		}

	// Create the array of transactions
	CreateTransArrayL();

	// Go!
	iFailureError = KErrTestFailed;
	CActiveScheduler::Start();

	// No particular status code is required for success in this test
	iExpectedStatusCode = 0;

	// Close anything opened
	CleanupStack::PopAndDestroy(); //close iSession

	// If any transaction failed then fail the test
	User::LeaveIfError(iFailureError);
	}
	
TInt CSocketShutdownModeTest::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CSocketShutdownModeTest::DoCancel()
	{
	}

const TDesC& CSocketShutdownModeTest::TestName()
	{
	_LIT(KHeaderTestName,"CSocketShutdownModeTest");
	return KHeaderTestName;
	}


TInt CSocketShutdownModeTest::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d on transaction ID=%d\n"), aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CSocketShutdownModeTest::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received
			iEngine->Utils().LogIt(_L("<Got response headers>"));
			} break;
		case THTTPEvent::ESubmit:
			{
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Some (more) body data has been received (in the HTTP response)
			// Dont care about body data, so simply release it.
			iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
			aTransaction.Response().Body()->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			iEngine->Utils().LogIt(_L("<Transaction Complete>"));
			++iCompleteTransCount;
			aTransaction.Close();
			if (--iOpenTransactionCount == 0)
				{
				// All Transactions completed, if we weren't expecting an 
				// interrupted close (abortive) then pass this test.
				iEngine->Utils().LogIt(_L("<All transactions completed>"));
				if(!iInterruptClose)
					{
					iFailureError = KErrNone;
					}
				CActiveScheduler::Stop();
				}
			
			// If we are expecting an interrupted close, close the session 
			// after 2 completed transaction.
			if( iInterruptClose && iCompleteTransCount == 2)
				{
				iEngine->Utils().LogIt(
					_L("<Abortive session close requested>"));
				iSession.Close();
				iFailureError = KErrNone;
				CActiveScheduler::Stop();
				}
			} break;
		case THTTPEvent::ESucceeded:
			{
			iEngine->Utils().LogIt(_L("<Transaction completed successfully>"));
			} break;
		case THTTPEvent::EFailed:
			{
			iEngine->Utils().LogIt(_L("<Transaction failed>"));
			aTransaction.Close();
			if (--iOpenTransactionCount == 0)
				{
				CActiveScheduler::Stop();
				}
			iFailureError = KErrTestFailed;
			} break;
		default:
			{
			iEngine->Utils().LogIt(_L("<unrecognised event> %d"),
				aEvent.iStatus);
			if (aEvent.iStatus < 0)
				{
				iFailureError = aEvent.iStatus;
				CActiveScheduler::Stop();
				}
			} 
			break;
		}
	}

void CSocketShutdownModeTest::CreateTransArrayL()
/**
	This function is called to create a transaction array.
	It may leave with one of the system-wide error codes if an attempt to append to an internal list fails.
*/
	{
	RStringF get = iSession.StringPool().StringF(HTTP::EGET, RHTTPSession::GetTable());
	iOpenTransactionCount = 0;
	for (TInt ii = 1; ii <= KNumOfTransLoops; ++ii)
		{
		// create the transactions and submit
		TUriParser8 urlParser;
		
		urlParser.Parse(KTxtUri1);
		RHTTPTransaction tr1 = iSession.OpenTransactionL(urlParser, *this, get);
		CleanupClosePushL(tr1);
		User::LeaveIfError(iTransArray.Append(tr1));
		CleanupStack::Pop(&tr1);
		iOpenTransactionCount++;
		tr1.SubmitL();
		
		urlParser.Parse(KTxtUri2);
		RHTTPTransaction tr2 = iSession.OpenTransactionL(urlParser, *this, get);
		CleanupClosePushL(tr2);
		User::LeaveIfError(iTransArray.Append(tr2));
		CleanupStack::Pop(&tr2);
		iOpenTransactionCount++;
		tr2.SubmitL();

		HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KTxtUri3(), iIniSettingsFile);
		CleanupStack::PushL(newUrl8);
		TPtr8 newUrlPtr8 = newUrl8->Des();

		urlParser.Parse(newUrlPtr8);
		RHTTPTransaction tr3 = iSession.OpenTransactionL(urlParser, *this, get);
		CleanupClosePushL(tr3);
		User::LeaveIfError(iTransArray.Append(tr3));
		CleanupStack::Pop(&tr3);
		CleanupStack::PopAndDestroy(newUrl8);
		iOpenTransactionCount++;
		tr3.SubmitL();
		
		newUrl8 = TSrvAddrVal::ReplaceHostNameL(KTxtUri4(), iIniSettingsFile);
		CleanupStack::PushL(newUrl8);
		newUrlPtr8 = newUrl8->Des();

		urlParser.Parse(newUrlPtr8);
		RHTTPTransaction tr4 = iSession.OpenTransactionL(urlParser, *this, get);
		CleanupClosePushL(tr4);
		User::LeaveIfError(iTransArray.Append(tr4));
		CleanupStack::Pop(&tr4);
		CleanupStack::PopAndDestroy(newUrl8);
		iOpenTransactionCount++;
		tr4.SubmitL();
		}
	}
