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
* Description:  Commonly used constants are put here.
*         
*
*/


#ifndef __CCOKIE_SERVER_DEF_H__
#define __CCOKIE_SERVER_DEF_H__

// INCLUDE FILES
	// System includes


// CONSTANTS

// These are used during Cookie Server thread creation
const TUint KDefaultHeapSize = 0x10000;
const TUint KDefaultMaxHeapSize = 0x40000;

// A version must be specified when creating a session with the server
const TUint KCookieServerMajorVersionNumber = 1;
const TUint KCookieServerMinorVersionNumber = 1;
const TUint KCookieServerBuildVersionNumber = 0;

// Literals
_LIT( KCookieServerSemaphore, "CookieStartupSemaphore" );

_LIT( KCookieServerName, "CookieServer" );
_LIT( KCookieServerThreadName, "!CookieServer" );

_LIT( KCookieServerExe, "CookieServer.exe" );

// Server UID
const TUid KCookieServerExeUid = { 0x101F8530 };

#endif	// __CCOKIE_SERVER_DEF_H__