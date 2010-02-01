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

#ifndef __CHTTPCLIENTFILTER_H__
#define __CHTTPCLIENTFILTER_H__

#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <stringpool.h>

/** Acts as a filter above the HTTP Protocol Handler
	
	This filter alters request and responses where necessary to ensure complaince with
	various sections of RFC 2616
*/
class CHttpClientFilter : public CBase, 
						  public MHTTPFilter
	{
public:

	static CHttpClientFilter* NewL(RHTTPSession aSession);
	virtual ~CHttpClientFilter();

private:	// methods from MHTTPFilterBase

	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

private:	// methods from MHTTPFilter

	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);

 private:

	CHttpClientFilter();
	void ConstructL(RHTTPSession aSession);

	void AlterRequestHeadersL(RHTTPTransaction aTransaction);
	void AlterResponseHeadersL(RHTTPTransaction aTransaction);
	void EnsureContentTypePresentL(RHTTPTransaction aTransaction);
	void EnsurePathExistsL(RHTTPTransaction aTransaction);
	void EnsureContentLengthL(RHTTPTransaction aTransaction);
	void EnsureNoEndToEndHeadersInConnectionHeaderL(RHTTPTransaction aTransaction);
	TBool IsHopByHopHeader(RStringF aHeaderName);
	void AddExpect100ContinueL(RHTTPTransaction aTransaction);
	void AddSessionHeadersL(RHTTPHeaders aTransactionHeaders, RHTTPHeaders aSessionHeaders);

 private:
     const TStringTable& iStringTable;
	TInt			iLoadCount;
	RStringPool		iStringPool;
	RHTTPSession	iSession;

	};

// Inlined as it's only called in 1 place.
inline CHttpClientFilter* CHttpClientFilter::NewL(RHTTPSession aSession)
	{
	CHttpClientFilter* self = new (ELeave) CHttpClientFilter;
	CleanupStack::PushL(self);
	self->ConstructL(aSession);
	CleanupStack::Pop(self); 
	return self;
	}

#endif	// __CHTTPCLIENTFILTER_H__ 
