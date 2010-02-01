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

#ifndef __THTTPTRLAYERPANIC_H__
#define __THTTPTRLAYERPANIC_H__

#include <e32std.h>

class THttpTrLayerPanic
/**	
The THttpTrLayerPanic class provides the panic function and codes for the 
http transport layer.
*/
	{
public:	// enums

/**	
	Panic codes for the http transport handler.
*/
	enum THttpTrLayerPanicCode
		{
		/** An input stream API was used when it is in the wrong state.
		*/
		EBadInputStreamState			= 0,
		/** An output stream API was used when it is in the wrong state.
		*/
		EBadOutputStreamState			= 1,
		/** The input stream state machine has broken.
		*/
		EBadInputStreamError			= 2,
		/** The output stream state machine has broken.
		*/
		EBadOutputStreamError			= 3,
		/** The socket listener is in an unexpected state.
		*/
		EBadSocketListenerState			= 4,
		/** The input stream was used before an observer bound to it.
		*/
		EInputStreamNotBound			= 5,
		/** The output stream was used before an observer bound to it.
		*/
		EOutputStreamNotBound			= 6,
		/** The socket connector is in an unexpected state.
		*/
		EBadSocketConnectorState		= 7,
		/** A NULL pointer to a connection object has been given.
		*/
		EExpectedConnectionNotSupplied	= 8,
		/** A secure socket was used before starting the secure handshake.
		*/
		ETlsSocketNotStarted			= 9,
		/** The input stream was suspended when it already was.
		*/
		EInputStreamAlreadySuspended	= 10,
		/** The input stream was resumed when it was not suspended.
		*/
		EInputStreamNotSuspended		= 11
		};

public:	// methods

	static void Panic(THttpTrLayerPanicCode aPanic);

	};

#endif	// __THTTPTRLAYERPANIC_H__
