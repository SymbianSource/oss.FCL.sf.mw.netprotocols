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
#include "redirect.h"
#include <uri8.h>
#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <httpstringconstants.h>
#include <httperr.h>
#include "corefilterspanic.h"

const TInt KRedirectLimit = 5; // Maximum level of redirection to stop infinite circular redirection

CRedirectFilter::CRedirectFilter(RHTTPSession aSession)
		: iSession(aSession)
	{
	}

CEComFilter* CRedirectFilter::InstallFilterL(TAny* aSession)
	{
	RHTTPSession* session = REINTERPRET_CAST(RHTTPSession*, aSession);
	CRedirectFilter* filter = new (ELeave) CRedirectFilter(*session);
	CleanupStack::PushL(filter);
	filter->ConstructL(*session);
	CleanupStack::Pop(filter); 
	return filter;
	}

void CRedirectFilter::ConstructL(RHTTPSession aSession)
	{
	iStringPool = aSession.StringPool();
	// Register the filter. 

	// 301 Moved Permanently
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EGotResponseHeaders, RStringF(), 
										   301, EStatusCodeHandler, iStringPool.StringF(HTTP::ERedirect,RHTTPSession::GetTable()));
	// 302 Found (deprecated, same as 307)
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EGotResponseHeaders, RStringF(), 
										   302, EStatusCodeHandler, iStringPool.StringF(HTTP::ERedirect,RHTTPSession::GetTable()));
	// 303 See other
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EGotResponseHeaders, RStringF(), 
										   303, EStatusCodeHandler, iStringPool.StringF(HTTP::ERedirect,RHTTPSession::GetTable()));
	// 307 Moved Temporarily
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EGotResponseHeaders, RStringF(), 
										   307, EStatusCodeHandler, iStringPool.StringF(HTTP::ERedirect,RHTTPSession::GetTable()));
	
	// Exceptional Cases which require special handling

	// 305 Use Proxy
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EGotResponseHeaders, RStringF(), 
										   305, EStatusCodeHandler, iStringPool.StringF(HTTP::ERedirect,RHTTPSession::GetTable()));

	// 304 This should be entirely handled by the client as it is only used in conditional get cases, so only the client
	// would know what to do
	}

CRedirectFilter::~CRedirectFilter()
	{
	// If we've been destroyed from the cleanup stack during creation
	// of the object, it might still be loaded. So check. (Normaly the
	// delete is initiated by the 'delete this' in MHFUnload)
	if (iLoadCount)
		{
		// As we're already in a destructor, MHFUnload must not delete us again
		iLoadCount = -1;
		iSession.FilterCollection().
			RemoveFilter(iStringPool.StringF(HTTP::ERedirect,RHTTPSession::GetTable()));
		}
	}

void CRedirectFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	++iLoadCount;
	}

void CRedirectFilter::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle)
	{
	if (--iLoadCount)
		return;

	delete this;
	}

void CRedirectFilter::MHFRunL(RHTTPTransaction aTransaction, 
									const THTTPEvent& /*aEvent*/)
	{
	switch (aTransaction.Response().StatusCode())
		{
	case 301:
	case 303:
		HandleRedirectL(aTransaction,THTTPEvent::ERedirectedPermanently);
		break;
	case 302:
	case 307:
		HandleRedirectL(aTransaction,THTTPEvent::ERedirectedTemporarily);
		break;
	case 305: 
		HandleUseProxyL(aTransaction);
		break;
	default:
		break;
		}
	}

TInt CRedirectFilter::MHFRunError(TInt /*aError*/, 
										RHTTPTransaction aTransaction,
										const THTTPEvent& /*aEvent*/)
	{
	// If anything left, we've run out of memory or something
	// similarly catastrophic has gone wrong.
	aTransaction.Fail();
	return KErrNone;
	}

void CRedirectFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CRedirectFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}

void CRedirectFilter::HandleRedirectL(RHTTPTransaction aTransaction, THTTPEvent::TTransactionWarning aWarningEvent)
	{
	TInt redirectCount = 0;
	RStringPool p = aTransaction.Session().StringPool();

	// see if this transaction has been redirected before
	THTTPHdrVal redirectCountProperty;
	if (aTransaction.PropertySet().Property(p.StringF(HTTP::ERedirectCount,RHTTPSession::GetTable()), 
											redirectCountProperty))
		{
		__ASSERT_DEBUG(redirectCountProperty.Type() == THTTPHdrVal::KTIntVal, 
			TFCoreFiltersPanic::Panic(TFCoreFiltersPanic::EInvalidRedirectCountProperty));
		redirectCount = redirectCountProperty.Int();
		__ASSERT_DEBUG(redirectCount > 0, TFCoreFiltersPanic::Panic(TFCoreFiltersPanic::EInvalidRedirectCountProperty));
		}

	TInt maxNumberOfRedirects = KRedirectLimit;
	THTTPHdrVal maxRedirect;
	if (aTransaction.Session().ConnectionInfo().Property(p.StringF(HTTP::EMaxRedirect,RHTTPSession::GetTable()), maxRedirect))
		{
		__ASSERT_DEBUG(maxRedirect.Type() == THTTPHdrVal::KTIntVal, 
			TFCoreFiltersPanic::Panic(TFCoreFiltersPanic::EInvalidMaxRedirectProperty));
		maxNumberOfRedirects = maxRedirect.Int();
		__ASSERT_DEBUG(maxNumberOfRedirects > 0, 
			TFCoreFiltersPanic::Panic(TFCoreFiltersPanic::EInvalidMaxRedirectProperty));
		}


	// Only redirect a certain number of times, and update the redirect count property of the transaction
	if (++redirectCount <= maxNumberOfRedirects)
		{
		aTransaction.PropertySet().SetPropertyL(p.StringF(HTTP::ERedirectCount,RHTTPSession::GetTable()), redirectCount);

		RStringF location = p.StringF(HTTP::ELocation,RHTTPSession::GetTable());
		RHTTPHeaders responseHeaders(aTransaction.Response().GetHeaderCollection());
		THTTPHdrVal locationValue;
		
		TInt err = responseHeaders.GetField(location, 0, locationValue);
		if (err == KErrNotFound) // location not present when redirected
			aTransaction.SendEventL(KErrHttpRedirectNoLocationField,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter);
		else 
			{
			User::LeaveIfError(err); 
			RHTTPRequest request = aTransaction.Request();
			RHTTPHeaders requestHeaders(request.GetHeaderCollection());
			requestHeaders.RemoveField(p.StringF(HTTP::EHost,RHTTPSession::GetTable()));
			TUriParser8 uri;
			uri.Parse(locationValue.StrF().DesC());
			request.SetURIL(uri);
			aTransaction.Cancel();
			aTransaction.SendEventL(aWarningEvent,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter);
			aTransaction.SubmitL();
			}
		}
	else // Possible circular redirection so pass on an error to the user
		{
		aTransaction.SendEventL(KErrHttpRedirectExceededLimit,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter);
		}
	}


void CRedirectFilter::HandleUseProxyL(RHTTPTransaction aTransaction)
	{
	aTransaction.SendEventL(KErrHttpRedirectUseProxy,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter);
	}



	
