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
// Test harness for HTTP Performance Testing. It performs 4 tests
// 1. 100 Get Transactions in parallel
// 2. 100 Post Transactions in parallel
// 3. 100 Get Transaction sequentially, waiting for 1st transaction to suceed before submitting next
// 4. 100 Post Transactions sequentiallly
// 
//

#include "HttpTestUtils.h"

#include "HttpTestCore.h"

#include "TestScripts.h"

#include "t_performancetrans.h"

// Constants used in this file
enum TPanic
	{
	EFailedToCreateEngine,
	};

// Name of test harness
_LIT(KHttpPerformanceTestTitle, "HTTP Performance Tests");

const TInt KNumberTestTransactions = 50;
_LIT(KHttpPerformanceGetURI, "http://10.29.23.4/");
_LIT(KHttpPerformancePostURI, "http://10.29.23.4/cgi-bin/blackhole");


void DoTestsL()
	{	
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
	TRAPD(err,engine = CHttpTestEngine::NewL(KHttpPerformanceTestTitle(), EFalse));
	if (err != KErrNone)
		User::Panic(_L("T_HTTPPERF"),EFailedToCreateEngine);
	CleanupStack::PushL(engine);

	// Read the default silent setting for the tests
	engine->Utils().SetSilent(ETrue);

	// Create the tests we're going to run.  Ownership is transferred to the test engine
	RPointerArray<CHttpTestBase> performanceTests;
	CleanupClosePushL(performanceTests);


	CTestPerformanceTrans* test;
	test = CTestPerformanceTrans::NewLC(KHttpPerformanceGetURI,KNumberTestTransactions, CTestPerformanceTrans::EGetParallel);
	User::LeaveIfError(performanceTests.Append(test));

	test = CTestPerformanceTrans::NewLC(KHttpPerformancePostURI,KNumberTestTransactions, CTestPerformanceTrans::EPostParallel);
	User::LeaveIfError(performanceTests.Append(test));

	test = CTestPerformanceTrans::NewLC(KHttpPerformanceGetURI,KNumberTestTransactions, CTestPerformanceTrans::EGetSequential);
	User::LeaveIfError(performanceTests.Append(test));
	
	test = CTestPerformanceTrans::NewLC(KHttpPerformancePostURI,KNumberTestTransactions, CTestPerformanceTrans::EPostSequential);
	User::LeaveIfError(performanceTests.Append(test));
	

	// Set the test set based on the menu choices
	engine->ConfigureTestSet(performanceTests);

	// Pop the tests which are now owned by the test engine
	CleanupStack::Pop(performanceTests.Count());
	CleanupStack::Pop(); // performanceTests, the array itself

	// All the action happens within the main event loop.
	CActiveScheduler::Start();

	// Returned from start so shutdown. Turn off test engine silence to get a summary report.
	engine->Utils().SetSilent(EFalse);
	engine->Utils().LogIt(_L("\nEnd of tests"));
	TInt pass, fail;
	engine->TestSummary(pass, fail);
	engine->Utils().LogIt(_L("\nTest Summary: %d passed, %d failed.\n"), pass, fail);
	engine->PressAnyKey();
	
	// Release the held open connection
	CHTTPTestUtils::ReleaseConnectionL();	

	CleanupStack::PopAndDestroy(2);// engine, as
	}

// main loop
//
GLDEF_C TInt E32Main()
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
