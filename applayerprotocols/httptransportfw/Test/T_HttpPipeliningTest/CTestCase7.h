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

#ifndef __CTESTCASE7_H__
#define __CTESTCASE7_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 10 simple requests, once the client has started, the 5th transaction is cancelled.
	It is expected that the cancelled transaction will fail but the rest must complete successfully.
	We expect that all the data would have been sent so any responses that have not been received
	will have to be resent.
*/
class CTestCase7 : public CPipeliningTestCase
	{
public:
	static CTestCase7* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCase7();
	
private:
	CTestCase7(CHTTPTestUtils& aTestUtils);
	void ConstructL();

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	TInt ExpectedError(RHTTPTransaction aTrans);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt RealExpectedConnectionCount() const;
	
private:
	CHTTPTestUtils&			iTestUtils;
	RHTTPTransaction		iTransToCancel;
	TInt					iTransCount;
	TBool					iIsTransCancelled;
	};
	
#endif // __CTESTCASE7_H__
