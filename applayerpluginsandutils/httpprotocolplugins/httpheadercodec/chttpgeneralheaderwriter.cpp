// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "chttpgeneralheaderwriter.h"

#include <http/rhttpsession.h>
#include <httpstringconstants.h>
#include <httperr.h>

#include "CHeaderField.h"

_LIT8(KSemiSpaceSep,"; ");
_LIT8(KCommaSpaceSep,", ");

CHttpGeneralHeaderWriter* CHttpGeneralHeaderWriter::NewL(RStringPool aStringPool)
/** 
	Factory constructor.
	@internalComponent
	@param		aStringPool	The current string pool.
	@return		A pointer to a fully initialised object.
	@leave		KErrNoMemory	Not enough memory to create object.
*/
	{
	return new(ELeave)CHttpGeneralHeaderWriter(aStringPool);
	}

CHttpGeneralHeaderWriter::~CHttpGeneralHeaderWriter()
/**
	Destructor
	@internalComponent
*/
	{
	}

CHttpGeneralHeaderWriter::CHttpGeneralHeaderWriter(RStringPool aStringPool)
: CHttpHeaderWriter(aStringPool)
/**
	Constructor
	@internalComponent
	@param		aStringPool	The current string pool.
*/
	{
	}

void CHttpGeneralHeaderWriter::EncodeCacheControlL(RHeaderField& aHeader) const
/**
	Encodes a Cache-Control header. RFC 2616 section 14.9 - 

		Cache-Control			=	"Cache-Control" ":" 1#cache-directive
		cache-directive			=	cache-request-directive 
								|	cache-response-directive
		cache-request-directive	=	"no-cache" 
								|	"no-store" 
								|	"max-age" "=" delta-seconds
								|	"max-stale" [ "=" delta-seconds ] 
								|	"min-fresh" "=" delta-seconds 
								|	"no-transform" 
								|	"only-if-cached" 
								|	cache-extension

		cache-response-directive=	"public" 
								|	"private" [ "=" <"> 1#field-name <"> ] 
								|	"no-cache" [ "=" <"> 1#field-name <"> ]
								|	"no-store" 
								|	"no-transform"
								|	"must-revalidate"
								|	"proxy-revalidate"
								|	"max-age" "=" delta-seconds
								|	"s-maxage" "=" delta-seconds
								|	cache-extension

		cache-extension			=	token [ "=" ( token | quoted-string ) ]

	The cache-control header value is a comma separated list of values with at
	least one value.
	@internalComponent
	@param		aHeader	The cache-control header field to encode.
	@leave		CHttpWriter::DoTokenCsvListHeaderL
	@todo		Is there any point in this? Why not call DoTokenCsvListHeaderL()
				directly
*/
	{
	THeaderFieldPartIter iter = aHeader.PartsL();
    iter.First();
	if (iter.AtEnd())
	   User::Leave(KErrHttpEncodeCacheControl);
	
	aHeader.BeginRawDataL();
	do
	    {
	    const CHeaderFieldPart* part = iter();
	    if(part == NULL)
	        User::Leave(KErrHttpEncodeCacheControl);	    
	    THTTPHdrVal ptVal = part->Value();
	    if (ptVal.Type() != THTTPHdrVal::KStrFVal)
	        User::Leave(KErrHttpEncodeCacheControl);
	    const TDesC8& val = ptVal.StrF().DesC(); 
	    if(val.Length() > 0)
	        {
	        aHeader.WriteRawDataL(val);
	        }
	    else
	        {
	        // Now we must have part as the strF value is KNulLDesC8
	        THeaderFieldParamIter iter2 = part->Parameters();
	        if(!iter2.AtEnd())
	            {
	            const CHeaderFieldParam* param = iter2();
	            aHeader.WriteRawDataL(param->Name().DesC());
	            aHeader.WriteRawDataL('=');
	            THTTPHdrVal val2 = param->Value();
	            if(val2.Type() == THTTPHdrVal::KTIntVal)
	                {
	                TBuf8<32> desc;
	                desc.AppendNum(val2.Int());
	                aHeader.WriteRawDataL(desc);
	                }
	            else if(val2.Type() == THTTPHdrVal::KStrFVal)
	                {
	                aHeader.WriteRawDataL(param->Name().DesC());
	                }	            
	            }
	        }
	    ++iter;
	    if(!iter.AtEnd())
	        aHeader.WriteRawDataL(KCommaSpaceSep());
	    
	    }while(!iter.AtEnd());
	aHeader.CommitRawData();
	}

void CHttpGeneralHeaderWriter::EncodeConnectionL(RHeaderField& aHeader) const
/**
	Decodes the Connection header. RFC2616 section 14.10 - 

		Connection = "Connection" ":" 1#(connection-token)
		connection-token  = token
	The connection header value is a comma separated list of values with at least
	one value.
	@internalComponent
	@param		aHeader	The connection header field to encode.
	@leave		CHttpWriter::DoTokenCsvListHeaderL
	@todo		Is there any point in this? Why not call DoTokenCsvListHeaderL()
				directly
*/
	{
	DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeConnection);
	}

void CHttpGeneralHeaderWriter::EncodeDateL(RHeaderField& aHeader) const
/**
	Encodes the date header. RFC2616 section 14.18 - 

		Date		=	"Date" ":" HTTP-Date

	The HTTP-Date format has a few formats. E.g. Date: Tue, 15 Nov 1994 08:12:31 GMT
	@internalComponent
	@param		aHeader	The date header field to encode.
	@leave		CHttpWriter::EncodeGenericDateL
	@todo		Is there any point in this? Why not call EncodeGenericDateL()
				directly
*/
	{
	EncodeGenericDateL(aHeader, KErrHttpEncodeDate);
	}

void CHttpGeneralHeaderWriter::EncodeTransferEncodingL(RHeaderField& aHeader) const
/**
	Encodes the transfer-encoding header. RFC2616 section 14.41 -

		Transfer-Encoding       =	"Transfer-Encoding" ":" 1#transfer-coding
		transfer-coding         =	"chunked" | transfer-extension
		transfer-extension      =	token *( ";" parameter )
		parameter               =	attribute "=" value
		attribute               =	token
		value                   =	token | quoted-string

	The transfer-encoding header is a comma separated list of values with at 
	least one value.
	@internalComponent
	@param		aHeader	The transfer-encoding header field to encode.
	@leave		CHttpWriter::DoTokenCsvListHeaderL
	@todo		Is there any point in this? Why not call DoTokenCsvListHeaderL()
				directly
*/
	{
	DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeTransferEncoding);
	}

void CHttpGeneralHeaderWriter::EncodeContentTypeL(RHeaderField& aHeader) const
/**
	Encodes the content-type header. RFC2616 section 14.17

		Content-Type			=	"Content-Type" ":" media-type
		media-type				=	type "/" subtype *( ";" parameter )
		type					=	token
		subtype					=	token
		parameter				=	attribute '=' value
		attribute               =	token
		value                   =	token | quoted-string
	
	The content-type header has a single media-type value. The atttribute 
	is normally the 'charset'. E.g. Content-Type: text/html; charset=ISO-8859-4
	@internalComponent
	@param		aHeader	The content-type header field to encode.
	@leave		RHeaderField::BeginRawDataL
	@leave		RHeaderField::WriteRawDataL
	@leave		CHttpWriter::WriteRawStrFPartL
	@leave		KErrHttpEncodeContentType	There are no value tokens, the
											parameter was empty or the parameter
											was not a string type value.
*/
	{
  __START_PERFORMANCE_LOGGER();
	// Check part 1
	THeaderFieldPartIter iter = aHeader.PartsL();
	iter.First();
	if( iter.AtEnd() )
		{
		// The header value is empty!
		User::Leave(KErrHttpEncodeContentType);
		}
	aHeader.BeginRawDataL();
	const CHeaderFieldPart* part = WriteRawStrFPartL(aHeader, iter, KErrHttpEncodeContentType);

	// Check for parameters...
	THeaderFieldParamIter iter2 = part->Parameters();
	iter2.First();
	while( !iter2.AtEnd() )
		{
		// Got a parameter - write a semicolon separator
		aHeader.WriteRawDataL(KSemiSpaceSep);
		const CHeaderFieldParam* param = iter2();
		if( !param )
			{
			// Empty parameter!
			User::Leave(KErrHttpEncodeContentType);
			}
		// Anticipate only string parameter values
		THTTPHdrVal paramVal = param->Value();
		if( paramVal.Type() != THTTPHdrVal::KStrFVal )
			{
			// Was not a string value...
			User::Leave(KErrHttpEncodeContentType);
			}
		RStringF paramName = iStringPool.StringF(param->Name());
		aHeader.WriteRawDataL(paramName.DesC());
		aHeader.WriteRawDataL('=');
		aHeader.WriteRawDataL(paramVal.StrF().DesC());
		
		// Move onto the next parameter, writing a separator as necessary
		++iter2;
		}
	// All done!
	aHeader.CommitRawData();
	__END_PERFORMANCE_LOGGER(_L(",CHttpGeneralHeaderWriter::EncodeContentTypeL()"));
	}

void CHttpGeneralHeaderWriter::EncodeContentLengthL(RHeaderField& aHeader) const
/**
	Encodes the content-length header. RFC2616 section 14.13 - 

		Content-Length    = "Content-Length" ":" 1*DIGIT

	The content-length value is a number with at least one digit.
	@internalComponent
	@param		aHeader	The content-length header field to encode.
	@leave		CHttpWriter::EncodeGenericNumberHeaderL
	@todo		Is there any point in this? Why not call EncodeGenericNumberHeaderL()
				directly. 
*/
	{
	EncodeGenericNumberHeaderL(aHeader, KErrHttpEncodeContentLength);
	}

/*
 *	Methods from CHeaderWriter
 */

void CHttpGeneralHeaderWriter::EncodeHeaderL(RHeaderField& aHeader)
/**
	Encodes the header field value. 
	@internalComponent
	@param		aHeader		The header field to be encoded.
	@leave		CHttpGeneralHeaderWriter::EncodeCacheControlL
	@leave		CHttpGeneralHeaderWriter::EncodeConnectionL
	@leave		CHttpGeneralHeaderWriter::EncodeDateL
	@leave		CHttpGeneralHeaderWriter::EncodeTransferEncodingL
	@leave		CHttpGeneralHeaderWriter::EncodeContentLengthL
	@leave		CHttpWriter::DoTokenCsvListHeaderL
	@leave		KErrNotSupported	The writer was asked to encode a header that
									is does not support.
*/
	{
	RStringF fieldStr = iStringPool.StringF(aHeader.Name());
	switch( fieldStr.Index(iStringTable) )
		{
	case HTTP::ECacheControl:
		{
		EncodeCacheControlL(aHeader);
		} break;
	case HTTP::EConnection:
		{
		EncodeConnectionL(aHeader);
		} break;
	case HTTP::EDate:
		{
		EncodeDateL(aHeader);
		} break;
	case HTTP::EPragma:
		{
		// RFC2616 section 14.32 - 
		//
		//		Pragma				=	"Pragma" ":" 1#pragma-directive
		//		pragma-directive	=	"no-cache"	|	extension-pragma
		//		extension-pragma	=	token [ "=" ( token ! quoted-string ) ]
		//
		// The pragma header value is a comma separated list of values with at 
		// least one value.
		DoTokenCsvListHeaderL(aHeader, KErrHttpEncodePragma);
		} break;
	case HTTP::ETransferEncoding:
		{
		EncodeTransferEncodingL(aHeader);
		} break;
	case HTTP::EContentEncoding:
		{
		DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeContentEncoding);
		} break;
	case HTTP::EContentLanguage:
		{
		// RFC2616 section 14.12 - 
		// 
		//		Content-Language	=	"Content-Language" ":" 1#language-tag
		// 
		// The content-language header value is a comma separated list of values
		// with at least one value.
		DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeContentLanguage);
		} break;
	case HTTP::EUpgrade:
		{
		DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeUpgrade);
		} break;
	case HTTP::EContentLength:
		{
		EncodeContentLengthL(aHeader);
		} break;
	case HTTP::EContentLocation:
		{
		DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeContentLocation);
		} break;
	case HTTP::EContentMD5:
		{
		DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeContentMD5);
		} break;
	case HTTP::EContentType:
		{
		EncodeContentTypeL(aHeader);
		} break;
	case HTTP::EExpires:
	case HTTP::ELastModified:
		{
		EncodeDateL(aHeader);
		}break;
	default:
		User::Leave(KErrNotSupported);
		break;
		}
	}
