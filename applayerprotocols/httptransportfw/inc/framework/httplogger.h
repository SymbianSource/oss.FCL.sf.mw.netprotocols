// Copyright (c) 2001-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 @file httplogger.h
 @warning : This file contains Rose Model ID comments - please do not delete
 @publishedAll
 @released
*/

#ifndef __HTTPLOGGER_H__
#define __HTTPLOGGER_H__

// System includes
#include <e32std.h>
#include <flogger.h>


//##ModelId=3C4C4C3E0139
class THttpLogger : public TDesOverflow
/** 
A utility used in HTTP for logging in debug builds.  Log files are placed in the
directory %EPOCROOT%/epoc32/wins/c/logs/http
@publishedAll
@released
*/
	{
public: // methods

	/** Destructor - closes the log
	##ModelId=3C4C4C3E0180
	*/
	IMPORT_C virtual ~THttpLogger();

	/** Create log file in directory KLogsdir\KWapLogsDirName - Note: ignore Drive and Path of aFilename
	@param aFileName Log file name.
	@param aShowDate Report date in log.
	@param aShowTime Report time in log.
	##ModelId=3C4C4C3E0175
	*/
	IMPORT_C void CreateFlogger(const TDesC& aFileName, TInt aShowDate, TInt aShowTime);

	/** Logs a list of values formatted by a format string.
	@param aFmt Format string
	##ModelId=3C4C4C3E0162
	*/
	IMPORT_C void LogIt(TRefByValue<const TDesC> aFmt, ...);
	              
	/** Writes aComment to test log file, logging file and test harness.
	@param aComment String to log
	##ModelId=3C4C4C3E0158
	*/
	IMPORT_C void WriteComment(const TDesC& aComment);

	/** Do a formatted dump of binary data.
	@param aData Data to log.
	##ModelId=3C4C4C3E0150
	*/
	IMPORT_C void DumpIt(const TDesC8& aData);

	/** Do a write of the supplied 8 bit data.
	@param aData String to log
	##ModelId=3C4C4C3E015A
	*/
	IMPORT_C void WriteComment(const TDesC8& aData);

private:
	
	/** Methods from TDesOverflow
	@param aDes modifiable descriptor whose overflow results in the call to this overflow handler.
	*/
	IMPORT_C virtual void Overflow(TDes& aDes);

private: // attributes

	// Logger handle
	//##ModelId=3C4C4C3E014D
	RFileLogger* iLogger;
	};

// By default, logging will always be switched on.  Later, this may be removed, leaving it to individual
// source files to enable logging where needed.
#define _LOGGING

#if defined (_DEBUG) && defined (_LOGGING)

// HTTP Logging macros
#define __DECLARE_LOG		THttpLogger iLogger;
#define __OPEN_LOG(L)		iLogger.CreateFlogger(_L(L), ETrue, ETrue);
#define __OPEN_LOG2(L)		iLogger.CreateFlogger(L, ETrue, ETrue);
#define __LOG(C)			iLogger.WriteComment(C);
#define __LOG1(C, X)		iLogger.LogIt(C, X);
#define __LOG2(C, X, Y)		iLogger.LogIt(C, X, Y);
#define __LOG3(C, X, Y, Z)	iLogger.LogIt(C, X, Y, Z);
#define __DUMPBIN(D)		iLogger.DumpIt(D);
#define __QINFO(aText)		{User::InfoPrint(aText); User::After(1000000);} 

#else if !defined (_DEBUG)

// NULL macros
#define __DECLARE_LOG		TUint8 iDummyLoggerNotUsed[sizeof(THttpLogger)];
#define __OPEN_LOG(L)		
#define __OPEN_LOG2(L)		
#define __LOG(C)			
#define __LOG1(C, X)		
#define __LOG2(C, X, Y)		
#define __LOG3(C, X, Y, Z)	
#define __DUMPBIN(D)		
#define __QINFO(aText) 
#endif // !_DEBUG

#endif // __HTTPLOGGER_H__
