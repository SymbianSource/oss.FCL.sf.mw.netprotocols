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
* Description:  Common constants are here, shared among client and server
*
*
*/


#ifndef __COOKIE_COMMON_CONSTANTS_H__
#define __COOKIE_COMMON_CONSTANTS_H__

// CONSTANTS

const TInt KCookieSessionBufferLength = 0x1000;

// Dynamic array granularities
const TInt KCookieStandardGranularity = 5;

// Default HTTP request port is 80
_LIT8( KCookieDefaultRequestPort, "80" );

// For effective-host names
_LIT8( KCookieLocalHostNamePostfix, ".local" );

// Version number that the client support
_LIT8( KCookieKnownVersion, "1" );

// Domain separator character
const TUint8 KCookieDomainSeparator = '.';

// Wildcard for pattern matching
const TUint8 KCookieWildcardAsterisk = '*';

#endif	// __COOKIE_COMMON_CONSTANTS_H__