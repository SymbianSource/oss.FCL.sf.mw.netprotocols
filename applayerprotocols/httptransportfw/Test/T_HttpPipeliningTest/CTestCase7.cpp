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

#include "CTestCase7.h"
#include "httptestutils.h"

const TInt KTransactionCount = 10;

CTestCase7* CTestCase7::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCase7* self = new (ELeave) CTestCase7(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCase7::CTestCase7(CHTTPTestUtils& aTestUtils)
: CPipeliningTestCase(), iTestUtils(aTestUtils), iTransCount(KTransactionCount)
	{
	}

void CTestCase7::ConstructL()
	{
	}

CTestCase7::~CTestCase7()
	{
	}

const TDesC& CTestCase7::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case 7");
	return KTxtTitle();
	}

TInt CTestCase7::TotalTransactionCount() const
	{
	return iTransCount;
	}
	
	
TInt CTestCase7::RealExpectedConnectionCount() const
 	{
 	return 1;
 	}	
 	
	
RHTTPTransaction CTestCase7::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	_LIT8(KTxtUri, "http://10.29.23.60/pipeline/hello.txt");
	TUriParser8 uri; 
	uri.Parse(KTxtUri());

	if( aIndex == 4 )
		{
		iTransToCancel = aSession.OpenTransactionL(uri, aClient, method);
		return iTransToCancel;
		}

	return aSession.OpenTransactionL(uri, aClient, method);
	}
	
const TDesC8& CTestCase7::GetRawRequest(TInt /*aConnectionIndex*/, TInt /*aTransIndex*/)
	{
	// This method shoulod never be called as we are going to a live server
	User::Invariant();
	
	return KNullDesC8();
	}
	
const TDesC8& CTestCase7::GetRawResponse(TInt /*aConnectionIndex*/, TInt /*aTransIndex*/)
	{
	// This method shoulod never be called as we are going to a live server
	User::Invariant();
	
	return KNullDesC8();
	}
	
TInt CTestCase7::ExpectedError(RHTTPTransaction aTrans)
	{
	if(!iIsTransCancelled)
		{
		if(aTrans.Id()==3)
			{
			iTransToCancel.Cancel();
			iIsTransCancelled = ETrue;
			--iTransCount;
			}
		}
		
	if( iTransToCancel.Id() == aTrans.Id() )
		return KErrCancel;

	return KErrNone;
	}
