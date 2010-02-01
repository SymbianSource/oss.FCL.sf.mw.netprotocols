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

#if !defined(__WAPTESTUTILS_H__)
#define __WAPTESTUTILS_H__

#include <e32base.h>
#include <e32test.h>
#include <f32file.h>
#include <flogger.h>

#if !defined(__LOGGING)
	#define __LOGGING
#endif

#include "waplog.h"
#include "logdef.h"

//	Constants
const TUid KUidWapTestUtils = {0x10005188};
const TInt KMaxUserEntrySize = 50;

//Test Harness Logging

#define KLogsDir				_L("c:\\logs\\")
#define KWapLogsDirName			_L("WapTest")

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


class CWapTestHarness : public CBase
/**
	Test harness providing logging features
	@publishedAll
	@deprecated
*/

	{
public:
	IMPORT_C static CWapTestHarness* NewLC(const TDesC& aTitle);
	IMPORT_C static CWapTestHarness* NewL(const TDesC& aTitle);
	~CWapTestHarness();

	IMPORT_C void StartTestL(const TDesC& aName);
	IMPORT_C void NextStep(const TDesC& aStepName);
	IMPORT_C void EndTest(TInt aErrorCode);
	IMPORT_C void LogIt(TRefByValue<const TDesC> aFmt, ...);
	IMPORT_C void operator()(TInt aResult,TInt aLineNum);
	IMPORT_C void operator()(TInt aResult);
	IMPORT_C void PressAnyKey();
	IMPORT_C void DumpData(HBufC8& aData, TBool logIt = EFalse);
	IMPORT_C void GetAnEntry(const TDesC & ourPrompt, TDes & currentstring);
	IMPORT_C TInt GetSelection(const TDesC& ourPrompt, const TDesC& validChoices);
	IMPORT_C void SetScript(RFile& scriptFile);
	inline void DoResourceLeakTest(TBool aValue);
	inline RTest& Test();

	inline static void DefaultLogFileName(TDes& aFileName);
private:
	CWapTestHarness(const TDesC& aTitle);
	void ConstructL(const TDesC& aTitle);
	void Panic(TInt aPanic);
	void TestHarnessFailed();
	void TestHarnessComplete();
	void ResourceLeakTest();
	inline void CreateFlogger(const TDesC& aFileName, TInt aShowDate=ETrue, TInt aShowTime=ETrue);
	inline void WriteComment(const TDesC& aComment);
	TInt ReadLineFromScript(TDes& aBuffer);

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
	RTest iTest;
	RFileLogger iFlogger;
	RFile* iScriptFile;
	__DECLARE_LOG;	//	Does CLogClient* iLogPtr;
	TInt iTestCount;
	CArrayPtrFlat<CTestInfo>* iFailedTests;
	TTime iStartTime;
	TBool iCanStartTest;
	TInt iStepNumber;
	TInt iStartHandleCount;
	TBool iDoResourceLeakTest;
	TBool iScriptRunning;
	};

class CWapTestHarness::CTestInfo : public CBase

/**
	Holds test number and name
	@publishedAll
	@deprecated
*/
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

#include <waptestutils.inl>

#endif	//	__WAPTESTUTILS_H__
