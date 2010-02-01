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

#include "chttpserverheaderreader.h"

#include <http/rhttpsession.h>
#include <httpstringconstants.h>
#include <httperr.h>
#include <inetprottextutils.h>

_LIT8(KCommaSpaceSep,", ");
_LIT8(KFieldSeparator, "\n");
_LIT8(KCodecSpace, " ");
_LIT8(KSemiSpaceSep," ;");

const TUint KCommaChar		= ',';
const TUint KSemiColonChar	= ';';
const TUint KEqualsChar		= '=';


CHttpServerHeaderReader* CHttpServerHeaderReader::NewL(RStringPool aStringPool)
	{
	return new (ELeave) CHttpServerHeaderReader(aStringPool);
	}

CHttpServerHeaderReader::~CHttpServerHeaderReader()
	{
	}

CHttpServerHeaderReader::CHttpServerHeaderReader(RStringPool aStringPool)
: CHttpHeaderReader(aStringPool)
	{
	}

// Helper to DoAcceptL(), DoAcceptCharsetL(), DoAcceptLanguageL() and DoAcceptEncodingL() 
// for comma-separated lists incorporating q-values..
// name : value, value; q= 2.2, value,...
void CHttpServerHeaderReader::DecodeGenericAcceptL(RHeaderField& aHeader, TInt aErrorCode) const
	{
	
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	TInt remaining = rawData.Length();
	TPtrC8 token;
	TInt tokensFound = 0;
	while (remaining > 0)
		{
		remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, token, KCommaChar);

		TInt pos = token.Locate(KSemiColonChar);
		if (pos < 0)
			{
			// No parameters. Just store the field value
			InetProtTextUtils::RemoveWhiteSpace(token, InetProtTextUtils::ERemoveBoth);
			SetNewFStringPartL(aHeader, tokensFound, token);
			}
		else if (pos==0)
			{
			// No valid q-value. Just store the parameter.
			User::Leave(aErrorCode);
			}
		else
			{
			// parameter value(s) exist.

			if (pos==token.Length())
				// if no field value exists. i.e. an invalid header
				User::Leave(aErrorCode);

			// store the field
			TPtrC8 fieldValue(token.Left(pos));
			TPtrC8 parameters(token.Mid(pos+1));
			InetProtTextUtils::RemoveWhiteSpace(fieldValue, InetProtTextUtils::ERemoveBoth);

			CHeaderFieldPart* part = SetNewFStringPartL(aHeader, tokensFound, fieldValue);

			TPtrC8 thisParam;
			do {
				// check if there is another parameter
				pos = parameters.Locate(KSemiColonChar);
				if (pos > 0)
					{
					if (pos==token.Length())
						// if no field value exists. i.e. an invalid header
						User::Leave(aErrorCode);

					thisParam.Set(parameters.Left(pos));
					parameters.Set(parameters.Mid(pos+1));
					}
				else
					thisParam.Set(parameters);

				
				TInt pPos = thisParam.Locate(KEqualsChar);
				if (pPos <= 0 || pPos==thisParam.Length())
					// Invalid parameter, missing '=' char, or missing field value.
					User::Leave(aErrorCode);

				TPtrC8 paramField(thisParam.Left(pPos));
 				TPtrC8 paramData(thisParam.Mid(pPos + 1));

				SetNewFStringParamL(*part, paramField, paramData);

				} while (pos > 0);
			}
		++tokensFound;
		}
		
		
	}

void CHttpServerHeaderReader::DecodeAuthorizationL(RHeaderField& aHeader) const
	{
	// RFC2616, section 14.8;  RFC2617, section 
	// Authorization  = "Authorization" ":" credentials
	// credentials = auth-scheme #auth-param
	// credentials = "Basic" basic-credentials
	// basic-credentials = base64-user-pass
    // base64-user-pass  = <base64 [4] encoding of user-pass, except not limited to 76 char/line>
    // user-pass   = userid ":" password
    // userid      = *<TEXT excluding ":">
    // password    = *TEXT
	
	// Parts are encoded with no punctuation after them and parameters are comma
	// separated with the value in quotes. This means that for basic
	// authentication, part 1 must be 'Basic' and part 2 is the
	// credentials. For digest, part1 is 'Digest', and the digest
	// response is stored in parameters.

	TPtrC8 buffer;
	aHeader.RawDataL(buffer);

	TInt totalBytesConsumed = 0;
	TInt numParam = 0;
	CHeaderFieldPart* part = NULL;
	TBool done = EFalse;

	TPtrC8 token;
	TInt bytesConsumed = InetProtTextUtils::ExtractNextTokenFromList(buffer, token, KCommaSpaceSep);
	part = SetNewFStringPartL(aHeader, numParam, token);		//first assignment
	++numParam;
	
	TBool equalsPresent = (token.Locate('=') != KErrNotFound);
	if ((totalBytesConsumed == bytesConsumed) && equalsPresent)
		{
		// The first token has an equals sign in it. That
		// can't be as it has to be in an Authorization scheme
		User::Leave(KErrHttpDecodeAuthorization);
		}

	RStringF basic = iStrPool.StringF(HTTP::EBasic , RHTTPSession::GetTable());

	if (token==basic.DesC())
		{
		// Get next part and store away
		bytesConsumed = InetProtTextUtils::ExtractNextTokenFromList(buffer, token, KCommaSpaceSep);
		++numParam;
		if (bytesConsumed==0)
			{
			// Need a second value if "Basic"
			User::Leave(KErrHttpDecodeAuthorization);
			}

		// Remove surrounding quotes
		TPtrC8 val;
		if (token[0] == '"')
			{
			bytesConsumed += 
				InetProtTextUtils::ExtractQuotedStringL(token,val);
			}
		else
			{
			val.Set(token);
			}
		SetNewFStringPartL(aHeader, numParam - 1, val);	
		}
	else // Get Digest parameters. Should check if it is digest
		{
		while (!done)
			{
			TPtrC8 token;
			TInt bytesConsumed = InetProtTextUtils::ExtractNextTokenFromList(buffer, token, KCommaSpaceSep);

			done = (bytesConsumed == 0);
			if (done) 
				{
				if (numParam == 0)// if we didn't find _anything_ at all...
					User::Leave(KErrHttpDecodeAuthorization);
				}
			else if (token.Length() > 0)
				{
				totalBytesConsumed += bytesConsumed;

				if (token.Locate('=') == KErrNotFound)
					{
					// Got a new part. Add it.
					++numParam;
					part = SetNewFStringPartL(aHeader, numParam - 1, token);
					}
				else
					{
					TPtrC8 paramName;
					TPtrC8 paramVal;
					bytesConsumed += GetParamNameAndValueL(token, paramName, paramVal, KErrHttpDecodeBasicAuth);
					SetNewStringParamL(*part, paramName, paramVal);
					}
				}
			}
		}
	}

void CHttpServerHeaderReader::DecodeHostL(RHeaderField& aHeader) const
	{
	// RFC2616, section 14.23
    // Host = "Host" ":" host [ ":" port ] ; Section 3.2.2
	
	// Our convention will be that a string called HTTP::EPort will be used to
	// set a parameter holding the integer port number

	// We are looking for a hostname and optional port number, 0 or more values

	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	TInt remaining = rawData.Length();
	TPtrC8 token;
	TInt tokensFound =0;
	
	remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, token, KFieldSeparator);

	if (remaining)
		{
		// Should not be more data
		}

	// Check for colon in our token
	TInt colonPos = token.Locate(':');
	InetProtTextUtils::RemoveWhiteSpace(token, InetProtTextUtils::ERemoveBoth);
	if (colonPos == KErrNotFound)
		{
		// Just a host name
		SetNewFStringPartL(aHeader, tokensFound, token);
		}
	else
		{
		//.. also has a port number. 

		TPtrC8 host(token.Left(colonPos));
		CHeaderFieldPart* part = SetNewFStringPartL(aHeader, tokensFound, host);

		if (colonPos==token.Length())
			// No port number supplied
			User::Leave(KErrHttpGeneralHeaderMissingHost);

		TPtrC8 port(token.Mid(colonPos + 1));
		TPtrC8 name(iStrPool.StringF(HTTP::EPort,RHTTPSession::GetTable()).DesC());
		SetNewFStringParamL(*part, name ,port);
		}
	}

void CHttpServerHeaderReader::DecodeExpectL(RHeaderField& aHeader) const
	{
	//	Expect       =  "Expect" ":" 1#expectation
	//
	//	expectation  =  "100-continue" | expectation-extension
	//	expectation-extension =  token [ "=" ( token | quoted-string )
	//	                         *expect-params ]
	//	expect-params =  ";" token [ "=" ( token | quoted-string ) ]

	TPtrC8 buffer;
	aHeader.RawDataL(buffer);

	TInt numParam = 0;

	TPtrC8 token;
	InetProtTextUtils::ExtractNextTokenFromList(buffer, token, KSemiSpaceSep);
	++numParam;
	
	SetNewFStringPartL(aHeader, 0, buffer);
	}

void CHttpServerHeaderReader::DecodeTEL(RHeaderField& aHeader) const
	{
	//	TE        = "TE" ":" #( t-codings )
	//	t-codings = "trailers" | ( transfer-extension [ accept-params ] )
	//	Examples of its use are:
	//	TE: deflate
	//	TE:
	//	TE: trailers, deflate;q=0.5
       
	DecodeGenericAcceptL(aHeader, KErrHttpDecodeTE);
	}
	
/*
 *	Methods from CHeaderReader
 */

void CHttpServerHeaderReader::DecodeHeaderL(RHeaderField& aHeader)
	{
	RStringF fieldStr = iStrPool.StringF(aHeader.Name());
	switch( fieldStr.Index(RHTTPSession::GetTable()) )
		{
	case HTTP::EAccept:
		DecodeGenericAcceptL(aHeader, KErrHttpDecodeAccept);
		break;
	case HTTP::EAcceptCharset:
		DecodeGenericAcceptL(aHeader, KErrHttpDecodeAcceptCharset);
		break;
	case HTTP::EAuthorization:
		DecodeAuthorizationL(aHeader);
		break;
	case HTTP::EAcceptLanguage:
		DecodeGenericAcceptL(aHeader, KErrHttpDecodeAcceptLanguage);
		break;
	case HTTP::EAcceptEncoding:
		DecodeGenericAcceptL(aHeader, KErrHttpDecodeAcceptEncoding);
		break;
	case HTTP::EHost:
		DecodeHostL(aHeader);
		break;
	case HTTP::EUserAgent:
		DecodeTokenListHeaderL(aHeader, KCodecSpace());
		break;
	case HTTP::EIfMatch:
		DecodeTokenListHeaderL(aHeader, KCommaSpaceSep());
		break;
	case HTTP::EIfNoneMatch:
		DecodeTokenListHeaderL(aHeader, KCommaSpaceSep());
		break;
	case HTTP::EIfModifiedSince:
		DecodeDateL(aHeader);
		break;
	case HTTP::EIfUnmodifiedSince:
		DecodeDateL(aHeader);
		break;
	case HTTP::EExpect:
		DecodeExpectL(aHeader);
		break;
	case HTTP::ETE:
		DecodeTEL(aHeader); 
		break;
	default:
		User::Leave(KErrNotSupported);
		}
	}
