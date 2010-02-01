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
// $Header$
// mjd, mark@mjdss.com, july 2002
// During implementation found a 'known' bug in the MSVC++ v6.0 compiler.
// The C2955 bug. 
// 
//

#ifndef __CMDIDS_H__
#define __CMDIDS_H__

//-----------------------------------------------------------------------------

#include "GT0149.h"

//-----------------------------------------------------------------------------

enum TCommandIDs
	{
	// Trace and log mode commands
	ECmdTrace = THA_KMaxCmdCustomized,
	ECmdLog,
	ECmdVer,

	// CASE and SELECT commands
	ECase,
	EEndCase,
	ECmdSelect,

/*
	//	HTTP DO and WAIT commands
	EDoHTTPSession,
	EDoHTTPClose,
	EDoHTTPConnect,
	EDoHTTPDisconnect,
	EDoHTTPMethod,
	EDoHTTPMethodAbort,
	EDoHTTPMethodClose,
	EWaitHTTPEvents,
	//EDoHTTPSConnect,
	//EDoHTTPSSetKeyExchange,

	// WAP DO and WAIT commands
	EDoSession,
	EDoClose,
	EDoConnect,
	EDoDisconnect,
	EDoMethodInvoke,
	EDoMethodAbort,
	EDoMethodClose,
	EWaitEvents,
	EDoWtlsConnect,
	EDoWtlsSetKeyExchange,


	// SET parameter commands
	ESetFramework,						//	switch to another framework (HTTP, WSP etc)
	ESetRemoteHost,
	ESetRemotePort,
	ESetLocalPort,
	ESetBearer,
	ESetSecure,
	ESetSessionType,
	ESetClientHeaders,
	ESetCapabilities,
	ESetMethod,
	ESetURI,
	ESetHeaders,
	ESetBody,
	ESetAckHeaders,
	ESetKeySuites,
	ESetCipherSuites,
	ESetSequenceNumberMode,
	ESetKeyRefreshRate,
	ESetSharedSecret,
	ESetRecordLengthUsage,
	ESetTwoPhase,
	ESetSendAddress,
	ESetSendPort,
	ESetSendBearer,
	ESetData,

	// SET configuration commands
	ESetHeadersSize,
	ESetBodySize,
	ESetMaxBodyLogged,
	ESetMinBodyDumped,
	ESetWaitTimeout,
	ESetHoldWhenNowait
*/
	};
	
#endif  // __CMDIDS_H__  
    
// End of File
