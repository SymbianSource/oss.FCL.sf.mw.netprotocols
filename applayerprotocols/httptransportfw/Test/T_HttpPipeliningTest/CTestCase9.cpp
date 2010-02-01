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

#include "CTestCase9.h"
#include "httptestutils.h"

const TInt KTransactionCount = 3;
const TInt KConnectionCount = 3;
_LIT8(KTxtRawRequest, "GET / HTTP/1.0\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.0 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");

CTestCase9* CTestCase9::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCase9* self = new (ELeave) CTestCase9(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCase9::CTestCase9(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CTestCase9::ConstructL()
	{
	}

CTestCase9::~CTestCase9()
	{
	}

const TDesC& CTestCase9::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case 9");
	return KTxtTitle();
	}

TInt CTestCase9::TransactionCount(TInt /*aConnectionIndex*/) const
	{
	return 1;
	}

TInt CTestCase9::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CTestCase9::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CTestCase9::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	// Change the session properties to be a HTTP/1.0 client
	if( aIndex == 0 )
		{
		RHTTPConnectionInfo connInfo = aSession.ConnectionInfo();
		RStringPool strPool = aSession.StringPool();
		THTTPHdrVal http10( strPool.StringF( HTTP::EHttp10, RHTTPSession::GetTable() ) );
		connInfo.SetPropertyL( strPool.StringF( HTTP::EHTTPVersion,RHTTPSession::GetTable() ), http10 );
		}

	RStringF method = aSession.StringPool().StringF(HTTP::EGET, RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}
	
const TDesC8& CTestCase9::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex<KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CTestCase9::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex<KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
	
