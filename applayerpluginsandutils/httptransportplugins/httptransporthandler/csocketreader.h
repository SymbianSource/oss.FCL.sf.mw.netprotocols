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

#ifndef __CSOCKETREADER_H__
#define __CSOCKETREADER_H__

#include <e32base.h>
#include <http/framework/logging.h>

#include "minputstream.h"
#include "mhttptimerobserver.h"
#include "chttptimer.h"

class CSocket;
class MInputStreamObserver;
class MSocketController;

class CSocketReader : public CActive,
					  public MInputStream,
					  public MHttpTimerObserver
/**	                                         
	The CSocketReader class encapsulates the reading functionality and behaviour
	for a connected socket. It implements the MInputStream API.
	@see		MInputStream
	@internalComponent
*/
	{
public:	// methods

	static CSocketReader* NewL(CSocket& aSocket, MSocketController& aController, TInt aRecvBufferSize, TBool aPriority);

	virtual ~CSocketReader();

	void SocketClosed(TInt aError);

	void Suspend();

	void Resume();

private:	// methods from MInputStream

	virtual void Bind(MInputStreamObserver& aObserver);

	virtual void ReceivedDataRes();

	virtual void ShutdownReq();

	virtual void SecureServerReq();

	virtual void Close();

	virtual const CX509Certificate* ClientCert();

	virtual void Shutdown();
	
	virtual void Reset();
	
	virtual TInt ImmediateRead ( TPtrC8& aData );
	
	virtual void Restart ();
	
	virtual void StartReceieveTimer (TInt aTimeoutValue);
	
private:	// methods from CActive

	virtual void RunL();
	
	virtual void DoCancel();
	
	virtual TInt RunError(TInt aError);

private:	// methods from MHttpTimerObserver
	
	virtual void TimeOut();

private:	// methods

	CSocketReader(CSocket& aSocket, MSocketController& aController, TBool aPriority);

	void CompleteSelf();
	
	void ConstructL( TInt aRecvBufferSize );

private:	// enums

/**	
	The state machine for the input stream.
*/
	enum TInputState
		{
		/** The input stream is waiting for an observer to bind itself to it 
			before doing a read. 
		*/
		EIdle			= 0,
		/** The input stream has an observer and it requested a read from the socket.
		*/
		ERead,
		/**	The input stream has received a buffer of data from the socket.
		*/
		EReceivedData,
		/**	The input stream is waiting for the observer to notify it that it 
			has finished with the current data.
		*/
		EPendingAck,
		/**	The observer has asked the input stream to close the socket.
		*/
		EClosing,
		/**	The socket has been closed - data can no longer be received from it.
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

/** The buffer into which the received data is placed.
*/
	RBuf8						iBuffer;

/**	The state of the input stream.
*/
	TInputState					iState;

/**	Flag to indicate that the stream is suspended.
*/
	TBool						iSuspended;

/** The observer for the input stream.
*/
	MInputStreamObserver*		iObserver;
	
//CHttpTimer Object
	CHttpTimer*					iReceiveTimer;

public:	// attributes

/** Logger handle
*/
	__FLOG_DECLARATION_MEMBER2

	};

#endif	// __CSOCKETREADER_H__
