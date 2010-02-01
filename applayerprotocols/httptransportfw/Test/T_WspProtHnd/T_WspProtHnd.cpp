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

#include <e32base.h>
#include <e32test.h>

#include "CWspCmdLineProcessor.h"
#include "CWspScriptProcessor.h"
#include "testscripts.h"

// Constants and definions
//
_LIT(KWspProtHndTestPanic, "T_WspProtHnd");
_LIT(KDriverTitle, "WSP Protocol Handler Test Driver");
_LIT(KDriverCompName, "wsp_pr_hnd_driver");
_LIT(KDriverIniName, "unittest_driver.ini");
_LIT(KDefaultSectionName, "Defaults");
_LIT(KInteractive, "Interactive");
_LIT(KInitialScript, "InitialScript");
_LIT(KFailed,"Failed to complete tests.");

/**
	Put in place a scheduler that can log the error
 */
class CTestScheduler : public CActiveScheduler
	{
	virtual void Error(TInt aError) const;
	};

void CTestScheduler::Error(TInt aError) const
	{
	User::Panic(KFailed,aError);
	}

GLDEF_C void Panic(TInt aPanic);

GLDEF_C void Panic(TInt aPanic)
	{
	User::Panic(KWspProtHndTestPanic, aPanic);
	}

GLDEF_C void DoTestsL()
	{
	// Create and install the active scheduler
 	CActiveScheduler* scheduler= new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	// Create the 'HTTP' test utilities
	CHTTPTestUtils* testUtils = CHTTPTestUtils::NewL(KDriverTitle());
	CleanupStack::PushL(testUtils);

	// Open the test driver .ini file, which holds the file names of the individual chat sessions
	CScriptFile* driverIniFile = CScriptFile::NewL(*testUtils, KDriverCompName(), KDriverIniName());
	CleanupStack::PushL(driverIniFile);

	// Read from the .ini file whether the test harness is to run interactively or not. Default is to be interactive
	// and that behaviour will be exhibited if the ini file is not found
	TBool isInteractive = driverIniFile->ItemValue(KDefaultSectionName(), KInteractive(), ETrue);
	const TDesC& initialScript = driverIniFile->ItemValue(KDefaultSectionName(), KInitialScript(), KNullDesC());

	if (isInteractive)
		{
		// Create the command-line driver
		CWspCmdLineProcessor* driver = CWspCmdLineProcessor::NewL(*(testUtils->Test().Console()),
																  initialScript);
		CleanupStack::PushL(driver);

		// Start the driver
		driver->Start();
		}
	else
		{
		// Create a scripted driver
		CWspScriptProcessor* driver = CWspScriptProcessor::NewL(*driverIniFile, *testUtils);
		CleanupStack::PushL(driver);

		// Start the driver
		driver->Start();
		}

	// Start the active scheduler
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(4, scheduler);	
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* tc=CTrapCleanup::New();
	TRAPD(err,DoTestsL());
	if (err!=KErrNone)
		User::Panic(KFailed,err);
	delete tc;
	__UHEAP_MARKEND;
	return KErrNone;
	}
