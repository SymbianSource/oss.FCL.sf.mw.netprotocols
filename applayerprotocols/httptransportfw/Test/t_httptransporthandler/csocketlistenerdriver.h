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

#ifndef __CSOCKETLISTENERDRIVER_H__
#define __CSOCKETLISTENERDRIVER_H__

#include <e32base.h>
#include <msocketlistenobserver.h>

#include "mserverdriverobserver.h"
#include "cdriverbase.h"

class MSocketFactory;
class CServerSocketDriver;

/**	@class		CSocketListenerDriver
	The CSocketListenerDriver class is the test driver for the listen service
	of a socket factory. It is an abstract class and should derivated from in
	order to create a concrete listener driver. A derived class needs to 
	override the pure virtual function SetCmdsL(). This function sets the 
	command stack in the test driver.
	
	The CSocketListenerDriver implements the MSocketListenObserver API so that
	it can notified of either received connections or errors in the listen 
	service.

	As a test driver to offers several commands to drive the listen service. The
	listen service can be started and stopped, and received connections can be 
	expected. Also, errors in the listen service and be expected.
	
	When a connection is received, the socket listener driver creates a server
	socket driver to control the input and output streams encapsulating the 
	connection. 
	
	Starting the server socket driver and expecting the server socket driver to 
	close are other commands.
	@componentInternal		
	@see		MSocketListenObserver
	@see		MSocketFactory
	@see		CServerSocketDriver
*/
class CSocketListenerDriver : public CDriverBase,
							  public MSocketListenObserver,
							  public MServerDriverObserver
	{
public:	// methods

	static CSocketListenerDriver* NewL(MDriverObserver& aObserver, MSocketFactory& aSocketFactory);

	virtual ~CSocketListenerDriver();

	void SetSingleListenL();

	void SetDoubleListenL();

private:	// methods from MSocketListenObserver

	virtual void ConnectionReceivedL(MInputStream& aInputStream, MOutputStream& aOutputStream);

	virtual TInt HandleListenError(TInt aError);

	virtual void MSocketListenObserver_Reserved();

private:	// methods from MServerDriverObserver

	virtual void ServerClosed();

private:	// methods from CDriverBase

	virtual void RunCmdL(TInt aCmd);

private:	// methods

	CSocketListenerDriver(MDriverObserver& aObserver, MSocketFactory& aSocketFactory);

private:	// enums

/** Enumeration of the command IDs for the socket listener driver.
	@componentInternal		
*/
	enum
		{
		/** 'Do' command to initiate the listen service.
		*/
		EDoListen					=0,
		/** 'Expect' command indicating that a connection to the listen service
			is received.
		*/
		EExpectConnected,
		/** 'Do' command to start the current server socket driver.
		*/
		EDoStartServerDriver,
		/** 'Expect' command indicating that a server socket driver is expected
			to close.
		*/
		EExpectServerClosed,
		/** 'Do' command to stop the listen service.
		*/
		EDoStopListen,
		/** 'Expect' command indicating that a listen service error is expected.
		*/
		EExpectListenError
		};

private:	// attributes

	MSocketFactory&						iSocketFactory;

	RPointerArray<CServerSocketDriver>	iPendingStore;

	RPointerArray<CServerSocketDriver>	iActiveStore;

	};

#endif	// __CSOCKETLISTENERDRIVER_H__
