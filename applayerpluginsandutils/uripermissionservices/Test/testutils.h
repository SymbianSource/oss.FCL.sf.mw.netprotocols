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
// A few utils for assisting testing
// @internalComponent
// 
//

#ifndef __TESTUTILS_H__
#define __TESTUTILS_H__

#include <e32test.h>

#define TEST(arg) Test((arg), __LINE__)
#define TESTE(arg, error) Test((arg), (error), __LINE__)

const TInt KTestTitleLength = 128;
/**
CTestWrapper is a simple testing class that wraps RTest.
The problem with RTest is that it panics on the first failure. All CTestWrapper does is
note failed tests and summerise failures at the end.
@internalComponent
*/
class CTestWrapper: public CBase
	{
private:
	class TFailedTest
		{
	public:
		TUint iTestNumber;
		TUint iCheckNumber;
		};
public:
	static CTestWrapper* NewLC(RTest& aTester);
	~CTestWrapper();

	void Start(const TDesC &aHeading);
	void Next(const TDesC &aHeading);
	void End();
	void Test(TInt aResult, TInt aLineNum);
	void Test(TInt aResult, TInt aError, TInt aLineNum);

	void SetFile(const TDesC& aFilename);

	RTest& Tester();

private:
	CTestWrapper(RTest& aTester);

private:
	TUint 				iTestCount;
	TUint 				iCheckCount;
	TUint 				iTotalCount;
	RTest&				iTester;
	RArray<TFailedTest> iFailedTests;
	TFileName			iFilename;
	};

#endif //__TESTUTILS_H__
