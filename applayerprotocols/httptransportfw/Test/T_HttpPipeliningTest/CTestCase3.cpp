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

#include "CTestCase3.h"
#include "httptestutils.h"

const TInt KConn1TransCount = 3;
const TInt KConn2TransCount = 1;
const TInt KConn3TransCount = 1;
const TInt KConnectionCount = 3;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");
_LIT8(KTxtRawResponseWithClose, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\nConnection: Close\r\n\r\nhello!");

CTestCase3* CTestCase3::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCase3* self = new (ELeave) CTestCase3(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCase3::CTestCase3(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CTestCase3::ConstructL()
	{
	}

CTestCase3::~CTestCase3()
	{
	}

const TDesC& CTestCase3::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case 3");
	return KTxtTitle();
	}

TInt CTestCase3::TransactionCount(TInt aConnectionIndex) const
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
		case 2:
			{
			transCount = KConn3TransCount;
			} break;
		default:
			User::Invariant();
		}

	return transCount;
	}

TInt CTestCase3::TotalTransactionCount() const
	{
	return KConn1TransCount + KConn2TransCount + KConn3TransCount;
	}

TInt CTestCase3::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CTestCase3::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<TotalTransactionCount(), User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());

	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CTestCase3::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<TotalTransactionCount()) && (aConnectionIndex<KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CTestCase3::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS(aTransIndex<TotalTransactionCount(), User::Invariant());
	
	if( aConnectionIndex==0 && aTransIndex==2 )
		return KTxtRawResponseWithClose();

	return KTxtRawResponse();
	}


