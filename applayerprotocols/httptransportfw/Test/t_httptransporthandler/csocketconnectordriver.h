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

#ifndef __CSOCKETCONNECTORDRIVER_H__
#define __CSOCKETCONNECTORDRIVER_H__

#include <e32base.h>
#include <msocketconnectobserver.h>

#include "mclientdriverobserver.h"
#include "cdriverbase.h"

class MSocketFactory;
class MSocketConnector;
class CClientSocketDriver;

/**	@class		CSocketConnectorDriver
	The CSocketConnectorDriver class is the test driver for the connect service
	of a socket factory. It is an abstract class and should derivated from in
	order to create a concrete connector driver. A derived class needs to 
	override the pure virtual function SetCmdsL(). This function sets the 
	command stack in the test driver.
	
	The CSocketConnectorDriver implements the MSocketConnectObserver API so that
	it can notified of either received connections or errors in the listen 
	service.

	As a test driver to offers several commands to drive the connect service. 
	The connect service can be started and stopped (via the MSocketConnector 
	API), and established connections can be expected. Also, errors in the connect
	service and be expected.
	
	When a connection is received, the socket connector driver creates a client
	socket driver to control the input and output streams encapsulating the 
	connection. 
	
	Starting the client socket driver and expecting the client socket driver to 
	close are other commands.
	@componentInternal		
	@see		MSocketConnectObserver
	@see		MSocketFactory
	@see		CClientSocketDriver
*/
class CSocketConnectorDriver : public CDriverBase,
							   public MSocketConnectObserver,
							   public MClientDriverObserver
	{
public:	// methods

	static CSocketConnectorDriver* NewL(MDriverObserver& aObserver, MSocketFactory& aSocketFactory);

	virtual ~CSocketConnectorDriver();

	void SetDoubleConnectL();

	void SetSingleConnectL();

	void SetSingleSecureConnectL();

private:	// methods from MSocketConnectObserver

	virtual void ConnectionMadeL(MInputStream& aInputStream, MOutputStream& aOutputStream);

	virtual TInt HandleConnectError(TInt aError);

	virtual void MSocketConnectObserver_Reserved();

private:	// methods from MClientDriverObserver

	virtual void ClientClosed();

private:	// methods from CDriverBase

	virtual void RunCmdL(TInt aCmd);

private:	// methods

	CSocketConnectorDriver(MDriverObserver& aObserver, MSocketFactory& aSocketFactory);

	void ConstructL();

private:	// enums

/** Enumeration of the command IDs for the socket connector driver.
	@componentInternal		
*/
	enum
		{
		/**	'Do' command to initialise a conenction.
		*/
		EDoConnect					=0,
		/** 'Do' command to initialise a secure connection.
		*/
		EDoSecureConnect,
		/**	'Expect' command indicating that a connection complete is expected.
		*/
		EExpectConnected,
		/**	'Do' command to start the client socket driver object.
		*/
		EDoStartClientDriver,
		/**	'Expect' command indicating that the client socket driver closing is
			expected.
		*/
		EExpectClientClosed,
		/**	'Do' command to stop the current connection.
		*/
		EDoStopConnect,
		/**	'Expect' command indicating that a connection error is expected.
		*/
		EExpectConnectError
		};

private:	// attributes

	MSocketFactory&			iSocketFactory;

	MSocketConnector*		iSocketConnector;

	CClientSocketDriver*	iClientSocketDriver;

	TBool					iSecure;

	};

#endif	// __CSOCKETCONNECTORDRIVER_H__
