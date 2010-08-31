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

// User includes
#include "csecuresocketcontroller.h"
#include "thttptrlayerpanic.h"
#include <x509certext.h>
#include <securesocket.h>
#include <ssl_internal.h>


CSecureSocketController* CSecureSocketController::NewL(TAny* aInitParams)
/**
	Factory constructor.
	@param		aSocket 			The socket that requires a secure connection,
									takes ownership.
	@param		aCommsInfoProvider	The Comms info provider for accessing client 
									security preferences.
	@return		Pointer to the newly constructed class.
*/
	{
	THttpSecureSocketParams* initParams = REINTERPRET_CAST(THttpSecureSocketParams*, aInitParams);
	CSecureSocketController* self = new (ELeave) CSecureSocketController(*(initParams->iSocket), *(initParams->iCommsInfoProvider));
	return self;
	}

CSecureSocketController::~CSecureSocketController()
/**
	Destructor.
*/
	{
	if( iTlsSocket )
		{
		iTlsSocket->Close();
		delete iTlsSocket;
		}
	else
		iSocket.Close(); // Has ownership of socket if secure layer was not created
	}

CSecureSocketController::CSecureSocketController(RSocket& aSocket, MCommsInfoProvider& aCommsInfoProvider)
: iSocket(aSocket), iCommsInfoProvider(aCommsInfoProvider)
/**
	Constructor.
	@param		aSocket 			The socket that requires a secure connection,
									takes ownership.
	@param		aCommsInfoProvider	The Comms info provider for accessing client 
									security preferences.
*/
	{
	}

void CSecureSocketController::StartSecureHandshakeL(TRequestStatus& aStatus, const TDesC8& aHostName, const  TDesC& aProtocolVersion)
/**
	Start a secure handshake to upgrade the socket to a secure connection.
	@param		aStatus		The request status, this will complete with KErrNone
							if	the secure handshake completed succesfully.
	@param		aHostName	The server host name, used for domain name checking 
							against certificates.
*/
	{
	// Create the secure layer
	if( iTlsSocket == NULL )
		{
        iTlsSocket = CSecureSocket::NewL(iSocket, aProtocolVersion);
        }
	// Get the security preferences, dialog prompt and security policy
	TBool dialogPref = ETrue;
	MSecurityPolicy* securityPolicy = NULL;
	iCommsInfoProvider.SecurityPreferences(dialogPref, securityPolicy);

	// Dialog preferences
	if( !dialogPref )
		User::LeaveIfError(iTlsSocket->SetDialogMode(EDialogModeUnattended));

	// Security policy preferences
	if( securityPolicy )
		{
		TPtrC8 ciphers = securityPolicy->GetTlsCipherSuites();
		if (ciphers.Length() >0)
			User::LeaveIfError(iTlsSocket->SetAvailableCipherSuites(ciphers));
		}

	// Set an option on the socket to not use SSL2
	User::LeaveIfError(iTlsSocket->SetOpt(KSoUseSSLv2Handshake, KSolInetSSL, KNullDesC8()));

	// Set an option on the socket to check the server certificate domain
	User::LeaveIfError(iTlsSocket->SetOpt(KSoSSLDomainName, KSolInetSSL, aHostName));
	
	iTlsSocket->StartClientHandshake(aStatus);
	}

void CSecureSocketController::RecvOneOrMore(TDes8& aBuffer, TRequestStatus& aStatus, TSockXfrLength& aLength)
/**
	Receives data from a connected remote host when it is available.
	@param		aBuffer The buffer for the data to be placed.
	@param		aStatus	The request status. Will complete with KErrNone when the
						data is avaiable, KErrEof indicating the the connection
						has closed and no more data will will be available or 
						any	other system error code.
	@param		aLength On return, this will contain how much data was read.
*/
	{
	__ASSERT_DEBUG( iTlsSocket!=NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::ETlsSocketNotStarted) );

	iTlsSocket->RecvOneOrMore(aBuffer, aStatus, aLength);
	}

void CSecureSocketController::CancelRecv()
/**
	Cancels an outstanding data receive call made by RecvOneOrMore().
*/
	{
	if( iTlsSocket )
		iTlsSocket->CancelRecv();
	}

void CSecureSocketController::Send(const TDesC8& aBuffer, TRequestStatus& aStatus)
/*
	Sends data to the connected remote host.
	@param		aBuffer The buffer containing the data to send to the remote host.
	@param		aStatus The request status, Will complete with KErrNone when the
						data is avaiable, KErrEof indicating the the connection
						has closed or any other system error code.
*/
	{
	__ASSERT_DEBUG( iTlsSocket!=NULL, THttpTrLayerPanic::Panic(THttpTrLayerPanic::ETlsSocketNotStarted) );
	
	iTlsSocket->Send(aBuffer, aStatus);
	}

void CSecureSocketController::CancelSend()
/**
	Cancels an outstanding send data to a remote host using Send().
*/
	{
	if( iTlsSocket )
		iTlsSocket->CancelSend();
	}

const CX509Certificate* CSecureSocketController::ServerCert()
/**
	Get the Server Certificate for this socket session.
	@return		An error code. KErrNone if aServerCert has been completed, 
				otherwise one of the system wide error codes
*/
	{
 	if( iTlsSocket ) 
		{
		return iTlsSocket->ServerCert();
 		}
	return NULL;
	}

void CSecureSocketController::CancelHandshake()
/**
	Cancel the secure handshake.
*/
	{
	if( iTlsSocket )
		iTlsSocket->CancelHandshake();
	}

TInt CSecureSocketController::CipherSuite(TDes8& aCipherSuite)
	{
	if (iTlsSocket)
		return iTlsSocket->CurrentCipherSuite(aCipherSuite);
	// else
	return KErrNotSupported;
	}

TInt CSecureSocketController::PendingBytesToRead ()
	{
	TInt bytesToRead;
	TInt err = iTlsSocket->GetOpt ( KSOReadBytesPending, KSOLSocket, bytesToRead );
	if ( err == KErrNone )
		return bytesToRead;
	return err;
	}

