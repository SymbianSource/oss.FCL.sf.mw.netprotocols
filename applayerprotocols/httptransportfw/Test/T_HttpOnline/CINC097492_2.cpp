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
// defines test class: CINC097492_2
// 
//

#include "CINC097492_2.h"
#include "examplecookiemanager.h"
#include <http/ccookiefilterinterface.h>

// User include
#include "csrvaddrval.h"

// Literals used in the file

// URL whose response status code is 200
_LIT8(KHttpUrl,			"http://www.seoconsultants.com");
// URL with a 302 response where the Content-Length is 0.
_LIT8(KHttpUrl1,		"http://www.seoconsultants.com/w3c/status-codes/302.asp");
// URL with a 302 response where the Content-Length is greater than 0.
_LIT8(KHttpUrl2,		"http://www.news.com");

/**
	This function is called to create a new instance of the class 
	CINC097492_2

	@param aTestNumber			The test number that has to be executed
	@param aIniSettingsFile		The script file that holds the sections
								from which the value has to be retrieved
	@leave 						Leaves with a standard error
*/
CINC097492_2* CINC097492_2::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CINC097492_2* self = new(ELeave)CINC097492_2(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

/**
	Destructor
*/
CINC097492_2::~CINC097492_2()
	{
	iTransArray.Close();
	}

/**
	This C++ constructor is called in the NewL function and is used to 
	instantiate iTestNumber and iIniSettingsFile

	@param aTestNumber		- The test number that has to be executed
	@param aIniSettingsFile	- The script file that holds the sections
							  from which the value has to be retrieved
*/
CINC097492_2::CINC097492_2(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	// does nothing
	}

/**
	This function is called to initiate the tests.
	
	@leave	Leaves with a standard error.
*/
void CINC097492_2::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);
	RStringPool strP = iSession.StringPool();

	TUriParser8 up;
	up.Parse(KHttpUrl);
	RHTTPTransaction trans1 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans1);
	iTransArray.Append(trans1);
	CleanupStack::Pop(&trans1);
	
	TUriParser8 up1;
	up1.Parse(KHttpUrl1);
	RHTTPTransaction trans2 = iSession.OpenTransactionL(up1, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans2);
	iTransArray.Append(trans2);
	CleanupStack::Pop(&trans2);

	TUriParser8 up2;
	up2.Parse(KHttpUrl2);
	RHTTPTransaction trans3 = iSession.OpenTransactionL(up2, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans3);
	iTransArray.Append(trans3);
	CleanupStack::Pop(&trans3);

	for(TInt i=0; i<iTransArray.Count(); i++)
		{
		iTransArray[i].SubmitL();
		iOpenTransCount++;
		}	
	
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(&iSession); //close iSession

	}

/**
	If a test is failed then is function is executed to log the error code
	with which the test failed

	@param	aErr	The error code with which the test failed.
*/
TInt CINC097492_2::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CINC097492_2::DoCancel()
	{
	}

const TDesC& CINC097492_2::TestName()
	{
	_LIT(KHeaderTestName,"CINC097492_2");
	return KHeaderTestName;
	}

TInt CINC097492_2::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CINC097492_2::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
			if(--iOpenTransCount == 0)
				{
				CActiveScheduler::Stop();
				}
			}
			break;
		case THTTPEvent::EFailed:
			{
			// The transaction failed so fail the test
			iEngine->Utils().LogIt(_L("<Transaction failed>"));
			aTransaction.Close();
			if(--iOpenTransCount == 0)
				{
				CActiveScheduler::Stop();
				}
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


void CINC097492_2::ConfigureSessionFiltersL(TFilterConfigurationIterator* /*aFilterConfigIter*/)
	{
	if (iTestNumber == 11)
		{
		User::Leave(KErrNone);
		}

	iCookieManager = CExampleCookieManager::NewL(iSession.StringPool());

	CCookieFilterInterface::InstallFilterL(iSession, iCookieManager);
	}


