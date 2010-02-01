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

#include "chttpclientheadercodec.h"

#include <http/rhttpsession.h>
#include <httpstringconstants.h>

#include "chttpclientheaderreader.h"
#include "chttpclientheaderwriter.h"
#include "chttpgeneralheadercodec.h"

CHttpClientHeaderCodec* CHttpClientHeaderCodec::NewL(TAny* aStringPool)
	{
	RStringPool* strPool = static_cast<RStringPool*>(aStringPool);
	CHttpClientHeaderCodec* self = new (ELeave) CHttpClientHeaderCodec(*strPool);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CHttpClientHeaderCodec::~CHttpClientHeaderCodec()
	{
	}

CHttpClientHeaderCodec::CHttpClientHeaderCodec(RStringPool aStringPool)
: CHeaderCodecPlugin(), iStringPool(aStringPool),
iStringTable(RHTTPSession::GetTable())
	{
	}

void CHttpClientHeaderCodec::ConstructL()
	{
	// Do second phase construction of the base class.
	CHeaderCodec::ConstructL();

	// Create the header writer and reader objects on behalf of the base class
	iWriter = CHttpClientHeaderWriter::NewL(iStringPool);
	iReader = CHttpClientHeaderReader::NewL(iStringPool);
	}

/*
 *	Methods from CHeaderCodec
 */

TBool CHttpClientHeaderCodec::CanEncode(RStringF aHeaderField) const
	{
	TBool encode = EFalse;
	switch( aHeaderField.Index(iStringTable) )
		{
	case HTTP::EAccept:
	case HTTP::EAcceptCharset:
	case HTTP::EAcceptLanguage:
	case HTTP::EAuthorization:
	case HTTP::EHost:
	case HTTP::EUserAgent:
	case HTTP::ECookie:
	case HTTP::EIfMatch:
	case HTTP::EIfNoneMatch:
	case HTTP::EIfModifiedSince:
	case HTTP::EIfUnmodifiedSince:
	case HTTP::ECookie2:
	case HTTP::ETE:
		{
		encode = ETrue;
		} break;
	default:
		// Return default value
		break;
		}
	return encode;
	}

TBool CHttpClientHeaderCodec::CanDecode(RStringF aHeaderField) const
	{
	TBool decode = EFalse;
	switch( aHeaderField.Index(iStringTable) )
		{
	case HTTP::ELastModified:
	case HTTP::EWWWAuthenticate:
	case HTTP::ESetCookie:
	case HTTP::ESetCookie2:
	case HTTP::EAge:
	case HTTP::EVary:
	case HTTP::EContentLanguage:
	case HTTP::EUpgrade:
		{
		decode = ETrue;
		} break;
	default:
		// Return default value
		break;
		}
	return decode;
	}

CHeaderCodec* CHttpClientHeaderCodec::FindDelegateCodecL(RStringF /*aHeaderField*/) const
	{
	return CHeaderCodecPlugin::CreateDelegateCodecL(iStringPool);
	}
