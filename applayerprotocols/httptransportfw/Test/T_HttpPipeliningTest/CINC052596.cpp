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

#include "CINC052596.h"
#include "httptestutils.h"

const TInt KTransactionCount = 2;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.0 200 OK\r\nContent-Length: 6\r\nConnection: keep-alive\r\n\r\nhello!");

CINC052596* CINC052596::NewL(CHTTPTestUtils& aTestUtils)
	{
	CINC052596* self = new (ELeave) CINC052596(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CINC052596::CINC052596(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CINC052596::ConstructL()
	{
	}

CINC052596::~CINC052596()
	{
	}

const TDesC& CINC052596::TestCaseName() const
	{
	_LIT(KTxtTitle, "Defect Fix INC037440");
	return KTxtTitle();
	}

TInt CINC052596::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CINC052596::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CINC052596::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CINC052596::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CINC052596::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
	
void CINC052596::ProcessHeadersL(RHTTPTransaction aTrans)
	{
	RStringF haaderName = aTrans.Session().StringPool().StringF(HTTP::EWWWAuthenticate, aTrans.Session().GetTable());
	THTTPHdrVal headerVal;
	RHTTPHeaders headers = aTrans.Response().GetHeaderCollection();
	User::LeaveIfError(headers.GetField(haaderName, 0, headerVal));
	}

