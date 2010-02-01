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
#include <uri8.h>
#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <httperr.h>

#include <httpstringconstants.h>

#include "httpcookiefilter.h"
#include <http/mhttpcookiemanager.h>
#include <http/ccookie.h>
#include "httpcookieerr.h"

CCookieFilter* CCookieFilter::InstallFilterL(TAny* aFilterParams)
	{
	TCookieFilterParams* filterParams = REINTERPRET_CAST(TCookieFilterParams*, aFilterParams);
	CCookieFilter* filter = new (ELeave) CCookieFilter(*(filterParams->iSession), *(filterParams->iCallback));
	CleanupStack::PushL(filter);
	filter->ConstructL();
	CleanupStack::Pop(filter); 
	return filter;
	}

CCookieFilter::CCookieFilter(RHTTPSession aSession, MHttpCookieManager& aCookieFilterCallback) :
	iStringPool(aSession.StringPool()),
	iSession(aSession), 
	iCookieManager(aCookieFilterCallback),
	iStringTable(RHTTPSession::GetTable())
	{
	}

void CCookieFilter::ConstructL()
	{
	iSession.FilterCollection().AddFilterL(*this, 
										   THTTPEvent::EGotResponseHeaders, 
										   RStringF(), 
										   KAnyStatusCode, 
										   ECookies, 
										   iStringPool.StringF(HTTP::ECookieFilter, iStringTable));
	
	iSession.FilterCollection().AddFilterL(*this, 
										   THTTPEvent::ESubmit, 
										   RStringF(),
										   KAnyStatusCode, 
										   ECookies, 
										   iStringPool.StringF(HTTP::ECookieFilter, iStringTable));
	}

CCookieFilter::~CCookieFilter()
	{
	// If we've been destroyed from the cleanup stack during creation
	// of the object, it might still be loaded. So check. (Normaly the
	// delete is initiated by the 'delete this' in MHFUnload)
	if (iLoadCount > 0)
		{
		// As we're already in a destructor, MHFUnload must not delete us again
		iLoadCount = -1;
		iSession.FilterCollection().
			RemoveFilter(iStringPool.StringF(HTTP::ECookieFilter, iStringTable));
		}
	}

void CCookieFilter::MHFLoad(RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/)
	{
	++iLoadCount;
	}

void CCookieFilter::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/)
	{
	if (--iLoadCount)
		return;

	delete this;
	}

void CCookieFilter::MHFRunL(RHTTPTransaction aTransaction, 
							const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
	case THTTPEvent::EGotResponseHeaders:
		HandleResponseHeadersL(aTransaction);
		break;
	case THTTPEvent::ESubmit:
		HandleRequestHeadersL(aTransaction);
		break;
	default:
		HTTPCookiePanic::Panic(HTTPCookiePanic::EInvalidFilterState);
		}
	}

TInt CCookieFilter::MHFRunError(TInt /*aError*/, 
								RHTTPTransaction aTransaction,
								const THTTPEvent& /*aEvent*/)
	{
	// If anything left, we've run out of memory or something
	// similarly catastrophic has gone wrong.
	aTransaction.Fail();
	return KErrNone;
	}

void CCookieFilter::HandleRequestHeadersL(RHTTPTransaction aTransaction)
	{
	RArray<CCookie*> cookies;
	CleanupClosePushL(cookies);
	TBool cookie2Reqd = EFalse;
	iCookieManager.GetCookiesL(aTransaction, cookies, cookie2Reqd);

	const TInt numCookies = cookies.Count();
	for (TInt ii=0; ii<numCookies; ++ii)
		AddCookieToRequestL(aTransaction.Request().GetHeaderCollection(), *cookies[ii], ii);

	CleanupStack::PopAndDestroy(&cookies);

	if(cookie2Reqd)
		{
		// Add a Cookie2 header
		RStringF cookie2 = iStringPool.StringF(HTTP::ECookie2, iStringTable);
		aTransaction.Request().GetHeaderCollection().SetFieldL(cookie2, THTTPHdrVal());
		}
	}

void CCookieFilter::AddCookieToRequestL(RHTTPHeaders aRequestHeaders, const CCookie& aCookie, TInt aPartIndex)
	{
	RStringF cookie = iStringPool.StringF(HTTP::ECookie, iStringTable);
	THTTPHdrVal hVal;
	aRequestHeaders.SetFieldL(cookie, THTTPHdrVal());

	// Add the cookie name and value pair
	aCookie.Attribute(CCookie::EName, hVal);
	aRequestHeaders.SetParamL(cookie, iStringPool.StringF(HTTP::ECookieName, iStringTable), hVal, aPartIndex);

	aCookie.Attribute(CCookie::EValue, hVal);
	aRequestHeaders.SetParamL(cookie, iStringPool.StringF(HTTP::ECookieValue, iStringTable), hVal, aPartIndex);

	// Add the other required cookie information - domain, path, port and version
	if (aCookie.Attribute(CCookie::EDomain, hVal) == KErrNone)
		aRequestHeaders.SetParamL(cookie, iStringPool.StringF(HTTP::EDomain, iStringTable), hVal, aPartIndex);

	if (aCookie.Attribute(CCookie::EPath, hVal) == KErrNone)
		aRequestHeaders.SetParamL(cookie, iStringPool.StringF(HTTP::EPath, iStringTable), hVal, aPartIndex);

	if (aCookie.Attribute(CCookie::EPort, hVal) == KErrNone)
		aRequestHeaders.SetParamL(cookie, iStringPool.StringF(HTTP::ECookiePort, iStringTable), hVal, aPartIndex);

	if (aCookie.Attribute(CCookie::EVersion, hVal) == KErrNone)
		aRequestHeaders.SetParamL(cookie, iStringPool.StringF(HTTP::EVersion, iStringTable), hVal, aPartIndex);
	}

void CCookieFilter::HandleResponseHeadersL(RHTTPTransaction aTransaction)
	{
	RHTTPHeaders responseHeaders = aTransaction.Response().GetHeaderCollection();

	RStringF cookieHeaderFieldName = iStringPool.StringF(HTTP::ESetCookie2, iStringTable);
	TInt numCookies = responseHeaders.FieldPartsL(cookieHeaderFieldName);
	if (numCookies > 0) 	// add SetCookie2 cookies
		{
		for (TInt ii = 0; ii < numCookies; ++ii)
			StoreOneCookieL(responseHeaders, ii, cookieHeaderFieldName, aTransaction.Request().URI());
		}
	else // only add SetCookie cookies if there are no newer SetCookie2 style cookies
		{
		cookieHeaderFieldName = iStringPool.StringF(HTTP::ESetCookie, iStringTable);
		numCookies = responseHeaders.FieldPartsL(cookieHeaderFieldName);
		if (numCookies > 0)
			{
			for (TInt ii = 0; ii < numCookies; ++ii)
				StoreOneCookieL(responseHeaders, ii, cookieHeaderFieldName, aTransaction.Request().URI());
			}
		}
	}

void CCookieFilter::StoreOneCookieL(RHTTPHeaders aRequestHeaders, TInt aPartIndex, RStringF aFieldName, const TUriC8& aUri)
	{
	THTTPHdrVal name, value;

	RStringF cookieName = iStringPool.StringF(HTTP::ECookieName, iStringTable);
	RStringF cookieValue = iStringPool.StringF(HTTP::ECookieValue, iStringTable);
	User::LeaveIfError(aRequestHeaders.GetParam(aFieldName, cookieName , name, aPartIndex));
	User::LeaveIfError(aRequestHeaders.GetParam(aFieldName, cookieValue, value, aPartIndex));

	RStringF setCookie2Name = iStringPool.StringF(HTTP::ESetCookie2, iStringTable);

	CCookie* cookie = CCookie::NewL(iStringPool, name.Str(), value.Str(), aFieldName==setCookie2Name);
	
	CleanupStack::PushL(cookie);

	THTTPHdrVal hVal;

	if (aRequestHeaders.GetParam(aFieldName, iStringPool.StringF(HTTP::EComment, iStringTable), hVal, aPartIndex) == KErrNone)
		cookie->SetAttributeL(CCookie::EComment, hVal);
	if (aRequestHeaders.GetParam(aFieldName, iStringPool.StringF(HTTP::ECommentURL, iStringTable), hVal, aPartIndex) == KErrNone)
		cookie->SetAttributeL(CCookie::ECommentURI, hVal);
	if (aRequestHeaders.GetParam(aFieldName, iStringPool.StringF(HTTP::EDiscard, iStringTable), hVal, aPartIndex) == KErrNone)
		cookie->SetAttributeL(CCookie::EDiscard, hVal);
	if (aRequestHeaders.GetParam(aFieldName, iStringPool.StringF(HTTP::EDomain, iStringTable), hVal, aPartIndex) == KErrNone)
		cookie->SetAttributeL(CCookie::EDomain, hVal);
	if (aRequestHeaders.GetParam(aFieldName, iStringPool.StringF(HTTP::EMaxAge, iStringTable), hVal, aPartIndex) == KErrNone)
		cookie->SetAttributeL(CCookie::EMaxAge, hVal);
	if (aRequestHeaders.GetParam(aFieldName, iStringPool.StringF(HTTP::EPath, iStringTable), hVal, aPartIndex) == KErrNone)
		cookie->SetAttributeL(CCookie::EPath, hVal);
	if (aRequestHeaders.GetParam(aFieldName, iStringPool.StringF(HTTP::ECookiePort, iStringTable), hVal, aPartIndex) == KErrNone)
		cookie->SetAttributeL(CCookie::EPort, hVal);
	if (aRequestHeaders.GetParam(aFieldName, iStringPool.StringF(HTTP::ESecure, iStringTable), hVal, aPartIndex) == KErrNone)
		cookie->SetAttributeL(CCookie::ESecure, hVal);
	if (aRequestHeaders.GetParam(aFieldName, iStringPool.StringF(HTTP::EVersion, iStringTable), hVal, aPartIndex) == KErrNone)
		cookie->SetAttributeL(CCookie::EVersion, hVal);
	if (aRequestHeaders.GetParam(aFieldName, iStringPool.StringF(HTTP::EExpires, iStringTable), hVal, aPartIndex) == KErrNone)
		cookie->SetAttributeL(CCookie::EExpires, hVal);
	
	CleanupStack::Pop(cookie);

	// then store the cookie
	iCookieManager.StoreCookieL(cookie, aUri);
	}


