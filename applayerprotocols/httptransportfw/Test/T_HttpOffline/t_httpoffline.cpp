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

// Individual test class includes
#if !defined(__T_TEXTMODEOFFLINETESTS_H__)
#include "T_TextModeOfflineTests.h"
#endif

#include "t_formencoder.h"
#include "t_hdrcodec.h"
#include "t_core.h"

// @todo rewrite this test properly to work with pipelining changes
//#include "t_respparse.h"

#include "t_filters.h"
//#include "t_httphdr.h"
#include "ctesthttpheadercodec.h"

// @todo rewrite this test properly to work with pipelining changes
//#include "t_textmodeheadersfilter.h"

#include "t_wsptrhndstub.h"


// Constants used in this file
enum TPanic
	{
	EFailedToCreateEngine,
	EFailedToReadIniFile
	};

// Global/constant declarations
_LIT(KHttpOfflineTestTitle, "HTTP Client Offline Unit Test Harness");

// Location of INI file that defines the test cases to be run
_LIT(KHttpOfflineTestCompName, "offline");
_LIT(KHttpOfflineTestScriptName, "UnitTests.ini");

// Labels for resources in the INI file
_LIT(KRunTestItemName, "RunTest");
_LIT(KDefaultSection, "All");
_LIT(KSilenceItemName, "Silent");
_LIT(KLeakTestCount, "LeakTestCount");
_LIT(KFirstLeakIteration, "FirstLeakIteration");


// Test names, used as section headings in the .ini file, and any resource names associated with the test
_LIT(KTestHeaders,				"Headers");
_LIT(KTestFormEncoder,			"FormEncoder");
_LIT(KTestHdrCodec,				"HdrCodec");
_LIT(KTestCore,					"Core");
_LIT(KTestFilter,				"Filter");

//_LIT(KTextModeHeaderFilter,		"TextModeFilter");
//_LIT(KTestRespParsSingleBuff,	"ResponseParser_SingleBuffer");
//_LIT(KTestRespParsSplitStatus,	"ResponseParser_SplitStatus");
//_LIT(KTestRespParsMultiBuff,	"ResponseParser_MultiBuffersSimpleBody");
//_LIT(KTestRespParsMultiBody,	"ResponseParser_MultiPartNormalBody");
//_LIT(KTestRespParsChunkedBody,	"ResponseParser_MultiPartChunkedBody");
//_LIT(KTestRespParsApacheHead,	"ResponseParser_ApacheHeadResponse");
//_LIT(KTestRespParsIisHead,		"ResponseParser_IISHeadResponse");
//_LIT(KTestLineBuffOverrun,		"ResponseParser_LineBuffOverrun");
//_LIT(KTestCycleRespBuffer,		"ResponseParser_CycleRespBuffer");
//_LIT(KMinBuffSizeItemName,		"MinBuffSize");
//_LIT(KBuffSizeIncItemName,		"BuffSizeInc");

//_LIT(KWSPTransportHandlerStub,	"WSPTransportHandlerStub");

const TInt KMaxHeapSize = 0x100000;
const TInt KMaxIntDesLen = 10;				// Des length required for displaying max 32 bit int
_LIT(KTestThreadName,						"TestThread");
_LIT(KRtestFailureLabel, 					"RTEST: FAILED due to a %S %d Panic ");


// A Fake version of chttpmanager

#include <e32std.h>
#include <e32base.h>
#include "CHTTPManager.h"


void DoTestsL()
	{	
	CTestScheduler* as = new(ELeave) CTestScheduler;
	CleanupStack::PushL(as);
	if (!as)
		User::Panic(_L("Failed to create active scheduler"),KErrNoMemory);
	CActiveScheduler::Install(as); // Install active scheduler

	// Create the test engine
	CHttpTestEngine* engine=NULL;
	TRAPD(err,engine = CHttpTestEngine::NewL(KHttpOfflineTestTitle(), EFalse));
	if (err != KErrNone)
		User::Panic(_L("T_HTTPOFFLIN"),EFailedToCreateEngine);
	CleanupStack::PushL(engine);

	engine->Utils().LogIt(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-FRAMEWORK-T_HTTPOFFLINE-0001 "));
	
	// Open the INI file to initialise the tests we're going to run
	CScriptFile* iniFile = NULL;
	TRAP(err, iniFile = CScriptFile::NewL(engine->Utils(), KHttpOfflineTestCompName, KHttpOfflineTestScriptName()));
	if (err != KErrNone)
		{
		engine->Utils().Test().Printf(_L("\n\nExpected to find INI file %S in C:\\HttpTest\\%S\n"),
										&KHttpOfflineTestScriptName(), &KHttpOfflineTestCompName());
		engine->PressAnyKey();
		User::Panic(_L("T_HTTPOFFLIN"),EFailedToReadIniFile);
		}
	CleanupStack::PushL(iniFile);

	// Read the default silent setting for the tests
	engine->Utils().SetSilent(iniFile->ItemValue(KDefaultSection, KSilenceItemName, EFalse));

	// Create the tests we're going to run.  Ownership is transferred to the test engine
	RPointerArray<CHttpTestBase> offlineTests;
	CleanupClosePushL(offlineTests);

	// Header fields, parts and parameters tests
	if (iniFile->ItemValue(KTestHeaders(), KRunTestItemName(), ETrue))
		{
		CTestHttpHeaderCodec* httpHdrTest = CTestHttpHeaderCodec::NewLC();
		offlineTests.Append(httpHdrTest);
		
		TInt numLeakTests = iniFile->ItemValue(KTestHeaders(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestHeaders(), KFirstLeakIteration(), 0);
		httpHdrTest->SetLeakTests(numLeakTests, firstLeakIter);
		}

	// Form encoder tests
	if (iniFile->ItemValue(KTestFormEncoder(), KRunTestItemName(), ETrue))
		{
		CFormEncoderTest* formEncTest = new(ELeave)CFormEncoderTest;
		CleanupStack::PushL(formEncTest);
		offlineTests.Append(formEncTest);

		TInt numLeakTests = iniFile->ItemValue(KTestFormEncoder(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestHeaders(), KFirstLeakIteration(), 0);
		formEncTest->SetLeakTests(numLeakTests, firstLeakIter);
		}

	// Header codec tests
	if (iniFile->ItemValue(KTestHdrCodec(), KRunTestItemName(), ETrue))
		{
		CHttpHdrCodecTest* hdrCodecTest = CHttpHdrCodecTest::NewLC();
		offlineTests.Append(hdrCodecTest);

		TInt numLeakTests = iniFile->ItemValue(KTestHdrCodec(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestHdrCodec(), KFirstLeakIteration(), 0);
		hdrCodecTest->SetLeakTests(numLeakTests, firstLeakIter);
		}

	// Core tests
	if (iniFile->ItemValue(KTestCore(), KRunTestItemName(), ETrue))
		{
		CHTTPCoreTest* coreTest = CHTTPCoreTest::NewLC();
		offlineTests.Append(coreTest);
		TInt numLeakTests = iniFile->ItemValue(KTestCore(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestCore(), KFirstLeakIteration(), 0);
		coreTest->SetLeakTests(numLeakTests, firstLeakIter);
		}

	// Filter mechanism tests
	if (iniFile->ItemValue(KTestFilter(), KRunTestItemName(), ETrue))
		{
		CHTTPFilterTest* filterTest = CHTTPFilterTest::NewL();
		CleanupStack::PushL(filterTest);
		offlineTests.Append(filterTest);
		TInt numLeakTests = iniFile->ItemValue(KTestFilter(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestFilter(), KFirstLeakIteration(), 0);
		filterTest->SetLeakTests(numLeakTests, firstLeakIter);
		}

/* @todo rewrite this test properly to work with pipelining changes

	// Textmode headers filter test
	if (iniFile->ItemValue(KTextModeHeaderFilter(), KRunTestItemName(), ETrue))
		{
		CTextModeHeadersFilterTest* textmodeheadersfiltertest = CTextModeHeadersFilterTest::NewLC();
		offlineTests.Append(textmodeheadersfiltertest);

		TInt numLeakTests = iniFile->ItemValue(KTextModeHeaderFilter(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTextModeHeaderFilter(), KFirstLeakIteration(), 0);
		textmodeheadersfiltertest->SetLeakTests(numLeakTests, firstLeakIter);
		}
*/

/* @todo rewrite this test properly to work with pipelining changes

	// Response Parser tests
	if (iniFile->ItemValue(KTestRespParsSingleBuff(), KRunTestItemName(), ETrue))
		{
		CRespParsTestSingleBuff* respParserTest = CRespParsTestSingleBuff::NewLC();
		offlineTests.Append(respParserTest);

		TInt numLeakTests = iniFile->ItemValue(KTestRespParsSingleBuff(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestRespParsSingleBuff(), KFirstLeakIteration(), 0);
		respParserTest->SetLeakTests(numLeakTests, firstLeakIter);
		}
	if (iniFile->ItemValue(KTestRespParsSplitStatus(), KRunTestItemName(), ETrue))
		{
		CRespParsTestSplitStatusLine* respParserTest = CRespParsTestSplitStatusLine::NewLC();
		offlineTests.Append(respParserTest);

		TInt numLeakTests = iniFile->ItemValue(KTestRespParsSplitStatus(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestRespParsSplitStatus(), KFirstLeakIteration(), 0);
		respParserTest->SetLeakTests(numLeakTests, firstLeakIter);
		}
	if (iniFile->ItemValue(KTestRespParsMultiBuff(), KRunTestItemName(), ETrue))
		{
		CRespParsTestMultipleBuffers* respParserTest = CRespParsTestMultipleBuffers::NewLC();
		offlineTests.Append(respParserTest);

		TInt numLeakTests = iniFile->ItemValue(KTestRespParsMultiBuff(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestRespParsMultiBuff(), KFirstLeakIteration(), 0);
		respParserTest->SetLeakTests(numLeakTests, firstLeakIter);
		}
	if (iniFile->ItemValue(KTestRespParsMultiBody(), KRunTestItemName(), ETrue))
		{
		CRespParsTestMultiBodyNormal* respParserTest = CRespParsTestMultiBodyNormal::NewLC();
		offlineTests.Append(respParserTest);

		TInt numLeakTests = iniFile->ItemValue(KTestRespParsMultiBody(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestRespParsMultiBody(), KFirstLeakIteration(), 0);
		respParserTest->SetLeakTests(numLeakTests, firstLeakIter);
		}
	if (iniFile->ItemValue(KTestRespParsChunkedBody(), KRunTestItemName(), ETrue))
		{
		CRespParsTestChunkedResponse* respParserTest = CRespParsTestChunkedResponse::NewLC();
		offlineTests.Append(respParserTest);

		TInt numLeakTests = iniFile->ItemValue(KTestRespParsChunkedBody(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestRespParsChunkedBody(), KFirstLeakIteration(), 0);
		respParserTest->SetLeakTests(numLeakTests, firstLeakIter);
		}
	if (iniFile->ItemValue(KTestRespParsApacheHead(), KRunTestItemName(), ETrue))
		{
		CRespParsTestApacheHeadResponse* respParserTest = CRespParsTestApacheHeadResponse::NewLC();
		offlineTests.Append(respParserTest);

		TInt numLeakTests = iniFile->ItemValue(KTestRespParsApacheHead(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestRespParsApacheHead(), KFirstLeakIteration(), 0);
		respParserTest->SetLeakTests(numLeakTests, firstLeakIter);
		}
	if (iniFile->ItemValue(KTestRespParsIisHead(), KRunTestItemName(), ETrue))
		{
		CRespParsTestIISHeadResponse* respParserTest = CRespParsTestIISHeadResponse::NewLC();
		offlineTests.Append(respParserTest);

		TInt numLeakTests = iniFile->ItemValue(KTestRespParsIisHead(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestRespParsIisHead(), KFirstLeakIteration(), 0);
		respParserTest->SetLeakTests(numLeakTests, firstLeakIter);
		}
	if (iniFile->ItemValue(KTestLineBuffOverrun(), KRunTestItemName(), ETrue))
		{
		CRespParsTestLineBuffOverrun* respParserTest = CRespParsTestLineBuffOverrun::NewLC();
		offlineTests.Append(respParserTest);

		TInt numLeakTests = iniFile->ItemValue(KTestLineBuffOverrun(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestLineBuffOverrun(), KFirstLeakIteration(), 0);
		respParserTest->SetLeakTests(numLeakTests, firstLeakIter);
		}
	if (iniFile->ItemValue(KTestCycleRespBuffer(), KRunTestItemName(), ETrue))
		{
		TInt minBuffSize = iniFile->ItemValue(KTestCycleRespBuffer(), KMinBuffSizeItemName(), 1);
		TInt buffSizeInc = iniFile->ItemValue(KTestCycleRespBuffer(), KBuffSizeIncItemName(), 1);
		CRespParsTestCyclingRespBuffer* respParserTest = CRespParsTestCyclingRespBuffer::NewLC(minBuffSize, buffSizeInc);
		offlineTests.Append(respParserTest);

		TInt numLeakTests = iniFile->ItemValue(KTestCycleRespBuffer(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestCycleRespBuffer(), KFirstLeakIteration(), 0);
		respParserTest->SetLeakTests(numLeakTests, firstLeakIter);
		}
	
*/		
/*	if (iniFile->ItemValue(KWSPTransportHandlerStub(), KRunTestItemName(), ETrue))
		{
		CWspTrHndStubTest* wspStubTest = CWspTrHndStubTest::NewLC();
		offlineTests.Append(wspStubTest);

		TInt numLeakTests = iniFile->ItemValue(KTestCycleRespBuffer(), KLeakTestCount(), 0);
		TInt firstLeakIter = iniFile->ItemValue(KTestCycleRespBuffer(), KFirstLeakIteration(), 0);
		wspStubTest->SetLeakTests(numLeakTests, firstLeakIter);
		}
*/
	// Set the test set based on the ini file settings
	engine->ConfigureTestSet(offlineTests);

	// Pop the tests which are now owned by the test engine
	TInt numTestsToPop = offlineTests.Count();
	CleanupStack::Pop(numTestsToPop);
	CleanupStack::Pop(); // offlineTests, the array itself

	// All the action happens within the main event loop
	CActiveScheduler::Start();

	engine->Utils().LogIt(_L("\nEnd of tests"));
	TInt pass, fail;
	engine->TestSummary(pass, fail);
	engine->Utils().LogIt(_L("\nTest Summary: %d passed, %d failed.\n"), pass, fail);
	engine->PressAnyKey();
	
	// Clean up
	CleanupStack::PopAndDestroy(3); // inifile, engine, as
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
	TRAPD(err,engine = CHttpTestEngine::NewL(KHttpOfflineTestTitle(), EFalse));
	if (err != KErrNone)
		{
		User::Panic(_L("T_OFFLINE"),err);
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
