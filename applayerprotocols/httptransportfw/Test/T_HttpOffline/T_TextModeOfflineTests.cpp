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
// defines test classes: CSomeOfflineTest, CSomeOtherOfflineTest
// 
//

#include "T_TextModeOfflineTests.h"


// CSomeOfflineTest-----------------------------------

void CSomeOfflineTest::DoRunL()
	{
	iExpectedStatusCode = 0;
	// insert test code here, allow it to leave if the test fails
	}
	
TInt CSomeOfflineTest::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CSomeOfflineTest::DoCancel()
	{
	}

CSomeOfflineTest::~CSomeOfflineTest()
	{
	}	

const TDesC& CSomeOfflineTest::TestName()
	{
	_LIT(KHeaderTestName,"CSomeOfflineTest");
	return KHeaderTestName;
	}


// CSomeOfflineTest-----------------------------------

CSomeOtherOfflineTest::~CSomeOtherOfflineTest()
	{
	}

const TDesC& CSomeOtherOfflineTest::TestName()
	{
	_LIT(KHeaderTestName,"CSomeOtherOfflineTest");
	return KHeaderTestName;
	}

void CSomeOtherOfflineTest::DoRunL()
	{
	iExpectedStatusCode = 0;
	// insert test code here, allow it to leave if the test fails
	}

TInt CSomeOtherOfflineTest::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CSomeOtherOfflineTest::DoCancel()
	{
	}

