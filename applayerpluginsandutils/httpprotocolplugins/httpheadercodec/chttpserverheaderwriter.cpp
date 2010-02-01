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

#include "chttpserverheaderwriter.h"

#include <http/rhttpsession.h>
#include <httpstringconstants.h>
#include <httperr.h>
#include <inetprottextutils.h>

#include "CHeaderField.h"

_LIT8(KCommaSpaceSep,", ");

CHttpServerHeaderWriter* CHttpServerHeaderWriter::NewL(RStringPool aStringPool)
	{
	return new (ELeave) CHttpServerHeaderWriter(aStringPool);
	}

CHttpServerHeaderWriter::~CHttpServerHeaderWriter()
	{
	}

CHttpServerHeaderWriter::CHttpServerHeaderWriter(RStringPool aStringPool)
: CHttpHeaderWriter(aStringPool)
	{
	}

// Encode WWWAuth
void CHttpServerHeaderWriter::EncodeWWWAuthenticateL(RHeaderField& aHeader) const
	{
	// RFC2616, section 14.47 WWW-Authenticate
	// RFC2617, 'HTTP Authentication: Basic and Digest Access Authentication'
	//
	// WWW-Authenticate  = "WWW-Authenticate" ":" 1#challenge
	// challenge   = auth-scheme 1*SP 1#auth-param
	// auth-scheme    = token
	// auth-param     = token "=" ( token | quoted-string )

	aHeader.BeginRawDataL();
	THeaderFieldPartIter iter = aHeader.PartsL();
	for (iter.First(); !iter.AtEnd();)
		{
		const CHeaderFieldPart* part = iter();
		if (part == NULL)
			User::Leave(KErrHttpEncodeDoWWWAuthenticate);

		THTTPHdrVal ptVal = part->Value();
		switch (ptVal.Type())
			{
		case THTTPHdrVal::KStrFVal:
			aHeader.WriteRawDataL(ptVal.StrF().DesC());
			break;

		case THTTPHdrVal::KStrVal:
			aHeader.WriteRawDataL(ptVal.Str().DesC());
			break;
			
		default:
			User::Leave(KErrHttpEncodeAuthorization);
			}

		// Now output the params
		THeaderFieldParamIter paramIter = part->Parameters();
		for (paramIter.First(); !paramIter.AtEnd();)
			{
			_LIT8(equalsQuote, "=\"");
			aHeader.WriteRawDataL(' ');
			aHeader.WriteRawDataL(paramIter()->Name().DesC());
			aHeader.WriteRawDataL(equalsQuote);
			THTTPHdrVal paramVal = paramIter()->Value();
			switch (paramVal.Type())
				{
			case THTTPHdrVal::KStrFVal:
				aHeader.WriteRawDataL(paramVal.StrF().DesC());
				break;
				
			case THTTPHdrVal::KStrVal:
				aHeader.WriteRawDataL(paramVal.Str().DesC());
				break;
				
			default:
				User::Leave(KErrHttpEncodeAuthorization);
				}
			aHeader.WriteRawDataL('"');
			
			// If this isn't the last param, add a comma seperator
			++paramIter;
			if (!paramIter.AtEnd())
				aHeader.WriteRawDataL(KCommaSpaceSep);
			}

		// If this isn't the last part, add a comma seperator
		++iter;
		if (!iter.AtEnd())
			aHeader.WriteRawDataL(KCommaSpaceSep);
		}
	aHeader.CommitRawData();

	}

/*
 *	Methods from CHeaderWriter
 */

void CHttpServerHeaderWriter::EncodeHeaderL(RHeaderField& aHeader)
	{

	RStringF fieldStr = iStringPool.StringF(aHeader.Name());
	switch( fieldStr.Index(RHTTPSession::GetTable()) )
		{
	case HTTP::EWWWAuthenticate:
		EncodeWWWAuthenticateL(aHeader);
		break;
	case HTTP::EAge:
		EncodeGenericNumberHeaderL(aHeader, KErrHttpEncodeDoAge);
		break;
	case HTTP::EVary:
		DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeDoVary);
		break;
	case HTTP::EContentLanguage:
		DoTokenCsvListHeaderL(aHeader, KErrHttpEncodeDoContentLanguage);
		break;
	default:
		User::Leave(KErrNotSupported);
		}
	}
