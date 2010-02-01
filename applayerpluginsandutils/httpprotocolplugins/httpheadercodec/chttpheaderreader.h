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

#ifndef __CHTTPHEADERREADER_H__
#define __CHTTPHEADERREADER_H__

#include <e32base.h>
#include <stringpool.h>
#include <http/framework/cheadercodec.h>


class CHeaderFieldParam;

/** CHTTPHeaderReader is used by its owning codec to do conversion of header data from the
	plain-text raw representation to the generic format (of parts and parameters).
*/
class CHttpHeaderReader : public CHeaderReader
	{
protected: // methods

	CHttpHeaderReader(RStringPool aStrPool);


	/** Splits generic type headers into strings based on position of the seprarator char, given as list of separators */
	void DecodeGenericL(RHeaderField& aHeader, const TDesC8& aSeparator) const;
	
	void DecodeTokenListHeaderL(RHeaderField& aHeader, const TDesC8& aSep) const;

	/** Decodes a generic numerical header. That is a header with one part that is a number */
	void DecodeGenericNumberL(RHeaderField& aHeader) const;
    void DecodeDateL(RHeaderField& aHeader) const; 


	/** Set in the specified header field a new part containing the supplied integer data.
	    This will override any raw data or any existing part at the stated index.  Returns
	    the new part that is created.
	*/
	CHeaderFieldPart* SetNewIntegerPartL(RHeaderField& aHeader, TInt aPartIndex, TInt aValue) const;

	/** Set in the specified header field a new part containing the supplied date/time.  This
	    will override any raw data or any existing part at the stated index, Returns the new
	    part that is created.
	*/
	CHeaderFieldPart* SetNewDatePartL(RHeaderField& aHeader, TInt aPartIndex, TDateTime& date) const;

	/** Set in the specified header field a new part containing the supplied string data. This
	    will override any raw data or any existing part at the stated index, Returns the new
	    part that is created.
	*/
	CHeaderFieldPart* SetNewFStringPartL(RHeaderField& aHeader, TInt aPartIndex, TPtrC8 aValue) const;

	CHeaderFieldPart* SetNewStringPartL(RHeaderField& aHeader, TInt aPartIndex, TPtrC8 aValue) const;

	/** Helper method for SetNewIntegerPartL(), SetNewDatePartL() and SetNewFStringPartL(),
	    based on a supplied general HTTP header value. Returns the new part that is created.
	*/
	CHeaderFieldPart* SetNewPartL(RHeaderField& aHeader, TInt aPartIndex, THTTPHdrVal& aValue) const;

	/** Set in the specified header field part a new parameter with the supplied name and
	    string value.  Returns  the new parameter created.
	*/
	CHeaderFieldParam* SetNewFStringParamL(CHeaderFieldPart& aHeaderPart, TPtrC8 aParamName, TPtrC8 aParamValue) const;
	CHeaderFieldParam* SetNewStringParamL(CHeaderFieldPart& aHeaderPart, TPtrC8 aParamName, TPtrC8 aParamValue) const;
	CHeaderFieldParam* SetNewIntegerParamL(CHeaderFieldPart& aHeaderPart, TPtrC8 aParamName, TPtrC8 aParamValue) const;
	
	/** Set in the specified header field part parameter name.
	*/
    void SetParamNameAndValueL(CHeaderFieldPart& aHeaderPart, const TDesC8& aParam, TBool aQuotedValueAllowed) const;

	/** Parses name value pairs e.g. xx=yy
	*/
	TInt GetParamNameAndValueL(TPtrC8& aBuffer, TPtrC8& aName, TPtrC8& aValue, TInt aErrorCode) const;
	

protected: // attributes

	RStringPool iStrPool;

	RStringF iFieldName;

	const TStringTable& iStringTable;
	};

#endif	// __CHTTPHEADERREADER_H__
