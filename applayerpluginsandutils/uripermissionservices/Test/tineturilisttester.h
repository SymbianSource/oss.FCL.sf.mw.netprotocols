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
// tbmoomtester.h
// 
//

#include <e32base.h>
#include <ineturilistdef.h>

class CTestWrapper;
class CInetUriListQuery;

class CInetUriListTester : public CBase
	{
	public:
	static CInetUriListTester* NewL ( CTestWrapper& aTest );

	~CInetUriListTester ();
	
	void TestL ( TBool aNormalRun = ETrue );
	void DoListCleanupL ();
	
	protected:
	CInetUriListTester ( CTestWrapper& aTest );
	void ConstructL ();
	
	private:	
	void DoStorageTestsL ( TBool aNormalRun );
	void DoQueryTestsL ( TBool aNormalRun );
	protected:
	CTestWrapper& iTestWrapper;
	};

