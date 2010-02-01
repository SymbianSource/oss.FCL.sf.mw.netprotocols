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

#include "chttpclientfilter.h"

#include <httpstringconstants.h>
#include <http/rhttpheaders.h>
#include <http/mhttpdatasupplier.h>
#include <httperr.h>

#include "thttpclientpanic.h"

CHttpClientFilter::~CHttpClientFilter()
	{
	// If this object has been destroyed from the cleanup stack during creation
	// of the object, it might still be loaded - check. Normaly the delete is 
	// initiated by the 'delete this' in MHFUnload.
	if( iLoadCount )
		{
		// As already in a destructor, MHFUnload must not delete this again.
		iLoadCount = -1;
		RStringF filterName = iStringPool.StringF(HTTP::EHttpClientFilter, iStringTable);
		iSession.FilterCollection().RemoveFilter(filterName);
		}
	}

CHttpClientFilter::CHttpClientFilter()
:CBase(), iStringTable(RHTTPSession::GetTable())
	{
	}

void CHttpClientFilter::ConstructL(RHTTPSession aSession)
	{
	iSession = aSession;
	iStringPool = iSession.StringPool();

	// Register the filter for ESubmit to check and alter outgoing headers and
	// EGotResponseHeaders for altering incoming response headers to add 
	// defaults if they are missing, e.g. add Content-Type with value 
	// application/octet-stream if it is missing. Also, register for the two
	// pipelining error codes - this is to be able to cancel and re-submit those 
	// transactions.
	RStringF filterName = iStringPool.StringF(HTTP::EHttpClientFilter, iStringTable);
	iSession.FilterCollection().AddFilterL(
										  *this, 
										  THTTPEvent::ESubmit, 
										  MHTTPFilter::EProtocolHandler + 20,
										  filterName
										  );
	iSession.FilterCollection().AddFilterL(
										  *this, 
										  THTTPEvent::EGotResponseHeaders, 
										  MHTTPFilter::EProtocolHandler + 20,
										  filterName
										  );
	iSession.FilterCollection().AddFilterL(
										  *this, 
										  KErrHttpNonPipeliningError, 
										  MHTTPFilter::EProtocolHandler + 20,
										  filterName
										  );
	iSession.FilterCollection().AddFilterL(
										  *this, 
										  KErrHttpPipeliningError, 
										  MHTTPFilter::EProtocolHandler + 20,
										  filterName
										  );
	}

void CHttpClientFilter::AlterRequestHeadersL(RHTTPTransaction aTransaction)
	{
	AddSessionHeadersL(aTransaction.Request().GetHeaderCollection(), iSession.RequestSessionHeadersL());

	EnsurePathExistsL(aTransaction);
	EnsureContentLengthL(aTransaction);
	EnsureNoEndToEndHeadersInConnectionHeaderL(aTransaction);
//removed the following Expect100 line to fix INC45389 & INC42505 - please do not add back in without checking these defects
//	AddExpect100ContinueL(aTransaction);
	}

void CHttpClientFilter::AlterResponseHeadersL(RHTTPTransaction aTransaction)
	{
	AddSessionHeadersL(aTransaction.Response().GetHeaderCollection(), iSession.ResponseSessionHeadersL());

	EnsureContentTypePresentL(aTransaction);
	}


void CHttpClientFilter::EnsurePathExistsL(RHTTPTransaction aTransaction)
	{
	RHTTPRequest request = aTransaction.Request();
	TUriC8 originalUri = request.URI();

	// There is alwaya a path in a uri - ensure that it is not empty.
	const TDesC8& path = originalUri.Extract(EUriPath);
	TBool pathPresent = (path.Length() > 0);
	TBool hostPresent = originalUri.IsPresent(EUriHost);

	// If there is no path then make sure the uri
	// ends in '/' - server root path.
	if( hostPresent && !pathPresent )
		{
		_LIT8(KDefaultUriPath, "/");

		CUri8* uri = CUri8::NewLC(originalUri);
		uri->SetComponentL(KDefaultUriPath, EUriPath);
		request.SetURIL(uri->Uri());
		CleanupStack::PopAndDestroy(uri);
		}
	}

void CHttpClientFilter::EnsureContentLengthL(RHTTPTransaction aTransaction)
	{
	RHTTPRequest request = aTransaction.Request();

	if( request.HasBody() )
		{
		THTTPHdrVal hdrVal;
		RStringF teName = iStringPool.StringF(HTTP::ETransferEncoding, iStringTable);

		// Get rid of Content-Length header if present
		// NOTE - cannot use a local variable for the Content-Length as causes a 
		// compiler bug in thumb builds - grand! : (
		RHTTPHeaders headers = request.GetHeaderCollection();
		headers.RemoveField(iStringPool.StringF(HTTP::EContentLength, iStringTable)); 
		
		TInt bodySize = request.Body()->OverallDataSize();
		if( bodySize != KErrNotFound )
			{
			// Size is known - set the ContentLength header.
			headers.SetFieldL(iStringPool.StringF(HTTP::EContentLength, iStringTable), THTTPHdrVal(bodySize));
			}
		else if( headers.GetField(teName, 0, hdrVal) == KErrNotFound )
			{
			// Size is unknown and there's been no Encoding indicated by the 
			// client - set the 'TransferEncoding: chunked'
			hdrVal.SetStrF(iStringPool.StringF(HTTP::EChunked, iStringTable));
			headers.SetFieldL(teName, hdrVal);
			}
		}
	}

void CHttpClientFilter::EnsureNoEndToEndHeadersInConnectionHeaderL(RHTTPTransaction aTransaction)
	{
	RHTTPHeaders headers = aTransaction.Request().GetHeaderCollection();
	RStringF connection = iStringPool.StringF(HTTP::EConnection,iStringTable);
	const TInt numConnectionHeaderParts = headers.FieldPartsL(connection);

	for( TInt ii = numConnectionHeaderParts - 1; ii >= 0; --ii ) 
		{
		// Examine connection-tokens from back to front so index is always valid. 
		// Check for an end to end header and remove it as a connection header
		// must not contain end to end headers.
		THTTPHdrVal value;
		TInt ret = headers.GetField(connection, ii, value);

		if( ( ret != KErrNotFound ) && ( value.Type() == THTTPHdrVal::KStrFVal ) )
			{
			RStringF valueStrF = value.StrF();
			if( valueStrF.Index(iStringTable) != HTTP::EClose  && 
				!IsHopByHopHeader(valueStrF) )
				{
				// The connection-token is not 'close' nor is it a end-to-end
				// header field name - remove it.
				User::LeaveIfError(headers.RemoveFieldPart(connection, ii)); 
				}
			}
		else
			{
			// The connection-token is not a hop-by-hop header field name -
			// remove it.
			User::LeaveIfError(headers.RemoveFieldPart(connection, ii));
			}			
		}
	}

TBool CHttpClientFilter::IsHopByHopHeader(RStringF aHeaderName)
	{
	switch( aHeaderName.Index(iStringTable) )
		{
	case HTTP::EKeepAlive:
	case HTTP::EProxyAuthorization:
	case HTTP::EProxyAuthenticate:
	case HTTP::ETE:
	case HTTP::ETrailer:
	case HTTP::ETransferEncoding:
	case HTTP::EUpgrade:
		return ETrue;
	default:
		// NOT a hop by hop header, at least not as defined in http 1.1
		return EFalse;
		};
	}

void CHttpClientFilter::EnsureContentTypePresentL(RHTTPTransaction aTransaction)
	{
	// From RFC 2616 Section 7.2.1 - 
	//
	// Any HTTP/1.1 message containing an entity-body SHOULD include a Content-Type
	// header field defining the media type of that body. If and only if the 
	// media type is not given by a Content-Type field, the recipient MAY attempt
	// to guess the media type via inspection of its content and/or the name 
	// extension(s) of the URI used to identify the resource. If the media type
	// remains unknown, the recipient SHOULD treat it as type "application/octet-stream".

	RHTTPHeaders headers = aTransaction.Response().GetHeaderCollection();
	THTTPHdrVal contentType;
	RStringF contentTypeString = iStringPool.StringF(HTTP::EContentType, iStringTable);
	if( headers.GetField(contentTypeString, 0, contentType) == KErrNotFound )
		{
		// There is no Content-Type header - add it with a value of 
		// "application/octet-stream".
		contentType.SetStrF(iStringPool.StringF(HTTP::EApplicationOctetStream, iStringTable));
		headers.SetFieldL(contentTypeString, contentType);
		}
	}
	 

void CHttpClientFilter::AddSessionHeadersL(RHTTPHeaders aTransactionHeaders, RHTTPHeaders aSessionHeaders)
	{
	THTTPHdrFieldIter iter = aSessionHeaders.Fields();

	while( !iter.AtEnd() )
		{
		RStringTokenF headerToken = iter();
		RStringF headerName = iStringPool.StringF(headerToken);

		// See if transaction headers already contains this header.
		TPtrC8 rawData;
		if( aTransactionHeaders.GetRawField(headerName, rawData) == KErrNotFound )
			{
			aSessionHeaders.GetRawFieldL(headerName,rawData);
			aTransactionHeaders.SetRawFieldL(headerName, rawData, KFieldSeparator);
			}
		++iter;
		}
	}

/*
 *	Methods from MHTTPFilterBase
 */

void CHttpClientFilter::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch( aEvent.iStatus )
		{
	case THTTPEvent::ESubmit:
		{
		AlterRequestHeadersL(aTransaction);
		} break;
	case THTTPEvent::EGotResponseHeaders:
		{
		AlterResponseHeadersL(aTransaction);
		} break;
	case KErrHttpNonPipeliningError:
	case KErrHttpPipeliningError:
		{
		// This transaction was being pipelined when an error occurred. Need to
		// cancel and then re-submit.
		aTransaction.Cancel();
		aTransaction.SubmitL();	
		}
		break;
	default:
		break;
		}
	}

void CHttpClientFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CHttpClientFilter::MHFRunError(TInt /*aError*/, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	// Something has gone wrong - probably no memory. Fail the transaction.
	aTransaction.Fail();
	return KErrNone;
	}

TInt CHttpClientFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}

/*
 *	Methods from MHTTPFilter
 */

void CHttpClientFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	++iLoadCount;
	}

void CHttpClientFilter::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle)
	{
	if( --iLoadCount )
		return;

	delete this;
	}
