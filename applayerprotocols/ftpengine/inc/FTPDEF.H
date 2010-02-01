/**
* Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
* FTP General definitions
* Author:	Philippe Gabriel
* 
*
*/



/**
 @file FTPDEF.H
 @internalComponent
*/

#if !defined(__FTPDEF_H__)
#define __FTPDEF_H__

#define MAX_BUFFER_SIZE 4096
#define IAC 0xFF
#define SYNCH	0xF2
#define IP	0xF4
#define CR 0x0D
#define LF 0x0A

#define DEFAULT_SERVER_PI_PORT 21
/**
standard server default
*/
#define DEFAULT_SERVER_DTP_PORT (DEFAULT_SERVER_PI_PORT-1)	


#endif //__FTPDEF_H__
