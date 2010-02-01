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

#include "CTestCase22.h"
#include "httptestutils.h"

const TInt KTransactionCount = 1;
const TInt KConnectionCount = 1;

_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\nX-Oma-Drm-Separate-Delivery: \r\n\r\nhello!");
_LIT8(KCustomField, "X-Oma-Drm-Separate-Delivery");

CTestCase22* CTestCase22::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCase22* self = new (ELeave) CTestCase22(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCase22::CTestCase22(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CTestCase22::ConstructL()
	{
	}

CTestCase22::~CTestCase22()
	{
	}

const TDesC& CTestCase22::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case 22");
	return KTxtTitle();
	}

TInt CTestCase22::TotalTransactionCount() const
	{
	return KTransactionCount;
	}
	
TInt CTestCase22::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CTestCase22::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CTestCase22::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CTestCase22::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}

void CTestCase22::ProcessHeadersL(RHTTPTransaction aTrans)
	{
	RHTTPResponse resp = aTrans.Response();
	TInt status = resp.StatusCode();
	
	RStringF headerName = aTrans.Session().StringPool().OpenFStringL(KCustomField);
	CleanupClosePushL(headerName);

	THTTPHdrVal headerVal;
	RHTTPHeaders headers = aTrans.Response().GetHeaderCollection();
	TPtrC8 rawValue;
	headers.GetRawField(headerName,rawValue);
	
	TPtrC8 hdrVal(KNullDesC8);
	if(rawValue.Compare(hdrVal) != 0)
		{
		iTestUtils.LogIt ( _L("\nTesting custom header NULL value failed.") );	
		User::Leave(KErrArgument);
		}
	iTestUtils.LogIt ( _L("\nTesting custom header NULL value Passed.") );
	CleanupStack::PopAndDestroy(&headerName);
	}


