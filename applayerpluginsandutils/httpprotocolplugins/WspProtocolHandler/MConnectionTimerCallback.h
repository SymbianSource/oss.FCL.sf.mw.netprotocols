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

#ifndef __MCONNECTIONTIMERCALLBACK_H__
#define __MCONNECTIONTIMERCALLBACK_H__

#include <e32std.h>

// @todo docing

//##ModelId=3C4C419D024F
class MConnectionTimerCallback
	{
public:	// Methods

/**	@fn				HandleConnectionTimedOut() =0
	Intended Usage	:	A connection timer calls this function on the callback
						object to indicate that the connection time has lapsed.
	@since			7.0
	@pre			The connection time has lapsed.
	@post			The connection request has been cancelled.
*/
	//##ModelId=3C4C419D0263
	virtual void HandleConnectionTimedOut() =0;

	};

#endif	// __MCONNECTIONTIMERCALLBACK_H__

