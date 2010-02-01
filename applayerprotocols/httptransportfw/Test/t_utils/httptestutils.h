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

#if !defined(__HTTPTESTUTILS_H__)
#define __HTTPTESTUTILS_H__

#include <e32base.h>
#include <e32test.h>
#include <f32file.h>


#include <http/framework/httplogger.h>

//	Constants
const TUid KUidWapTestUtils = {0x10005188};
const TInt KMaxUserEntrySize = 128;

//Test Harness Logging

#define KLogsDir				_L("c:\\logs\\")
#define KTestHeader				_L("Test Results")
#define KTestStarting			_L("Test %d Starting")
#define KTestStartingWithDesc	_L("Test %d Starting (%S)")
#define KNextTestStepWithDesc	_L("Test %d Step %d (%S)")
#define KTestPassed				_L("Test %d OK")
#define KTestFailed				_L("Test %d FAILED (error = %d)")
#define KTestHarnessCompleted	_L("Tests Completed Ok")
#define KTestHarnessFailed		_L("Test FAILED (%d failed test(s))")
#define KTestFailInfo			_L("Test %d : %S failed with Error %d") 
#define KTestCommentPrepend		_L("\t")
#define KRTestFailed			_L("RTEST: FAIL : Failed with error %d\n")

//forwd decl
class CHttpTestBase;

//##ModelId=3C4C4C420207
class CHTTPTestUtils : public CBase, public TDesOverflow
//
//	Test harness providing logging features
	{
public:
	//##ModelId=3C4C4C430136
	IMPORT_C static CHTTPTestUtils* NewLC(const TDesC& aTitle);
	//##ModelId=3C4C4C43012C
	IMPORT_C static CHTTPTestUtils* NewL(const TDesC& aTitle);
	//##ModelId=3C4C4C430122
	~CHTTPTestUtils();
	//##ModelId=3C4C4C430118
	IMPORT_C void JustStartTestL(const TDesC& aName);
	//##ModelId=3C4C4C430104
	IMPORT_C void StartTestL(const TDesC& aName);
	//##ModelId=3C4C4C4300FA
	IMPORT_C void JustEndTest();
	//##ModelId=3C4C4C4300E6
	IMPORT_C void NextStep(const TDesC& aStepName);
	//##ModelId=3C4C4C4300DC
	IMPORT_C void EndTest(TInt aErrorCode);
	//##ModelId=3C4C4C4300BE
	IMPORT_C void LogIt(TRefByValue<const TDesC> aFmt, ...);
	//##ModelId=3C4C4C43008D
	IMPORT_C void operator()(TInt aResult,TInt aLineNum);
	//##ModelId=3C4C4C4300B4
	IMPORT_C void operator()(TInt aResult);
	//##ModelId=3C4C4C43008C
	IMPORT_C void PressAnyKey();
	//##ModelId=3C4C4C43006E
	IMPORT_C void DumpData(const TDesC8& aData, TBool logIt = EFalse);
	//##ModelId=3C4C4C430050
	IMPORT_C void GetAnEntry(const TDesC & ourPrompt, TDes & currentstring);
	//##ModelId=3C4C4C430032
	IMPORT_C TInt GetSelection(const TDesC& ourPrompt, const TDesC& validChoices);
	//##ModelId=3C4C4C43001E
	IMPORT_C void SetScript(RFile& scriptFile);
	//##ModelId=3C4C4C43000B
	inline void DoResourceLeakTest(TBool aValue);
	//##ModelId=3C4C4C43000A
	inline RTest& Test();

	//##ModelId=3C4C4C4203DF
	inline static void DefaultLogFileName(TDes& aFileName);

	//##ModelId=3C4C4C4203DE
	IMPORT_C static void InitCommsL();
	//##ModelId=3C4C4C4203D4
	IMPORT_C static void HoldOpenConnectionL();
	//##ModelId=3C4C4C4203CB
	IMPORT_C static void ReleaseConnectionL();

	//##ModelId=3C4C4C4203CA
	inline TBool IsSilent() const;
	//##ModelId=3C4C4C4203B6
	inline void SetSilent(TBool aSilent);

	//##ModelId=3C4C4C42038E
	IMPORT_C TInt ResolveFile(const TDesC& aComponent, const TDesC& aFileName, TParse& aParseOut);
	//##ModelId=3C4C4C42037A
	IMPORT_C HBufC8* ReadFileLC(const TDesC& aFile);

	IMPORT_C void CleanupFailedTestsArray ();
	IMPORT_C void CreateFailedTestsArrayL ();
private:	// Methods from TDesOverflow

	IMPORT_C virtual void Overflow(TDes& aDes);

private:
	//##ModelId=3C4C4C420366
	CHTTPTestUtils(const TDesC& aTitle);
	//##ModelId=3C4C4C420352
	void ConstructL(const TDesC& aTitle);
	//##ModelId=3C4C4C420348
	void Panic(TInt aPanic);
	//##ModelId=3C4C4C42033E
	void TestHarnessFailed();
	//##ModelId=3C4C4C420335
	void TestHarnessComplete();
	//##ModelId=3C4C4C420334
	void ResourceLeakTest();
	//##ModelId=3C4C4C420320
	inline void WriteComment(const TDesC& aComment);
	//##ModelId=3C4C4C42030C
	TInt ReadLineFromScript(TDes& aBuffer);

	//##ModelId=3C4C4C420302
	void CreateLogFiles(const TDesC& aTestName);

	void Print(const TDesC& aDescriptor);


private:
	//	Helper class to store failed tests
	class CTestInfo;
	enum TPanicCode
		{
		EBadStartTest,
		EBadEndTest,
		EBadCLogPtr
		};
private:
	//##ModelId=3C4C4C4202F8
	RTest iTest;
	//##ModelId=3C4C4C4202E6
	RFs iFs;
	//##ModelId=3C4C4C4202D0
	RFile* iScriptFile;
	__DECLARE_LOG	
	//##ModelId=3C4C4C4202BE
	RFileLogger iTestSummaryLogger;
	//##ModelId=3C4C4C4202B2
	TInt iTestCount;
	//##ModelId=3C4C4C42029F
	CArrayPtrFlat<CTestInfo>* iFailedTests;
	//##ModelId=3C4C4C42027F
	TTime iStartTime;
	//##ModelId=3C4C4C420263
	TBool iCanStartTest;
	//##ModelId=3C4C4C420259
	TInt iStepNumber;
	//##ModelId=3C4C4C42024F
	TInt iStartHandleCount;
	//##ModelId=3C4C4C420243
	TBool iDoResourceLeakTest;
	//##ModelId=3C4C4C420239
	TBool iScriptRunning;
	//##ModelId=3C4C4C420227
	TBool iSilent; // if true logs only to file and not to console
	//##ModelId=3C4C4C42021E
	CTestInfo* iCurrentTest;
	};

class CHTTPTestUtils::CTestInfo : public CBase
//
//	Holds test number and name
	{
public:
	static CTestInfo* NewLC(const TDesC& aName, TInt aNumber, TInt aErrorCode);
	static CTestInfo* NewL(const TDesC& aName, TInt aNumber, TInt aErrorCode);
	~CTestInfo();

	void SetNameL(const TDesC& aName);
	void SetNumber(TInt aNumber);
	void SetErrorCode(TInt aErrorCode);

	inline TPtr Name() const;
	inline TInt Number() const;
	inline TInt ErrorCode() const;
private:
	CTestInfo();
	void ConstructL(const TDesC& aName, TInt aNumber, TInt aErrorCode);
private:
	HBufC* iName;
	TInt iNumber;
	TInt iErrorCode;
	};

#include "httptestutils.inl"

#endif	//	__HTTPTESTUTILS_H__
