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

#ifndef __CTESTCASE17_H__
#define __CTESTCASE17_H__

#include <e32base.h>
#include <http.h>

#include "CBatchingTestCase.h"

class CHTTPTestUtils;

/*	Batching 5 simple requesst to test server. The 3rd response has a
	Connection: Close header. This expects the first 3 requests are batched and
	the last 2 are not batched. This test case will ensure that time-out 
	batching is handled correctly with multiple connections requested by the server.
*/
class CTestCase17 : public CBatchingTestCase
	{
public:
	static CTestCase17* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCase17();
	
private:
	CTestCase17(CHTTPTestUtils& aTestUtils);
	void ConstructL();

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
	
#endif // __CTESTCASE17_H__
