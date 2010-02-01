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

#include "cclientsocketdriver.h"

#include "mclientdriverobserver.h"
#include "testdata.h"

// 'this' used in base member initializer list, The 'this' pointer being used is a base class pointer.
#pragma warning( disable : 4355 )

CClientSocketDriver* CClientSocketDriver::NewL(
											  MDriverObserver&			aTestObserver,
											  MClientDriverObserver&	aObserver,
											  MInputStream&				aInputStream,
											  MOutputStream&			aOutputStream,
											  TBool						aSecure
											  )
/**	
	Factory constructor.
	@componentInternal		
	@param		aDriverObserver		The test driver observer.
	@param		aObserver			The observer for the client socket driver.
	@param		aInputStream		The input stream.
	@param		aOutputStream		The output stream.
	@param		aSecure				Flag indicating a secure connection.
	@return		A pointer to a fully initialised object.
*/
	{
	CClientSocketDriver* self = new (ELeave) CClientSocketDriver(aTestObserver, aObserver, aSecure);
	CleanupStack::PushL(self);
	self->ConstructL(aInputStream, aOutputStream);
	CleanupStack::Pop(self);
	return self;
	}

CClientSocketDriver::~CClientSocketDriver()
/**	
	Destructor.
	@componentInternal		
*/
	{
	}

CClientSocketDriver::CClientSocketDriver(MDriverObserver& aDriverObserver, MClientDriverObserver& aObserver, TBool aSecure)
: CSocketDriver(aDriverObserver, *this, aSecure), iObserver(aObserver)
/**	
	Constructor.
	@componentInternal		
	@param		aDriverObserver		The test driver observer.
	@param		aObserver			The observer for the client socket driver.
	@param		aSecure				Flag indicating a secure connection.
*/
	{
	}

/*
 *	Methods from CSocketDriver
 */

void CClientSocketDriver::SetCmdsL()
/**	
	@eee		CSocketDriver
	@componentInternal
*/
	{
	// Create cmd stack...
	if(iSecure)
		AppendDoCmdL(EDoSetSecure);
	AppendDoCmdL(EDoSendData);
	AppendExpectCmdL(EExpectDataSent);
	AppendDoCmdL(EDoExpectData);
	AppendExpectCmdL(EExpectDataReceived);
	if(!iSecure) // Don't force the close connection as we dont know how much data will be received from the server
		AppendDoCmdL(EDoReaderClose);
	AppendExpectCmdL(EExpectReaderClosed);
	AppendExpectCmdL(EExpectWriterClosed);
	}

const TDesC8& CClientSocketDriver::SendData()
/**	
	@eee		CSocketDriver
	@componentInternal
*/
	{
	// We need to send different data if the connection is secure as we are connecting to a real server
	if(iSecure)
		return KSecureRequestData();

	return KRequestData();
	}

const TDesC8& CClientSocketDriver::ReceiveData()
/**	
	@eee		CSocketDriver
	@componentInternal
*/
	{
	// Expect different data when using a secure connection as we are connecting to a real server
	if(iSecure)
		return KSecureResponseData();

	return KResponseData();
	}

/*
 *	Methods from MSocketDriverObserver
 */

void CClientSocketDriver::SocketClosed()
/**	
	@eee		MSocketDriverObserver
	@componentInternal
*/
	{
	iObserver.ClientClosed();
	}
