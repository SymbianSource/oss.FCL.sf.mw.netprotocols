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
//

#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

#if !defined HTTP_H
#include <http.h>
#endif

class CHTTPFilterTest : public CHttpTestBase, public MHTTPTransactionCallback
	{
public:
	static CHTTPFilterTest* NewL();
	void DoTestsL();

	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	virtual void DoRunL();
	void DoRunSessionEventTestsL();
	virtual const TDesC& TestName();
	virtual void DoCancel();
	CHTTPTestUtils& Utils();

	void EndTest(TBool aResult);

	void StartSchedulerL();

	TInt iResult;
	TBool iSchedulerRunning;
	};

