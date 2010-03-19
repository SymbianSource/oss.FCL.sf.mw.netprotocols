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

#include "CDEF143497.h"
#include "httptestutils.h"
#include "testhttpfilter.h"	

const TInt KTransactionCount = 1;
const TInt KConnectionCount = 1;
_LIT8(KTxtRawRequest1, "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nUser-Agent: 3gpp-gba\r\n\r\n");
_LIT8(KTxtRawResponse1, "HTTP/1.1 401 Authorization Required\r\nWWW-Authenticate: Basic realm=\"Secure Area\"\r\nContent-Length: 6\r\n\r\nhello!");
_LIT8(KTxtRawRequest2,"GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nAuthorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==\r\nUser-Agent: 3gpp-gba\r\n\r\n");
_LIT8(KTxtRawResponse2, "HTTP/1.0 200 Ok\r\nContent-Length: 6\r\n\r\nhello!");
_LIT8(KNullResponse, " ");

CDEF143497 * CDEF143497 ::NewL(CHTTPTestUtils& aTestUtils)
	{
	CDEF143497 * self = new (ELeave) CDEF143497 (aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CDEF143497 ::CDEF143497 (CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils)
	{
	}

void CDEF143497 ::ConstructL()
	{
	}

CDEF143497 ::~CDEF143497 ()
	{
	}

const TDesC& CDEF143497 ::TestCaseName() const
	{
	_LIT(KTxtTitle, "Defect Fix CDEF143497");
	return KTxtTitle();
	}

TInt CDEF143497 ::TotalTransactionCount() const
	{
	return KTransactionCount;
	}

TInt CDEF143497 ::ConnectionCount() const
	{
	return KConnectionCount;
	}


RHTTPTransaction CDEF143497 ::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	CTestFilterInterface::InstallFilterL(aSession);
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://127.0.0.1");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CDEF143497 ::GetRawRequest(TInt /*aConnectionIndex*/, TInt /*aTransIndex*/)
	{
	static TInt RequestCounter =0;
	if(RequestCounter==0)
	    {
	    RequestCounter++;
	    return KTxtRawRequest1();
	    }
	if(RequestCounter==1)
	    {
	   	return KTxtRawRequest2();
	    }
	else
		return KNullResponse();
	}
	
const TDesC8& CDEF143497 ::GetRawResponse(TInt /*aConnectionIndex*/, TInt /*aTransIndex*/)
	{
	static TInt ResponseCounter =0;
	if(ResponseCounter==0)
	    { 
	    ResponseCounter++;
	    return KTxtRawResponse1();
	    }
	if(ResponseCounter==1)
		return KTxtRawResponse2();
	else
		return KNullResponse();

	}

	TInt CDEF143497::ExpectedError(RHTTPTransaction /*aTrans*/)
	{
	return KErrNone;
	}

TBool CDEF143497::GetCredentialsL(const TUriC8& /*aURI*/, RString aRealm, RStringF /*aAuthenticationType*/, RString& aUsername, RString& aPassword)
	{
	_LIT8(KUsername, "Aladdin");
	_LIT8(KPassword, "open sesame");
	RStringPool p = aRealm.Pool();
	aUsername = p.OpenStringL(KUsername);
	aPassword = p.OpenStringL(KPassword);
	return ETrue;
	}

