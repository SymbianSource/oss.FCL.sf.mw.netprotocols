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

#ifndef __REDIRECT_H__
#define __REDIRECT_H__

#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <http/cecomfilter.h>


//##ModelId=3B1E52A00353
class CRedirectFilter : public CEComFilter, public MHTTPFilter
	{
 public:
	static CEComFilter* InstallFilterL(TAny* aSession);	
 
	//##ModelId=3B1E52A0039D
	~CRedirectFilter();

public:	// Methods from MHTTPFilterBase

	// @see MHTTPFilterBase::MHFRunL
	//##ModelId=3B1E52A0039A
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	// @see MHTTPFilterBase::MHFSessionRunL
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);

	// @see MHTTPFilterBase::MHFRunError
	//##ModelId=3B1E52A00391
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent);

	// @see MHTTPFilterBase::MHFSessionRunError
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

public:	// Methods from MHTTPFilter

	// @see MHTTPFilter::MHFUnload
	//##ModelId=3B1E52A00389
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);

	// @see MHTTPFilter::MHFLoad
	//##ModelId=3B1E52A00386
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);

 private:
	
	//##ModelId=3B1E52A00385
	CRedirectFilter(RHTTPSession aSession);
	//ContructL also Installs the filter.
	//##ModelId=3B1E52A0037F
	void ConstructL(RHTTPSession aSession);

	// Function to handle the receipt of 301 Moved Permanently, 302 Found, 307 Moved Temporarily or 303 See Other
	//##ModelId=3B1E52A0037D
	void HandleRedirectL(RHTTPTransaction aTransaction,THTTPEvent::TTransactionWarning aWarningEvent);

	// Handle a 305 Use Proxy response
	//##ModelId=3B1E52A0037B
	void HandleUseProxyL(RHTTPTransaction aTransaction);

 private:
	// A count to make sure we delete at the right time.
	//##ModelId=3B1E52A00373
	TInt iLoadCount;
	//##ModelId=3BA6003B0264
	RStringPool iStringPool;

	//##ModelId=3BA6003B0250
	RHTTPSession iSession;
	};

#endif //__REDIRECT_H_
