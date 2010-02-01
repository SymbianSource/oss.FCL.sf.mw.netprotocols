// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MINPUTSTREAM_H__
#define __MINPUTSTREAM_H__

#include <e32std.h>

class MInputStreamObserver;
class CX509Certificate;

class MInputStream
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
@see		MInputStreamObserver
*/
	{
public:	// methods

/**	
	This binds an observer to the input stream. The bound observer is responsible
	for notifying the input stream when the current received data (if any) is no
	longer needed.
	@param		aObserver	An input stream observer.
	@pre		The input stream is either in the Idle or PendingAck state.
	@post		The input stream is in the Read state if the previous state was
				the Idle state, or it remains in the PendingAck state.
	@panic		EBadInputStreamState	The input stream is not in the Idle or
										PendingAck state.
	@internalComponent
*/
	virtual void Bind(MInputStreamObserver& aObserver) =0;

/**	
	Informs the input stream that the received data is no longer needed. The
	input stream can request a read from the socket.
	@pre		The input stream is in the PendingAck state and an observer has
				been bound to it.
	@post		The input stream is in the Read state.
	@panic		EInputStreamNotBound	The input stream has no observer bound 
										to it.
	@panic		EBadInputStreamState	The input stream is not in the EPendingAck
										state.
	@internalComponent
*/
	virtual void ReceivedDataRes() =0;

/**	
	Closes the input stream asynchronously. The corresponding output stream is
	also closed. The input stream observer will be notified when the stream is
	closed. The corresponding output stream observer is also notified.
	@pre		The input stream is not in the Closing or Closed state and an 
				observer has been bound to it.
	@post		The input stream is in the Closing state.
	@panic		EInputStreamNotBound	The input stream has no observer bound 
										to it.
	@panic		EBadInputStreamState	The input stream is in the Closing or 
										Closed state.
	@internalComponent
*/
	virtual void ShutdownReq() =0;

/**	
	Requests that the connection upgrade to a secure connection. The stream will
	wait for to receive a secure handshake.
	@internalComponent
*/
	virtual void SecureServerReq() = 0;

/**	
	Closes the input stream synchronously. The observer is not notified. The
	corresponding output stream is also closed and its observer notified.
	@pre		The input stream is not in the Closing or Closed state and an 
				observer has been bound to it.
	@post		The input stream is in the Closed state and no longer valid.
	@panic		EInputStreamNotBound	The input stream has no observer bound 
										to it.
	@panic		EBadInputStreamState	The input stream is in the Closing or 
										Closed state.
	@internalComponent
*/
	virtual void Close() =0;

/**	
	Get the Client Certificate for this socket session. 
	@return		The server certificate information for the connected host. this may be NULL
					if the information is not available.
	@internalComponent
*/
	virtual const CX509Certificate* ClientCert() =0;
	
/**
	Resets the state to EClosed
	@internalComponent
*/
	virtual void Reset() =0;

	virtual TInt ImmediateRead ( TPtrC8& aData ) =0;
	
	virtual void Restart () =0;
	
	virtual void StartReceieveTimer (TInt aTimeoutValue) =0;
	
/**
	Reserved function for future expansion.	
*/
	virtual void Shutdown() =0;

	};

#endif	// __MINPUTSTREAM_H__
