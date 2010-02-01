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

#include "T_PipeliningConfigFilter.h"
#include <httpstringconstants.h>
#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>

// 	Position of the Pipelinging Config Filter is ECache+10, therefore this test filter needs to go
// 	directlty below that filter i.e. (ECache+10)-1
const TInt KTestFilterPosition = (MHTTPFilter::ECache+10)-1;


CPipeliningConfigFilterTest* CPipeliningConfigFilterTest::NewL(RHTTPSession aSession, TInt aNumberOfTransactions, TBool aPipelinedTransactions)
	{
	CPipeliningConfigFilterTest* self = new (ELeave) CPipeliningConfigFilterTest(aNumberOfTransactions, aPipelinedTransactions);
	CleanupStack::PushL(self);
	self->ConstructL(aSession);
	CleanupStack::Pop(self); 
	return self;
	}

CPipeliningConfigFilterTest::CPipeliningConfigFilterTest(TInt aNumberOfTransactions, TBool aPipelinedTransactions)
: CBase(), iNumberOfTransactions(aNumberOfTransactions), iPipelinedTransactions(aPipelinedTransactions)
	{
	}

void CPipeliningConfigFilterTest::ConstructL(RHTTPSession aSession)
	{
	iTestFailed = ETrue;
	iSession = aSession;
	iStringPool = iSession.StringPool();

	// Register the filter for ESubmit to check outgoing requests
	_LIT8(KFilterName, "PipeliningConfigFilterTest");
	iFilterName = iStringPool.OpenFStringL(KFilterName);
	iSession.FilterCollection().AddFilterL(
										  *this, 
										  THTTPEvent::ESubmit,
										  KTestFilterPosition,
										  iFilterName
										  );
	}
	
CPipeliningConfigFilterTest::~CPipeliningConfigFilterTest()
	{
	if (iLoadCount)
		{
		// As already in a destructor, MHFUnload must not delete this again.
		iLoadCount = -1;
		iSession.FilterCollection().RemoveFilter(iFilterName);
		}
	iFilterName.Close();
	}

void CPipeliningConfigFilterTest::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch(aEvent.iStatus)
		{
	case THTTPEvent::ESubmit:
		{
		++iCurrentTransactionNumber;
		if (iCurrentTransactionNumber == iNumberOfTransactions)
			{
			CheckForPipeliningSupport(aTransaction);
			}
		} 
		break;
	default:
		{
		if (aEvent.iStatus< 0)
			{
			aTransaction.SendEventL(THTTPEvent::EFailed,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter ) ;
			}
		}
		break;
		}
	}

void CPipeliningConfigFilterTest::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CPipeliningConfigFilterTest::MHFRunError(TInt /*aError*/, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	// Something has gone wrong - fail the transaction.
	aTransaction.Fail();
	return KErrNone;
	}

TInt CPipeliningConfigFilterTest::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}

void CPipeliningConfigFilterTest::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	++iLoadCount;
	}

void CPipeliningConfigFilterTest::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle)
	{
	if (--iLoadCount)
		return;

	delete this;
	}

void CPipeliningConfigFilterTest::CheckForPipeliningSupport(RHTTPTransaction aTransaction)
	{
	RStringF pipeliningProperty = iStringPool.StringF(HTTP::EHttpPipelining, RHTTPSession::GetTable());
	THTTPHdrVal pipeliningValue;
	
	TBool pipeliningEnabled = ETrue;
	TBool found = aTransaction.PropertySet().Property(pipeliningProperty, pipeliningValue);
	if(found)
		{
		__ASSERT_DEBUG(pipeliningValue.Type() == THTTPHdrVal::KStrFVal, User::Invariant());
		pipeliningEnabled = (pipeliningValue.StrF().Index(RHTTPSession::GetTable()) != HTTP::EDisablePipelining);
		}
		
	if (pipeliningEnabled == iPipelinedTransactions)
		{
		iTestFailed = EFalse;
		}
	else
		iTestFailed = ETrue;
	}

TBool CPipeliningConfigFilterTest::GetTestResult()
	{
	return iTestFailed;
	}
