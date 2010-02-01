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
 @file MWspCOSessionInvoker.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCOSESSIONINVOKER_H__
#define __MWSPCOSESSIONINVOKER_H__

// System includes
#include <e32std.h>
#include <wsp/wsptypes.h>

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;


class MWspCOSessionInvoker
/** 
@class MWspCOSessionInvoker
@since			7.0
The MWspCOSessionInvoker and MWspCOSessionCallback form the API that provides
the WSP Connection-Orientated Session Service Session Management, Session
Resume and Exception Handling facilities, as described by the WSP 
Specification, July 2001. 

The MWspCOSessionInvoker interface is supplied by the Provider entity. The 
MWspCOSessionCallback interface is supplied by the Client entity. The Provider
implements the communications necessary to send the primitives issued by the 
Client to the Server, and from the Server to the Client.

The APIs that provide connection information, session headers and capabilities
are supplied to the Provider from the Client during configuration. The Provider
uses these APIs to return any Server session headers and the negotiated 
capabilities from the Server

The use of the Session Resume facility during the lifetime of a WSP session is
dependent on this being a successfully negotiated capability for that session.
@publishedAll
@deprecated
*/
	{
public:	// Methods

/**
	@fn				ConnectReq() =0
	Intended Usage	:	This represents the S-Connect.req primitive. The Provider
						initiates WSP session establishment with the Server. The 
						expected primitives to be issued from the Provider are 
						S-Connect.cnf or S-Disconnect.ind.
	@since			7.0
	@pre 			The WSP Session is in the Null state.
	@post			The Provider has initiated a connection with the Server (a 
					WAP Gateway). The WSP Session is in the Connecting state.
 */
	virtual void ConnectReq() =0;

/**
	@fn				DisconnectReq(TWspReason aReasonCode) =0
	Intended Usage	:	This represents the S-Disconnect.req primitive.The Client
						requests that the WSP session be disconnected. The session
						is not disconnected until the Provider has given the 
						indication for this primitive, S-Disconnect.ind. Before 
						the disconnect indication, the Provider must have aborted
						any outstanding method and push transactions. The Client 
						will not issue any further primitives.
	@since			7.0
	@param			aReasonCode	A parameter that indicates the reason for the 
								disconnection. The value maps to the 'WSP Reason' type 
								defined in the WSP Specification, July 2001.
	@pre 			The WSP session is in any state other than Null or Closing.
	@post			The Provider is disconnecting the session. The WSP session 
					is in the Closing state.
 */
	virtual void DisconnectReq(TWspReason aReason) =0;

/**
	@fn				SuspendReq() =0
	Intended Usage	:	This represents the S-Suspend.req primitive. The Client 
						requests that the WSP session be suspended. This function
						can only called if the Session Resume facility has been
						successfully negotiated for this WSP session. The session
						is not suspended until the Provider	has given the 
						indication to this primitive, S-Suspend.ind. Before the 
						suspend indication, the Provider must have aborted any
						outstanding method and push transactions. The Client 
						will only issue the S-Resume.req and S-Disconnect.req
						primitives.
	@since			7.0
	@pre 			The Session Resume facitlity has been successfully negotiated
					for this WSP session. The WSP session is either in the 
					Connected or Resuming state.
	@post			The Provider has requested that the server suspend the WSP 
					session. The WSP session is in the Suspending state.
 */
	virtual void SuspendReq() =0;

/**
	@fn				ResumeReq() =0
	Intended Usage	:	This represents the S-Resume.req primitive. The Client 
						requests that the WSP session be resumed. This function
						can only called if the Session Resume facility has been
						successfully negotiated for this WSP session.If the 
						session is in the Connected state, this primitive can be
						used to update the Session Headers used in the WSP 
						session.
	@since			7.0
	@pre 			The Session Resume facitlity has been successfully negotiated
					for this WSP session. The WSP session is in either the Connected, 
					Suspending or Suspended state.
	@post			The Provider has requested that the Server resume the WSP 
					session. The WSP session is in the Resuming state.
 */
	virtual void ResumeReq() =0;

	};

#endif	// __MWSPCOSESSIONINVOKER_H__
