// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CRecvTimeOut.h"

const TInt KTransactionCount = 1;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n");
_LIT8(KTxtRawResponse, "HTTP/1.0 200 OK\nContent-Length: 12\n\nHello world!");

CRecvTimeOut* CRecvTimeOut::NewL(CHTTPTestUtils& aTestUtils)
	{
	CRecvTimeOut* self = new (ELeave) CRecvTimeOut(aTestUtils);
	return self;	
	}
	
CRecvTimeOut::~CRecvTimeOut()
	{
	
	}

CRecvTimeOut::CRecvTimeOut(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	
	}
	


const TDesC& CRecvTimeOut::TestCaseName() const
	{
	_LIT(KTxtTitle, "CRecvTimeOut");
	return KTxtTitle();	
	}
	
TInt CRecvTimeOut::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CRecvTimeOut::ConnectionCount() const
	{
	return KConnectionCount;
	}
	
RHTTPTransaction CRecvTimeOut::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	
	RHTTPTransaction trans = aSession.OpenTransactionL(uri, aClient, method); 
	//Set TimeOut Values
	RStringF sendTimeOutProp = aSession.StringPool().StringF(HTTP::ESendTimeOutValue, aSession.GetTable());
	THTTPHdrVal sendTimeOutPropValue;
	sendTimeOutPropValue.SetInt(10);
	trans.PropertySet().SetPropertyL(sendTimeOutProp, sendTimeOutPropValue);

	RStringF recvTimeOutProp = aSession.StringPool().StringF(HTTP::EReceiveTimeOutValue, aSession.GetTable());
	THTTPHdrVal recvTimeOutPropValue;
	recvTimeOutPropValue.SetInt(10);
	trans.PropertySet().SetPropertyL(recvTimeOutProp, recvTimeOutPropValue);
	
	return trans;
	}
	
const TDesC8& CRecvTimeOut::GetRawRequest( TInt aConnectionIndex, TInt aTransIndex )
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex <= KConnectionCount), User::Invariant());
	
	return KTxtRawRequest();
	}
	
const TDesC8& CRecvTimeOut::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<KTransactionCount) && (aConnectionIndex <= KConnectionCount), User::Invariant());
	
	return KTxtRawResponse();
	}
	


