// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "cinc112633_2.h"

_LIT8(KHttpUrl,			"http://ban-build_iws04.intra/auth/auth_file.html");
_LIT8(KHttpUrl1,		"http://ban-build_iws04.intra/file1.html");
_LIT8(KHttpUrl2,		"http://ban-build_iws04.intra/file2.html");

/**
	This function is called to create a new instance of the class 
	CINC112633_2

	@param aTestNumber			The test number that has to be executed
	@param aIniSettingsFile		The script file that holds the sections
								from which the value has to be retrieved
	@leave 						Leaves with a standard error
*/
CINC112633_2* CINC112633_2::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CINC112633_2* self = new(ELeave)CINC112633_2(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

/**
	Destructor
*/
CINC112633_2::~CINC112633_2()
	{
	}

/**
	This C++ constructor is called in the NewL function and is used to 
	instantiate iTestNumber and iIniSettingsFile

	@param aTestNumber		- The test number that has to be executed
	@param aIniSettingsFile	- The script file that holds the sections
							  from which the value has to be retrieved
*/
CINC112633_2::CINC112633_2(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	// does nothing
	}

/**
	This function is called to initiate the tests.
	
	@leave	Leaves with a standard error.
*/
void CINC112633_2::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);
	
	InstallAuthenticationL(iSession);
	
	RStringPool strP = iSession.StringPool();

	TUriParser8 up;
	up.Parse(KHttpUrl);
	RHTTPTransaction trans1 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	trans1.SubmitL();
	CActiveScheduler::Start();
	
	TUriParser8 up1;
	up1.Parse(KHttpUrl1);
	RHTTPTransaction trans2 = iSession.OpenTransactionL(up1, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	trans2.SubmitL();
	CActiveScheduler::Start();

	TUriParser8 up2;
	up2.Parse(KHttpUrl2);
	RHTTPTransaction trans3 = iSession.OpenTransactionL(up2, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	trans3.SubmitL();
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession

	}

/**
	If a test is failed then is function is executed to log the error code
	with which the test failed

	@param	aErr	The error code with which the test failed.
*/
TInt CINC112633_2::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CINC112633_2::DoCancel()
	{
	}

const TDesC& CINC112633_2::TestName()
	{
	_LIT(KHeaderTestName,"CINC112633_2");
	return KHeaderTestName;
	}

TInt CINC112633_2::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CINC112633_2::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received
			iEngine->Utils().LogIt(_L("<Got response headers>"));
			MHTTPDataSupplier* body = aTransaction.Response().Body ();
			if ( body )
				{
				body->ReleaseData ();
				}
			} break;
		case THTTPEvent::ESubmit:
			{
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Some (more) body data has been received (in the HTTP response)
			iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
			// Get the body data supplier
			MHTTPDataSupplier* iRespBody = aTransaction.Response().Body();
			// Append to the output file.
			TPtrC8 bodyData;
			TBool lastChunk = iRespBody->GetNextDataPart( bodyData );
			iRespBody->ReleaseData();
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

TBool CINC112633_2::GetCredentialsL(const TUriC8& /*aURI*/, RString aRealm, 
		   RStringF /*aAuthenticationType*/,
		   RString& aUsername, 
		   RString& aPassword)

	{
	_LIT8(KPassword, "naveen");
	
	TBuf8<KMaxUserEntrySize> usrAndPwd(KPassword);
	
	TRAPD(err, aUsername = aRealm.Pool().OpenStringL(usrAndPwd));
	if (!err)
		{
		TRAP(err, aPassword = aRealm.Pool().OpenStringL(usrAndPwd));
		if (!err)
			return ETrue;
		}
	return EFalse;
	}

