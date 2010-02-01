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

#include "chttpclientheaderwriter.h"

#include <httpstringconstants.h>
#include <http/rhttpsession.h>
#include <httperr.h>
#include <inetprottextutils.h>

#include "CHeaderField.h"

_LIT8(KSemiSpaceSep,"; ");
_LIT8(KCodecSpace, " ");

const TInt KMaxNumPortDigits			= 5;
const TInt KMaxNumPortValue = 0xffff;	// RFC 793

const TInt KNormalAcceptHeaderLen = 96;
const TInt KNormalAcceptCharsetHeaderLen = 64;
const TInt KNormalAcceptEncodingHeaderLen = 64;
const TInt KNormalAcceptLanguageHeaderLen = 64;
CHttpClientHeaderWriter* CHttpClientHeaderWriter::NewL(RStringPool aStringPool)
/** 
	Factory constructor.
	@internalComponent
	@param		aStringPool	The current string pool.
	@return		A pointer to a fully initialised object.
	@leave		KErrNoMemory	Not enough memory to create object.
*/
	{
	return new (ELeave) CHttpClientHeaderWriter(aStringPool);
	}

CHttpClientHeaderWriter::~CHttpClientHeaderWriter()
/**
	Destructor
	@internalComponent
*/
	{
	}

CHttpClientHeaderWriter::CHttpClientHeaderWriter(RStringPool aStringPool)
: CHttpHeaderWriter(aStringPool)
/**
	Constructor
	@internalComponent
	@param		aStringPool	The current string pool.
*/	{
	}

void CHttpClientHeaderWriter::EncodeAcceptL(RHeaderField& aHeader) const
/**
	Encodes the accept header. RFC2616 section 14.1 - 
  
		Accept			   =	"Accept" ":" #( media-range [ accept-params ] )
							
		media-range			=	( "*" "/" "*"
							|	( type "/" "*" )
							|	( type "/" subtype )
							)	*( ";" parameter )

		accept-params		=	";" "q" "=" qvalue *( accept-extension )
		accept-extension	=	";" token [ "=" ( token | quoted-string ) ]

	The accept header value is a comma separated list of values that may be
	empty.
	@internalComponent
	@param		aHeader	The accept header field to encode.
	@leave		CHttpWriter::DoGeneralAcceptHeaderL
	@todo		Is this function needed at all - go straight to DoGeneralAcceptHeaderL?
*/
	{
	DoGeneralAcceptHeaderL( KNormalAcceptHeaderLen,
						  aHeader, 
						  iStringPool.StringF(HTTP::EAccept, iStringTable),
						  KErrHttpEncodeAccept
						  );
	}

void CHttpClientHeaderWriter::EncodeAcceptCharsetL(RHeaderField& aHeader) const
/**
	Encodes the accept-charset header. RFC2616 section 14.2 - 
	
		Accept-Chartset		=	"Accept-Charset" ":" 1#( ( charset | "*" ) [ ";" "q" "=" qvalue ] )

	The accept-charset header value is a comma separated list of values that has
	at least one value.
	@internalComponent
	@param		aHeader	The accept-charset header field to encode.
	@leave		CHttpWriter::DoGeneralAcceptHeaderL
	@todo		Is this function needed at all - go straight to DoGeneralAcceptHeaderL?
*/
	{
	DoGeneralAcceptHeaderL( KNormalAcceptCharsetHeaderLen,
						  aHeader,
						  iStringPool.StringF(HTTP::EAcceptCharset,iStringTable),
						  KErrHttpEncodeAcceptCharset
						  );
	}

void CHttpClientHeaderWriter::EncodeAcceptLanguageL(RHeaderField& aHeader) const
/**
	Encodes the accept-language header. RFC2616 section 14.4 - 
	
		Accept-Language		=	"Accept-Language" ":" 1#( language-range [ ";" "q" "=" qvalue ] )

		language-range		= ( ( 1*8ALPHA *( "-" 1*8ALPHA ) ) ! "*" )

	The accept-language header value is a comma separated list of values that has
	at least one value.
	@internalComponent
	@param		aHeader	The accept-language header field to encode.
	@leave		CHttpWriter::DoGeneralAcceptHeaderL
	@todo		Is this function needed at all - go straight to DoGeneralAcceptHeaderL?
*/
	{
	DoGeneralAcceptHeaderL(KNormalAcceptLanguageHeaderLen,
						  aHeader,
						  iStringPool.StringF(HTTP::EAcceptLanguage,iStringTable),
						  KErrHttpEncodeAcceptLanguage
						  );
	}

void CHttpClientHeaderWriter::EncodeAuthorizationL(RHeaderField& aHeader) const
/**
	Encodes the authorization header. RFC2616 section 14.4 - 
	
		Authorization		=	"Authorization" ":" credentials
		credentials			=	auth-scheme #auth-param
							|	"Digest" digest-response
							|	"Basic" basic-credentials

		basic-credentials	=	base64-user-pass
		base64-user-pass	=	<base64 [4] encoding of user-pass, except not limited to 76 char/line>
		user-pass			=	userid ":" password
		userid				=	*<TEXT excluding ":">
		password			=	*TEXT
		
		digest-response		=	1#( username | realm | nonce | digest-uri |	response |
								[ algorithm ] | [ cnonce ] |
								[ opaque ] | [ message-qop ] |
								[ nonce-count ] | [ auth-param ] )

		username			=	"username" "=" username-value
		username-value		=	quoted-string
		digest-uri			=	"uri" "=" digest-uri-value
		digest-uri-value	=	request-uri ; As specified by HTTP/1.1
		message-qop			=	"qop" "=" qop-value
		cnonce				=	"cnonce" "=" cnonce-value
		cnonce-value		=	nonce-value
		nonce-count			=	"nc" "=" nc-value
		nc-value			=	8LHEX
		response			=	"response" "=" request-digest
		request-digest		=	<"> 32LHEX <">
		LHEX				=	"0" | "1" | "2" | "3" |
								"4" | "5" | "6" | "7" |
								"8" | "9" | "a" | "b" |
								"c" | "d" | "e" | "f"
	
	Encoding here is interpreted very simply. Parts are encoded with no 
	punctuation after them and parameters are comma separated with the value in
	quotes. This means that for basic authentication, part 1 must be 'Basic' and
	part 2 is the credentials. For digest, part1 is 'Digest', and the 
	digest-response is stored in parameters.
	@internalComponent
	@param		aHeader	The authorization header field to encode.
	@leave		RHeaderField::BeginRawDataL
	@leave		RHeaderField::PartsL
	@leave		RHeaderField::WriteRawDataL
	@leave		KErrHttpEncodeAuthorization	There were no parts, or the part
											value type or the parameter value 
											type was not a string or a folded 
											string.
*/
	{
	__START_PERFORMANCE_LOGGER();
	// Check part 1
	aHeader.BeginRawDataL();
	THeaderFieldPartIter iter = aHeader.PartsL();
	for( iter.First(); !iter.AtEnd(); )
		{
		const CHeaderFieldPart* part = iter();
		if( part == NULL )
			{
			// No parts!!
			User::Leave(KErrHttpEncodeAuthorization);
			}
		THTTPHdrVal ptVal = part->Value();
		switch( ptVal.Type() )
			{
		case THTTPHdrVal::KStrFVal:
			{
			aHeader.WriteRawDataL(ptVal.StrF().DesC());
			} break;
		case THTTPHdrVal::KStrVal:
			{
			aHeader.WriteRawDataL(ptVal.Str().DesC());
			} break;			
		default:
			User::Leave(KErrHttpEncodeAuthorization);
			break;
			}

		// Now output the params
		THeaderFieldParamIter paramIter = part->Parameters();
		for( paramIter.First(); !paramIter.AtEnd(); )
			{
			const  TInt param = paramIter()->Name().Index(iStringTable);

			const TBool quoted = (param != HTTP::ENc && param != HTTP::EAlgorithm  && param != HTTP::EUri  && param != HTTP::EStale);
			aHeader.WriteRawDataL(' ');
			aHeader.WriteRawDataL(paramIter()->Name().DesC());
			aHeader.WriteRawDataL('=');
			
			if( quoted ) 
                {
                aHeader.WriteRawDataL('"');
                }

			THTTPHdrVal paramVal = paramIter()->Value();
			switch (paramVal.Type())
				{
			case THTTPHdrVal::KStrFVal:
				{
				aHeader.WriteRawDataL(paramVal.StrF().DesC());
				} break;				
			case THTTPHdrVal::KStrVal:
				{
				aHeader.WriteRawDataL(paramVal.Str().DesC());
				} break;				
			default:
				User::Leave(KErrHttpEncodeAuthorization);
				break;
				}
			if( quoted )
				{
				aHeader.WriteRawDataL('"');
				}

			
			++paramIter;
			if (!paramIter.AtEnd())
				{
				// This is not the last param so add a comma
				aHeader.WriteRawDataL(',');
				}
			}
		++iter;
		if( !iter.AtEnd() )
			{
			// This is not the last part so add a space.
			aHeader.WriteRawDataL(' ');
			}
		}
	aHeader.CommitRawData();
	__END_PERFORMANCE_LOGGER(_L(",CHttpClientHeaderWriter::EncodeAuthorizationL()"));
	}

void CHttpClientHeaderWriter::EncodeHostL(RHeaderField& aHeader) const
/**
	Encodes the host header. RFC2616 section 14.23

		Host			=	"Host" ":" host [ ":" port ]

	The host header value may be empty.
	@internalComponent
	@param		aHeader	The host header field to encode.
	@leave		RHeaderField::BeginRawDataL
	@leave		RHeaderField::PartsL
	@leave		RHeaderField::WriteRawDataL
	@leave		KErrHttpEncodeHost	There were no parts, or the part value type
									or the parameter value type was not a string
									or a folded string.
*/
	{
	__START_PERFORMANCE_LOGGER();
	// Our convention will be that a string called HTTP::EPort will be used to
	// set a parameter holding the integer port number
	// Check part 1
	THeaderFieldPartIter iter1 = aHeader.PartsL();
	iter1.First();
	if( iter1.AtEnd() )
		{
		User::Leave(KErrHttpEncodeHost);
		}
	const CHeaderFieldPart* part = iter1();
	if( part == NULL)
		{
		User::Leave(KErrHttpEncodeHost);
		}
	THTTPHdrVal pt1val = part->Value();
	if( pt1val.Type() != THTTPHdrVal::KStrFVal )
		{
		User::Leave(KErrHttpEncodeHost);
		}
	// Write the host string
	RStringF hostStr = pt1val.StrF();
	aHeader.BeginRawDataL();
	aHeader.WriteRawDataL(hostStr.DesC());

	// Check for a port number
	THeaderFieldParamIter iter2 = part->Parameters();
	iter2.First();
	if( !iter2.AtEnd() )
		{
		// Got a parameter - if its the port, check then write it
		const CHeaderFieldParam* param = iter2();
		if( !param )
			{
			User::Leave(KErrHttpEncodeHostPort);
			}
		if( param->Name() == iStringPool.StringF(HTTP::EPort,iStringTable) )
			{
			// Get the port value
			THTTPHdrVal portVal = param->Value();
			if (portVal.Type() != THTTPHdrVal::KTIntVal)
				{
				User::Leave(KErrHttpEncodeHostPort);
				}
			TBuf8<KMaxNumPortDigits> portDesC;
			const TInt portValue = portVal.Int();
			if(portValue > KMaxNumPortValue) 
				{
				User::Leave(KErrHttpEncodeHostPort);
				}
			portDesC.Num(portValue);
			aHeader.WriteRawDataL(':');
			aHeader.WriteRawDataL(portDesC);
			}
		}
	aHeader.CommitRawData();
	__END_PERFORMANCE_LOGGER(_L(",CHttpClientHeaderWriter::EncodeHostL()"));
	}

void CHttpClientHeaderWriter::EncodeTEL(RHeaderField& aHeader) const
/**
	Encode the TE header. RFC2616 section 14.39.

		TE			=	"TE" ":" #( t-codings )
		t-codings	=	"trailers" | ( transfer-extension [ accept-params ] )
	The TE header field value is comma separated list that may be empty.
	@internalComponent
	@param		aHeader	The host header field to encode.
	@leave		CHttpWriter::DoGeneralAcceptHeaderL
*/
	{       
	DoGeneralAcceptHeaderL(KHttpDefaultRawChunkSize, aHeader, iStringPool.StringF(HTTP::ETE,iStringTable), KErrHttpEncodeTE);
	}

void CHttpClientHeaderWriter::EncodeUserAgentL(RHeaderField& aHeader) const
	{
	// User-Agent     = "User-Agent" ":" 1*( product | comment )
	//  Example: User-Agent: CERN-LineMode/2.15 libwww/2.17b3
	DoTokenListHeaderL(aHeader, KCodecSpace, KErrHttpEncodeUserAgent);
	}

void CHttpClientHeaderWriter::EncodeCookieL(RHeaderField& aHeader) const
	{
		__START_PERFORMANCE_LOGGER();
	// NETSCAPE Persistent Client State HTTP Cookies (http://www.netscape.com/newsref/std/cookie_spec.html)
	// Cookie Header Line = "Cookie: " 1*COOKIE
	// COOKIE = NAME=VALUE

	// RFC 2965 HTTP State Management Mechanism
	// cookie = "Cookie" cookie-version 1* ((";" | ",") cookie-value)
	// cookie-value = NAME "=" VALUE [";" path] [";" domain] [";" port]
	// NAME = attr
	// VALUE = value
	// path = "$Path" "=" value
	// domain = "$Domain" "=" value
	// port = "$Port" ["=" <"> value <">]
	

	// cookies are stored internally as parts in a header (the cookie header);
	// each header part has a number of parameters which indicate parts of the cookie header. Netscape style cookies do not have
	// a version parameter. If we get a netscape style cookie then only the name and value get written out.

	// Otherwise the cookie will be either a RFC 2109 or RFC 2965 style cookie which can be treated identically
	
	THeaderFieldPartIter iter = aHeader.PartsL();
	iter.First();
	if (iter.AtEnd())
		User::Leave(KErrHttpEncodeCookie);

	TBool usingNetscapeStyleCookies = ETrue;
	TBool setVersion = EFalse;
	
	aHeader.BeginRawDataL();
	while(!iter.AtEnd())
		{
		// Write out one cookie
		const CHeaderFieldPart* part = iter();
		if (part == NULL)
			User::Leave(KErrHttpEncodeCookie);
		
		if (!setVersion)
			{
			setVersion = ETrue;
			RStringF version = iStringPool.StringF(HTTP::EVersion,iStringTable);
			THeaderFieldParamIter paramIter = part->Parameters();
			paramIter.First();
			while (!paramIter.AtEnd())
				{
				const CHeaderFieldParam* param = paramIter();
				if (param->Name() == version)
					{
					usingNetscapeStyleCookies = EFalse;
					EncodeOneCookieAttributeL(aHeader, *param);
					aHeader.WriteRawDataL(KSemiSpaceSep);
					break;
					}
				++paramIter;
				}
			}
		EncodeOneCookieL(aHeader, *part, usingNetscapeStyleCookies);

		++iter;
		if (!iter.AtEnd())
			aHeader.WriteRawDataL(KSemiSpaceSep);
		}
	aHeader.CommitRawData();
	__END_PERFORMANCE_LOGGER(_L(",CHttpClientHeaderWriter::EncodeCookieL()"));
	}


void CHttpClientHeaderWriter::EncodeOneCookieAttributeL(RHeaderField& aHeader, const CHeaderFieldParam& aAttribute) const
	{
	const TBool quoted = ( aAttribute.Name().Index(iStringTable) == HTTP::ECookiePort );
	aHeader.WriteRawDataL( '$' );
	aHeader.WriteRawDataL( aAttribute.Name().DesC() );
	aHeader.WriteRawDataL( '=' );
	if( quoted )
		{
		aHeader.WriteRawDataL( '"' );
		}
	aHeader.WriteRawDataL( aAttribute.Value().StrF().DesC() );
	if( quoted )
		{
		aHeader.WriteRawDataL( '"' );
		}
	}

void CHttpClientHeaderWriter::EncodeOneCookieL(RHeaderField& aHeader, const CHeaderFieldPart& aCookieFieldPart, TBool aIsNetscapeStyleCookie) const
	{
	// Always write out the name and value attributes which should come first and second. Only write out the other attributes 
	// if they are one of Path, Domain, Port

	THeaderFieldParamIter paramIter = aCookieFieldPart.Parameters();
	paramIter.First();

	TBool separatorNeeded = EFalse;
	while( !paramIter.AtEnd() )
		{
		const CHeaderFieldParam* param = paramIter();

		switch( param->Name().Index(iStringTable) )
			{
		case HTTP::EDomain:
		case HTTP::EPath:
		case HTTP::ECookiePort:
			{
			if( !aIsNetscapeStyleCookie )
				{
				aHeader.WriteRawDataL(KSemiSpaceSep);
				EncodeOneCookieAttributeL(aHeader, *param);
				}
			} break;
		case HTTP::ECookieName:
			{
			if( separatorNeeded )
				aHeader.WriteRawDataL(KSemiSpaceSep);
			aHeader.WriteRawDataL(param->Value().Str().DesC());
			} break;
		case HTTP::ECookieValue:
			{
			aHeader.WriteRawDataL('=');
			aHeader.WriteRawDataL(param->Value().Str().DesC());
			separatorNeeded = ETrue;
			} break;
		default:
			break;
			}
		++paramIter;
		}
	}

		
/*
 *	Methods from CHeaderWriter
 */

void CHttpClientHeaderWriter::EncodeHeaderL(RHeaderField& aHeader)
	{
	RStringF fieldStr = iStringPool.StringF(aHeader.Name());
	switch( fieldStr.Index(iStringTable) )
		{
	case HTTP::EAccept:
		{
		EncodeAcceptL(aHeader);
		} break;
	case HTTP::EAcceptCharset:
		{
		EncodeAcceptCharsetL(aHeader);
		} break;
	case HTTP::EAuthorization:
		{
		EncodeAuthorizationL(aHeader);
		} break;
	case HTTP::EAcceptLanguage:
		{
		EncodeAcceptLanguageL(aHeader);
		} break;
	case HTTP::EAcceptEncoding:
		{
		DoGeneralAcceptHeaderL(KNormalAcceptEncodingHeaderLen, aHeader, iStringPool.StringF(HTTP::EAcceptEncoding,iStringTable), KErrHttpEncodeAcceptEncoding);
		} break;
	case HTTP::EHost:
		{
		EncodeHostL(aHeader);
		} break;
	case HTTP::EUserAgent:
		{
		EncodeUserAgentL(aHeader);
		} break;
	case HTTP::ECookie:
		{
		EncodeCookieL(aHeader);
		} break;
	case HTTP::EIfMatch:
		{
		DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeIfMatch);
		} break;
	case HTTP::EIfNoneMatch:
		{
		DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeIfMatch);
		} break;
	case HTTP::EIfModifiedSince:
		{
		EncodeGenericDateL(aHeader, KErrHttpEncodeIfModifiedSince);
		} break;
	case HTTP::EIfUnmodifiedSince:
		{
		EncodeGenericDateL(aHeader, KErrHttpEncodeIfUnmodifiedSince);
		} break;
	case HTTP::ECookie2:
		{
		EncodeGenericNumberHeaderL(aHeader, KErrHttpEncodeCookie2);
		} break;
	case HTTP::ETE:
		{
		EncodeTEL(aHeader);
		} break;
	default:
		User::Leave(KErrNotSupported);
		break;
		}
	}
