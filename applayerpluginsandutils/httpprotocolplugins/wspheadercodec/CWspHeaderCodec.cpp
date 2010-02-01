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

// User Includes
#include "cwspheadercodec.h"
#include "cwspheaderwriter.h"
#include "cwspheaderreader.h"

// System Includes
#include <wspstringconstants.h>
#include <wspparamconstants.h>
#include <wspcontenttypes.h>
#include <wspregcontenttypes.h>
#include <wspcharactersets.h>
#include <wspstdconstants.h>
#include <wsplanguages.h>
#include <wsptypeconstants.h>

CWspHeaderCodec* CWspHeaderCodec::NewL(RStringPool aStrPool, const TStringTable& aStrTable)
	{
	CWspHeaderCodec* self = new(ELeave)CWspHeaderCodec(aStrPool, aStrTable);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CWspHeaderCodec::~CWspHeaderCodec()
	{
	}

TBool CWspHeaderCodec::CanEncode(RStringF aHeaderField) const
	{
	TBool encode = EFalse;
	switch( aHeaderField.Index(iStrTable) )
		{
		// List headers in reverse order of encoding version
		// No v1.4 headers
		// v1.3
		case WSP::EEncodingVersion:
		case WSP::ECookie:
		case WSP::ETrailer:
		case WSP::ETE:
		case WSP::EExpect:
			{
			// These are v1.3 headers so if the WSP version is less then do not encode
			if( iWspVersion < EVersion1_3 )
				break;	// use defaul return of EFalse
			// Drop through...
			}

		//v1.2
		case WSP::EProfile:
		case WSP::EProfileDiff:
		case WSP::EXWapApplicationId:
			{
			// These are v1.2 headers so if the WSP version is less then do not encode
			if( iWspVersion < EVersion1_2 )
				break;	// use defaul return of EFalse
			// Drop through...
			}

		//v1.1
		case WSP::EAccept:
		case WSP::EAcceptCharset:
		case WSP::EAcceptCharsetDep:
		case WSP::EAcceptEncoding:
		case WSP::EAcceptEncodingDep:
		case WSP::EAcceptLanguage:
		case WSP::EAllow:
		case WSP::EAuthorization:
		case WSP::EContentEncoding:
		case WSP::EContentLanguage:
		case WSP::EContentLocation:
		case WSP::EContentMD5:
		case WSP::EContentRange:
		case WSP::EContentRangeDep:
		case WSP::EContentType:	
		case WSP::EDate:
		case WSP::EFrom:
		case WSP::ELastModified:
		case WSP::EPragma:
		case WSP::ERange:
		case WSP::EReferer:
		case WSP::EUpgrade:
		case WSP::EUserAgent:
		case WSP::EVia:
		case WSP::EWarning:
			{
			encode = ETrue;
			} break;
		}
	return encode;
	}

TBool CWspHeaderCodec::CanDecode(RStringF aHeaderField) const
	{
	TBool decode = EFalse;
	switch( aHeaderField.Index(iStrTable) )
		{
		case WSP::EContentType:
		case WSP::EDate:
		case WSP::ELastModified:
		case WSP::EPragma:
		case WSP::EVary:
		case WSP::EWWWAuthenticate:
		case WSP::ESetCookie:
		case WSP::EContentLocation:
		case WSP::ELocation:
		case WSP::EEncodingVersion:
		case WSP::EAcceptRanges:
		case WSP::EContentEncoding:
		case WSP::EContentLanguage:
		case WSP::EContentMD5:
		case WSP::EProxyAuthenticate:
		case WSP::EServer:
		case WSP::ERetryAfter:
		case WSP::EVia:
		case WSP::ETrailer:
		case WSP::EContentRange:
		case WSP::EContentRangeDep:
		case WSP::EXWapApplicationId:
		case WSP::EXWapInitiatorURI:
		case WSP::EPushFlag:
		case WSP::EAllow:
		case WSP::EUpgrade:
		case WSP::EWarning:
		case WSP::EProfileWarning:
		case WSP::EProfileWarningDep:
			{
			decode =  ETrue;
			} break;
		}
	return decode;
	}

CHeaderCodec* CWspHeaderCodec::FindDelegateCodecL(RStringF /*aHeaderField*/) const
	{
	// Create and return the WSP default header codec
	return CWspDefaultHeaderCodec::NewL(iStrPool);;
	}

CWspHeaderCodec::CWspHeaderCodec(RStringPool aStrPool, const TStringTable& aStrTable)
	: CHeaderCodec(), iStrPool(aStrPool), iStrTable(aStrTable), iWspVersion(EVersion1_2)
	{
	}

void CWspHeaderCodec::ConstructL()
	{
	// Add the well known parameter values string table
	iStrPool.OpenL(WSPParam::Table);
	iStrPool.OpenL(WSPContentTypes::Table);
	iStrPool.OpenL(WSPRegContentTypes::Table);
	iStrPool.OpenL(WSPCharacterSets::Table);
	iStrPool.OpenL(WSPStdConstants::Table);
	iStrPool.OpenL(WSPLanguages::Table);
	iStrPool.OpenL(WSPTypeConstants::Table);

	// First do 2nd phase construction of the base class
	CHeaderCodec::ConstructL();

	// Create the header writer and reader objects on behalf of the base class
	iWriter = CWspHeaderWriter::NewL(iStrPool, iStrTable, *this);
	iReader = CWspHeaderReader::NewL(iStrPool, iStrTable, *this);
	}

CWspHeaderCodec::TWspVersion CWspHeaderCodec::GetWspVersion() const
	{
	return iWspVersion;
	}

void CWspHeaderCodec::SetWspVersion(TWspVersion aWspVersion)
	{
	iWspVersion = aWspVersion;
	}

TInt CWspHeaderCodec::EncodeFieldName(RStringF aFieldName) const
	{
	return (STATIC_CAST(CWspHeaderWriter*, iWriter))->EncodeFieldName(aFieldName);
	}

// Implementation of Default WSP Header Codec
//-------------------------------------------------------------------------

CWspDefaultHeaderCodec* CWspDefaultHeaderCodec::NewL(RStringPool aStrPool)
	{
	CWspDefaultHeaderCodec* me = new(ELeave)CWspDefaultHeaderCodec(aStrPool);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
	return me;
	}

CWspDefaultHeaderCodec::~CWspDefaultHeaderCodec()
	{
	}

TBool CWspDefaultHeaderCodec::CanEncode(RStringF /*aHeaderField*/) const
	{
	// the default codec handles anything else that slips through, hence return True - it can handle the field.
	return ETrue;
	}

TBool CWspDefaultHeaderCodec::CanDecode(RStringF /*aHeaderField*/) const
	{
	// the default codec handles anything else that slips through, hence return True - it can handle the field.
	return ETrue;
	}

CHeaderCodec* CWspDefaultHeaderCodec::FindDelegateCodecL(RStringF /*aHeaderField*/) const
	{
	// the default codec cannot delegate, hence no delegated codec is created and return NULL
	return NULL;
	}

CWspDefaultHeaderCodec::CWspDefaultHeaderCodec(RStringPool aStrPool)
	: CHeaderCodec(), iStrPool(aStrPool)
	{
	}

void CWspDefaultHeaderCodec::ConstructL()
	{
	// First do 2nd phase construction of the base class
	CHeaderCodec::ConstructL();

	// Create the header writer and reader objects on behalf of the base class
	iWriter = CWspDefaultHdrWriter::NewL(iStrPool);
	iReader = CWspDefaultHdrReader::NewL(iStrPool);
	}
