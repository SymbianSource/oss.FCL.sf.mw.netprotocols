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

#include "csocketlistenerdriver.h"

#include <msocketfactory.h>

#include "cserversocketdriver.h"
#include "thttptrhndtestpanic.h"

GLREF_D TBool 	gInvalidListenPort; 

const TInt KPortNumber	= 80;
const TUint KInvalidPortNumber = 0xffff;

CSocketListenerDriver* CSocketListenerDriver::NewL(MDriverObserver& aObserver, MSocketFactory& aSocketFactory)
/**
	@componentInternal		
	@param		aObserver		The driver observer.
	@param		aSocketFactory	The socket factory.
	@return		A pointer to a fully initialised object.
*/
	{
	return new (ELeave) CSocketListenerDriver(aObserver, aSocketFactory);
	}

CSocketListenerDriver::~CSocketListenerDriver()
/**	
	Destructor.
	@componentInternal		
*/
	{
	iPendingStore.ResetAndDestroy();
	iActiveStore.ResetAndDestroy();
	}

CSocketListenerDriver::CSocketListenerDriver(MDriverObserver& aObserver, MSocketFactory& aSocketFactory)
: CDriverBase(aObserver), iSocketFactory(aSocketFactory)
/**	
	Constructor
	@componentInternal		
	@param		aObserver		The test driver observer
	@param		aSocketFactory	The socket factory.
*/
	{
	}

void CSocketListenerDriver::SetDoubleListenL()
/**
	Sets the command stack to have the following behaviour.

	It starts the listen service in the provided socket factory. It expects a
	connection to be received - it creates a server socket driver to control 
	the connected socket. A second connection is expected to be maded - a 
	second server socket driver is created. The listen service is then stopped 
	once both server socket drivers have closed.
	@componentInternal
*/
	{
	// Create cmd stack
	AppendDoCmdL(EDoListen);
	AppendExpectCmdL(EExpectConnected);
	AppendExpectCmdL(EExpectConnected);
	AppendDoCmdL(EDoStartServerDriver);
	AppendDoCmdL(EDoStartServerDriver);
	AppendExpectCmdL(EExpectServerClosed);
	AppendExpectCmdL(EExpectServerClosed);
	AppendDoCmdL(EDoStopListen);
	AppendExpectCmdL(EExpectListenError);
	}

void CSocketListenerDriver::SetSingleListenL()
/**
	Sets the command stack to have the following behaviour.

	It starts the listen service in the provided socket factory. It expects a
	connection to be received - it creates a server socket driver to control
	the connected socket. The listen service is then stopped once the server 
	socket driver has closed.
	@componentInternal
*/
	{
	// Create cmd stack
	AppendDoCmdL(EDoListen);
	AppendExpectCmdL(EExpectConnected);
	AppendDoCmdL(EDoStartServerDriver);
	AppendExpectCmdL(EExpectServerClosed);
	AppendDoCmdL(EDoStopListen);
	AppendExpectCmdL(EExpectListenError);
	}

/*
 *	Methods from CDriverBase
 */

void CSocketListenerDriver::RunCmdL(TInt aCmd)
/**	
	@see		CDriverBase
	@componentInternal
*/
	{
	// Check the step number
	switch( aCmd )
		{
	case EDoListen:
		{
		DoLog(_L("Socket Listener Driver - start listen"));
		if (!gInvalidListenPort)	
			{	
			iSocketFactory.ListenL(*this, KPortNumber);
			}
		else
			{
			iSocketFactory.ListenL(*this, KInvalidPortNumber);	
			}			
		} break;
	case EDoStopListen:
		{
		DoLog(_L("Socket Listener Driver - stop listen"));

		iSocketFactory.StopListen();
		} break;
	case EDoStartServerDriver:
		{
		// Are there any socket drivers to start?!
		if( iPendingStore.Count() == 0 )
			User::Leave(KErrNotFound);

		// Remove the first driver of the pending store and add to the active store...
		CServerSocketDriver* serverSocketDriver = iPendingStore[0];
		iPendingStore.Remove(0);
		iPendingStore.Compress();
		CleanupStack::PushL(serverSocketDriver);
		User::LeaveIfError(iActiveStore.Append(serverSocketDriver));
		CleanupStack::Pop(serverSocketDriver);

		DoLog(_L("Socket Listener Driver - start server socket driver"));

		// Start the socket driver....
		serverSocketDriver->Start();

		} break;
	default:
		THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EBadCommandId);
		break;
		}
	}

/*
 *	Methods from MSocketListenObserver
 */

void CSocketListenerDriver::ConnectionReceivedL(MInputStream& aInputStream, MOutputStream& aOutputStream)
/**	
	@see		MSocketListenObserver
	@componentInternal
*/
	{
	DoLog(_L("Socket Listener Driver - connection received"));

	if( TestExpectedCmd(EExpectConnected, KErrNone) )
		{
		// Create the server socket driver...
		CServerSocketDriver* serverSocketDriver = CServerSocketDriver::NewL(*this, *this, aInputStream, aOutputStream);
		CleanupStack::PushL(serverSocketDriver);

		// Add to the pending store....
		User::LeaveIfError(iPendingStore.Append(serverSocketDriver));
		CleanupStack::Pop(serverSocketDriver);
		}
	}

TInt CSocketListenerDriver::HandleListenError(TInt aError)
/**	
	@see		MSocketListenObserver
	@componentInternal
*/
	{
	DoLog(_L("Socket Listener Driver - listen error"));

	TestExpectedCmd(EExpectListenError, aError);
	return KErrNone;
	}

void CSocketListenerDriver::MSocketListenObserver_Reserved()
/**	
	@see		MSocketListenObserver
	@componentInternal
*/
	{
	User::Invariant();
	}

/*
 *	Methods from MServerDriverObserver
 */

void CSocketListenerDriver::ServerClosed()
/**	
	@see		MServerDriverObserver
	@componentInternal
*/
	{
	DoLog(_L("Socket Listener Driver - server socket driver closed"));

	TestExpectedCmd(EExpectServerClosed, KErrNone);
	}
