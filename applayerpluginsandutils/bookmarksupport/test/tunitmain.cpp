// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "testutils.h"
#include "tunittests.h"

_LIT(KTitle, "Bookmark Unit Tests");

LOCAL_D RTest gTest(KTitle());

LOCAL_D CTrapCleanup* gTheTrapCleanup;
LOCAL_D CTestWrapper* gTestWrapper;

const TInt KTestCleanupStack=0x500;

LOCAL_C void DoTestsL()
	{
	DoCBookmarkBaseUnitL(gTestWrapper);
	DoCBookmarkUnitL(gTestWrapper);
	DoCBookmarkFolderUnitL(gTestWrapper);
	DoPropertiesUnitL(gTestWrapper);
	DoAttachmentUnitL(gTestWrapper);
	DoCBookmarkDbUnitL(gTestWrapper);
	DoCPropertyRegUnitL(gTestWrapper);
	
	}


LOCAL_C void TestL()
	{
	gTestWrapper = CTestWrapper::NewLC(gTest);
	gTestWrapper->Start(_L("@SYMTestCaseID IWS-APPPROTOCOLS-BOOKMARKS-TUNITMAIN-0001 test"));

	TRAPD(ret,DoTestsL());
	gTestWrapper->TESTE(ret==KErrNone, ret);

	gTestWrapper->End();
	CleanupStack::PopAndDestroy(gTestWrapper);
	}

LOCAL_C void setupCleanup()
//
// Initialise the cleanup stack.
//
    {
	gTheTrapCleanup=CTrapCleanup::New();
	gTest(gTheTrapCleanup!=NULL);
	TRAPD(r,\
		{\
		for (TInt i=KTestCleanupStack;i>0;i--)\
			CleanupStack::PushL((TAny*)0);\
		CleanupStack::Pop(KTestCleanupStack);\
		});
	gTest(r==KErrNone);
	}


GLDEF_C TInt E32Main()
    {
	setupCleanup();
	__UHEAP_MARK;
	//

	TRAPD(ret,TestL());
	gTest(ret==KErrNone);
	//

	__UHEAP_MARKEND;
	delete gTheTrapCleanup;
	return KErrNone;
    }

