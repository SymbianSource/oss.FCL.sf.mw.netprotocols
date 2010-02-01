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

#ifndef __CSOCKETLISTENER_H__
#define __CSOCKETLISTENER_H__

#include <e32base.h>
#include <http/framework/logging.h>

class CSocket;
class MSocketListenObserver;
class MSocketControllerFactory;
class MCommsInfoProvider;


class CSocketListener : public CActive
/**	
	The CSocketListener class provides socket listening behaviour. Once a socket
	listener object is started (Listen() API) it will notify its observer when a
	remote host has established a connection with the listening port.

	The observer, an MSocketListenObserver object, is notified of a connection 
	using the MSocketListenObserver::ConnectionReceivedL() API. The input and 
	output streams that encapsulate the connected socket are passed to the 
	observer.

	The socket listener will continue to listen for connecting remote hosts until
	the StopListen() API is called.

	If the socket listener encounters any problems it notified its observer using
	the MSocketListenObserver::HandleListenError(TInt aError) API. If a problem
	does occur the socket listener will revert to its Idle (non-listening) state.
	@internalComponent
*/
	{
public:	// methods

	static CSocketListener* NewL(
								MSocketControllerFactory&	aSocketControllerFactory,
								MCommsInfoProvider&			aCommsInfoProvider,
								TBool aPriority
								);

	virtual ~CSocketListener();

	void Listen(MSocketListenObserver& aObserver, TUint16 aPort);

	void StopListen();

private:	// methods from CActive

	virtual void RunL();
	
	virtual void DoCancel();
	
	virtual TInt RunError(TInt aError);

private:	// methods

	CSocketListener(
				   MSocketControllerFactory&	aSocketControllerFactory,
				   MCommsInfoProvider&			aCommsInfoProvider,
				   TBool aPriority
				   );

	void CompleteSelf();

private:	// enums

/**	
	The state machine for the socket listener.
*/
	enum TListenState
		{
		/**	The socket listener is idle.
		*/
		EIdle			= 0,
		/** The listen observer has requested that the socket listener start
			listening on the specified port.
		*/
		EStartListen,
		/** The socket listener is listening on the specified port.
		*/
		EListening,
		/** The socket listener has been notified that a connection has been 
			established with the listening socket.
		*/
		EConnected
		};

private:	// attributes

/**	The socket controller factory
*/
	MSocketControllerFactory&	iSocketControllerFactory;

/** The comms info provider object.
*/
	MCommsInfoProvider&			iCommsInfoProvider;

/** The state of the socket listener.
*/
	TListenState				iState;

/** The socket listen observer
*/
	MSocketListenObserver*		iObserver;

/**	The port number on which the socket listener is listenining on.
*/
	TUint16						iPort;

/**	The socket object that is listening on the specified port.
*/
	CSocket*					iListeningSocket;

/**	The empty socket object that will accept the connection. It is married to
	the connection by the listening socket.
*/
	CSocket*					iAcceptingSocket;

public:	// attibutes

/** Logger handle
*/
	__FLOG_DECLARATION_MEMBER2

	};

#endif	// __CSOCKETLISTENER_H__
