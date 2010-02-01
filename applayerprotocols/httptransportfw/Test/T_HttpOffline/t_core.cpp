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

#include <e32std.h>
#include <e32test.h>
#include <f32file.h>
#include "t_core.h"

_LIT8(KPost, "POST");
_LIT8(KHTTPText, "HTTP");
_LIT8(KGoneText, "This resource is gone, it is an ex-resource.");


class CMyFilter : public CBase, public MHTTPFilter
	{
public:
	static CMyFilter* NewL();
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle);
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle);

	TInt iLoadCount;
	TInt iUnloadCount;
	};

CMyFilter* CMyFilter::NewL()
	{
	return new (ELeave) CMyFilter;
	}

void CMyFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	iLoadCount++;
	}

void CMyFilter::MHFUnload(RHTTPSession, THTTPFilterHandle)
	{
	iUnloadCount++;
	}

void CMyFilter::MHFRunL(RHTTPTransaction /*aTransaction*/,
						const THTTPEvent& /*aEvent*/)
	{
	}

void CMyFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CMyFilter::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

TInt CMyFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}


CHTTPCoreTest* CHTTPCoreTest::NewLC()
	{
	CHTTPCoreTest* t = new (ELeave) CHTTPCoreTest;
	CleanupStack::PushL(t);
	return t;
	}

void CHTTPCoreTest::DoRunL()
	{
	// Firstly test the creation of the first transaction with
	// different numbers of filters added first. This is to make sure
	// that leak tests exhaustively test session setup despite the
	// granularity of the filter queue array.
	for (TInt ii = 0; ii < 10; ii++)
		{
		CMyFilter* filter = CMyFilter::NewL();
		CleanupStack::PushL(filter);
		RHTTPSession session;
		CleanupClosePushL(session);
		session.OpenL();
		for (TInt jj = 0; jj < ii; jj++)
			{
			session.FilterCollection().AddFilterL(*filter,
												  THTTPEvent(THTTPEvent::EAnyTransactionEvent),
												  RStringF(), KAnyStatusCode,
												  1, RStringF());
			}
		TRAPD(error, session.OpenTransactionL(TUriParser8(), *this, 
								 session.StringPool().OpenFStringL(KPost)));
		if (error)
			{
			// If this fails, the session should either be completeley
			// created, or not created at all. Check the number of
			// filters is a number we'd expect.
			THTTPFilterIterator iter = session.FilterCollection().Query();
			TInt count;
			for (count = 0; !iter.AtEnd(); ++iter, ++count)
				;

			// Assume we install 7 filters.
			if (count != ii && count != ii + 11)
				User::Leave(-1);
			else
				User::Leave(error);
			}
		CleanupStack::PopAndDestroy(2);
		}


	iExpectedStatusCode = 0;
	CMyFilter* filter = CMyFilter::NewL();
	CleanupStack::PushL(filter);
	RHTTPSession a;
	CleanupClosePushL(a);
	a.OpenL();
	iEngine->Utils().LogIt(_L("Adding filters"));
	a.FilterCollection().AddFilterL(*filter, THTTPEvent(THTTPEvent::EAnyTransactionEvent),
				 RStringF(), KAnyStatusCode, 1, RStringF());
	a.FilterCollection().AddFilterL(*filter, THTTPEvent(THTTPEvent::EAnyTransactionEvent),
				 RStringF(), KAnyStatusCode, 2, RStringF());
	a.FilterCollection().AddFilterL(*filter, THTTPEvent(THTTPEvent::EAnyTransactionEvent),
				 RStringF(), KAnyStatusCode, 3, RStringF());
	a.FilterCollection().AddFilterL(*filter, THTTPEvent(THTTPEvent::EAnyTransactionEvent), 
				 RStringF(), KAnyStatusCode, 4, RStringF());
	a.FilterCollection().AddFilterL(*filter, THTTPEvent(THTTPEvent::EAnyTransactionEvent), 
				 RStringF(), KAnyStatusCode, 5, RStringF());
	a.FilterCollection().AddFilterL(*filter, THTTPEvent(THTTPEvent::EAnyTransactionEvent), 
				 RStringF(), KAnyStatusCode, 6, RStringF());
	a.FilterCollection().AddFilterL(*filter, THTTPEvent(THTTPEvent::EAnyTransactionEvent), 
				 RStringF(), KAnyStatusCode, 7, RStringF());
	a.FilterCollection().AddFilterL(*filter, THTTPEvent(THTTPEvent::EAnyTransactionEvent), 
				 RStringF(), KAnyStatusCode, 8, RStringF());
	a.FilterCollection().AddFilterL(*filter, THTTPEvent(THTTPEvent::EAnyTransactionEvent), 
				 RStringF(), KAnyStatusCode, 9, RStringF());
	if (filter->iLoadCount != 9)
		User::Leave(-1);
	iEngine->Utils().LogIt(_L("Creating transaction"));
	RHTTPTransaction b;
	b = a.OpenTransactionL(TUriParser8(), *this, 
							 a.StringPool().OpenFStringL(KPost));
	RHTTPRequest c;
	c = b.Request();
	iEngine->Utils().LogIt(_L("Checking request lacks a body"));
	if (c.HasBody())
		User::Leave(-1);
	c.SetBody(*this);
	iEngine->Utils().LogIt(_L("Checking addition of a body"));
	if (c.Body()!= this)
		User::Leave(-1);
	iEngine->Utils().LogIt(_L("Checking request now has a body"));
	if ((!c.HasBody())) User::Leave(-1);
	iEngine->Utils().LogIt(_L("Checking method"));
	if ((c.Method() != a.StringPool().OpenFStringL(KPost))) User::Leave(-1);
	iEngine->Utils().LogIt(_L("Checking URI"));
	if ((c.URI().UriDes().Length() != 0)) User::Leave(-1);
	RHTTPResponse d = b.Response();
	iEngine->Utils().LogIt(_L("Checking status code setting"));
	d.SetStatusCode(410);
	if ((d.StatusCode() != 410)) User::Leave(-1);
	iEngine->Utils().LogIt(_L("Checking status text setting"));
	RStringF e;
	e = a.StringPool().OpenFStringL(KGoneText);
	d.SetStatusText(e);
	if ((d.StatusText() != e)) User::Leave(-1);
	iEngine->Utils().LogIt(_L("Checking version number setting"));
	TVersion f(7,4,3);
	d.SetVersion(f);
	if (d.Version().iMajor != f.iMajor || 
		d.Version().iMinor != f.iMinor) User::Leave(-1);
	d.SetVersion(f);
	iEngine->Utils().LogIt(_L("deleting transaction"));
	b.Close();
	RStringF http = a.StringPool().OpenFStringL(KHTTPText);
	iEngine->Utils().LogIt(_L("deleting session"));
	e.Close();
	http.Close();
	a.Close();
	CleanupStack::Pop(); // a
	if ((filter->iUnloadCount != 9)) User::Leave(-1);	
	CleanupStack::PopAndDestroy(filter);
	}

void CHTTPCoreTest::MHFRunL(RHTTPTransaction /*aTransaction*/,
						const THTTPEvent& /*aEvent*/)
	{
	}

TInt CHTTPCoreTest::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

const TDesC& CHTTPCoreTest::TestName()
	{
	_LIT(KHeaderTestName,"CHttpCoreTest");
	return KHeaderTestName;
	}
void CHTTPCoreTest::DoCancel()
	{
	}
	
