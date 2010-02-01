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
 @file MWspCLMethodCallback.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCLMETHODCALLBACK_H__
#define __MWSPCLMETHODCALLBACK_H__

// System includes
#include <e32base.h>

// Forward declarations
class MWspCLMethodInvoker;

class MWspCLMethodCallback
/**
@class MWspCLMethodCallback
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
public:	// Methods

/**
	@fn				UnitMethodResultInd(TInt aStatus, const TDesC8& aResponseHeaders, const TDesC8& aResponseBody) =0
	Intended Usage	:	This represents the S-Unit-MethodResult.ind primitive. 
						The Provider issues this when it receives a method
						response from the Server.
	@since			7.0
	@param			aStatus				A status code whose value is given by 
										the Status types defined in the WSP 
										Specification, July 2001 .
	@param			aResponseHeaders	A buffer with the encoded request header.
	@param			aResponseBody		A buffer with the data associated with the 
										request. Only used if the method aMethod 
										supports an entity body.
	@pre 			There is a MWspCLMethodCallback object associated with the 
					Transaction Id in the S-Unit-MethodResult.ind primitive.
	@post			The MWspCLMethodCallback object associated with the Transaction
					Id in the S-Unit-MethodResult.ind primitive is no longer valid.
 */
	virtual void UnitMethodResultInd(
									TInt			aStatus, 
									const TDesC8&	aResponseHeaders, 
									const TDesC8&	aResponseBody
									) =0;

	};

#endif	// __MWSPCLMETHODCALLBACK_H__