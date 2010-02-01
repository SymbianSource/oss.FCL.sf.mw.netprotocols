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
* Description: 
*		Implemented logger functionality of the module
*
*
*/


#ifndef HTTP_AUTH_LOGGER_H
#define HTTP_AUTH_LOGGER_H



// INCLUDE FILES
#include <e32svr.h>


// CONSTANTS

const TInt KHttpAuthMajorVersion = 1;
const TInt KHttpAuthMinorVersion = 0;
const TInt KHttpAuthBuildVersion = 1;
    
_LIT( KHttpAuthLogMessageAppBanner,   "HttpAuth: application (%d.%d.%d) started" );
_LIT( KHttpAuthLogEnterFn,            "HttpAuth: -> %S" );
_LIT( KHttpAuthLogLeaveFn,            "HttpAuth: <- %S" );
_LIT( KHttpAuthLogExit,               "HttpAuth: application exit" );
_LIT( KHttpAuthLogTimeFormatString,   "%H:%T:%S:%*C3" );


// ATTENTION! We're depending on FLogger DLL, because it assumes the existance 
// of C:\LOGS directory.
_LIT( KHttpAuthFLoggerDependency,	"c:\\logs\\" );
_LIT( KHttpAuthDoubleBackSlash,		"\\" );
_LIT( KHttpAuthLogDir,               "httpauth" );
_LIT( KHttpAuthLogFile,              "httpauth.log" );

#ifdef _DEBUG

//Logging to file in THUMB UDEB
#include <flogger.h>
#define AUTHLOGGER_CREATE { RFileLogger::WriteFormat( KHttpAuthLogDir, KHttpAuthLogFile, EFileLoggingModeOverwrite, KHttpAuthLogMessageAppBanner, KHttpAuthMajorVersion, KHttpAuthMinorVersion, KHttpAuthBuildVersion ); }
#define AUTHLOGGER_DELETE { RFileLogger::Write( KHttpAuthLogDir, KHttpAuthLogFile, EFileLoggingModeAppend, KHttpAuthLogExit ); }
#define AUTHLOGGER_ENTERFN( a ) { _LIT( temp, a ); RFileLogger::WriteFormat( KHttpAuthLogDir, KHttpAuthLogFile, EFileLoggingModeAppend, KHttpAuthLogEnterFn, &temp ); }
#define AUTHLOGGER_LEAVEFN( a ) { _LIT( temp, a ); RFileLogger::WriteFormat( KHttpAuthLogDir, KHttpAuthLogFile, EFileLoggingModeAppend, KHttpAuthLogLeaveFn, &temp ); }
#define AUTHLOGGER_WRITE( a ) { _LIT( temp, a ); RFileLogger::Write( KHttpAuthLogDir, KHttpAuthLogFile, EFileLoggingModeAppend, temp ); }
#define AUTHLOGGER_WRITE_FORMAT8( a, b ) { _LIT8( temp, a ); RFileLogger::WriteFormat( KHttpAuthLogDir, KHttpAuthLogFile, EFileLoggingModeAppend, temp, b ); }
#define AUTHLOGGER_WRITE_FORMAT( a, b ) { _LIT( temp, a ); RFileLogger::WriteFormat( KHttpAuthLogDir, KHttpAuthLogFile, EFileLoggingModeAppend, temp, b ); }
#define AUTHLOGGER_WRITE_TIMESTAMP( a ) { _LIT( temp, a ); TTime time; time.HomeTime(); TBuf<256> buffer; time.FormatL( buffer, KHttpAuthLogTimeFormatString ); buffer.Insert( 0, temp ); RFileLogger::Write( KHttpAuthLogDir, KHttpAuthLogFile, EFileLoggingModeAppend, buffer); }

#else // THUMB

//Doing nothing in UREL
#define AUTHLOGGER_CREATE
#define AUTHLOGGER_DELETE
#define AUTHLOGGER_ENTERFN( a )
#define AUTHLOGGER_LEAVEFN( a )
#define AUTHLOGGER_WRITE( a )
#define AUTHLOGGER_WRITE_FORMAT8( a, b )
#define AUTHLOGGER_WRITE_FORMAT( a, b )
#define AUTHLOGGER_WRITE_TIMESTAMP( a )

#endif //_DEBUG
#endif	// HTTP_AUTH_LOGGER_H

// End of file