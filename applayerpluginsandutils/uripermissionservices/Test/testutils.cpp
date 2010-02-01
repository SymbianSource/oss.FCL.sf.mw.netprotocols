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

CTestWrapper* CTestWrapper::NewLC(RTest& aTester)
	{
	CTestWrapper* self = new (ELeave) CTestWrapper(aTester);
	CleanupStack::PushL(self);
	return self;
	}

CTestWrapper::CTestWrapper(RTest& aTester) : iTester(aTester)
	{
	}

CTestWrapper::~CTestWrapper()
	{
	iTester.Close();
	}

void CTestWrapper::Start(const TDesC& aHeading)
	{
	iTester.Start(aHeading);
	iTestCount = 1;
	iCheckCount = 0;
	iTotalCount = 0;
	}

void CTestWrapper::Next(const TDesC& aHeading)
	{
	iTester.Next(aHeading);
	++iTestCount;
	iCheckCount = 0;
	}

void CTestWrapper::End()
	{
	TInt failedCount = iFailedTests.Count();
	iTester.Printf(_L(""));
	iTester.Printf(_L("*** Summary ***"));
	iTester.Printf(_L("Tests run:	%d"), iTotalCount);
	iTester.Printf(_L("Test failures:	%d"), failedCount);
	if (failedCount != 0)
		{
		TInt i = 0;
		for (; i < failedCount; ++i)
			{
			iTester.Printf(_L("Failed level [%d] at check number [%d]\n"),
					iFailedTests[i].iTestNumber, iFailedTests[i].iCheckNumber);
			}
		}
	else
		{
		iTester.End(); // prints success message
		}
	iFailedTests.Close();
	}

void CTestWrapper::Test(TInt aResult, TInt aLineNum)
    {
    // Test a condition.
    ++iCheckCount;
    ++iTotalCount;
	iTester(ETrue); // this just updates the iTester test count so that it is in sync with the wrapper.
    if (!aResult)
        {
		TFailedTest test;
		test.iTestNumber = iTestCount;
		test.iCheckNumber = iCheckCount;
        iTester.Printf(_L("FAIL : Failed level [%d] check [%d] in file [%S] at line number [%d]\n"), iTestCount, iCheckCount, &iFilename, aLineNum);
        if (iFailedTests.Append(test) != KErrNone)
        	iTester.Panic(_L("Checkpoint can't be added to failed list"));
        }
    }

void CTestWrapper::Test(TInt aResult, TInt aError, TInt aLineNum)
    {
    // Test a condition.
    ++iCheckCount;
    ++iTotalCount;
	iTester(ETrue); // this just updates the iTester test count so that it is in sync with the wrapper.
    if (!aResult)
        {
		TFailedTest test;
		test.iTestNumber = iTestCount;
		test.iCheckNumber = iCheckCount;
        iTester.Printf(_L("FAIL : Failed level [%d] check [%d] in file [%S] at line number [%d] with error code [%d]\n"), iTestCount, iCheckCount, &iFilename, aLineNum, aError);
        if (iFailedTests.Append(test) != KErrNone)
        	iTester.Panic(_L("Checkpoint can't be added to failed list"));
        }
    }

void CTestWrapper::SetFile(const TDesC& aFilename)
	{
	iFilename = aFilename;
	}

RTest& CTestWrapper::Tester()
	{
	return iTester;
	}
