// Copyright (c) 2001-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ctcptransportlayer.h"
#include "csocketlistener.h"
#include "csocketconnector.h"
#include "csocketcontroller.h"
#include "thttptrlayerpanic.h"
#include "mconnectionprefsprovider.h"

#include "csocketreader.h"
#include "csocketwriter.h"

_LIT(KTcpProtName, "tcp");

const TInt KMaxHostResolverCacheCount = 8; // 8 Should be sufficient here as we can have maximum of 8 connections
                                           // at anytime to the host. And it is not neccassarly mean that 8 host resolvers
                                           // will be operating simulataneously. 
CTcpTransportLayer* CTcpTransportLayer::NewL(TAny* aTransportConstructionParams)
/**	
	The factory constructor.
	@param		aTransportConstructionParams	A pointer to the instantiaton 
												parameters
	@return		A pointer to a fully constructed object.
*/
	{
	THttpTransportConstructionParams* params = reinterpret_cast<THttpTransportConstructionParams*>(aTransportConstructionParams);
	CTcpTransportLayer* self = new (ELeave) CTcpTransportLayer(params->iConnectionPrefsProvider);
	CleanupStack::PushL(self);
	self->ConstructL(params);
	CleanupStack::Pop(self);
	return self;
	}

CTcpTransportLayer::~CTcpTransportLayer()
/**	
	Destructor.
*/
	{
	// Delete the socket listener
	delete iSocketListener;

	// Delete the socket connectors
	iConnectorStore.ResetAndDestroy();

	// Delete the socket controllers
	iControllerStore.ResetAndDestroy();
	
	
	// Empty and close the host resolver cache.
	EmptyHostResolverCache();
	iHostResolverCache.Close();
	
	// Close the socket server session if owned
	if( iOwnsConnection )
		{
		if( iConnection )
			{
			iConnection->Close();
			delete iConnection;
			}
		}
	if( iOwnsSocketConnection )
		{
		iSocketServer.Close();
		}

	__FLOG_CLOSE;
	}

CTcpTransportLayer::CTcpTransportLayer(MConnectionPrefsProvider& aTransLayerObserver)
: CHttpTransportLayer(), iConnectionPrefsProvider(aTransLayerObserver), iHostResolverCache(KMaxHostResolverCacheCount)
/**	
	Constructor.
*/
	{
	__FLOG_OPEN("http", "httptransporthandler.txt");
	__FLOG(_T8("HTTP Transport Handler Log"));
	}

void CTcpTransportLayer::ConstructL(THttpTransportConstructionParams* aTransportConstructionParams)
/**	
	Second phase constructor. Initialises the object.
*/
	{
	// Callback on the observer for a Comms connection, otherwise create our own
	// Get socket server handle if set, otherwise start socket server. 
	// Get RConnection handle if the connection is started else don't create our own
	// as we may or may not need one. eg; for localhost we don't need RConnection.	
	iPriority = aTransportConstructionParams->iPriority;
	TInt socketServerHandle = 0;
	if( iConnectionPrefsProvider.SupplySocketServerHandle( socketServerHandle ) )
		{
		iOwnsSocketConnection = EFalse;
		__ASSERT_ALWAYS( socketServerHandle!=0, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EExpectedConnectionNotSupplied) );

		iSocketServer.SetHandle(socketServerHandle);
		
		// Get RConnection handle if one available
		if ( iConnectionPrefsProvider.SupplyCommsConnection ( iConnection ) )
			{
			__ASSERT_ALWAYS( iConnection!=NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EExpectedConnectionNotSupplied) );			
			iOwnsConnection = EFalse;						
			}

		__FLOG_0(_T8("Transport Layer does not own SockServ session"));
		}
	else
		{
		iOwnsSocketConnection = ETrue;
		// Connect to the socket server and the handle

		User::LeaveIfError(iSocketServer.Connect());
		iConnectionPrefsProvider.SetSocketServerHandleL ( iSocketServer.Handle() );

		__FLOG_0(_T8("Transport Layer owns SockServ session"));
		}

	// Get the protocol description
	User::LeaveIfError(iSocketServer.FindProtocol(KTcpProtName(), iProtocolDescription));
	}

/*
 *	Methods from CHttpTransportLayer
 */

void CTcpTransportLayer::CHttpTransportLayer_Reserved()
/**
	@see		CHttpTransportLayer
*/
	{
	User::Invariant();
	}

/*
 *	Methods from MSocketFactory
 */

void CTcpTransportLayer::ListenL(MSocketListenObserver& aObserver, TUint16 aPort)
/**
	@see		MSocketFactory
*/
	{
	if( iSocketListener == NULL )
		{
		// Create the socket listener object
		iSocketListener = CSocketListener::NewL(SocketControllerFactory(), CommsInfoProvider(), iPriority);


#if defined (_DEBUG) && defined (_LOGGING)
		iSocketListener->__logger__ = this->__logger__;
#endif
		}
	// Start the socket listener...
	iSocketListener->Listen(aObserver, aPort);
	}

void CTcpTransportLayer::StopListen()
/**
	@see		MSocketFactory
*/
	{
	__ASSERT_DEBUG( iSocketListener != NULL, User::Invariant() );

	// Stop the socket listener...
	iSocketListener->StopListen();
	}

MSocketConnector& CTcpTransportLayer::ConnectL(MSocketConnectObserver& aObserver, const TDesC8& aRemoteHost, TUint16 aRemotePort)
/**
	@see		MSocketFactory
*/
	{
   if(iOwnsSocketConnection)
        {
        TInt socketServerHandle = 0;
        if(iConnectionPrefsProvider.SupplySocketServerHandle(socketServerHandle)) 
            {
            __FLOG_2(_T8("ConnectL: iSocketServer.Handle(): %d socketServerHandle: %d"), iSocketServer.Handle(), socketServerHandle);
            if ( iSocketServer.Handle() != socketServerHandle ) 
                {
                __FLOG_0(_T8("Socket server handle is switching. Clear the connections / socket opened with the old socket server"));                
                // Close all the sockets as we are swicthing to a different socket server
                for(TInt i = 0; i < iControllerStore.Count(); ++i)
                    {
                    iControllerStore[i]->InputStream().Shutdown();
                    // Socket and controller will be deleted by itself
                    }
                iConnectorStore.ResetAndDestroy();
                EmptyHostResolverCacheIfNeeded();
                }
            }	        
        }		

   // Create the socket connector
	CSocketConnector* socketConnector = CSocketConnector::NewL(
															  SocketConnectorStore(),
															  SocketControllerFactory(),
															  CommsInfoProvider(),iPriority
															  );
	CleanupStack::PushL(socketConnector);

#if defined (_DEBUG) && defined (_LOGGING)
	socketConnector->__logger__ = this->__logger__;
#endif

   TInetAddr* addr = NULL;
   const TUint count = iControllerStore.Count(); 
    for(TUint i = 0; i < count; ++i)
        {
        CSocketController* controller = iControllerStore[i];
        if(controller->HostAndPortMatches(aRemoteHost, aRemotePort))
            {
            addr = &controller->RemoteAddress();
            }
        }

	// Start connection to remote host
	socketConnector->ConnectL(aObserver, aRemoteHost, aRemotePort, addr);

	// Add to the connector store
	User::LeaveIfError(iConnectorStore.Append(socketConnector));

	// Ownership of the socket connector now transferred to the store.
	CleanupStack::Pop(socketConnector);

	return *socketConnector;
	}

void CTcpTransportLayer::MSocketFactory_Reserved()
/**
	@see		MSocketFactory
*/
	{
	User::Invariant();
	}

/*
 *	Methods from MSocketControllerFactory
 */

CSocketController* CTcpTransportLayer::CreateSocketControllerLC(CSocket* aConnectedSocket)
/**	
	@see		MSocketControllerFactory
*/
	{
	__ASSERT_DEBUG( aConnectedSocket != NULL, User::Invariant() );

	// A connection has been established on the given socket - create a socket
	// controller object to own the connected socket. 
	CSocketController* socketController = CSocketController::NewL(aConnectedSocket, iConnectionPrefsProvider, iPriority);
	CleanupStack::PushL(socketController);

#if defined (_DEBUG) && defined (_LOGGING)
	CSocketReader* reader = static_cast<CSocketReader*>(&socketController->InputStream());
	CSocketWriter* writer = static_cast<CSocketWriter*>(&socketController->OutputStream());

	reader->__logger__ = this->__logger__;
	writer->__logger__ = this->__logger__;
#endif

	return socketController;
	}

CSocketController* CTcpTransportLayer::CreateSocketControllerLC(CSocket* aConnectedSocket, const TDesC& aRemoteHost, TUint16 aRemotePort, const TInetAddr& aRemoteAddr)
    {
    __ASSERT_DEBUG( aConnectedSocket != NULL, User::Invariant() );
    // A connection has been established on the given socket - create a socket
    // controller object to own the connected socket. 
    CSocketController* socketController = CSocketController::NewL(aConnectedSocket, iConnectionPrefsProvider, iPriority);
    CleanupStack::PushL(socketController);
    socketController->AssignRemoteHostInfoL(aRemoteHost, aRemotePort, aRemoteAddr);
#if defined (_DEBUG) && defined (_LOGGING)
    CSocketReader* reader = static_cast<CSocketReader*>(&socketController->InputStream());
    CSocketWriter* writer = static_cast<CSocketWriter*>(&socketController->OutputStream());

    reader->__logger__ = this->__logger__;
    writer->__logger__ = this->__logger__;
#endif

    return socketController;    
    
    }

void CTcpTransportLayer::AddToStoreL(CSocketController* aSocketController)
/**	
	@see		MSocketControllerFactory
*/
	{
	__ASSERT_DEBUG( aSocketController, User::Invariant() );

	// Ownership has been transferred - place on cleanup stack.
	CleanupStack::PushL(aSocketController);

	// Append to the store
	TInt err = iControllerStore.Append(aSocketController);
	if(err!=KErrNone)
	{
		aSocketController->InputStream().Reset();	
		aSocketController->OutputStream().Reset();
		User::Leave(err);
	}

	// The store now owns the socket controller
	CleanupStack::Pop(aSocketController);

	// Notify the socket controller that it's in the store
	aSocketController->NotifyInStore(SocketControllerStore());
	}

/*
 *	Methods from MSocketConnectorStore
 */

void CTcpTransportLayer::ConnectionCompleted(CSocketConnector& aOrphanedSocketConnector)
/**	
	@see		MSocketConnectorStore
*/
	{
	// Find the appropriate index for this connector
	TInt index = iConnectorStore.Count();
	TBool found = EFalse;
	while( !found && index > 0 )
		{
		found = &aOrphanedSocketConnector == iConnectorStore[--index];
		}
	__ASSERT_DEBUG( found, User::Invariant() );
	
	// Remove the socket connector from the store and compress the store.
	iConnectorStore.Remove(index);
	iConnectorStore.Compress();
	
	// Empty the host resolver cache if needed
	EmptyHostResolverCacheIfNeeded();
	}

/*
 *	Methods from MSocketControllerStore
 */

void CTcpTransportLayer::SocketControllerShutdown(CSocketController& aOrphanedSocketController)
/**	
	@see		MSocketControllerStore
*/
	{
	// Find the appropriate index for this connector
	TInt index = iControllerStore.Count();
	TBool found = EFalse;
	while( !found && index > 0 )
		{
		found = &aOrphanedSocketController == iControllerStore[--index];
		}
	__ASSERT_DEBUG( found, User::Invariant() );
	
	// Remove the socket controller from the store and compress the store.
	iControllerStore.Remove(index);
	iControllerStore.Compress();
	
   // Empty the host resolver cache if needed
    EmptyHostResolverCacheIfNeeded();
	}

/*
 *	Methods from MCommsInfoProvider
 */

RSocketServ& CTcpTransportLayer::SocketServer()
/**	
	@see		MCommsInfoProvider
*/
	{
	// Callback on the observer for a Comms connection, otherwise create our own
 	// Get socket server handle if set, otherwise start socket server. 
 	// Get RConnection handle if the connection is started else don't create our own
 	// as we may or may not need one. eg; for localhost we don't need RConnection.
    if ( iOwnsSocketConnection )
		{
 	    TInt socketServerHandle = 0;
 	    if( iConnectionPrefsProvider.SupplySocketServerHandle( socketServerHandle ) ) 
			{
            if ( iSocketServer.Handle() != socketServerHandle ) 
				{
 		        iOwnsSocketConnection = EFalse;
                iSocketServer.Close();
 		        __ASSERT_ALWAYS( socketServerHandle!=0, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EExpectedConnectionNotSupplied) );
 		        iSocketServer.SetHandle(socketServerHandle);
 		        
 		        __FLOG_0(_T8("Transport Layer does not own SockServ session"));
 	            // Get the protocol description
 	            iSocketServer.FindProtocol(KTcpProtName(), iProtocolDescription);
                }
 			}
		}
	return iSocketServer;
	}

TProtocolDesc& CTcpTransportLayer::ProtocolDescription()
/**	
	@see		MCommsInfoProvider
*/
	{
	return iProtocolDescription;
	}

RConnection& CTcpTransportLayer::Connection()
/**	
	@see		MCommsInfoProvider
*/
	{
	return *iConnection;
	}

void CTcpTransportLayer::SecurityPreferences(TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy)
/**	
	@see		MCommsInfoProvider
*/
	{
	iConnectionPrefsProvider.GetSecurityPrefs(aDialogPrompt, aSecurityPolicy);
	}

TBool CTcpTransportLayer::OwnsConnection()
/**	
	@see		MCommsInfoProvider
*/
	{
	return iOwnsConnection;
	}


TInt  CTcpTransportLayer::SessionId()
/**	
	@see		MCommsInfoProvider
*/
	{
	return iConnectionPrefsProvider.SessionId();
	}

void CTcpTransportLayer::StartDefaultCommsConnectionL ()
	{
	// See client has one RConnection started
	if ( iConnectionPrefsProvider.SupplyCommsConnection ( iConnection ) )
		{
		__ASSERT_ALWAYS( iConnection!=NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EExpectedConnectionNotSupplied) );			
		iOwnsConnection = EFalse;
		return;
		}	
	// Otherwise create and start one
	iConnection = new(ELeave) RConnection();
	iOwnsConnection = ETrue;
	User::LeaveIfError(iConnection->Open(iSocketServer));
	User::LeaveIfError(iConnection->Start());  //using default CommDb Settings
	iConnectionPrefsProvider.SetCommsConnectionL( iConnection );
	__FLOG_0(_T8("Transport Layer owns default comms connection"));
	}

TBool CTcpTransportLayer::HasConnection()
	{
	if ( iOwnsConnection || iConnection == NULL )
		{
        RConnection* aRConnFromHttpSess;
 		// Get RConnection handle if one available
 		if ( iConnectionPrefsProvider.SupplyCommsConnection ( aRConnFromHttpSess ) )
			{
 			if ( aRConnFromHttpSess != iConnection )
 				{
 				__ASSERT_ALWAYS( aRConnFromHttpSess!=NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::EExpectedConnectionNotSupplied) );			
 				iOwnsConnection = EFalse;
 			    if( iConnection )
 					{
 				    iConnection->Close();
 				    delete iConnection;
 				    }
 	            iConnection = aRConnFromHttpSess;					
 				}
 			}
         }
	return ( iConnection != NULL );
	}

void CTcpTransportLayer::HostResolverFromCache(RHostResolver& aResolver)
    {
    TInt count = iHostResolverCache.Count();
    if(count > 0)
        {
        RDebug::Printf("Returning the host resolver from cache...");
        // Returns the last host resolver that is added
        aResolver = iHostResolverCache[count - 1];
        iHostResolverCache.Remove(count - 1); // Remove from the cache.        
        }
    }

void CTcpTransportLayer::AddToHostResolverCache(RHostResolver& aResolver)
    {
    if(iHostResolverCache.Append(aResolver) != KErrNone)
        {
        aResolver.Close();
        }
    }

void CTcpTransportLayer::EmptyHostResolverCacheIfNeeded()
    {
    // Remove the host resolver if
    // 1/ if the Connector store is empty and
    // 2/ if the socket controller is empty
    // This is important to get the mobility and one click connectivity cases working
    // Otherwise the inconsistent behaviour will result as the RConnection & RSocketServ can be handled
    // entirely outside of the HTTP stack, ie; from the application
    if(iConnectorStore.Count() == 0 && iControllerStore.Count() == 0)
        {
        EmptyHostResolverCache();
        }
    }

void CTcpTransportLayer::EmptyHostResolverCache()
    {
    TInt count = iHostResolverCache.Count();
    while(count > 0)
        {
        // Close the RHostResolver and remove from the array
        iHostResolverCache[count - 1].Close();
        iHostResolverCache.Remove(--count);
        }
    iHostResolverCache.Compress();
    }
