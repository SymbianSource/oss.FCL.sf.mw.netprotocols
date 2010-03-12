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

#ifndef __CSECURESOCKETCONTROLLER_H__
#define __CSECURESOCKETCONTROLLER_H__

#include <e32base.h>

#include "chttpsecuresocket.h"

class CSecureSocket;

class CSecureSocketController : public CHttpSecureSocket
/**
	This controller class manages secure socket connections for the provided 
	socket. This class handles the connecting and handshaking of the secure 
	socket and allows the clients to continue using the secure socket as they 
	would a normal socket.
	@internalComponent
*/
	{
public:	// methods

	static CSecureSocketController* NewL(TAny* aInitParams);

	virtual ~CSecureSocketController();

	void StartSecureHandshakeL(TRequestStatus& aStatus, const TDesC8& aHostName,const  TDesC& aProtocolVersion);

	void RecvOneOrMore(TDes8& aBuffer, TRequestStatus& aStatus, TSockXfrLength& aLength);

	void CancelRecv();

	void Send(const TDesC8& aBuffer, TRequestStatus& aStatus);

	void CancelSend();

	const CX509Certificate* ServerCert();

	void CancelHandshake();
	
	TInt CipherSuite(TDes8& aCipherSuite);

    TInt PendingBytesToRead ();

private:	// methods

	CSecureSocketController(RSocket& aSocket, MCommsInfoProvider& aCommsInfoProvider);


private:	// attributes

/** The socket that requires a secure connection, should already be opened and
	takes ownership.
*/
	RSocket& iSocket;
	
/** Reference to the Comms infomation provider for security preferences.
*/
	MCommsInfoProvider& iCommsInfoProvider;

/** The secure socket layer. Created when once a connection has been made.
*/
	CSecureSocket* iTlsSocket;

	};

#endif	// __CSECURESOCKETCONTROLLER_H__
