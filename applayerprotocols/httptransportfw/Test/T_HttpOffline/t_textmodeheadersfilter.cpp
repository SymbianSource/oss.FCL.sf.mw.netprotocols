// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "t_textmodeheadersfilter.h"
#include "chttpclientfilter.h"

_LIT(KTitleStr, "HTTP TextMode Filter Test");

CTextModeHeadersFilterTest* CTextModeHeadersFilterTest::NewLC()
	{
	CTextModeHeadersFilterTest* me = new(ELeave) CTextModeHeadersFilterTest;
	CleanupStack::PushL(me);
	return me;
	}

CTextModeHeadersFilterTest::~CTextModeHeadersFilterTest()
	{
	}

void CTextModeHeadersFilterTest::DoRunL()
	{
	iExpectedStatusCode = 0;

	iSession.OpenL();
	CleanupClosePushL(iSession);
	iFilter = CHttpClientFilter::NewL(iSession); // owned by session
	TestConnectionHeaderL();
	CleanupStack::PopAndDestroy();
	}

const TDesC& CTextModeHeadersFilterTest::TestName()
	{
	return KTitleStr;
	}


void CTextModeHeadersFilterTest::DoCancel()
	{
	}

void CTextModeHeadersFilterTest::MHFRunL(RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	}

TInt CTextModeHeadersFilterTest::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

void CTextModeHeadersFilterTest::TestConnectionHeaderL()
	{
	iEngine->Utils().LogIt(_L("Creating transaction"));
	RStringPool p = iSession.StringPool();

	RHTTPTransaction transaction;
	transaction =  iSession.OpenTransactionL(TUriParser8(), *this, p.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	RHTTPHeaders requestHeaders = transaction.Request().GetHeaderCollection();

	THTTPHdrVal hdrVal;
	RStringF connection = p.StringF(HTTP::EConnection,RHTTPSession::GetTable());

	// Add just Connection:close
	hdrVal.SetStrF(p.StringF(HTTP::EClose,RHTTPSession::GetTable()));
	requestHeaders.SetFieldL(connection, hdrVal);
	iFilter->MHFRunL(transaction, THTTPEvent(THTTPEvent::ESubmit));

	if (requestHeaders.FieldPartsL(connection) != 1)
		FailTestL();

	// Add a bogus (accept) field to the connection header, should get removed
	hdrVal.SetStrF(p.StringF(HTTP::EAccept,RHTTPSession::GetTable()));
	requestHeaders.SetFieldL(connection, hdrVal);
	iFilter->MHFRunL(transaction, THTTPEvent(THTTPEvent::ESubmit));

	if (requestHeaders.FieldPartsL(connection) != 1)
		FailTestL();


	// Add a hop by hop header to the connection header, should stay
	hdrVal.SetStrF(p.StringF(HTTP::ETransferEncoding,RHTTPSession::GetTable()));
	requestHeaders.SetFieldL(connection, hdrVal);
	iFilter->MHFRunL(transaction, THTTPEvent(THTTPEvent::ESubmit));

	if (requestHeaders.FieldPartsL(connection) != 2)
		FailTestL();
	
	
	// Add a bogus (accept) field to the connection header and a hop by hop header.
		// Add a hop by hop header to the connection header, should stay
	hdrVal.SetStrF(p.StringF(HTTP::EAccept,RHTTPSession::GetTable()));
	requestHeaders.SetFieldL(connection, hdrVal);
	hdrVal.SetStrF(p.StringF(HTTP::ETransferEncoding,RHTTPSession::GetTable()));
	requestHeaders.SetFieldL(connection, hdrVal);
	iFilter->MHFRunL(transaction, THTTPEvent(THTTPEvent::ESubmit));

	if (requestHeaders.FieldPartsL(connection) != 3)
		FailTestL();
	
	}

