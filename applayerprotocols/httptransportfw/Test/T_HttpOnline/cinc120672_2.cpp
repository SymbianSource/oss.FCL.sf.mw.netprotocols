// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "cinc120672.h"


_LIT8(KHttpUrl,		"http://www.motbrw.com/kjava/3GSMIOT/index.html");

/**
	This function is called to create a new instance of the class 
	CINC120672_2

	@param aTestNumber			The test number that has to be executed
	@param aIniSettingsFile		The script file that holds the sections
								from which the value has to be retrieved
	@leave 						Leaves with a standard error
*/

CINC120672_2* CINC120672_2::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CINC120672_2* self = new(ELeave)CINC120672_2(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

/**
	Destructor
*/
CINC120672_2::~CINC120672_2()
	{
	 
	}

/**
	This C++ constructor is called in the NewL function and is used to 
	instantiate iTestNumber and iIniSettingsFile

	@param aTestNumber		- The test number that has to be executed
	@param aIniSettingsFile	- The script file that holds the sections
							  from which the value has to be retrieved
*/
CINC120672_2::CINC120672_2(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	}

/**
	If a test is failed then is function is executed to log the error code
	with which the test failed

	@param	aErr	The error code with which the test failed.
*/
TInt CINC120672_2::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CINC120672_2::DoCancel()
	{
	}

const TDesC& CINC120672_2::TestName()
	{
	_LIT(KHeaderTestName,"CINC120672_2");
	return KHeaderTestName;
	}

/**
	This function is called to initiate the tests.
	
	@leave	Leaves with a standard error.
*/
void CINC120672_2::DoRunL()
	{
	TUriParser8 up;
	up.Parse(KHttpUrl);
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);
	InstallAuthenticationL(iSession); 
	RStringPool strP = iSession.StringPool();
	// Open a GET transactions, specifying this object as the request body data supplier
	iTransaction = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CHttpTimer1* iTimer = new(ELeave) CHttpTimer1(*this);
	iTransaction.SubmitL();
	iTimer->After(10000000);
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(&iSession); //close iSession
	delete iTimer;

	}


TInt CINC120672_2::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}


void CINC120672_2::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
			iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
			MHTTPDataSupplier* body = aTransaction.Response().Body();
			TPtrC8 ptr;
			body->GetNextDataPart(ptr);
			body->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			iEngine->Utils().LogIt(_L("<Transaction Complete>"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			// The transaction succeeded
			iEngine->Utils().LogIt(_L("<Transaction succeeded>"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			}
			break;
		case THTTPEvent::EFailed:
			{
			// The transaction failed so fail the test
			iEngine->Utils().LogIt(_L("<Transaction failed>"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
		default:
			{
			_LIT(KLogDescription, "<unrecognised event> %d");
			iEngine->Utils().LogIt(KLogDescription,aEvent.iStatus);
			if (aEvent.iStatus < 0)
				{
				iFailureError = aEvent.iStatus;
				CActiveScheduler::Stop();
				}
			} 
			break;
		}
	
	return;
	}

void CINC120672_2::Timedout()
	{
	  CActiveScheduler::Stop();
	}

TBool CINC120672_2::GetCredentialsL(const TUriC8& /*aURI*/, RString /*aRealm*/, 
								   RStringF /*aAuthenticationType*/,
								   RString& /*aUsername*/, 
								   RString& /*aPassword*/)

	{
	  
	iTransaction.Cancel();
	return EFalse;
	}
