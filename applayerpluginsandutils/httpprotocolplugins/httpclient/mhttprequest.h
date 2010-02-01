// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MHTTPREQUEST_H__
#define __MHTTPREQUEST_H__

#include <e32std.h>

class MHttpDataOptimiser;
class MHttpRequest
	{
public:

	virtual void StartRequest() =0;

	virtual void CancelRequest() =0;

	virtual void RequestDataSent() =0;
	
	virtual void OnRequestSendTimeOut() =0;
	
	virtual TInt SendTimeOutValue() =0;

	virtual TBool CheckRequestPendingComplete() =0;

	virtual TBool NeedDisconnectNotification() =0;
	
	virtual MHttpDataOptimiser* HttpDataOptimiser(TBool& aBatchingEnabled) =0;
	};

#endif	//  __MHTTPREQUEST_H__
