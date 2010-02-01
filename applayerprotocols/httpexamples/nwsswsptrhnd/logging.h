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

/**
 @file logging.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __LOGGING_H__
#define __LOGGING_H__

// System includes
#include <e32std.h>
#include <f32file.h>
#include <flogger.h>

// By default, logging will always be switched on.  Later, this may be removed, leaving it to individual
// source files to enable logging where needed.
#define _LOGGING

#if defined (_DEBUG) && defined (_LOGGING)

// HTTP Logging macros
#define __DECLARE_LOG		RFileLogger iLogger;
#define __OPENLOG(D, L)		iLogger.Connect(); \
							TParse p; \
							_LIT(KLogFile, L); \
							p.Set(KLogFile(), NULL, NULL); \
							_LIT(KLogDir, D); \
							iLogger.CreateLog(KLogDir(), p.NameAndExt(), EFileLoggingModeOverwrite); \
							iLogger.SetDateAndTime(ETrue, ETrue);
#define __LOG_TITLE(T)		iLogger.Write(TPtrC8((const TText8 *)(T)));
#define __CLOSELOG			iLogger.Close();
#define __LOG(C)			iLogger.Write(TPtrC8((const TText8 *)(C)));
#define __LOG1(C, X)		iLogger.WriteFormat(TPtrC8((const TText8 *)(C)), X);
#define __LOG2(C, X, Y)		iLogger.WriteFormat(TPtrC8((const TText8 *)(C)), X, Y);
#define __LOG3(C, X, Y, Z)	iLogger.WriteFormat(TPtrC8((const TText8 *)(C)), X, Y, Z);
#define __DUMP(H, M, D)		{_LIT(KHeader, H); \
							_LIT(KMargin, M); \
							iLogger.HexDump((const TText*)(KHeader().Ptr()), (const TText*)(KMargin().Ptr()), D.Ptr(), D.Length());}
#define __QINFO(aText)		{User::InfoPrint(aText); User::After(1000000);} 

#elif !defined (_DEBUG)

// NULL macros
#define __DECLARE_LOG		TUint8 iDummyLoggerNotUsed[sizeof(RFileLogger)];
#define __OPENLOG(D, L)
#define __LOG_TITLE(T)
#define __CLOSELOG
#define __LOG(C)			
#define __LOG1(C, X)		
#define __LOG2(C, X, Y)		
#define __LOG3(C, X, Y, Z)	
#define __DUMP(H, M, D)		
#define __QINFO(aText) 
#endif // !_DEBUG

#endif // __LOGGING_H__
