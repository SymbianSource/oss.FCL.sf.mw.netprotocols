/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


#ifndef __CTESTCASEPIPELINEFALLBACK_H__
#define __CTESTCASEPIPELINEFALLBACK_H__

#include <e32base.h>
#include <http.h>

#include "CBatchingTestCase.h"

class CHTTPTestUtils;

/*	
*/
class CTestCasePipelineFallback : public CBatchingTestCase
	{
public:
	static CTestCasePipelineFallback* NewL(CHTTPTestUtils& aTestUtils);
	~CTestCasePipelineFallback();
	TBool  FallingBack() const;
	void IncFallingBack();
private:
	CTestCasePipelineFallback(CHTTPTestUtils& aTestUtils);
	void ConstructL();

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt ConnectionCount() const;
	
private:
	CHTTPTestUtils&			iTestUtils;
	TInt                    iFallingback;
	};
	
#endif // __CTESTCASEPIPELINEFALLBACK_H__
