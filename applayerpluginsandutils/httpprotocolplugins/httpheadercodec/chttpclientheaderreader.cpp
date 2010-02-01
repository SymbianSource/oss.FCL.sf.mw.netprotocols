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

#include "chttpclientheaderreader.h"

#include <http/rhttpsession.h>
#include <httpstringconstants.h>
#include <httperr.h>
#include <inetprottextutils.h>

_LIT8(KFieldSeparator, "\n");
_LIT8(KCommaNewline, ",\n");

CHttpClientHeaderReader* CHttpClientHeaderReader::NewL(RStringPool aStringPool)
	{
	return new (ELeave) CHttpClientHeaderReader(aStringPool);
	}

CHttpClientHeaderReader::~CHttpClientHeaderReader()
	{
	// The reader and writer classes are owned and destroyed by the base class CHeaderCodec
	}

CHttpClientHeaderReader::CHttpClientHeaderReader(RStringPool aStringPool)
: CHttpHeaderReader(aStringPool)
	{
	}

void CHttpClientHeaderReader::DecodeSetCookieL(RHeaderField& aHeader) const
	{
   
	// The generic header will add each cookie on a separate line. Each cookie forms 1 part.
	// Each cookie will be made up of a number of parameters with name being the first and value the second, following that 
	// will be just the actual parameters that form part of a cookie.
	
	TPtrC8 buffer;
	aHeader.RawDataL(buffer);	
	TInt remainingSize = buffer.Length();

	TPtrC8 token;
	TInt cookieCount = 0;
	while (remainingSize > 0)
		{
		// Extract tokens seperated by header field separator defined as KFieldSeparator in textmodestd.h
		remainingSize -= InetProtTextUtils::ExtractNextTokenFromList(buffer, token, KFieldSeparator);
		DecodeOneCookieL(aHeader, token, cookieCount);
		++cookieCount;
		}
	}

void CHttpClientHeaderReader::DecodeOneCookieL(RHeaderField& aHeader, const TDesC8& aCookieData, TInt aCookieNumber) const
	{
	// NETSCAPE Persistent Client State HTTP Cookies (http://www.netscape.com/newsref/std/cookie_spec.html)
	// SetCookie: NAME=VALUE [; expires=DATE] [; path=PATH] [; domain= DOMAIN_NAME] [; secure]

	// RFC 2109
	// set-cookie = "Set-Cookie:" 1#cookie
	// cookie = NAME "=" VALUE *(";" cookie-av)
	// cookie-av = "Comment" "=" value | "Domain" "=" value | "Max-Age" "=" value | "Path" "=" value | "Secure" |
	//				| "Version" "=" 1*DIGIT


	// RFC 2965 
	// set-cookie = "Set-Cookie2:" 1#cookie
	// cookie = NAME "=" VALUE *(";" set-cookie-av)
	// set-cookie-av = "Comment" "=" value | CommentURIL "=" <"> http_URL <">| "Discard" | "Domain" "=" value 
	//					| "Max-Age" "=" value | "Path" "=" value | "Port" ["=" <"> portlist <">] |
	//					"Secure" | "Version" "=" 1*DIGIT
	// portlist = 1#portnum
	// portnum = 1*DIGIT

	CHeaderFieldPart* cookie = SetNewFStringPartL(aHeader, aCookieNumber, iStrPool.StringF(HTTP::ECookie,iStringTable).DesC());
	TPtrC8 buffer(aCookieData);
	TInt remainingSize = buffer.Length();

	TPtrC8 token;
	TBool setCookieNameAndValue = EFalse;
	while (remainingSize > 0)
		{
		remainingSize -= InetProtTextUtils::ExtractNextTokenFromList(buffer, token, ';');
		
		if (setCookieNameAndValue)
			{
			// first attribute is NAME = VALUE which needs to be stored as 2 attributes
			SetParamNameAndValueL(*cookie, token, EFalse);
			}
		else
			{
			SetCookieNameAndValueL(*cookie, token);
			setCookieNameAndValue = ETrue;
			}
		}
	}

void CHttpClientHeaderReader::SetCookieNameAndValueL(CHeaderFieldPart& aCookie, const TDesC8& aNameValue) const
	{

	TInt equalPos = aNameValue.Locate('=');
	if (equalPos <= 0)
		User::Leave(KErrHttpDecodeCookie);

	TPtrC8 nameVal(aNameValue.Left(equalPos));
	InetProtTextUtils::RemoveWhiteSpace(nameVal, InetProtTextUtils::ERemoveBoth);
	TPtrC8 name(iStrPool.StringF(HTTP::ECookieName,iStringTable).DesC());
	SetNewStringParamL(aCookie, name ,nameVal);

	TPtrC8 valueVal(aNameValue.Right(aNameValue.Length() - (equalPos+1)));
	InetProtTextUtils::RemoveWhiteSpace(valueVal, InetProtTextUtils::ERemoveBoth);
	TPtrC8 value(iStrPool.StringF(HTTP::ECookieValue,iStringTable).DesC());
	SetNewStringParamL(aCookie, value ,valueVal);
	}


// Convert the WWW-Authenticate header field from OTA to generic form.
void CHttpClientHeaderReader::DecodeWWWAuthenticateL( RHeaderField& aHeader ) const
	{
	// RFC2616, section 14.47 WWW-Authenticate
	// RFC2617, 'HTTP Authentication: Basic and Digest Access Authentication'
	//
	// WWW-Authenticate  = "WWW-Authenticate" ":" 1#challenge
	// challenge   = auth-scheme 1*SP 1#auth-param
	// auth-scheme    = token
	// auth-param     = token "=" ( token | quoted-string )

	// There may be one or more challenge, in a comma-separated list.
	TPtrC8 buffer;
	aHeader.RawDataL( buffer );

	TInt totalBytesConsumed = 0;
	TInt numChallenges = 0;
	CHeaderFieldPart* part = NULL;
	TBool done = EFalse;
	while ( !done )
		{
		_LIT8( commaSpaceNewline, ", \n" );
		TPtrC8 token;
		TInt bytesConsumed = InetProtTextUtils::ExtractNextTokenFromList( buffer, token, commaSpaceNewline );

		done = (bytesConsumed == 0);
		if ( done && ( numChallenges == 0 ) ) // if we didn't find _anything_ at all...
			{
			User::Leave( KErrHttpDecodeWWWAuthenticate );
			}

		if ( !done && ( token.Length() > 0 ) )
			{
			totalBytesConsumed += bytesConsumed;

			TBool equalsPresent = ( token.Locate( '=' ) != KErrNotFound );

			if ( ( totalBytesConsumed == bytesConsumed ) && equalsPresent )
				{
				// The first token has an equals sign in it. That
				// can't be as it has to be an authentication scheme
				User::Leave( KErrHttpDecodeWWWAuthenticate );
				}

			if ( !equalsPresent )
				{
				// Got a new part. Add it.
				++numChallenges;
				part = SetNewFStringPartL( aHeader, numChallenges - 1, token );
				
			if( token.Compare( iStrPool.StringF(HTTP::ENTLM, iStringTable).DesC() ) == 0 )
				{
 					TInt consumed = InetProtTextUtils::ExtractNextTokenFromList( buffer, token, commaSpaceNewline );
 					if( consumed > 0 )
 						{
 						++numChallenges;
 						part = SetNewFStringPartL( aHeader, numChallenges -1, token );
						}
					}
				}
			else
				{
				// Got a param & parameter value.
				TPtrC8 paramName;
				TInt paramBytesConsumed = InetProtTextUtils::ExtractNextTokenFromList( token, paramName, '=' );

				if ( paramBytesConsumed == 0 )
					{
					User::Leave( KErrHttpDecodeBasicAuth );
					}

				// Obtain the parameter value. It is a string which
				// may or may not be quoted. 
				TPtrC8 paramVal;
				if ( token.Length() > 0 && token[0] == '"' )
					{
					bytesConsumed += InetProtTextUtils::ExtractQuotedStringL( token, paramVal );
					}
				else
					{
					paramVal.Set( token );
					}

				SetNewStringParamL( *part, paramName, paramVal );
				}
			}
		}
	}

/*
 *	Methods from CHeaderReader
 */

void CHttpClientHeaderReader::DecodeHeaderL(RHeaderField& aHeader)
	{
	RStringF fieldStr = iStrPool.StringF(aHeader.Name()); // this doesn't have to be closed
	switch(fieldStr.Index(iStringTable))
		{
		case HTTP::EWWWAuthenticate:
			{
			DecodeWWWAuthenticateL(aHeader);
			} break;
		case HTTP::ESetCookie:
		case HTTP::ESetCookie2:
			DecodeSetCookieL(aHeader);
			break;
		case HTTP::EAge:
			DecodeGenericNumberL(aHeader);
			break;
		case HTTP::EVary:
		case HTTP::EContentLanguage:
		case HTTP::EUpgrade:
			DecodeGenericL(aHeader, KCommaNewline);
			break;
		case HTTP::ELastModified:
			DecodeDateL(aHeader);
			break;
		default:
			User::Leave(KErrNotSupported);
		}
	}
