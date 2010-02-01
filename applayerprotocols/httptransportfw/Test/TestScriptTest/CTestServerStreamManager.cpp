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


#include <minputstream.h>
#include <moutputstream.h>

#include "CTestServerStreamManager.h"
#include "httptestutils.h"
#include "MPipeliningTestCase.h"

const TInt KTimeOut = 50000000;
const TInt KResponseBatchSize = 5;
_LIT8(KTxtConnectionClose, "Connection: Close");

CTestServerStreamManager* CTestServerStreamManager::NewL(CHTTPTestUtils& aTestUtils, TInt aConnectionIndex, MPipeliningTestCase* aTestCase, MInputStream* aInputStream, MOutputStream* aOutputStream)
	{
	CTestServerStreamManager* self = new (ELeave) CTestServerStreamManager(aTestUtils, aConnectionIndex, aTestCase, aInputStream, aOutputStream);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestServerStreamManager::CTestServerStreamManager(CHTTPTestUtils& aTestUtils, TInt aConnectionIndex, MPipeliningTestCase* aTestCase, MInputStream* aInputStream, MOutputStream* aOutputStream)
: CTimer(EPriorityNormal), iTestUtils(aTestUtils), iInputStream(aInputStream), iOutputStream(aOutputStream), iTestCase(aTestCase), iConnectionIndex(aConnectionIndex)
	{
	CActiveScheduler::Add(this);

	iTransCount = iTestCase->TransactionCount(iConnectionIndex);
	
	iInputStream->Bind(*this);
	iOutputStream->Bind(*this);
	}

CTestServerStreamManager::~CTestServerStreamManager()
	{
	Cancel();
	if(iInputStream != NULL)
		{
		iInputStream->Close();
		}
	if(iOutputStream != NULL)
		{
		iOutputStream->Close();
		}
	delete iDataStore;
	delete iDataToSend;
	
	delete iHttpTimer;
	delete iASW;
	}

void CTestServerStreamManager::ConstructL()
	{
	CTimer::ConstructL();
	iHttpTimer = new(ELeave) CHttpTimer(*this);
	iASW = new(ELeave) CActiveSchedulerWait();
	}
	
// From MInputStreamObserver
void CTestServerStreamManager::ReceivedDataIndL(const TDesC8& aBuffer)
	{
	// Got data
	_LIT(KTxtGotData, "Server - received request data.");
	iTestUtils.LogIt(KTxtGotData());
	iTestUtils.DumpData(aBuffer, ETrue);

	if(iDataStore == NULL)
		{
		iDataStore = aBuffer.AllocL(); // First data chunk received
		}
	else
		{
		// Already got data before so append the data to the existing data
		TInt newLength = (iDataStore->Length()) + aBuffer.Length();
		iDataStore = iDataStore->ReAllocL( newLength );
		TPtr8 buf = iDataStore->Des();
		buf.Append(aBuffer);
		}
	
	iInputStream->ReceivedDataRes();
	
	if(iTestCase->ErrorVal() == KErrDisconnected)	
		{
		iInputStream->ShutdownReq();
		}
	else
		{
		// Try processing the requests
		SendDataL();
		}
	}
	
void CTestServerStreamManager::SecureServerCnf()
	{
	}
	
void CTestServerStreamManager::InputStreamCloseInd(TInt aError)
	{
	_LIT(KTxtInputClosed, "Server - Input stream %d closed. Error: %d.");
	iTestUtils.LogIt(KTxtInputClosed(), iConnectionIndex + 1, aError);

	iInputStream = NULL;
	}
	
void CTestServerStreamManager::MInputStreamObserver_Reserved()
	{
	User::Invariant();
	}

// From MOutputStreamObserver
void CTestServerStreamManager::SendDataCnfL()
	{
	_LIT(KTxtDataSent, "Server - Data Sent.");
	iTestUtils.LogIt(KTxtDataSent());
	_LIT(KTxtTitle, "Defect Fix INC114315");
 	if (iTestCase->TestCaseName().Match(KTxtTitle) == 0)
 		    {
 		    _LIT(KTxtCloseConn, "Server - Closing connection.");
 		    iTestUtils.LogIt(KTxtCloseConn());
 		    iInputStream->ShutdownReq();
 		    }
	if( iMoreResponseBatches )
		{
		// If there is more batches to process try and process them
		iMoreResponseBatches = EFalse;
		TRAPD(err, SendDataL());
		if(err != KErrNone)
			{
			_LIT(KTxtSendError, "Server - Error %d sending data. Closing stream.");
			iTestUtils.LogIt(KTxtSendError(), err);
			iInputStream->ShutdownReq();
			}
		}
	else
		{
		delete iDataToSend;
		iDataToSend = NULL;

		// Do we need to close the connection
		if(iCloseConnection)
			{
			_LIT(KTxtCloseConn, "Server - Closing connection.");
			iTestUtils.LogIt(KTxtCloseConn());

			if( (iTestCase->TestCaseName().Match(_L("Test Case 3")) == 0) 
				|| (iTestCase->TestCaseName().Match(_L("CINC073400")) == 0) 
				|| (iTestCase->TestCaseName().Match(_L("Test Case 19")) == 0) 
				|| (iTestCase->TestCaseName().Match(_L("Test Case INC073400")) == 0) )
				{
				TTimeIntervalMicroSeconds32 time(5000000);
				iHttpTimer->After(time);
				iASW->Start();
				}

			if(iInputStream)
				iInputStream->ShutdownReq();
			}
			_LIT(KTxtTitle, "Defect Fix CINC077703");
		if (iTestCase->TestCaseName().Match(KTxtTitle) == 0)
			{

			iInputStream->ShutdownReq();
			}
		}
	}
	
void CTestServerStreamManager::SecureClientCnf()
	{
	}
	
void CTestServerStreamManager::OutputStreamCloseInd(TInt aError)
	{
	_LIT(KTxtOutputClosed, "Server - Output stream %d closed. Error: %d.");
	iTestUtils.LogIt(KTxtOutputClosed(), iConnectionIndex + 1, aError);

	iOutputStream = NULL;
	}
	
void CTestServerStreamManager::MOutputStreamObserver_Reserved()
	{
	}

TBool CTestServerStreamManager::ProcessRequestL()
	{
	TBool processingRequest = ETrue;
	TInt currentBatch = 0;
	while( (processingRequest && (iCurrentTrans < iTransCount)) && (currentBatch<KResponseBatchSize) )
		{
		// Do we have enough data to respond to the current transaction?
		TPtrC8 rawRequest = iTestCase->GetRawRequest(iConnectionIndex, iCurrentTrans);
		TInt requestLength = rawRequest.Length();
		TPtrC8 dataWindow = iDataStore->Mid(iDataPos);
		if( requestLength <= dataWindow.Length() )
			{
			// Check that the raw request and the actual request match
			if( dataWindow.FindF(rawRequest) != 0 )
				{
				_LIT(KTxtRequestDataMismatch, "Server - Fail. Request data for transaction %d does not match expected data.");
				iTestUtils.LogIt(KTxtRequestDataMismatch(), iCurrentTrans + 1);
				_LIT(KTxtExpectedData, "Server - Expected data: %S");
				HBufC* rawRequest16 = HBufC::NewL(rawRequest.Length());
				HBufC* dataWindow16 = HBufC::NewL(dataWindow.Length());
				(rawRequest16->Des()).Copy(rawRequest);
				(dataWindow16->Des()).Copy(dataWindow);
				TPtrC rawRequest16Ptr = rawRequest16->Des();
				TPtrC dataWindow16Ptr = dataWindow16->Des();
				iTestUtils.LogIt(KTxtExpectedData(), &rawRequest16Ptr);
				iTestUtils.LogIt(KTxtExpectedData(), &dataWindow16Ptr);

				User::Leave(KErrNotFound);
				}

			// Prepare the response data to send
			iDataPos += requestLength;
			processingRequest = ETrue;
			TPtrC8 rawResponse = iTestCase->GetRawResponse(iConnectionIndex, iCurrentTrans);
			if(iDataToSend == NULL)
				{
				iDataToSend = rawResponse.AllocL();
				}
			else
				{
				TInt responseLength = rawResponse.Length();
				iDataToSend = iDataToSend->ReAllocL( (iDataToSend->Length()) + responseLength );
				TPtr8 buffer = iDataToSend->Des();
				buffer.Append(rawResponse);
				}
			
			// Check for a Connection: Close in the request
			iCloseConnection = IsConnectionCloseInData(rawRequest, rawResponse);
			if(iCloseConnection)
				{
				processingRequest = EFalse;
				}

			++iCurrentTrans;
			++currentBatch;
			}
		else
			{
			// No more requests can be processed
			processingRequest = EFalse;
			}

		// Flag that we have more processing to do with we are still processing the data
		// but we have reached the maximum batch size.
		if( processingRequest && currentBatch == KResponseBatchSize)
			{
			iMoreResponseBatches = ETrue;
			}
		else
			{
			iMoreResponseBatches = EFalse;
			}
		}

	if(iDataToSend != NULL)
		{
		return ETrue;
		}

	return EFalse;
	}

TBool CTestServerStreamManager::IsConnectionCloseInData(const TDesC8& aRequest, const TDesC8& aResponse) const
	{
	if( (aRequest.FindF(KTxtConnectionClose()) != KErrNotFound) || (aResponse.FindF(KTxtConnectionClose()) != KErrNotFound) )
		{
		// Either the request or response has Connection: Close header
		_LIT(KTxtConnectionCloseDetected, "Server - Connection: Close detected in request or response, transaction: %d");
		iTestUtils.LogIt(KTxtConnectionCloseDetected(), iCurrentTrans + 1);

		return ETrue;
		}

	return EFalse;
	}

void CTestServerStreamManager::SendDataL()
	{
	Cancel();
	
	_LIT(KTcTitle, "CRecvTimeOut");
	if (iTestCase->TestCaseName().Match(KTcTitle) == 0)
		{
		_LIT(KTxtTimeout, "Server - Data Send Timeout.");
		iTestUtils.LogIt(KTxtTimeout());
		After(61000000);
		return;
		}

	if( !iMoreResponseBatches )
		{
		// Try processing the requests
		if( ProcessRequestL() )
			{
			// We have enough, send the response.
			iOutputStream->SendDataReqL(*iDataToSend);
			
			_LIT(KTxtSendData, "Server - Sending response data.");
			iTestUtils.LogIt(KTxtSendData());
			iTestUtils.DumpData(*iDataToSend, ETrue);
			}
		else
			{
			After(KTimeOut); // Start the inactivity timer
			}
		}
	}

void CTestServerStreamManager::RunL()
	{
	_LIT(KTcTitle, "CRecvTimeOut");
	if (iTestCase->TestCaseName().Match(KTcTitle) == 0)
		{
		_LIT(KTxtTimeout, "TC-Receive Timeout Timer RunL.");
		iTestUtils.LogIt(KTxtTimeout());
		return;
		}

	if( iStatus.Int() == KErrNone )
		{
		// The connection has timed out.
		_LIT(KTxtTimedOut, "Server - Fail, Connection %d timed out. Current transaction: %d");
		iTestUtils.LogIt(KTxtTimedOut(), iConnectionIndex + 1, iCurrentTrans + 1);

		iInputStream->ShutdownReq();
		}
	else
		{
			User::LeaveIfError(iStatus.Int());
		}	
	}

MHttpResponse* CTestServerStreamManager::CurrentResponse()
	{
	return NULL;
	}

void CTestServerStreamManager::OnReceiveTimeOut()
	{
	
	}

void CTestServerStreamManager::OnSendTimeOut()
	{
	
	}

TInt CTestServerStreamManager::SendTimeOutVal()
	{
	return 0;
	}


void CTestServerStreamManager::TimeOut()
	{
	iASW->AsyncStop();	
	}


CHttpTimer::CHttpTimer(MTimerClient& aClient)
:CActive(EPriorityStandard), iClient(aClient)
	{
	CActiveScheduler::Add(this);
	iTimer.CreateLocal();
	}

CHttpTimer::~CHttpTimer()
	{
	Cancel();
	iTimer.Close();	
	}
	
void CHttpTimer::After(TTimeIntervalMicroSeconds32 anInterval)
	{
	iTimer.After(iStatus, anInterval);	
	SetActive();
	}

void CHttpTimer::DoCancel()
	{
	
	}
	
void CHttpTimer::RunL()
	{
	iClient.TimeOut();
	}	
