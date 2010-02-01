// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ctestsocketwriter.h"

#include <moutputstream.h>

#include "mtestwriterobserver.h"
#include "thttptrhndtestpanic.h"


GLREF_D TBool gInvalidSecureHost;
GLREF_D TBool gWriterShutdown;
GLREF_D TBool gReqCertificate;

CTestSocketWriter* CTestSocketWriter::NewL(MTestWriterObserver& aObserver, MOutputStream& aOutputStream)
/**	
	Factory constructor.
	@componentInternal		
	@param		aObserver		The socket writer observer.
	@param		aOutputStream	The output stream.
	@return		A pointer to a fully created object.
*/
	{
	return new (ELeave) CTestSocketWriter(aObserver, aOutputStream);
	}

CTestSocketWriter::~CTestSocketWriter()
/**	
	Destructor.
	@componentInternal		
*/
	{
	Cancel();

	// Shutdown the output stream
	if( iOutputStream && iBoundToOutputStream )
		iOutputStream->Close();
	}

CTestSocketWriter::CTestSocketWriter(MTestWriterObserver& aObserver, MOutputStream& aOutputStream)
: CActive(CActive::EPriorityStandard), iObserver(aObserver), iOutputStream(&aOutputStream)
/**	
	Constructor.
	@componentInternal 
	@param		aObserver		The socket writer observer.
	@param		aOutputStream	The output stream.
*/
	{
	CActiveScheduler::Add(this);
	}

void CTestSocketWriter::SendData(const TDesC8& aData)
/**	
	Request to send data to the output stream. The data supplied is sent to the
	output data stream. The supplied descriptor buffer should remain valid until
	the send terminates. The data is sent in packets.
	@componentInternal		
	@param		aData	The data to be sent.
*/
	{
	__ASSERT_DEBUG( iState == EIdle, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EBadSocketWriterState) );

	// Observer wants to send data - store the data.
	iSendData.Set(aData);

	// Bind to the output stream
	iOutputStream->Bind(*this);
	iBoundToOutputStream = ETrue;

	if(iSecure)
		iState = ESecureHandshake;
	else
		iState = ESendData;

	// Move into the SendData state and self-complete
	CompleteSelf();
	}

void CTestSocketWriter::CloseWriter()
/**	
	Closes the socket writer asynchronously.
	@componentInternal		
*/
	{
	__ASSERT_DEBUG( iState == EIdle, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EBadSocketWriterState) );

	// Move into the Closing state and self-complete
	iState = EClosing;
	CompleteSelf();
	}

void CTestSocketWriter::CompleteSelf()
/**	
	Self-completes function. Ensures that the active scheduler calls the RunL()
	at the possible moment.
	@componentInternal		
*/
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

/*
 *	Methods from MOutputStreamObserver
 */

void CTestSocketWriter::SendDataCnfL()
/**
	@see		MOutputStreamObserver
	@componentInternal
*/
	{
	__ASSERT_DEBUG( iState == EPendingAck, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EBadSocketWriterState) );

	// Reset the send buffer
	iSendBuffer.Zero();

	// Is there more data to send?
	if( iSendData.Length() == 0 )
		{
		// Nope - inform the observer that all the data has sent
		iObserver.DataSent();

		// Move to the Idle state
		iState = EIdle;
		}
	else
		{
		// Got more data to send - move to the SendData state and go active
		iState = ESendData;
		CompleteSelf();
		}
	}

void CTestSocketWriter::OutputStreamCloseInd(TInt aError)
/**
	@see		MOutputStreamObserver
	@componentInternal
*/
	{
	// The output stream is no longer valid.
	iOutputStream = NULL;

	// Move into the Closed state
	iState = EClosed;

	// The output stream is closed - inform observer.
	iObserver.WriterClosed(aError);
	}

void CTestSocketWriter::MOutputStreamObserver_Reserved()
/**
	@see		MOutputStreamObserver
	@componentInternal
*/
	{
	User::Invariant();
	}

/*
 *	Methods from CActive
 */

void CTestSocketWriter::RunL()
/**
	Asynchronous request servicing.
	@componentInternal		
*/
	{
	switch( iState )
		{
	case ESecureHandshake:
		{
		if (!gWriterShutdown && !gInvalidSecureHost)
			{
			iOutputStream->SecureClientReq(_L8("lon-cn-lxwaptest2.closedtest.intra"));	
			}		
		else if(gWriterShutdown && gInvalidSecureHost)
			{
			iOutputStream->SecureClientReq(_L8("lon-cn-lxwap.closedtest.intra"));
			iOutputStream->ShutdownReq();
			}		
		else if(!gWriterShutdown && gInvalidSecureHost)
			{
			iOutputStream->SecureClientReq(_L8("google.com"));
			}
		} break;
	case ESendData:
		{
		// Need to get the next buffer's worth of data.
		TInt dataLength = iSendBuffer.MaxLength();

		// Adjust length is there is less data to send than a buffer's worth
		TInt sendLength = iSendData.Length();
		if( sendLength < dataLength )
			{
			dataLength = sendLength;
			}

		// Copy the data into the buffer
		iSendBuffer.Copy(iSendData.Left(dataLength));

		// Update the send data...
		iSendData.Set(iSendData.Mid(dataLength));

		// Send the data to the output stream
		iOutputStream->SendDataReqL(iSendBuffer);

		// Move to the PendingAck state
		iState = EPendingAck;
		
		if (gReqCertificate)
			{	
			iOutputStream->ServerCert();
			}
			
		} break;
	case EClosing:
		{
		// Request close on the output stream
		iOutputStream->ShutdownReq();

		// Move to the PendingClosed state
		iState = EPendingClosed;
		} break;
	default:
		break;
		}
	}

void CTestSocketWriter::DoCancel()
/**	
	Cancel the asynchronous request
*/
	{
	// Nothing to do...
	}

TInt CTestSocketWriter::RunError(TInt /*aError*/)
/**	
	Handle asynchronous request servicing error. The RunL() has left and the 
	error is handled here.
	@componentInternal		
	@param		aError	The error code.
	@return		A value of KErrNone if the error has been handled, or any other
				value if it has not been.
*/
	{
	return KErrNone;
	}

void CTestSocketWriter::SecureClientCnf()
	{
	iState = ESendData;
	CompleteSelf();
	}

void CTestSocketWriter::SetSecure(TBool aSecure)
	{
	iSecure = aSecure;
	}
	
void CTestSocketWriter::OnSendTimeOut()
	{
	
	}

TInt CTestSocketWriter::SendTimeOutVal()
	{
	return 0;
	}
