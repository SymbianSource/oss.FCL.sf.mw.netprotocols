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

#ifndef __CTESTCASE26_H__
#define __CTESTCASE26_H__

#include <e32base.h>
#include <http.h>

#include "CBatchingTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 14 simple request to test apache server to simulate a browser.
	First request to the homepage and following requests to the same server
	getting different resources.
*/
class CTestCase26 : public CBatchingTestCase
	{
public:
	static CTestCase26* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCase26();
	
private:
	CTestCase26(CHTTPTestUtils& aTestUtils);
	void ConstructL();

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt RealExpectedConnectionCount() const;
	TBool SetTransportHandlerPriority() const;
	
private:
	CHTTPTestUtils&			iTestUtils;
	};
	
#endif // __CTESTCASE26_H__
