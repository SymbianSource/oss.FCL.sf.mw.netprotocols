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

#ifndef __CHTTPHEADERWRITER_H__
#define __CHTTPHEADERWRITER_H__

#include <e32base.h>
#include <stringpool.h>
#include <http/framework/cheadercodec.h>


/**	CHttpHeaderWriter is used by its owning codec to do conversion of header data from
	the generic format (of parts and parameters) to the plain-text representation.
*/
class CHttpHeaderWriter : public CHeaderWriter
	{
protected: // methods

	CHttpHeaderWriter(RStringPool aStringPool);

	/** Helper to DoAcceptL(), DoAcceptCharsetL(), DoAcceptLanguageL() and DoAcceptEncodingL() for comma-separated
	    lists incorporating q-values.
	    @param aHeader The header field being encoded
	    @param aFieldType A string reference to the field type being encoded
	    @param aErrorCode An error value to use when encoding fails
	*/
	void DoGeneralAcceptHeaderL(TInt aNormalHeaderLen, RHeaderField& aHeader, RStringF aFieldType, TInt aErrorCode) const;

	/** Helper to DoConnectionL(), DoContentEncodingL(), DoTransferEncodingL(), etc., which take comma-separated
	    token lists.
	    @param aHeader The header field being encoded
	    @param aErrorCode An error value to use when encoding fails
	*/
	void DoTokenCsvListHeaderL(RHeaderField& aHeader, TInt aErrorCode) const;

	/** Helper to DoUserAgentL(), etc., which take token lists with arbitrary separators
	    @param aHeader The header field being encoded
	    @param aFieldType A string reference to the field type being encoded
	    @param aErrorCode An error value to use when encoding fails
	*/
	void DoTokenListHeaderL(RHeaderField& aHeader, const TDesC8& aSep, TInt aErrorCode) const;
	const CHeaderFieldPart* WriteRawStrFPartL(RHeaderField& aHeader, THeaderFieldPartIter& aIter, TInt aErrorCode) const;
	void EncodeGenericDateL(RHeaderField& aHeader, TInt aErrorCodeToUseOnFailure) const;
	void EncodeGenericNumberHeaderL(RHeaderField& aHeader, TInt aErrorCodeToUseOnFailure) const;

protected: // attributes

	RStringPool iStringPool;

	const TStringTable&    iStringTable;
	};

#endif	// __CHTTPHEADERWRITER_H__
