// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CTESTSOCKETWRITER_H__
#define __CTESTSOCKETWRITER_H__

#include <e32base.h>
#include <moutputstreamobserver.h>

const TInt KDefaultBuffer =	100;

class MOutputStream;
class MTestWriterObserver;

/**	@class		CTestSocketWriter
	The CTestSocketWriter class encapsulates the output stream of a connected
	socket. The state machine is initially in the Idle state. When the observer
	requests that data should be sent (using the SendData() API), the test 
	socket writer binds to the output stream. The state changes the SendData 
	state and the object self-completes. Note the descriptor buffer containing
	the data to be sent must remain valid until the socket writer terminates.

	When in the SendData the test socket writer does a send request. The data to
	send is placed in the send packet and the sedn data is updated, removing the
	data sent in the current buffer.
	
	The test socket writer moves into the PendingAck state and waits for the 
	output stream to notify the it that the data has been sent. This is done via
	the MOutputStreamObserver::SendDataCnf() API. 
	
	When the test socket writer is notified that the data has been sent it checks
	to see if more data needs to be sent. If there is then the test socket writer 
	moves to the SendData state and self-completes. If there is no more data to 
	send the test socket writer moves to the Idle state.
	
	The test socket writer observer can request that the output stream be closed 
	using the CloseWriter() API. The test socket writer must be in the Idle state
	or else there will be a panic. The state changes to Closing and the test 
	socket writer self-completes.

	In the Closing state the test socket writer requests that the output stream
	close asynchronously. The test socket writer moves to the PendingClosed 
	state and waits to be notified that the output stream is closed.
	
	Once the stream has closed, the test socket writer is notified via the 
	MOutputStreamObserver::OutputStreamCloseInd() API. The test socket writer 
	moves to the Closed state.

	If the output stream is closed, either because the corresponding input stream
	has been closed, the remote host has closed the socket or there has been a 
	socket error, the socket writer is notified via the 
	MOutputStreamObserver::OutputStreamCloseInd() API.
	@componentInternal		
*/
class CTestSocketWriter : public CActive,
						  public MOutputStreamObserver
	{
public:	// methods

	static CTestSocketWriter* NewL(MTestWriterObserver& aObserver, MOutputStream& aOutputStream);

	virtual ~CTestSocketWriter();

	void SendData(const TDesC8& aData);

	void SetSecure(TBool aSecure);

	void CloseWriter();

private:	// methods from MOutputStreamObserver

	virtual void SendDataCnfL();

	virtual void OutputStreamCloseInd(TInt aError);

	virtual void SecureClientCnf();

	virtual TBool SecureRetry(){ return EFalse;}

	virtual void MOutputStreamObserver_Reserved();
    
	virtual void OnSendTimeOut();
 
	virtual TInt SendTimeOutVal();

private:	// methods from CActive

	virtual void RunL();

	virtual void DoCancel();

	virtual TInt RunError(TInt aError);

private:	// methods

	CTestSocketWriter(MTestWriterObserver& aObserver, MOutputStream& aOutputStream);

	void CompleteSelf();

private:	// enums

/**	@enum		TWriterState
	The state machine for the socket writer.
	@componentInternal		
*/
	enum TWriterState
		{
		/**	The socket writer is in idle.
		*/
		EIdle					=0,
		/** The socket writer needs to send the next data packet.
		*/
		ESendData,
		/**	The socket writer is waiting for the output stream to notify it that
			the data packet has been sent.
		*/
		EPendingAck,
		/**	The socket writer observer has requested that the socket writer close
			the output stream.
		*/
		EClosing,
		/**	The socket writer is waiting for the output stream to notify it that
			it has closed.
		*/
		EPendingClosed,
		/**	The socket writer is closed - no more data can be sent.
		*/
		EClosed,

		ESecureHandshake
		};

private:	// attributes

	MTestWriterObserver&	iObserver;
	MOutputStream*			iOutputStream;
	TWriterState			iState;
	TBool					iBoundToOutputStream;
	TPtrC8					iSendData;
	TBuf8<KDefaultBuffer>	iSendBuffer;
	TBool					iSecure;
	};

#endif	// __CTESTSOCKETWRITER_H__
