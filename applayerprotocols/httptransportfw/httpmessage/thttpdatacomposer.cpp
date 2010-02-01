// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "thttpdatacomposer.h"

#include "mhttpbuffersupplier.h"
#include "thttpmessagepanic.h"

const TUint KCarriageReturn	= '\r';
const TUint KLineFeed		= '\n';
const TUint KSpace			= ' ';
const TUint KColon			= ':';

THttpDataComposer::THttpDataComposer(MHttpBufferSupplier& aBufferSupplier)
: iBufferSupplier(aBufferSupplier), iBuffer(TPtr8(NULL, 0))
/**
	Constructor.
	@param		aBufferSupplier	The buffer supplier.
*/
	{
	iRequiredLength = 0;
	}

void THttpDataComposer::Release()
/**
	Release the current data.
	@post		The buffer is empty.
*/
	{
	iBuffer.Zero();
	iRequiredLength = 0;
	}

void THttpDataComposer::Reset()
/**
	Reset the data composer. The data buffer is deleted.
	@post		The data buffer is deleted.
*/
	{
	iBuffer.Set(NULL, 0, 0);
	iRequiredLength = 0;
	iBufferSupplier.DeleteBuffer();
	}

void THttpDataComposer::AddTokenL(const TDesC8& aToken, TDelimiterType aDelimiter)
/**
	Adds the token followed by the specified delimiter to the data buffer. The 
	delimiter types can be Space, Colon or CRLF. The Colon delimiter includes a
	following Space character.
	@param		aToken		The token to be added to the data buffer.
	@param		aDelimiter	The type of delimiter that should follow the token.
	@panic		EHttpMessagePanicBadDelimiterType	An illegal delimiter type 
													was specified.												
*/
	{
	// Update the required length - add token and delimiter length
	iRequiredLength += aToken.Length();
	switch( aDelimiter )
		{
	case EColonSpace:
	case ECRLF:
		{
		// These delimiters have two characters.
		++iRequiredLength;
		// Drop into the Space delimiter case...
		}
	case ESpace:
		{
		// Only a single character
		++iRequiredLength;
		// Drop into the None delimiter case...
		}
	case ENone:
		// No change to required length.
		break;
	default:
		THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDelimiterType);
		break;
		};

	// Is there enough space in the buffer?
	if( iRequiredLength > iBuffer.MaxLength() )
		{
		// Re-allocate the buffer.
		iBufferSupplier.ReAllocBufferL(iRequiredLength, iBuffer);
		}

	// Append the token and delimiter
	iBuffer.Append(aToken);
	switch( aDelimiter )
		{
	case EColonSpace:
		{
		// Append the colon - drop into the Space case to add a space char
		iBuffer.Append(KColon);
		}
	case ESpace:
		{
		iBuffer.Append(KSpace);
		} break;
	case ECRLF:
		{
		// Append the CR followed by the LF.
		iBuffer.Append(KCarriageReturn);
		iBuffer.Append(KLineFeed);
		} break;
	case ENone:
		// Do nothing...
		break;
	default:
		THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDelimiterType);
		break;
		};
	}
