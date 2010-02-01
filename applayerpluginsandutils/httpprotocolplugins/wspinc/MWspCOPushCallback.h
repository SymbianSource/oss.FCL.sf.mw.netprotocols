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
 @file MWspCOPushCallback.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCOPUSHCALLBACK_H__
#define __MWSPCOPUSHCALLBACK_H__

// System includes
#include <e32base.h>

// Forward declarations
class MHTTPDataSupplier;


class MWspCOPushCallback
/**
@class MWspCOPushCallback
@since			7.0
The MWspCOPushInvoker and MWspCOPushCallback form the API that proides the
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

In the case of the Confirmed Push facility, the number of these objects that
the Client supplies to the Provider is decided by the negotiated Push MOR. 
If the Push facility has been successfully negotiated, the Client must supply
a further Push transaction object.
@publishedAll
@deprecated
*/
	{
public:	// Methods

/**
	@fn				PushInd(const TDesC8& aPushHeader, const TDesC8& aPushBody) =0
	Intended Usage	:	This represents the S-Push.ind primitive. The Provider 
						issues this when it receives unconfirmed Push information.
						The Client supplies a new listening Push transaction object,
						via the MWspCOPushInvoker API, to maintain the correct 
						number of these objects in the Provider.
	@since			7.0
	@param			aPushHeader	A buffer that contains the byte-encoded headers
								for the Push information.
	@param			aPushBody	A buffer that contains the body of the Push 
								information.
	@pre 			There is a listening Push transaction object.
	@post			The number of listening Push transaction objects are reduced
					by one.
 */
	virtual void PushInd(
						const TDesC8&	aPushHeader,
						const TDesC8&	aPushBody
						) =0;

/**
	@fn				ConfirmedPushInd(const TDesC8& aPushHeader, const TDesC8& aPushBody) =0
	Intended Usage	:	This represents the S-ConfirmedPush.ind primitive. The
						Provider issues this when it receives Confirmed Push 
						information. The Provider expects either the 
						S-ConfirmedPush.res or the S-PushAbort.req primitives to
						be issued from the Client.
	@since			7.0
	@param			aPushHeader	A buffer that contains the byte-encoded headers
								for the Push information.
	@param			aPushBody	A buffer that contains the body of the Push 
								information.
	@pre 			The WSP push transaction is in the Null state.
	@post			The WSP push transaction is in the Receiving state.
 */
	virtual void ConfirmedPushInd(
								 const TDesC8&	aPushHeader, 
								 const TDesC8&	aPushBody
								 ) =0;

/**
	@fn				PushAbortInd(TWspReason aReason) =0
	Intended Usage	:	This represents the S-PushAbort.ind primitive. This is
						issued when Confirmed Push information has been rejected.
	@since			7.0
	@param			aReason	A parameter that indicates the cause of the abort.
							The value maps to the Reason type defined in the 
							WSP Specification, July 2001. 
	@pre 			The WSP push transaction is in the Receiving or Aborting state.
	@post			The WSP push transaction is in the Null state.
 */
	virtual void PushAbortInd(TWspReason aReason) =0;

	};

#endif	// __MWSPCOPUSHCALLBACK_H__
