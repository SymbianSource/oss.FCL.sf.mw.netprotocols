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

#include "cserversocketdriver.h"

#include "mserverdriverobserver.h"

#include "testdata.h"

// 'this' used in base member initializer list, The 'this' pointer being used is a base class pointer.
#pragma warning( disable : 4355 )

CServerSocketDriver* CServerSocketDriver::NewL(
											  MDriverObserver&			aTestObserver,
											  MServerDriverObserver&	aObserver,
											  MInputStream&				aInputStream,
											  MOutputStream&			aOutputStream
											  )
/**	
	Factory constructor.
	@componentInternal		
	@param		aDriverObserver		The test driver observer.
	@param		aObserver			The observer for the server socket driver.
	@param		aInputStream		The input stream.
	@param		aOutputStream		The output stream.
	@return		A pointer to a fully initialised object.
*/
	{
	CServerSocketDriver* self = new (ELeave) CServerSocketDriver(aTestObserver, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aInputStream, aOutputStream);
	CleanupStack::Pop(self);
	return self;
	}

CServerSocketDriver::~CServerSocketDriver()
/**	
	Destructor.
	@componentInternal		
*/
	{
	}

CServerSocketDriver::CServerSocketDriver(MDriverObserver& aDriverObserver, MServerDriverObserver& aObserver)
: CSocketDriver(aDriverObserver, *this, EFalse), iObserver(aObserver)
/**	
	Constructor.
	@componentInternal		
	@param		aDriverObserver		The test driver observer.
	@param		aObserver			The observer for the server socket driver.
*/
	{
	}

/*
 *	Methods from CSocketDriver
 */

void CServerSocketDriver::SetCmdsL()
/**	
	@see		CSocketDriver
	@componentInternal
*/
	{
	// Create cmd stack...
	AppendDoCmdL(EDoExpectData);
	AppendExpectCmdL(EExpectDataReceived);
	AppendDoCmdL(EDoSendData);
	AppendExpectCmdL(EExpectDataSent);
	AppendExpectCmdL(EExpectReaderClosed);
	AppendExpectCmdL(EExpectWriterClosed);
	}

const TDesC8& CServerSocketDriver::SendData()
/**	
	@see		CSocketDriver
	@componentInternal
*/
	{
	return KResponseData();
	}

const TDesC8& CServerSocketDriver::ReceiveData()
/**	
	@see		CSocketDriver
	@componentInternal
*/
	{
	return KRequestData();
	}

/*
 *	Methods from MSocketDriverObserver
 */

void CServerSocketDriver::SocketClosed()
/**	
	@see		MSocketDriverObserver
	@componentInternal
*/
	{
	iObserver.ServerClosed();
	}
