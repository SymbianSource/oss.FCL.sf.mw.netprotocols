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
#include <http/framework/cheadercodec.h>

// Local includes
#include "CHeaders.h"
#include "CHeaderField.h"

// Class signature
#include <http/framework/rheaderfield.h>
	
/**
Obtain the name of the header field, e.g. Accept
@return
	The header field name as a string
*/
EXPORT_C RStringF RHeaderField::Name() const
	{
	return iHeaderField.Name();
	}

/** Access the header value parts via an iterator
	@return			An iterator constructed to point at the first part
	@leave KErrNotSupported - if decoding when counting parts fails
*/
EXPORT_C 
THeaderFieldPartIter RHeaderField::PartsL()
	{
	return iHeaderField.PartsL();
	}

/** Get the header field's raw data representation.  This will flip the
	internal state of the header, if it's not already in Raw form.
	Requires a Codec to be set. May leave due to decode failures with an 
	error code from <httperr.h>
	@param			aRawData	(inout) An 8-bit pointer descriptor that is set during
										the call to point at the data buffer
										representing the header in its raw form.
	@post			The header field is in either its 'raw' state (following a
					successful conversion) or in a 'corrupt' state if decoding failed.
	@leave KErrNotSupported - if decoding when counting parts fails
*/
EXPORT_C 
void RHeaderField::RawDataL(TPtrC8& aRawData)
	{
	iHeaderField.RawDataL(aRawData);
	}
	
/** Get a handle to the headers collection that this field belongs to
	@return			The header collection handle.
*/
EXPORT_C 
RHTTPHeaders RHeaderField::Collection()
	{
	return (iHeaderField.iOwner)->Handle();
	}

/**
	Set a part in the header. It replaces any existing part at the
	specified index.  Note this is for use only by the header codec,
	when converting Raw data into parsed header parts.
@param aPart
	(in) The part to be added.  The header takes ownership of
	the new part.
@param aIndex
	(in) The index within the header of field of the part.
*/
EXPORT_C void RHeaderField::SetPartL(CHeaderFieldPart* aPart, TInt aIndex)
	{
	iHeaderField.SetPartL(aPart, aIndex);
	}

/**	When the header codec starts converting parsed header data into the
	raw form, it must first call this method to indicate that the raw
	data is being generated.  Memory allocations are done in chunks set
	at the specified size.
	Note this method is for use only by the header codec. 
	@param			aChunkSize	(in) (optional) The size to be used for raw data chunk
												allocation.
	@pre			The header field is in the 'parsed' state
	@post			The header field is still in the 'parsed' state
*/
EXPORT_C
void RHeaderField::BeginRawDataL(TInt aChunkSize)
	{
	iHeaderField.BeginRawDataL(aChunkSize);
	}

/** Add more Raw data, supplied as a descriptor.  The data is appended
	to the existing contents of the field's internal raw data buffer,
	which is extended as necessary.
	Note this method is for use only by the header codec.
	@param			aData		(in) The data to append to the buffer.
	@pre			The header field is in the 'parsed' state
	@post			The header field is still in the 'parsed' state
*/
EXPORT_C
void RHeaderField::WriteRawDataL(const TDesC8& aData)
	{
	iHeaderField.WriteRawDataL(aData);
	}

/**
	Add more Raw data, supplied as a single character.  The data is
	appended to the existing contents of the field's internal raw data
	buffer, which is extended as necessary.	
	Note this method is for use only by the header codec.
	@param aData
		(in) The character data to append to the buffer.
	@pre			The header field is in the 'parsed' state
	@post			The header field is still in the 'parsed' state
*/
EXPORT_C void RHeaderField::WriteRawDataL(TChar aData)
	{
	iHeaderField.WriteRawDataL(aData);
	}

/** Commits the Raw data.  The parsed header parts and parameters are
	removed, leaving the field in a the raw data form.
	Note this method is for use only by the header codec.
	@pre			The header field is in the 'parsed' state
	@post			The header field is in the 'raw' state
*/
EXPORT_C
void RHeaderField::CommitRawData()
	{
	iHeaderField.CommitRawData();
	}
