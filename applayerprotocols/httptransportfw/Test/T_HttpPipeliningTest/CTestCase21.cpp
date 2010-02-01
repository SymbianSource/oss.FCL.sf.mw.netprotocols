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

#include "CTestCase21.h"
#include "httptestutils.h"

const TInt KTransactionCount = 3;
const TInt KConnectionCount = 1;
const TInt KBufferSize = 26; // Batching buffer size in bytes
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");

CTestCase21* CTestCase21::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCase21* self = new (ELeave) CTestCase21(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCase21::CTestCase21(CHTTPTestUtils& aTestUtils)
: CBatchingTestCase(), iTestUtils(aTestUtils)
	{
	}

void CTestCase21::ConstructL()
	{
	}

CTestCase21::~CTestCase21()
	{
	}

const TDesC& CTestCase21::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case 21");
	return KTxtTitle();
	}

TInt CTestCase21::TotalTransactionCount() const
	{
	return KTransactionCount;
	}
	
TInt CTestCase21::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CTestCase21::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	
	// Buffer size needs to be reduced in the session property before the first transaction.
	if( aIndex == 0 )
		{
		RHTTPConnectionInfo connInfo = aSession.ConnectionInfo();
		connInfo.SetPropertyL(aSession.StringPool().StringF(HTTP::EBatchingBufferSize,RHTTPSession::GetTable()), KBufferSize);
		}
		
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CTestCase21::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CTestCase21::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
