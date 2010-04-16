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

#include "csocketwriter.h"

#include "csocket.h"
#include "msocketcontroller.h"
#include "moutputstreamobserver.h"
#include "httptransporthandlercommon.h"
#include "thttptrlayerpanic.h"

CSocketWriter* CSocketWriter::NewL(CSocket& aSocket, MSocketController& aController, TBool aPriority)
/**	
	The factory constructor.
	@param		aSocket		The connected socket. This owned by the observer.
	@param		aController	The socket controller that owns the socket.
	@return		A pointer to a fully constructed object.
*/
	{
	return new (ELeave) CSocketWriter(aSocket, aController, aPriority);
	}

CSocketWriter::~CSocketWriter()
/**	
	Destructor.
*/
	{
	__ASSERT_DEBUG( iState == EIdle || iState == EClosed, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadOutputStreamState) );
    
	//Delete the Timer object.
	delete iSendTimer;

	// Cancel any outstanding requests
	Cancel();

//	__FLOG_CLOSE;
	}

CSocketWriter::CSocketWriter(CSocket& aSocket, MSocketController& aController, TBool aPriority)
: CActive(CActive::EPriorityStandard), iSocket(aSocket), iController(aController)
/**	
	Constructor.
	@param		aSocket		The connected socket. This owned by the observer.
	@param		aController	The socket controller that owns the socket.
*/
	{
	if(aPriority) 
		{
		CActive::SetPriority(CActive::EPriorityHigh);	
		}
	CActiveScheduler::Add(this);

//	__FLOG_OPEN("http", "httptransporthandler.txt");
	}

void CSocketWriter::SocketClosed(TInt aError)
/**	
	Notifies the output stream that the socket is closed. The output stream 
	observer is notified that the stream is closed. No more data can be sent to
	the socket.
	@param		aError	The error code explaining why the socket has closed. A
						value of KErrNone indicates that the input stream 
						observer requested that the socket be closed.
	@pre		None.
	@post		The output stream is in the Closed state.
*/
	{
	// Cancel any outstanding requests
	Cancel();

	// The socket has shutdown. Inform the output stream observer that the 
	// output stream is closed.
	if( iObserver )
		iObserver->OutputStreamCloseInd(aError);

	// Move to the Closed state
	iState = EClosed;
	}

/*
 *	Methods from MOutputStream
 */

void CSocketWriter::Bind(MOutputStreamObserver& aObserver)
/**	
	@see		MOutputStream
*/
	{
	__ASSERT_DEBUG( iState == EIdle || iState == EPendingSend, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadOutputStreamState) );

	// Bind to the output stream observer
	iObserver = &aObserver;

	// Move to the PendingSend state
	iState = EPendingSend;
	}

void CSocketWriter::SendDataReqL(const TDesC8& aBuffer)
/**	
	@see		MOutputStream
*/
	{
	__ASSERT_DEBUG( iObserver != NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EOutputStreamNotBound) );
	__ASSERT_DEBUG( iState == EPendingSend, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadOutputStreamState) );
    
	//Get the TimeOut Value.
	TInt timeoutValue = iObserver->SendTimeOutVal();
	if(timeoutValue > 0)
		{
		
		//Convert to Microseconds.
		if(timeoutValue <= KMinTimeoutValue)
			{
			//Default Value is 60 Seconds
			timeoutValue = KDefTimeoutValue;
			}
		else
			{
			timeoutValue = timeoutValue * KMicrovalue;
			}
		
		//Create and Start the HTTP timer
		if(!iSendTimer)
			{
			iSendTimer = CHttpTimer::NewL(*this);
			}
		iSendTimer->After(timeoutValue);
		}

	// The output stream observer requests that the supplied buffer is sent to
	// the socket. Request a write to the socket
	iSocket.Send(aBuffer, iStatus);

#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KIpv6MaxAddrSize> ip;
	TUint16	remotePort;
	TUint16 localPort;
	iController.ConnectionInfo(ip, remotePort, localPort);

	__FLOG_4(_T8("Sending %d bytes on local port %d to %S, remote port %d"), aBuffer.Length(), localPort, &ip, remotePort);
	__FLOG_0(_T8("----------"));
	__FLOG_HEXDUMP(aBuffer.Ptr(), aBuffer.Length());
	__FLOG_0(_T8("----------"));
#endif

	// Move to the SentData and go active
	iState = ESentData;
	SetActive();
	}

void CSocketWriter::ShutdownReq()
/**	
	@see		MOutputStream
*/
	{
	__ASSERT_DEBUG( iObserver != NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EOutputStreamNotBound) );
	__ASSERT_DEBUG( iState != EClosing || iState != EClosed, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadOutputStreamState) );

	// Cancel any outstanding requests
	Cancel();
	
#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KIpv6MaxAddrSize> ip;
	TUint16	remotePort;
	TUint16 localPort;
	iController.ConnectionInfo(ip, remotePort, localPort);

	__FLOG_0(_T8("!! Shutting down output stream"));
	__FLOG_3(_T8("-> on local port %d with %S, remote port %d"), localPort, &ip, remotePort);
#endif
	
	// Shutdown the socket
	iSocket.Shutdown(iStatus);

	// Move to the Closing state and go active
	iState = EClosing;
	SetActive();
	}

void CSocketWriter::SecureClientReq(const TDesC8& aHostName)
/**
	The request to upgrade the client to a secure connection. This method begins
	the secure handshake required to upgrade to a secure connection.
	@param		aHostName	The domain name of the server for checking against 
							the server certificates.
	@pre		The socket writer must be in the EPendingSend state.
*/
	{
	__ASSERT_DEBUG( iState == EPendingSend, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadOutputStreamState) );

#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KIpv6MaxAddrSize> ip;
	TUint16	remotePort;
	TUint16 localPort;
	iController.ConnectionInfo(ip, remotePort, localPort);

	__FLOG_0(_T8("!! Upgrading to secure (client) connection"));
	__FLOG_3(_T8("-> on local port %d with %S, remote port %d"), localPort, &ip, remotePort);
#endif

	// Notify the controller to suspend the input stream.
	iController.StreamSuspend(MSocketController::EOutputStream);
	
	// Store the host name as required for secure certificate domain name 
	// chacking. Move to the StartSecureHandshake state and self complete.
	iHostName.Set(aHostName);
	iState = EStartSecureHandshake;
	CompleteSelf();
	}

void CSocketWriter::Close()
/**	
	@see		MOutputStream
*/
	{
	__ASSERT_DEBUG( iObserver != NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EOutputStreamNotBound) );
	__ASSERT_DEBUG( iState != EClosing || iState != EClosed, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadOutputStreamState) );

	// Cancel any outstanding requests
	Cancel();

#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KIpv6MaxAddrSize> ip;
	TUint16	remotePort;
	TUint16 localPort;
	iController.ConnectionInfo(ip, remotePort, localPort);

	__FLOG_0(_T8("!! Closing output stream"));
	__FLOG_3(_T8("-> on local port %d with %S, remote port %d"), localPort, &ip, remotePort);
#endif

	// There is no need to do anything here - by informing the socket controller
	// that the output stream is closed it will notify the input stream and then
	// close the socket.

	// Move to the Closed state
	iState = EClosed;

	// Inform the socket controller that the output stream is closed
	iController.StreamClosed(KErrCancel, MSocketController::EOutputStream);
	}


const CX509Certificate* CSocketWriter::ServerCert()
/**	
	@see		MOutputStream
*/
	{
	return iSocket.ServerCert();
	}
	
TInt CSocketWriter::CipherSuite(TDes8& aCipherSuite)
/**	
	@see		MOutputStream
*/
	{
	return iSocket.CipherSuite(aCipherSuite);
	}

void CSocketWriter::MOutputStream_Reserved()
/**	
	@see		MOutputStream
*/
	{
	User::Invariant();
	}
	
/**
	Resets the state to EClosed
	@componentInternal
*/
void CSocketWriter::Reset()
	{
	// Inform the connection manager that we are closing the output stream 
	// due to no memory
	iObserver->OutputStreamCloseInd ( KErrNoMemory );
	iState = EClosed;	
	}

void CSocketWriter::CompleteSelf()
/**	
	Requests that the socket writer complete itself. This will caused the 
	RunL() to be called by the scheduler at the next opportunity.
	@pre		The socket connector object is not active.
	@post		The socket connector object is active and the request has been
				completed.
*/
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

/*
 * Methods from CActive
 */

void CSocketWriter::RunL()
/**	
	The request servicing function. Behaviour depends on the state of the output
	stream.
*/
	{
	__ASSERT_DEBUG( iObserver != NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EOutputStreamNotBound) );

	// Leave if the socket reported an error
	User::LeaveIfError(iStatus.Int());
    
	if(iSendTimer)
		{
		iSendTimer->Cancel();
		}

	switch( iState )
		{
	case ESentData:
		{
		__OOM_LEAVE_TEST

		// The data has successfully been written to the socket - move to the 
		// PendingSend state. 
		iState = EPendingSend;

		// Inform the observer that the data has been sent.
		iObserver->SendDataCnfL();
		} break;
	case EClosing:
		{
		__OOM_LEAVE_TEST

		// The socket has shutdown - move to the Closed state
		iState = EClosed;

		// Inform the observer that the output stream is closed.
		iObserver->OutputStreamCloseInd(KErrNone);

		// Inform the socket controller that the output stream is closed
		iController.StreamClosed(KErrNone, MSocketController::EOutputStream);
		} break;
	case EStartSecureHandshake:
		{
		// Start the secure handshake
		iState = ESecureHandshakeComplete;
		_LIT(KTxtTls, "tls1.0");
 		_LIT(KTxtSsl,"ssl3.0");
 		if (!iObserver->SecureRetry())
 		    {
             iSocket.UpgradeToSecureL(iStatus, iHostName,KTxtTls()); //first negotiate with latest version of TLS
 		    }
 		else
 		    {
             iSocket.UpgradeToSecureL(iStatus, iHostName,KTxtSsl()); //now try old ssl version
 		    }
		SetActive();
		} break;
	case ESecureHandshakeComplete:
		{
		__OOM_LEAVE_TEST

		// Secure handshake has completed successfully so inform the observer.
		iState = EPendingSend;
		iObserver->SecureClientCnf();

#if defined (_DEBUG) && defined (_LOGGING)
		TBuf8<KIpv6MaxAddrSize> ip;
		TUint16	remotePort;
		TUint16 localPort;
		iController.ConnectionInfo(ip, remotePort, localPort);

		__FLOG_0(_T8("!! Upgrade to secure (client) connection successful"));
		__FLOG_3(_T8("-> on local port %d with %S, remote port %d"), localPort, &ip, remotePort);
#endif

		// Notify the controller to resume the input stream.
		iController.StreamResume(MSocketController::EOutputStream);
		} break;
	case EPendingSend:
	case EClosed:
	case EIdle:
	default:
		THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadOutputStreamState);
		break;
		}
	}

void CSocketWriter::DoCancel()
/**	
	The asynchronous request cancel function. If the output stream has requested
	a write to the socket (ie it is in the SentData state) this function cancels
	the write request.
*/
	{
	if( iState == ESentData )
		{
		// There is a pending write request - cancel it
		iSocket.CancelSend();
		}
	else if( iState == ESecureHandshakeComplete )
		{
		// Cancel the upgrade to secure connection.
		iSocket.CancelUpgradeToSecure();
		}
	}

TInt CSocketWriter::RunError(TInt aError)
/**	
	The error handler for when RunL() leaves. If this has been called then the 
	write request or the socket shutdown request has failed. The socket can no 
	longer be used. The output stream observer is notified that the stream is 
	closed. No more data can be sent to the socket.
	@return		A value of KErrNone indicating that the the error has been 
				handled.
	@pre		The output stream has requested a service (a write or a shutdown) 
				from socket. The request has failed. It is either in the SentData
				or Closing state.
	@post		The output stream is in the Closed state.
*/
	{
	__ASSERT_DEBUG( iObserver != NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EOutputStreamNotBound) );

#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KIpv6MaxAddrSize> ip;
	TUint16	remotePort;
	TUint16 localPort;
	iController.ConnectionInfo(ip, remotePort, localPort);

	__FLOG_1(_T8("!! Output stream error : %d"), aError);
	__FLOG_3(_T8("-> Connection on local port %d with %S, remote port %d closed"), localPort, &ip, remotePort);
#endif

	// Move to the Closed state
	iState = EClosed;

	// The socket request has failed - the socket connection is broken. Need
	// to inform the output stream observer that the output stream is closed.
	iObserver->OutputStreamCloseInd(aError);

	// Inform the socket controller that the output stream is closed
	iController.StreamClosed(aError, MSocketController::EOutputStream);

	return KErrNone;
	}

void CSocketWriter::TimeOut()
	{
	// Cancel any outstanding requests
	Cancel();
	iObserver->OnSendTimeOut();
	}

void CSocketWriter::SetTCPCorking(TBool aValue)
    {
    iSocket.SetTCPCorking(aValue);
    }


