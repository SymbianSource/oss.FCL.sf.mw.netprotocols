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

#include "chttpdefaultheaderreader.h"
#include "CHeaderField.h"
#include <inetprottextutils.h>


_LIT8(KNewLine, "\n");

CHttpDefaultHeaderReader* CHttpDefaultHeaderReader::NewL(RStringPool aStringPool)
/** 
	Factory constructor.
	@internalComponent
	@param		aStringPool	The current string pool.
	@return		A pointer to a fully initialised object.
	@leave		KErrNoMemory	Not enough memory to create object.
*/
	{
	return new (ELeave) CHttpDefaultHeaderReader(aStringPool);
	}

CHttpDefaultHeaderReader::~CHttpDefaultHeaderReader()
/**
	Destructor
	@internalComponent
*/
	{
	}

CHttpDefaultHeaderReader::CHttpDefaultHeaderReader(RStringPool aStringPool)
: CHeaderReader(), iStringPool(aStringPool)
/**
	Constructor
	@internalComponent
	@param		aStringPool	The current string pool.
*/
	{
	}

/*
 *	Methods from CHeaderReader
 */

void CHttpDefaultHeaderReader::DecodeHeaderL(RHeaderField& aHeader)
/** 
	Decodes the raw header field value. The default behaviour for fields which 
	are unknown consists of copying the raw data into the header fields first 
	part as a string.
	@internalComponent
	@param		aHeader		The header field to decode.
	@todo
*/
	{
	// Get the over-the-air encoded data and trim white space from it
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	TBool moreParts = ETrue;
	TPtrC8 currentPartData(KNullDesC8);
	TInt remaining = rawData.Length();
	TInt index =0;	
		
	do
		{
		// Search for '\n' separator.
		remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, currentPartData, KNewLine);
		if (remaining <= 0)
			{
			moreParts =  EFalse;
			}		
					
		// create and set the new header part at position zero, using the OTA data without modification
		RStringF partStr = iStringPool.OpenFStringL(currentPartData);
		CleanupClosePushL(partStr);
		CHeaderFieldPart* part = CHeaderFieldPart::NewL(partStr);
		CleanupStack::PushL(part);
		aHeader.SetPartL(part,index);
		CleanupStack::Pop(part);
		CleanupStack::PopAndDestroy(&partStr);
		++index;
		} while (moreParts);
	}
