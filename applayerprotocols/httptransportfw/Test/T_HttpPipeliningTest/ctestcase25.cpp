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

#include "ctestcase25.h"
#include "httptestutils.h"

const TInt KTransactionCount = 50;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");

CTestCase25* CTestCase25::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCase25* self = new (ELeave) CTestCase25(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCase25::CTestCase25(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CTestCase25::ConstructL()
	{
	}

CTestCase25::~CTestCase25()
	{
	}

const TDesC& CTestCase25::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case 25");
	return KTxtTitle();
	}

TInt CTestCase25::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CTestCase25::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CTestCase25::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}
	
const TDesC8& CTestCase25::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex<KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CTestCase25::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex<KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}

TBool CTestCase25::SetTransportHandlerPriority() const
	{
	return ETrue;
	}
	
