// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestScriptTestStep.h"
#include "CPipeliningTestEngine.h"

/**
Constructor. Sets the test step name
*/
CTestScriptTestStep::CTestScriptTestStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestScriptTestStep);
	}

/**
Tries to create a file mentioned in the ini file.
@internalTechnology
@param		None
@return		EPass or EFail indicating the success or failure of file creation.
*/
TVerdict CTestScriptTestStep::doTestStepL()
	{
	
	// Create and install the active scheduler
 	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	// Create the test engine
	CPipeliningTestEngine* testEngine = CPipeliningTestEngine::NewL();
	CleanupStack::PushL(testEngine);

	TPtrC ptr(ConfigSection());
	TRAPD(error, testEngine->StartTestsL(ptr));
	
	if(error != KErrNone)
		{
		SetTestStepError(error);
		SetTestStepResult(EFail);		
		}
	else
		{
		SetTestStepResult(EPass);		
		}

	CleanupStack::PopAndDestroy(testEngine);
	CleanupStack::PopAndDestroy(scheduler); 
	
	return TestStepResult();
	
	}	// doTestStepL

