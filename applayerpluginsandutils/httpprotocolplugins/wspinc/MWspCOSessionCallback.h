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
 @file MWspCOSessionCallback.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCOSESSIONCALLBACK_H__
#define __MWSPCOSESSIONCALLBACK_H__

// System includes
#include <e32std.h>
#include <wsp/wsptypes.h>

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;


class MWspCOSessionCallback
/**
@class MWspCOSessionCallback
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
	@fn				ConnectCnf() =0
	Intended Usage	:	This represents the S-Connect.cnf primitive. The Provider 
						issues this primitive when a WSP session has been 
						successfully established. Any Session Headers sent by the 
						Server are passed to the Client.
	@since			7.0
	@pre 			The WSP session is in the Connecting state.
	@post			The WSP session is established with the Server. The WSP 
					session is in the Connected state.
 */
	virtual void ConnectCnf() =0;

/**
	@fn				ResumeCnf() =0
	Intended Usage	:	This represents the S-Resume.cnf primitive. The Provider
						issues this primitive when a WSP session has been
						successfully resumed. This primitive can only be issued
						if the Session Resume facility has been successfully 
						negotiated. Any Session Headers sent by the Server are
						passed to the Client.
	@since			7.0
	@pre 			The Session Resume facitlity has been successfully negotiated
					for this WSP session. The WSP session is in the Resuming state.
	@post			The WSP session is in the Connected state.
 */
	virtual void ResumeCnf() =0;

/**
	@fn				DisconnectInd(TWspReason aReason, TBool aRedirectSecurity, TWspRedirectedAddress& aRedirectAddress, const TDesC8& aErrorHeaders, const TDesC8& aErrorBody) =0
	Intended Usage	:	This represents the S-Disconnect.ind primitive. The	
						Provider issues this primitive when the WSP session has
						been disconnected. Any incomplete method or push 
						transactions have been aborted. If the aReasonCode 
						indicates that the redirection is temporary, then the
						original address should be used in future connection
						attempts. If the redirection is permanent, then the Client
						should use the new address in all future connection 
						attempts.
	@since			7.0
	@param			aReason				A parameter that indicates the cause of 
										the disconnection. The value maps to the
										Reason type defined in the WSP 
										Specification, July 2001.
	@param			aRedirectSecurity	A flag to indicate that the current secure
										session may be reused with the redirected 
										server address.
	@param			aRedirectAddress	An object containing an alternative address
										that must be used to establish a WSP session 
										with the same service.
	@param			aErrorHeaders		Together with aErrorBody, this provides some
										meaningful information as to why the session
										was disconnected.
	@param			aErrorBody			See explanation of aErrorHeader.
	@pre 			The WSP session is in any state other than Null.
	@post			The WSP session is in the Null state.
 */
	virtual void DisconnectInd(
							  TWspReason				aReason, 
							  TBool						aRedirectSecurity, 
							  TWspRedirectedAddress&	aRedirectAddress, 
							  const TDesC8&				aErrorHeader, 
							  const TDesC8&				aErrorBody
							  ) =0;

/**
	@fn				SuspendInd(TWspReason aReason) =0
	Intended Usage	:	This represents the S-Suspend.ind primitive. The Provider
						issues this primitive when a WSP session has been 
						suspended. This primitive is only issued if the Session
						Resume facility has been successfully negotiated. 
	@since			7.0
	@param			aReason	A parameter that indicates the cause of the suspension.
							The value maps to the Reason type defined in the WSP 
							Specification, July 2001.
	@pre 			The Session Resume facitlity has been successfully negotiated
					for this WSP session. The WSP session is in either the Connected,
					Suspending or Resuming state.
	@post			The WSP session is in the Suspended state.
 */
	virtual void SuspendInd(TWspReason aReason) =0;

/**
	@fn				ExceptionInd(const TDesC8& aExceptionData) =0
	Intended Usage	:	This represents the S-Exception.ind primitive. The 
						Provider issues this when it receives an event from the
						Server that is not related to a transaction nor changes 
						the state of the WSP session.
	@since			7.0
	@param			aExceptionData	A buffer containing the information sent by 
									the Server.
	@pre 			The WSP session is not in the Null or Suspended state.
	@post			Unspecified
 */
	virtual void ExceptionInd(const TDesC8& aExceptionData) =0;

	};


#endif	// __MWSPCOSESSIONCALLBACK_H__
