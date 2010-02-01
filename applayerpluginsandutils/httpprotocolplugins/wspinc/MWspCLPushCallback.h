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
 @file MWspCLPushCallback.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCLPUSHCALLBACK_H__
#define __MWSPCLPUSHCALLBACK_H__

// System includes
#include <e32base.h>

// Forward declarations
class MWspCLPushInvoker;


class MWspCLPushCallback
/**
@class MWspCLPushCallback
@since			7.0
The MWspCLPushInvoker and MWspCLPushCallback form the API that provides the 
WSP Connectionless Session Service Push facility, as in the WSP Specification,
July 2001. The MWspCLMethodInvoker interface is supplied by the Provider 
entity. The MWspCLMethodCallback interface is supplied by the Client entity.
The Provider implements the communications necessary to send the primitives 
issued by the Client to the Server, and from the Server to the Client.

The Client object that encapsulates a push transaction implements the call-back
API MWspCLPushCallback. Internally, the Provider should associate each of these
objects with a WSP Push Id in order to distinguish between push transactions.
@publishedAll
@deprecated
*/
	{
public:	// Methods

/**
	@fn				UnitPushInd(const TDesC8& aPushHeader, const TDesC8& aPushBody) =0
	Intended Usage	:	This represents the S-Unit-Push.ind primitive. The 
						Provider issues this when it receives Push information.
						The Client supplies a new listening Push transaction 
						object, via the MWspCLPushInvoker API, to maintain the
						correct number of these objects in the Provider.
	@since			7.0
	@param			aPushHeader	A buffer that contains the byte-encoded headers 
								for the Push information.
	@param			aPushBody	A buffer that contains the body of the Push 
								information.
	@pre 			There is a listening Push transaction object available in 
					the Provider.
	@post			The number of available Push transaction objects available
					in the Provider is reduced by one.
 */
	virtual void UnitPushInd(
							const TDesC8&	aPushHeader,
							const TDesC8&	aPushBody
							) =0;

	};

#endif	// __MWSPCLPUSHCALLBACK_H__

