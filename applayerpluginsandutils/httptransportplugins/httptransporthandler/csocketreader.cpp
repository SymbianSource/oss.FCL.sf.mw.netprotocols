// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "csocketreader.h"

#include "csocket.h"
#include "msocketcontroller.h"
#include "minputstreamobserver.h"
#include "httptransporthandlercommon.h"
#include "thttptrlayerpanic.h"
#include "chttpasyncwaiter.h"

CSocketReader* CSocketReader::NewL(CSocket& aSocket, MSocketController& aController, TInt aRecvBufferSize, TBool aPriority)
/**	
	The factory constructor.
	@param		aSocket		The connected socket. This owned by the observer.
	@param		aController	The socket controller that owns the socket.
	@return		A pointer to a fully constructed object.
	@internalComponent
*/
	{
	CSocketReader* self = new (ELeave) CSocketReader(aSocket, aController, aPriority);
	CleanupStack::PushL(self);
	self->ConstructL( aRecvBufferSize );
	CleanupStack::Pop(self); 
	return self;
	}

void CSocketReader::ConstructL( TInt aRecvBufferSize )
	{
	iBuffer.CreateL ( aRecvBufferSize );
	}

CSocketReader::~CSocketReader()
/**	
	Destructor.
	@internalComponent
*/
	{
	__ASSERT_DEBUG( iState == EIdle || iState == EClosed, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadInputStreamState) );
    
	//Delete the Timer Object.
	delete iReceiveTimer;

	// Cancel any outstanding requests
	Cancel();
	iBuffer.Close ();
//	__FLOG_CLOSE;
	}

CSocketReader::CSocketReader(CSocket& aSocket, MSocketController& aController, TBool aPriority)
: CActive(CActive::EPriorityStandard), iSocket(aSocket), iController(aController)
/**	
	Constructor.
	@param		aSocket		The connected socket. This owned by the observer.
	@param		aController	The socket controller that owns the socket.
	@internalComponent
*/
	{
	if(aPriority)
		{
		CActive::SetPriority(CActive::EPriorityHigh);	
		}
	CActiveScheduler::Add(this);

//	__FLOG_OPEN("http", "httptransporthandler.txt");
	}

void CSocketReader::CompleteSelf()
/**	
	Requests that the input stream complete itself. This will caused the RunL()
	to be called by the scheduler at the next opportunity.
	@pre		The input stream is not active.
	@post		The input stream object is active and the request has been
				completed.
	@internalComponent				
*/
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CSocketReader::SocketClosed(TInt aError)
/**	
	Notifies the input stream that the socket is closed. The input stream 
	observer is notified that the stream is closed. No more data can be received
	from the socket.
	@param		aError	The error code explaining why the socket has closed. A
						value of KErrNone indicates that the output stream 
						observer requested that the socket be closed.
	@pre		None.
	@post		The input stream is in the Closed state.
	@internalComponent
*/
	{
	// Cancel any outstanding requests
	Cancel();

	// The socket has shutdown. Inform the input stream observer that the input
	// stream is closed.
	if( iObserver )
		iObserver->InputStreamCloseInd(aError);

	// Move to the Closed state
	iState = EClosed;
	}

void CSocketReader::Suspend()
/**
	Notifies the input stream that it should suspend its activity.
	@pre		The input stream is not already suspended.
	@post		Any pending read is cancelled and the stream is suspended.
	@internalComponent
*/
	{
	__ASSERT_DEBUG( !iSuspended, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EInputStreamAlreadySuspended) );

	// Cancel any outstanding requests
	Cancel();

#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KIpv6MaxAddrSize> ip;
	TUint16	remotePort;
	TUint16 localPort;
	iController.ConnectionInfo(ip, remotePort, localPort);

	__FLOG_0(_T8("!! Suspending input stream"));
	__FLOG_3(_T8("-> on local port %d with %S, remote port %d"), localPort, &ip, remotePort);
#endif

	// Check to see a read requests had been issued.
	if( iState == EReceivedData )
		{
		// Yep - change to the Read state so that read request is re-issued when
		// the stream resumes.
		iState = ERead;
		}

	// Stream is now suspended.
	iSuspended = ETrue;
	}

void CSocketReader::Resume()
/**
	Notifies the input stream that it should resume its activity.
	@pre		The input stream is suspended.
	@post		The stream resumes. A read request is issued if required. 
	@internalComponent
*/
	{
	__ASSERT_DEBUG( iSuspended, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EInputStreamNotSuspended) );

#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KIpv6MaxAddrSize> ip;
	TUint16	remotePort;
	TUint16 localPort;
	iController.ConnectionInfo(ip, remotePort, localPort);

	__FLOG_0(_T8("!! Resumimng input stream"));
	__FLOG_3(_T8("-> on local port %d with %S, remote port %d"), localPort, &ip, remotePort);
#endif

	// Check to see a read requests needs to be issued.
	if( iState == ERead )
		{
		// Yep - self complete to get the read request issued.
		CompleteSelf();
		}

	// Stream is no longer suspended.
	iSuspended = EFalse;
	}

/*
 *	Methods from MInputStream
 */

void CSocketReader::Bind(MInputStreamObserver& aObserver)
/**	
	@see		MInputStream
	@internalComponent
*/
	{
	__ASSERT_DEBUG( iState == EIdle || iState == EPendingAck, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadInputStreamState) );

	// Bind to the input stream observer
	iObserver = &aObserver;
	
	if( iState == EIdle )
		{
		// Move to the Read state and self complete, only if not suspended.
		iState = ERead;
		if( !iSuspended )
			CompleteSelf();
		}
	// Otherwise, stay in the PendingAck state and wait for input stream 
	// observer to notify that it has finished with the buffer.
	}

void CSocketReader::ReceivedDataRes()
/**	
	@see		MInputStream
	@internalComponent
*/
	{
	__ASSERT_DEBUG( iObserver != NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EInputStreamNotBound) );
	__ASSERT_DEBUG( iState == EPendingAck, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadInputStreamState) );

	// The input stream observer has finished with the received data buffer. 
	// Reset the buffer and prepare to read the input stream again.
	iBuffer.Zero();

	// Move to the Read state and self complete, only if not suspended.
	iState = ERead;
	if( !iSuspended )
		CompleteSelf();
	}

void CSocketReader::ShutdownReq()
/**	
	@see		MInputStream
	@internalComponent
*/
	{
	__ASSERT_DEBUG( iObserver != NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EInputStreamNotBound) );
	__ASSERT_DEBUG( iState != EClosing || iState != EClosed, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadInputStreamState) );

	// Cancel any outstanding requests
	Cancel();

#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KIpv6MaxAddrSize> ip;
	TUint16	remotePort;
	TUint16 localPort;
	iController.ConnectionInfo(ip, remotePort, localPort);

	__FLOG_0(_T8("!! Shutting down input stream"));
	__FLOG_3(_T8("-> on local port %d with %S, remote port %d"), localPort, &ip, remotePort);
#endif

	// Shutdown the socket.
	iSocket.Shutdown(iStatus);

	// Move to the Closing state and go active
	iState = EClosing;
	SetActive();
	}

void CSocketReader::Close()
/**	
	@see		MInputStream
	@internalComponent
*/
	{
	__ASSERT_DEBUG( iObserver != NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EInputStreamNotBound) );
	__ASSERT_DEBUG( iState != EClosing || iState != EClosed, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadInputStreamState) );	

	// Cancel any outstanding requests
	Cancel();

#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KIpv6MaxAddrSize> ip;
	TUint16	remotePort;
	TUint16 localPort;
	iController.ConnectionInfo(ip, remotePort, localPort);

	__FLOG_0(_T8("!! Closing input stream"));
	__FLOG_3(_T8("-> on local port %d with %S, remote port %d"), localPort, &ip, remotePort);
#endif

	// There is no need to do anything here - by informing the socket controller
	// that the input stream is closed it will notify the output stream and then
	// close the socket.

	// Move to the Closed state
	iState = EClosed;

	// Inform the socket controller that the input stream is closed
	iController.StreamClosed(KErrCancel, MSocketController::EInputStream);
	}

const CX509Certificate* CSocketReader::ClientCert()
/**	
	@see		MInputStream
	@internalComponent
*/
	{
	return NULL;
	}

void CSocketReader::StartReceieveTimer (TInt aTimeoutValue)
	{
	//Get the TimeOut Value.
	if(aTimeoutValue > 0)
		{
		TInt timeoutValue = 0;
		//Convert to Microseconds.
		if(aTimeoutValue <= KMinTimeoutValue)
			{
			//Default Value is 60 Seconds
			timeoutValue = KDefTimeoutValue;
			}
		else
			{
			timeoutValue = aTimeoutValue * KMicrovalue;
			}
		
		//Create and Start the HTTP timer
		if(!iReceiveTimer)
			{
			iReceiveTimer = CHttpTimer::NewL(*this);
			}
		iReceiveTimer->After(timeoutValue);
		}
	}

/*
 *	Methods from CActive
 */

void CSocketReader::RunL()
/**	
	The request servicing function. Behaviour depends on the state of the input
	stream.
	@internalComponent
*/
	{
	__ASSERT_DEBUG( iObserver != NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EInputStreamNotBound) );

	// Leave if the socket reported an error
	User::LeaveIfError(iStatus.Int());

	switch( iState )
		{
	case ERead:
		{
		iBuffer.Zero ();
		// Request a read from the socket
		iSocket.RecvOneOrMore(iBuffer, iStatus);

		// Move to the Received state and go active - wait for read to complete
		iState = EReceivedData;
		SetActive();		
		} break;
	case EReceivedData:
		{
		__OOM_LEAVE_TEST

		// Received data - move to the PendingAck state and wait for observer to 
		// notify that it has finished with the buffer.
		iState = EPendingAck;
        
		if(iReceiveTimer)
			{
			iReceiveTimer->Cancel();
			}

		// Inform the observer of the received data. 
		iObserver->ReceivedDataIndL(iBuffer);

#if defined (_DEBUG) && defined (_LOGGING)
		TBuf8<KIpv6MaxAddrSize> ip;
		TUint16	remotePort;
		TUint16 localPort;
		iController.ConnectionInfo(ip, remotePort, localPort);
	
		__FLOG_4(_T8("Received %d bytes on local port %d from %S, remote port %d"), iBuffer.Length(), localPort, &ip, remotePort);
		__FLOG_0(_T8("----------"));
		__FLOG_HEXDUMP(iBuffer.Ptr(), iBuffer.Length());
		__FLOG_0(_T8("----------"));
#endif
		} break;
	case EClosing:
		{
		__OOM_LEAVE_TEST

		// The socket has shutdown - move to the Closed state
		iState = EClosed;
        
		if(iReceiveTimer)
			{
			iReceiveTimer->Cancel();
			}

		// Inform the observer that the input stream is closed.
		iObserver->InputStreamCloseInd(KErrNone);

		// Inform the socket controller that the input stream is now shut
		iController.StreamClosed(KErrNone, MSocketController::EInputStream);
		} break;
	case EPendingAck:
	case EClosed:
	case EIdle:
	default:
		THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadInputStreamError);
		break;
		}
	}
	
void CSocketReader::DoCancel()
/**	
	The asynchronous request cancel function. If the input stream has requested a 
	read from the socket (ie it is in the ReceivedData state) it cancels the read
	request.
	@internalComponent
*/
	{
	if( iState == EReceivedData )
		{
		// There is a pending read request - cancel it
		iSocket.CancelRecv();
		}
	}
	
TInt CSocketReader::RunError(TInt aError)
/**	
	The asynchronous request error handler. If this has been called then the 
	read request or the socket shutdown request has failed. The socket can no 
	longer be used. The input stream observer is notified that the stream is 
	closed. No more data can be received from the socket.
	@return		A value of KErrNone.indicating that the the error has been 
				handled.
	@pre		The input stream has requested a service (a read or a shutdown)
				from socket. The request has failed. It is either in the Read or
				Closing state.
	@post		The input stream is in the Closed state.
	@internalComponent
*/
	{
	__ASSERT_DEBUG( iObserver != NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EInputStreamNotBound) );

#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KIpv6MaxAddrSize> ip;
	TUint16	remotePort;
	TUint16 localPort;
	iController.ConnectionInfo(ip, remotePort, localPort);

	__FLOG_1(_T8("!! Input stream error : %d"), aError);
	__FLOG_3(_T8("-> Connection on local port %d with %S, remote port %d closed"), localPort, &ip, remotePort);
#endif

	// Move to the Closed state
	iState = EClosed;

	// The socket request has failed - the socket connection is broken. Need
	// to inform the input stream observer that the input stream is closed.
	iObserver->InputStreamCloseInd(aError);

	// Inform the socket controller that the input stream is closed
	iController.StreamClosed(aError, MSocketController::EInputStream);

	return KErrNone;
	}

void CSocketReader::SecureServerReq()
/**
	Upgrades the socket reader to a secure socket. This is currently not supported and will result
	in the stream being closed and KErrNotSupported sent to the observer.
	@internalComponent
*/
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNotSupported);
	SetActive();
	}

/**
	Resets the state to EClosed
	@internalComponent
*/
void CSocketReader::Reset()
	{
	// Inform the connection manager that we are closing the output stream 
	// due to no memory
	iObserver->InputStreamCloseInd ( KErrNoMemory );
	iState = EClosed;	
	}

TInt CSocketReader::ImmediateRead ( TPtrC8& aData )
 	{
	TInt bytesToRead = iSocket.PendingBytesToRead ();
    __FLOG_1 (_T8("Bytes to read : %d"), bytesToRead);
	if ( bytesToRead < KErrNone )
		return bytesToRead;
	if ( bytesToRead > iBuffer.Size() )
		bytesToRead = iBuffer.Size(); 
	
	if ( bytesToRead > 0 )
 	     {
        // Now we know that there is some data to be read. 		
 		// Suspend the current AO to ensure that the AO does not trigger in between
 		// our operation.
		Suspend(); 		
		CHttpAsyncWaiter *waiter = CHttpAsyncWaiter::New();
		if ( waiter == NULL)
	    	{
	    	return KErrNoMemory;
	    	}

		// Issue a read
		iSocket.RecvOneOrMore ( iBuffer, waiter->iStatus );
		// Wait for the socket read completion.
		waiter->StartAndWait();
		TInt result = waiter->Result();
		if( result == KErrNone )
		   {
			aData.Set ( iBuffer );
		// The socket reader sent the data for processing and we have to wait for the 
		// acknowledgement for the process completion, so moving the state to EPendingAck.
			iState = EPendingAck;
			result = bytesToRead;
		    }
		delete waiter;
		// Resume the current AO.
		Resume();
		__FLOG_1(_T8("Immediate read completes with the result = %d"), result);
		return result;
 		}
	return KErrNone;	
 	}

void CSocketReader::Restart ()
	{
	if ( iState == EIdle )		
		{
		// Move to the Read state and self complete, only if not suspended.
		iState = ERead;
		if( !iSuspended )
			CompleteSelf();
		}
	}

void CSocketReader::TimeOut()
	{	
	// Cancel any outstanding requests
	Cancel();

	iObserver->OnReceiveTimeOut();
	}

void CSocketReader::Shutdown()
    {	
    Cancel(); // Cancel any outstanding requests
    iSocket.ShutdownImmediate();

    // The socket has shutdown - move to the Closed state
    iState = EClosed;

    if(iReceiveTimer)
        {
        iReceiveTimer->Cancel();
        }

    // Inform the observer that the input stream is closed.
    iObserver->InputStreamCloseInd(KErrCancel);

    // Inform the socket controller that the input stream is now shut
    iController.StreamClosed(KErrCancel, MSocketController::EInputStream);
    }
