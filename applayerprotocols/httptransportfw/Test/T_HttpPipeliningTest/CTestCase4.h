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

#ifndef __CTESTCASE4_H__
#define __CTESTCASE4_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 5 simple request to pipelining test server. The last request has a
	Connection: Close header. This expects the all the requests to be pieplined
	on one connection.
*/
class CTestCase4 : public CPipeliningTestCase
	{
public:
	static CTestCase4* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCase4();
	
private:
	CTestCase4(CHTTPTestUtils& aTestUtils);
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
	
#endif // __CTESTCASE4_H__
