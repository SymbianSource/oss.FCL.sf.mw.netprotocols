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

/**
 @file RHeaderField.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __RHEADERFIELD_H__
#define __RHEADERFIELD_H__

// System includes
#include <e32base.h>
#include <http/thttphdrval.h>
#include <http/rhttppropertyset.h>
#include <http/rhttpheaders.h>

// Forward declarations
class CHeaderField;
class THeaderFieldPartIter;


/**
The default chunk size for Raw data should be set to the average length of a header field when encoded in
plaintext.
@publishedAll
@released
*/
const TInt KDefaultRawChunkSize= 32;


//##ModelId=3B1E52A50396
class RHeaderField
/**
An proxy object for a single field in an HTTP header.  The class is used by
implementations of CHeaderCodec in order to manipulate the header at the part and
parameter level, or to obtain raw data, during a header encode/decode operation.
@publishedAll
@released
*/
	{
public: // methods

	//##ModelId=3BA6004002BC
	RHeaderField(CHeaderField& aHeaderField);

	IMPORT_C
	//##ModelId=3B1E52A503C0
	RStringF Name() const;

	IMPORT_C
	//##ModelId=3B1E52A503BF
	THeaderFieldPartIter PartsL();

	IMPORT_C
	//##ModelId=3B1E52A503B9
	void RawDataL(TPtrC8& aRawData);

	IMPORT_C
	//##ModelId=3BA6004002B2
	RHTTPHeaders Collection();

	IMPORT_C
	//##ModelId=3B1E52A503B6
	void SetPartL(CHeaderFieldPart* aPart, TInt aIndex);

	IMPORT_C
	//##ModelId=3B1E52A503B4
	void BeginRawDataL(TInt aChunkSize = KDefaultRawChunkSize);

	IMPORT_C
	//##ModelId=3B1E52A503AB
	void WriteRawDataL(const TDesC8& aData);

	IMPORT_C
	//##ModelId=3B1E52A503AD
	void WriteRawDataL(TChar aData);

	IMPORT_C
	//##ModelId=3B1E52A503AA
	void CommitRawData();

private: // attributes

	/** The internal representation of this header field
	*/
	//##ModelId=3B1E52A503A2
	CHeaderField& iHeaderField;
};

inline RHeaderField::RHeaderField(CHeaderField& aHeaderField)
	: iHeaderField(aHeaderField)
	{
	}

#endif // __CHEADERFIELD_H__
