// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// $Header$
// The header file introducing the basic event management control (active
// objects) for our domain command supprt.
// based loosely on the WSP version...
// Note: you must derive from this for the domain you require, providing the
// appropriate callbacks - this way reuse and oo is maintained.
// 
//

#ifndef __CEVENTDISPATCHER_H__
#define __CEVENTDISPATCHER_H__

//-----------------------------------------------------------------------------
//  Include Files  

#include <e32base.h>

//-----------------------------------------------------------------------------

#include "MTimeoutCallback.h"
#include "MHTTPEventCallback.h"

//-----------------------------------------------------------------------------

class CTimeoutTimer;

//-----------------------------------------------------------------------------

class CEventDispatcher : public CActive, 
												 public MTimeoutCallback
{
public:	// Methods

	static CEventDispatcher *NewL(MHTTPEventCallback &aCallback);
	~CEventDispatcher();

	virtual void WaitForHTTPEvent(RHTTPSession *aObj, TInt aTimeoutValueSeconds);
	virtual void CancelWait();


private:	// Methods from CActive

	virtual void RunL();
	virtual void DoCancel();

private:	// Methods from MTimeoutCallback

	virtual void Timeout();

private:	// Methods

	CEventDispatcher(MHTTPEventCallback & aCallback);
	void ConstructL();

private:	// Attributes

	CTimeoutTimer *iTimer;
	MHTTPEventCallback &iCallback;
	THTTPEvent iEvent;
	RHTTPSession *iSession;
	RHTTPTransaction *iTransaction;
};

#endif	// __CEVENTDISPATCHER_H__