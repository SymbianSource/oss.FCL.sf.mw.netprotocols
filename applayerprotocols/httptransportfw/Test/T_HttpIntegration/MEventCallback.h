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

#ifndef __MEVENTCALLBACK_H__
#define __MEVENTCALLBACK_H__

//-----------------------------------------------------------------------------

#include <e32std.h>

//-----------------------------------------------------------------------------

#include <http.h>

//-----------------------------------------------------------------------------

class MHTTPEventCallback
{
public:	// Methods

	virtual void HandleHTTPEvent(THTTPEvent aEvent, RHTTPTransaction* aTrans, TRequestStatus aStatus) = 0;
};

//-----------------------------------------------------------------------------

#endif	// __MEVENTCALLBACK_H__

//-----------------------------------------------------------------------------

