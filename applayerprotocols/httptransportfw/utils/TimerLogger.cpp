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

#include <f32file.h> // for TParse, in Flogger stuff
#include <e32std.h>

#include "timerlogger.h"

#define KTestHeader				_L8("HTTP PERFORMANCE LOGGER\n\n\n")
#define KTestLogs				_L8("TEST NAME,METHOD NAME,TIME IN MICROSECONDS\n\n")
#define KTestCommentPrepend		_L("\t")

_LIT(KFileName, "c:\\logs\\http\\peformance_logger.csv");
_LIT(KFilePath, "c:\\logs\\http\\");

#define KMaxLogLineLength 128

/**
 Destructor
*/
EXPORT_C TTimerLogger::~TTimerLogger()
	{
#if defined (__LOG_PERFORMANCE) && !defined (_DEBUG)
	iFile.Close();
	iFs.Close();
#endif
	}

void TTimerLogger::LogIt(const TDesC& aComment)
	{
	TInt err = iFs.Connect();
	if(err == KErrNone)
		{
		iFs.MkDirAll(KFilePath);
		err = iFile.Open(iFs, KFileName, EFileWrite);
		if(err == KErrNotFound)
			{
			iFile.Create(iFs, KFileName, EFileWrite);
			iFile.Write(KTestHeader);
			iFile.Write(KTestLogs);
			}
		TFileText file;
	    file.Set(iFile);
		file.Write(aComment);
		iFile.Close();
		iFs.Close();
		}
	else	
		{
		User::InfoPrint(_L("Cannot write to file"));
		}
	}
	

/** 
    Initiates the timer
*/
EXPORT_C void TTimerLogger::StartTimer()
	{
	#if defined (__LOG_PERFORMANCE) && !defined (_DEBUG)
	iStartTime.UniversalTime();
	#endif
	}


/** 
  Terminates the timer
  @param aComment Name of API.
*/
EXPORT_C void TTimerLogger::EndTimer(const TDesC& aComment)
	{
    #if defined (__LOG_PERFORMANCE) && !defined (_DEBUG)
	iEndTime.UniversalTime();
	TTimeIntervalMicroSeconds iTimeDifference = iEndTime.MicroSecondsFrom(iStartTime);	
	_LIT(KTimeDiff, ",%d microseconds\n");
	RBuf myBuf;
	TInt err = myBuf.Create (aComment.Length()+64);
	__ASSERT_ALWAYS(KErrNone == err, User::Invariant() );
	myBuf.Append (aComment );
	myBuf.AppendFormat(KTimeDiff, iTimeDifference.Int64());
    LogIt(myBuf);
	myBuf.Close();	
	iStartTime = 0;
	iEndTime = 0;
	#endif
	}


/** Methods from TDesOverflow
	@param aDes modifiable descriptor whose overflow results in the call to this overflow handler.
*/
EXPORT_C void TTimerLogger::Overflow(TDes& aDes)
	{
	#if defined (__LOG_PERFORMANCE) && !defined (_DEBUG)
	// Overflow has occured - end log line with '...'
	_LIT(KErrOverflowMsg, "...");
	if( aDes.MaxLength() >= KErrOverflowMsg().Length() + aDes.Length() )
		aDes.Append(KErrOverflowMsg);
	#endif
	}

/**
 Displays the test case name
 @param aComment Name of API.
*/
EXPORT_C void TTimerLogger::TestName(const TDesC& aComment)
	{
	#if defined (__LOG_PERFORMANCE) && !defined (_DEBUG)
    LogIt(aComment);
	#endif
	}
