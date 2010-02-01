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

#include "CINC036384.h"
#include "httptestutils.h"

const TInt KTransactionCount = 1;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 OK \r\nTransfer-Encoding: chunked\r\n\r\n4\r\ntest\r\n4\r\ntest chunk that is bigger than the chunk size\r\n0\r\n\r\n");

CINC036384* CINC036384::NewL(CHTTPTestUtils& aTestUtils)
	{
	CINC036384* self = new (ELeave) CINC036384(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CINC036384::CINC036384(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CINC036384::ConstructL()
	{
	}

CINC036384::~CINC036384()
	{
	}

const TDesC& CINC036384::TestCaseName() const
	{
	_LIT(KTxtTitle, "Defect Fix INC036384");
	return KTxtTitle();
	}

TInt CINC036384::TotalTransactionCount() const
	{
	return KTransactionCount;
	//return 100;
	}

TInt CINC036384::ConnectionCount() const
	{
	return KConnectionCount;
	//return 0;
	}
	
RHTTPTransaction CINC036384::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	//_LIT8(KTxtUri, "http://www.cnn.com");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CINC036384::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CINC036384::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
