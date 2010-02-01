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
 @file MWspCLMethodInvoker.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCLMETHODINVOKER_H__
#define __MWSPCLMETHODINVOKER_H__

// System includes
#include <e32base.h>

// Forward declarations
class MWspCLMethodCallback;


class MWspCLMethodInvoker
/**
@class MWspCLMethodInvoker
@since			7.0
The MWspCLMethodInvoker and MWspCLMethodCallback form the API that provides
the WSP Connectionless Session Service Method Invocation facility, as 
described in the WSP Specification, July 2001. 

The MWspCLMethodInvoker interface is supplied by the Provider entity. The 
MWspCLMethodCallback interface is supplied by the Client entity. The 
Provider implements the communications necessary to send the primitives 
issued by the Client to the Server, and from the Server to the Client.

The Client object that encapsulates a method transaction implements the call-back
API MWspCLMethodCallback. Internally, the Provider should associate each of these
objects with a WSP Transaction Id in order to distinguish between method transactions.
@publishedAll
@deprecated
*/
	{
public:	// Method

/**
	@fn				UnitMethodInvoke(MWspCLMethodCallback& aMethodCallback, RStringF aMethod, const TUriC8& aRequestUri, const TDesC8& aRequestHeaders, const TDesC8& aRequestBody) =0
	Intended Usage	:	This represents the S-Unit-MethodInvoke primitive. The
						Provider initiates a non-confirmed method with the Server.
						The Client expects the S-Unit-MethodResult primitive to 
						be issued from the Provider.
	@since			7.0
	@param			aMethodCallback	A refernce to an instance of the call-back 
									API MWspCLMethodCallback.It uniquely 
									identifies this  method transaction for the
									lifetime of the transaction.
	@param			aMethod			An RStringF object that describes the HTTP 
									method for this transaction.
	@param			aRequestUri		A URI object that specifies the resource
									to which this operation applies.
	@param			aRequestHeaders	A buffer with the encoded request header.
	@param			aRequestBody	A buffer with the data associated with the 
									request. Only used if the method aMethod 
									supports an entity body.
	@pre 			The underlying layers are prepared for communication. 
	@post			The Provider is waiting for a S-Unit-MethodResult primitive
					from the Server.
 */
	virtual void UnitMethodInvoke(
								 MWspCLMethodCallback&	aMethodCallback, 
								 RStringF				aMethod, 
								 const TUriC8&			aRequestUri, 
								 const TDesC8&			aRequestHeader, 
								 const TDesC8&			aRequestBody
								 ) =0;

/**
	@fn				CancelMethodInvoke(MWspCLMethodCallback& aMethodCallback) =0
	Intended Usage	:	The Client wishes to cancel the method. The call-back
						object pointed to by aMethodCallback is no longer valid.
						The Provider ignores any method responses associated with
						aMethodCallback.
	@since			7.0
	@param			aMethodCallback	A refernce to an instance of the call-back 
									API MWspCLMethodCallback.It uniquely 
									identifies this  method transaction for the
									lifetime of the transaction.
	@pre 			There exists a Transaction Id associated with aMethodCallback.
	@post			Any received S-Unit-MethodResult primitive with a Transaction 
					Id associated with aMethodCallback is ignored.
 */
	virtual void CancelMethodInvoke(MWspCLMethodCallback& aMethodCallback) =0;

	};

#endif	// __MWSPCLMETHODINVOKER_H__