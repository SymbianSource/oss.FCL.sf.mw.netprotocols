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

/**
 @file
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__RHTTPCONNECTIONINFO_H__
#define	__RHTTPCONNECTIONINFO_H__ 

// System includes 
#include <e32std.h>
#include <stringpool.h>
#include <http/rhttppropertyset.h>

//##ModelId=3C4C187A01CA
class RHTTPConnectionInfo : public RHTTPPropertySet
/**
The RHTTPConnectionInfo is used for setting/getting 
values used for setting up a connection over a protocol.  All methods are inherited
directly from RHTTPPropertySet.

To lookup/change any of the following properties, use 
RHTTPPropertySet::Property(...) and RHTTPPropertySet::SetPropertyL(...) 

To remove properties, use RHTTPPropertySet::RemoveProperty(...) for a named property,
or RHTTPPropertySet::RemoveAllProperties() for all.

The following values are currently defined. Note that new values
could be added by individual filters. In general, properties
should be defined before the first transaction is created; filters
are not obliged to check with the connection info if the values
have changed after this point.

HTTP::EProxyUsage (default HTTP::EDoNotUseProxy) (HTTP::EDoNotUseProxy | HTTP::EUseProxy)

HTTP::EProxyAddress (A Uri for the Proxy server. Should include the port number
if it is not the default of 8080)

HTTP::EMaxNumTransportHandlers (default 4)

HTTP::EMaxNumTransPerTranspHndlr (default 5)

HTTP::EHTTPVersion	(default HTTP::EHttp11) (HTTP::EHttp11 | HTTP::EHttp10)

HTTP::EProtocol (default, and currently only defined value is HTTP::EHTTP)

HTTP::EHttpBatching (default HTTP::EDoNotEnableBatching) (HTTP::EDoNotEnableBatching | HTTP::EEnableBatching)

HTTP::EBatchingBufferSize (default 1400)

HTTP Client can create a connection and set its own preferences, instead of using the 
default connection preferences specified in COMMS Database. In such cases, the client 
is responsible for creating the RSocketServ session and RConnection using
RSocketServ::Connect(..) and RConnection::Open(..). 	   
Client must also start the connection using RConnection::Start(..) and pass the 
connection preferences to the function using TConnPref.
The following session properties should then be set by the client.

HTTP::EHttpSocketServ (Handle to RSocketServ session, which is the value obtained 
                       by calling RSocketServ::Handle())

HTTP::EHttpSocketConnection (Reference to the RConnection object, obtained from 
                             reinterpret_cast(TInt, &connection))

The HTTP Client can also specify what action should be take if during a secure 
handshakes the server cert cannot be trusted. The choice is for the handshake
to automatically cancelled or for a dialog to be raised and the user given the
option of continuing with the handshake or cancelling it. The following session
property should be used to specify the behaviour.

HTTP::ESecureDialog (default HTTP::EDialogPrompt) (HTTP::EDialogPrompt | EDialogNoPrompt)

The HTTP Client can specify the socket shutdown mode when the session is closing. At the time
of closing the session, any outstanding transactions on open sockets can specify that the
socket shutdown mode be set to immediate. The following property can be used to enable this
behaviour.

HTTP::ESocketShutdownMode (default HTTP::ESocketShutdownNormal) (HTTP::ESocketShutdownNormal | HTTP::ESocketShutdownImmediate

The HTTP Client can limit the number of transactions that it will send down one connection when
using pipelining. The following value should be set to an integer. When set any outstanding transaction
that exceed this number will either be sent separately down a different connection or will
be queued until a space becomes available.

HTTP::EMaxNumTransactionsToPipeline (default KMaxTInt) (Integer)

The session ID is an optional non-negative integer which is attached to the session and any sockets 
it creates. The session ID will be set on any sockets created by using RSocket::SetOpt() with
the level KSOLHttpSessionInfo and the name KSOHttpSessionId (defined in httpsocketconstants.h).
The session ID can be retrieved in a Flow Hook by implementing MIp6Hook::SetFlowOption() to 
look for the aforementioned option level and name. 
@see MIp6Hook

HTTP::ESessionId (no default) (Integer)


@publishedAll
@released
*/
	{
	};

#endif // __RHTTPCONNECTIONINFO_H__
