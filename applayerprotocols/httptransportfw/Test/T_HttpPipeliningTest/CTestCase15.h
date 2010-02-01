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

#ifndef __CTESTCASE15_H__
#define __CTESTCASE15_H__

#include <e32base.h>
#include <http.h>

#include "CBatchingTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 3 requests to test server with batching enabled. The first and second
	requests as simple small requests and the thrid request a very large request
	greater than the buffer size. The last request is large enough that it spans
	over multiple buffers. The buffer size is reduced to 25 bytes to simplify the
	request. The first request should be sent immediately and the last sent over
	multiple batches as the last request to ensure that it is sent correctly.
*/
class CTestCase15 : public CBatchingTestCase
	{
public:
	static CTestCase15* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCase15();
	
private:
	CTestCase15(CHTTPTestUtils& aTestUtils);
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
	
#endif // __CTESTCASE15_H__
