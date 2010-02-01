// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32test.h>
#include <ecom/ecom.h>

#include "ctestengine.h"

const TInt KMaxHeapSize = 0x100000;
const TInt KMaxIntDesLen = 10;				// Des length required for displaying max 32 bit int
_LIT(KTestThreadName,						"TestThread");
_LIT(KRtestFailureLabel, 					"RTEST: FAILED due to a %S %d Panic ");
_LIT(KPanicOnLeave,							"Leave DoTestL()");// Panic if DeTestL() does a Leave().


GLDEF_C void DoTestsL()
	{
	// The following two lines create a semaphore which removes the security dialog
	// displayed when an untrusted certificate is received from a SSL server
	RSemaphore semaphore;
	semaphore.CreateGlobal(_L("T_AUTOSSL"), 0);

	// Create and install the active scheduler
 	CActiveScheduler* scheduler= new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	// Create the test engine
	CTestEngine* testEngine = CTestEngine::NewL();
	CleanupStack::PushL(testEngine);

	// Do tests...
	testEngine->DoTestsL();
	
	CleanupStack::PopAndDestroy(2, scheduler);	
	}
	
TInt TestEntryPoint(TAny* /*aParam*/)
	{
	__UHEAP_MARK;
	
	CTrapCleanup* theCleanup = CTrapCleanup::New(); // Install exception handler	
	
	// Run all the tests
	TRAPD(err,DoTestsL());
	if (err!=KErrNone)
		User::Panic(KPanicOnLeave,err);
	
	delete theCleanup;
	REComSession::FinalClose();
	
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
	CTestEngine* engine=NULL;
	TRAPD(err,engine = CTestEngine::NewL());
	if (err != KErrNone)
		{
		User::Panic(_L("T_HTTP_TRANSP_H"),err);
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
			
	engine->Log(logMessagePtr);
	
	// Panic here so that nothing else is written to the log file
	User::Panic(panicName, panicNumber);
	
	// The code below provided for completenes, but should never be reached.
	
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
	
