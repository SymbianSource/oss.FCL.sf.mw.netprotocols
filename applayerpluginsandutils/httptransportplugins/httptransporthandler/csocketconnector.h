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

#ifndef __CSOCKETCONNECTOR_H__
#define __CSOCKETCONNECTOR_H__

#include <e32base.h>
#include <in_sock.h>
#include <http/framework/logging.h>

#include "msocketconnector.h"

class CSocket;
class MSocketConnectorStore;
class MSocketConnectObserver;
class MSocketControllerFactory;
class MCommsInfoProvider;


class CSocketConnector : public CActive,
						 public	MSocketConnector
/**	
	The CSocketConnector class provides socket connecting behaviour. Once the
	socket connector object has been started (ConnectL() API) it will notify its
	observer when it has established a connection with the specified remote host.

	The socket connector initially does a DNS lookup for the host name provided
	in the ConnectL() API. Once the IP address has been found for the host name
	the socket connector will attempt to establish a TCP connection with the 
	remote host.

	When a connection has been established a socket controller object is created
	by the socket controller factory to encapsulate the connected socket. This
	provides the input and output streams for the socket.

	The observer, an MSocketConnectObserver object, is notified of a connection
	using the MSocketConnectObserver::ConnectionMadeL() API. The input and output
	streams that encapsulate the connected socket are passed to the observer.

	The connected socket is then placed in the socket controller store by the 
	socket controller factory. This transfers the ownership of the socket
	controller object to the store.

	After the socket controller ownership has been transferred, the socket 
	connector removes itself from the socket connector store and then suicides.

	If the socket connector encounters any problems it notifies its observer using
	the MSocketConnectObserver::HandleConnectError(TInt aError) API. If a problem 
	does occur the socket connector will suicide after handling the error.
	@internalComponent
*/
	{
public:	// methods

	static CSocketConnector* NewL(
								 MSocketConnectorStore&		aStore,
								 MSocketControllerFactory&	aSocketControllerFactory,
								 MCommsInfoProvider&		aCommsInfoProvider,
								 TBool aPriority
								 );

	virtual ~CSocketConnector();

	void ConnectL(MSocketConnectObserver& aObserver, const TDesC8& aRemoteHost, TUint16 aRemotePort, TInetAddr* aRemoteAddress);

private:	// methods from MSocketConnector

	virtual void StopConnect();

	virtual void MSocketConnector_Reserved();

private:	// methods from CActive

	virtual void RunL();
	
	virtual void DoCancel();
	
	virtual TInt RunError(TInt aError);

private:	// methods

	CSocketConnector(
					MSocketConnectorStore&		aStore,
					MSocketControllerFactory&	aSocketControllerFactory,
					MCommsInfoProvider&			aCommsInfoProvider,
					TBool aPriority
					);

	void CompleteSelf();

	void Suicide();
	
	TInt DoPendingDNSLookup();
	TInt DoConnect();
private:	// enums

/**	
	The state machine for the socket connector.
*/
	enum TConnectState
		{
		/** The socket connector is idle.
		*/
		EIdle				= 0,
		/** A connection has been requested. The DNS lookup needs to be initiated
			to find the IP address of the remote host.
		*/
		EPendingDNSLookup,
		/**	The IP address of the remote host has been found. Initiated a TCP
			connection to that remote host.
		*/
		EConnecting,
		/**	The connection has been established. Ownership of the connected socket
			must be passed to the observer.
		*/
		EConnected,
		};

private:	// attributes

/** The socket connector store.
*/
	MSocketConnectorStore&		iStore;

/**	The socket controller factory.
*/
	MSocketControllerFactory&	iSocketControllerFactory;

/** The comms info provider.
*/
	MCommsInfoProvider&			iCommsInfoProvider;

/** The state of the socket connector.
*/
	TConnectState				iState;

/** The host resolver session.
*/
	RHostResolver				iHostResolver;

/** The socket connect observer.
*/
	MSocketConnectObserver*		iObserver;

/**	The host name/IP address for the remote client.
*/
	HBufC*						iHost;

/**	The port number on remote host with which to connect to
*/
	TUint16						iPort;

/** The DNS entry object for the remote client
*/
	TNameEntry					iHostDnsEntry;
	
/** The address of the remote host
*/
	TInetAddr					iAddress;

/**	The socket object that is connecting to the remote client
*/
	CSocket*					iConnectingSocket;

public:	// attributes

/** Logger handle
*/
	__FLOG_DECLARATION_MEMBER2

	};

#endif	// __CSOCKETCONNECTOR_H__
