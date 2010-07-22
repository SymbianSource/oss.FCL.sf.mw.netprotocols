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

#ifndef __CSOCKET_H__
#define __CSOCKET_H__

#include <e32base.h>
#include <in_sock.h>

class MCommsInfoProvider;
class CHttpSecureSocket;
class CX509Certificate;

class CSocket: public CBase
/**	
	The CSocket class encapsulates a socket. It provides a limited version of
	the RSocket API. It can expanded to encapsulate the CSecureSocket API too.
	@internalComponent
*/
	{
public:	// enums

/**	
	The TSocketType enumerates the type of socket, e.g if the socket should be
	initiased for a specified protocol. Can be expanded to specify a secure 
	socket.
	
*/
	enum TSocketType
		{
		/**	The socket should be initialised for the specified protocol.
		*/
		EProtocolSocket			= 0,
		/**	The socket should be not be initialised for any protocol.
		*/
		EBlankSocket,
		};

public:	// methods

	static CSocket* NewL(MCommsInfoProvider& aCommsInfoProvider, TSocketType aSocketType);
	
	static CSocket* New(MCommsInfoProvider& aCommsInfoProvider, TSocketType aSocketType);
	
	virtual ~CSocket();

	TInt Listen(TUint aQSize, TUint16 aPort);

	void Accept(CSocket& aBlankSocket, TRequestStatus& aStatus);

	void CancelAccept();

	void Connect(TInetAddr& aAddr, TRequestStatus& aStatus);

	void CancelConnect();

	void RecvOneOrMore(TDes8& aBuffer, TRequestStatus& aStatus);

	void CancelRecv();

	void Send(const TDesC8& aBuffer, TRequestStatus& aStatus);

	void CancelSend();

	void Shutdown(TRequestStatus& aStatus);
	
	void ShutdownImmediate();

	void RemoteName(TInetAddr& aAddr);

	void LocalName(TInetAddr& aAddr);

	const CX509Certificate* ServerCert();
	
	TInt CipherSuite(TDes8& aCipherSuite);

	void UpgradeToSecureL(TRequestStatus& aStatus, const TDesC8& aHostName,const TDesC& aProtocol);

	void CancelUpgradeToSecure();

	TInt PendingBytesToRead ();
	
	void SetTCPCorking(TBool aValue);

private:	// methods

	CSocket(MCommsInfoProvider& aCommsInfoProvider);

	void ConstructL(TSocketType aSocketType);
	
	TInt Construct(TSocketType aSocketType);
private:

/**	The comms info provider
*/
	MCommsInfoProvider&		iCommsInfoProvider;

/**	The socket.
*/
	RSocket					iSocket;

/**	The number of bytes read.
*/
	TSockXfrLength			iBytesReceived;

/** Controller for secure sockets, only created if a secure socket is required
*/
	CHttpSecureSocket* iSecureSocketController;
	};

#endif	// __CSOCKET_H__
