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

#include "cpipeliningtestclient.h"
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
	iHttpTimer = new(ELeave) CHttpTimer1(*this);
	iASW = new(ELeave) CActiveSchedulerWait();
	}

CPipeliningTestClient::~CPipeliningTestClient()
	{
	iTransArray.Reset();
	iTransArray.Close();
	iSession.Close();
	delete iHttpTimer;
	delete iASW;
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
			iTestCase->ProcessHeadersL(aTransaction);
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Done with that bit of body data
			aTransaction.Response().Body()->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			} break;
		case KErrHttpOptimiserFailsTrans:
			{
			_LIT(KTxtMessage, "Cancelling/Failing Transaction\n");
			iTestUtils.LogIt(KTxtMessage);
			aTransaction.Fail(THTTPFilterHandle::EProtocolHandler);
			} break;
		case THTTPEvent::ESucceeded:
			{
			++iCurrentTrans;
			aTransaction.Close();
			_LIT(KTxtSuccessful, "Client - Transaction %d, completed successfully.");
			iTestUtils.LogIt(KTxtSuccessful, tranID);
            if ((iTestCase->TestCaseName().Match(_L("Test Case 5")) == 0)||(iTestCase->TestCaseName().Match(_L("Test Case 6")) == 0)||(iTestCase->TestCaseName().Match(_L("Test Case 25")) == 0))
				{	
				TTimeIntervalMicroSeconds32 time(1000000);
				iHttpTimer->After(time);
				iASW->Start();	
				}
			if( expectedError != KErrNone )
				{
				_LIT(KTxtErrorExpected, "Client - Expected error code %d");
				iTestUtils.LogIt(KTxtErrorExpected, expectedError);

				EndTestL(KErrNotFound);
				}

			if( iTestCase->TotalTransactionCount() == iCurrentTrans )
				EndTestL(KErrNone);
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
					EndTestL(KErrNone);
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
		case THTTPEvent::EReceiveTimeOut:
			{
			iTestUtils.Test().Printf(_L("Receive TimeOut\n"));
			_LIT(KTxtErrRecvTimeout, "Client - Receive Timeout.");
			iTestUtils.LogIt(KTxtErrRecvTimeout());
			}break;
		case THTTPEvent::ESendTimeOut:
			{
			iTestUtils.Test().Printf(_L("Send TimeOut\n"));
			_LIT(KTxtErrSendTimeout, "Client - Send Timeout.");
			iTestUtils.LogIt(KTxtErrSendTimeout());
			}break;
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
		_LIT(KTxtTitle, "Test Case Pipeline Fallback");
		THTTPHdrVal numberConnectionsVal;
		if (connInfo.Property(numberConnectionsString, numberConnectionsVal))
			{
			numberConnections = numberConnectionsVal.Int();
			connInfo.RemoveProperty(numberConnectionsString);
			
			if ((numberConnections > expectedConnections) && iTestCase->TestCaseName().Match(KTxtTitle) != 0)
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
	__ASSERT_DEBUG(iTestCase!=NULL, User::Invariant());

	if( iTestCase->EnableBatching() )
		{
		// @todo we need to add the session property for enabling batching
		RHTTPConnectionInfo connInfo = GetSession().ConnectionInfo();
		THTTPHdrVal batchingSupport(iSession.StringPool().StringF(HTTP::EEnableBatching,RHTTPSession::GetTable()));
		connInfo.SetPropertyL(iSession.StringPool().StringF(HTTP::EHttpBatching,RHTTPSession::GetTable()), batchingSupport);
		}
		
	if( iTestCase->TransportHandlerPriority() )
		{
		RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
		THTTPHdrVal enableTranspHndlrPriority(iSession.StringPool().StringF(HTTP::EEnableTranspHndlrPriority, RHTTPSession::GetTable()));
		connInfo.SetPropertyL(iSession.StringPool().StringF(HTTP::ETranspHndlrPriority, RHTTPSession::GetTable()), enableTranspHndlrPriority);
		}

	TBool batchTrans = iTestCase->BatchTransactions();

	// Create transactions
	for( TInt ii=0; ii < iTestCase->TotalTransactionCount(); ++ii )
		{
		RHTTPTransaction trans = iTestCase->GetTransactionL(ii, GetSession(), *this);
		CleanupClosePushL(trans);
		AddTransactionL(trans);
		if(!batchTrans)
			trans.SubmitL();
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
	
void CPipeliningTestClient::SetTestCase(MPipeliningTestCase* aTestCase)
	{
	iTestCase = aTestCase;
	}

void CPipeliningTestClient::TimeOut()
	{
	iASW->AsyncStop();		
	}

CHttpTimer1::CHttpTimer1(MTimerClient1& aClient)
:CActive(EPriorityStandard), iClient(aClient)
	{
	CActiveScheduler::Add(this);
	iTimer.CreateLocal();
	}

CHttpTimer1::~CHttpTimer1()
	{
	Cancel();
	iTimer.Close();	
	}
	
void CHttpTimer1::After(TTimeIntervalMicroSeconds32 anInterval)
	{
	iTimer.After(iStatus, anInterval);	
	SetActive();
	}

void CHttpTimer1::DoCancel()
	{
	
	}
	
void CHttpTimer1::RunL()
	{
	iClient.TimeOut();
	}	

