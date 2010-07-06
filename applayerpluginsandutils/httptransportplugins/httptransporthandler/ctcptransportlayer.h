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

#ifndef __CTCPTRANSPORTLAYER_H__
#define __CTCPTRANSPORTLAYER_H__

#include <e32base.h>
#include <in_sock.h>
#include <http/framework/logging.h>

#include "chttptransportlayer.h"
#include "msocketcontrollerfactory.h"
#include "msocketconnectorstore.h"
#include "msocketcontrollerstore.h"
#include "mcommsinfoprovider.h"

class CSocket;
class CSocketListener;
class CSocketConnector;
class CSocketController;
class MConnectionPrefsProvider;

const TUint KUidTcpTransportLayer = 0x101f55ed;

class CTcpTransportLayer : public CHttpTransportLayer,
						   public MSocketControllerFactory,
						   public MSocketConnectorStore,
						   public MSocketControllerStore,
						   public MCommsInfoProvider
/**	
	The CTcpTransportLayer class provides an implementation of the ECOM 
	interface CHttpdTransportHandler. This implementation uses a TCP/IP transport
	to provide input and output streams to connected remote hosts.

	The CTcpTransportLayer class also provides the MSocketFactory API. This 
	API together with the MSocketListenObserver and MSocketConnectObserver APIs
	are used to supply input and output streams to remote hosts.
	@internalComponent
	@see		CHttpTransportLayer
	@see		MSocketListenObserver
	@see		MSocketConnectObserver
*/
	{						 
public:	// methods

	static CTcpTransportLayer* NewL(TAny* aTransportConstructionParams);

	virtual ~CTcpTransportLayer();

private:	// methods from CHttpTransportLayer

	virtual void CHttpTransportLayer_Reserved();
	
private:	// methods from MSocketFactory

	virtual void ListenL(MSocketListenObserver& aObserver, TUint16 aPort);
	
	virtual void StopListen();

	virtual MSocketConnector& ConnectL(MSocketConnectObserver& aObserver, const TDesC8& aRemoteHost, TUint16 aRemotePort);

	virtual void MSocketFactory_Reserved();

private:	// methods from MSocketControllerFactory

	virtual CSocketController* CreateSocketControllerLC(CSocket* aConnectedSocket);
	virtual CSocketController* CreateSocketControllerLC(CSocket* aConnectedSocket, const TDesC& aRemoteHost, TUint16 aRemotePort, const TInetAddr& aRemoteAddr);
	virtual void AddToStoreL(CSocketController* aSocketController);

private:	// methods from MSocketConnectorStore

	virtual void ConnectionCompleted(CSocketConnector& aOrphanedSocketConnector);

private:	// methods from MSocketControllerStore

	virtual void SocketControllerShutdown(CSocketController& aOrphanedSocketController);

private:	// methods from MCommsInfoProvider

	virtual RSocketServ& SocketServer();

	virtual TProtocolDesc& ProtocolDescription();

	virtual RConnection& Connection();

	virtual void SecurityPreferences(TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy);

	virtual TBool OwnsConnection();
	
	virtual TInt SessionId();

	virtual TBool HasConnection();
	
	virtual void StartDefaultCommsConnectionL ();

	virtual void HostResolverFromCache(RHostResolver& aResolver);
	
	virtual void AddToHostResolverCache(RHostResolver& aResolver);

private:	// methods

	CTcpTransportLayer(MConnectionPrefsProvider& aTransLayerObserver);
	
	void ConstructL(THttpTransportConstructionParams* aTransportConstructionParams);

	inline MSocketControllerFactory& SocketControllerFactory();

	inline MSocketConnectorStore& SocketConnectorStore();

	inline MSocketControllerStore& SocketControllerStore();

	inline MCommsInfoProvider& CommsInfoProvider();
	
	void EmptyHostResolverCacheIfNeeded();
	
	void EmptyHostResolverCache();
	
private:	// attributes

/** The connection preferences provider
*/
	MConnectionPrefsProvider&			iConnectionPrefsProvider;

/**	The socket server session.
*/
	RSocketServ							iSocketServer;

/**	The protocol description.
*/
	TProtocolDesc						iProtocolDescription;

/** Pointer to the socket connection required for multi-homing
*/
	RConnection*						iConnection;

/** Flag to indicate whether the comms connection is owned
*/
	TBool								iOwnsConnection;

/**	Flag to indicate whether the socket connection is owned
*/	
	TBool								iOwnsSocketConnection;

/**	The socket listener object
*/
	CSocketListener*					iSocketListener;

/**	The socket connector store
*/
	RPointerArray<CSocketConnector>		iConnectorStore;

/** The socket controller store
*/
	RPointerArray<CSocketController>	iControllerStore;
	
	RArray<RHostResolver>               iHostResolverCache;
	
	TBool iPriority;

/** Logger handle
*/
	__FLOG_DECLARATION_MEMBER2

	};

#include "ctcptransportlayer.inl"

#endif //	__CTCPTRANSPORTLAYER_H__
