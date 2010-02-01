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

#include "cinc105767.h"
#include "httptestutils.h"
#include <es_sock.h>
#include <httperr.h>

_LIT8 ( KUriTransaction, "http://10.29.23.12:9000/tryit?req=4131.txt&resp=/dev/null" );
_LIT8 ( KUriTransaction2, "http://10.29.23.12/mp3/sing_along.mp3" );
_LIT8 ( KUriTransaction3, "http://10.29.23.12:9001");

/**
	This function is called to create a new instance of the class 
	CINC105767

	@param aTestNumber			The test number that has to be executed
	@param aIniSettingsFile		The script file that holds the sections
								from which the value has to be retrieved
	@leave 						Leaves with a standard error
*/
CINC105767* CINC105767::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CINC105767* self = new(ELeave)CINC105767(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

/**
	Destructor
*/
CINC105767::~CINC105767()
	{
	}

/**
	This C++ constructor is called in the NewL function and is used to 
	instantiate iTestNumber and iIniSettingsFile

	@param aTestNumber		- The test number that has to be executed
	@param aIniSettingsFile	- The script file that holds the sections
							  from which the value has to be retrieved
*/
CINC105767::CINC105767(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	// does nothing
	}

/**
	This function is called to initiate the tests.
	
	@leave	Leaves with a standard error.
*/
void CINC105767::DoRunL()
	{
	TUriParser8 up;
	up.Parse(KUriTransaction);

	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	RStringPool strP = iSession.StringPool();

	// Open a GET transactions, specifying this object as the request body data supplier
	iTransaction = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt( _L("\nSubmitting first transaction\n") );
	iTransaction.SubmitL();
	iTransaction.PropertySet().SetPropertyL (iSession.StringPool().StringF(HTTP::ENotifyOnDisconnect,RHTTPSession::GetTable()), iSession.StringPool().StringF(HTTP::EEnableDisconnectNotification,RHTTPSession::GetTable())); 
	iExpectedError = KErrHttpResponseNotReceived;
	CActiveScheduler::Start();	
	User::LeaveIfError ( iFailureError );
	iTransaction.Close ();
	
	up.Parse(KUriTransaction2);
	iTransaction = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));	
	iEngine->Utils().LogIt( _L("\nSubmitting second transaction\n") );
	iTransaction.SubmitL();
	iExpectedError = KErrHttpPartialResponseReceived;
	CActiveScheduler::Start();	
	User::LeaveIfError ( iFailureError );
	iTransaction.Close ();
	
	up.Parse(KUriTransaction3);
	iTransaction = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));	
	RHTTPRequest rq = iTransaction.Request();
	rq.SetBody(*this);
	RHTTPHeaders hdr = rq.GetHeaderCollection();
	THTTPHdrVal length(OverallDataSize());
	hdr.SetFieldL(strP.StringF(HTTP::EContentLength,RHTTPSession::GetTable()), length);
	THTTPHdrVal contType(strP.StringF(HTTP::ETextPlain, RHTTPSession::GetTable()));
	hdr.SetFieldL(strP.StringF(HTTP::EContentType, RHTTPSession::GetTable()), contType);
	
	iEngine->Utils().LogIt( _L("\nSubmitting third transaction\n") );	
	iTransaction.SubmitL();
	iTransaction.PropertySet().SetPropertyL (iSession.StringPool().StringF(HTTP::ENotifyOnDisconnect,RHTTPSession::GetTable()), iSession.StringPool().StringF(HTTP::EEnableDisconnectNotification,RHTTPSession::GetTable())); 
	iExpectedError = KErrHttpRequestNotSent;
	CActiveScheduler::Start();	
	User::LeaveIfError ( iFailureError );
	iTransaction.Close ();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession
	}

/**
	If a test is failed then is function is executed to log the error code
	with which the test failed

	@param	aErr	The error code with which the test failed.
*/
TInt CINC105767::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CINC105767::DoCancel()
	{
	}

const TDesC& CINC105767::TestName()
	{
	_LIT(KHeaderTestName,"CINC105767");
	return KHeaderTestName;
	}

TInt CINC105767::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CINC105767::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
			iTransaction.PropertySet().SetPropertyL (iSession.StringPool().StringF(HTTP::ENotifyOnDisconnect,RHTTPSession::GetTable()), iSession.StringPool().StringF(HTTP::EEnableDisconnectNotification,RHTTPSession::GetTable())); 			
			CloseConnection ();
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
				if ( aEvent.iStatus != iExpectedError )
					iFailureError = aEvent.iStatus;
				CActiveScheduler::Stop();
				}
			} 
			break;
		}
	return;
	}

void CINC105767::CloseConnection ()
	{
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
	}

TBool CINC105767::IsConnectionActive ( RConnection& aConnection )
	{
	TRequestStatus status;
	TUint secs = 1; // 1 secs
	TBool state = EFalse;

	TPckg < TBool > stateDes( state );

	aConnection.IsConnectionActiveRequest( secs, stateDes, status );
	User::WaitForRequest ( status );

	return state;
	}

TBool CINC105767::GetNextDataPart(TPtrC8& aDataChunk)
	{
	iEngine->Utils().LogIt( _L("\nPosting some data.\n") );
	_LIT8 ( KPostData, "abcdefghijklmnopqrstuvwxvz");
	aDataChunk.Set(KPostData());
	return EFalse;		
	}

void CINC105767::ReleaseData()
	{
	// Do nothing	
	}

TInt CINC105767::OverallDataSize()
	{
	// return some value
	return 20 * 1024;
	}

TInt CINC105767::Reset ()
	{
	return KErrNone;	
	}
