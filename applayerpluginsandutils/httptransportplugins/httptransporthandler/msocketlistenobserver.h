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

#ifndef __MSOCKETLISTENOBSERVER_H__
#define __MSOCKETLISTENOBSERVER_H__

#include <e32std.h>

class MInputStream;
class MOutputStream;

class MSocketListenObserver
/**	
The MSocketListenObserver class provides the callback API for notification
of a socket listening request. The observer has requested the listening
service via the MSocketFactory API.

The observer is either notified of a received connection or of any errors 
that occur in the listening service. The listening service can be stopped 
via the MSocketFactory API. 

If any errors occur in the listening service then the service is stopped. If
a connection is received, the listening service continues.	
@see		MSocketFactory
*/
	{
public:	// methods

/**	
	Notifies the socket listen observer that a connection with a remote host
	has been received. The input and output streams encapsulating the connected
	socket are provided. 
	@param		aInputStream	The input stream.
	@param		aOutputStream	The output stream.
	@see		MInputStream
	@see		MOutputStream
*/
	virtual void ConnectionReceivedL(MInputStream& aInputStream, MOutputStream& aOutputStream) =0;

/**	
	Notifies the socket listen observer that error has occured in the listening
	service. The error code will have a value of KErrCancel if the observer has
	stopped the listen service via the MSocketFactory::StopListen() API.
	@param		aError	The error code.
	@return		A value of KErrNone if the error has been handled or any other 
				value other value if its has not been handled.
*/
	virtual TInt HandleListenError(TInt aError) =0;

private:	// methods

/**
	Reserved function for future expansion.
*/
	virtual void MSocketListenObserver_Reserved() =0;

	};

#endif	// __MSOCKETLISTENOBSERVER_H__
