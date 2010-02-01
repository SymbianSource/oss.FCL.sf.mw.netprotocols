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

#ifndef __CTESTCASE9_H__
#define __CTESTCASE9_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 3 simple requests as a HTTP/1.0 client. As HTTP/1.0 does not support pipelining,
	it expected that a connection is opened for each request
*/
class CTestCase9 : public CPipeliningTestCase
	{
public:
	static CTestCase9* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCase9();
	
private:
	CTestCase9(CHTTPTestUtils& aTestUtils);
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
	
#endif // __CTESTCASE9_H__
