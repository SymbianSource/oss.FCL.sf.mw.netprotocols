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
 @file MWspCOMethodInvoker.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCOMETHODINVOKER_H__
#define __MWSPCOMETHODINVOKER_H__

// System includes
#include <e32std.h>
#include <stringpool.h>

// Forward declarations
class MWspCOMethodCallback;
class MHTTPDataSupplier;
class TUriC8;


class MWspCOMethodInvoker
/** 
@class MWspCOMethodInvoker
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

/** @fn				MethodInvokeReq(MWspCOMethodCallback& aMethodCallback, RStringF aMethod, const TUriC8& aRequestUri, const TDesC8& aRequestHeaders, const TDesC8& aRequestBody, TBool aMoreData) =0
	Intended Usage	:	This represents the S-MethodInvoke.req primitive. The
						Provider initiates a method request, associating the 
						parameter aMethodCallback with a Transaction Id for
						this request. This Id is unique for the lifetime of the
						transaction. 
						
						The expected primitives to be received are S-MethodInvoke.cnf
						or S-MethodAbort.ind. The method is aborted if the Server
						Message size is exceeded. 
						
						The aMoreData flag indicates whether there are subsequent
						S-MethodInvokeData primitives for this transaction. The
						data for entity body will not excced the SDU.
	@since			7.0
	@param			aMethodCallback		A reference to an instance of the call-
										back API MWspCOMethodCallback. It 
										uniquely identifies this method 
										transaction for the lifetime of the 
										transaction.
	@param			aMethod				An RStringF object that describes the 
										HTTP method for this transaction.
	@param			aRequestUri			A URI object that specifies the resource
										to which this operation applies.
	@param			aRequestHeaders		A reference to a buffer with the encoded
										request headers.
	@param			aRequestBody		A reference to a buffer containing the 
										data associated with the request. Only 
										used if the method aMethod supports an
										entity body.
	@param			aMoreData			A boolean flag indicating whether there
										are subsequent S-MethodInvokeData 
										primitives for this transaction.
	@pre 			The WSP session is in the Connecting, Connected or Resuming 
					state. The method MOR has not been reached. The WSP method 
					transaction is in Null state. 
	@post			The WSP method transaction is in Requesting state. Neither 
					the Server SDU size nor the Server Message size have been 
					exceeded.
*/
	virtual void MethodInvokeReq(
								MWspCOMethodCallback&	aMethodCallback, 
								RStringF				aMethod, 
								const TUriC8&			aRequestUri, 
								const TDesC8&			aRequestHeaders, 
								const TDesC8&			aRequestBody,
								TBool					aMoreData
								) =0;

/** @fn				MethodInvokeDataReq(MWspCOMethodCallback& aMethodCallback, const TDesC8& aRequestBody, const TDesC8& aTrailerHeaders, TBool aMoreData) =0
	Intended Usage	:	This represents the S-MethodInvokeData.req primitive. 
						The Provider sends more data associated with a 
						previous S-MethodInvoke.req primitive.  The Transport
						Handler uses the transaction callback to identify which
						previous S-MethodInvoke.req corresponds correctly.
						
						The aMoreData flag indicates whether there are subsequent
						S-MethodInvokeData primitives for this transaction. The
						data for entity body will not excced the SDU.

						If there are trailer headers these are provided by the
						input parameter aTrailerHeaders. This is only valid if
						this is the last S-MethodInvokeData primitive.

						The expected callback primitives that may follow are 
						S-MethodInvokeData.cnf or S-MethodAbort.ind. The method 
						is aborted if the negotiated Server Message size is 
						exceeded.
	@since			7.0
	@param			aMethodCallback	A reference to an instance of the call-back 
									API MWspCOMethodCallback. It uniquely 
									identifies this method transaction for the 
									lifetime of the transaction.
	@param			aRequestBody	A reference to a buffer containing the data
									associated with the request.
	@param			aTrailerHeaders	A reference to a buffer with the encoded 
									trailer headers. This must be empty unless
									this is the last S-MethodInvokeData 
									primitive.
	@param			aMoreData		A boolean flag indicating whether there are 
									subsequent S-MethodInvokeData primitives for
									this transaction.
	@pre 			The WSP method transaction is in the Requesting state.
	@post			The WSP method transaction remains in the Requesting state.
					Neither the Server SDU size nor the Server Message size have
					been exceeded.
*/
	virtual void MethodInvokeDataReq(
									MWspCOMethodCallback&	aMethodCallback,
									const TDesC8&			aRequestBody,
									const TDesC8&			aTrailerHeaders,
									TBool					aMoreData
									) =0;

/** @fn				MethodAbortReq(MWspCOMethodCallback& aMethodCallback) =0
	Intended Usage	:	This represents the S-MethodAbort.req primitive. The 
						Provider aborts the method transaction specified by 
						aMethodCallback. The Client expects the S-MethdoInvoke.ind
						primitive to indicate that the transaction is aborted.
	@since			7.0
	@param			aMethodCallback	A reference to an instance of the call-back
									API MWspCOMethodCallback. It uniquely 
									identifies this method transaction for the
									lifetime of the transaction.
	@pre 			The WSP method transaction is in any state other than the
					Null or Aborting state.
	@post			The WSP method transaction is in the Aborting state.
*/
	virtual void MethodAbortReq(MWspCOMethodCallback& aMethodCallback) =0;

/** @fn				MethodResultRes(MWspCOMethodCallback& aMethodCallback, const TDesC8& aAckHeaders) =0
	Intended Usage	:	This represents the S-MethodResult.res primitive. The
						sending of acknowledgement headers is a negotiated
						capability. If the Server has not agreed to this
						capability, then the parameter aAckHeaders is ignored.
	@since			7.0
	@param			aMethodCallback	A reference to an instance of the call-back
									API MWspCOMethodCallback. It uniquely 
									identifies this method transaction for the 
									lifetime of the transaction.
	@param			aAckHeaders		A buffer that contains the optional set of
									headers containing some information to return
									to the Server. This may be empty.
	@pre 			The WSP method transaction is in the Waiting2 or Completing
					state.
	@post			If the WSP method transaction was in the Waiting2 state, it 
					remains in that state. If the WSP method transaction was in 
					the Completing state, it moves into the Null state and the
					method transaction is complete.
*/
	virtual void MethodResultRes(
								MWspCOMethodCallback&	aMethodCallback, 
								const TDesC8&				aAckHeaders
								) =0;

/** @fn				MethodResultDataRes(MWspCOMethodCallback& aMethodCallback, const TDesC8& aAckHeaders) =0
	Intended Usage	:	This represents the S-MethodResultData.res primitive. 
						The sending of acknowledgement headers is a negotiated
						capability. If the Server has not agreed to this
						capability, then the parameter aAckHeaders is ignored.
	@since			7.0
	@param			aMethodCallback	A reference to an instance of the call-back
									API MWspCOMethodCallback. It uniquely 
									identifies this method transaction for the 
									lifetime of the transaction.
	@param			aAckHeaders		A buffer that contains the optional set of 
									headers containing some information to return
									to the Server. This may be empty.
	@pre 			The WSP method transaction is in the Waiting2 or Completing
					state.
	@post			If the WSP method transaction was in the Waiting2 state, it 
					remains in that state. If the WSP method transaction was in 
					the Completing state, it moves into the Null state amd the
					method transaction is complete.
*/
	virtual void MethodResultDataRes(
									MWspCOMethodCallback&	aMethodCallback, 
									const TDesC8&			aAckHeaders
									) =0;

	};

#endif // __MWSPCOMETHODINVOKER_H__

