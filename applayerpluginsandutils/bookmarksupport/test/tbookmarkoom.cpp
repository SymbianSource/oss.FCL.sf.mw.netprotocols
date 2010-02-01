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

#include <e32base.h>
#include "testutils.h"
#include "tbmoomtester.h"

_LIT(KTitle, "Bookmark OOM Tests");

LOCAL_D RTest gTest( KTitle() );

LOCAL_D CTestWrapper* gTestWrapper;


LOCAL_C void StartOomTestL ()
	{
	CBmOomTester* oomTester = CBmOomTester::NewL ( *gTestWrapper );
	CleanupStack::PushL ( oomTester );		
	TRAPD ( err, oomTester->TestL () );	
	User::LeaveIfError ( err );
	CleanupStack::PopAndDestroy ( oomTester );	
	}

LOCAL_C void DoOomTestsL ()
	{		
	gTestWrapper->Next(_L("[Bookmark tests] OOM testing"));
	TInt ret = KErrNoMemory;
	TInt failAt = 0;
	while(ret != KErrNone)
		{
		failAt++;
		gTestWrapper->Tester().Printf(_L("OOM test step: %d\n"),failAt);

		__UHEAP_SETFAIL( RHeap::EDeterministic, failAt );
		__UHEAP_MARK;
		TRAP( ret, StartOomTestL() );
		__UHEAP_MARKEND;
		__UHEAP_RESET;
		
		gTestWrapper->TESTE(ret == KErrNoMemory || ret == KErrNone, ret);
		}
	}

LOCAL_C void DoTestL ()
	{
	gTestWrapper = CTestWrapper::NewLC ( gTest );
	gTestWrapper->Start( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-BOOKMARKS-TBOOKMARKOOM-0001 test" ) );
	
	TRAPD ( err, DoOomTestsL () );
	gTestWrapper->TESTE ( err == KErrNone, err );

	gTestWrapper->End ();
	CleanupStack::PopAndDestroy ( gTestWrapper );
	}

GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;

	// Install exception handler
	CTrapCleanup* theCleanup = CTrapCleanup::New();

	TRAPD( ret, DoTestL() );
	gTest( ret == KErrNone );
	
	delete theCleanup;
	
	__UHEAP_MARKEND;
	return KErrNone;
    }

