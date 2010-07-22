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

#include <x509cert.h>

#include "chttprequestbatcher.h"


const TInt KBatchingTimeoutMicroSeconds = 100000;


CHttpRequestBatcher* CHttpRequestBatcher::NewL(MOutputStream& aOutputStream, const TInt aBufferSize)
	{
	CHttpRequestBatcher* self = new (ELeave) CHttpRequestBatcher(aOutputStream, aBufferSize);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CHttpRequestBatcher::CHttpRequestBatcher(MOutputStream& aOutputStream, const TInt aBufferSize)
: CActive(CActive::EPriorityStandard + 1), iDataToSend(TPtr8(NULL, 0)), iExcessData(TPtrC8(NULL, 0)), iMaxBufferSize(aBufferSize)
	{
	__FLOG_OPEN("http", "httpclienthandler.txt");

	CActiveScheduler::Add(this);
	iOutputStream = &aOutputStream;
	iOutputStream->Bind(*this);

	iFuncPtr = &CHttpRequestBatcher::SendRequestImmediatelyL;
	}

void CHttpRequestBatcher::ConstructL()
	{
	iBuffer = HBufC8::NewL(iMaxBufferSize);
	iDataToSend.Set(iBuffer->Des());

	iTimer.CreateLocal();
	iTimerCompleted = EFalse;

	}


CHttpRequestBatcher::~CHttpRequestBatcher()
	{
	Cancel();
	delete iBuffer;
	iTimer.Close();

	__FLOG_CLOSE;
	}


void CHttpRequestBatcher::SendRequestImmediatelyL(const TDesC8& aBuffer)
	{
#if defined (_DEBUG) && defined (_LOGGING)
		__FLOG_0(_T8("!! Send the first request i.e. not batching"));
#endif

	iOutputStream->SendDataReqL(aBuffer);
	}

void CHttpRequestBatcher::SendRequestsBatchedL(const TDesC8& aBuffer)
	{
	StartTimer();

	BatchRequestsL(aBuffer);
	}

void CHttpRequestBatcher::BatchRequestsL(const TDesC8& aBuffer)
	{
	if (iTimerCompleted)
		iExcessData.Set(aBuffer);

	else if ((aBuffer.Length() + iDataToSend.Length()) < iMaxBufferSize)
		{
		iDataToSend.Append(aBuffer);
		
		// Buffer is not yet full, so request observer to send more data
		iObserver->SendDataCnfL();
		}
	else
		{
		TInt pos = iMaxBufferSize - iDataToSend.Length();

		// Fill up the buffer
		iDataToSend.Append(aBuffer.Left(pos));

		// Cancel the timer
		Cancel();

#if defined (_DEBUG) && defined (_LOGGING)
		__FLOG_0(_T8("!! Buffer has been filled - cancel timer and send batched requests now"));
#endif

		// Send the request with a filled-up buffer
		iOutputStream->SendDataReqL(iDataToSend);

		// Save the excess data from this request
		iExcessData.Set(aBuffer.Mid(pos));
		}
	}

void CHttpRequestBatcher::StartTimer()
	{
	if(!IsActive())
		{
#if defined (_DEBUG) && defined (_LOGGING)
		__FLOG_0(_T8("!! Start the batching timer"));
#endif

		SetActive();
		iTimer.After(iStatus, KBatchingTimeoutMicroSeconds);
		}
	}


/*
 *	Methods from MOutputStream
 */

void CHttpRequestBatcher::Bind(MOutputStreamObserver& aObserver)
	{
	// Bind to the output stream observer
	iObserver = &aObserver;
	}

void CHttpRequestBatcher::SendDataReqL(const TDesC8& aBuffer)
	{
	// Call the appropriate function depending on what the function pointer has been set to.
	// For the first request, the pointer will be set to the SendRequestImmediately() function.
	// For subsequent requests, i.e. when batching, the pointer will be set to the
	// SendRequestsBatched() function.

	(*this.*iFuncPtr)(aBuffer);
	}

void CHttpRequestBatcher::ShutdownReq()
	{
	iOutputStream->ShutdownReq();
	}

void CHttpRequestBatcher::SecureClientReq(const TDesC8& aHostName)
	{
	iOutputStream->SecureClientReq(aHostName);
	}

void CHttpRequestBatcher::Close()
	{
	iOutputStream->Close();
	}

const CX509Certificate* CHttpRequestBatcher::ServerCert()
 	{
	return iOutputStream->ServerCert();
 	}
	
TInt CHttpRequestBatcher::CipherSuite(TDes8& aCipherSuite)
	{
	return iOutputStream->CipherSuite(aCipherSuite);
	}

void CHttpRequestBatcher::MOutputStream_Reserved()
	{
	User::Invariant();
	}

void CHttpRequestBatcher::Reset ()
	{
	iOutputStream->Reset ();
	}

/*
 *	Methods from MOutputStreamObserver
 */

void CHttpRequestBatcher::SendDataCnfL()
	{
	// If this is the first request send a confirmation to the observer that the data
	// has been sent and then set the function pointer to the SendRequestBatched() method.
	if (iFuncPtr == &CHttpRequestBatcher::SendRequestImmediatelyL)
		{
#if defined (_DEBUG) && defined (_LOGGING)
		__FLOG_0(_T8("!! Received conf back for first request i.e. not batching"));
#endif

		iObserver->SendDataCnfL();
		iFuncPtr = &CHttpRequestBatcher::SendRequestsBatchedL;
		}

	// If the timer did not complete then by implication a filled-up buffer was sent in the request.
	else if (!iTimerCompleted)
		{
#if defined (_DEBUG) && defined (_LOGGING)
		__FLOG_0(_T8("!! Received conf back for batched requests"));
#endif

		iDataToSend.Zero();
		HandleExcessDataL();
		}
	else	// Timer did complete
		{
		iDataToSend.Zero();

		// Check for any requests that may have been sent by the observer after the RunL fired
		// i.e. the timer completed, but before we received a conf from the output stream.
		if (iExcessData.Length() > 0)
			HandleExcessDataL();

		// Reset the flag
		iTimerCompleted = EFalse;
		}
	}

void CHttpRequestBatcher::HandleExcessDataL()
	{
	if (iExcessData.Length() > iMaxBufferSize)
		{
#if defined (_DEBUG) && defined (_LOGGING)
		__FLOG_0(_T8("!! Handling excess data - have enough data to fill up another buffer so send it"));
#endif

		// It's possible that the excess data is larger than the maximum allowable size of the buffer in
		// which case this excess data is used to fill up another buffer and a request made.
		iDataToSend.Append(iExcessData.Left(iMaxBufferSize));
		iOutputStream->SendDataReqL(iDataToSend);
		iExcessData.Set(iExcessData.Mid(iMaxBufferSize));
		}
	else
		{
		// Not enough data to fill up a buffer therefore store the data
		iDataToSend.Append(iExcessData);
		iExcessData.Set(KNullDesC8());
		iObserver->SendDataCnfL();

#if defined (_DEBUG) && defined (_LOGGING)
		__FLOG_0(_T8("!! Handling excess data - but not enough data to fill up buffer"));
#endif

		// start the timer again
		StartTimer();			
		}
	}


void CHttpRequestBatcher::SecureClientCnf()
	{
	iObserver->SecureClientCnf();
	}

void CHttpRequestBatcher::OutputStreamCloseInd(TInt aError)
	{
	Cancel();
	iObserver->OutputStreamCloseInd(aError);
	}
	
TBool CHttpRequestBatcher::SecureRetry()
     {
         return iObserver->SecureRetry();
     }

void CHttpRequestBatcher::MOutputStreamObserver_Reserved()
	{
	User::Invariant();
	}


/*
 * Methods from CActive
 */

void CHttpRequestBatcher::RunL()
	{
	if (iDataToSend.Length() > 0)
		{
#if defined (_DEBUG) && defined (_LOGGING)
		__FLOG_0(_T8("!! Timer has completed before buffer has been filled - send request now"));
#endif

		// Timer has completed before the buffer has been filled, therefore send the request now.
		iOutputStream->SendDataReqL(iDataToSend);
		}
	iTimerCompleted = ETrue;
	}

void CHttpRequestBatcher::DoCancel()
	{
	iTimer.Cancel();
	}

TInt CHttpRequestBatcher::RunError(TInt /*aError*/)
	{
	// The RunL() cannot leave - nothing to do here.
	return KErrNone;
	}

void CHttpRequestBatcher::OnSendTimeOut()
	{
	iObserver->OnSendTimeOut();
	}
 
TInt CHttpRequestBatcher::SendTimeOutVal()
	{
	return iObserver->SendTimeOutVal();
	}

void CHttpRequestBatcher::SetTCPCorking(TBool  aValue )
    {
    if (iOutputStream)
            {
            iOutputStream->SetTCPCorking(aValue);
            }
    }


