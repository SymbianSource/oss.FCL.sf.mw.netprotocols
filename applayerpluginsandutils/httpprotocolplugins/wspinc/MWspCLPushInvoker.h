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
 @file MWspCLPushInvoker.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCLPUSHINVOKER_H__
#define __MWSPCLPUSHINVOKER_H__

// System includes
#include <e32base.h>

// Forward declarations
class MWspCLPushCallback;


class MWspCLPushInvoker
/**
@class MWspCLPushInvoker
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
	@fn				InitPushReceive(MWspCLPushCallback& aPushCallback) =0
	Intended Usage	:	Supplies the Provider with a Push transaction object.
						Also, informs the Provider that the Client is ready to
						receive Push information. The Provider uses this call-
						back object to pass Push information to the Client.
	@since			7.0
	@param			aPushCallback	A reference to an instance of the call-back 
									API MWspCLPushCallback.It uniquely identifies
									this method transaction for the lifetime of 
									the transaction.
	@pre 			The underlying layers are prepared for communication. 
	@post			The Provider is waiting for a S-Unit-Push primitive from 
					the Server. Increases the number of listening Push 
					transactions by one.
 */
	virtual void InitPushReceive(MWspCLPushCallback& aPushCallback) =0;

/**
	@fn				CancelPushReceive(MWspCLPushCallback& aPushCallback) =0
	Intended Usage	:	Removes the listening Push transaction. The Client no
						longer wishes to receive Push information via the 
						specified MWspCLPushCallback object.
	@since			7.0
	@param			aPushCallback	A reference to an instance of the call-back 
									API MWspCLPushCallback.It uniquely identifies
									this method transaction for the lifetime of 
									the transaction.
	@pre 			The Push transaction object is exists.
	@post			The aPushCallback object is no longer valid. The number of
					listening Push transaction objects in the Provider is reduced
					by one.
 */
	virtual void CancelPushReceive(MWspCLPushCallback& aPushCallback) =0;

	};

#endif	// __MWSPCLPUSHINVOKER_H__

