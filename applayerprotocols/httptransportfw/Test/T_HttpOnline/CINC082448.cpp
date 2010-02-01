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
//

#include "CINC082448.h"
#include "httptestutils.h"
#include "es_sock.h"

_LIT8 ( KUriTransaction1, "http://10.29.23.12/1.jpg" );

/**
	This function is called to create a new instance of the class 
	CINC082448

	@param aTestNumber			The test number that has to be executed
	@param aIniSettingsFile		The script file that holds the sections
								from which the value has to be retrieved
	@leave 						Leaves with a standard error
*/
CINC082448* CINC082448::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CINC082448* self = new(ELeave)CINC082448(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

/**
	Destructor
*/
CINC082448::~CINC082448()
	{
	}

/**
	This C++ constructor is called in the NewL function and is used to 
	instantiate iTestNumber and iIniSettingsFile

	@param aTestNumber		- The test number that has to be executed
	@param aIniSettingsFile	- The script file that holds the sections
							  from which the value has to be retrieved
*/
CINC082448::CINC082448(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	// does nothing
	}

/**
	This function is called to initiate the tests.
	
	@leave	Leaves with a standard error.
*/
void CINC082448::DoRunL()
	{
	TUriParser8 up;
	up.Parse(KUriTransaction1);

	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	RStringPool strP = iSession.StringPool();

	// Open a GET transactions, specifying this object as the request body data supplier
	iTransaction = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt( _L("\nSubmitting first transaction\n") );
	iTransaction.SubmitL();
	CActiveScheduler::Start();
	
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool ();

	THTTPHdrVal value;
	TBool hasValue = connInfo.Property ( stringPool.StringF(HTTP::EHttpSocketConnection, RHTTPSession::GetTable()), value );
	if (hasValue)
		{
		RConnection* conn = REINTERPRET_CAST( RConnection*, value.Int() );
		TUint count;
		//get the no of active connections
		TInt err = conn->EnumerateConnections( count );
		if ( err == KErrNone )
			{
			if ( IsConnectionActive( *conn ) )
				{
				iEngine->Utils().LogIt( _L("\nConnection is active. Stop the active connection.\n") );
				conn->Stop ();  // stop the active connection
				if ( !IsConnectionActive(  *conn ) )
					{
					iEngine->Utils().LogIt( _L("\nConnection stopped.\n") );
					}
				else
					{
					iEngine->Utils().LogIt( _L("\nConnection stopped failed.\n") );
					}				
				}				
			}		
		else
			{
			iEngine->Utils().LogIt(_L("Unable to enumerate number of connections, Error: %d"), err);
			User::Leave ( KErrNotFound );
			}
		}

	RHTTPTransaction myTransaction2 = iSession.OpenTransactionL ( up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable() ));
	iEngine->Utils().LogIt( _L("Submitting second transaction\n") );
	myTransaction2.SubmitL();
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(&iSession); //close iSession

	}

/**
	If a test is failed then is function is executed to log the error code
	with which the test failed

	@param	aErr	The error code with which the test failed.
*/
TInt CINC082448::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CINC082448::DoCancel()
	{
	}

const TDesC& CINC082448::TestName()
	{
	_LIT(KHeaderTestName,"CINC082448");
	return KHeaderTestName;
	}

TInt CINC082448::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CINC082448::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
			TPtrC8 bodyData;
			if(aTransaction.Response().Body()->GetNextDataPart(bodyData) )
				{
				iEngine->Utils().LogIt(_L("1,Got Last Chunk...\n"));
				}
			aTransaction.Response().Body()->ReleaseData();
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

TBool CINC082448::IsConnectionActive ( RConnection& aConnection )
	{
	TRequestStatus status;
	TUint secs = 1; // 1 secs
	TBool state = EFalse;

	TPckg < TBool > stateDes( state );

	aConnection.IsConnectionActiveRequest( secs, stateDes, status );
	User::WaitForRequest ( status );

	return state;
	}

void CINC082448::ConfigureSessionFiltersL(TFilterConfigurationIterator* /*aFilterConfigIter*/)
	{
	}

TBool CINC082448::GetCredentialsL(const TUriC8& /*aURI*/, RString /*aRealm*/, 
								   RStringF /*aAuthenticationType*/,
								   RString& /*aUsername*/, 
								   RString& /*aPassword*/)

	{
	User::Leave(KErrGeneral);
	return EFalse;
	}
