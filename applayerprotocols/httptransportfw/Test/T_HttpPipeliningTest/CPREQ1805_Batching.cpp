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

#include "CPREQ1805_Batching.h"
#include "httptestutils.h"

const TInt KTransactionCount = 5;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");

CPREQ1805Batching* CPREQ1805Batching::NewL(CHTTPTestUtils& aTestUtils)
	{
	CPREQ1805Batching* self = new (ELeave) CPREQ1805Batching(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CPREQ1805Batching::CPREQ1805Batching(CHTTPTestUtils& aTestUtils)
: CBatchingTestCase(), iTestUtils(aTestUtils)
	{
	}

void CPREQ1805Batching::ConstructL()
	{
	}

CPREQ1805Batching::~CPREQ1805Batching()
	{
	}

const TDesC& CPREQ1805Batching::TestCaseName() const
	{
	_LIT(KTxtTitle, "Pipelining test for PREQ1805");
	return KTxtTitle();
	}

TInt CPREQ1805Batching::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CPREQ1805Batching::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CPREQ1805Batching::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	// Stting HttpDataOptimiser to session 
	aSession.SetupHttpDataOptimiser(*this);
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CPREQ1805Batching::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CPREQ1805Batching::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
	
void CPREQ1805Batching::EncodeL (const TDesC8& aHttpData, HBufC8* &aEncodedData)
	{
	
	_LIT(KTxtMessage, "######## CPREQ1805Batching::Encode ########\n");
	iTestUtils.LogIt(KTxtMessage);

	// Encoding: copying the same request data from aHttpData to aEncodedData
 	TRAPD(err,
		{
		aEncodedData=  aHttpData.AllocLC();
		CleanupStack::Pop();
		});	
	if(err != KErrNone)
		{
		_LIT(KTxtErrMsg, "CPREQ1805Batching - Leave in CPREQ1805Batching::Encode: %D\n");
		iTestUtils.LogIt(KTxtErrMsg(),err);
		}
	}

void CPREQ1805Batching::DecodeL (const TDesC8& aData, HBufC8*& aHttpData, TBool& aTransFail)
	{
	
	_LIT(KTxtMessage, "######## CPREQ1805Batching::Decode ########\n");
	iTestUtils.LogIt(KTxtMessage);
	
	static int count = 0;
	TRAPD(err, 
		{
		aHttpData = aData.AllocLC();
		CleanupStack::Pop();	
		});
	if(err != KErrNone)
		{
		_LIT(KTxtErrMsg, "CPREQ1805Batching - Leave in CPREQ1805Batching::Decode: %D\n");
		iTestUtils.LogIt(KTxtErrMsg(),err);
		}
		
	// Checking to fail the transaction.
	//if(count == 2)
		//aTransFail = ETrue;	
	//else
		aTransFail = EFalse;
			
	count++;
	}
	

