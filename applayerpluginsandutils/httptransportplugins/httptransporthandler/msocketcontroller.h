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

#ifndef __MSOCKETCONTROLLER_H__
#define __MSOCKETCONTROLLER_H__

#include <e32std.h>

class MSocketController
/**	
	The observer API for the socket reader and writer. Is used by the socket 
	reader or writer to notify the socket controller of when either the input or
	output stream has been closed. It also provides name and port info for the
	remote host.
	@internalComponent
	@see		CInputStream
*/
	{
public:	// enums

/**	
	The TStremaType enumerates the types of streams.
*/
	enum TStreamType
		{
		/**	The input stream.
		*/
		EInputStream		= 0,
		/**	The output stream.
		*/
		EOutputStream
		};	

public:	// methods

/**	
	This notifies the socket controller that either the input or the output 
	stream is closed. That stream is no longer valid.
	@param		aError		The error code explaining why the stream has closed.
							A value of KErrNone indicates that the stream 
							observer requested that the stream be closed.
	@param		aStreamType	The stream that has been closed.
*/
	virtual void StreamClosed(TInt aError, MSocketController::TStreamType aStreamType) =0;

/**	
	This notifies the socket controller that either the input or the output 
	stream is suspended. The socket controller should suspend activity on the
	other stream.
	@param		aStreamType	The stream that has been suspended.
*/
	virtual void StreamSuspend(MSocketController::TStreamType aStreamType) =0;

/**	
	This notifies the socket controller that either the input or the output 
	stream has resumed. The socket controller should resume activity on the
	other stream.
	@param		aStreamType	The stream that has resumed.
*/
	virtual void  StreamResume(MSocketController::TStreamType aStreamType) =0;

/**
	Provides name and port info for the remote host and the local port that the 
	connection is established on. This functionality is only available in debug 
	builds and therefore this function should only be used in debug builds.
	@param		aRemoteHost	The output buffer to hold the remote host name.
	@param		aRemotePort	The output argument for the remote host port.
	@param		aLocalPort	The output argument for the local port.
	@panic		EInvariantFalse	The function has been called in a release build.
*/
	virtual void ConnectionInfo(TDes8& aRemoteHost, TUint16& aRemotePort, TUint16& aLocalPort) =0;

	};

#endif	// __MSOCKETCONTROLLER_H__

