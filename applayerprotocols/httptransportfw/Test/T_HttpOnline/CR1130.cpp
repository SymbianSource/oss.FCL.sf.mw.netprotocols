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
// defines test class: CR1130
// 
//

#include "CR1130.h"
#include "examplecookiemanager.h"
#include <http/ccookiefilterinterface.h>

// User include
#include "csrvaddrval.h"

// Literals used in the file
_LIT8(KHttpUrl1,		"http://10.29.23.12/manual/images/index.gif");


/**
	This function is called to create a new instance of the class 
	CR1130

	@param aTestNumber			The test number that has to be executed
	@param aIniSettingsFile		The script file that holds the sections
								from which the value has to be retrieved
	@leave 						Leaves with a standard error
*/
CR1130* CR1130::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CR1130* self = new(ELeave)CR1130(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

/**
	Destructor
*/
CR1130::~CR1130()
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
CR1130::CR1130(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	}

/**
	This function is called to initiate the tests.
	
	@leave	Leaves with a standard error.
*/
void CR1130::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);
	
	THTTPHdrVal hdrVal ( 10000 );
	RHTTPConnectionInfo httpConnInfo = iSession.ConnectionInfo();
	httpConnInfo.SetPropertyL ( iSession.StringPool().StringF(HTTP::ERecvBufferSize,RHTTPSession::GetTable()), hdrVal );

	CreateTransArrayL();
	
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession
	}

/**
	If a test is failed then is function is executed to log the error code
	with which the test failed

	@param	aErr	The error code with which the test failed.
*/
TInt CR1130::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CR1130::DoCancel()
	{
	}

const TDesC& CR1130::TestName()
	{
	_LIT(KHeaderTestName,"CR1130");
	return KHeaderTestName;
	}

TInt CR1130::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CR1130::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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


void CR1130::ConfigureSessionFiltersL(TFilterConfigurationIterator* /*aFilterConfigIter*/)
	{
	if (iTestNumber == 11)
		{
		User::Leave(KErrNone);
		}

	iCookieManager = CExampleCookieManager::NewL(iSession.StringPool());

	CCookieFilterInterface::InstallFilterL(iSession, iCookieManager);
	}
	
void CR1130::CreateTransArrayL()
	{
	RStringPool strP = iSession.StringPool();
	TUriParser8 up;

	up.Parse(KHttpUrl1);
	RHTTPTransaction trans1 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans1);
	iTransArray.Append(trans1);
	CleanupStack::Pop(&trans1);

	for(TInt i=0; i<iTransArray.Count(); i++)
		{
		iTransArray[i].SubmitL();
		iOpenTransCount++;
		}
	}


