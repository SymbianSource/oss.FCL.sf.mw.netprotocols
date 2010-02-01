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

#include "tineturilisttester.h"
#include "tineturilisttests.h"
#include "testutils.h"
#include <e32test.h>


CInetUriListTester* CInetUriListTester::NewL ( CTestWrapper& aTest )
	{
	CInetUriListTester* self = new (ELeave) CInetUriListTester ( aTest );
	return self;
	}

CInetUriListTester::CInetUriListTester ( CTestWrapper& aTest ) 
: iTestWrapper ( aTest )
	{
	
	}
	
CInetUriListTester::~CInetUriListTester ()
	{
	}

void CInetUriListTester::TestL ( TBool aNormalRun )
	{
	DoStorageTestsL (aNormalRun);
	DoQueryTestsL (aNormalRun);
	}

void CInetUriListTester::DoStorageTestsL (TBool aNormalRun)
	{
	CInetUriListTestsBase* storageTests = CInetUriListStorageTests::NewL ( iTestWrapper, aNormalRun );
	CleanupStack::PushL (storageTests);
	storageTests->RunTestsL ();
	CleanupStack::PopAndDestroy (); // storageTests	
	}

void CInetUriListTester::DoQueryTestsL (TBool aNormalRun)
	{
	CInetUriListTestsBase* queryTests = CInetUriListQuery::NewLC ( iTestWrapper, aNormalRun );
	queryTests->RunTestsL ();
	CleanupStack::PopAndDestroy (); // storageTests			
	}

void CInetUriListTester::DoListCleanupL ()
	{
	CInetUriListCleanup* listCleanup = CInetUriListCleanup::NewL (iTestWrapper);
	CleanupStack::PushL ( listCleanup );
	listCleanup->DoOperationL ();
	CleanupStack::PopAndDestroy (); // listCleanup				
	}


