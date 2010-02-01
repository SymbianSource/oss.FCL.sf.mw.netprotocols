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

#ifndef __CTESTCASE22_H__
#define __CTESTCASE22_H__
#include <e32base.h>
#include <http.h>

#include "CBatchingTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 3 requests to test server with batching enabled. All requests are simple
	small requests but the buffer size is set the same size as the request. The request
	size is 26 bytes.
*/
class CTestCase22 : public CPipeliningTestCase
	{
public:
	static CTestCase22* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCase22();
	
private:
	CTestCase22(CHTTPTestUtils& aTestUtils);
	void ConstructL();

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TotalTransactionCount() const;
	TInt ConnectionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	void ProcessHeadersL(RHTTPTransaction aTrans);
	
private:
	CHTTPTestUtils&			iTestUtils;
	};



#endif /*__CTESTCASE22_H__*/


