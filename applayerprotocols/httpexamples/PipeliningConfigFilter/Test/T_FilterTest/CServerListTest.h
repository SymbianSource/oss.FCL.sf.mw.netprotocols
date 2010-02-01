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

#include <e32base.h>
#include "HttpTestCore.h"

class CDomainList;

class CServerListTest : public CHttpTestBase
	{
public:
	static CServerListTest* NewL();
	~CServerListTest();
	
	// From CHttpTestBase
	virtual const TDesC& TestName();
	virtual void DoRunL();
	virtual void DoCancel();
	
	void RunTestCase1L();
	void RunTestCase2L();
	void RunTestCase3L();
	void RunTestCase4L();
	void RunTestCase5L();
	void RunTestCase6L();
	
private:
	CServerListTest();
	void ConstructL();
	void ResetListL();

private:
	CDomainList* iDomainList;
	RStringPool iStringPool;
	const TStringTable& iStringTable;
	};
