// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// Framework includes
#if !defined(__HTTPTESTUTILS_H__)
#include "httptestutils.h"
#endif

#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

#if !defined(__TESTSCRIPTS_H__)
#include "TestScripts.h"
#endif


#include "SingleTrans.h"

// Individual test class includes
#include "t_CLE-4Y5LC9.h"
#include "t_KHN-4Z7DT7.h"
#include "t_INC035621.h"
#include "T_INC053172.h"



// Constants used in this file
enum TPanic
	{
	EFailedToCreateEngine,
	EFailedToReadIniFile
	};

// Name of test harness
_LIT(KHttpRegressionTestTitle,		"HTTP Regression Tests");

// Location of INI file that defines the test cases to be run
_LIT(KHttpRegressionTestCompName,	"regression");
_LIT(KHttpregressionTestscriptName, "RegressionTests.ini");

_LIT(KSettingsIniCompName,			"settings");
_LIT(KSettingsIniName,				"settings.ini");

// Labels for resources in the INI file
_LIT(KRunTestItemName,				"RunTest");
_LIT(KDefaultSection,				"Defaults");
_LIT(KSilenceItemName,				"Silent");
_LIT(KLeakTestCount,				"LeakTestCount");
_LIT(KFirstLeakIteration,			"FirstLeakIteration");
_LIT(KSectionSingleTrans,			"SINGLE_TRANS");
_LIT(KZero,							"0");

// Test names, used as section headings in the .ini file, and any resource 
// names associated with the test
_LIT(KTestKHN4Z7DT7,				"KHN-4Z7DT7");
_LIT(KTestCLE4Y5LC9_1,				"CLE-4Y5LC9_1");
_LIT(KTestCLE4Y5LC9_2,				"CLE-4Y5LC9_2");
_LIT(KTestINC035621,				"INC035621");
_LIT(KTestINC053172, "INC053172");

const TInt KMaxHeapSize = 0x100000;
const TInt KMaxIntDesLen = 10;				// Des length required for displaying max 32 bit int
_LIT(KTestThreadName,						"TestThread");
_LIT(KRtestFailureLabel, 					"RTEST: FAILED due to a %S %d Panic ");


void DoTestsL()
	{	
	TBool runTestDefault = EFalse;

	CTestScheduler* as = new(ELeave) CTestScheduler;
	CleanupStack::PushL(as);
	if (!as)
		User::Panic(_L("Failed to create active scheduler"),KErrNoMemory);
	CActiveScheduler::Install(as); // Install active scheduler
	
	// Initialise Comms, required in a minimal console environment
	CHTTPTestUtils::InitCommsL();

	// Force a connection to be held open keep NT RAS connected...
	CHTTPTestUtils::HoldOpenConnectionL();
	
	// Create the test engine
	CHttpTestEngine* engine=NULL;
	TRAPD(err,engine = CHttpTestEngine::NewL(KHttpRegressionTestTitle(), EFalse));
	if (err != KErrNone)
		User::Panic(_L("T_HTTPREGRES"),EFailedToCreateEngine);
	CleanupStack::PushL(engine);

	engine->Utils().LogIt(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-FRAMEWORK-T_HTTPREGRESSION-0001  "));
	
	// Open the INI file to initialise the tests we're going to run
	CScriptFile* iniFile = NULL;
	TRAP(err, iniFile = CScriptFile::NewL(engine->Utils(), KHttpRegressionTestCompName, KHttpregressionTestscriptName()));
	if (err != KErrNone)
		User::Panic(_L("T_HTTPREGRES"),EFailedToReadIniFile);
	CleanupStack::PushL(iniFile);

	// Open the settings.ini file which has the server IP/Name that would
	// be used by the tests.
	CScriptFile* iniSettingsFile = NULL;
	TRAP(err, iniSettingsFile = CScriptFile::NewL(engine->Utils(), KSettingsIniCompName, KSettingsIniName()));
	if (err != KErrNone)
		{
		User::Panic(KHttpRegressionTestCompName(),err);
		}
	CleanupStack::PushL(iniSettingsFile);

	// Read the default silent setting for the tests
	engine->Utils().SetSilent(iniFile->ItemValue(KDefaultSection, KSilenceItemName, EFalse));

	// Create the tests we're going to run.  Ownership is transferred to the test engine
	RPointerArray<CHttpTestBase> regressionTests;
	CleanupClosePushL(regressionTests);


// READ DEFAULTS
	CArrayPtrFlat<CScriptSection>& sections=iniFile->Sections();
	TInt sectionsCount = sections.Count();
	CScriptSection* section = NULL;
	
	for (TInt ii=0;ii<sectionsCount;++ii)
		{
		section = sections[ii];
	
		// DEFAULTS
		if (section->SectionName().CompareF(KDefaultSection) == 0)
			{
			if (iniFile->Section(ii).ItemL(KSilenceItemName).Value().CompareF(KZero) == 0)
   				engine->SetSilent(EFalse);
 			else  	
				engine->SetSilent(ETrue);
 			if (iniFile->Section(ii).ItemL(KRunTestItemName).Value().CompareF(KZero) != 0)
				runTestDefault = ETrue;
			}

		// SCRIPTED SINGLE_TRANS TESTS - These are purely done by a single transaction
	
		if (section->SectionName().CompareF(KSectionSingleTrans) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(ii).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
 				runTest = !(iniFile->Section(ii).Item(0).Value().CompareF(KZero) == 0);
 			if (runTest)
   				{
   				CHttpTestBase* singleTrans = CHttpTestSingleTrans::NewL(*iniFile,iniSettingsFile,ii);
				CleanupStack::PushL(singleTrans);
   				regressionTests.Append(singleTrans);
   				}
			}
		}

// NON SCRIPTED TESTS - These are done by separate classes


	// Post body rewind tests
	if (iniFile->ItemValue(KTestCLE4Y5LC9_1(), KRunTestItemName(), ETrue))
		{
		CTestFixCLE4Y5LC9_1* CLE4Y5LC9_1 = CTestFixCLE4Y5LC9_1::NewLC(iniSettingsFile);
		regressionTests.Append(CLE4Y5LC9_1);
		
		TInt numLeakTests = iniFile->ItemValue(KTestCLE4Y5LC9_1(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestCLE4Y5LC9_1(), KFirstLeakIteration(), 0);
		CLE4Y5LC9_1->SetLeakTests(numLeakTests, firstLeakIter);
		}

	if (iniFile->ItemValue(KTestCLE4Y5LC9_2(), KRunTestItemName(), ETrue))
		{
		CTestFixCLE4Y5LC9_2* CLE4Y5LC9_2 = CTestFixCLE4Y5LC9_2::NewLC(iniSettingsFile);
		regressionTests.Append(CLE4Y5LC9_2);
		
		TInt numLeakTests = iniFile->ItemValue(KTestCLE4Y5LC9_2(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestCLE4Y5LC9_2(), KFirstLeakIteration(), 0);
		CLE4Y5LC9_2->SetLeakTests(numLeakTests, firstLeakIter);
		}

	if (iniFile->ItemValue(KTestKHN4Z7DT7(), KRunTestItemName(), ETrue))
		{
		CTestFixKHN4Z7DT7* KHN4Z7DT7 = CTestFixKHN4Z7DT7::NewLC(iniSettingsFile);
		regressionTests.Append(KHN4Z7DT7);
		
		TInt numLeakTests = iniFile->ItemValue(KTestKHN4Z7DT7(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestKHN4Z7DT7(), KFirstLeakIteration(), 0);
		KHN4Z7DT7->SetLeakTests(numLeakTests, firstLeakIter);
		}

	if (iniFile->ItemValue(KTestINC035621(), KRunTestItemName(), ETrue))
		{
		CTestFixINC035621* INC035621 = CTestFixINC035621::NewLC(iniSettingsFile);
		regressionTests.Append(INC035621);
		
		TInt numLeakTests = iniFile->ItemValue(KTestINC035621(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestINC035621(), KFirstLeakIteration(), 0);
		INC035621->SetLeakTests(numLeakTests, firstLeakIter);
		}

	if (iniFile->ItemValue(KTestINC053172(), KRunTestItemName(), ETrue))
		{
		CTestFixINC053172* INC053172 = CTestFixINC053172::NewLC();
		regressionTests.Append(INC053172);
		
		TInt numLeakTests = iniFile->ItemValue(KTestINC053172(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestINC053172(), KFirstLeakIteration(), 0);
		INC053172->SetLeakTests(numLeakTests, firstLeakIter);
		}

	// Set the test set based on the menu choices
	engine->ConfigureTestSet(regressionTests);

	// Pop the tests which are now owned by the test engine
	TInt numTestsToPop = regressionTests.Count();
	CleanupStack::Pop(numTestsToPop); // multiCancelTest
	CleanupStack::Pop(); // regressionTests, the array itself

	// All the action happens within the main event loop.
	CActiveScheduler::Start();

	// Returned from start so shutdown. Turn off test engine silence to get a summary report.
	engine->Utils().SetSilent(EFalse);
	engine->Utils().LogIt(_L("\nEnd of tests"));
	TInt pass, fail;
	engine->TestSummary(pass, fail);
	engine->Utils().LogIt(_L("\nTest Summary: %d passed, %d failed.\n"), pass, fail);
	//engine->PressAnyKey();
	
	// Release the held open connection
	CHTTPTestUtils::ReleaseConnectionL();	

	CleanupStack::PopAndDestroy(4, as);// inifile, engine, iniSettingsFile
	}

TInt TestEntryPoint(TAny* /*aParam*/)
    {
__UHEAP_MARK;

	// Install exception handler
	CTrapCleanup* theCleanup = CTrapCleanup::New(); 
	TRAP_IGNORE(DoTestsL());
	delete theCleanup;

__UHEAP_MARKEND;
	User::Heap().Check();
    return(KErrNone);
	}
	
void LogOnPanicL(RThread& aTestThread)	
	{
	CActiveScheduler* as = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(as);
	if (!as)
		{
		User::Panic(_L("Failed to create active scheduler"),KErrNoMemory);
		}
	CActiveScheduler::Install(as); // Install active scheduler
		
	// Initialise Comms, required in a minimal console environment
	CHTTPTestUtils::InitCommsL();

	// Force a connection to be held open keep NT RAS connected...
	CHTTPTestUtils::HoldOpenConnectionL();
	
	// Create the test engine
	CHttpTestEngine* engine=NULL;
	TRAPD(err,engine = CHttpTestEngine::NewL(KHttpRegressionTestTitle(), EFalse));
	if (err != KErrNone)
		{
		User::Panic(_L("T_PIPELINING"),err);
		}
	CleanupStack::PushL(engine);
	
	// Create log message
	TExitCategoryName panicName = aTestThread.ExitCategory();
	TInt panicNumber = aTestThread.ExitReason();
	HBufC* logMessage = HBufC::NewLC(KRtestFailureLabel().Length() + 
								   panicName.Length() +	
								   KMaxIntDesLen);
	TPtr logMessagePtr = logMessage->Des();
	logMessagePtr.Format(KRtestFailureLabel, &panicName, panicNumber);
			
	engine->Utils().LogIt(logMessagePtr);
	
	// Panic here so that nothing else is written to the log file
	User::Panic(panicName, panicNumber);
	
	// The code below provided for completenes, but should never be reached
	
	// Release the held open connection
	CHTTPTestUtils::ReleaseConnectionL();	
	
	CleanupStack::PopAndDestroy(3, as); //as, engine, logMessage
	}
		
// main loop
//
GLDEF_C TInt E32Main()
    {  
	__UHEAP_MARK;  
	CTrapCleanup* theCleanup = CTrapCleanup::New(); // Install exception handler  
	
	// Create a new thread for running tests in
	RThread testThread;
	TInt err = testThread.Create(KTestThreadName, TestEntryPoint, KDefaultStackSize,  
							 KMinHeapSize, KMaxHeapSize, NULL);
	if(err == KErrNone)
		{
		testThread.Resume();	
		}
	else
		{
		return err;
		}
	
	// Kick off test thread and wait for it to exit
	TRequestStatus tStat;
	testThread.Logon(tStat);
	User::WaitForRequest(tStat);
	
	// Log if paniced
	if(testThread.ExitType() == EExitPanic)
		{
		TRAP_IGNORE(LogOnPanicL(testThread));	
		}
	
	testThread.Close();
	delete theCleanup;		
	__UHEAP_MARKEND;		
	return KErrNone;
	}
		
