// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "csocketconnector.h"

#include "csocket.h"
#include "msocketconnectobserver.h"
#include "msocketconnectorstore.h"
#include "msocketcontrollerfactory.h"
#include "mcommsinfoprovider.h"
#include "csocketcontroller.h"
#include "httptransporthandlercommon.h"
#include "thttptrlayerpanic.h"

#if defined (_DEBUG) && defined (_LOGGING)
const TInt KHostNameSize	= 126;
#endif

CSocketConnector* CSocketConnector::NewL(
										MSocketConnectorStore&		aStore,
										MSocketControllerFactory&	aSocketControllerFactory,
										MCommsInfoProvider&			aCommsInfoProvider, TBool aPriority
										)
/**	
	The factory constructor.
	@param		aStore						The socket connector store.
	@param		aSocketControllerFactory	The socket controller factory.
	@param		aCommsInfoProvider			The comms info provider.
	@return		A pointer to a fully constructed object.
*/
	{
	return new (ELeave) CSocketConnector(aStore, aSocketControllerFactory, aCommsInfoProvider,aPriority);
	}

CSocketConnector::~CSocketConnector()
/**	
	Destructor.
*/
	{
	// Cancel any outstanding requests
	Cancel();

	// Cleanup...
	delete iHost;
	delete iConnectingSocket;

//	__FLOG_CLOSE;
	}

CSocketConnector::CSocketConnector(
								  MSocketConnectorStore&	aStore,
								  MSocketControllerFactory&	aSocketControllerFactory,
								  MCommsInfoProvider&		aCommsInfoProvider,
								  TBool aPriority
								  )
: CActive(aPriority ? CActive::EPriorityHigh : CActive::EPriorityStandard), iStore(aStore),
									   iSocketControllerFactory(aSocketControllerFactory),
									   iCommsInfoProvider(aCommsInfoProvider)
/**	
	Constructor.
	@param		aStore						The socket connector store.
	@param		aSocketControllerFactory	The socket controller factory.
	@param		aCommsInfoProvider			The comms info provider.
*/	{
	if(aPriority)
		{
		CActive::SetPriority(CActive::EPriorityHigh);	
		}
	
	CActiveScheduler::Add(this);

//	__FLOG_OPEN("http", "httptransporthandler.txt");
	}

void CSocketConnector::ConnectL(MSocketConnectObserver& aObserver, const TDesC8& aRemoteHost, TUint16 aRemotePort, TInetAddr* aRemoteAddress)
/**	
	Start connection to specified remote host. The socket connector starts 
	connecting to the specified remote host on the specified port.
	@param		aRemoteHost		The host name/IP address of the remote host
	@param		aRemotePort		The port number of the remote host
	@pre		The socket connector is in the Idle state.
	@post		The socket connector is not in the Idle state.
*/
	{
	__ASSERT_DEBUG( iState == EIdle, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadSocketConnectorState) );

	// Set the observer
	iObserver = &aObserver;

	// Copy the remote host IP address and port
	iHost = HBufC::NewL(aRemoteHost.Length());
	iHost->Des().Copy(aRemoteHost);
	iPort = aRemotePort;
		
	TInt error = KErrNone;
    // Move to the PendingDNSLookup state and self complete.
    if(aRemoteAddress == NULL)
        {
        RDebug::Printf("RemoteAddress is NULL so doing a DNS lookup");
        iState = EPendingDNSLookup;
        // Address is unknown / DNS lookup is needed
        error = DoPendingDNSLookup();
        }
    else
        {
        RDebug::Printf("Remote address is known so doing a direct connect");
        iState = EConnecting;
        // Address is know. No lookup is needed. Just go and connect.
        iHostDnsEntry().iAddr = *aRemoteAddress;
        error = DoConnect();
        }
    
    if(error != KErrNone)
        {
        iState = EPendingDNSLookup;
        // Error the AO and handle the error in the normal path.
        TRequestStatus* pStat = &iStatus;
        User::RequestComplete(pStat, error);
        SetActive();      
        }
	}

void CSocketConnector::CompleteSelf()
/**	
	Requests that the socket connector complete itself. This will caused the 
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

void CSocketConnector::Suicide()
/**	
	The socket connector is finished, it needs to remove itself from the store
	and self-destruct.
*/
	{
	// Inform store that connection is complete and then suicide.
	iStore.ConnectionCompleted(*this);
	delete this;
	}

/*
 *	Methods from MSocketConnector
 */

void CSocketConnector::StopConnect()
/**
	@see		MSocketConnector
*/
	{
	// Cancel any outstanding requests.
	Cancel();

#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KHostNameSize> host;
	host.Copy((*iHost).Left(KHostNameSize)); //just get the KHostNameSize characters

	__FLOG_0(_T8("!! Stopping connection"));
	__FLOG_1(_T8("-> connect to host %S stopped"), &host);
#endif

	Suicide();
	}

void CSocketConnector::MSocketConnector_Reserved()
/**	
	@see		MSocketConnector
*/
	{
	User::Invariant();
	}

/*
 *	Methods from CActive
 */

void CSocketConnector::RunL()
/**	
	The request servicing function. Behaviour depends on the state of the socket
	connector.
*/
	{
	// Leave if there has been an error
	User::LeaveIfError(iStatus.Int());

	switch( iState )
		{
	case EPendingDNSLookup:
		{
		User::LeaveIfError(DoPendingDNSLookup());
		} break;
	case EConnecting:
		{
		User::LeaveIfError(DoConnect());
		} break;
	case EConnected:
		{
		__OOM_LEAVE_TEST

#if defined (_DEBUG) && defined (_LOGGING)
		TBuf8<KHostNameSize> host;
		host.Copy((*iHost).Left(KHostNameSize)); //just get the KHostNameSize characters

		TInetAddr addr;
		iConnectingSocket->RemoteName(addr);

		TBuf<KIpv6MaxAddrSize> ip16bit;
		addr.Output(ip16bit);

		TBuf8<KIpv6MaxAddrSize> ip;
		ip.Copy(ip16bit);

		TInetAddr local;
		iConnectingSocket->LocalName(local);

		__FLOG_1(_T8("!! Connection with host %S established"), &host);
		__FLOG_3(_T8("-> on local port %d with %S, remote port %d"), local.Port(), &ip, addr.Port());
#endif

		// A connection has been made with the remote client - lose ownership of
		// the connecting socket.
		CSocket* connectedSocket = iConnectingSocket;
		iConnectingSocket = NULL;

		// Create a socket controller object to own the connected socket.
		CSocketController* socketController = iSocketControllerFactory.CreateSocketControllerLC(connectedSocket, *iHost, iPort, iAddress);

		// Inform the socket connect observer that a TCP connection is established - 
		// pass back the input and output stream objects.
		iObserver->ConnectionMadeL(socketController->InputStream(), socketController->OutputStream());

		// Remove socket controller from cleanup stack - transferring ownership
		// to the store.
		CleanupStack::Pop(socketController);

		// Add the socket controller in the store - ownership is transferred to
		// the store.
		iSocketControllerFactory.AddToStoreL(socketController);

		// Socket connector is finished - suicide.
		Suicide();
		} break;
	case EIdle:
	default:
		THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadSocketConnectorState);
		break;
		}
	}

void CSocketConnector::DoCancel()
/**	
	The asynchronous request cancel.
*/
	{
	// Check state
	switch( iState )
		{
	case EConnecting:
		{
		// DNS lookup is pending - cancel
		iHostResolver.Cancel();
		iCommsInfoProvider.AddToHostResolverCache(iHostResolver); // Add to the cache.
		} break;
	case EConnected:
		{
		if( iConnectingSocket )
			{
			// Connection is pending - cancel and delete the socket
			iConnectingSocket->CancelConnect();
			delete iConnectingSocket;
			iConnectingSocket = NULL;
			}
		} break;
	case EIdle:
	case EPendingDNSLookup:
	default:
		// Do nothing...
		break;
		}
	}

TInt CSocketConnector::RunError(TInt aError)
/**	
	The error handler for when RunL() leaves. This function does any necessary
	cleanup. The socket connector is then set to suicide.
	@param		aError	The leave code.
	@return		A value of KErrNone.if the error has been handled, any other 
				value if it has not been handled.
	@post		The socket connector is in the Suicide state.
*/
	{
#if defined (_DEBUG) && defined (_LOGGING)
	TBuf8<KHostNameSize> host;
	host.Copy((*iHost).Left(KHostNameSize)); //just get the KHostNameSize characters

	__FLOG_1(_T8("!! Error : %d"), aError);
	__FLOG_1(_T8("-> connect to host %S failed"), &host);
#endif

	TInt errorToPropagate = aError;
	TInt error = KErrNone;

	switch( iState )
		{
	case EPendingDNSLookup:
	case EConnecting:
		{
		// In EReadyForDNS or EReadyToConnect states, KErrNotReady may be 
		// returned by the comms call that require a connection. The RConnection
		// that is passed in MUST be started or this error will occur. If we own
		// the RConnection we should start the RConnection again, else if our
		// client own the RConnection, we just propagate the Comms error.
		if( iCommsInfoProvider.HasConnection() && iCommsInfoProvider.OwnsConnection() && aError == KErrNotReady )
			{
			__FLOG_0(_T8("-> re-starting comms interface..."));

			// We own the RConnection and the error is KErrNotReady
			errorToPropagate = iCommsInfoProvider.Connection().Start();

			if( errorToPropagate == KErrNone )
				{
				__FLOG_0(_T8("!! Re-started comms interface"));
				__FLOG_1(_T8("-> try connecting to host %S again"), &host);

				// RConnection started successfully, try to connect again
				CompleteSelf();
				return KErrNone;
				}
#if defined (_DEBUG) && defined (_LOGGING)
			else
				{
				__FLOG_1(_T8("!! Error : %d"), errorToPropagate);
				__FLOG_0(_T8("-> failed to re-start comms interface"));
				__FLOG_1(_T8("-> connect to host %S failed"), &host);
				}			
#endif
			}
		// Opening the connecting socket has failed or the DNS lookup completed
		// with an error code - suicide after notifying the observer - do nothing 
		// except drop through to the Connected state case...
		}
	case EConnected:
		{
		// Either the socket controller factory left in AddToStoreL() or in
		// CreateSocketControllerLC(), or the observer left in ConnectionMadeL()
		// or the connect request completed with an error code. Inform the 
		// observer of the error.
		error = iObserver->HandleConnectError(errorToPropagate);
		} break;
	case EIdle:
	default:
		THttpTrLayerPanic::Panic(THttpTrLayerPanic::EBadSocketConnectorState);
		break;
		}

	Suicide();

	return error;
	}

TInt CSocketConnector::DoPendingDNSLookup()
    {
#if defined (_DEBUG) && defined (_LOGGING)
        TBuf8<KHostNameSize> host;
        host.Copy((*iHost).Left(KHostNameSize)); //just get the KHostNameSize characters

        __FLOG_1(_T8("Doing DNS lookup -> searching for host %S"), &host);
#endif        
     TInt error = KErrNone;
     
     iCommsInfoProvider.HostResolverFromCache(iHostResolver); // Get the RHostResolver from the cache
     if(iHostResolver.SubSessionHandle() <= 0)
         {
         RDebug::Printf("No host resolver. Open a new one...");
         if ( iCommsInfoProvider.HasConnection() )
            {
            // Open the host resolver session with the preffered connection
            error = iHostResolver.Open(iCommsInfoProvider.SocketServer(),
                                       iCommsInfoProvider.ProtocolDescription().iAddrFamily, 
                                       KProtocolInetUdp,
                                       iCommsInfoProvider.Connection());                
             }
          else
            {                           
            // Open the host resolver session with no connection
            error = iHostResolver.Open(iCommsInfoProvider.SocketServer(),
                                       iCommsInfoProvider.ProtocolDescription().iAddrFamily, 
                                       KProtocolInetUdp);                
            }
         }
     
     if(error != KErrNone)
        {
        return error;
        }
        
    // Start the DNS lookup for the remote host name.
    iHostResolver.GetByName(*iHost, iHostDnsEntry, iStatus);

    // Move to the Connecting state and go active
    iState = EConnecting;
    SetActive();    
    return error;
    }

TInt CSocketConnector::DoConnect()
    {
    // DNS lookup successful - form the internet address object
    iAddress = TInetAddr(iHostDnsEntry().iAddr);
    iAddress.SetPort(iPort);
    
    // Add the RHostResolver to the cache.
    if(iHostResolver.SubSessionHandle() > 0)
        {
        iCommsInfoProvider.AddToHostResolverCache(iHostResolver);
        }
    
#if defined (_DEBUG) && defined (_LOGGING)
    TBuf8<KHostNameSize> host;
    host.Copy((*iHost).Left(KHostNameSize)); //just get the KHostNameSize characters

    TBuf<KIpv6MaxAddrSize> ip16bit;
    iAddress.Output(ip16bit);

    TBuf8<KIpv6MaxAddrSize> ip;
    ip.Copy(ip16bit);
    
    __FLOG_2(_T8("DNS lookup complete -> host %S has IP address %S"), &host, &ip);
#endif
   
    // Start a default RConnection, if one is not started and not local loopback address        
    if ( !iCommsInfoProvider.HasConnection() && !iAddress.IsLoopback() )
        {
        // it is ok to TRAP here as the method will be called only once.
        TRAPD(error, iCommsInfoProvider.StartDefaultCommsConnectionL ());
        if(error != KErrNone)
            {
            return error;
            }
        }

    // Create the connecting socket
    iConnectingSocket = CSocket::New(iCommsInfoProvider, CSocket::EProtocolSocket);
    if(!iConnectingSocket)
        {
        return KErrNoMemory;
        }
    
    // Start connecting to the remote client
    iConnectingSocket->Connect(iAddress, iStatus);
    SetActive();
    __FLOG_2(_T8("Connecting -> to host %S on IP address %S"), &host, &ip);

    // Move to the Connected state and go active
    iState = EConnected;
    return KErrNone;
    }


