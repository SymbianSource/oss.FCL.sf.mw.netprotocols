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

#ifndef __MINPUTSTREAMOBSERVER_H__
#define __MINPUTSTREAMOBSERVER_H__

#include <e32std.h>

class MHttpResponse;
class MInputStreamObserver
/**	
The MInputStream and MInputStreamObserver classes provide the API to receive
data from a connected remote host. They encapsulate the inbound stream of a 
connected socket. 

The input socket observer must bind itself to the input stream before using
any of the other MInputStream functions. The MInputStream::Bind() API is 
used to do this binding. The input stream does not do its first read until 
an observer has been bound to it. When done for the first time the input 
stream moves from the Idle state to the Read state.

When the input stream is in the Read state, it requests a read from the 
socket. This moves the input stream into the ReceivedData state and it waits
for the socket to notify it when it receives any data from the remote host. 
When the input stream receives the notification it notifies its observer via
the MInputStreamObserver::ReceivedDataInd() API. The input stream moves into
the PendingAck state.

The buffer containing the received data remains valid until the observer 
notifies the input stream that it is no longer needed. The observer can do
this using the MInputStream::ReceivedDataRes() API. The input stream returns
to the Read state and issues another read request to the socket.

The input stream can only be re-bound to another observer when it is in the 
PendingAck state. This does not change the state of the input stream. The 
new observer is responsible for notifying the input stream when the received 
data buffer is no longer needed.
	  
There are two ways to shutdown the stream - asynchronously (standard use) and
synchronously. 

In normal use the asynchronous MInputSocket::ShutdownReq() API can be used. 
The input stream changes to the Closing state. The observer is notified that
the stream has closed via the MInputStreamObserver::InputStreamCloseInd()
API. The input stream is then in the Closed state. It is no longer valid to
use the input stream and to do so will cause an access violation.

With the asynchronous shutdown the corresponding output stream is also shutdown.
Its observer is notified that the output stream has been closed. Similarly, 
if the corresponding output stream has been shutdown sychronously or 
asynchronously the input stream observer will be notified that the stream 
has been closed.

The MInputSocket::Close() API closes the input stream synchronously. In 
this case the observer will not be notified. Once the the call completes 
the input stream is in the Closed state and is no longer valid. This
synchronous close should be used when an asynchronous shutdown is not 
appropriate, e.g. when deleting the observer object in error conditions.

Similar to the asynchronous shutdown the corresponding output stream is also 
shutdown and its observer notified.
@see		MInputStream
*/
	{
public:	// methods

/**	
	Notifies the observer that data has been received from the connected host.
	The supplied buffer will remain valid until the input stream is notified
	that it is no longer needed.
	@param		aBuffer		A buffer containing the received data.
	@post		The input stream is in the PendingAck state.
*/
	virtual void ReceivedDataIndL(const TDesC8& aBuffer) =0;

/**	
	Notifies the input stream observer that secure upgrade on the connection
	has completed successfully.
	@internalComponent
*/
	virtual void SecureServerCnf() =0;

/**	
	Notifies the input stream observer that the socket connection with the
	remote host has been closed. The input stream is no longer valid.
	@param		aError	The error code explaining why the socket connection has
						closed. A value of KErrNone indicates that either the
						input or output stream observer requested that the 
						socket connection be closed.
	@post		The input stream is in the Closed state and is no longer valid.
*/
	virtual void InputStreamCloseInd(TInt aError) =0;
	
	virtual MHttpResponse* CurrentResponse() =0;
    
	virtual void OnReceiveTimeOut() =0;
	
private:	// methods

/**	
	Reserved function for future expansion.
*/
	virtual void MInputStreamObserver_Reserved() =0;
	};

#endif	// __MINPUTSTREAMOBSERVER_H__
