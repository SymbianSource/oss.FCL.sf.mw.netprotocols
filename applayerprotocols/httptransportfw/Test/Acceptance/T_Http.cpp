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
// The test files do not log the url that has been accessed. This
// is because we use an 8 bit HBufC or TPtr. The LogIt() function
// needs a 16 bit TDesC so we have commented the LogIt() function
// for the time being. This would require to overload the LogIt()
// function so that it can accept even 8 bit descriptors.
// 
//

#include <flogger.h>
#include "HttpTestCore.h"
#include "httptestutils.h"
#include "TestScripts.h"

// ITERATION 1
#include "Tc1.h"
#include "Tc2.h"
#include "Tc4.h"
#include "Tc5.h"
#include "Tc6.h"
#include "Tc7.h"
#include "Tc8.h"
#include "Tc9.h"
#include "Tc10.h"
#include "Tc11.h"
#include "Tc12.h"

// ITERATION 2 GET
#include "get12.h"
#include "get13.h"
#include "get14.h"
#include "Get17.h"
#include "Get18.h"
#include "get21.h"

// ITERATION 2 POST
#include "post12.h"
#include "post13.h"
#include "post14.h"
#include "post23.h"

//ITERATION 3 POST

#include "Iter3Post6.h"
#include "Iter3Post7.h"

// Iteration 5
#include "Iter5Chunked1.h"
#include "Iter5Chunked2.h"

#include "3BasicAuthTransactions.h"
#include "3DigestAuthTransactions.h"
#include "EofTransactionResubmission.h"


#include "SingleTrans.h"
#include "WspTestTransactions.h"
// Constants used in this file

// Location of INI file that defines the test cases to be run
_LIT(KAcceptanceTestCompName,	"acceptance");
_LIT(KAcceptanceTestScriptName, "TestCases.ini");

_LIT(KSettingsIniCompName,		"settings");
_LIT(KSettingsIniName,			"settings.ini");

// Labels for section names and resources in the INI file
_LIT(KSectionNameIter1Fmt,		"Iter1TestCase%d");

_LIT(KSectionDefaults,			"Defaults");
_LIT(KItemSilent,				"Silent");
_LIT(KSectionSingleTrans,		"SINGLE_TRANS");
_LIT(KSectionWspSingleTrans,	"WSP_SINGLE_TRANS");
_LIT(KSectionGet12,				"GET12");
_LIT(KSectionGet13,				"GET13");
_LIT(KSectionGet14,				"GET14");
_LIT(KSectionGet17,				"GET17");
_LIT(KSectionGet18,				"GET18");
_LIT(KSectionGet21,				"GET21");

_LIT(KSectionPost12,			"POST12");
_LIT(KSectionPost13,			"POST13");
_LIT(KSectionPost14,			"POST14");
_LIT(KSectionPost23,			"POST23");

_LIT(KSection3Post6,			"Iter3Post6");
_LIT(KSection3Post7,			"Iter3Post7");

_LIT(KSection5Chunked1,			"Iter5Chunked1");
_LIT(KSection5Chunked2,			"Iter5Chunked2");
_LIT(K3BasicAuthTransactions,	"3BasicAuthTransactions");
_LIT(K3DigestAuthTransactions,	"3DigestAuthTransactions");
_LIT(KEofTransactionResubmission, "EofTransactionResubmission");

_LIT(KRunTestItemName,			"RunTest");

_LIT(KLeakTestCount,			"LeakTestCount");
_LIT(KFirstLeakIteration,		"FirstLeakIteration");

// Other stuff.
_LIT(KHttpTestTitle,			"Http Acceptance Tests");

const TInt KMaxHeapSize = 0x100000;
const TInt KMaxIntDesLen = 10;	// Des length required for displaying max 32 bit int
_LIT(KTestThreadName,			"TestThread");
_LIT(KRtestFailureLabel, 		"RTEST: FAILED due to a %S %d Panic ");
_LIT(KPanicOnLeave,				"Leave DoTestL()");// Panic if DeTestL() does a Leave().

void AppendIteration1Tests(CScriptFile& aIniFile, 
						   CScriptFile& aIniSettingsFile, 
						   RPointerArray<CHttpTestBase>& aTestArray)
	{
	// ITERATION 1 TESTS
	TBuf<24> sectionName;
	sectionName.Format(KSectionNameIter1Fmt, 1);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		CHttpTestCase1* tc1 = new(ELeave) CHttpTestCase1;
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc1->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc1);
		}
	sectionName.Format(KSectionNameIter1Fmt, 2);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		CHttpTestCase2* tc2 = new(ELeave) CHttpTestCase2(&aIniSettingsFile);
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc2->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc2);
		}
	sectionName.Format(KSectionNameIter1Fmt, 4);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		CHttpTestCase4* tc4 = new(ELeave) CHttpTestCase4(&aIniSettingsFile);
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc4->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc4);
		}
	sectionName.Format(KSectionNameIter1Fmt, 5);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		CHttpTestCase5* tc5 = new(ELeave) CHttpTestCase5(&aIniSettingsFile);
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc5->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc5);
		}
	sectionName.Format(KSectionNameIter1Fmt, 6);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		CHttpTestCase6* tc6 = new(ELeave) CHttpTestCase6(&aIniSettingsFile);
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc6->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc6);
		}
	sectionName.Format(KSectionNameIter1Fmt, 7);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		CHttpTestCase7* tc7 = new(ELeave) CHttpTestCase7(&aIniSettingsFile);
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc7->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc7);
		}
	sectionName.Format(KSectionNameIter1Fmt, 8);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		CHttpTestCase8* tc8 = new(ELeave) CHttpTestCase8(&aIniSettingsFile);
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc8->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc8);
		}
	sectionName.Format(KSectionNameIter1Fmt, 9);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		CHttpTestCase9* tc9 = new(ELeave) CHttpTestCase9(&aIniSettingsFile);
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc9->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc9);
		}
	sectionName.Format(KSectionNameIter1Fmt, 10);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		CHttpTestCase10* tc10 = new(ELeave) CHttpTestCase10(&aIniSettingsFile);
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc10->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc10);
		}
	sectionName.Format(KSectionNameIter1Fmt, 11);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		// this one fails!?
		CHttpTestCase11* tc11 = new(ELeave) CHttpTestCase11(&aIniSettingsFile);
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc11->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc11);
		}
	sectionName.Format(KSectionNameIter1Fmt, 12);
	if (aIniFile.ItemValue(sectionName, KRunTestItemName, EFalse))
		{
		CHttpTestCase12* tc12 = new(ELeave) CHttpTestCase12(&aIniSettingsFile);
		TInt numLeakTests = aIniFile.ItemValue(sectionName, KLeakTestCount(), 0);
		TInt firstLeakIter = aIniFile.ItemValue(sectionName, KFirstLeakIteration(), 0);
		tc12->SetLeakTests(numLeakTests, firstLeakIter);
		aTestArray.Append(tc12);
		}
	}

void StartL()
	{	
	// The following two lines create a semaphore which removes the security dialog
	// displayed when an untrusted certificate is received from a SSL server
	RSemaphore semaphore;
	semaphore.CreateGlobal(_L("T_AUTOSSL"), 0);

	TBool runTestDefault = EFalse;

	CTestScheduler* as = new(ELeave) CTestScheduler;
	CleanupStack::PushL(as);
	if (!as)
		{
		User::Panic(_L("Failed to create active scheduler"),KErrNoMemory);
		}
	CActiveScheduler::Install(as); // Install active scheduler
	
	// tialise Comms, required in a minimal console environment
	CHTTPTestUtils::InitCommsL();
	
	CHttpTestEngine* engine = NULL;
	// not silent by default
	TRAPD(err,engine = CHttpTestEngine::NewL(KHttpTestTitle(), ETrue));
	if (err != KErrNone)
		{
		User::Panic(_L("T_HTTP_ENG"),err);
		}
	CleanupStack::PushL(engine);

	engine->Utils().LogIt(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-FRAMEWORK-ACCPT-T_HTTP-0001  "));
	
	// Open the INI file to initialise the tests we're going to run
	CScriptFile* iniFile = NULL;
	TRAP(err, iniFile = CScriptFile::NewL(engine->Utils(), KAcceptanceTestCompName));
	if (err != KErrNone)
		{
		User::Panic(_L("T_HTTP_INI"),err);
		}
	CleanupStack::PushL(iniFile);
	iniFile->ReadScriptL(KAcceptanceTestScriptName());

	// Open the settings.ini file which has the server IP/Name that would
	// be used by the tests.
	CScriptFile* iniSettingsFile = NULL;
	TRAP(err, iniSettingsFile = CScriptFile::NewL(engine->Utils(), KSettingsIniCompName, KSettingsIniName()));
	if (err != KErrNone)
		{
		User::Panic(KAcceptanceTestCompName(),err);
		}
	CleanupStack::PushL(iniSettingsFile);

	// Create an array containing all the tests we're going to run. 
	// Ownership is transferred to the test engine
	RPointerArray<CHttpTestBase> acceptanceTests;
	CleanupClosePushL(acceptanceTests);
	
	CArrayPtrFlat<CScriptSection>& sections=iniFile->Sections();

	//create the tests for iteration2   and  append them in the array
	AppendIteration1Tests(*iniFile, *iniSettingsFile, acceptanceTests);

	//create the  tests for iteration2   and  append them in the array
	TInt secCount = sections.Count();
	TInt count= secCount;
	CScriptSection* section = NULL;
	for (TInt i=0;i<count;i++)
		{
		//create an array of tests to be sent to the engine
		section = sections[i];
		_LIT(KZero, "0");

		if (section->SectionName().CompareF(KSectionDefaults) == 0)
			{
			if (iniFile->Section(i).ItemL(KItemSilent).Value().CompareF(KZero) == 0)
				{
   				engine->SetSilent(EFalse);
				}
 			else
				{
				engine->SetSilent(ETrue);
				}
 			if (iniFile->Section(i).ItemL(KRunTestItemName).Value().CompareF(KZero) != 0)
				{
				runTestDefault = ETrue;
				}
			}

		if ( section->SectionName().CompareF(KSectionSingleTrans) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestBase* singleTrans = CHttpTestSingleTrans::NewL(*iniFile,iniSettingsFile,i);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				singleTrans->SetLeakTests(numLeakTests, firstLeakIter);
   				acceptanceTests.Append(singleTrans);
   				}
			}

		if ( section->SectionName().CompareF(KSectionWspSingleTrans) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestBase* wspTrans = CWspTestTransactions::NewL(*iniFile, iniSettingsFile,i);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				wspTrans->SetLeakTests(numLeakTests, firstLeakIter);
   				acceptanceTests.Append(wspTrans);
   				}
			}

		if ( section->SectionName().CompareF(KSectionGet12) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCaseGet12* get12 = new(ELeave) CHttpTestCaseGet12(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				get12->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(get12);
   				}
			}

		if ( section->SectionName().CompareF(KSectionGet13) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCaseGet13* get13 = new(ELeave) CHttpTestCaseGet13(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				get13->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(get13);
   				}
			}
		if ( section->SectionName().CompareF(KSectionGet14) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCaseGet14* get14 = new(ELeave) CHttpTestCaseGet14(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				get14->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(get14);
   				}
			}
		if ( section->SectionName().CompareF(KSectionGet17) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCaseGet17* get17 = new(ELeave) CHttpTestCaseGet17(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				get17->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(get17);
   				}
			}

		if ( section->SectionName().CompareF(KSectionGet18) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
	   			CHttpTestCaseGet18* get18 = new(ELeave) CHttpTestCaseGet18(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				get18->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(get18);
   				}
			}
		if ( section->SectionName().CompareF(KSectionGet21) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCaseGet21* get21 = new(ELeave) CHttpTestCaseGet21;
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				get21->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(get21);
   				}
			}
		if ( section->SectionName().CompareF(KSectionPost12) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCasePost12* post12 = new(ELeave) CHttpTestCasePost12(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				post12->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(post12);
   				}
			}
		if ( section->SectionName().CompareF(KSectionPost13) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCasePost13* post13 = new(ELeave) CHttpTestCasePost13(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				post13->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(post13);
   				}
			}	
		if ( section->SectionName().CompareF(KSectionPost14) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCasePost14* post14 = new(ELeave) CHttpTestCasePost14(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				post14->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(post14);
   				}
			}
		if ( section->SectionName().CompareF(KSectionPost23) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCasePost23* post23 = new(ELeave) CHttpTestCasePost23(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				post23->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(post23);
   				}
			}
		if ( section->SectionName().CompareF(KSection3Post6) == 0)
			{
			TBool runTest = runTestDefault;
   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCaseIter3Post6* iter3Post6= new(ELeave) CHttpTestCaseIter3Post6(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				iter3Post6->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(iter3Post6);
   				}
			}
		if ( section->SectionName().CompareF(KSection3Post7) == 0)
			{
			TBool runTest = runTestDefault;
   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCaseIter3Post7* iter3Post7 = new(ELeave) CHttpTestCaseIter3Post7(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				iter3Post7->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(iter3Post7);
   				}
			}
		if ( section->SectionName().CompareF(KSection5Chunked1) == 0)
			{
			TBool runTest = runTestDefault;
   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCaseIter5Chunked1* iter5Chunked1 = new(ELeave) CHttpTestCaseIter5Chunked1(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				iter5Chunked1->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(iter5Chunked1);
   				}
			}
		if ( section->SectionName().CompareF(KSection5Chunked2) == 0)
			{
			TBool runTest = runTestDefault;
   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
   				CHttpTestCaseIter5Chunked2* iter5Chunked2 = new(ELeave) CHttpTestCaseIter5Chunked2(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				iter5Chunked2->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(iter5Chunked2);
   				}
			}

		if ( section->SectionName().CompareF(K3BasicAuthTransactions) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
	   			C3BasicAuthTransactions* basicAuthTrans = new(ELeave) C3BasicAuthTransactions(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				basicAuthTrans->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(basicAuthTrans);
   				}
			}

		if ( section->SectionName().CompareF(K3DigestAuthTransactions) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
	   			C3DigestAuthTransactions* digestAuthTrans = new(ELeave) C3DigestAuthTransactions(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				digestAuthTrans->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(digestAuthTrans);
   				}
			}

		if ( section->SectionName().CompareF(KEofTransactionResubmission) == 0)
			{
			TBool runTest = runTestDefault;

   			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
   				{
	   			CEofTransactionResubmission* eofTransResubmission = new(ELeave) CEofTransactionResubmission(iniSettingsFile);
				TInt numLeakTests = iniFile->ItemValue(iniFile->Section(i).SectionName(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(iniFile->Section(i).SectionName(), KFirstLeakIteration(), 0);
				eofTransResubmission->SetLeakTests(numLeakTests, firstLeakIter);
				acceptanceTests.Append(eofTransResubmission);
   				}
			}
		}
	
	// Set the test array then pop because the engine takes ownership
	engine->ConfigureTestSet(acceptanceTests);		

	//--------
	CleanupStack::Pop(&acceptanceTests);// 

	// All the action happens within the main event loop
	CActiveScheduler::Start();

	TInt pass, fail;
	engine->TestSummary(pass, fail);
	engine->Utils().LogIt(_L("\nTest Summary: %d passed, %d failed.\n"), 
		pass, fail);
	engine->PressAnyKey();
	// Returned from start so shutdown	
	//RL: pause a sec to get a gander
	engine->Utils().LogIt(_L("\nEnd of test program "));	
	CleanupStack::PopAndDestroy(4); // engine, iniSettingsFile, iniFile, as
	}

TInt TestEntryPoint(TAny* /*aParam*/)
    {
__UHEAP_MARK;

	// Install exception handler
	CTrapCleanup* theCleanup = CTrapCleanup::New(); 
	
	// Run all the tests
	TRAPD(err,StartL());
	if (err!=KErrNone)
		User::Panic(KPanicOnLeave,err);
	
	delete theCleanup;

__UHEAP_MARKEND;
	User::Heap().Check();
    return(KErrNone);
	}
	
void LogOnPanicL(RThread& aTestThread)	
	{
	CTestScheduler* as = new(ELeave) CTestScheduler;
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
	TRAPD(err,engine = CHttpTestEngine::NewL(KHttpTestTitle(), EFalse));
	if (err != KErrNone)
		{
		User::Panic(_L("T_HTTP_ENG"), err);
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

