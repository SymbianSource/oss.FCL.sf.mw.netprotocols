// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file
 @internalComponent
*/
#ifndef __TIMERLOGGER_H__
#define __TIMERLOGGER_H__

#include <e32std.h>
#include <flogger.h>
#include <f32file.h>

// Uncomment the below line to generate performance logger
// #define __LOG_PERFORMANCE

/** A utility used in HTTP for performing HTTPIOP performance tests in release builds.  Log file is placed in the
directory %EPOCROOT%/epoc32/winscw/c/logs/http
@internalComponent
@released
*/
class TTimerLogger : public TDesOverflow
{
public: // methods
    IMPORT_C ~TTimerLogger();
	IMPORT_C void StartTimer();
	IMPORT_C void EndTimer(const TDesC&);
	IMPORT_C void TestName(const TDesC&);	
   
private:
	IMPORT_C virtual void Overflow(TDes& aDes);
	void LogIt(const TDesC& aComment);

private: // attributes

	RFs iFs;
	RFile iFile;
	TTime iStartTime;
	TTime iEndTime; 
};
#if defined __LOG_PERFORMANCE	 
// HTTP Logging macros
#define __DECLARE_PERFORMANCE_LOG		mutable TTimerLogger iTLogger;
#define __START_PERFORMANCE_LOGGER()	  iTLogger.StartTimer()
#define __END_PERFORMANCE_LOGGER(C)		  iTLogger.EndTimer(C)
#define __TESTNAME(C)		              iTLogger.TestName(C);

#else
#define __DECLARE_PERFORMANCE_LOG
#define __START_PERFORMANCE_LOGGER()	  
#define __END_PERFORMANCE_LOGGER(C)		  
#define __TESTNAME(C)		              

#endif //__LOG_PERFORMANCE
#endif // __TIMERLOGGER_H__
