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

#ifndef __MCONNECTINVOKERCALLBACK_H__
#define __MCONNECTINVOKERCALLBACK_H__

#include <e32base.h>

/** The MConnectInvokerCallback is used to perform an automatic session connect.
	An active object uses this callback to get the protocol handler to initiate
	the session connect.
*/
class MConnectInvokerCallback
	{
public:	// Methods

/**	@fn				HandleConnectInvocationL() =0
	Intended Usage	:
	@since			7.0
	@post			A session connect has been initiated with the new proxy.
*/
	virtual void HandleConnectInvocationL() =0;

/** @fn				ConnectInvokerCallbackError(TInt aError) =0
	Intended Usage	:	Error handling function for when RunL() leaves.
	@since			7.0
	@param			aError	The error with which RunL() left.
	@return			An error code. If the input error value could be handled, 
					then KErrNone is returned, otherwise the input error code
					is returned.
*/
	virtual TInt ConnectInvokerCallbackError(TInt aError) =0;

	};

#endif	// __MPROXYREDIRECTIONCALLBACK_H__
