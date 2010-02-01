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
// 
//

#ifndef __CTIMEOUTTIMER_H__
#define __CTIMEOUTTIMER_H__

//-----------------------------------------------------------------------------
//  Include Files  

#include <e32base.h>
#include <e32std.h>
#include <s32file.h>
#include <f32file.h>
#include <e32hal.h>
#include <fbs.h>

//-----------------------------------------------------------------------------

#include "MTimeoutCallback.h"

//-----------------------------------------------------------------------------
//class MTimeoutCallback;

//-----------------------------------------------------------------------------

class CTimeoutTimer : public CTimer
{
public:	// Methods

	static CTimeoutTimer* NewL(MTimeoutCallback& aCallback);

	~CTimeoutTimer();

private:	// Methods from CTimer

	virtual void RunL();

private:	// Methods

	CTimeoutTimer(MTimeoutCallback& aCallback);

	void ConstructL();

private:	// Attributes

	MTimeoutCallback&	iCallback;

};

//-----------------------------------------------------------------------------

#endif	// __CTIMEOUTTIMER_H__

//-----------------------------------------------------------------------------
//	End of File
//-----------------------------------------------------------------------------


