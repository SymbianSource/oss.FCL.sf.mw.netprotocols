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

#define KHttpLogsDirName			_L("http")
const TInt KTestMaxLogLineLength = 128;

inline RTest& CHTTPTestUtils::Test()
//
//	Return iTest
	{
	return iTest;
	}

inline void CHTTPTestUtils::DoResourceLeakTest(TBool aValue)
//
// Turn on/off the resource handle leak test
	{
	iDoResourceLeakTest = aValue;
	}

inline void CHTTPTestUtils::DefaultLogFileName(TDes& aFileName)
//
//	Sets aFilename to "KLogsDir\KHttpLogsDirName\<TEST HARNESS NAME>.<PLATFORM>.<VARIANT>.log"
	{
	aFileName.Copy(RProcess().FileName());
	TInt start = aFileName.LocateReverse('\\');
	TInt end = aFileName.LocateReverse('.');
	aFileName = aFileName.Mid(start + 1, end - start - 1);

	// create the log filename
	aFileName.Insert(0, _L("\\"));
	aFileName.Insert(0, KHttpLogsDirName);
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

inline void CHTTPTestUtils::WriteComment(const TDesC& aComment)
//
//	Writes aComment to test log file, logging file and test harness
	{
	if (!iSilent)
		iTest.Printf(_L("%S\n"), &aComment);

	__LOG(aComment);

	// If the flogger connection was made successfully
	if(iTestSummaryLogger.Handle() != 0)
		{
		TPtrC line;
		line.Set(aComment);

		while (line.Length() > KTestMaxLogLineLength)
			{
			iTestSummaryLogger.Write(line.Left(KTestMaxLogLineLength));
			line.Set(line.Right(line.Length() - KTestMaxLogLineLength));
			}
		iTestSummaryLogger.Write(line.Left(line.Length()));
		}
	}


inline TBool CHTTPTestUtils::IsSilent() const
//
//	Returns errorcode
	{
	return iSilent;
	}


inline void CHTTPTestUtils::SetSilent(TBool aSilent)
//
//	Sets silence on or off
	{
	iSilent = aSilent;
	}


inline TPtr CHTTPTestUtils::CTestInfo::Name() const
//
//	Returns name
	{
	return iName->Des();
	}

inline TInt CHTTPTestUtils::CTestInfo::Number() const
//
//	Returns number
	{
	return iNumber;
	}

inline TInt CHTTPTestUtils::CTestInfo::ErrorCode() const
//
//	Returns errorcode
	{
	return iErrorCode;
	}
