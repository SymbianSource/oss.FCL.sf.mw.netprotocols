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

#ifndef __CTESTCASE13_H__
#define __CTESTCASE13_H__

#include <e32base.h>
#include <http.h>

#include "CBatchingTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 2 simple requests to test server with batching enabled. Both requests are
	very small, the first request is sent immediately, and the second is sent on the timer
	expiry as the second request is small and the buffer will not get filled up.
*/
class CTestCase13 : public CBatchingTestCase
	{
public:
	static CTestCase13* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCase13();
	
private:
	CTestCase13(CHTTPTestUtils& aTestUtils);
	void ConstructL();

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TotalTransactionCount() const;
	TInt ConnectionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	
private:
	CHTTPTestUtils&			iTestUtils;
	};
	
#endif // __CTESTCASE13_H__
