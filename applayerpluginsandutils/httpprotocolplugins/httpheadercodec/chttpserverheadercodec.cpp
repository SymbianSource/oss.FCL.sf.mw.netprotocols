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

#include "chttpserverheadercodec.h"

#include <http/rhttpsession.h>
#include <httpstringconstants.h>

#include "chttpserverheaderreader.h"
#include "chttpserverheaderwriter.h"
#include "chttpgeneralheadercodec.h"

CHttpServerHeaderCodec* CHttpServerHeaderCodec::NewL(TAny* aStringPool)
	{
	RStringPool* strPool = static_cast<RStringPool*>(aStringPool);
	CHttpServerHeaderCodec* self = new (ELeave) CHttpServerHeaderCodec(*strPool);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CHttpServerHeaderCodec::~CHttpServerHeaderCodec()
	{
	}

CHttpServerHeaderCodec::CHttpServerHeaderCodec(RStringPool aStringPool)
: CHeaderCodecPlugin(), iStringPool(aStringPool)
	{
	}

void CHttpServerHeaderCodec::ConstructL()
	{
	// First do 2nd phase construction of the base class
	CHeaderCodec::ConstructL();

	// Create the header writer and reader objects on behalf of the base class
	iWriter = CHttpServerHeaderWriter::NewL(iStringPool);
	iReader = CHttpServerHeaderReader::NewL(iStringPool);
	}

/*
 *	Method from CHeaderCodec
 */

TBool CHttpServerHeaderCodec::CanEncode(RStringF aHeaderField) const
	{
	TBool encode = EFalse;
	switch( aHeaderField.Index(RHTTPSession::GetTable()) )
		{
	case HTTP::EWWWAuthenticate:
	case HTTP::EAge:
	case HTTP::EVary:
    case HTTP::EContentLanguage:
		{
		encode = ETrue;
		} break;
	default:
		// Return default value
		break;
		}
	return encode;
	}

TBool CHttpServerHeaderCodec::CanDecode(RStringF aHeaderField) const
	{
	TBool decode = EFalse;
	switch( aHeaderField.Index(RHTTPSession::GetTable()) )
		{
	case HTTP::EAccept:
	case HTTP::EAcceptCharset:
	case HTTP::EAuthorization:
	case HTTP::EAcceptLanguage:
	case HTTP::EHost:
	case HTTP::EUserAgent:
	case HTTP::EIfMatch:
	case HTTP::EIfNoneMatch:
	case HTTP::EIfModifiedSince:
	case HTTP::EIfUnmodifiedSince:
	case HTTP::EExpect:
	case HTTP::ETE:
		{
		decode = ETrue;
		} break;
	default:
		// Return default value
		break;
		}
	return decode;
	}

CHeaderCodec* CHttpServerHeaderCodec::FindDelegateCodecL(RStringF /*aHeaderField*/) const
	{
	return CHeaderCodecPlugin::CreateDelegateCodecL(iStringPool);
	}
