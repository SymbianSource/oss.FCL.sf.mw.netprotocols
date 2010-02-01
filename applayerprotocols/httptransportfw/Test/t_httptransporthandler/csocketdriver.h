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

#ifndef __CSOCKETDRIVER_H__
#define __CSOCKETDRIVER_H__

#include <e32base.h>

#include "cdriverbase.h"
#include "mtestreaderobserver.h"
#include "mtestwriterobserver.h"

class MInputStream;
class MOutputStream;
class MSocketDriverObserver;
class CTestSocketReader;
class CTestSocketWriter;

/**	@class		CSocketDriver
	The CSocketDriver class is an abstract test driver clas. It provides the 
	commands needed to drive a socket reader and a socket writer. These objects
	control the input and the output streams respectively of a connected socket.
	A derived class needs to override the pure virtual function SetCmdsL(). This
	function sets the command stack in the test driver.

	The commands offered allow expected data to be received from the input 
	stream via the socket reader. Specified data can be sent to the output 
	stream via the socket writer. Both the socket reader and writer can be closed.
	@componentInternal		
	@see		CSocketReader
	@see		CSocketWriter	
*/
class CSocketDriver : public CDriverBase,
					  public MTestReaderObserver,
					  public MTestWriterObserver
	{
public:	// methods

	virtual ~CSocketDriver();

private:	// methods from CDriverBase

	virtual void RunCmdL(TInt aCmd);

private:	// methods from MTestReaderObserver
	
	virtual void DataReceived();

	virtual void ReaderClosed(TInt aError);

private:	// methods from MTestWriterObserver

	virtual void DataSent();

	virtual void WriterClosed(TInt aError);

protected:	// methods

	CSocketDriver(
				 MDriverObserver&			aDriverObserver,
				 MSocketDriverObserver&		aObserver,
				 TBool						aSecure
				 );

	void ConstructL(MInputStream& aInputStream, MOutputStream& aOutputStream);

private:	// methods

/**	@fn			SetCmdsL() =0
	Sets the command stack.
	@componentInternal		
*/
	virtual void SetCmdsL() =0;

/**	@fn			SendData() =0
	Supplies the a descriptor buffer containing the data to be sent
	@componentInternal		
*/
	virtual const TDesC8& SendData() =0;

/**	@fn
	@componentInternal		
*/
	virtual const TDesC8& ReceiveData() =0;

protected:	// enum

/**	Enumeration of the command IDs for the socket driver.
	@componentInternal
*/
	enum
		{
		/** 'Do' command that initiates the socket reader to expect some specified
			data.
		*/
		EDoExpectData			=0,
		/**	'Expect' command indicating that the specified data has been received.
		*/
		EExpectDataReceived,
		/**	'Do' command that initiates the socket writer to send the specified
			data.
		*/
		EDoSendData,
		/** 'Expect' command indicating that the specified data has been sent.
		*/
		EExpectDataSent,
		/**	'Do' command that closes the socket reader.
		*/
		EDoReaderClose,
		/** 'Do' command that closes the socket writer.
		*/
		EDoWriterClose,
		/**	'Expect' command that indicating that the socket reader has been closed.
		*/
		EExpectReaderClosed,
		/**	'Expect' command that indicating that the socket writer has been closed.
		*/
		EExpectWriterClosed,
		EDoSetSecure
		};

protected:
	/** Flag to indicate that the connection is a secure one
	*/
	TBool					iSecure;

private:	// attributes

	MSocketDriverObserver&		iObserver;
	CTestSocketReader*			iReader;
	CTestSocketWriter*			iWriter;
	TBool						iReaderOpen;
	TBool						iWriterOpen;

	};

#endif	// __CSOCKETDRIVER_H__
