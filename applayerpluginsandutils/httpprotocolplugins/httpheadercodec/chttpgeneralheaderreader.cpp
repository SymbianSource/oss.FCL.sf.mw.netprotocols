// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "chttpgeneralheaderreader.h"

#include <http/rhttpsession.h>
#include <httpstringconstants.h>
#include <httperr.h>
#include <inetprottextutils.h>

_LIT8(KCommaSpaceSep,", ");
_LIT8(KCommaNewline, ",\n");
_LIT8(KEqualQuote, "=\"");

CHttpGeneralHeaderReader* CHttpGeneralHeaderReader::NewL(RStringPool aStringPool)
/** 
	Factory constructor.
	@internalComponent
	@param		aStringPool	The current string pool.
	@return		A pointer to a fully initialised object.
	@leave		KErrNoMemory	Not enough memory to create object.
*/
	{
	return new(ELeave) CHttpGeneralHeaderReader(aStringPool);
	}

CHttpGeneralHeaderReader::~CHttpGeneralHeaderReader()
/**
	Destructor
	@internalComponent
*/	{
	}

CHttpGeneralHeaderReader::CHttpGeneralHeaderReader(RStringPool aStringPool)
: CHttpHeaderReader(aStringPool)
/**
	Constructor
	@internalComponent
	@param		aStringPool	The current string pool.
*/
	{
	}

void CHttpGeneralHeaderReader::DecodeCacheControlL(RHeaderField& aHeader) const
/**
	Decodes a Cache-Control header. RFC 2616 section 14.9 - 

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
	@param		aHeader	The cache-control header field to decode.
	@leave		CHttpReader::DecodeGenericL
	@todo		Is there any point in this? Why not call DecodeGenericL() straight away
*/
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	TInt remaining = rawData.Length();
	TPtrC8 token;
	TInt tokensFound =0;
	RStringF maxAge = iStrPool.StringF(HTTP::EMaxAge, iStringTable);
	RStringF maxStale = iStrPool.StringF(HTTP::EMaxStale, iStringTable);
	RStringF minFresh = iStrPool.StringF(HTTP::EMinFresh, iStringTable);
	RStringF smaxAge = iStrPool.StringF(HTTP::ESMaxAge, iStringTable);
	while(remaining > 0)
	    {
		remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, token, KCommaNewline());
	

		if(token.Locate('=') == KErrNotFound)
		   {
		  SetNewFStringPartL(aHeader, tokensFound, token);    
		   }
		else
		   {
		   CHeaderFieldPart*  part = SetNewFStringPartL(aHeader, tokensFound, KNullDesC8());
		   TPtrC8 paramName;
		   InetProtTextUtils::ExtractNextTokenFromList(token, paramName, '=');
		   if ( (paramName.Compare(maxAge.DesC()) == 0) ||
		   (paramName.Compare(maxStale.DesC()) == 0) ||
		   (paramName.Compare(minFresh.DesC()) == 0)||
		   (paramName.Compare(smaxAge.DesC()) == 0) )
		       { 
		       SetNewIntegerParamL(*part, paramName, token); 
		       }
		   else
		       {
		       SetNewFStringParamL(*part, paramName, token); 
		       } 
	       	   }
	 ++tokensFound; 
	    }
	}
		

void CHttpGeneralHeaderReader::DecodeConnectionL(RHeaderField& aHeader) const
/**
	Decodes the Connection header. RFC2616 section 14.10 - 

		Connection = "Connection" ":" 1#(connection-token)
		connection-token  = token
	The connection header value is a comma separated list of values with at least
	one value.
	@internalComponent
	@param		aHeader	The connection header field to decode.
	@leave		CHttpReader::DecodeGenericL
	@todo		Is there any point in this? Why not call DecodeGenericL() straight away
*/
	{
	DecodeGenericL(aHeader, KCommaNewline);
	}

void CHttpGeneralHeaderReader::DecodeTransferEncodingL(RHeaderField& aHeader) const
/**
	Decodes the transfer-encoding header. RFC2616 section 14.41 -

		Transfer-Encoding       =	"Transfer-Encoding" ":" 1#transfer-coding
		transfer-coding         =	"chunked" | transfer-extension
		transfer-extension      =	token *( ";" parameter )
		parameter               =	attribute "=" value
		attribute               =	token
		value                   =	token | quoted-string

	The transfer-encoding header is a comma separated list of values with at 
	least one value.
	@internalComponent
	@param		aHeader	The transfer-encoding header field to decode.
	@leave		RHeaderField::RawDataL
	@leave		CHttpReader::SetNewFStringPartL
	@leave		CHttpReader::SetNewFStringParamL
	@leave		KErrHttpDecodeTransferEncoding	There was a transfer-extension
												with no token before the 
												parameter.
*/
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	TInt remaining = rawData.Length();
	TInt xfrCodingsFound = 0;

	// Iterate over the token list
	while( remaining > 0 )
		{
		// locate a token and set the new header part at the appropriate position.
		// Note - the token delimiter may be a '\n' or a ','
		TPtrC8 xfrCoding;
		remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, xfrCoding, KCommaNewline);

		// Search for ';' separator to determine if there is a parameter within 
		// this transfer-coding. Don't bother check for 'chunked' per se, just 
		// treat it as equivalent to a transfer-extension which means we would
		// erroneously store parameters passed with 'chunked' (which the RFC doesn't allow)
		TInt pos = xfrCoding.Locate(';');
		if( pos == 0 )
			{
			// No transfer extension before the parameter pair!
			User::Leave(KErrHttpDecodeTransferEncoding);
			}
		if( pos == KErrNotFound )
			{
			// A transfer encoding with no parameter. Trim surrounding whitespace.
			InetProtTextUtils::RemoveWhiteSpace(xfrCoding, InetProtTextUtils::ERemoveBoth);

			// Create and set the new header part at position zero
			SetNewFStringPartL(aHeader, xfrCodingsFound, xfrCoding);
			}
		else
			{
			// Found the ';' separator - presumably a parameter/value pair.
			TPtrC8 xfrExtn;
			xfrExtn.Set(xfrCoding.Left(pos));
			InetProtTextUtils::RemoveWhiteSpace(xfrExtn, InetProtTextUtils::ERemoveBoth);

			// Set the transfer extension as a new header part
			CHeaderFieldPart* part = SetNewFStringPartL(aHeader, xfrCodingsFound, xfrExtn);

			// Get the param. Advance over the separator to the attrib-value pair.
			xfrCoding.Set(xfrCoding.Mid(pos + 1));

			// Search for '=' separator. If not found, then the param/value pair
			// is badly formed - ignore for the sake of robustness.
			pos = xfrCoding.Locate(TChar('='));
			if( pos > 0 ) // shortest parameter name, 1 char
				{
				TPtrC8 parameter;
				parameter.Set(xfrCoding.Left(pos));
				InetProtTextUtils::RemoveWhiteSpace(parameter, InetProtTextUtils::ERemoveBoth);

				TPtrC8 paramValue;
				paramValue.Set(xfrCoding.Mid(pos + 1));
				InetProtTextUtils::RemoveWhiteSpace(paramValue, InetProtTextUtils::ERemoveBoth);

				// set the parameter of the header part
				SetNewFStringParamL(*part, parameter, paramValue);
				}
			}
		++xfrCodingsFound;
		}
	}

void CHttpGeneralHeaderReader::DecodeContentTypeL(RHeaderField& aHeader) const
/**
	Decodes the content-type header. RFC2616 section 14.17

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
	@param		aHeader	The content-type header field to decode.
	@leave		RHeaderField::RawDataL
	@leave		CHttpReader::SetNewFStringPartL
	@leave		CHttpReader::SetNewFStringParamL
	@leave		KErrHttpDecodeContentType	There was a transfer-extension with 
											no token before the parameter.
*/
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	TInt remaining = rawData.Length();

	// Search for '\n' separator. In the case when a duplicate header has been received,
	// only use the first instance of the valid data.
	TInt newLinePos = rawData.Locate('\n');
	if (newLinePos != KErrNotFound)
		{
		rawData.Set(rawData.Left(newLinePos));
		}

	// Search for ';' separator
	TInt pos = rawData.Locate(TChar(';'));
	if( pos == 0 )
		{
		// No media type!
		User::Leave(KErrHttpDecodeContentType);
		}
	if( pos == KErrNotFound )
		{
		// Media type only - no parameter. Trim surrounding whitespace.
		InetProtTextUtils::RemoveWhiteSpace(rawData, InetProtTextUtils::ERemoveBoth);

		// create and set the new header part at position zero
		SetNewFStringPartL(aHeader, 0, rawData);
		}
	else
		{
		// found the separator; there is presumably a parameter/value pair
		TPtrC8 mediaType;
		mediaType.Set(rawData.Left(pos));
		InetProtTextUtils::RemoveWhiteSpace(mediaType, InetProtTextUtils::ERemoveBoth);

		// set the media type as a new header part
		CHeaderFieldPart* part = SetNewFStringPartL(aHeader, 0, mediaType); // part 0, i.e. the first (and only) part

		// get the param
		remaining -= pos;
		if (remaining <= 0)
			User::Leave(KErrHttpDecodeContentType);
		rawData.Set(rawData.Mid(pos + 1)); // move rawData onto the string after the ; char

		TPtrC8 token;
		while (remaining > 0)
			{
			remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, token, ';');
			if (!token.Length())
				{
				// no more tokens.
				break;
				}
			SetParamNameAndValueL(*part, token, ETrue);
			}
		}
	}

void CHttpGeneralHeaderReader::DecodeContentDispositionL(RHeaderField& aHeader) const
/**
	Decodes the content-dispostion header. RFC2183 section 2

		disposition				:=	"Content-Disposition" ":" 
									disposition-type
									*(";" disposition-parm)
									
		disposition-type		:=	"inline"
									/ "attachment"
									/ extension-token 
									; values are not case-sensitive
		
		disposition-param		:=	filename-parm
                       				/ creation-date-parm
                       				/ modification-date-parm
                       				/ read-date-parm
                       				/ size-parm
                       				/ parameter
		
		filename-parm 			:= 	"filename" "=" value

     	creation-date-parm 		:= 	"creation-date" "=" quoted-date-time

     	modification-date-parm 	:= 	"modification-date" "=" quoted-date-time

     	read-date-parm 			:= 	"read-date" "=" quoted-date-time

     	size-parm 				:= 	"size" "=" 1*DIGIT

     	quoted-date-time 		:= 	quoted-string
                      				; contents MUST be an RFC 822 `date-time'
                      				; numeric timezones (+HHMM or -HHMM) MUST be used
		
	
	@internalComponent
	@param		aHeader	The content-disposition header field to decode.
	@leave		RHeaderField::RawDataL
	@leave		CHttpReader::SetNewFStringPartL
	@leave		CHttpReader::SetNewFStringParamL
	@leave		KErrHttpDecodeContentDisposition	There was a transfer-extension with 
											no token before the parameter.
*/
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	TInt remaining = rawData.Length();

	// Search for '\n' separator. In the case when a duplicate header has been received,
	// only use the first instance of the valid data.
	TInt newLinePos = rawData.Locate('\n');
	if (newLinePos != KErrNotFound)
		{
		rawData.Set(rawData.Left(newLinePos));
		}

	// Search for ';' separator
	TInt pos = rawData.Locate(TChar(';'));
	if( pos == 0 )
		{
		// No disposition type!
		User::Leave(KErrHttpDecodeContentDisposition);
		}
	if( pos == KErrNotFound )
		{
		// Disposition type only - no parameter. Trim surrounding whitespace.
		InetProtTextUtils::RemoveWhiteSpace(rawData, InetProtTextUtils::ERemoveBoth);

		// create and set the new header part at position zero
		SetNewFStringPartL(aHeader, 0, rawData);
		}
	else
		{
		// found the separator; there is presumably a parameter/value pair
		TPtrC8 mediaType;
		mediaType.Set(rawData.Left(pos));
		InetProtTextUtils::RemoveWhiteSpace(mediaType, InetProtTextUtils::ERemoveBoth);

		// set the disposition type as a new header part
		CHeaderFieldPart* part = SetNewFStringPartL(aHeader, 0, mediaType); // part 0, i.e. the first (and only) part

		// get the param
		remaining -= pos;
		if (remaining <= 0)
			{
			User::Leave(KErrHttpDecodeContentDisposition); 
			}
		rawData.Set(rawData.Mid(pos + 1)); // move rawData onto the string after the ; char

		while (remaining > 0)
			{
				if ( rawData.Find(KEqualQuote) != KErrNotFound )
				{			     			     
				TPtrC8 rawDataBuf;
				rawDataBuf.Set(rawData);
				
				TPtrC8 param;		
				InetProtTextUtils::ExtractNextTokenFromList(rawDataBuf, param, '=');
				
				TPtrC8 paramVal;					  
				if(rawDataBuf.Locate('"') == 0)
				{				   					 
					// Go past the first '"' after the '='
					rawDataBuf.Set(rawDataBuf.Mid(rawDataBuf.Locate('"')+1));
					if ( rawDataBuf.Length() > 0)  
					{
						InetProtTextUtils::ExtractNextTokenFromList(rawDataBuf, paramVal, '"');
					
					rawData.Set(rawDataBuf);
					if (rawData.Locate(';') != KErrNotFound)
					{
					// Go past the ';' character.
					rawData.Set(rawData.Mid(1));
					}
					
					SetNewFStringParamL(*part, param, paramVal);
					}
					else
					{
					// No value for the param, so set an empty param.
					SetNewFStringParamL(*part, param, KNullDesC8());	
					}
				}
				else
				{
					InetProtTextUtils::ExtractNextTokenFromList(rawDataBuf, paramVal, ';');
					rawData.Set(rawDataBuf);
					SetNewFStringParamL(*part, param, paramVal);			    
				}
				}
				else
				{
					TPtrC8 token;
					remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, token,';' );				
					
					if (!token.Length())
					{
					// no more tokens.
					break;
					}
					SetParamNameAndValueL(*part, token, ETrue);
				}
			}			
		}
	}
	
/*
 *	Methods from CHeaderReader
 */

void CHttpGeneralHeaderReader::DecodeHeaderL(RHeaderField& aHeader)
/** 
	Decodes the raw header field value.
	@internalComponent
	@param		aHeader		The header field to decode.
	@leave		CHttpGeneralHeaderReader::DecodeCacheControlL
	@leave		CHttpGeneralHeaderReader::DecodeConnectionL
	@leave		CHttpGeneralHeaderReader::DecodeContentTypeL
	@leave		CHttpReader::DecodeDateL
	@leave		CHttpReader::DecodeGenericL
	@leave		CHttpReader::DecodeGenericNumberL
	@leave		KErrNotSupported	The reader was asked to decode a header that
									is does not support.
*/
	{
	RStringF fieldStr = iStrPool.StringF(aHeader.Name());
	switch( fieldStr.Index(iStringTable) )
		{
	case HTTP::ECacheControl:
		{
		DecodeCacheControlL(aHeader);
		} break;
	case HTTP::EConnection:
		{
		DecodeConnectionL(aHeader);
		} break;
	case HTTP::EDate:
	case HTTP::EExpires:
		{
		DecodeDateL(aHeader);
		} break;
	case HTTP::EPragma:
	case HTTP::EContentLanguage:
	case HTTP::EContentLocation: 	// Only expect one value
	case HTTP::EContentMD5: 		// Only expect one value
	case HTTP::EUpgrade:
		{
		DecodeGenericL(aHeader, KCommaNewline());
		} break;
	case HTTP::ETransferEncoding:
		{
		DecodeTransferEncodingL(aHeader);
		} break;
	case HTTP::EContentEncoding:
		{
		DecodeTokenListHeaderL(aHeader, KCommaSpaceSep());
		} break;
	case HTTP::EContentLength:
		{
		DecodeGenericNumberL(aHeader);
		} break;
	case HTTP::EContentType:
		{
		DecodeContentTypeL(aHeader);
		} break;
	case HTTP::EContentDisposition:
		{
		DecodeContentDispositionL(aHeader);	
		} break;
	default:
		User::Leave(KErrNotSupported);
		break;
		}
	}
