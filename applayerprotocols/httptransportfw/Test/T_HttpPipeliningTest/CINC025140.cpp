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

#include "CINC025140.h"
#include "httptestutils.h"

const TInt KTransactionCount = 1;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 \r\nContent-Length: 6\r\n\r\nhello!");

CINC025140* CINC025140::NewL(CHTTPTestUtils& aTestUtils)
	{
	CINC025140* self = new (ELeave) CINC025140(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CINC025140::CINC025140(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CINC025140::ConstructL()
	{
	}

CINC025140::~CINC025140()
	{
	}

const TDesC& CINC025140::TestCaseName() const
	{
	_LIT(KTxtTitle, "Defect Fix INC025140");
	return KTxtTitle();
	}

TInt CINC025140::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CINC025140::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CINC025140::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CINC025140::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CINC025140::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
