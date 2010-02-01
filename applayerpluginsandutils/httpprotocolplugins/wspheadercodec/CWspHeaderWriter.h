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

#ifndef __CWSPHEADERWRITER_H__
#define __CWSPHEADERWRITER_H__

// System Includes
#include <e32std.h>
#include <wspencoder.h>
#include <http/framework/cheadercodec.h>

// User Includes
#include "cwspheadercodec.h"

// Forward declarations
class CHeaderFieldParam;

/**
@since			7.0
This class provides the encoding functionality for encoding all of the supported
headers. The single method EncodeHeaderL() is used passing in the header field
and the particular header field is then encoded into its faw format.
*/
//##ModelId=3C4C41A600FD
class CWspHeaderWriter : public CHeaderWriter
	{
public: // methods

	/**
	Standard virtual destructor
	@since			7.0
	@pre 			Fully constructed object
	@post			Object is destroyed
	*/
	//##ModelId=3C4C41A6027C
	virtual ~CWspHeaderWriter();

	/**
	Standard factory 2 phase construction
	Error Condition: Low memory conditions
	@since			7.0
	@param			aStrPool Handle to an opened string pool with the WSP string table already opened
	@param			aStrTable The WSP string table to use for the header field names
	@param			aCodec The codec that owns this header writer
	@return			A pointer to the newly constructed class
	@pre 			None
	@post			Object is constructed
	*/
	//##ModelId=3C4C41A60272
	static CWspHeaderWriter* NewL(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec);

	/**
	Standard factory 2 phase construction leaving a handle on the cleanup stack.
	Error Condition: Low memory conditions
	@since			7.0
	@param			aStrPool Handle to an opened string pool with the WSP string table already opened
	@param			aStrTable The WSP string table to use for the header field names
	@param			aCodec The codec that owns this header writer
	@return			A pointer to the newly constructed class
	@pre 			None
	@post			Object is constructed with a handle on the cleanup stack
	*/
	//##ModelId=3C4C41A60267
	static CWspHeaderWriter* NewLC(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec);

	/**
	This method will encode the header field that is passed in into its raw
					form.
	Error Condition: Invalid or corrupt data formats and out of memeory conditions
	@since			7.0
	@param			aHeader The header field to encode
	@pre 			The header field is in its parsed state
	@post			The header field is in its raw format state
	@leave		KErrNotSupported If the header field is not supported
					KErrNoMemory Out of memory conditions
					KErrCorrupt If the data is invalid or is corrupt
	*/
	//##ModelId=3C4C41A6025E
	virtual void EncodeHeaderL(RHeaderField& aHeader);

	/**
	This method encodes the field name into its encoded token value. For WSP
					transport the encoded token must be a short integer where the top bit is
					set to 1. This method will NOT set the top bit to 1.
	@since			7.0
	@param			aFieldName The name of the field to encode
	@return			The encoded token value for the header field name. This value will always be
					between 0 and 127 inclusive or KErrNotFound if the header name does not have
					an encoded token value.
	*/
	//##ModelId=3C4C41A6025C
	TInt EncodeFieldName(RStringF aFieldName) const;

protected: // methods

	/**
	Standard default constructor.
	@since			7.0
	@param			aStrPool Handle to an opened string pool with the WSP string table already opened
	@param			aStrTable The WSP string table to use for the header field names
	@param			aCodec The codec that owns this header writer
	@pre 			None
	@post			Object is constructed
	*/
	//##ModelId=3C4C41A60252
	CWspHeaderWriter(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec);

private: // methods

	/**
	Encodes the 'Accept' header
	*/
	//##ModelId=3C4C41A60249
	void EncodeAcceptL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Accept-charset' header
	*/
	//##ModelId=3C4C41A60240
	void EncodeAcceptCharsetL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Accept-encoding' header
	*/
	//##ModelId=3C4C41A6023E
	void EncodeAcceptEncodingL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Accept-language' header
	*/
	//##ModelId=3C4C41A60235
	void EncodeAcceptLanguageL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Allow' header
	*/
	//##ModelId=3C4C41A6022C
	void EncodeAllowL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Authorization' header
	*/
	//##ModelId=3C4C41A6022A
	void EncodeAuthorizationL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Content-encoding' header
	*/
	//##ModelId=3C4C41A60222
	void EncodeContentEncodingL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Content-language' header
	*/
	//##ModelId=3C4C41A60220
	void EncodeContentLanguageL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Content-location' header
	*/
	//##ModelId=3C4C41A60217
	void EncodeContentLocationL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Content-MD5' header
	*/
	//##ModelId=3C4C41A6020E
	void EncodeContentMD5L(RHeaderField& aHeader) const;

	/**
	Encodes the 'Content-range' header
	*/
	//##ModelId=3C4C41A6020C
	void EncodeContentRangeL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Content-type' header
	*/
	//##ModelId=3C4C41A60203
	void EncodeContentTypeL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Cookie' header
	*/
	//##ModelId=3C4C41A601FA
	void EncodeCookieL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Date' header
	*/
	//##ModelId=3C4C41A601F8
	void EncodeDateL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Encoding-version' header
	*/
	//##ModelId=3C4C41A601F0
	void EncodeEncodingVersionL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Expect' header
	*/
	//##ModelId=3C4C41A601EE
	void EncodeExpectL(RHeaderField& aHeader) const;

	/**
	Encodes the 'From' header
	*/
	//##ModelId=3C4C41A601E4
	void EncodeFromL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Last-modified' header
	*/
	//##ModelId=3C4C41A601DA
	void EncodeLastModifiedL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Pragma' header
	*/
	//##ModelId=3C4C41A601D2
	void EncodePragmaL(RHeaderField& aHeader) const;
	
	/**
	Encodes the 'Profile' header
	*/
	void EncodeProfileL(RHeaderField& aHeader) const;
	
	/**
	Encodes the 'Profile-Diff' header
	*/
	void EncodeProfileDiffL(RHeaderField& aHeader) const;
 
	/**
	Encodes the 'Range' header
	*/
	//##ModelId=3C4C41A601D0
	void EncodeRangeL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Referer' header
	*/
	//##ModelId=3C4C41A601C7
	void EncodeRefererL(RHeaderField& aHeader) const;

	/**
	Encodes the 'TE' header
	*/
	//##ModelId=3C4C41A601BE
	void EncodeTEL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Trailer' header
	*/
	//##ModelId=3C4C41A601BC 
	void EncodeTrailerL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Upgrade' header
	*/
	//##ModelId=3C4C41A601B2
	void EncodeUpgradeL(RHeaderField& aHeader) const;
 
	/**
	Encodes the 'User-agent' header
	*/
	//##ModelId=3C4C41A601A9
	void EncodeUserAgentL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Via' header
	*/
	//##ModelId=3C4C41A601A0
	void EncodeViaL(RHeaderField& aHeader) const;

	/**
	Encodes the 'Warning' header
	*/
	//##ModelId=3C4C41A6019E
	void EncodeWarningL(RHeaderField& aHeader) const;

	/**
	Encodes the 'X-Wap-Application-Id' header
	*/
	//##ModelId=3C4C41A60195
	void EncodeXWapApplicationIdL(RHeaderField& aHeader) const;

// Generic encoding methods
 
	/**
	Encodes generic headers that have a date value
	*/
	//##ModelId=3C4C41A6018C
	void GenericEncodeDateL(RHeaderField& aHeader) const;

	/**
	Encodes generic headers that have a text string value
	*/
	//##ModelId=3C4C41A6018A
	void GenericEncodeTextStringL(RHeaderField& aHeader) const;

// Helper methods

	/**
	Encodes a parameter value. The method returns a heap based descriptor containing
	the encoded buffer. The caller takes ownership of the buffer.
	*/
	//##ModelId=3C4C41A60180
	void EncodeParameterL(const CHeaderFieldParam& aHeaderFieldParam, CWspHeaderEncoder& aEncoder) const;
 
	/**
	Encodes a single 'Q' value. The method returns a heap based descriptor containing
	the encoded buffer. The caller takes ownership of the buffer.
	*/
	//##ModelId=3C4C41A60177
	HBufC8* EncodeQValueL(RStringF aQValue) const;

	/**
	Encodes a single version value. The method returns the encoded version value of the
	version passed in as a parameter in a string format. The method will return 0 if
	the version should be encoded as a text-value.
	*/
	//##ModelId=3C4C41A6016E
	TUint8 EncodeVersionValueL(RStringF aVersionValue) const;

	/**
	Encodes a header name. The method returns an integer value of the length of the
	header name.
	*/
	//##ModelId=3C4C41A60164
	TInt EncodeHeaderNameL(RHeaderField& aHeader) const;

	/**
	Encodes a single character set value. The method returns the encoded charset value
	of the passed in string.
	*/
	//##ModelId=3C4C41A60150
	TInt GetCharacterSetValue(RStringF aCharSet) const;

	/**
	Checks the passed in string for any invalid characters as specified in RFC2616.
	*/
	//##ModelId=3C4C41A6014E
	TInt CheckTokenText(RStringF aTokenText) const;

	/**
	Checks the passed in string for any invalid characters as specified in RFC2616.
	*/
	//##ModelId=3C4C41A60144
	TInt CheckTextString(RStringF aTextString) const;

	/**
	Gets encoding for content-type text value with correct WSP version control.
	Returns KErrNotFound if no encoding is available
	*/
	TInt EncodeContentTypeValue(THTTPHdrVal& aContentType) const;

	TInt EncodeParameterTokenValue(RStringF aParameterName) const;


private: // attributes

	//##ModelId=3C4C41A6013C
	RStringPool iStrPool;

	//##ModelId=3C4C41A60130
	const TStringTable& iStrTable;

	//##ModelId=3C4C41A6011F
	CWspHeaderCodec& iCodec;
	};

/**
@since			7.0
This class provides the encoding functionality for headers that are supported
by the WSP Header Codec. This encoder provides default functionality and 
encodes the header field as a text-string as defined in the WSP specifications
at www.wapforum.org.
*/
class CWspDefaultHdrWriter : public CHeaderWriter
	{
public: // methods

	/**
	Standard virtual destructor
	@since			7.0
	@pre 			Fully constructed object
	@post			Object is destroyed
	*/
	virtual ~CWspDefaultHdrWriter();

	/**
	Standard factory 2 phase construction
	Error Condition: Low memory conditions
	@since			7.0
	@param			aStrPool Handle to an opened string pool with the WSP string table already opened
	@return			A pointer to the newly constructed class
	@pre 			None
	@post			Object is constructed
	*/
	static CWspDefaultHdrWriter* NewL(RStringPool aStrPool);

	/**
	Standard factory 2 phase construction leaving a handle on the cleanup stack.
	Error Condition: Low memory conditions
	@since			7.0
	@param			aStrPool Handle to an opened string pool with the WSP string table already opened
	@return			A pointer to the newly constructed class
	@pre 			None
	@post			Object is constructed with a handle on the cleanup stack
	*/
	static CWspDefaultHdrWriter* NewLC(RStringPool aStrPool);

	/**
	This method will encode the header field that is passed in into its raw
					form.
	Error Condition: Invalid or corrupt data formats and out of memeory conditions
	@since			7.0
	@param			aHeader The header field to encode
	@pre 			The header field is in its parsed state
	@post			The header field is in its raw format state
	@leave		KErrNotSupported If the header field is not supported
					KErrNoMemory Out of memory conditions
					KErrCorrupt If the data is invalid or is corrupt
	*/
	virtual void EncodeHeaderL(RHeaderField& aHeader);

protected: // methods

	/**
	Standard default constructor.
	@since			7.0
	@param			aStrPool Handle to an opened string pool with the WSP string table already opened
	@pre 			None
	@post			Object is constructed
	*/
	CWspDefaultHdrWriter(RStringPool aStrPool);

private: // attributes

	RStringPool iStrPool;
	};

#endif // __CWSPHEADERWRITER_H__
