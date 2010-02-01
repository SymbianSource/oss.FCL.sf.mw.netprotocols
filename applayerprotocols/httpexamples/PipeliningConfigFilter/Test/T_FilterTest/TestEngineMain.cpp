// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ctestenginemain.cpp
// 
//

#include <e32base.h>
#include "HttpTestCore.h"
#include "httptestutils.h"
#include "TestScripts.h"
#include "CPipeliningConfigDriver.h"
#include "CServerListTest.h"

// Standard Lits
_LIT(KFilterTestTitle, "Filter Test Harness");
_LIT(KTestCompName, "Filters");
_LIT(KFilterTestScriptName, "FilterTestCases.ini");

_LIT(KSectionDefaults, "Defaults");
_LIT(KItemSilent, "Silent");
_LIT(KRunTestItemName, "RunTest");
_LIT(KLeakTestCount, "LeakTestCount");
_LIT(KFirstLeakIteration, "FirstLeakIteration");

// Lits for Pipelining Config Filter Test
_LIT(KPipeliningConfigTest, "PipeliningConfigFilter");
_LIT(KServerList, "ServerList");

void StartL()
	{	
	TBool runTestDefault = EFalse;

	CTestScheduler* as = new(ELeave) CTestScheduler;
	CleanupStack::PushL(as);
	if (!as)
		User::Panic(_L("Failed to create active scheduler"),KErrNoMemory);
	CActiveScheduler::Install(as); // Install active scheduler
	
	// Initialise Comms, required in a minimal console environment
	CHTTPTestUtils::InitCommsL();
	
	CHttpTestEngine* engine = NULL;
	TRAPD(err,engine = CHttpTestEngine::NewL(KFilterTestTitle(), ETrue)); // not silent by default
	if (err != KErrNone)
		User::Panic(_L("T_FILTER_TEST_ENG"),err);
	CleanupStack::PushL(engine);

	// Open the INI file to initialise the tests we're going to run
	CScriptFile* iniFile = NULL;
	TRAP(err, iniFile = CScriptFile::NewL(engine->Utils(), KTestCompName));
	if (err != KErrNone)
		User::Panic(_L("T_FILTER_TEST_INI"),err);
	CleanupStack::PushL(iniFile);
	iniFile->ReadScriptL(KFilterTestScriptName());

	// Create an array containing all the tests we're going to run. 
	// Ownership is transferred to the test engine
	RPointerArray<CHttpTestBase> filterTests;
	CleanupClosePushL(filterTests);
	
	CArrayPtrFlat<CScriptSection>& sections=iniFile->Sections();

	TInt secCount = sections.Count();
	TInt count= secCount;
	CScriptSection* section = NULL;
	for (TInt ii=0; ii<count; ++ii)
		{
		//create an array of tests to be sent to the engine
		section = sections[ii];
		_LIT(KZero, "0");

		// Deafaults section
		if (section->SectionName().CompareF(KSectionDefaults) == 0)
			{
			if (iniFile->Section(ii).ItemL(KItemSilent).Value().CompareF(KZero) == 0)
   				engine->SetSilent(EFalse);
 			else  	
				engine->SetSilent(ETrue);
 			if (iniFile->Section(ii).ItemL(KRunTestItemName).Value().CompareF(KZero) != 0)
				runTestDefault = ETrue;
			}

		// Pipelining Config Filter test section
		if ( section->SectionName().CompareF(KPipeliningConfigTest) == 0)
			{
			TBool runTest = runTestDefault;
			
   			TPtrC itemPtr(iniFile->Section(ii).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
 				runTest = !(iniFile->Section(ii).Item(0).Value().CompareF(KZero) == 0);
 			if (runTest)
   				{
   				CHttpTestBase* pipelineConfigTest = CPipeliningConfigDriver::NewL(*iniFile, ii);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(ii).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(ii).SectionName(), KFirstLeakIteration(), 0);
				pipelineConfigTest->SetLeakTests(numLeakTests, firstLeakIter);
				CleanupStack::PushL(pipelineConfigTest);
   				User::LeaveIfError(filterTests.Append(pipelineConfigTest));
   				CleanupStack::Pop(pipelineConfigTest);
   				}
   			}
   				
   		// Domain list unit test section
   		if ( section->SectionName().CompareF(KServerList) == 0)
			{
			TBool runTest = runTestDefault;
			
			CScriptSectionItem* item = iniFile->Section(ii).Item(KRunTestItemName);
			if(item)
				{
				runTest = !(item->Value().CompareF(KZero()) ==0 );
				}
				
 			if (runTest)
   				{
   				CHttpTestBase* serverListTest = CServerListTest::NewL();
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(ii).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(ii).SectionName(), KFirstLeakIteration(), 0);
				serverListTest->SetLeakTests(numLeakTests, firstLeakIter);
				CleanupStack::PushL(serverListTest);
   				User::LeaveIfError(filterTests.Append(serverListTest));
   				CleanupStack::Pop(serverListTest);
   				}
			}
		}
	
	// Set the test array then pop because the engine takes ownership
	engine->ConfigureTestSet(filterTests);		
	
	CleanupStack::Pop(&filterTests); 

	// All the action happens within the main event loop
	CActiveScheduler::Start();

	TInt pass, fail;
	engine->TestSummary(pass, fail);
	engine->Utils().LogIt(_L("\nTest Summary: %d passed, %d failed.\n"), pass, fail);
	engine->PressAnyKey();
	// Returned from start so shutdown	
	//RL: pause a sec to get a gander
	engine->Utils().LogIt(_L("\nEnd of test program "));	
	CleanupStack::PopAndDestroy(3); // engine, iniFile, as
	}



// main loop
//
GLDEF_C TInt E32Main()
    {
__UHEAP_MARK;

	// Install exception handler
	CTrapCleanup* theCleanup = CTrapCleanup::New(); 
	TRAP_IGNORE(StartL());
	delete theCleanup;

__UHEAP_MARKEND;
	User::Heap().Check();
    return(KErrNone);
	}
