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
 @file MWspCOMethodCallback.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCOMETHODCALLBACK_H__
#define __MWSPCOMETHODCALLBACK_H__

// System includes
#include <e32std.h>
#include <wsp/wsptypes.h>

// Forward class declarations
class MWspCOMethodInvoker;
class MHTTPDataSupplier;

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;


class MWspCOMethodCallback
/** 
@class MWspCOMethodCallback
@since			7.0
The MWspCOMethodInvoker and MWspCOMethodCallback form the API that provides
the WSP Connection-Orientated Session Service Method Invocation facility, as
described in the WSP Specification, July 2001.

The MWspCOMethodInvoker interface is supplied by the Provider entity. The 
MWspCOMethodCallback interface is supplied by the Client entity. The Provider
implements the communications necessary to send the primitives issued by the
Client to the Server, and from the Server to the Client.

A method transaction can only be invoked under the following circumstances;
	1)	The WSP session is in either the Connecting, Connected or Resuming
		state.
	2)	The number of of outstanding method transactions has not reached the
		negotiated Method MOR.
Both these circumstances must be statisfied before a method may be invoked.

Further, during the capability negotiations, the Protocol Option for Large 
Data Transfer may be refused (WSP Specification 8.3.2.3). If this is the case,
then the use of the S-MethodInvokeData (by the Client) and S-MethodResultData
(by the Server) is forbidden.

The effect of not supporting LDT is that POST data may not be streamed and 
must not overflow the negotiated Server SDU size. The amount of allowed data
in the S-MethodInvoke.req is dependent on the size of the destination URI 
and the method headers. A method transaction that exceeds the Server SDU size
is aborted by the Client and is not invoked on the Provider.

Another case in which the Client may abort a method transaction is when the 
negotiated Server Message size has been exceeded. This also applies when LDT 
is supported in the WSP session. The Client aborts the method invocation on
the Provider as well.

The object that encapsulates the concept of a method transaction implements 
the call-back API MWspCOMethodCallback. The Provider associates each of
these objects with a Transaction Id used to distinguish between method 
transactions during a WSP session.
@publishedAll
@deprecated
*/
	{
public:	// Methods

/** @fn				MethodInvokeCnf() =0
	Intended Usage	:	This represents the S-MethodInvoke.cnf primitive. The 
						Provider issues this primitive when it has received the
						response to the method request invocation from the Server
						(S-MethodInvoke.res). If the negotiated Server Message
						size has been exceeded by the S-MethodInvoke.ind primitive,
						then the Provider aborts the method transaction.

	@since			7.0
	@pre 			The WSP method transaction is in the Requesting state.
	@post			If there are subsequent S-MethodInvokeData.req primitives to
					be issued for this method request, then the WSP method 
					transaction remains in the Requesting state. If the entire
					request has been sent to the Server, then the WSP method
					transaction is in the Waiting state.
*/
	virtual void MethodInvokeCnf() =0;

/** @fn				MethodInvokeDataCnf() =0
	Intended Usage	:	This represents the S-MethodInvokeData.cnf primitive. 
						The Provider issues this primitive when it has received
						the response to the method request invocation from the 
						Server (S-MethodInvokeData.res).If the negotiated Server
						Message size has been exceeded, then the Provider aborts 
						the method transaction.
	@since			7.0
	@pre 			The WSP method transaction is in the Requesting state.
	@post			If there are subsequent S-MethodInvokeData.req primitives 
					to be issued for this method request, then the WSP method 
					transaction remains in the Requesting state. If the entire
					request has been sent to the Server, then the WSP method 
					transaction is in the Waiting state.
*/
	virtual void MethodInvokeDataCnf() =0;

/** @fn				MethodAbortInd(TWspReason aReason) =0
	Intended Usage	:	This represents the S-MethodAbort.ind primitive. This
						is issued when the method transaction has been aborted.
	@since			7.0
	@param			aReason	A parameter that indicates the cause of the abort.
							The value maps to the Reason type defined in the WSP
							Specification, July 2001.
	@pre 			The WSP method transaction is not in the Null state.
	@post			The WSP method transaction is in the Null state.
*/
	virtual void MethodAbortInd(TWspReason aReason) =0;

/** @fn				MethodResultInd(TInt aStatus, const TDesC8& aResponseHeaders, MHTTPDataSupplier& aResponseBody, TBool aMoreData) =0
	Intended Usage	:	This represents the S-MethodResult.ind primitive. The
						Provider issues this primitive when it has received the
						response for this method transaction from the Server.

						The aMoreData flag indicates whether there are subsequent
						S-MethodResultData primitives to follow. This is only 
						possible if LDT is supported.

						If there are subsequent S-MethodResultData primitives 
						for this transaction, then the data supplier object
						aResponseBody is used to obtain the body data in all the
						subsequent S-MethodResultData primitives.

						The Server expects the S-MethodResult.res primitive to
						be sent from the Client.
	@since			7.0
	@param			aStatus				A status code whose value is given by 
										the Status types defined in the WSP 
										Specification, July 2001 .
	@param			aResponseHeaders	A reference to buffer that provides the
										encoded response headers.
	@param			aResponseBody		A reference to a data supplier object
										that provides the data associated 
										with the response.
	@param			aMoreDate			A flag indicating that there are subsequent
										S-MethodResultData primitives to follow
										for this transaction.
	@pre 			The WSP method transaction is in the Waiting state.
	@post			If subsequent S-MethodResultData primitives are expected,
					then the WSP method transaction is in the Waiting2 state. 
					If all the response information has been received, then the
					WSP method transaction is in the Completing state.
*/
	virtual void MethodResultInd(
								TInt					aStatus, 
								const TDesC8&			aResponseHeaders, 
								MHTTPDataSupplier&		aResponseBody,
								TBool					aMoreData
								) =0;

/** @fn				MethodResultDataInd(const TDesC8& aTrailerHeaders, TBool aMoreData) =0
	Intended Usage	:	This represents the S-MethodResultData.ind primitive.
						The Provider sends more data associated with a 
						previous S-MethodResult.ind primitive.
						
						The remaining body data is obtained using the data 
						supplier object aResponseBody provided to the Client
						as a parameter of the previous call to MethodResultInd().

						The aMoreData flag indicates whether there are subsequent
						S-MethodResultData primitives to follow.

						If there are trailer headers these are provided by the
						input parameter aTrailerHeaders. This is only valid if
						this is the last S-MethodResultData primitive.

						The Server expects the S-MethodResultData.res primitive 
						to be sent from the Client.
	@since			7.0
	@param			aTrailerHeaders	A reference to a buffer with the encoded 
									trailer headers. This must be empty unless
									this is the last S-MethodResultData	
									primitive.
	@param			aMoreDate		A flag indicating that there are subsequent 
									S-MethodResultData primitives to follow for 
									this transaction.
	@pre 			The WSP method transaction is in the Waiting2 state.
	@post			If subsequent S-MethodResultData primitives are expected,
					then the WSP method transaction is in the Waiting2 state. 
					If all the response information has been received, then the
					WSP method transaction is in the Completing state.
*/
	virtual void MethodResultDataInd(const TDesC8& aTrailerHeaders, TBool aMoreData) =0;

	};

#endif	// __MWSPCOMETHODCALLBACK_H__