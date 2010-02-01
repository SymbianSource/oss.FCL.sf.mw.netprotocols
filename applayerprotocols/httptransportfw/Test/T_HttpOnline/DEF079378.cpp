// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// defines test class: CDEF079378
// 
//

#include "DEF079378.h"
#include "examplecookiemanager.h"
#include <http/ccookiefilterinterface.h>

// User include
#include "csrvaddrval.h"

// Literals used in the file
_LIT8(KHttpAuthenticationUrl,		"http://10.29.23.12/jar/kFire004.jar");


/**
	This function is called to create a new instance of the class 
	CDEF079378

	@param aTestNumber			The test number that has to be executed
	@param aIniSettingsFile		The script file that holds the sections
								from which the value has to be retrieved
	@leave 						Leaves with a standard error
*/
CDEF079378* CDEF079378::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CDEF079378* self = new(ELeave)CDEF079378(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

/**
	Destructor
*/
CDEF079378::~CDEF079378()
	{
//	delete iCookieManager;
//	iTransArray.Close();
	}

/**
	This C++ constructor is called in the NewL function and is used to 
	instantiate iTestNumber and iIniSettingsFile

	@param aTestNumber		- The test number that has to be executed
	@param aIniSettingsFile	- The script file that holds the sections
							  from which the value has to be retrieved
*/
CDEF079378::CDEF079378(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	// does nothing
	}

/**
	This function is called to initiate the tests.
	
	@leave	Leaves with a standard error.
*/
void CDEF079378::DoRunL()
	{
	TUriParser8 up;
	up.Parse(KHttpAuthenticationUrl);

	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	// Install this class as the callback for authentication requests
	InstallAuthenticationL(iSession);
	RStringPool strP = iSession.StringPool();

	// Open a GET transactions, specifying this object as the request body data supplier
	iTransaction = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	
	iTransaction.SubmitL();
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession

	}

/**
	If a test is failed then is function is executed to log the error code
	with which the test failed

	@param	aErr	The error code with which the test failed.
*/
TInt CDEF079378::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CDEF079378::DoCancel()
	{
	}

const TDesC& CDEF079378::TestName()
	{
	_LIT(KHeaderTestName,"CDEF079378");
	return KHeaderTestName;
	}

TInt CDEF079378::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CDEF079378::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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


void CDEF079378::ConfigureSessionFiltersL(TFilterConfigurationIterator* /*aFilterConfigIter*/)
	{
	if (iTestNumber == 11)
		{
		User::Leave(KErrNone);
		}

	iCookieManager = CExampleCookieManager::NewL(iSession.StringPool());

	CCookieFilterInterface::InstallFilterL(iSession, iCookieManager);
	}

TBool CDEF079378::GetCredentialsL(const TUriC8& /*aURI*/, RString /*aRealm*/, 
								   RStringF /*aAuthenticationType*/,
								   RString& /*aUsername*/, 
								   RString& /*aPassword*/)

	{
	User::Leave(KErrGeneral);
	return EFalse;
	}
