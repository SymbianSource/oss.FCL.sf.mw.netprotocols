// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CPipeliningTestClient.h"
#include "httptestutils.h"
#include "MPipeliningTestCase.h"
#include "MPipeliningTestObserver.h"

CPipeliningTestClient* CPipeliningTestClient::NewL(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver)
	{
	CPipeliningTestClient* self = new (ELeave) CPipeliningTestClient(aTestUtils, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CPipeliningTestClient::CPipeliningTestClient(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver)
: iTestUtils(aTestUtils), iObserver(aObserver)
	{
	}

void CPipeliningTestClient::ConstructL()
	{
	iSession.OpenL();
	}

CPipeliningTestClient::~CPipeliningTestClient()
	{
	iTransArray.Reset();
	iTransArray.Close();
	iSession.Close();
	iNetworkConnection.Close();
	iSocketServ.Close();
	}

RHTTPSession& CPipeliningTestClient::GetSession()
	{
	return iSession;
	}
	
// From MHTTPTransactionCallback
void CPipeliningTestClient::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	TInt expectedError = iTestCase->ExpectedError(aTransaction);		
	TInt tranID = aTransaction.Id();
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Done with that bit of body data
			aTransaction.Response().Body()->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			TRAPD(err1, iTestCase->ProcessHeadersL(aTransaction));
			if ( err1 != KErrNone )
				{
				_LIT(KProcessHeadersError, "Client - Transaction %d Failed, Error = %d");
				iTestUtils.LogIt(KProcessHeadersError, tranID, err1);
				EndTestL(err1);
				}
			} break;
		case THTTPEvent::ESucceeded:
			{
			++iCurrentTrans;
			aTransaction.Close();
			_LIT(KTxtSuccessful, "Client - Transaction %d, completed successfully.");
			iTestUtils.LogIt(KTxtSuccessful, tranID);

			if( expectedError != KErrNone )
				{
				_LIT(KTxtErrorExpected, "Client - Expected error code %d");
				iTestUtils.LogIt(KTxtErrorExpected, expectedError);
				EndTestL(KErrNotFound);
				}

			if( iTestCase->TotalTransactionCount() == iCurrentTrans )
				{	
				EndTestL(KErrNone);
				}
			} break;
		case THTTPEvent::EFailed:
			{
			++iCurrentTrans;
			aTransaction.Close();
			if( expectedError == iLastError )
				{
				_LIT(KTxtExpectedError, "Client - Transaction %d Failed, Expected: %d, Actual: %d");
				iTestUtils.LogIt(KTxtExpectedError(), tranID, expectedError, iLastError);
				if( iTestCase->TotalTransactionCount() == iCurrentTrans )
					{
					EndTestL(KErrNone);
					}
				}
			else
				{
				_LIT(KTxtErrorUnexpected, "Client - Error code mismatch.");
				iTestUtils.LogIt(KTxtErrorUnexpected());
				EndTestL(KErrNotFound);
				}
			} break;
		case THTTPEvent::ERedirectedPermanently:
			{
			} break;
		case THTTPEvent::ERedirectedTemporarily:
			{
			} break;
		case THTTPEvent::ERedirectRequiresConfirmation:
 			{
 			aTransaction.SubmitL();
 			} break;
		default:
			{
			if (aEvent.iStatus < 0)
				{
				
				iLastError = aEvent.iStatus;
				_LIT(KTxtClientError, "Client - Transaction %d, received error code %d");
				iTestUtils.LogIt(KTxtClientError, tranID, iLastError);
				
				}
			
			} break;
		}
	}
	
	
void CPipeliningTestClient::EndTestL(TInt aErrorCode)
	{
	#if defined (_DEBUG)
	_LIT(KTxtClientError, "Error - Code %d");	
	iTestUtils.LogIt(KTxtClientError, aErrorCode);      	
	
	if (aErrorCode == KErrNone)
		{
		TInt expectedConnections = iTestCase->RealExpectedConnectionCount();
		RStringPool stringPool = iSession.StringPool();
		_LIT8(KNumberConnectionManagers, "__NumConnectionManagers");
		RStringF numberConnectionsString = stringPool.OpenFStringL(KNumberConnectionManagers);
		CleanupClosePushL(numberConnectionsString);
		RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
		TInt numberConnections =0;
	
		THTTPHdrVal numberConnectionsVal;
		if (connInfo.Property(numberConnectionsString, numberConnectionsVal))
			{
			numberConnections = numberConnectionsVal.Int();
			connInfo.RemoveProperty(numberConnectionsString);
			
			if (numberConnections > expectedConnections)
				{
                _LIT(KTxtClientError, "Client - Connections %d, Expected: %d");
                iTestUtils.LogIt(KTxtClientError, numberConnections, expectedConnections);				
				aErrorCode = KErrNotFound;
				}

			}
		
		CleanupStack::PopAndDestroy(&numberConnectionsString);
		}
	#endif
	
	iObserver.EndTest(aErrorCode);
	}	
	
	
TInt CPipeliningTestClient::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KTxtRunError, "Client - MHFRunError - Transaction %d with error code %d.");
	iTestUtils.LogIt(KTxtRunError(), aTransaction.Id(), aError);

	return KErrNone;
	}

void CPipeliningTestClient::AddTransactionL(RHTTPTransaction aTransaction)
	{
	User::LeaveIfError(iTransArray.Append(aTransaction));
	}
	
void CPipeliningTestClient::StartClientL()
	{
	__ASSERT_DEBUG(iTestCase != NULL, User::Invariant());

	if( iTestCase->EnableBatching() )
		{
		// @todo we need to add the session property for enabling batching
		RHTTPConnectionInfo connInfo = GetSession().ConnectionInfo();
		THTTPHdrVal batchingSupport(iSession.StringPool().StringF(HTTP::EEnableBatching,RHTTPSession::GetTable()));
		connInfo.SetPropertyL(iSession.StringPool().StringF(HTTP::EHttpBatching,RHTTPSession::GetTable()), batchingSupport);
		}

	TBool batchTrans = iTestCase->BatchTransactions();	
	// Create transactions
	for( TInt ii=0; ii < iTestCase->TotalTransactionCount(); ++ii )
		{
		RHTTPTransaction trans = iTestCase->GetTransactionL(ii, GetSession(), *this);
		CleanupClosePushL(trans);
		AddTransactionL(trans);
		if(!batchTrans)
			{
			trans.SubmitL();
			}
		CleanupStack::Pop(&trans);
		}
	
	if(batchTrans)
		{
		for( TInt jj=0; jj < iTransArray.Count(); ++jj )
			{
			iTransArray[jj].SubmitL();
			}
		}
	}
	
TInt CPipeliningTestClient::LastError()
{
	return iLastError;
}

void CPipeliningTestClient::SetTestCase(MPipeliningTestCase* aTestCase)
	{
	iTestCase = aTestCase;
	}

void CPipeliningTestClient::StartAndSetNetworkToHttp()
    {
    if(iSocketServ.Handle() != KNullHandle)
        {
        iNetworkConnection.Close();
        iSocketServ.Close();
        }
    User::LeaveIfError(iSocketServ.Connect());
    User::LeaveIfError(iNetworkConnection.Open(iSocketServ));
    User::LeaveIfError(iNetworkConnection.Start());
    // Set the RSocketServ & RConnection
    RStringPool strPool = iSession.StringPool();
    RHTTPConnectionInfo connInfo1 = iSession.ConnectionInfo();
    connInfo1.SetPropertyL ( strPool.StringF(HTTP::EHttpSocketServ, 
                             RHTTPSession::GetTable() ), 
                             THTTPHdrVal (iSocketServ.Handle()) );
    TInt connPtr = REINTERPRET_CAST(TInt, &iNetworkConnection);                
    connInfo1.SetPropertyL ( strPool.StringF(HTTP::EHttpSocketConnection, 
                             RHTTPSession::GetTable() ), 
                             THTTPHdrVal(connPtr));    
    }

