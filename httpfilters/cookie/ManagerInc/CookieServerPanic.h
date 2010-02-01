/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Cookie Manager's panic handling at server-side.
*
*/


#ifndef __COOKIE_SERVER_PANIC_H__
#define __COOKIE_SERVER_PANIC_H__

// INCLUDE FILES
	// System includes
#include <e32std.h>
#include <e32base.h>

// CONSTANTS

_LIT( KCookieServerPanicString, "Cookie" );

// Server panic codes
enum TCookieServerPanic
	{
	EMainSchedulerError1,
	EMainSchedulerError2,
    EArrayIndexOutOfRange
	};

// Global panic function
GLREF_C void PanicServer( TCookieServerPanic aPanic );

#endif	// __COOKIE_SERVER_PANIC_H__