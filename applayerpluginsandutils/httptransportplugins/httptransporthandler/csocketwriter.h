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

#ifndef __CSOCKETWRITER_H__
#define __CSOCKETWRITER_H__

#include <e32base.h>
#include <http/framework/logging.h>

#include "moutputstream.h"
#include "mhttptimerobserver.h"
#include "chttptimer.h"

class CSocket;
class MOutputSocketObserver;
class MSocketController;

class CSocketWriter : public CActive,
					  public MOutputStream,
					  public MHttpTimerObserver
/**	
	The CSocketWriter class encapsulates the writing functionality and behaviour
	for a connected socket. It implements the MOutputStream API.
	@see		MOutputStream
	@internalComponent
*/
	{
public:	// methods

	static CSocketWriter* NewL(CSocket& aSocket, MSocketController& aController, TBool aPriority);

	virtual ~CSocketWriter();

	void SocketClosed(TInt aError);

	virtual void SecureClientReq(const TDesC8& aHostName);

private:	// methods from MOutputStream

	virtual void Bind(MOutputStreamObserver& aObserver);

	virtual void SendDataReqL(const TDesC8& aBuffer);

	virtual void ShutdownReq();

	virtual void Close();

	virtual const CX509Certificate* ServerCert();
	
	virtual TInt CipherSuite(TDes8& aCipherSuite);

	virtual void MOutputStream_Reserved();

	virtual void Reset();
	
	virtual void SetTCPCorking(TBool aValue);
	
	void CompleteSelf();

private:	// methods from CActive


	virtual void RunL();
	
	virtual void DoCancel();
	
	virtual TInt RunError(TInt aError);

private:	// methods from MHttpTimerObserver
	
	virtual void TimeOut();

private:	// methods

	CSocketWriter(CSocket& aSocket, MSocketController& aController, TBool aPriority);

private:	// enums

/**	
	The state machine for the output stream.
	@internalComponent		
*/
	enum TOutputState
		{
		/** The output stream is waiting for an observer to bind itself to it.
		*/
		EIdle			= 0,
		/** The output stream is ready to send data to the socket. It is waiting
			for the observer to request a data send.
		*/
		EPendingSend,
		/** Data has been sent to the socket. The output stream is waiting for 
			the socket to notify it that the write was successful.
		*/
		ESentData,
		/**	The observer has requested that the connection be upgraded to secure.
			Need to start the secure handshake.
		*/
		EStartSecureHandshake,
		/** The secure handshake has completed successfully. Inform the observer
			that the connection is now secure.
		*/
		ESecureHandshakeComplete,
		/**	The observer has asked the output stream to close the socket.
		*/
		EClosing,
		/**	The socket has been closed - data can no longer be sent to it.
		*/
		EClosed
		};

private:	// attributes

/** The connected socket.
*/
	CSocket&					iSocket;

/** The socket controller that owns the socket.
*/
	MSocketController&			iController;

/**	The state of the output stream.
*/
	TOutputState				iState;

/**	The observer for the output stream.
*/
	MOutputStreamObserver*		iObserver;
 
//CHttpTimer Object
	CHttpTimer*					iSendTimer;

	TPtrC8						iHostName;

public:	// attributes

/** Logger handle
*/
	__FLOG_DECLARATION_MEMBER2

	};

#endif	// __CSOCKETWRITER_H__
