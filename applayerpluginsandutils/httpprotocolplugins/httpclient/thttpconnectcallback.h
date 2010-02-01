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

#ifndef __THTTPCONNECTCALLBACK_H__
#define __THTTPCONNECTCALLBACK_H__

#include <e32std.h>
#include <http/mhttptransactioncallback.h>

class MHttpConnectObserver;

class THttpConnectCallback : public MHTTPTransactionCallback
	{
public:

	THttpConnectCallback(MHttpConnectObserver& aObserver);

private:	// methods from MHTTPTransactionCallback

	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

private:

	MHttpConnectObserver&	iObserver;

	};

#endif	// __THTTPCONNECTCALLBACK_H__
