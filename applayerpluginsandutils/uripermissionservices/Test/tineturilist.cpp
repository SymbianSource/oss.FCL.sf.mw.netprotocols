// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <ineturilistdef.h>

#include <s32mem.h>
#include <s32buf.h>
#include "testutils.h"
#include <ineturilist.h>
#include "tineturilisttester.h"

_LIT(KTitle, "InetUriList Dll Tests");

LOCAL_D RTest gTest( KTitle() );

LOCAL_D CTestWrapper* gTestWrapper;

LOCAL_C void RunTestsL ( TBool aNormalRun = ETrue )
	{	
	CInetUriListTester* listTester = CInetUriListTester::NewL ( *gTestWrapper );
	CleanupStack::PushL ( listTester );
	listTester->TestL (aNormalRun);
	listTester->DoListCleanupL ();
	CleanupStack::PopAndDestroy (); // listTester
	}
	
	
LOCAL_C void RunOoomTestsL ()
	{
	gTestWrapper->Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-INETURILIST-LIST-0001 [Inet Uri List tests] OOM testing"));
	TRAPD( ret, RunTestsL(EFalse) );
	gTestWrapper->TESTE( ret==KErrNone, ret );
	}

LOCAL_C void DoTestsL ()
	{	
	gTestWrapper = CTestWrapper::NewLC(gTest);
	gTestWrapper->Start(_L("test"));
		
	TRAPD ( err, RunTestsL () );
	gTestWrapper->TESTE( err==KErrNone, err );	

	TRAP ( err, RunOoomTestsL () );
	gTestWrapper->TESTE( err==KErrNone, err );	

	gTestWrapper->End();
	CleanupStack::PopAndDestroy(gTestWrapper);
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* tc = CTrapCleanup::New();
	TRAPD( err, DoTestsL() );
	if ( err != KErrNone )
		User::Panic( _L( "Test failed with error code: %i" ), err );
	delete tc;
	__UHEAP_MARKEND;
	return KErrNone;
	}
