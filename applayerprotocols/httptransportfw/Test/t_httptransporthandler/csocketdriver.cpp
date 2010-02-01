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

#include "csocketdriver.h"

#include "msocketdriverobserver.h"
#include "ctestsocketreader.h"
#include "ctestsocketwriter.h"
#include "thttptrhndtestpanic.h"

CSocketDriver::~CSocketDriver()
/**
	Destructor.
	@componentInternal		
*/
	{
	delete iReader;
	delete iWriter;
	}

CSocketDriver::CSocketDriver(MDriverObserver& aDriverObserver, MSocketDriverObserver& aObserver, TBool aSecure)
: CDriverBase(aDriverObserver), iSecure(aSecure), iObserver(aObserver)
/**	
	Constructor.
	@componentInternal		
	@param		aDriverObserver			The driver observer.
	@param		aSocketDriverObserver	The socket driver observer.
	@param		aSecure					Flag indicating a secure connection.
*/
	{
	}

void CSocketDriver::ConstructL(MInputStream& aInputStream, MOutputStream& aOutputStream)
/**	
	Second phase constructor.
	@componentInternal		
	@param		aInputStream	The input stream.
	@param		aOutputStream	The output stream.
*/
	{
	// Create the socket reader and writer objects
	iReader = CTestSocketReader::NewL(*this, aInputStream, iSecure);
	iWriter = CTestSocketWriter::NewL(*this, aOutputStream);

	// Get the cmds from the derived class.
	SetCmdsL();
	}

/*
 *	Methods from MReaderObserver
 */

void CSocketDriver::DataReceived()
/**	
	@see		MReaderObserver
	@componentInternal
*/
	{
	DoLog(_L("Socket Driver - data received"));

	TestExpectedCmd(EExpectDataReceived, KErrNone);
	}

void CSocketDriver::ReaderClosed(TInt aError)
/**	
	@see		MReaderObserver
	@componentInternal
*/
	{
	DoLog(_L("Socket Driver - reader closed"));

	iReaderOpen = EFalse;

	if( TestExpectedCmd(EExpectReaderClosed, aError) )
		{
		if( !iWriterOpen )
			{
			DoLog(_L("Socket Driver - socket closed"));

			// No need to wait for the writer to be closed - inform the observer
			iObserver.SocketClosed();
			}
		}
	}

/*
 *	Methods from MWriterObserver
 */

void CSocketDriver::DataSent()
/**	
	@see		MWriterObserver
	@componentInternal
*/
	{
	DoLog(_L("Socket Driver - data sent"));

	TestExpectedCmd(EExpectDataSent, KErrNone);
	}

void CSocketDriver::WriterClosed(TInt aError)
/**	
	@see		MWriterObserver
	@componentInternal
*/
	{
	DoLog(_L("Socket Driver - writer closed"));

	iWriterOpen = EFalse;

	if( TestExpectedCmd(EExpectWriterClosed, aError) )
		{
		if( !iReaderOpen )
			{
			DoLog(_L("Socket Driver - socket closed"));

			// No need to wait for the reader to be closed - inform the observer
			iObserver.SocketClosed();
			}
		}
	}

/*
 *	Methods from CDriverBase
 */

void CSocketDriver::RunCmdL(TInt aCmd)
/**
	@see		CDriverBase
	@componentInternal
*/
	{
	switch( aCmd )
		{
	case EDoSetSecure:
		{
		iWriter->SetSecure(ETrue);
		} break;
	case EDoExpectData:
		{
		DoLog(_L("Socket Driver - expect receive data"));

		iReader->ExpectData(ReceiveData());
		iReaderOpen = ETrue;
		} break;
	case EDoSendData:
		{
		DoLog(_L("Socket Driver - send data"));

		iWriter->SendData(SendData());
		iWriterOpen = ETrue;
		} break;
	case EDoWriterClose:
		{
		DoLog(_L("Socket Driver - close writer"));

		iWriter->CloseWriter();
		} break;
	case EDoReaderClose:
		{
		DoLog(_L("Socket Driver - close reader"));

		iReader->CloseReader();
		} break;
	default:
		__ASSERT_DEBUG( 0, THttpTrHndTestPanic::Panic(THttpTrHndTestPanic::EBadCommandId) );
		break;
		}
	}
