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

#ifndef __MOUTPUTSTREAM_H__
#define __MOUTPUTSTREAM_H__

#include <e32std.h>

class MOutputStreamObserver;
class CX509Certificate;

class MOutputStream
/**	
The MOutputStream and MOutputStreamObserver classes provide the API to send
data to a connected remote host. They encapsulate the outbound stream of a 
connected socket. 

The output socket observer must bind itself to the output stream before using
any of the other MOutputStream functions. The MOutputStream::Bind() API is 
used to do this binding. When done for the first time the output stream moves
from the Idle state to the PendingSend state.

Once an observer has been bound to the output stream data can be sent to the
connected host using the MOutputStream::SendDataReq() API. This can only be 
done when the output stream is in the PendingSend state otherwise a panic 
occurs. The MOutputStream::SendDataReq() API changes the output stream state
to the SentData state. 

The supplied data buffer must remain valid until the observer is notified 
that the send has been successful. This is done using the 
MOutputStreamObserver::SendDataCnf() API. The output stream moves back to 
the PendingSend state.

The output stream can only be re-bound to another observer when in the 
PendingSend state. The re-binding does not change the state of the output
stream.
	  
There are two ways to shutdown the stream - asynchronously (standard use) and
synchronously. 

In normal use the asynchronous MOutputSocket::ShutdownReq() API can be used. 
The output stream changes to the Closing state. The observer is notified that
the stream has closed via the MOutputStreamObserver::OutputStreamCloseInd()
API. The output stream is then in the Closed state. It is no longer valid to
use the output stream and to do so will cause an access violation.

With the asynchronous shutdown the corresponding input stream is also shutdown.
Its observer is notified that the input stream has been closed. Similarly, 
if the corresponding input stream has been shutdown synchronously or 
asynchronously the output stream observer will be notified that the stream 
has been closed.

The MOutputSocket::Close() API closes the output stream synchronously. In 
this case the output stream observer will not be notified. Once the the call
completes the output stream is in the Closed state and is no longer valid. 
This synchronous close should be used when an asynchronous shutdown is not 
appropriate, e.g. when deleting the observer object in error conditions.

Similar to the asynchronous shutdown the corresponding input stream is also 
shutdown and its observer notified.

The MOutputStream::SecureClientReq() API allows the connection to be upgraded
to be secure. This request initiates a secure handshake - the local host of 
the connection is the client and the remote host is the server in the 
handshake. During the handshake, the corresponding input stream is suspended -
no data will be received.

If the secure handshake is successful the output stream observer is notified 
using the MOutputStreamObserver::SecureClientCnf() API. The input stream
resumes its activity. If the secure handshake was not successful then the 
stream handles the error in the normal manner.

The certificate information for the secure connection can be obtained using 
the MOutputStream::ServerCert() API.
@see		MOutputStreamObserver
*/
	{
public:	// methods

/**	
	This binds an observer to the output stream.
	@param		aObserver	An output stream observer.
	@pre		The output stream is either in the Idle or PendingSend state.
	@post		The output stream is in the PendingSend state.
	@panic		EBadOutputStreamState	The output stream is not in the Idle or
										PendingSend state.
*/
	virtual void Bind(MOutputStreamObserver& aObserver) =0;

/**	
	Requests that the output stream send the supplied data to the connected host.
	The observer will be notified when the data has been successfully sent. The 
	data buffer must remain valid until this notification.
	@pre		The output stream is in the PendingSend state and an observer has
				been bound to it.
	@post		The output stream is in the SentData state.
	@panic		EOutputStreamNotBound	The output stream has no observer bound 
										to it.
	@panic		EBadOutputStreamState	The output stream is not in the 
										PendingSend state.
*/
	virtual void SendDataReqL(const TDesC8& aBuffer) =0;

/**	
	Closes the output stream asynchronously. The corresponding input stream is
	also closed. The output stream observer will be notified when the stream is
	closed. The corresponding input stream observer is also notified.
	@pre		The output stream is not in the Closing or Closed state and an 
				observer has been bound to it.
	@post		The output stream is in the Closing state.
	@panic		EOutputStreamNotBound	The output stream has no observer bound 
										to it.
	@panic		EBadOutputStreamState	The output stream is in the Closing	or 
										Closed state.
*/
	virtual void ShutdownReq() =0;

/**	
	Issue a request to upgrade the client to use a secure connection. The stream
	will issue a secure handshake and wait for a successful response from the
	connected host. The corresponding input stream is suspended until the secure
	handshake completes. 
	@pre		The output stream is in the PendingSend state
	@post		The output stream is in the StartSecureHandshake state and the
				input stream is suspended.
	@param		aHostName	The host name of the request used for checking the
							domain name against the server certificates.
	@panic		EBadOutputStreamState	The output stream is not in the PendingSend
										state.
*/
	virtual void SecureClientReq(const TDesC8& aHostName) = 0;

/**
	Closes the output stream synchronously. The observer is not notified. The
	corresponding input stream is also closed and its observer notified.
	@pre		The output stream is not in the Closing or Closed state and an 
				observer has been bound to it.
	@post		The output stream is in the Closed state and no longer valid.
	@panic		EOutputStreamNotBound	The output stream has no observer bound 
										to it.
	@panic		EBadOutputStreamState	The output stream is in the Closing	or 
										Closed state.
*/
	virtual void Close() =0;

/**	
	Get the Server Certificate for this socket session.
	@return		The server certificate information for the connected host. this may be NULL
				if the information is not available.
*/
	virtual const CX509Certificate* ServerCert() =0;

/**
	Get the current Cipher Suite for this socket session.
	@param		aCipherSuite	A descriptor which will be filled with the cipher suite. 
								This is a 2 digit code as defined by RFC 2246. 
	@return		An error code. KErrNone on sucess. KErrNotSupported if this socket session is not secure.
*/
	virtual TInt CipherSuite(TDes8& aCipherSuite) = 0;

/**
	Resets the state to EClosed
	@componentInternal
*/
	virtual void Reset() =0;

/**
 * Enable/Disable TCP corking during upload 
 * 
 */	
	virtual void SetTCPCorking(TBool aValue) =0;
	
private:	// methods

/**	
	Reserved function for future expansion.
*/
	virtual void MOutputStream_Reserved() =0;

	};

#endif	// __MOUTPUTSTREAM_H__
