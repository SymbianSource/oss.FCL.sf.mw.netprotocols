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


#include "CTestCasePipelineFallback.h"
#include "httptestutils.h"

const TInt KTransactionCount = 30; // Submit 30 transactions
const TInt KConnectionCount = 1;   
const TInt KBufferSize = 70; // Batching buffer size in bytes of 2 requests.
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");

CTestCasePipelineFallback* CTestCasePipelineFallback::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCasePipelineFallback* self = new (ELeave) CTestCasePipelineFallback(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCasePipelineFallback::CTestCasePipelineFallback(CHTTPTestUtils& aTestUtils)
: CBatchingTestCase(), iTestUtils(aTestUtils)
	{
	}

void CTestCasePipelineFallback::ConstructL()
	{
	}

CTestCasePipelineFallback::~CTestCasePipelineFallback()
	{
	}

const TDesC& CTestCasePipelineFallback::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case Pipeline Fallback");
	return KTxtTitle();
	}

TInt CTestCasePipelineFallback::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CTestCasePipelineFallback::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CTestCasePipelineFallback::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
    // Buffer size needs to be reduced in the session property before the first transaction.
    if( aIndex == 0 )
        {
        RHTTPConnectionInfo connInfo = aSession.ConnectionInfo();
        // Set the batching buffer size
        connInfo.SetPropertyL(aSession.StringPool().StringF(HTTP::EBatchingBufferSize,RHTTPSession::GetTable()), KBufferSize);
        
        // Set optimal pipelining
        RStringF strOptimalPipelineVal = aSession.StringPool().StringF(HTTP::EHttpEnableOptimalPipelining, RHTTPSession::GetTable());
        connInfo.SetPropertyL(aSession.StringPool().StringF(HTTP::EHttpOptimalPipelining, RHTTPSession::GetTable()), strOptimalPipelineVal);
        connInfo.SetPropertyL(aSession.StringPool().StringF(HTTP::EMaxNumTransportHandlers, RHTTPSession::GetTable()), 1);
        connInfo.SetPropertyL(aSession.StringPool().StringF(HTTP::EMaxNumTransactionsToPipeline,  RHTTPSession::GetTable()), 5);
        }
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CTestCasePipelineFallback::GetRawRequest(TInt /*aConnectionIndex*/, TInt /*aTransIndex*/)
	{
//	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CTestCasePipelineFallback::GetRawResponse(TInt /*aConnectionIndex*/, TInt /*aTransIndex*/)
	{
//	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount), User::Invariant());

	return KTxtRawResponse();
	}

void CTestCasePipelineFallback::IncFallingBack()
    {
    ++iFallingback;
    }

TBool  CTestCasePipelineFallback::FallingBack() const
    {
    return (iFallingback == 2); // Max connection failure is 2
    }



