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

#include "CHeaderDecode.h"
#include "httptestutils.h"

const TInt KTransactionCount = 1;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 OK\r\nWWW-Authenticate: BASIC realm=\r\nContent-Length: 6\r\nContent-Length: 6\r\n\r\nhello!");

CHeaderDecode* CHeaderDecode::NewL(CHTTPTestUtils& aTestUtils)
	{
	CHeaderDecode* self = new (ELeave) CHeaderDecode(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CHeaderDecode::CHeaderDecode(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CHeaderDecode::ConstructL()
	{
	}

CHeaderDecode::~CHeaderDecode()
	{
	}

const TDesC& CHeaderDecode::TestCaseName() const
	{
	_LIT(KTxtTitle, "Header Decode");
	return KTxtTitle();
	}

TInt CHeaderDecode::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CHeaderDecode::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CHeaderDecode::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CHeaderDecode::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CHeaderDecode::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
	
void CHeaderDecode::ProcessHeadersL(RHTTPTransaction aTrans)
	{
	RStringPool stringPool = aTrans.Session().StringPool();
	RHTTPHeaders headers = aTrans.Response().GetHeaderCollection();
	THTTPHdrVal hdrVal;

	//Check Content-Length header parameter is stored correctly
	RStringF contentLengthStr = stringPool.StringF(HTTP::EContentLength, aTrans.Session().GetTable());
	User::LeaveIfError(headers.GetField(contentLengthStr,0,hdrVal));

	if(headers.FieldPartsL(contentLengthStr) != 1)
		User::Leave(KErrArgument);

	if (hdrVal.Int() != 6)
		User::Leave(KErrArgument);
	}
