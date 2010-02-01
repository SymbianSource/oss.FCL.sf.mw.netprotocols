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

#include "ValidationFilter.h"
#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <http/rhttpsession.h>
#include <http/mhttpdatasupplier.h>
#include <httpstringconstants.h>
#include <httperr.h>
#include "corefilterspanic.h"

CValidationFilter::CValidationFilter()
	{ 
	__LOG(_L("**********VF LOADED**********"));
	}

CEComFilter* CValidationFilter::InstallFilterL(TAny* aSession)
	{
	RHTTPSession* session = REINTERPRET_CAST(RHTTPSession*, aSession);
	CValidationFilter* filter = new (ELeave) CValidationFilter();
	CleanupStack::PushL(filter);
	filter->ConstructL(*session);
	CleanupStack::Pop(filter); 
	return filter;
	}

void CValidationFilter::ConstructL(RHTTPSession aSession)
	{
	iStringPool = aSession.StringPool();
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EAnyTransactionEvent,		// Any event
											RStringF(),									// Any header
											KAnyStatusCode,								// Any status code
											MHTTPFilter::ETidyUp,						// Priority of filter
											iStringPool.StringF(HTTP::EValidation,RHTTPSession::GetTable()));	// Name of filter
	}

void CValidationFilter::MHFLoad(RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/)
	{
	}

CValidationFilter::~CValidationFilter()
	{
	__LOG(_L("**********VF UNLOADED**********"));
	}


void CValidationFilter::MHFUnload(RHTTPSession,THTTPFilterHandle)
	{
	// We must be only registered on one session, as we register in our
	// ConstructL and no-one else has a pointer to us. Therefore, we
	// know we can be deleted at this point.
	delete this;
	}

void CValidationFilter::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	if (aEvent.iUID != KHTTPUid)
		return;
	

	switch(aEvent.iStatus)
		{
	case THTTPEvent::ESubmit:
		{
		__LOG(_L("VF has detected ESubmit event "));
		iTransactionFailed=0; // reset it for a new transaction
		TInt err = KErrNone;
		TRAP(err, ValidateRequestMethodL(aTransaction));
		__ASSERT_DEBUG(err == KErrNone, TFCoreFiltersPanic::Panic(TFCoreFiltersPanic::EValidationFilterFailure));
		}
		break;
	case THTTPEvent::EResponseComplete:
		{
		__LOG(_L("VF has detected EResponseComplete event "));
		//Assumption: no need to validate servers response headers or methods
		iTransactionFailed = ValidateResponseStatusCode(aTransaction);
	
		THTTPEvent event = THTTPEvent::EFailed;

		if(!iTransactionFailed )// no failures(4xx or 5xx messages) or errors(request errors)
			{
			event = THTTPEvent::ESucceeded;
			__LOG(_L("VF sends an incoming ESucceeded"));
			}
		else
			__LOG(_L("VF sends an incoming EFailed"));
		aTransaction.SendEventL(event,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter ) ;

		}
		break;
	default:
		if (aEvent.iStatus< 0)  //this error can only happen from incoming event
			{
			__LOG(_L("VF has detected a 'negative' event "));
			aTransaction.SendEventL(THTTPEvent::EFailed,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter ) ;
			__LOG(_L("VF sends an incoming EFailed"));
			}
		break;
		}
	}


TInt CValidationFilter::MHFRunError(TInt /*aError*/, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	// If anything left, we've run out of memory or something
	// similarly catastrophic has gone wrong.
	aTransaction.Fail();
	return KErrNone;
	}

void CValidationFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CValidationFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}

void CValidationFilter::ValidateRequestMethodL(RHTTPTransaction aTransaction)
	{
	TInt method = aTransaction.Request().Method().Index(RHTTPSession::GetTable());
	switch (method)
		{
	case HTTP::EGET:
		ValidateRequestGetL(aTransaction);
		break;
	case HTTP::EPOST:
		ValidateRequestPostL(aTransaction);
		break;
	case HTTP::EPUT:
		ValidateRequestPutL(aTransaction);
		break;
	case HTTP::EHEAD:
		ValidateRequestHeadL(aTransaction);
		break;
	case HTTP::ETRACE:
		ValidateRequestTraceL(aTransaction);
		break;
	default:
		//  if  method is not a standard http 1.1 event we must not validate it
		;
		}
		
	if(!iTransactionFailed)
		iTransactionFailed = ValidateRequestHeadersL(aTransaction);

	}

TBool CValidationFilter::ValidateResponseStatusCode(RHTTPTransaction aTransaction)
	{
	TInt statusCode = aTransaction.Response().StatusCode();
	// 400 to 599 is the error and warning range of http status codes
	if (statusCode >= 300 && statusCode <1000)
		return ETrue;
	return EFalse;
	}
	
TBool CValidationFilter::RequestFailIfContainsBodyL(RHTTPTransaction aTransaction)
	{
	if (aTransaction.Request().HasBody())
		{
		FailAndCancelL(aTransaction, KErrHttpRequestHasBody);
		return ETrue; 
		}
	return EFalse;
	}


void CValidationFilter::ValidateRequestGetL(RHTTPTransaction aTransaction)
	{
	iTransactionFailed = RequestFailIfContainsBodyL(aTransaction);
	}


void CValidationFilter::ValidateRequestHeadL(RHTTPTransaction aTransaction)
	{
	iTransactionFailed = RequestFailIfContainsBodyL(aTransaction);
	}


void CValidationFilter::ValidateRequestTraceL(RHTTPTransaction aTransaction)
	{
	iTransactionFailed = RequestFailIfContainsBodyL(aTransaction);
	}

void CValidationFilter::ValidateRequestPostL(RHTTPTransaction aTransaction)
	{
	if(!aTransaction.Request().HasBody()) 
		{
		FailAndCancelL(aTransaction,KErrHttpRequestBodyMissing);
		iTransactionFailed = ETrue;
		}

	// 1.0 Origin Server can't handle body  data where size is unknonwn [rfc2616 section 4.4.5]
	else if (aTransaction.Request().HasBody() && 
		aTransaction.Request().Body()->OverallDataSize() == KErrNotFound)
		{
		// Get version
		RHTTPConnectionInfo connInfo = aTransaction.Session().ConnectionInfo();
		THTTPHdrVal httpVersion;
		if (connInfo.Property(iStringPool.StringF(HTTP::EHTTPVersion,RHTTPSession::GetTable()), httpVersion))
			{
			if ( httpVersion.StrF() == iStringPool.StringF(HTTP::EHttp10,RHTTPSession::GetTable()) )
				{
				FailAndCancelL(aTransaction,KErrHttpPostReqBodyWithoutSizeOnHTTP10);
				iTransactionFailed=ETrue;
				}
			}
		}
	}

void CValidationFilter::ValidateRequestPutL(RHTTPTransaction aTransaction)
	{
	if(!aTransaction.Request().HasBody()) 
		{
		FailAndCancelL(aTransaction, KErrHttpRequestBodyMissing);
		iTransactionFailed=ETrue;
		}
	}


TBool CValidationFilter::ValidateRequestHeadersL(RHTTPTransaction aTransaction)
	{
	// This method returns ETrue if the transaction must fail due to invalid headers in the request

	RHTTPHeaders  requestHeaderSet =aTransaction.Request().GetHeaderCollection();
	//request with body must have at least a content-type header for the body
	TBool hasBody = aTransaction.Request().HasBody();
	if (hasBody)
		{
		THTTPHdrVal hVal;
		TBool hasContentType = (requestHeaderSet.GetField(iStringPool.StringF(HTTP::EContentType,RHTTPSession::GetTable()),0,hVal) != KErrNotFound);
		if (!(hasContentType )) 
			{
			FailAndCancelL(aTransaction, KErrHttpEntityHeaderMissingContentType);
			return ETrue;
			}
		}

	TBool removedNonAllowedField = EFalse;
	THTTPHdrFieldIter fields = requestHeaderSet.Fields();
	fields.First();
	while (fields.AtEnd() == EFalse)
		{
		TInt field = iStringPool.StringF(fields()).Index(RHTTPSession::GetTable());
		switch (field)
			{	
		// Entity Headers
		case HTTP::EAllow:
		case HTTP::EContentEncoding:
		case HTTP::EContentLanguage:
		case HTTP::EContentLength:
		case HTTP::EContentLocation:
		case HTTP::EContentMD5:
		case HTTP::EContentRange:
		case HTTP::EContentType:
		case HTTP::ELastModified:
		case HTTP::EExpires:	
			if (hasBody) // if no body in the request, remove all entity headers 
				break;
			// else fall through and remove the field
		// Response Headers (not allowed in requests)
		case HTTP::EAcceptRanges:
		case HTTP::EAge:
		case HTTP::EETag:
		case HTTP::ELocation:
		case HTTP::EProxyAuthenticate:
		case HTTP::ERetryAfter:
		case HTTP::EServer:
		case HTTP::ESetCookie:
		case HTTP::EVary:
		case HTTP::EWWWAuthenticate:

		case HTTP::ETrailer:
			requestHeaderSet.RemoveField(iStringPool.StringF(field,RHTTPSession::GetTable()));
			removedNonAllowedField = ETrue;
			break;
		default:
			break;
			};
		++fields;
		}
	if (removedNonAllowedField)
		{
		aTransaction.SendEventL(KErrHttpInvalidHeaderInRequest,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter);
		return ETrue;
		}
	
	return EFalse;
	}


void CValidationFilter::FailAndCancelL(RHTTPTransaction aTransaction, THTTPEvent aStatus)
	{
	aTransaction.Cancel(THTTPFilterHandle::ECurrentFilter);
	__LOG(_L("VF cancels the transaction"));

	aTransaction.SendEventL(aStatus,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter );
	__LOG1(_L("VF sends an incoming error: %d"), aStatus.iStatus);

	aTransaction.SendEventL(THTTPEvent::EFailed,THTTPEvent::EIncoming,THTTPFilterHandle::ECurrentFilter );
	__LOG(_L("VF sends an incoming EFailed"));
	}
