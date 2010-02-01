// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CPREQ1805.h"
#include "httptestutils.h"

const TInt KTransactionCount = 5;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.1 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");

CPREQ1805* CPREQ1805::NewL(CHTTPTestUtils& aTestUtils)
	{
	CPREQ1805* self = new (ELeave) CPREQ1805(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CPREQ1805::CPREQ1805(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CPREQ1805::ConstructL()
	{
	}

CPREQ1805::~CPREQ1805()
	{
	}

const TDesC& CPREQ1805::TestCaseName() const
	{
	_LIT(KTxtTitle, "Pipelining test for PREQ1805");
	return KTxtTitle();
	}

TInt CPREQ1805::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CPREQ1805::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CPREQ1805::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	// Stting HttpDataOptimiser to session 
	aSession.SetupHttpDataOptimiser(*this);
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CPREQ1805::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex-1 < KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CPREQ1805::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex-1 < KConnectionCount), User::Invariant());

	return KTxtRawResponse();
	}
	
void CPREQ1805::EncodeL (const TDesC8& aHttpData, HBufC8* &aEncodedData)
	{
	
	_LIT(KTxtMessage, "######## CPREQ1805::Encode ########\n");
	iTestUtils.LogIt(KTxtMessage);

	// Encoding: copying the same request data from aHttpData to aEncodedData
 	TRAPD(err,
		{
		aEncodedData=  aHttpData.AllocLC();
		CleanupStack::Pop();
		});	
	if(err != KErrNone)
		{
		_LIT(KTxtErrMsg, "CPREQ1805 - Leave in CPREQ1805::Encode: %D\n");
		iTestUtils.LogIt(KTxtErrMsg(),err);
		}
	}

void CPREQ1805::DecodeL (const TDesC8& aData, HBufC8*& aHttpData, TBool& aTransFail)
	{
	
	_LIT(KTxtMessage, "######## CPREQ1805::Decode ########\n");
	iTestUtils.LogIt(KTxtMessage);
	
	static int count = 0;
	TRAPD(err, 
		{
		aHttpData = aData.AllocLC();
		CleanupStack::Pop();	
		});
	if(err != KErrNone)
		{
		_LIT(KTxtErrMsg, "CPREQ1805 - Leave in CPREQ1805::Decode: %D\n");
		iTestUtils.LogIt(KTxtErrMsg(),err);
		}
		
	// Checking to fail the transaction.
	if(count == 2)
		aTransFail = ETrue;	
	else
		aTransFail = EFalse;
			
	count++;
	}
	

