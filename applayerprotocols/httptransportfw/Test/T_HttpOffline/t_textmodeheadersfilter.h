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

class MHTTPFilter;

class CTextModeHeadersFilterTest : public CHttpTestBase, public MHTTPTransactionCallback
	{
public:
	static CTextModeHeadersFilterTest* NewLC();
	virtual void DoRunL();
	virtual const TDesC& TestName();
	virtual void DoCancel();
	~CTextModeHeadersFilterTest();

	// from MHTTPTransactionCallback
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

private:
	inline void FailTestL();
	void TestConnectionHeaderL();

private:
	RHTTPSession iSession;
	MHTTPFilter* iFilter;
	};
 

inline void CTextModeHeadersFilterTest::FailTestL()
	{
	User::Leave(KErrGeneral);
	}
