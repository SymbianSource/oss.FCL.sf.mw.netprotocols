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
// defines test class: CINC097492
// 
//

#include "CINC097492.h"
#include "examplecookiemanager.h"
#include <http/ccookiefilterinterface.h>

// User include
#include "csrvaddrval.h"

// Literals used in the file
_LIT8(KHttpUrl1,		"http://10.29.23.12/manual/index.html");
_LIT8(KHttpUrl2,		"http://10.29.23.12/manual/new_features_1_3.html");
_LIT8(KHttpUrl3,		"http://10.29.23.12/manual/images/index.gif");
_LIT8(KHttpUrl4,		"http://10.29.23.12/manual/install.html");
_LIT8(KHttpUrl5,		"http://10.29.23.12/manual/invoking.html");
_LIT8(KHttpUrl6,		"http://10.29.23.12/manual/stopping.html");
_LIT8(KHttpUrl7,		"http://10.29.23.12/manual/mod/directives.html");
_LIT8(KHttpUrl8,		"http://10.29.23.12/manual/images/custom_errordocs.gif");
_LIT8(KHttpUrl9,		"http://10.29.23.12/manual/images/home.gif");
_LIT8(KHttpUrl10,	"http://10.29.23.12/manual/images/home.gif");

/**
	This function is called to create a new instance of the class 
	CINC097492

	@param aTestNumber			The test number that has to be executed
	@param aIniSettingsFile		The script file that holds the sections
								from which the value has to be retrieved
	@leave 						Leaves with a standard error
*/
CINC097492* CINC097492::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CINC097492* self = new(ELeave)CINC097492(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

/**
	Destructor
*/
CINC097492::~CINC097492()
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
CINC097492::CINC097492(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	}

/**
	This function is called to initiate the tests.
	
	@leave	Leaves with a standard error.
*/
void CINC097492::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	CreateTransArrayL();
	
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession
	}

/**
	If a test is failed then is function is executed to log the error code
	with which the test failed

	@param	aErr	The error code with which the test failed.
*/
TInt CINC097492::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CINC097492::DoCancel()
	{
	}

const TDesC& CINC097492::TestName()
	{
	_LIT(KHeaderTestName,"CINC097492");
	return KHeaderTestName;
	}

TInt CINC097492::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CINC097492::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
			if(aTransaction.Request().URI().UriDes().Compare(KHttpUrl7) == 0)
				{
				iTransArray[8].Cancel();
				iOpenTransCount--;
				}

			// The transaction's response is complete
			iEngine->Utils().LogIt(_L("<Transaction Complete>"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			if(aTransaction.Request().URI().UriDes().Compare(KHttpUrl4) == 0)
				{
				iTransArray[5].Cancel();
				iOpenTransCount--;
				}
			
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


void CINC097492::ConfigureSessionFiltersL(TFilterConfigurationIterator* /*aFilterConfigIter*/)
	{
	if (iTestNumber == 11)
		{
		User::Leave(KErrNone);
		}

	iCookieManager = CExampleCookieManager::NewL(iSession.StringPool());

	CCookieFilterInterface::InstallFilterL(iSession, iCookieManager);
	}
	
void CINC097492::CreateTransArrayL()
	{
	RStringPool strP = iSession.StringPool();
	TUriParser8 up;

	up.Parse(KHttpUrl1);
	RHTTPTransaction trans1 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans1);
	iTransArray.Append(trans1);
	CleanupStack::Pop(&trans1);
	
	up.Parse(KHttpUrl2);
	RHTTPTransaction trans2 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans2);
	iTransArray.Append(trans2);
	CleanupStack::Pop(&trans2);

	up.Parse(KHttpUrl3);
	RHTTPTransaction trans3 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans3);
	iTransArray.Append(trans3);
	CleanupStack::Pop(&trans3);
	
	up.Parse(KHttpUrl4);
	RHTTPTransaction trans4 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans4);
	iTransArray.Append(trans4);
	CleanupStack::Pop(&trans4);

	up.Parse(KHttpUrl5);
	RHTTPTransaction trans5 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans5);
	iTransArray.Append(trans5);
	CleanupStack::Pop(&trans5);

	up.Parse(KHttpUrl6);
	RHTTPTransaction trans6 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans6);
	iTransArray.Append(trans6);
	CleanupStack::Pop(&trans6);
		
	up.Parse(KHttpUrl7);
	RHTTPTransaction trans7 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans7);
	iTransArray.Append(trans7);
	CleanupStack::Pop(&trans7);

	up.Parse(KHttpUrl8);
	RHTTPTransaction trans8 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans8);
	iTransArray.Append(trans8);
	CleanupStack::Pop(&trans8);

	up.Parse(KHttpUrl9);
	RHTTPTransaction trans9 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans9);
	iTransArray.Append(trans9);
	CleanupStack::Pop(&trans9);

	up.Parse(KHttpUrl10);
	RHTTPTransaction trans10 = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(trans10);
	iTransArray.Append(trans10);
	CleanupStack::Pop(&trans10);

	for(TInt i=0; i<iTransArray.Count(); i++)
		{
		iTransArray[i].SubmitL();
		iOpenTransCount++;
		}
	}


