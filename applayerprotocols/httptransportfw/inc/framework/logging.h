// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @publishedPartner
 @released
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
#define __FLOG_DECLARATION_MEMBER		RFileLogger __logger__
#define __FLOG_DECLARATION_MEMBER2       RFileLogger __logger__;
#define __FLOG_OPEN(subsys,compnt) if( __logger__.Connect() == KErrNone ) \
										{ \
										TParse p; \
										_LIT(KLogFile, compnt); \
										p.Set(KLogFile(), NULL, NULL); \
										_LIT(KLogDir, subsys); \
										__logger__.CreateLog(KLogDir(), p.NameAndExt(), EFileLoggingModeOverwrite); \
										__logger__.SetDateAndTime(ETrue, ETrue);\
										} 
#define __FLOG_CLOSE					__logger__.Close()

#define __FLOG_0(text)						__logger__.Write(text)
#define __FLOG_1(text,a)					__logger__.WriteFormat(text,a)
#define __FLOG_2(text,a,b)					__logger__.WriteFormat(text,a,b)
#define __FLOG_3(text,a,b,c)				__logger__.WriteFormat(text,a,b,c)
#define __FLOG_4(text,a,b,c,d)				__logger__.WriteFormat(text,a,b,c,d)
#define __FLOG_5(text,a,b,c,d,e)			__logger__.WriteFormat(text,a,b,c,d,e)
#define __FLOG_6(text,a,b,c,d,e,f)			__logger__.WriteFormat(text,a,b,c,d,e,f)
#define __FLOG_7(text,a,b,c,d,e,f,g)		__logger__.WriteFormat(text,a,b,c,d,e,f,g)
#define __FLOG_8(text,a,b,c,d,e,f,g,h)		__logger__.WriteFormat(text,a,b,c,d,e,f,g,h)
#define __FLOG_9(text,a,b,c,d,e,f,g,h,i)	__logger__.WriteFormat(text,a,b,c,d,e,f,g,h,i)
#define __FLOG(text)						__logger__.Write(text)
#define __FLOG_VA(fmt, va_list)				__logger__.Write(fmt, va_list)
#define __FLOG_HEXDUMP(data,length)			__logger__.HexDump(0,0,data,length)

#define	_T8(text)							TPtrC8((const TText8 *)(text))

#elif !defined (_DEBUG)

// NULL macros
#define __FLOG_DECLARATION_MEMBER		TUint8 iDummyLoggerNotUsed[sizeof(RFileLogger)]
#define __FLOG_DECLARATION_MEMBER2            
#define __FLOG_OPEN(subsys,compnt)		
#define __FLOG_CLOSE

#define __FLOG_0(text)						
#define __FLOG_1(text,a)					
#define __FLOG_2(text,a,b)					
#define __FLOG_3(text,a,b,c)				
#define __FLOG_4(text,a,b,c,d)				
#define __FLOG_5(text,a,b,c,d,e)			
#define __FLOG_6(text,a,b,c,d,e,f)			
#define __FLOG_7(text,a,b,c,d,e,f,g)		
#define __FLOG_8(text,a,b,c,d,e,f,g,h)		
#define __FLOG_9(text,a,b,c,d,e,f,g,h,i)	
#define __FLOG(text)						
#define __FLOG_VA(fmt, va_list)				
#define __FLOG_HEXDUMP(data,length)

#define	_T8(text)

#endif // !_DEBUG

#endif // __LOGGING_H__
