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

#ifndef __MSOCKETCONNECTOR_H__
#define __MSOCKETCONNECTOR_H__

#include <e32std.h>

class MSocketConnector
/**	
The MSocketConnector class provides an API to control a socket connection
request. The socket connection would have been requested via the 
MSocketFactory API. An observer is required for the socket connection - a
MSocketConnectObserver object.

The MSocketConnector can stop the connection only if the socket connect
observer has not notified of either a successful connection or that an error
has occurred.

If the connection is stopped then the observer will receive no notification
and the MSocketConnector object is longer valid.
@see		MSocketFactory
@see		MSocketConnectObserver
*/
	{
public:	// methods

/**	
	Stops the connect service. The connect service can only be stopped if the
	connection has not been established - i.e. before the appropriate 
	MSocketConnectObserver object has been called ConnectionMadeL() API. The
	connect observer will be notified on the MSocketConnectObserver::HandleConnectError()
	API with an error code of KErrCancel when the connect service has actually
	stopped.
	@pre		The socket connect observer has not been notified.
	@post		The socket connector is no longer valid.
*/
	virtual void StopConnect() =0;

private:	// methods

/**
	Reserved function for future expansion.
*/
	virtual void MSocketConnector_Reserved() =0;

	};

#endif	// __MSOCKETCONNECTOR_H__
