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

#include "chttpgeneralheadercodec.h"

#include <http/rhttpsession.h>
#include <httpstringconstants.h>

#include "chttpdefaultheadercodec.h"
#include "chttpgeneralheaderwriter.h"
#include "chttpgeneralheaderreader.h"

CHttpGeneralHeaderCodec* CHttpGeneralHeaderCodec::NewL(TAny* aStringPool)
/** 
	Factory constructor.
	@internalComponent
	@param		aStringPool	The current string pool.
	@return		A pointer to a fully initialised object.
	@leave		KErrNoMemory	Not enough memory to create object.
	@leave		CHttpGeneralHeaderCodec::ConstructL
*/
	{
	RStringPool* strPool = static_cast<RStringPool*>(aStringPool);
	CHttpGeneralHeaderCodec* self = new (ELeave) CHttpGeneralHeaderCodec(*strPool);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CHttpGeneralHeaderCodec::~CHttpGeneralHeaderCodec()
/**	
	Destructor.
	@internalComponent
*/
	{
	}

CHttpGeneralHeaderCodec::CHttpGeneralHeaderCodec(RStringPool aStringPool)
: CHeaderCodecPlugin(), iStringPool(aStringPool), iStringTable(RHTTPSession::GetTable())
/**
	Constructor
	@internalComponent
	@param		aStringPool	The current string pool.
*/
	{
	}

void CHttpGeneralHeaderCodec::ConstructL()
/**
	Second phase constructor
	@internalComponent
	@leave		CHeaderCodec::ConstructL
	@leave		CHttpGeneralHeaderWriter::NewL
	@leave		CHttpGeneralHeaderReader::NewL
*/
	{
	// Do second phase construction of the base class
	CHeaderCodec::ConstructL();

	// Create the header writer and reader objects on behalf of the base class
	iWriter = CHttpGeneralHeaderWriter::NewL(iStringPool);
	iReader = CHttpGeneralHeaderReader::NewL(iStringPool);
	}

/*
 *	Methods from CHeaderCodec
 */

TBool CHttpGeneralHeaderCodec::CanEncode(RStringF aHeaderField) const
/**
	Asks if the codec can encode the specified header. The default codec can 
	encode all headers.
	@internalComponent
	@param		aHeaderField	The header to be encoded.
	@return		A value of ETrue if the codec can encode the header or a value 
				of EFalse if not.
*/
	{
	TBool encode = EFalse;
	switch( aHeaderField.Index(iStringTable) )
		{
	case HTTP::ECacheControl:
	case HTTP::EConnection:
	case HTTP::EDate:
	case HTTP::EPragma:
	case HTTP::ETransferEncoding:
	case HTTP::EContentEncoding:
	case HTTP::EContentLanguage:
	case HTTP::EContentLength:
	case HTTP::EContentLocation:
	case HTTP::EContentMD5:
	case HTTP::EContentType:
	case HTTP::EContentDisposition:
	case HTTP::EExpires:
	case HTTP::ELastModified:
	case HTTP::EUpgrade:
		{
		// Can encode these headers
		encode = ETrue;
		} break;
	default:
		// Return default value
		break;
		}
	return encode;
	}

TBool CHttpGeneralHeaderCodec::CanDecode(RStringF aHeaderField) const
/**
	Asks if the codec can decode the specified header. The default codec can 
	decode all headers.
	@internalComponent
	@param		aHeaderField	The header to be decoded.
	@return		A value of ETrue if the codec can decode the header or a value 
				of EFalse if not.
*/
	{
	return CanEncode(aHeaderField);
	}

CHeaderCodec* CHttpGeneralHeaderCodec::FindDelegateCodecL(RStringF /*aHeaderField*/) const
/**
	Asks the codec to find a delegate codec that can deal with the header field.
	The http default codec is the final codec that can be used.
	@internalComponent
	@param		aHeaderField	The header field to be handled.
	@return		A pointer to the http default codec.
	@leave		CHttpDefaultHeaderCodec::NewL
*/
	{
	return CHeaderCodecPlugin::CreateDelegateCodecL(iStringPool);
	}
