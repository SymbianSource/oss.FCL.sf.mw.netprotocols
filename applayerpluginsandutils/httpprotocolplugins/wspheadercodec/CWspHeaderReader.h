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

#ifndef __CWSPHEADERREADER_H__
#define __CWSPHEADERREADER_H__

// System Includes
#include <e32std.h>
#include <wspdecoder.h>
#include <http/framework/cheadercodec.h>

// User Includes
#include "cwspheadercodec.h"

// Forward declarations
class CHeaderFieldPart;
class CHeaderFieldParam;


/**
@internalAll
@since			7.0
This class is the by the WSP header codec to provide the decoding functionality required
by WSP to decode header fields from a raw binary format to a useful parsed format. The
decoding is done as stated in the WSP specifications avaialable from www.wapforum.org.
*/
//##ModelId=3C4C41AB029F
class CWspHeaderReader : public CHeaderReader
	{
public: // methods

	/**
	Standard virtual destructor.
	@since			7.0
	@pre 			Object is constructed
	@post			Object is destroyed
	*/
	//##ModelId=3C4C41AC0108 
	virtual ~CWspHeaderReader();

	/**
	Standard factory 2 phase construction.
	Error Condition: Low memory conditions
	@since			7.0
	@param			aStrPool A handle to an opened string pool with the WSP table opened
	@param			aStrTable The WSP table that contains the header field names
	@param			aCodec A reference to the parent owning codec
	@return			A pointer to the newly created object
	@pre 			None
	@post			Object is fully constructed
	@leave		KErrNoMemory
	*/
	//##ModelId=3C4C41AC00FD
	static CWspHeaderReader* NewL(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec);

	/**
	Standard factory 2 phase construction leaving a handle on the cleanup stack.
	Error Condition: Low memory conditions
	@since			7.0
	@param			aStrPool A handle to an opened string pool with the WSP table opened
	@param			aStrTable The WSP table that contains the header field names
	@param			aCodec A reference to the parent owning codec
	@return			A pointer to the newly created object
	@pre 			None
	@post			Object is fully constructed with a handle on the cleanup stack
	@leave		KErrNoMemory
	*/
	//##ModelId=3C4C41AC00F3
	static CWspHeaderReader* NewLC(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec);

	/**
	This method is used by the codec to decode any supported header field that
					passed in.
	Error Condition: Low memory conditions or invalid or corrupt data
	@since			7.0
	@param			aHeader A reference to the header field to decode
	@pre 			The header field is its raw format state
	@post			The header field is decoded and in its parsed state
	@leave		KErrNoMemory
					KErrNotSupported if the header field is not supported
					KErrCorrupt if the data is invalid or corrupt
	*/
	//##ModelId=3C4C41AC00EA
	virtual void DecodeHeaderL(RHeaderField& aHeader);

protected: // methods

	/**
	Standard default constructor.
	Error Condition: Low memory conditions
	@since			7.0
	@param			aStrPool A handle to an opened string pool with the WSP table opened
	@param			aStrTable The WSP table that contains the header field names
	@param			aCodec A reference to the parent owning codec
	@pre 			None
	@post			Object is constructed
	 */
	//##ModelId=3C4C41AC00DF
	CWspHeaderReader(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec);

private: // methods

	/** Decodes the Content-type header field
	*/
	//##ModelId=3C4C41AC00D6RS 
	void DecodeContentTypeL(RHeaderField& aHeader) const;

	/** Decodes the Date header field
	*/
	//##ModelId=3C4C41AC00D4
	void DecodeDateL(RHeaderField& aHeader) const;

	/** Decodes the Last-modified header field
	*/
	//##ModelId=3C4C41AC00CC
	void DecodeLastModifiedL(RHeaderField& aHeader) const;

	/** Decodes the Pragma header field
	*/
	//##ModelId=3C4C41AC00CA
	void DecodePragmaL(RHeaderField& aHeader) const;

	/** Decodes the Vary header field
	*/
	//##ModelId=3C4C41AC00C1
	void DecodeVaryL(RHeaderField& aHeader) const;

	/** Decodes the WWW-Authenticate header field
	*/
	//##ModelId=3C4C41AC00B8
	void DecodeWWWAuthenticateL(RHeaderField& aHeader) const;

	/** Decodes the Set-Cookie header field
	*/
	//##ModelId=3C4C41AC00B6
	void DecodeSetCookieL(RHeaderField& aHeader) const;

	/** Decodes the Content-location header field
	*/
	//##ModelId=3C4C41AC00AC
	void DecodeContentLocationL(RHeaderField& aHeader) const;

	/** Decodes the Location header field
	*/
	//##ModelId=3C4C41AC00A4
	void DecodeLocationL(RHeaderField& aHeader) const;

	/** Decodes the Encoding-version header field
	*/
	//##ModelId=3C4C41AC00A2
	void DecodeEncodingVersionL(RHeaderField& aHeader) const;

	/** Decodes the Accept-ranges header field
	*/
	//##ModelId=3C4C41AC0099
	void DecodeAcceptRangesL(RHeaderField& aHeader) const;

	/** Decodes the Content-encoding header field
	*/
	//##ModelId=3C4C41AC0090
	void DecodeContentEncodingL(RHeaderField& aHeader) const;

	/** Decodes the Content-language header field
	*/
	//##ModelId=3C4C41AC008E
	void DecodeContentLanguageL(RHeaderField& aHeader) const;

	/** Decodes the Content-MD5 header field
	*/
	//##ModelId=3C4C41AC0085
	void DecodeContentMD5L(RHeaderField& aHeader) const;

	/** Decodes the Proxy-authenticate header field
	*/
	//##ModelId=3C4C41AC007C
	void DecodeProxyAuthenticateL(RHeaderField& aHeader) const;

	/** Decodes the Server header field
	*/
	//##ModelId=3C4C41AC007A
	void DecodeServerL(RHeaderField& aHeader) const;

	/** Decodes the Retry-after header field
	*/
	//##ModelId=3C4C41AC0071
	void DecodeRetryAfterL(RHeaderField& aHeader) const;

	/** Decodes the Via header field
	*/
	//##ModelId=3C4C41AC0068
	void DecodeViaL(RHeaderField& aHeader) const;

	/** Decodes the Trailer header field
	*/
	//##ModelId=3C4C41AC0066
	void DecodeTrailerL(RHeaderField& aHeader) const;

	/** Decodes the Content-range header field
	*/
	//##ModelId=3C4C41AC005E
	void DecodeContentRangeL(RHeaderField& aHeader) const;

	/** Decodes the X-Wap-Application-Id header field
	*/
	//##ModelId=3C4C41AC005C
	void DecodeXWapApplicationIdL(RHeaderField& aHeader) const;

	/** Decodes the X-Wap-Initiator-URI header field
	*/
	//##ModelId=3C4C41AC0053 
	void DecodeXWapInitiatorURIL(RHeaderField& aHeader) const;

	/** Decodes the Push-flag header field
	*/
	//##ModelId=3C4C41AC0049
	void DecodePushFlagL(RHeaderField& aHeader) const;

	/** Decodes the Allow header field
	*/
	//##ModelId=3C4C41AC0040
	void DecodeAllowL(RHeaderField& aHeader) const;

	/** Decodes the Upgrade header field
	*/
	//##ModelId=3C4C41AC003E
	void DecodeUpgradeL(RHeaderField& aHeader) const;

	/** Decodes the Warning header field
	*/
	//##ModelId=3C4C41AC0035
	void DecodeWarningL(RHeaderField& aHeader) const;
	
	/** Decodes the Profile-Warning header field
	*/
	void DecodeProfileWarningL(RHeaderField& aHeader) const;

// utility helper methods

	/** Decodes the warn codes
	*/
	//##ModelId=3C4C41AC002C
	void DecodeGenericWarnCodeL(RHeaderField& aHeader, TWspPrimitiveDecoder& aDecoder) const;
	
	/** Checks for NULL strings, ie strings with no but data with a NULL terminator
	*/
	//##ModelId=3C4C41AB03CE
	TBool CheckForNullStringL(TPtrC8& aRawData, TInt& aBytesRead, TWspPrimitiveDecoder& aDecoder) const;

	/** Decodes version values
	*/
	//##ModelId=3C4C41AB03C3
	void DecodeGenericVersionL(RHeaderField& aHeader, TWspPrimitiveDecoder& aDecoder, TInt aIndex) const;

	/** Check the length of data is greater than a required minimum
	*/
	//##ModelId=3C4C41AB03B9
	inline void CheckLengthL(TPtrC8& aRawData, TInt aMinLength) const;

	/** Check for a NULL data part and adds a NULL descriptor part to the parsed data
	*/
	//##ModelId=3C4C41AB03AE
	TBool CheckNullDesPartL(RHeaderField& aHeader, TPtrC8& aRawData, TInt aPartIndex) const;

	/** Adds a new string part to the parsed data
	*/
	//##ModelId=3C4C41AB039C
	TInt AddNewDecoderStringPartL(RHeaderField& aHeader, TWspPrimitiveDecoder& aDecoder, TInt aIndex=0) const;

// generic encoding methods
 
	/** Decodes generic integer values
	*/
	//##ModelId=3C4C41AB039A
	void DecodeGenericIntegerValueL(RHeaderField& aHeader) const;

	/** Decodes generic new string values
	*/
	//##ModelId=3C4C41AB0390
	void DecodeGenericNewStringValueL(RHeaderField& aHeader) const;

	/** Decodes generic data values
	*/
	//##ModelId=3C4C41AB0387
	void DecodeGenericDateValueL(RHeaderField& aHeader) const;

	/** Decodes generic field names
	*/
	//##ModelId=3C4C41AB037E
	void DecodeGenericFieldNameL(RHeaderField& aHeader) const;

	/** Decodes generic string parameter types
	*/
	//##ModelId=3C4C41AB0374
	TInt DecodeGenericSingleParameterL(TPtrC8& aRawParam, CHeaderFieldPart& aHeaderFieldPart) const;

	/** Decodes well known parameter tokens
	*/
	//##ModelId=3C4C41AB0368
	void DecodeWellKnownParamTokenL(TWspPrimitiveDecoder& aDecoder, TInt& aBytesRead,
									TPtrC8& aRawParamBlock, CHeaderFieldPart& aHeaderFieldPart) const;

	/** Decodes untyped parameter types
	*/
	//##ModelId=3C4C41AB0355
	void DecodeUntypedParamL(TWspPrimitiveDecoder& aDecoder, TInt& aBytesRead,
									TPtrC8& aRawParamBlock, CHeaderFieldPart& aHeaderFieldPart) const;

	/** Decodes generic parameter tokens
	*/
	//##ModelId=3C4C41AB034A
	TInt DecodeGenericParamTokenL(TWspPrimitiveDecoder& aDecoder, const TStringTable& aStrTable,
							  THTTPHdrVal& aParamValue, RStringF& aParamDesValue) const;

	/** Decodes character-sets
	*/
	//##ModelId=3C4C41AB033F
	void GetCharacterSetFromValueL(TInt aValue, RStringF& aCharSetStr) const;

	/** Decodes generic challenge data
	*/
	//##ModelId=3C4C41AB0335
	void DecodeGenericChallengeL(RHeaderField& aHeader) const;

// generic part methods

	/** Sets a new part
	*/
	//##ModelId=3C4C41AB032B
	CHeaderFieldPart& SetNewPartL(RHeaderField& aHeader, TInt aPartIndex, THTTPHdrVal& aPartVal) const;

	/** Sets a new integer part
	*/
	//##ModelId=3C4C41AB0318
	CHeaderFieldPart& SetNewIntegerPartL(RHeaderField& aHeader, TInt aPartIndex, TInt aValue) const;
 
	/** Sets a new case insensitive string part
	*/
	//##ModelId=3C4C41AB030D
	CHeaderFieldPart& SetNewFStringPartL(RHeaderField& aHeader, TInt aPartIndex, TPtrC8 aValue) const;
 
	/** Sets a new case sensitive string part
	*/
	//##ModelId=3C4C41AB02FB
	CHeaderFieldPart& SetNewStringPartL(RHeaderField& aHeader, TInt aPartIndex, TPtrC8 aValue) const;
 
	/** Sets an existing case insensitive string part
	*/
	//##ModelId=3C4C41AB02F0
	CHeaderFieldPart& SetFStringPartL(RHeaderField& aHeader, TInt aPartIndex, RStringF aStrVal) const;

	/** Sets a new date value part
	*/
	//##ModelId=3C4C41AB02E5
	CHeaderFieldPart& SetNewDatePartL(RHeaderField& aHeader, TInt aPartIndex, TDateTime& aValue) const;

	/** Adds a new param to the supplied part
	*/
	//##ModelId=3C4C41AB02D1
	CHeaderFieldParam& SetNewParamL(CHeaderFieldPart& aHeaderPart, TPtrC8 aParamName, THTTPHdrVal aParamValue) const;

private: // attributes

	/** Handle to an opened string pool with the WSP header field names table opened
	*/
	//##ModelId=3C4C41AB02C9
	RStringPool iStrPool;

	/** Reference to the WSP header field names table
	*/
	//##ModelId=3C4C41AB02BF
	const TStringTable& iStrTable;

	/** Reference to the parent owning codec
	*/
	//##ModelId=3C4C41AB02B3
	CWspHeaderCodec& iCodec;
	};

inline void CWspHeaderReader::CheckLengthL(TPtrC8& aRawData, TInt aMinLength) const
	{
	if(aRawData.Length() < aMinLength)
		User::Leave(KErrCorrupt);
	}

/**
@internalAll
@since			7.0
This class is the by the WSP header codec to provide the decoding functionality required
by WSP to decode header fields from a raw binary format to a useful parsed format. This
decoder is the default decoder for headers that have no specific decoding pattern and
expects text-string format data as defined WSP specifications avaialable from www.wapforum.org.
*/
class CWspDefaultHdrReader : public CHeaderReader
	{
public: // methods

	/**
	Standard virtual destructor.
	@since			7.0
	@pre 			Object is constructed
	@post			Object is destroyed
	*/
	//##ModelId=3C4C41AC0108 
	virtual ~CWspDefaultHdrReader();

	/**
	Standard factory 2 phase construction.
	Error Condition: Low memory conditions
	@since			7.0
	@param			aStrPool A handle to an opened string pool
	@return			A pointer to the newly created object
	@pre 			None
	@post			Object is fully constructed
	@leave		KErrNoMemory
	*/
	static CWspDefaultHdrReader* NewL(RStringPool aStrPool);

	/**
	Standard factory 2 phase construction leaving a handle on the cleanup stack.
	Error Condition: Low memory conditions
	@since			7.0
	@param			aStrPool A handle to an opened string pool
	@return			A pointer to the newly created object
	@pre 			None
	@post			Object is fully constructed with a handle on the cleanup stack
	@leave		KErrNoMemory
	*/
	static CWspDefaultHdrReader* NewLC(RStringPool aStrPool);

	/**
	This method is used by the codec to decode any header field that is
					passed in. Assumes the header field value is a text-string.
	Error Condition: Low memory conditions or invalid or corrupt data
	@since			7.0
	@param			aHeader A reference to the header field to decode
	@pre 			The header field is its raw format state
	@post			The header field is decoded and in its parsed state
	@leave		KErrNoMemory
					KErrNotSupported if the header field is not supported
					KErrCorrupt if the data is invalid or corrupt
	*/
	virtual void DecodeHeaderL(RHeaderField& aHeader);

protected: // methods

	/**
	Standard default constructor.
	Error Condition: Low memory conditions
	@since			7.0
	@param			aStrPool A handle to an opened string pool
	@pre 			None
	@post			Object is constructed
	 */
	//##ModelId=3C4C41AC00DF
	CWspDefaultHdrReader(RStringPool aStrPool);

private: // attributes

	/** 
		Handle to an opened string pool
	*/
	RStringPool iStrPool;
	};

#endif // __CWSPHEADERREADER_H__
