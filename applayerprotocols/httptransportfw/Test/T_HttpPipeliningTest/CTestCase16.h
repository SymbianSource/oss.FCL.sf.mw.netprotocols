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

#ifndef __CTESTCASE16_H__
#define __CTESTCASE16_H__

#include <e32base.h>
#include <http.h>

#include "CBatchingTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 5 simple requests to test server with batching enabled. The 3rd request
	has a Connection: Close header. This expects the first 3 requests are batched and
	the last 2 are batched on a another connection. This test case will ensure that
	the time-out batching is correcly handled when multiple connections are used requested
	by the client.
*/
class CTestCase16 : public CBatchingTestCase
	{
public:
	static CTestCase16* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCase16();
	
private:
	CTestCase16(CHTTPTestUtils& aTestUtils);
	void ConstructL();
	void AddConnectionCloseHeaderL(RHTTPTransaction aTransaction);

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TransactionCount(TInt aConnectionIndex) const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt ConnectionCount() const;

private:
	CHTTPTestUtils&			iTestUtils;
	};
	
#endif // __CTESTCASE16_H__
