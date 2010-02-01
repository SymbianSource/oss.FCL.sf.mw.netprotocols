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
#include <delimitedpath8.h>
#include <delimitedquery8.h>
#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <httpstringconstants.h>
#include <httperr.h>
#include "corefilterspanic.h"
#include <escapeutils.h>

const TInt KRedirectLimit = 8; // Maximum level of redirection to stop infinite circular redirection

CRedirectFilter::CRedirectFilter(RHTTPSession aSession)
		: iSession(aSession), iStringTable(RHTTPSession::GetTable())
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
										   301, EStatusCodeHandler, iStringPool.StringF(HTTP::ERedirect,iStringTable));
	// 302 Found (deprecated, same as 307)
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EGotResponseHeaders, RStringF(), 
										   302, EStatusCodeHandler, iStringPool.StringF(HTTP::ERedirect,iStringTable));
	// 303 See other
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EGotResponseHeaders, RStringF(), 
										   303, EStatusCodeHandler, iStringPool.StringF(HTTP::ERedirect,iStringTable));
	// 307 Moved Temporarily
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EGotResponseHeaders, RStringF(), 
										   307, EStatusCodeHandler, iStringPool.StringF(HTTP::ERedirect,iStringTable));
	
	// Exceptional Cases which require special handling

	// 305 Use Proxy
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EGotResponseHeaders, RStringF(), 
										   305, EStatusCodeHandler, iStringPool.StringF(HTTP::ERedirect,iStringTable));

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
			RemoveFilter(iStringPool.StringF(HTTP::ERedirect,iStringTable));
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
		HandleRedirectL(aTransaction,THTTPEvent::ERedirectedPermanently);
		break;
	case 302:
	case 303:
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
	if (aTransaction.PropertySet().Property(p.StringF(HTTP::ERedirectCount,iStringTable), 
											redirectCountProperty))
		{
		__ASSERT_DEBUG(redirectCountProperty.Type() == THTTPHdrVal::KTIntVal, 
			TFCoreFiltersPanic::Panic(TFCoreFiltersPanic::EInvalidRedirectCountProperty));
		redirectCount = redirectCountProperty.Int();
		__ASSERT_DEBUG(redirectCount > 0, TFCoreFiltersPanic::Panic(TFCoreFiltersPanic::EInvalidRedirectCountProperty));
		}

	TInt maxNumberOfRedirects = KRedirectLimit;
	THTTPHdrVal maxRedirect;
	if (aTransaction.Session().ConnectionInfo().Property(p.StringF(HTTP::EMaxRedirect,iStringTable), maxRedirect))
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
		aTransaction.PropertySet().SetPropertyL(p.StringF(HTTP::ERedirectCount,iStringTable), redirectCount);

		RStringF location = p.StringF(HTTP::ELocation,iStringTable);
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
			requestHeaders.RemoveField(p.StringF(HTTP::EHost,iStringTable));
			TUriParser8 uriParser;
			uriParser.Parse( locationValue.StrF().DesC()  );
			SetRedirectUriL( request, uriParser );
			aTransaction.Cancel();

			
			// 10.3.2 301 Moved Permanently:
			// The requested resource has been assigned a new permanent URI and any future references to this resource
			// SHOULD use one of the returned URIs. If the 301 status code is received in response to a request
			// other than GET or HEAD, the user agent MUST NOT automatically redirect the request unless it can be confirmed
			// by the user, since this might change the conditions under which the request was issued.
			 
			// 10.3.8 307 Temporary Redirect:
			// The requested resource resides temporarily under a different URI. Since the redirection MAY be 
			// altered on occasion, the client SHOULD continue to use the Request-URI for future requests. 
			// This responseis only cacheable if indicated by a Cache-Control or Expires header field.
			
			// If the returned status code is 301 or 302 or 307 and the requested method is GET or HEAD,then the transaction 
			// is simply submitted. If not then the filter sends the client an event - ERedirectRequiresConfirmation
			// Then Client has to make the decision either to submit the transaction with the
			// new redirected URI or to close the transaction.
			
			TInt method = aTransaction.Request().Method().Index(iStringTable);
			TInt statusCode = aTransaction.Response().StatusCode();

			if( (statusCode == 301 || statusCode == 307) && 
									!((method==HTTP::EGET) || (method==HTTP::EHEAD)) )
				{
				aTransaction.SendEventL(THTTPEvent::ERedirectRequiresConfirmation,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter);
				}
			else
				{
				// 10.3.3 302 Found:
				// The requested resource resides temporarily under a different URI. 
				// Since the redirection might be altered on occasion, the client 
				// SHOULD continue to use the Request-URI for future requests. This 
				// response is only cacheable if indicated by a Cache-Control or 
				// Expires header field.
				// Note: RFC 1945 and RFC 2068 specify that the client is not allowed
				// to change the method on the redirected request.  However, most
				// existing user agent implementations treat 302 as if it were a 303
				// response, performing a GET on the Location field-value regardless
				// of the original request method. The status codes 303 and 307 have
				// been added for servers that wish to make unambiguously clear which
				// kind of reaction is expected of the client.


				// 10.3.4 303 See Other
				// The response to the request can be found under a different URI and
                // SHOULD be retrieved using a GET method on that resource.
				// The different URI SHOULD be given by the Location field in the
                // response. Unless the request method was HEAD, the entity of the
                // response SHOULD contain a short hypertext note with a hyperlink to
                // the new URI(s).
				if ( statusCode == 302 || statusCode == 303 )
					{
						requestHeaders.RemoveField(p.StringF(HTTP::EContentLength,iStringTable));
						requestHeaders.RemoveField(p.StringF(HTTP::EContentType,iStringTable));
						aTransaction.Request().RemoveBody();
						aTransaction.Response().RemoveBody();
						request.SetMethod(p.StringF(HTTP::EGET, iStringTable));
					}
				aTransaction.SendEventL(aWarningEvent,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter);
				aTransaction.SubmitL();
				}
			}
		}
	else // Possible circular redirection so pass on an error to the user
		{
		aTransaction.SendEventL(KErrHttpRedirectExceededLimit,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter);
		}

	}


void CRedirectFilter::HandleUseProxyL(RHTTPTransaction aTransaction)
	{
	__START_PERFORMANCE_LOGGER();
	aTransaction.SendEventL(KErrHttpRedirectUseProxy,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter);
     __END_PERFORMANCE_LOGGER(_L(",CValidationFilter::ValidateResponseStatusCode()"));
	}


void CRedirectFilter::EscapeEncodeUriL ( CUri8& aUri ) const
	{
	_LIT8 ( KCharsToEscape, " " );

	const TUriC8& uri ( aUri.Uri() );
	if ( uri.IsPresent ( EUriPath ) ) 
		{
		HBufC8* escapedPath = EscapeUtils::SpecificEscapeEncodeL ( uri.Extract ( EUriPath ), KCharsToEscape  );
		CleanupStack::PushL ( escapedPath );
		aUri.SetComponentL ( *escapedPath, EUriPath );
		CleanupStack::PopAndDestroy ( escapedPath );		
	    }
	
	if ( uri.IsPresent ( EUriQuery ) ) 
		{
		HBufC8* escapedQuery = EscapeUtils::SpecificEscapeEncodeL ( uri.Extract ( EUriQuery ), KCharsToEscape  );
		CleanupStack::PushL ( escapedQuery );
		aUri.SetComponentL ( *escapedQuery, EUriQuery );
		CleanupStack::PopAndDestroy ( escapedQuery );		
		}		
	}

void CRedirectFilter::SetRedirectUriL(RHTTPRequest aRequest, const TUriC8& aUri) const
	{
	 CUri8* newUri = NULL;

	if (aUri.IsPresent(EUriHost))
		{
		newUri = CUri8::NewLC ( aUri );
		}

	else // the location field contained a relative Uri.
		{
		newUri = CUri8::ResolveL(aRequest.URI(), aUri);
		CleanupStack::PushL(newUri);
		}
			EscapeEncodeUriL ( *newUri );	
	// Check for the fragment in the new uri.
	// need to append if the fragment is present in the initial uri and not present in the redirected.
	if ( aRequest.URI().IsPresent( EUriFragment ) && !newUri->Uri().IsPresent( EUriFragment ) )
		{
		newUri->SetComponentL ( aRequest.URI().Extract ( EUriFragment ), EUriFragment );
		}
	aRequest.SetURIL(newUri->Uri());
	CleanupStack::PopAndDestroy(newUri);
	}


	
