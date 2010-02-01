/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* FTP protocol DLL
* Author:	Philippe Gabriel
* Some defs for debugging
* 
*
*/



/**
 @file DEBUG.H
 @internalComponent
*/

#if !defined(__DEBUG_H__)
#define __DEBUG_H__

#if defined(_DEBUG)
    #define __FTPPROTDEBUG__
#endif

#if !defined(__FTPPROTDEBUG__)
/**
@internalComponent
*/
	#define FTPPROTDEBUG(mask,x)
	#define FTPPROTDEBUG1(mask,x,y)
	#define FTPPROTDEBUG2(mask,x,y)
	#define FTPPROTDEBUG3(mask,x,y,z)
#else

#include <comms-infras/commsdebugutility.h>
//////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////

/**
Log files name
*/
_LIT(KTestLogFileName,"ftpsess.log");
_LIT(KTestLogDir,"ftplog");

/**
Main Debug Mask
*/
#define _DBGflags 0xffff

/**
Debug flags per module
FTPPROT Dll
*/
#define _DBGFtpprot		(1<<0)
#define _DBGAnsparse	(1<<1)
#define _DBGDtpchnl		(1<<2)
#define _DBGPichnl		(1<<3)
#define _DBGResolver	(1<<4)
#define _DBGFtpprotDLL	(_DBGAnsparse | _DBGDtpchnl | _DBGPichnl | _DBGResolver | _DBGFtpprot | _DBGFtpseterr)
/** FTPSESS Dll */
#define _DBGFtpsess		(1<<5)
#define _DBGFtpsessDLL	(_DBGFtpsess | _DBGFtpseterr)
/** various */
#define _DBGFtpseterr	(1<<6)

/**
conditions flags
@internalComponent
*/
#define _DBGEnter	(1<<15)
#define _DBGExit	(1<<16)
#define _DBGRecv	(1<<17)
#define _DBGSnd		(1<<18)
#define _DBGError	(1<<19)

/**
@internalComponent
*/
inline void ftpprotdebug(TUint32 aMask, const TPtrC& aDes)
	{
	if (_DBGflags & aMask)
		RFileLogger::Write(KTestLogDir,KTestLogFileName,EFileLoggingModeAppend,aDes);
	}
#define FTPPROTDEBUG(mask,x) ftpprotdebug(mask,x)

/**
@internalComponent
*/
inline void ftpprotdebug1(TUint32 aMask, const TPtrC& aDes, TUint aNum)
	{
	if (_DBGflags & aMask)
		RFileLogger::WriteFormat(KTestLogDir,KTestLogFileName,EFileLoggingModeAppend,aDes,aNum);
	}
#define FTPPROTDEBUG1(mask,x,y) ftpprotdebug1(mask,x,y)

/**
@internalComponent
*/
inline void ftpprotdebug2(TUint32 aMask, const TPtrC& aDes1, const TPtrC& aDes2)
	{
	if (_DBGflags & aMask)
		RFileLogger::WriteFormat(KTestLogDir,KTestLogFileName,EFileLoggingModeAppend,aDes1,&aDes2);
	}

/**
@internalComponent
*/
#define FTPPROTDEBUG2(mask,x,y) ftpprotdebug2(mask,x,y)

/**
@internalComponent
*/
inline void ftpprotdebug3(TUint32 aMask, const TPtrC& aDes, TUint aNum1, TUint aNum2)
	{
	if(_DBGflags & aMask)
		RFileLogger::WriteFormat(KTestLogDir,KTestLogFileName,EFileLoggingModeAppend,aDes,aNum1, aNum2);
	}
#define FTPPROTDEBUG3(mask,x,y,z) ftpprotdebug3(mask,x,y,z)

#endif //_DEBUG

#endif //__DEBUG_H__
