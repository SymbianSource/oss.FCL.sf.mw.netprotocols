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

#ifndef __CTESTSOCKETREADER_H__
#define __CTESTSOCKETREADER_H__

#include <e32base.h>
#include <minputstreamobserver.h>

class MInputStream;
class MTestReaderObserver;

/**	@class		CTestSocketReader
	The CTestSocketReader class encapsulates the input stream of a connected 
	socket. The state machine is initially in the Idle state. When data is 
	expected to be received, the observer passes the expected data using the 
	ExpectData() API. The descriptor buffer passed in must remain valid until 
	the test socket reader termibates.

	The ExpectData() API binds the test socket reader to the input stream. The
	state changes to PendingReceivedData and the test socket reader waits for 
	the input strean to notify it that data has been received.

	The input stream uses the MInputStreamObserver::ReceivedDataInd() API to 
	notify the test socket reader that data has been received. The test socket
	reader verifies that the received data is part of the expected data. If so
	that part is removed from the expected data. The test socket reader moves 
	to the ReceiveAck state and self-completes. If the received data was not 
	part of the expected data or if there was extra data in the received buffer
	then a panic occurs.

	In the ReceiveAck state the test socket reader notifies the input stream 
	that it has finished with the received buffer. If all the expected data has
	been received then the test socket reader moves to the Idle state. If more 
	data is expected the test socket reader moves to the PendingReceivedData.

	The test socket reader observer can request that the input stream be closed
	using the CloseReader() API. The test socket reader must be in the Idle 
	state or else there will be a panic. The state changes to Closing and the 
	test socket reader self-completes.

	In the Closing state the test socket reader requests that the input stream
	close asynchronously. The test socket reader moves to the PendingClosed 
	state and waits to be notified that the input stream is closed.
	
	Once the stream has closed, the test socket reader is notified via the 
	MInputStreamObserver::InputStreamCloseInd() API. The test socket reader 
	moves to the Closed state.

	If the input stream is closed, either because the corresponding output 
	stream has been closed, the remote host has closed the socket or there has
	been a socket error, the test socket reader is notified via the 
	MInputStreamObserver::InputStreamCloseInd() API.
	@componentInternal		
*/
class CTestSocketReader : public CActive,
						  public MInputStreamObserver
	{
public:	// methods

	static CTestSocketReader* NewL(MTestReaderObserver& aObserver, MInputStream& aInputStream, TBool aSecure);

	virtual ~CTestSocketReader();

	void ExpectData(const TDesC8& aData);

	void CloseReader();

private:	// methods from MInputStreamObserver

	virtual void ReceivedDataIndL(const TDesC8& aBuffer);
	
	virtual void InputStreamCloseInd(TInt aError);

	virtual void SecureServerCnf();

	virtual void MInputStreamObserver_Reserved();
	
	MHttpResponse* CurrentResponse();
    
    virtual void OnReceiveTimeOut(); 
	
private:	// methods from CActive

	virtual void RunL();

	virtual void DoCancel();

	virtual TInt RunError(TInt aError);

private:	// methods

	CTestSocketReader(MTestReaderObserver& aObserver, MInputStream& aInputStream, TBool aSecure);

	TInt UpdateData(const TDesC8& aReceivedData, TPtrC8& aExcessData);

	TInt CheckResponseStatusL(const TDesC8& aReceivedData);

	void CompleteSelf();

private:	// enums

/**	@enum		TReaderState
	The test socket reader state machine.
	@componentInternal		
*/
	enum TReaderState
		{
		/**	The test socket reader is idle.
		*/
		EIdle					=0,
		/**	The test socket reader is expecting to receive more data.
		*/
		EPendingReceivedData,
		/**	The test socket reader needs to acknowledge the received data packet.
		*/
		EReceiveAck,
		/** The test socket reader observer has requested that the test socket reader 
			close the input stream.
		*/
		EClosing,
		/**	The test socket reader is waiting for the input stream to notify it that
			it has closed.
		*/
		EPendingClosed,
		/**	The test socket reader is closed - no more data can be received.
		*/
		EClosed
		};

private:	// attributes

	MTestReaderObserver&	iObserver;
	MInputStream*			iInputStream;
	TReaderState			iState;
	TBool					iBoundToInputStream;
	TPtrC8					iExpectedData;
	TPtrC8					iReceivedBuffer;
	TBool					iSecure;
	TBool					iSecureTestPassed;
	};

#endif	// __CTESTSOCKETREADER_H__
