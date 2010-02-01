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

#include "CINC078630.h"
#include "httptestutils.h"

const TInt KTransactionCount = 1;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "POST Microsoft-Server-ActiveSync?DeviceId=Foo&DeviceType=9999&Cmd=Ping HTTP/1.1\r\nHost: 127.0.0.1\r\nAccept-Language: en-us\r\nMS-ASProtocolVersion: 2.5\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");

CINC078630* CINC078630::NewL(CHTTPTestUtils& aTestUtils)
	{
	CINC078630* self = new (ELeave) CINC078630(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CINC078630::CINC078630(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CINC078630::ConstructL()
	{
	}

CINC078630::~CINC078630()
	{
	}

const TDesC& CINC078630::TestCaseName() const
	{
	_LIT(KTxtTitle, "Defect Fix INC078630");
	return KTxtTitle();
	}

TInt CINC078630::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CINC078630::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CINC078630::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	_LIT8(KTxtUri, "Microsoft-Server-ActiveSync?DeviceId=Foo&DeviceType=9999&Cmd=Ping");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	
	
	RStringF method = aSession.StringPool().StringF(HTTP::EPOST,RHTTPSession::GetTable());
	RHTTPTransaction trans = aSession.OpenTransactionL(uri, aClient, method);
	RHTTPHeaders headers = trans.Request().GetHeaderCollection();
		
		
	_LIT8(KTxtHeaderName0, "Host");
	_LIT8(KTxtHeaderValue0, "127.0.0.1");
	RStringF headerStrName0 = aSession.StringPool().OpenFStringL(KTxtHeaderName0());
	CleanupClosePushL(headerStrName0);
	RStringF headerStrValue0 = aSession.StringPool().OpenFStringL(KTxtHeaderValue0());
	CleanupClosePushL(headerStrValue0);
	THTTPHdrVal headerValue0(headerStrValue0);
	headers.SetFieldL(headerStrName0, headerValue0);
	CleanupStack::PopAndDestroy(2, &headerStrName0);
	
	_LIT8(KTxtHeaderName, "Accept-Language");
	_LIT8(KTxtHeaderValue, "en-us");
	RStringF headerStrName = aSession.StringPool().OpenFStringL(KTxtHeaderName());
	CleanupClosePushL(headerStrName);
	RStringF headerStrValue = aSession.StringPool().OpenFStringL(KTxtHeaderValue());
	CleanupClosePushL(headerStrValue);
	THTTPHdrVal headerValue(headerStrValue);
	headers.SetFieldL(headerStrName, headerValue);
	CleanupStack::PopAndDestroy(2, &headerStrName);
	
	_LIT8(KTxtHeaderName1, "MS-ASProtocolVersion");
	_LIT8(KTxtHeaderValue1, "2.5");
	RStringF headerStrName1 = aSession.StringPool().OpenFStringL(KTxtHeaderName1());
	CleanupClosePushL(headerStrName1);
	RStringF headerStrValue1 = aSession.StringPool().OpenFStringL(KTxtHeaderValue1());
	CleanupClosePushL(headerStrValue1);
	THTTPHdrVal headerValue1(headerStrValue1);
	headers.SetFieldL(headerStrName1, headerValue1);
	CleanupStack::PopAndDestroy(2, &headerStrName1);
	
	return trans;
	
	}

const TDesC8& CINC078630::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CINC078630::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
	
