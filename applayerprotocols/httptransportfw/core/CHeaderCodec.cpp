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

// System includes
#include <e32std.h>

// User includes


// Class signature
#include <http/framework/cheadercodec.h>


EXPORT_C
CHeaderCodec::~CHeaderCodec()
	{
	delete iWriter;
	delete iReader;
	delete iDelegateCodec;
	}

EXPORT_C
void CHeaderCodec::EncodeHeaderL(RHeaderField& aHeader) const
	{
	RStringF fieldName = aHeader.Name();
	if (CanEncode(fieldName))
		iWriter->EncodeHeaderL(aHeader);
	else
		{
		// our reader can't deal with this header.  Identify and delegate to a different codec
		if (!iDelegateCodec)
			// try and find a delegate codec as we don't yet have one
			iDelegateCodec = FindDelegateCodecL(fieldName);
		
		if (iDelegateCodec)
			iDelegateCodec->EncodeHeaderL(aHeader);
		else
			// we couldn't find a codec that supports encoding this header
			User::Leave(KErrNotSupported);
		}
	}

EXPORT_C
void CHeaderCodec::DecodeHeaderL(RHeaderField& aHeader) const
	{
	RStringF fieldName = aHeader.Name();
	if (CanDecode(fieldName))
		iReader->DecodeHeaderL(aHeader);
	else
		{
		if (!iDelegateCodec)
			// try and find a delegate codec as we don't yet have one
			iDelegateCodec = FindDelegateCodecL(fieldName);

		if (iDelegateCodec)
			iDelegateCodec->DecodeHeaderL(aHeader);
		else
			// we couldn't find a codec that supports decoding this header
			User::Leave(KErrNotSupported);
		}
	}

EXPORT_C
CHeaderCodec::CHeaderCodec()
	{
	}

EXPORT_C
void CHeaderCodec::ConstructL()
	{
	}

EXPORT_C
CHeaderWriter::~CHeaderWriter()
	{
	}

EXPORT_C
CHeaderWriter::CHeaderWriter()
	{
	}

EXPORT_C
void CHeaderWriter::ConstructL()
	{
	}


EXPORT_C
CHeaderReader::~CHeaderReader()
	{
	}

EXPORT_C
CHeaderReader::CHeaderReader()
	{
	}

EXPORT_C
void CHeaderReader::ConstructL()
	{
	}


