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

#include "CTestCase11.h"
#include "httptestutils.h"

const TInt KTransactionCount = 20;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");

CTestCase11* CTestCase11::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCase11* self = new (ELeave) CTestCase11(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCase11::CTestCase11(CHTTPTestUtils& aTestUtils)
: CBatchingTestCase(), iTestUtils(aTestUtils)
	{
	}

void CTestCase11::ConstructL()
	{
	}

CTestCase11::~CTestCase11()
	{
	}

const TDesC& CTestCase11::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case 11");
	return KTxtTitle();
	}

TInt CTestCase11::TotalTransactionCount() const
	{
	return KTransactionCount;
	}
	
TInt CTestCase11::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CTestCase11::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CTestCase11::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CTestCase11::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
