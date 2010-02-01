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

#ifndef __CTESTCASE8_H__
#define __CTESTCASE8_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 3 simple requests to a HTTP/1.0 test server. 1.0 servers do not support pipelining
	so it expected that a connection is opened for each request
*/
class CTestCase8 : public CPipeliningTestCase
	{
public:
	static CTestCase8* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCase8();
	
private:
	CTestCase8(CHTTPTestUtils& aTestUtils);
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
	
#endif // __CTESTCASE8_H__
