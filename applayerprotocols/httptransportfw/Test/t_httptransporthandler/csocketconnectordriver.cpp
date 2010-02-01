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

#include "csocketconnectordriver.h"

#include <msocketfactory.h>
#include <msocketconnector.h>

#include "cclientsocketdriver.h"
#include "thttptrhndtestpanic.h"

const TInt KPortNumber	= 80;
const TInt KSecurePortNumber	= 443;

GLREF_D TBool	gLongHostName;
GLREF_D TBool 	gInvalidConnectPort; 

CSocketConnectorDriver* CSocketConnectorDriver::NewL(MDriverObserver& aObserver, MSocketFactory& aSocketFactory)
/**
	@componentInternal		
	@param		aObserver		The driver observer.
	@param		aSocketFactory	The socket factory.
	@return		A pointer to a fully initialised object.
*/
	{
	return new (ELeave) CSocketConnectorDriver(aObserver, aSocketFactory);
	}

CSocketConnectorDriver::~CSocketConnectorDriver()
/**	
	Destructor
	@componentInternal		
*/
	{
	delete iClientSocketDriver;
	}

CSocketConnectorDriver::CSocketConnectorDriver(MDriverObserver& aObserver, MSocketFactory& aSocketFactory)
: CDriverBase(aObserver), iSocketFactory(aSocketFactory)
/**
	Constructor
	@componentInternal		
	@param		aObserver		The driver observer.
	@param		aSocketFactory	The socket factory.
*/
	{
	}

void CSocketConnectorDriver::SetDoubleConnectL()
/**
	Sets the command stack to have the following behaviour.

	It initiates a connect service in the provided socket factory. It expects a
	connection to be made - it creates a client socket driver to control the
	connected socket. The client socket driver is expected to close.

	Then a second connection is initiated and then immediately stopped. A wait
	delay is started and the test driver terminates once the wait has completed.
	@componentInternal
*/
	{
	// Create cmd stack
	AppendDoCmdL(EDoConnect);
	AppendExpectCmdL(EExpectConnected);
	AppendDoCmdL(EDoStartClientDriver);
	AppendExpectCmdL(EExpectClientClosed);
	AppendDoCmdL(EDoConnect);
	AppendDoCmdL(EDoStopConnect);
	AppendExpectCmdL(EExpectConnectError);
	}

void CSocketConnectorDriver::SetSingleConnectL()
/**
	Sets the command stack to have the following behaviour.

	It initiates a connect service in the provided socket factory. It expects a
	connection to be made - it creates a client socket driver to control the 
	connected socket. The client socket driver is expected to close.
	@componentInternal
*/
	{
	// Create cmd stack
	AppendDoCmdL(EDoConnect);
	AppendExpectCmdL(EExpectConnected);
	AppendDoCmdL(EDoStartClientDriver);
	AppendExpectCmdL(EExpectClientClosed);
	}

void CSocketConnectorDriver::SetSingleSecureConnectL()
/**
	Sets the command stack to have the following behaviour.

	It initiates a secure connect service in the provided socket factory. It expects a
	connection to be made - it creates a client socket driver to control the 
	connected socket. The client socket driver is expected to close.
	@componentInternal
*/
	{
	// Create cmd stack
	AppendDoCmdL(EDoSecureConnect);
	AppendExpectCmdL(EExpectConnected);
	AppendDoCmdL(EDoStartClientDriver);
	AppendExpectCmdL(EExpectClientClosed);
	}

/*
 *	Methods from CDriverBase
 */

void CSocketConnectorDriver::RunCmdL(TInt aCmd)
/**	
	@see		CDriverBase
	@componentInternal
*/
	{
	// Check the step number
	switch( aCmd )
		{
	case EDoConnect:
		{
		DoLog(_L("Socket Connector Driver - initiate connection"));
		
		if(gLongHostName)
			{
			iSocketConnector = &iSocketFactory.ConnectL(*this, _L8(": This is a long Invalid address to which connection cannot be established                                                                                       "), KPortNumber);
			}
		else
			{
			iSocketConnector = &iSocketFactory.ConnectL(*this, _L8("127.0.0.1"), KPortNumber);
			}
		} break;
	case EDoSecureConnect:
		{
		DoLog(_L("Socket Connector Driver - initiate secure connection"));
		iSecure = ETrue;
		if (!gInvalidConnectPort)
			{
			iSocketConnector = &iSocketFactory.ConnectL(*this, _L8("lon-cn-lxwaptest2.closedtest.intra"), KSecurePortNumber);	
			}
		else
			{
			iSocketConnector = &iSocketFactory.ConnectL(*this, _L8("lon-cn-lxwaptest2.closedtest.intra"), KPortNumber);
			}			
		} break;
	case EDoStartClientDriver:
		{
		__ASSERT_DEBUG( iClientSocketDriver, User::Invariant() );

		DoLog(_L("Socket Connector Driver - start client socket driver"));

		iClientSocketDriver->Start();		
		} break;
	case EDoStopConnect:
		{
		DoLog(_L("Socket Connector Driver - stop connection"));

		iSocketConnector->StopConnect();
		} break;
	default:
		__ASSERT_DEBUG( 0, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EBadCommandId) );
		break;
		}
	}

/*
 *	Methods from MSocketConnectObserver
 */

void CSocketConnectorDriver::ConnectionMadeL(MInputStream& aInputStream, MOutputStream& aOutputStream)
/**	
	@see		MSocketConnectObserver
	@componentInternal
*/
	{
	DoLog(_L("Socket Connector Driver - connection made"));

	if( TestExpectedCmd(EExpectConnected, KErrNone) )
		{
		iClientSocketDriver = CClientSocketDriver::NewL(*this, *this, aInputStream, aOutputStream, iSecure);
		}
	}

TInt CSocketConnectorDriver::HandleConnectError(TInt aError)
/**	
	@see		MSocketConnectObserver
	@componentInternal
*/
	{
	DoLog(_L("Socket Connector Driver - connect error"));

	TestExpectedCmd(EExpectConnectError, aError);
	return KErrNone;
	}

void CSocketConnectorDriver::MSocketConnectObserver_Reserved()
/**	
	@see		MSocketConnectObserver
	@componentInternal
*/
	{
	User::Invariant();
	}

/*
 *	Methods from MClientDriverObserver
 */

void CSocketConnectorDriver::ClientClosed()
/**	
	@see		MClientDriverObserver
	@componentInternal
*/
	{
	DoLog(_L("Socket Connector Driver - client driver closed"));

	TestExpectedCmd(EExpectClientClosed, KErrNone);
	}
