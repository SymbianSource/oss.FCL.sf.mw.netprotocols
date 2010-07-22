// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "csocket.h"
#include "chttpsecuresocket.h"
#include "mcommsinfoprovider.h"
#include "thttptrlayerpanic.h"

#include  <http/rhttpconnectioninfo.h>
#include <httpsocketconstants.h>
#include <in6_opt.h> 

const TInt KDefaultFlags		= 0;
const TInt KTcpTriggeredKeepAlive	= 2;
const TInt KSocketRecvBufSize = 16 * 1024;
const TInt KSocketSendBufSize = 16 * 1024;
const TInt KSocketDefaultSendBufSize = 4 * 1024;

CSocket* CSocket::NewL(MCommsInfoProvider& aCommsInfoProvider, TSocketType aSocketType)
/**	
	The factory constructor.
	@param		aCommsInfoProvider	The comms info provider object.
	@param		aSocketType			The type of the socket.
	@return		A pointer to a fully constructed object.
*/
	{
	CSocket* self = new (ELeave) CSocket(aCommsInfoProvider);
	CleanupStack::PushL(self);
	self->ConstructL(aSocketType);
	CleanupStack::Pop(self);
	return self;
	}

CSocket* CSocket::New(MCommsInfoProvider& aCommsInfoProvider, TSocketType aSocketType)
    {
    CSocket* self = new CSocket(aCommsInfoProvider);
    if(self)
        {
        TInt error = self->Construct(aSocketType);
        if(error != KErrNone)
            {
            delete self;
            self = NULL;
            }
        }
    return self;
    }

CSocket::~CSocket()
/**
	Destructor.
*/
	{
	// Close socket and notify the socket status observer
	if( iSecureSocketController )
		delete iSecureSocketController;
	else
		iSocket.Close();
	}

CSocket::CSocket(MCommsInfoProvider& aCommsInfoProvider)
: CBase(), iCommsInfoProvider(aCommsInfoProvider)
/**	
	Constructor.
	@param		aCommsInfoProvider	The comms info provider object.
*/
	{
	}

void CSocket::ConstructL(TSocketType aSocketType)
/**	
	Second phase constructor.
	@param		aSocketType	The type of the socket.
*/
	{
    User::LeaveIfError(Construct(aSocketType));
	}

TInt CSocket::Construct(TSocketType aSocketType)
    {
    TInt error = KErrNone;
    switch( aSocketType )
        {
    case EProtocolSocket:
        {
        if ( iCommsInfoProvider.HasConnection() )
            {
            // Open a protocol socket with a RConnection
            error = iSocket.Open(iCommsInfoProvider.SocketServer(),
                                 iCommsInfoProvider.ProtocolDescription().iAddrFamily, 
                                 iCommsInfoProvider.ProtocolDescription().iSockType, 
                                 iCommsInfoProvider.ProtocolDescription().iProtocol,
                                 iCommsInfoProvider.Connection()
                                 );              
            }
        else
            {
            // Open a protocol socket without a RConnection, we don't need one ( Loopback address )
            error = iSocket.Open(iCommsInfoProvider.SocketServer(),
                                 iCommsInfoProvider.ProtocolDescription().iAddrFamily, 
                                 iCommsInfoProvider.ProtocolDescription().iSockType, 
                                 iCommsInfoProvider.ProtocolDescription().iProtocol
                                 );                                          
            }
        } break;
    case EBlankSocket:
        {
        // Open a blank socket
        error = iSocket.Open(iCommsInfoProvider.SocketServer());
        } break;
    default:
        User::Invariant();
        }
    if(error == KErrNone)
        {
        TInt id = iCommsInfoProvider.SessionId();
        if(id>=0) 
            {
            // set socket option
            iSocket.SetOpt(KSOHttpSessionId, KSOLHttpSessionInfo, id);
            iSocket.SetOpt(KSoTcpKeepAlive, KSolInetTcp, KTcpTriggeredKeepAlive);
            }
        if(aSocketType != EBlankSocket)
            {
            iSocket.SetOpt(KSoTcpNoDelay,KSolInetTcp,1);  // Disable the nagle.
            iSocket.SetOpt(KSORecvBuf, KSOLSocket, KSocketRecvBufSize); // Set the socket recv buf to be 16K
            }
        }
    return error;
    }

TInt CSocket::Listen(TUint aQSize, TUint16 aPort)
/**	
	Start the listen service. The socket is bound to the local port specified by
	aPort. The listen service is then started. The aQSize argument specifies the
	number of connections that can be received simultaneously, awaiting to be 
	accepted.
	@param		aQSize		The length of the listening queue.
	@param		aPort		The local port number on which to listen.
	@return		A value of KErrNone if the listen service was successfully 
				started.
*/
	{
	// Bind the socket to the port
	TSockAddr addr;
	addr.SetPort(aPort);

	TInt error = iSocket.Bind(addr);

	if( error == KErrNone )
		{
		// Start the listening service
		error = iSocket.Listen(aQSize);
		}
	return error;
	}

void CSocket::Accept(CSocket& aBlankSocket, TRequestStatus& aStatus)
/**	
	Start asynchronous accept service. The socket should have had the listening
	service started. When a connection has been received, the blank socket will
	be given the connection. The request status is completed either when a 
	connection is received or an error has occured - this is reflected in the 
	value of the request status.
	@param		aBlankSocket	The blank socket which will be given the 
								connection if one is received.
	@param		aStatus			The request status that is completed when the
								accept service completes.
*/
	{
	if( iSecureSocketController )
		{
		// Secure socket do not support secure listening
		aStatus=KRequestPending;
		TRequestStatus* pStat = &aStatus;
		User::RequestComplete(pStat, KErrNotSupported);
		}
	else
		iSocket.Accept(aBlankSocket.iSocket, aStatus);
	}

void CSocket::CancelAccept()
/**	
	Cancel the accept service.
*/
	{
	iSocket.CancelAccept();
	}

void CSocket::Connect(TInetAddr& aAddr, TRequestStatus& aStatus)
/**	
	Start asynchronous connect service. The address contains the IP address and
	port with which a tcp connection should be established with. The request 
	status is completed either when a connection has been established or an error
	has occurred - this is reflected in the value of the request status.
	@param		aAddr	The IP address and port of the remote host.
	@param		aStatus	The request status that is completed when the connect 
						service completes.
*/
	{
	iSocket.Connect(aAddr, aStatus);
	}

void CSocket::CancelConnect()
/**	
	Cancel the connect service.
*/
	{
	iSocket.CancelConnect();
	}

void CSocket::RecvOneOrMore(TDes8& aBuffer, TRequestStatus& aStatus)
/**	
	Receive data from socket asynchronously. Any data received by the socket is 
	placed in the buffer supplied by aBuffer. The request status is completed 
	either when data has been received or an error has occurred - this is 
	reflected in the value of the request status.
	@param		aBuffer	The buffer where any received data is placed.	
	@param		aStatus	The request status that is completed when the receive 
						service completes.
*/
	{
	if( iSecureSocketController )
		// aFlags not used for secure sockets
		iSecureSocketController->RecvOneOrMore(aBuffer, aStatus, iBytesReceived);
	else
		iSocket.RecvOneOrMore(aBuffer, KDefaultFlags, aStatus, iBytesReceived);
	}

void CSocket::CancelRecv()
/**	
	Cancel the receive service.
*/
	{
	if( iSecureSocketController )
		iSecureSocketController->CancelRecv();
	else
		iSocket.CancelRecv();
	}

void CSocket::Send(const TDesC8& aBuffer, TRequestStatus& aStatus)
/**	
	Send data to the socket asynchronously. The data in the supplied buffer is 
	sent to the socket. The request status is completed either when data has 
	been sent or an error has occurred - this is reflected in the value of the
	request status.
	@param		aBuffer	The buffer containing the data to be sent.
	@param		aStatus	The request status that is completed when the send 
						service completes.
*/
	{
	if( iSecureSocketController )
		iSecureSocketController->Send(aBuffer, aStatus);
	else
		iSocket.Write(aBuffer, aStatus);
	}

void CSocket::CancelSend()
/**	
	Cancel the send service.
*/
	{
	if( iSecureSocketController )
		iSecureSocketController->CancelSend();
	else
		iSocket.CancelWrite();
	}

void CSocket::Shutdown(TRequestStatus& aStatus)
/**	
	Shutdown the connection asynchronously. Any pending receive or send requests
	are allowed to complete before the connection is shutdown. The request 
	status is completed when either the connection has shutdown or an error has
	occurred - this is reflected in the value of the request status. This request
	cannot be cancelled. This method does not apply to secure connections. The
	request will complete with KErrNotSupported if used with a secure connection.
	@param		aStatus	The request status that is completed when the shutdown 
						service completes.
*/
	{
	if( iSecureSocketController )
		{
		// Not supported with secure connections
		aStatus=KRequestPending;
		TRequestStatus* pStat = &aStatus;
		User::RequestComplete(pStat, KErrNotSupported);
		}
	else
		iSocket.Shutdown(RSocket::ENormal, aStatus);
	}
	
void CSocket::ShutdownImmediate()
/**	
	Shutdown the connection synchronously. This method will shutdown the socket
	connection immediately using the abortive closing functionality. This method
	has no effect with secure connections.
*/
	{
	if( iSecureSocketController==NULL )
		{
		TRequestStatus shutdownStatus = KRequestPending;
		iSocket.Shutdown(RSocket::EImmediate, shutdownStatus);
		User::WaitForRequest(shutdownStatus);
		}
	}

void CSocket::RemoteName(TInetAddr& aAddr)
/**	
	Get the remote host name. The IP address and port of the remote host is set
	in the output argument.
	@param		aAddr	The output argument where the IP address and port of the
						remote host is placed.
*/
	{
	iSocket.RemoteName(aAddr);
	}

void CSocket::LocalName(TInetAddr& aAddr)
/**	
	Get the local socket name. The IP address and port of the local socket is 
	set in the output argument.
	@param		aAddr	The output argument where the IP address and port of the
						local socket is placed.
*/
	{
	iSocket.LocalName(aAddr);
	}

const CX509Certificate* CSocket::ServerCert()
 /**
 	Get the Server Certificate for this socket session.
	@return		A Certificate with the certificate information. This may be 
				NULL if the information is not available.
 */
 	{
 	if( iSecureSocketController )
		{
		return iSecureSocketController->ServerCert();
		}
	return NULL;
 	}
 	
	
TInt CSocket::CipherSuite(TDes8& aCipherSuite)
/**
	Get the current Cipher Suite for this socket session.
	@param		aCiptherSuite	A descriptor which will be filled with the cipher suite. 
								This is a 2 digit code as defined by RFC 2246. 
	@return		An error code. KErrNone on sucess. KErrNotSupported if this socket session is not secure.
*/
	{
	if (iSecureSocketController)
		return iSecureSocketController->CipherSuite(aCipherSuite);
	//else
	return KErrNotSupported;
	}

void CSocket::UpgradeToSecureL(TRequestStatus& aStatus, const TDesC8& aHostName,const TDesC& aProtocol)
/**
	Asynchronous request to upgrade the socket to a secure socket.
	@param	aStatus The request status, this will complete with KErrNone on the successful
					upgrade to a secure connection.
	@param	aHostName The host name of the server used for domain name checking
*/
	{
	// Check that the socket is already a secure one.
	if( iSecureSocketController )
		{
		// The socket connection is already secure, simply complete the request
		aStatus=KRequestPending;
		TRequestStatus* pStat = &aStatus;
		User::RequestComplete(pStat, KErrNone);
		}
	else
		{
		// Create the secure socket controller and start the secure handshake
		TRAPD(error, iSecureSocketController = CHttpSecureSocket::NewL(iSocket, iCommsInfoProvider));
		if (error == KErrNotFound || error == KEComErrNoInterfaceIdentified)
			User::Leave(KErrNotSupported);
		else if (error != KErrNone)
			User::Leave(error);
		iSecureSocketController->StartSecureHandshakeL(aStatus, aHostName,aProtocol);
		}
	}

void CSocket::CancelUpgradeToSecure()
/**
	Cancel the upgrade to a secure connection.
*/
	{
	if( iSecureSocketController )
		iSecureSocketController->CancelHandshake();
	}

TInt CSocket::PendingBytesToRead ()
	{
    if ( iSecureSocketController )
		    return iSecureSocketController->PendingBytesToRead ();
	TInt bytesToRead;
	TInt err = iSocket.GetOpt ( KSOReadBytesPending, KSOLSocket, bytesToRead );
	if ( err == KErrNone )
		return bytesToRead;
	return err;
	}
	
void CSocket::SetTCPCorking(TBool aValue)
    {
    if(aValue)
    	{
    	iSocket.SetOpt(KSoTcpNoDelay,KSolInetTcp,0);  // Disable the nagle.
      iSocket.SetOpt(KSOSendBuf, KSOLSocket, KSocketSendBufSize);
      }
    else
    	{
    	iSocket.SetOpt(KSoTcpNoDelay,KSolInetTcp,1);  // Disable the nagle.
      iSocket.SetOpt(KSOSendBuf, KSOLSocket, KSocketDefaultSendBufSize);        
      }
    
    iSocket.SetOpt(KSoTcpCork, KSolInetTcp, aValue);       
    
    }
