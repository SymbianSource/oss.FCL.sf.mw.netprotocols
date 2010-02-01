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

// System includes
#include <f32file.h> // for TParse, in Flogger stuff
#include <http/framework/httplogger.h> // Class signature

// Local includes

#define KLogsDir				_L("c:\\logs\\")
#define KHttpLogsDirName			_L("http")
#define KTestHeader				_L("HTTP Log")
#define KTestCommentPrepend		_L("\t")

#define KMaxLogLineLength 128

EXPORT_C
THttpLogger::~THttpLogger()
	{
#if defined (_DEBUG)
	if(iLogger)
		{
		iLogger->Close();
		delete iLogger;
		}
#endif
	}

EXPORT_C
#if defined (_DEBUG)
/**
Function to create log file
@param aFileName The descriptor that holds the name of the log file to be created.
@param aShowDate If set to 1, the date of creation of the log file is recorded.
@param aShowTime If set to 1, the time of creation of the log file is recorded.
*/
void THttpLogger::CreateFlogger(const TDesC& aFileName, TInt aShowDate, TInt aShowTime)
//
//	Create log file in directory KLogsdir\KWapLogsDirName - Note: ingore Drive and Path of aFilename
	{
	if(!iLogger)
		{
		iLogger = new RFileLogger;
		}

	if(iLogger)
		{
		TInt error = iLogger->Connect();
		if(error == KErrNone)
			{
			TParse p;
			p.Set(aFileName, NULL, NULL);
			iLogger->CreateLog(KHttpLogsDirName, p.NameAndExt(), EFileLoggingModeOverwrite);
			iLogger->SetDateAndTime(aShowDate, aShowTime);
			iLogger->Write(KTestHeader);
			}
		else
			User::InfoPrint(_L("Flogger connect failed"));
		}
	else
		User::InfoPrint(_L("Flogger create failed"));
	}
#elif !defined (_DEBUG)
void THttpLogger::CreateFlogger(const TDesC& , TInt , TInt ) {}
#endif

EXPORT_C
#if defined (_DEBUG)
/**
Function to print inputed log string on front end emulator and to write it to the log file.
@param aFmt The descriptor containing the format string. The TRefByValue class provides a constructor which takes a TDesC type.
@see TRefByValue
@see TDesC
*/
void THttpLogger::LogIt(TRefByValue<const TDesC> aFmt, ...)
//
//	Messages to the front end emulator and to the log file
	{
	VA_LIST list;
	VA_START(list,aFmt);

	TBuf<KMaxFileName> buf;
	buf.AppendFormatList(aFmt,list,this);
	VA_END(list);

	WriteComment(buf);
	}
#elif !defined (_DEBUG)
void THttpLogger::LogIt(TRefByValue<const TDesC> , ...) {}
#endif



#if defined (_DEBUG)
/**
Function to write a comment string to test log file, logging file and test harness.
@param aComment The descriptor containing the comment string
*/
EXPORT_C void THttpLogger::WriteComment(const TDesC& aComment)
//
//	Writes aComment to test log file, logging file and test harness
	{
	if(iLogger)
		{
		// If connection to flogger was made
		if(iLogger->Handle() != 0)
			{
			TPtrC line;
			line.Set(aComment);

			while (line.Length() > KMaxLogLineLength)
				{
				iLogger->Write(line.Left(KMaxLogLineLength));
				line.Set(line.Right(line.Length() - KMaxLogLineLength));
				}
			
			iLogger->Write(line.Left(line.Length()));
			}
		}
	}

#elif !defined (_DEBUG)
EXPORT_C void THttpLogger::WriteComment(const TDesC& ) {}
#endif



EXPORT_C
#if defined (_DEBUG)
/**
Function to do a formatted dump of binary data.
@param aData The descriptor that holds the binary data
*/
void THttpLogger::DumpIt(const TDesC8& aData)
//Do a formatted dump of binary data
	{
	// Iterate the supplied block of data in blocks of cols=80 bytes
	const TInt cols=16;
	TInt pos = 0;
	TBuf<KMaxLogLineLength> logLine;
	TBuf<KMaxLogLineLength> anEntry;
	while (pos < aData.Length())
		{
		//start-line exadecimal( a 4 digit number)
		anEntry.Format(TRefByValue<const TDesC>_L("%04x : "), pos);
		logLine.Append(anEntry.Left(KMaxLogLineLength));

		// Hex output
		TInt offset;
		for (offset = 0; offset < cols; offset++)
			{
			if (pos + offset < aData.Length())
				{
				TInt nextByte = aData[pos + offset];
				anEntry.Format(TRefByValue<const TDesC>_L("%02x "), nextByte);
				logLine.Append(anEntry);
				}
			else
				{
				//fill the remaining spaces with blanks untill the cols-th Hex number 
				anEntry.Format(TRefByValue<const TDesC>_L("   "));
				logLine.Append(anEntry);
				}
			}
			anEntry.Format(TRefByValue<const TDesC>_L(": "));
			logLine.Append(anEntry);

		// Char output
		for (offset = 0; offset < cols; offset++)
			{
			if (pos + offset < aData.Length())
				{
				TInt nextByte = aData[pos + offset];
				if ((nextByte >= 32) && (nextByte <= 127))
					{
					anEntry.Format(TRefByValue<const TDesC>_L("%c"), nextByte);
					logLine.Append(anEntry);
					}
				else
					{
					anEntry.Format(TRefByValue<const TDesC>_L("."));
					logLine.Append(anEntry);
					}
				}
			else
				{
				anEntry.Format(TRefByValue<const TDesC>_L(" "));
				logLine.Append(anEntry);
				}
			}
			LogIt(TRefByValue<const TDesC>_L("%S\n"), &logLine);	
			logLine.Zero();

		// Advance to next  byte segment (1 seg= cols)
		pos += cols;
		}
	}
#elif !defined (_DEBUG)
void THttpLogger::DumpIt(const TDesC8& ) {}
#endif

EXPORT_C
#if defined (_DEBUG)
/**
Function to do a write of the supplied data, literally where possible.
@param aData The descriptor that holds the supplied data.
*/
void THttpLogger::WriteComment(const TDesC8& aData)
//Do a write of the supplied data, literally where possible
	{
	if(iLogger)
		{
		// If the connection to flogger was made
		if(iLogger->Handle() != 0)
			{
			TPtrC8 line;
			line.Set(aData);
			while (line.Length() > KMaxLogLineLength)
				{
				iLogger->Write(line.Left(KMaxLogLineLength));
				line.Set(line.Right(line.Length() - KMaxLogLineLength));
				}
			
			iLogger->Write(line.Left(line.Length()));
			}
		}
	}
#elif !defined (_DEBUG)
void THttpLogger::WriteComment(const TDesC8& ) {}
#endif

EXPORT_C
#ifdef _DEBUG
void THttpLogger::Overflow(TDes& aDes)
	{
	// Overflow has occured - end log line with '...'
	_LIT(KErrOverflowMsg, "...");
	if( aDes.MaxLength() >= KErrOverflowMsg().Length() + aDes.Length() )
		aDes.Append(KErrOverflowMsg);
	}
#else
void THttpLogger::Overflow(TDes& /*aDes*/)
	{
	}
#endif
