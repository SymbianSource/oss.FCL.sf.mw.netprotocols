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

class CBmTests;
class CTestWrapper;

class CBmOomTester : public CBase
	{
	public:
	static CBmOomTester* NewL ( CTestWrapper& aTest );
	CBmOomTester ( CTestWrapper& aTest );
	~CBmOomTester ();
	
	void TestL ();
	
	private:
	void ResetBkmrkDbL ();
	void ConstructL ();
	void ResetRepositoryL ( TUid aUid );
	
	private:
	CBmTests* iBmTester;
	CTestWrapper& iTestWrapper;
	};

