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

#include "ctestsocketreader.h"

#include <minputstream.h>
#include <sslerr.h>

#include "mtestreaderobserver.h"
#include "thttptrhndtestpanic.h"

GLREF_D TBool gReaderShutdown;

CTestSocketReader* CTestSocketReader::NewL(MTestReaderObserver& aObserver, MInputStream& aInputStream, TBool aSecure)
/**	
	Factory constructor.
	@componentInternal		
	@param		aObserver		The test socket reader observer.
	@param		aInputStream	The input stream.
	@param		aSecure			Flag indicating a secure connection.
	@return		A pointer to a fully initialised object.
*/
	{
	return new (ELeave) CTestSocketReader(aObserver, aInputStream, aSecure);
	}

CTestSocketReader::~CTestSocketReader()
/**	
	@componentInternal		
*/
	{
	Cancel();

	// Close the input stream
	if( iInputStream && iBoundToInputStream )
		iInputStream->Close();
	}

CTestSocketReader::CTestSocketReader(MTestReaderObserver& aObserver, MInputStream& aInputStream, TBool aSecure)
: CActive(CActive::EPriorityStandard), iObserver(aObserver), iInputStream(&aInputStream), iSecure(aSecure)
/**	
	Constructor.
	@componentInternal		
	@param		aObserver		The test socket reader observer.
	@param		aInputStream	The input stream.
	@param		aSecure			Flag indicating a secure connection.
*/
	{
	CActiveScheduler::Add(this);
	}

void CTestSocketReader::ExpectData(const TDesC8& aData)
/**	
	Request to expect data from the input stream. The data supplied is compared
	to that received from the input stream. The supplied descriptor buffer 
	must remain valid until the receive terminates.
	@componentInternal		
	@param		aData	The data to be received.
*/
	{
	__ASSERT_DEBUG( iState == EIdle, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EBadSocketReaderState) );

	// Observer expects a request - store the expected data.
	iExpectedData.Set(aData);

	// Bind to the input stream
	iInputStream->Bind(*this);
	iBoundToInputStream = ETrue;

	// Move into the PendingReceivedData state
	iState = EPendingReceivedData;
	}

TInt CTestSocketReader::UpdateData(const TDesC8& aReceivedData, TPtrC8& aExcessData)
/**	
	Checks the receivced data and updates the remaining expected data. The data
	received is checked to see if matches that in the expected data. If so the
	remaining expected data is updated to not include the data received. If all 
	the expected data has been received and the received data packet contains 
	more data, then this is returned through the output argument.
	@componentInternal		
	@param		aReceivedData	The descriptor buffer containing the recived data
								packet.
	@param		aExcessData		An output argument that is given is set to any
								excess data in the received data packet.
	@return		A value of KErrNone if the received data packet is part of the 
				expected data. A value of KErrNotFound if it is not.
*/
	{
	// Check to see if the received buffer is part of the expected data
	TInt receivedLength = aReceivedData.Length();
	TInt expectedLength = iExpectedData.Length();

	if( expectedLength < receivedLength )
		{
		// There is more received data than what's left of the expected data - 
		// this implies pipelining...
		receivedLength = expectedLength;
		}
	
	// Check the received data against the expected data
	TInt err = KErrNone;	
	if( iExpectedData.Left(receivedLength).Compare(aReceivedData.Left(receivedLength)) == 0 )
		{
		// The received data matches the expected data - update the expected 
		// data to remove the received part.
		iExpectedData.Set(iExpectedData.Mid(receivedLength));
		
		// Is there any more?
		if( iExpectedData.Length() == 0 )
			{
			// Set any execess data...
			aExcessData.Set(aReceivedData.Mid(receivedLength));
			}
		}
	else
		{
		err = KErrNotFound;
		}
	return err;
	}

TInt CTestSocketReader::CheckResponseStatusL(const TDesC8& aReceivedData)
/**	
	Checks the received data and searches for the expected status line. A flag is
	set once its is found and any remaining data is allowed to be received.
	@componentInternal		
	@param		aReceivedData	The descriptor buffer containing the recieved data
								packet.
	@return		A value of KErrNone if the received data contains the expected status
				line or there is not enough received data to check. KErrNotFound if
				the status line received does not match what is expected.
*/
	{
	TInt foundStatus = 0;
	// Check if we have enough data to check the status
	if(aReceivedData.Length() >= iExpectedData.Length())
		{
		// We have enough data to check the status code
		foundStatus = aReceivedData.FindF(iExpectedData);

		// If we have the correct status then we dont need to worry about any more data
		if(foundStatus == 0)
			iSecureTestPassed = ETrue;
		}
	
	return foundStatus;
	}

void CTestSocketReader::CompleteSelf()
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

void CTestSocketReader::CloseReader()
/**	
	Closes the socket reader asynchronously.
	@componentInternal		
*/
	{
	__ASSERT_DEBUG( iState == EIdle, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EBadSocketReaderState) );

	// Move into the Closing state and self-complete
	iState = EClosing;
	CompleteSelf();
	}

/*
 *	Methods from MInputStreamObserver
 */

void CTestSocketReader::ReceivedDataIndL(const TDesC8& aBuffer)
/**	
	@see		MInputStreamObserver
	@componentInternal
*/
	{
	__ASSERT_DEBUG( iState == EPendingReceivedData, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EBadSocketReaderState) );

	if(iSecure)
		{
		// If we are secure we are connecting to real servers so only check for the correct status line
		// and make sure that the test hasn't already passed from the first chunk of data
		if(!iSecureTestPassed)
			User::LeaveIfError(CheckResponseStatusL(aBuffer));
		}
	else
		{
		// Check the received data...
		TPtrC8 excess;
		#ifdef _DEBUG
			TBool match = KErrNone ==
		#endif
		UpdateData(aBuffer, excess);

		__ASSERT_DEBUG( match, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EReceivedBadData) );

		// The data matched - pipelining not supported.
		__ASSERT_DEBUG( excess.Length() == 0, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EReceivedPipelinedData) );
		}

	// Need to ack the received data. Move to the ReceiveAck state and self-
	// complete.
	iState = EReceiveAck;
	CompleteSelf();
	}

void CTestSocketReader::InputStreamCloseInd(TInt aError)
/**	
	@see		MInputStreamObserver
	@componentInternal
*/
	{
	// If this is a secure connection the data has finished receiving and the socket has sent KErrEof
	// so inform the observer that the data has been received
	if( (aError==KErrEof || aError==KErrSSLAlertCloseNotify) && iSecureTestPassed )
		iObserver.DataReceived();

	// The input stream is no longer valid.
	iInputStream = NULL;

	// Move into the Closed state
	iState = EClosed;

	// The input stream is closed - inform observer.
	iObserver.ReaderClosed(aError);
	}

void CTestSocketReader::MInputStreamObserver_Reserved()
/**	
	@see		MInputStreamObserver
	@componentInternal
*/
	{
	User::Invariant();
	}

/*
 *	Methods from CActive
 */

void CTestSocketReader::RunL()
/**	
	Asynchronous request servicing.
	@componentInternal		
*/
	{
	switch( iState )
		{
	case EReceiveAck:
		{
		// Notify the input stream that the current data part has been finished
		// with and check to see if more request data is expected.
		iInputStream->Bind(*this);

		iInputStream->ReceivedDataRes();

		
		if( iExpectedData.Length() == 0 )
			{
			// The request is complete - inform the observer
			iObserver.DataReceived();

			// Move to the Idle state
			iState = EIdle;
			if (gReaderShutdown)
				{
				iInputStream->ShutdownReq();
				}
			}
		else
			{
			// More request data expected - move to the PendingReceivedData
			iState = EPendingReceivedData;
			}
		} break;
	case EClosing:
		{
		// Request close on the input stream
		iInputStream->ShutdownReq();

		// Move to the PendingClosed state
		iState = EPendingClosed;
		} break;
	default:
		break;
		}
	}

void CTestSocketReader::DoCancel()
/**	
	Cancel the asynchronous request
*/	{
	// Nothing to do...
	}

TInt CTestSocketReader::RunError(TInt /*aError*/)
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

void CTestSocketReader::SecureServerCnf()
	{
	}
	
MHttpResponse* CTestSocketReader::CurrentResponse()
	{
	return NULL;
	}

void CTestSocketReader::OnReceiveTimeOut()
	{
	
	}
