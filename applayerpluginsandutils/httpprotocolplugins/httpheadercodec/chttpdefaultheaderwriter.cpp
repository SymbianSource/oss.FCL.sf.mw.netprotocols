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

#include "chttpdefaultheaderwriter.h"

#include <httperr.h>

#include "CHeaderField.h"

// "Sun, 06 Nov 1994 08:49:37 GMT"  ; RFC 822, updated by RFC 1123
_LIT(KDateFormatRfc1123,"%F%*E, %D %*N %Y %H:%T:%S GMT");

CHttpDefaultHeaderWriter* CHttpDefaultHeaderWriter::NewL(RStringPool aStringPool)
/** 
	Factory constructor.
	@internalComponent
	@param		aStringPool	The current string pool.
	@return		A pointer to a fully initialised object.
	@leave		KErrNoMemory	Not enough memory to create object.
*/
	{
	return new (ELeave) CHttpDefaultHeaderWriter(aStringPool);
	}

CHttpDefaultHeaderWriter::~CHttpDefaultHeaderWriter()
/**
	Destructor
	@internalComponent
*/
	{
	}

CHttpDefaultHeaderWriter::CHttpDefaultHeaderWriter(RStringPool aStringPool)
: CHeaderWriter(), iStringPool(aStringPool)
/**
	Constructor
	@internalComponent
	@param		aStringPool	The current string pool.
*/
	{
	}

/*
 *	Methods from CHeaderWriter
 */

void CHttpDefaultHeaderWriter::EncodeHeaderL(RHeaderField& aHeader)
/** 
	Encodes the header field value. The default behaviour for fields which are 
	unknown consists of copying the first header part into the raw data.
	@internalComponent
	@param		aHeader		The header field to be encoded.
	@todo
*/
	{
	// By default, the best is to write out the first part verbatim
	THeaderFieldPartIter iter = aHeader.PartsL();
	iter.First();
	if( iter.AtEnd() )
		User::Leave(KErrHttpEncodeDefault);

	const CHeaderFieldPart* part = iter();
	if( part == NULL )
		User::Leave(KErrHttpEncodeDefault);

	THTTPHdrVal value = part->Value();
	aHeader.BeginRawDataL();

	// Encoding depends on value type
	switch( value.Type() )
		{
	case THTTPHdrVal::KTIntVal:
		{
		// Integer - convert to descriptor form
		const TInt KNumDigitsInMaxInt = 8;
		TBuf8<KNumDigitsInMaxInt> valBuf;
		valBuf.Num(value.Int());
		aHeader.WriteRawDataL(valBuf);
		} break;
	case THTTPHdrVal::KStrFVal:
		{
		// Folded string - encoce as is.
		aHeader.WriteRawDataL(value.StrF().DesC());
		} break;
	case THTTPHdrVal::KStrVal:
		{
		// String - encoce as is.
		aHeader.WriteRawDataL(value.Str().DesC());
		} break;
	case THTTPHdrVal::KDateVal:
		{
		// Date value - convert to form given in RFC 822
		const TInt KNumCharsInRfc1123Date = 40;
		TBuf<KNumCharsInRfc1123Date> dateTimeStringRfc1123;
		TTime t(value.DateTime());
		t.FormatL(dateTimeStringRfc1123,KDateFormatRfc1123);

		// Convert from unicode
		TBuf8<KNumCharsInRfc1123Date> dateTimeString8;
		dateTimeString8.Copy(dateTimeStringRfc1123);
		aHeader.WriteRawDataL(dateTimeString8);
		} 
		break;
	default:
		// Nothing to write if no type is set. The value will be left empty.
		break;
		}
	aHeader.CommitRawData();
	}
