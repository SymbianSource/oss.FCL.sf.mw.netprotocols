// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

inline RTest& CWapTestHarness::Test()
//
//	Return iTest
	{
	return iTest;
	}

inline void CWapTestHarness::DoResourceLeakTest(TBool aValue)
//
// Turn on/off the resource handle leak test
	{
	iDoResourceLeakTest = aValue;
	}

inline void CWapTestHarness::DefaultLogFileName(TDes& aFileName)
//
//	Sets aFilename to "KLogsDir\LWapLogsDirName\<TEST HARNESS NAME>.<PLATFORM>.<VARIANT>.log"
	{
	aFileName.Copy(RProcess().FileName());
	TInt start = aFileName.LocateReverse('\\');
	TInt end = aFileName.LocateReverse('.');
	aFileName = aFileName.Mid(start + 1, end - start - 1);

	// create the log filename
	aFileName.Insert(0, _L("\\"));
	aFileName.Insert(0, KWapLogsDirName);
	aFileName.Insert(0, KLogsDir);

#if defined(__WINS__)
	aFileName.Append(_L(".WINS."));
#else
	aFileName.Append(_L(".MARM."));
#endif

#if defined(_DEBUG)
	aFileName.Append(_L("DEB."));
#else
	aFileName.Append(_L("REL."));
#endif

	aFileName.Append(_L("LOG"));
	}

inline void CWapTestHarness::WriteComment(const TDesC& aComment)
//
//	Writes aComment to test log file, logging file and test harness
	{
	if(aComment.Length() < (KMaxFileName - 2))
		{
		iTest.Printf(_L("%S\n"), &aComment);
		iFlogger.Write(aComment);
		}

#if defined (__LOGGING)
	__ASSERT_DEBUG(iLogPtr,Panic(EBadCLogPtr));
	__LOG (aComment);
#endif
	}

inline void CWapTestHarness::CreateFlogger(const TDesC& aFileName, TInt aShowDate, TInt aShowTime)
//
//	Create log file in directory KLogsdir\KWapLogsDirName - Note: ingore Drive and Path of aFilename
	{
	iFlogger.Connect();
	TParse p;
	p.Set(aFileName, NULL, NULL);
	iFlogger.CreateLog(KWapLogsDirName, p.NameAndExt(), EFileLoggingModeOverwrite);
	iFlogger.SetDateAndTime(aShowDate, aShowTime);
	iFlogger.Write(KTestHeader);
	}

inline TPtr CWapTestHarness::CTestInfo::Name() const
//
//	Returns name
	{
	return iName->Des();
	}

inline TInt CWapTestHarness::CTestInfo::Number() const
//
//	Returns number
	{
	return iNumber;
	}

inline TInt CWapTestHarness::CTestInfo::ErrorCode() const
//
//	Returns errorcode
	{
	return iErrorCode;
	}
