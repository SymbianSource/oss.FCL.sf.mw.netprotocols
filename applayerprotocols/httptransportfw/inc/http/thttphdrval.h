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
 @file
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__THTTPHDRVAL_H__
#define	__THTTPHDRVAL_H__

// System includes
#include <e32std.h>
#include <stringpool.h>


//##ModelId=3C4C187E027A
class THTTPHdrVal
/** 
A representation of a value found in an HTTP header field. Three
fundamental types are used in HTTP - integers, strings (which may
be free text, 'enumeration values', URIs, etc. or date/times.

The class allows any of the three values to be held and the current
value (as well as its type) to be determined.

THTTPHdrVal instances are used to assign values to header fields, their
parts and their associated parameters.
@publishedAll
@released
@see RHTTPHeaders
*/
	{
public:
	/**
		@enum THTTPValType
	 */
	typedef enum {
	KTIntVal = 0x00,  /**< the value is an integer*/
	KStrVal  = 0x01,  /**< the value is a case-sensitive string*/
	KDateVal = 0x02,   /**< the value is a date/time*/
	KStrFVal = 0x03,   /**< the value is a case-insensitive string*/
	KNoType =  0x04    /**< no value has yet been set*/
	} THTTPValType;

public: // classes

	// A converter for 'q' parameter values; apparently real numbers in the HTTP RFC but here passed as fixed point
//##ModelId=3B1E66F2024E
	class TQConv
		{
	public:
		// Construct from a real number representation of 'q'
	//##ModelId=3B1E66F20282
		TQConv(TReal aQ);

		// Construct from a fixed-point representation of 'q'
	//##ModelId=3B1E66F2028C
		TQConv(TInt aQ);

		// Integer cast operator to get the fixed-point representation
	//##ModelId=3B1E66F20281
		operator TInt() const;

		// TReal cast operator to get the real number representation
	//##ModelId=3B1E66F20280
		operator TReal() const;
	private:
		// The real number q-value
	//##ModelId=3B1E66F2027A
		TReal iQ;
		// The integer fixed-point q-value representation
	//##ModelId=3B1E66F2026E
		TInt iFQ;
		};

public:
	/** Default constructor	
		Used when constructing an empty header value that is to be filled in by
		a call to a function. e.g. RHTTPHeaders::GetField
	*/
	IMPORT_C THTTPHdrVal();

	/** Constructor for an integer field value
		@param aIntVal The integer value
	*/
	IMPORT_C THTTPHdrVal(TInt aIntVal);

	/** Constructor for an string value
		@param aStrVal The string value
	*/
	IMPORT_C THTTPHdrVal(RStringF aStrVal);
	/** Constructor for an string value
		@param aStrVal The string value
	*/
	IMPORT_C THTTPHdrVal(RString aStrVal);

	/** Constructor for an date/time value
		@param aDateVal The date/time value
	*/
	IMPORT_C THTTPHdrVal(TDateTime aDateVal);

	/** Obtain a copy of this header value.  This is critical when the header contains a string, since
		a new string reference must be created
		@return The copied header value
	*/
	//##ModelId=3C4C187E02F3
	IMPORT_C THTTPHdrVal Copy() const;

	/** Determine the type of this header field value
		@return A THTTPValType enumeration describing the value's type
	*/
	//##ModelId=3C4C187E02F2
	IMPORT_C THTTPValType Type() const;

	/** Obtain the (integer) value held within
		Panics if the value is of the wring type
		@pre Requires that the object was previously set to hold a TInt
		@return An integer value
	*/
	//##ModelId=3C4C187E02EA
	IMPORT_C TInt Int() const;

	/** Cast operator to obtain the HTTP value as an integer.  
		Note this doesn't convert from a different form to integer!
		Panics if the value is of the wrong type
		@pre Requires that the object was previously set to hold a TInt
	*/
	//##ModelId=3C4C187E02E9
	inline operator TInt() const;

    /** Obtain the (string) value held within
		Panics if the value is of the wring type
		@pre Requires that the object was previously set to hold a string
		@return A string
	*/
	//##ModelId=3C4C187E02E8
	IMPORT_C RStringF StrF() const;
	//##ModelId=3C4C187E02E0
	IMPORT_C RString Str() const;

	/** Cast operator to obtain the HTTP value as a string.  
		Note this doesn't convert from a different form to string!
		Panics if the value is of the wring type
		@pre Requires that the object was previously set to hold a string
	*/
	//##ModelId=3C4C187E02DF
	inline operator RStringF() const;
	/** Cast operator to obtain the HTTP value as a string.  
		Note this doesn't convert from a different form to string!
		Panics if the value is of the wring type
		@pre Requires that the object was previously set to hold a string
	*/
	//##ModelId=3C4C187E02DE
	inline operator RString() const;

	/** Obtain the (date/time) value held within.
		Panics if the value is of the wrong type
		@pre Requires that the object was previously set to hold a TDateTime
		@return An date/time value
	*/
	//##ModelId=3C4C187E02D6
	IMPORT_C TDateTime DateTime() const;

	/** Cast operator to obtain the HTTP value as an date/time.  
		Note this doesn't convert from a different form to date!
		Panics if the value is of the wring type
		@pre Requires that the object was previously set to hold a TDateTime
	*/
	//##ModelId=3C4C187E02D5
	inline operator TDateTime() const;

	/** Set the header value to be the supplied integer. 
		@warning Any previous value, or its type, are lost.
		@param aIntVal The integer value
	*/
	//##ModelId=3C4C187E02CC
	IMPORT_C void SetInt(TInt aIntVal);

	/** Set the header value to be the supplied string. 
		@warning Any previous value, or its type, are lost.
		@param aStrVal The string value
	*/
	//##ModelId=3C4C187E02CA
	IMPORT_C void SetStrF(RStringF aStrVal);
	/** Set the header value to be the supplied string. Any previous
		value, or its type, are lost.
		@param aStrVal The string value */
	//##ModelId=3C4C187E02C2
	IMPORT_C void SetStr(RString aStrVal);

	/** Set the header value to be the supplied date/time. Any previous value, or its type, are lost.
		@param aDateVal The date/time value
	*/
	//##ModelId=3C4C187E02C0
	IMPORT_C void SetDateTime(TDateTime aDateVal);

	/** Comparison operator.  Does a comparison based on the current type.
		@param aVal The header value to compare. */
	//##ModelId=3C4C187E02B8
	IMPORT_C TBool operator==(THTTPHdrVal aVal) const;

	/** Comparison operator.  Does a comparison based on the current type.
		@param aVal The string to compare. */
	//##ModelId=3C4C187E02B6
	IMPORT_C TBool operator!=(THTTPHdrVal aVal) const;

private:
	/** 
		The current type
	*/
	//##ModelId=3C4C187E02AC
	THTTPValType iType;

	/** 
		The value 
	*/
	//##ModelId=3C4C187E02A4
	TInt iVal;
	//##ModelId=3C4C187E029A
	TInt iMoreSpace;
	};

inline THTTPHdrVal::TQConv::TQConv(TReal aQ)
	: iQ(aQ), iFQ((TInt)(iQ*1000))
	{
	}

inline THTTPHdrVal::TQConv::TQConv(TInt aFQ)
	: iQ((TReal)(aFQ/1000.)), iFQ(aFQ)
	{
	}

inline THTTPHdrVal::TQConv::operator TInt() const
	{
	return iFQ;
	}

inline THTTPHdrVal::TQConv::operator TReal() const
	{
	return iQ;
	}

inline THTTPHdrVal::operator TInt() const
	{
	return Int();
	}

inline THTTPHdrVal::operator RStringF() const
	{
	return StrF();
	}

inline THTTPHdrVal::operator RString() const
	{
	return Str();
	}

inline THTTPHdrVal::operator TDateTime() const
	{
	return DateTime();
	}



#endif // __THTTPHDRVAL_H__
