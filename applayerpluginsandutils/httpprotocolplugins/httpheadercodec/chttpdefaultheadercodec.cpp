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

#include "chttpdefaultheadercodec.h"

#include "chttpdefaultheaderwriter.h"
#include "chttpdefaultheaderreader.h"

CHttpDefaultHeaderCodec* CHttpDefaultHeaderCodec::NewL(TAny* aStringPool)
/** 
	Factory constructor.
	@internalComponent
	@param		aStringPool	The current string pool.
	@return		A pointer to a fully initialised object.
	@leave		KErrNoMemory	Not enough memory to create object.
	@leave		CHttpDefaultHeaderCodec::ConstructL
*/
	{
	RStringPool* strPool = static_cast<RStringPool*>(aStringPool);
	CHttpDefaultHeaderCodec* self = new (ELeave) CHttpDefaultHeaderCodec(*strPool);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CHttpDefaultHeaderCodec::~CHttpDefaultHeaderCodec()
/**	
	Destructor.
	@internalComponent
*/
	{
	}

CHttpDefaultHeaderCodec::CHttpDefaultHeaderCodec(RStringPool aStringPool)
: CHeaderCodecPlugin(), iStringPool(aStringPool)
/**
	Constructor
	@internalComponent
	@param		aStringPool	The current string pool.
*/
	{
	}

void CHttpDefaultHeaderCodec::ConstructL()
/**
	Second phase constructor
	@internalComponent
	@leave		CHeaderCodec::ConstructL
	@leave		CHttpDefaultHeaderWriter::NewL
	@leave		CHttpDefaultHeaderReader::NewL
*/
	{
	// Do second phase construction of the base class
	CHeaderCodec::ConstructL();

	// Create the header writer and reader objects on behalf of the base class.
	iWriter = CHttpDefaultHeaderWriter::NewL(iStringPool);
	iReader = CHttpDefaultHeaderReader::NewL(iStringPool);
	}

/*
 *	Methods from CHeaderCodec
 */

TBool CHttpDefaultHeaderCodec::CanEncode(RStringF /*aHeaderField*/) const
/**
	Asks if the codec can encode the specified header. The default codec can 
	encode all headers.
	@internalComponent
	@param		aHeaderField	The header to be encoded.
	@return		A value of ETrue as all headers can be encoded.
*/
	{
	return ETrue;
	}

TBool CHttpDefaultHeaderCodec::CanDecode(RStringF /*aHeaderField*/) const
/**
	Asks if the codec can decode the specified header. The default codec can 
	decode all headers.
	@internalComponent
	@param		aHeaderField	The header to be decoded.
	@return		A value of ETrue as all headers can be decoded.
*/
	{
	return ETrue;
	}

CHeaderCodec* CHttpDefaultHeaderCodec::FindDelegateCodecL(RStringF /*aHeaderField*/) const
/**
	Asks the codec to find a delegate codec that can deal with the header field.
	The default codec is the end of the line - there are no other codecs.
	@internalComponent
	@param		aHeaderField	The header field to be handled.
	@return		A value of NULL as there are no other codecs.
*/
	{
	return NULL;
	}
