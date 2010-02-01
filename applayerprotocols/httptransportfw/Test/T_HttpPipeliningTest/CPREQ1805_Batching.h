// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CPREQ1805_Batching_H__
#define __CPREQ1805_Batching_H__

#include <e32base.h>
#include <http.h>

#include "CBatchingTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 5 simple request to pipelining test server.
*/
class CPREQ1805Batching : public CBatchingTestCase, public MHttpDataOptimiser
	{
public:
	static CPREQ1805Batching* NewL(CHTTPTestUtils& aTestUtils);
	~CPREQ1805Batching();
	
private:
	CPREQ1805Batching(CHTTPTestUtils& aTestUtils);
	void ConstructL();

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt ConnectionCount() const;
	
	
protected:
	//These methods from MHttpDataOptimiser
	virtual void EncodeL (const TDesC8& aHttpData, HBufC8*& aEncodedData);
	virtual void DecodeL (const TDesC8& aData, HBufC8*& aHttpData, TBool& aTransFail);
			
private:
	CHTTPTestUtils&			iTestUtils;
	};
	
#endif // __CPREQ1805_Batching_H__
