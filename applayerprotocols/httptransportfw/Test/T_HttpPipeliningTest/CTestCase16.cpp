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

#include "CTestCase16.h"
#include "httptestutils.h"

const TInt KConn1TransCount = 3;
const TInt KConn2TransCount = 2;
const TInt KConnectionCount = 2;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawRequestWithClose, "GET / HTTP/1.1\r\nConnection: Close\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");


CTestCase16* CTestCase16::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCase16* self = new (ELeave) CTestCase16(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCase16::CTestCase16(CHTTPTestUtils& aTestUtils)
: CBatchingTestCase(), iTestUtils(aTestUtils)
	{
	}

void CTestCase16::ConstructL()
	{
	}

CTestCase16::~CTestCase16()
	{
	}

const TDesC& CTestCase16::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case 16");
	return KTxtTitle();
	}

TInt CTestCase16::TransactionCount(TInt aConnectionIndex) const
	{
	TInt transCount = 0;
	switch(aConnectionIndex)
		{
		case 0:
			{
			transCount = KConn1TransCount;
			} break;
		case 1:
			{
			transCount = KConn2TransCount;
			} break;
		default:
			User::Invariant();
		}

	return transCount;
	}

TInt CTestCase16::TotalTransactionCount() const
	{
	return KConn1TransCount+KConn2TransCount;
	}

TInt CTestCase16::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CTestCase16::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<TotalTransactionCount(), User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());

	if( aIndex==2 )
		{
		RHTTPTransaction trans = aSession.OpenTransactionL(uri, aClient, method);
		AddConnectionCloseHeaderL(trans);
		return trans;
		}

	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CTestCase16::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS(aTransIndex<TransactionCount(aConnectionIndex), User::Invariant());
	
	if( aConnectionIndex==0 && aTransIndex==2 )
		return KTxtRawRequestWithClose();

	return KTxtRawRequest();
	}
	
const TDesC8& CTestCase16::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS(aTransIndex<TransactionCount(aConnectionIndex), User::Invariant());
	
	return KTxtRawResponse();
	}
	
void CTestCase16::AddConnectionCloseHeaderL(RHTTPTransaction aTransaction)
	{
	RStringF name = aTransaction.Session().StringPool().StringF(HTTP::EConnection, RHTTPSession::GetTable());
	THTTPHdrVal value = aTransaction.Session().StringPool().StringF(HTTP::EClose, RHTTPSession::GetTable());

	aTransaction.Request().GetHeaderCollection().SetFieldL(name, value);
	}
