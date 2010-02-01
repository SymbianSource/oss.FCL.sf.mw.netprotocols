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

#ifndef __MSOCKETCONNECTOBSERVER_H__
#define __MSOCKETCONNECTOBSERVER_H__

#include <e32std.h>

class MInputStream;
class MOutputStream;

class MSocketConnectObserver
/**
The MSocketConnectObserver class provides the callback API for notification
of a socket connection request. The observer has requested the connection 
via the MSocketFactory API. 

The observer is either notified of a successful connection or of any errors 
that occur. The socket connect observer can stop the connection using the 
MSocketConnector API. This can only be done if the observer has not been 
notified.
@see		MSocketFactory
@see		MSocketConnector
*/
	{
public:	// methods

/**	
	Notifies the socket connect observer that a connection with the remote host
	has been established. The input and output streams encapsulating the 
	connected socket are provided. 
	@param		aInputStream	The input stream.
	@param		aOutputStream	The output stream.
	@see		MInputStream
	@see		MOutputStream
*/
	virtual void ConnectionMadeL(MInputStream& aInputStream, MOutputStream& aOutputStream) =0;

/**	
	Notifies the socket connect observer that error has occured in the connecting
	service. The error code will have a value of KErrCancel if the observer has
	stopped the connect service via the MSocketConnectObserver::StopConnect() API.
	@param		aError	The error code.
	@return		A value of KErrNone if the error has been handled or any other 
				value other value if its has not been handled.
*/
	virtual TInt HandleConnectError(TInt aError) =0;

private:	// methods

/**
	Reserved function for future expansion.
*/
	virtual void MSocketConnectObserver_Reserved() =0;
	
	};

#endif	// __MSOCKETCONNECTOBSERVER_H__
