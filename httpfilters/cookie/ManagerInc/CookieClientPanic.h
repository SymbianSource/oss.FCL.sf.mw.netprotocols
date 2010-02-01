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
* Description:  Cookie Manager's panic handling at client-side.
*
*/


// INCLUDE FILES
	// System includes
#include <e32def.h>

// CONTANTS

_LIT( KCookieClientPanicString, "HTTP-COOKIE" );

// Client panics
enum TCookieClientPanic
	{
	ECookieBadRequest = 1,
	ECookieBrokenGetRequest,
	ECookieGetRequestPackingFailure,
	ECookieGetRequestListEmpty
    };