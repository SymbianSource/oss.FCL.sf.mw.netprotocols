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
//

#ifndef __CWSPEVENTDISPATCHER_H__
#define __CWSPEVENTDISPATCHER_H__

#include "MTimeoutCallback.h"
#include "MWspEventCallback.h"

class MWspEventCallback;
class CTimeoutTimer;

#include <http.h>

//-----------------------------------------------------------------------------

class CWspEventDispatcher : public CActive, public MTimeoutCallback
	{
public:	// Methods

	static CWspEventDispatcher* NewL(MWspEventCallback& aCallback);

	~CWspEventDispatcher();

	void WaitForWspEvent(TInt aTimeoutValueSeconds);
	void WaitForWspEvent();

	void CancelWait();

private:	// Methods from CActive

	virtual void RunL();

	virtual void DoCancel();

private:	// Methods from MTimeoutCallback

	virtual void Timeout();

private:	// Methods

	CWspEventDispatcher(MWspEventCallback& aCallback);

	void ConstructL();

private:	// Attributes

	MWspEventCallback&			iCallback;

	CTimeoutTimer*				iTimer;

	TInt						iEvent;
			
	RHTTPTransaction *iTransaction;

	};
#endif	// __CWSPEVENTDISPATCHER_H__
