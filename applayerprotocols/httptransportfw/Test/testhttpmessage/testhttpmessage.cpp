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
//

#include "testhttpmessage.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include "ctestengine.h"
//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this


LOCAL_C void DoStartL()
	{
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

//	MainL();
	// Create the test engine
	CTestEngine* testEngine = CTestEngine::NewL();
	CleanupStack::PushL(testEngine);

	// Do tests...
	testEngine->RunTestsL();
	// Delete active scheduler
	CleanupStack::PopAndDestroy(2,scheduler);
	}

//  Global Functions

GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Create output console
	TRAPD(createError, console = Console::NewL(KTextConsoleTitle, TSize(
			KConsFullScreen, KConsFullScreen)));
	if (createError)
		return createError;

	// Run application code inside TRAP harness
	TRAPD(mainError, DoStartL());
	if (mainError)
		console->Printf(KTextFailed, mainError);
	delete console;
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}

