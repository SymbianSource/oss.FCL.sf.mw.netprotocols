// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CINC077703.h"

const TInt KTransactionCount = 1;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.0 302 Moved\nStatus: 302 Moved\nPragma: no-cache\nLocation: http://127.0.0.1\n");
_LIT8(KTxtRawResponse1, "HTTP/1.0 200 OK\nContent-Length: 12\n\nHello world!");

CINC077703* CINC077703::NewL(CHTTPTestUtils& aTestUtils)
	{
	CINC077703* self = new (ELeave) CINC077703(aTestUtils);
	return self;	
	}
	
CINC077703::~CINC077703()
	{
	
	}

CINC077703::CINC077703(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	
	}
	


const TDesC& CINC077703::TestCaseName() const
	{
	_LIT(KTxtTitle, "Defect Fix CINC077703");
	return KTxtTitle();	
	}
	
TInt CINC077703::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CINC077703::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CINC077703::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}
	
const TDesC8& CINC077703::GetRawRequest( TInt aConnectionIndex, TInt aTransIndex )
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex <= KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CINC077703::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex <= KConnectionCount), User::Invariant());
	if ( aConnectionIndex == 0 )
		return KTxtRawResponse();
	else
		return KTxtRawResponse1();
	}
	


