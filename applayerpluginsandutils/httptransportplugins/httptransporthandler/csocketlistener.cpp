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

#include "csocketlistener.h"

#include "csocket.h"
#include "msocketcontrollerfactory.h"
#include "mcommsinfoprovider.h"
#include "msocketlistenobserver.h"
#include "csocketcontroller.h"
#include "httptransporthandlercommon.h"
#include "thttptrlayerpanic.h"

const TInt KListenQSize	= 5;

CSocketListener* CSocketListener::NewL(
  									  MSocketControllerFactory&	aSocketControllerFactory,
									  MCommsInfoProvider&		aCommsInfoProvider, TBool aPriority
									  )
/**	
	The factory constructor.
	@param		aSocketControllerFactory	The socket controller factory.
	@param		aCommsInfoProvider			The comms info provider.
	@return		A pointer to a fully constructed object.
*/
	{
	return new (ELeave) CSocketListener(aSocketControllerFactory, aCommsInfoProvider, aPriority);
	}

CSocketListener::~CSocketListener()
/**	
	Destructor.
*/
	{
	// Cancel any outstanding requests
	Cancel();

	// Cleanup...
	delete iAcceptingSocket;
	delete iListeningSocket;

//	__FLOG_CLOSE;
	}

CSocketListener::CSocketListener(
								MSocketControllerFactory&	aSocketControllerFactory,
								MCommsInfoProvider&			aCommsInfoProvider,
								TBool aPriority
								)
: CActive(CActive::EPriorityStandard), iSocketControllerFactory(aSocketControllerFactory),
									   iCommsInfoProvider(aCommsInfoProvider)
/**	
	Constructor.
	@param		aSocketControllerFactory	The socket controller factory.
	@param		aCommsInfoProvider			The comms info provider.
*/
	{
	if(aPriority)
		{
		CActive::SetPriority(CActive::EPriorityHigh);	
		}
	CActiveScheduler::Add(this);

//	__FLOG_OPEN("http", "httptransporthandler.txt");
	}

void CSocketListener::Listen(MSocketListenObserver& aObserver, TUint16 aPort)
/**	
	The socket listener starts listening on the specified port.
	@param		aPort	The specified listening port.
	@pre		The socket listener is in the Idle state.
	@post		The socket listener is not in the Idle state.
*/
	{
	__ASSERT_DEBUG( iState == EIdle, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadSocketListenerState) );

	iObserver = &aObserver;
	iPort = aPort;

	// Move to the StartListen state and self complete
	iState = EStartListen;
	CompleteSelf();
	}

void CSocketListener::StopListen()
/**	
	The socket listener stops listening. If the socket listener is in the Idle state this
	function will have no effect.
	@pre		The socket listener is not in the Idle state.
	@post		The socket listener is in the Idle state.
*/
	{
	if( iState != EIdle )
		{
		// Cancel any outstanding requests
		Cancel();

		// Call RunL() with an iStatus of KErrCancel - this will notify the observer
		// that the listening service has stopped.
		TRequestStatus* pStat = &iStatus;
		User::RequestComplete(pStat, KErrCancel);
		SetActive();

		__FLOG_0(_T8("!! Listen service stopped"));
		__FLOG_2(_T8("-> cancel listen on local port %d : expect %d error code"), iPort, iStatus.Int());
		}
	}

void CSocketListener::CompleteSelf()
/**	
	Requests that the socket listener complete itself. This will cause the 
	RunL() to be called by the scheduler at the next opportunity.
	@pre		The socket listener object is not active.
	@post		The socket listener object is active and the request has been
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

void CSocketListener::RunL()
/**	
	The request servicing function. Behaviour depends on the state of the socket
	listener.
*/
	{
	// Leave if the listening socket reported an error
	User::LeaveIfError(iStatus.Int());

	switch( iState )
		{
	case EStartListen:
		{
		__ASSERT_DEBUG( iListeningSocket == NULL, User::Invariant() );

		__FLOG_1(_T8("Start listen service -> on local port %d"), iPort);

		// Open the listening socket on specified port
		iListeningSocket = CSocket::NewL(iCommsInfoProvider, CSocket::EProtocolSocket);

		// Start the listening service
		User::LeaveIfError(iListeningSocket->Listen(KListenQSize, iPort));

		// Move to the Listening state and self complete
		iState = EListening;
		CompleteSelf();
		} break;
	case EListening:
		{
		// Create a new accepting socket - marry it to the listening socket
		iAcceptingSocket = CSocket::NewL(iCommsInfoProvider, CSocket::EBlankSocket);
		iListeningSocket->Accept(*iAcceptingSocket, iStatus);

		// Move to the Connected state and go active
		iState = EConnected;
		SetActive();
		} break;
	case EConnected:
		{
		__OOM_LEAVE_TEST

#if defined (_DEBUG) && defined (_LOGGING)
		TInetAddr addr;
		iAcceptingSocket->RemoteName(addr);

		TBuf<KIpv6MaxAddrSize> ip16bit;
		addr.Output(ip16bit);

		TBuf8<KIpv6MaxAddrSize> ip;
		ip.Copy(ip16bit);

		__FLOG_1(_T8("!! Listen service received connection on local port %d"), iPort);
		__FLOG_2(_T8("-> connection with %S, remote port %d"), &ip, addr.Port());
#endif

		// A remote client has connected to the listening socket - lose ownership
		// of the accepting socket.
		CSocket* connectedSocket = iAcceptingSocket;
		iAcceptingSocket = NULL;

		// Create a socket controller object to own the connected socket.
		CSocketController* socketController = iSocketControllerFactory.CreateSocketControllerLC(connectedSocket);

		// Inform the socket listen observer that a TCP connection is established - 
		// pass back the input and output stream objects.
		iObserver->ConnectionReceivedL(socketController->InputStream(), socketController->OutputStream());

		// Remove socket controller from cleanup stack - transferring ownership
		// to the store.
		CleanupStack::Pop(socketController);

		// Add the socket controller in the store - ownership is transferred to
		// the store.
		iSocketControllerFactory.AddToStoreL(socketController);
		
		__FLOG_1(_T8("Continue listen service -> on local port %d"), iPort);

		// Move to the Listening state and self complete
		iState = EListening;
		CompleteSelf();
		} break;
	case EIdle:
	default:
		THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadSocketListenerState);
		break;
		}
	}

void CSocketListener::DoCancel()
/**	
	The asynchronous request cancel.
*/
	{
	// Check state...
	if( iState == EConnected )
		{
		// Need to cancel the accept request
		iListeningSocket->CancelAccept();
		}
	}

TInt CSocketListener::RunError(TInt aError)
/**	
	The request servicing error handler. This function does any necessary cleanup.
	The socket listener is then placed in the Idle state.
	@param		aError	The leave code.
	@return		A value of KErrNone.if the error has been handled, any other 
				value if it has not been handled.
	@post		The socket listener is in the Idle state.
*/
	{
	__FLOG_1(_T8("!! Error : %d"), aError);
	__FLOG_1(_T8("-> listen on local port %d failed"), iPort);

	switch( iState )
		{
	case EConnected:
		{
		// Either the socket controller factory left in AddToStoreL() or in 
		// CreateSocketControllerLC(), or the observer left in 
		// ConnectionReceivedL() or the accept request completed with an error
		// code - which ever just need to delete the accepting socket.
		delete iAcceptingSocket;
		iAcceptingSocket = NULL;
		// Drop through to the Listening state case...
		}
	case EListening:
		{
		// Failed to open the accepting socket - do nothing except drop through
		// to the PendingListening state case...
		}
	case EStartListen:
		{
		// In EReadyForDNS or EReadyToConnect states, KErrNotReady may be 
		// returned by the comms call that require a connection. The RConnection
		// that is passed in MUST be started or this error will occur. If we own
		// the RConnection we should start the RConnection again, else if our 
		// client own the RConnection, we just propagate the Comms error.
		if( iCommsInfoProvider.HasConnection() && iCommsInfoProvider.OwnsConnection() && aError == KErrNotReady && iState != EConnected )
			{
			__FLOG_0(_T8("-> re-starting comms interface..."));

			// We own the RConnection and the error is KErrNotReady
			aError = iCommsInfoProvider.Connection().Start();
			if( aError==KErrNone )
				{
				__FLOG_0(_T8("!! Re-started comms interface"));
				__FLOG_1(_T8("-> start listen on local port %d again"), iPort);

				// RConnection started successfully, try to connect again
				CompleteSelf();
				return KErrNone;
				}
#if defined (_DEBUG) && defined (_LOGGING)
			else
				{
				__FLOG_1(_T8("!! Error : %d"), aError);
				__FLOG_0(_T8("-> failed to re-start comms interface"));
				__FLOG_1(_T8("-> listen on port %d failed"), iPort);
				}			
#endif
			}

		// Failed to either start the listening service or create the listening 
		// socket - delete the listening socket.
		delete iListeningSocket;
		iListeningSocket = NULL;

		// Move back to the Idle state.
		iState = EIdle;
		} break;
	case EIdle:
	default:
		THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadSocketListenerState);
		break;
		}

	// Inform the manager of the error.
	return iObserver->HandleListenError(aError);
	}
