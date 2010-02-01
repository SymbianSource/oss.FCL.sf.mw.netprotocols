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

#include "CTestCase15.h"
#include "httptestutils.h"

const TInt KTransactionCount = 3;
const TInt KConnectionCount = 1;
const TInt KBufferSize = 25; // Batching buffer size in bytes
_LIT8(KTxtRawRequest1, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawRequest2, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nThis_Is_A_Long_Header_Name: This_Is_A_Long_Header_Value_abcdefghijklmnopqrstuvwxyz1234567890\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");

CTestCase15* CTestCase15::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCase15* self = new (ELeave) CTestCase15(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCase15::CTestCase15(CHTTPTestUtils& aTestUtils)
: CBatchingTestCase(), iTestUtils(aTestUtils)
	{
	}

void CTestCase15::ConstructL()
	{
	}

CTestCase15::~CTestCase15()
	{
	}

const TDesC& CTestCase15::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case 15");
	return KTxtTitle();
	}

TInt CTestCase15::TotalTransactionCount() const
	{
	return KTransactionCount;
	}
	
TInt CTestCase15::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CTestCase15::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
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
		
	// If this is the 2nd request add large amount of header data to make the request
	// larger than the buffer size
	if( aIndex == 2 )
		{
		// Add a fairly large header
		RHTTPTransaction trans = aSession.OpenTransactionL(uri, aClient, method);
		RHTTPHeaders headers = trans.Request().GetHeaderCollection();
		
		_LIT8(KTxtHeaderName, "This_Is_A_Long_Header_Name");
		_LIT8(KTxtHeaderValue, "This_Is_A_Long_Header_Value_abcdefghijklmnopqrstuvwxyz1234567890");
		RStringF headerStrName = aSession.StringPool().OpenFStringL(KTxtHeaderName());
		CleanupClosePushL(headerStrName);
		RStringF headerStrValue = aSession.StringPool().OpenFStringL(KTxtHeaderValue());
		CleanupClosePushL(headerStrValue);
		THTTPHdrVal headerValue(headerStrValue);
		headers.SetFieldL(headerStrName, headerValue);
		CleanupStack::PopAndDestroy(2, &headerStrName);
		
		return trans;
		}
		
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CTestCase15::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());
	
	if( aTransIndex == 2 )
		return KTxtRawRequest2();
		
	return KTxtRawRequest1();
	}
	
const TDesC8& CTestCase15::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
