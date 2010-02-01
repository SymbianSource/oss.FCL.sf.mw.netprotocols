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
 @file MWspCOPushInvoker.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCOPUSHINVOKER_H__
#define __MWSPCOPUSHINVOKER_H__

// System includes
#include <e32std.h>
#include <wsp/wsptypes.h>

// Forward declarations
class MWspCOPushCallback;


class MWspCOPushInvoker
/** 
@class MWspCOPushInvoker
@since			7.0
The MWspCOPushInvoker and MWspCOPushCallback form the API that provides the
WSP Connection-Orientated Session Servise Push and Confirmed Push facilities,
as decribed in the WSP Specification, July 2001.

The MWspCOPsuhInvoker interface is supplied by the Provider entity. The 
MWspCOPsuhCallback interface is supplied by the Client entity. The Provider
implements the communications necessary to send the primitives issued by the 
Client to the Server, and from the Server to the Client.

The use of the Push and Confirmed Push facilities during the lifetime of the
WSP Session is dependent on these being successfully negotiated capabilities
for that session.

The Push MOR is also negotiated with the Server, and defines the number of 
outstanding confirmed push invocations that can be active at one time.

The object that implements the call-back API MWspCOPushCallback encapsulates
the concept of a Push transaction. The Provider associates each of these 
objects with a Push Id used to distinguish between push transactions during 
a WSP session.

The sending of Push information is initiated by the Server, but for this
information to be passed onto the Client, the Provider must be informed that
the Client is ready to receive Push transactions. This is achieved by 
supplying the Provider with Push transaction objects that are prepared to 
receive Push information - listening Push transaction objects. 

In the case of the Confirmed Push facility, the exact number of these objects
that the Client supplies to the Provider is determined by the negotiated Push MOR. 

Furthermore, if the Push facility has been successfully negotiated, the Client
must supply one additional Push transaction object.
@publishedAll
@deprecated
*/
	{
public:	// Methods

/**
	@fn				PushListen(MWspCOPushCallback& aPushCallback) =0
	Intended Usage	:	Provides a Push transaction to the Provider. Also, informs
						the Provider that the Client can receive Push information.
						The Provider uses this call-back object to pass Push and
						Confirmed Push transactions to the Client.
	@since			7.0
	@param			aPushCallback	A reference to an instance of the call-back 
									API MWspCOPushCallback.It uniquely identifies
									this method transaction for the lifetime of 
									the transaction.
	@pre 			The WSP session is in the Connecting, Connected or Resuming
					state. Either the Push facility, Confirmed Push facility or 
					both have been successfully negotiated with the Server.
	@post			Increases the number of listening Push transactions by one.
 */
	virtual void PushListen(MWspCOPushCallback& aPushCallback) =0;

/**
	@fn				CancelPushListen(MWspCOPushCallback& aPushCallback) =0
	Intended Usage	:	Removes a listening Push transaction. The Client no longer
						wishes to receive Push information via the specified Push 
						transaction call-back object.
	@since			7.0
	@param			aPushCallback	A reference to an instance of the call-back 
									API MWspCOPushCallback.It uniquely identifies 
									this method transaction for the lifetime of 
									the transaction.
	@pre 			The Push transaction object exists.
	@post			Reduces the number of listening Push transactions by one.
 */
	virtual void CancelPushListen(MWspCOPushCallback& aPushCallback) =0;

/**
	@fn				PushAbortReq(MWspCOPushCallback& aPushCallback, TWspReason aReason) =0
	Intended Usage	:	This represents the S-PushAbort.req primitive. The Client
						issues this when it has received Confirmed Push 
						information that it wishes to reject.The Client expects
						the S-PushAbort.ind to be issued by the Provider.
	@since			7.0
	@param			aPushCallback	A reference to an instance of the call-back 
									API MWspCOPushCallback.It uniquely identifies 
									this method transaction for the lifetime of 
									the transaction.
	@param			aReason			A parameter that indicates the cause of the 
									abort.The value maps to the Reason type 
									defined in the July 2001 WSP Specification.	
	@pre 			The call-back reference aPushCallback represents a Confirmed 
					Push transaction. The WSP push transaction is in the Receiving
					state.
	@post			The WSP push tranasaction is in the Aborting state.
 */
	virtual void PushAbortReq(
							 MWspCOPushCallback&	aPushCallback, 
							 TWspReason				aReason
							 ) =0;

/**
	@fn				ConfirmedPushRes(MWspCOPushCallback& aPushCallback, const TDesC8& aAckHeaders) =0
	Intended Usage	:	This represents the S-ConfirmedPush.res primitive. The
						Protocol Handler issues this primitive to acknowledge
						the receipt of Confirmed Push information. The call-back
						pointer is no longer valid after this call.
	@since			7.0
	@param			aPushCallback	A reference to an instance of the call-back 
									API MWspCOPushCallback.It uniquely identifies 
									this method transaction for the lifetime of 
									the transaction. This is no longer valid once 
									this call returns.
	@param			aAckHeaders		A buffer that contains the optional set of 
									headers containing some information to return 
									to the Server.
	@pre 			The WSP push transaction is in the Receiving state.
	@post			The WSP push transaction is in the Null state.	The call-back
					object aPushCallback is no longer valid.
 */
	virtual void ConfirmedPushRes(
								 MWspCOPushCallback&	aPushCallback, 
								 const TDesC8&			aAckHeaders
								 ) =0;

	};

#endif	// __MWSPCOPUSHINVOKER_H__