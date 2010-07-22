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

#include "ctestcase26.h"
#include "httptestutils.h"

const TInt KTransactionCount = 14;

CTestCase26* CTestCase26::NewL(CHTTPTestUtils& aTestUtils)
	{
	CTestCase26* self = new (ELeave) CTestCase26(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestCase26::CTestCase26(CHTTPTestUtils& aTestUtils)
: CBatchingTestCase(), iTestUtils(aTestUtils)
	{
	}

void CTestCase26::ConstructL()
	{
	}

CTestCase26::~CTestCase26()
	{
	}

const TDesC& CTestCase26::TestCaseName() const
	{
	_LIT(KTxtTitle, "Test Case 26");
	return KTxtTitle();
	}

TInt CTestCase26::TotalTransactionCount() const
	{
	return KTransactionCount;
	}
	
RHTTPTransaction CTestCase26::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS(aIndex<KTransactionCount, User::Invariant());
	
	TPtrC8 uriDes;
	
	switch(aIndex)
		{
		case 0:
			{
			uriDes.Set(_L8("http://10.29.23.12"));
			} break;
		case 1:
			{
			uriDes.Set(_L8("http://10.29.23.12/manual/index.html"));
			} break;
		case 2:
			{
			uriDes.Set(_L8("http://10.29.23.12/manual/new_features_1_3.html"));
			} break;
		case 3:
			{
			uriDes.Set(_L8("http://10.29.23.12/wap/suite-08Jun2000/"));
			} break;
		case 4:
			{
			uriDes.Set(_L8("http://10.29.23.12/wap/suite-20Jun2000/"));
			} break;
		case 5:
			{
			uriDes.Set(_L8("http://10.29.23.12/manual/upgrading_to_1_3.html"));
			} break;
		case 6:
			{
			uriDes.Set(_L8("http://10.29.23.12/manual/install.html"));
			} break;
		case 7:
			{
			uriDes.Set(_L8("http://10.29.23.12/manual/invoking.html"));
			} break;
		case 8:
			{
			uriDes.Set(_L8("http://10.29.23.12/stats.html"));
			} break;
		case 9:
			{
			uriDes.Set(_L8("http://10.29.23.12/manual/stopping.html"));
			} break;
		case 10:
			{
			uriDes.Set(_L8("http://10.29.23.12/manual/mod/directives.html"));
			} break;
		case 11:
			{
			uriDes.Set(_L8("http://10.29.23.12/manual/images/custom_errordocs.gif"));
			} break;
		case 12:
			{
			uriDes.Set(_L8("http://10.29.23.12/manual/images/home.gif"));
			} break;
		case 13:
			{
			uriDes.Set(_L8("http://10.29.23.12/manual/images/index.gif"));
			} break;
		default:
			// Shouldn't happen!
			User::Invariant();
			break;
		}
	
	
	RStringF method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
	TUriParser8 uri; 
	uri.Parse(uriDes);
	return aSession.OpenTransactionL(uri, aClient, method);
	}

const TDesC8& CTestCase26::GetRawRequest(TInt /*aConnectionIndex*/, TInt /*aTransIndex*/)
	{
	// This method shoulod never be called as we are going to a live server
	User::Invariant();
	
	return KNullDesC8();
	}
	
const TDesC8& CTestCase26::GetRawResponse(TInt /*aConnectionIndex*/, TInt /*aTransIndex*/)
	{
	// This method shoulod never be called as we are going to a live server
	User::Invariant();
	
	return KNullDesC8();
	}
	
	
TInt CTestCase26::RealExpectedConnectionCount() const
	{
	return 1;
	}
	
TBool CTestCase26::SetTransportHandlerPriority() const
	{
	return ETrue;
	}
