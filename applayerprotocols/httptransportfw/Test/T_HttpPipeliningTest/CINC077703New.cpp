// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CINC077703New.h"

const TInt KTransactionCount = 1;
const TInt KConnectionCount = 1;

_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.0 302 Moved\nStatus: 302 Moved\nPragma: no-cache\nContent-Type: text/html\nContent-Length: 6\nLocation: http://127.0.0.1\n\nHello!");
_LIT8(KTxtRawResponse1, "HTTP/1.0 302 Moved\nStatus: 302 Moved\nPragma: no-cache\nConnection: close\nLocation: http://127.0.0.1\n");
_LIT8(KTxtRawResponse2, "HTTP/1.0 200 OK\nContent-Length: 12\n\nHello world!");

CINC077703New* CINC077703New::NewL(CHTTPTestUtils& aTestUtils)
	{
	CINC077703New* self = new (ELeave) CINC077703New(aTestUtils);
	return self;	
	}
	
CINC077703New::~CINC077703New()
	{
	
	}

CINC077703New::CINC077703New(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	
	}
	
const TDesC& CINC077703New::TestCaseName() const
	{
	_LIT(KTxtTitle, "Defect Fix CINC077703 New");
	return KTxtTitle();	
	}
	
TInt CINC077703New::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CINC077703New::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CINC077703New::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}
	
const TDesC8& CINC077703New::GetRawRequest( TInt aConnectionIndex, TInt aTransIndex )
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex <= (KConnectionCount+1)), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CINC077703New::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (KConnectionCount+1), User::Invariant());
	if ( aConnectionIndex == 0 )
		return KTxtRawResponse();
	if ( aConnectionIndex == 1 )
		return KTxtRawResponse1();
	else
		return KTxtRawResponse2();
	}
