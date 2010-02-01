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
 @file CHeaderCodec.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __CHEADERCODEC_H__
#define __CHEADERCODEC_H__

// System includes
#include <e32base.h>
#include <http/framework/rheaderfield.h>

// Forward declarations
class CHeaderWriter;
class CHeaderReader;


//##ModelId=3C4C18850126
class CHeaderCodec : public CBase
/**
An abstract HTTP header codec.  

Each instance of a concrete subclass of CHeaderCodec is associated with, and owned
by, a specific	CProtocolHandler.  It provides on-demand encoding/decoding of
HTTP header data, between the generic format used by clients/filters to formulate
requests and interpret responses, and the raw format used by
the transport handlers (and origin servers/gateways/etc.)

The CHeaderCodec has an associated instance of a sub-class of each of
CHeaderReader and CHeaderWriter.  It delegates the actual encoding and
decoding function to these instances.

Header codecs provide a mechanism for extensibility whereby if one codec can't
decode (or encode) a given header, it locates a codec that can, and then delegates
the task to that codec instead.
@publishedAll
@released
*/
	{
public: // methods

/** 
	Intended Usage:	Destructor - cleans up and releases resources to the system.
*/
	//##ModelId=3C4C18850194
	IMPORT_C virtual ~CHeaderCodec();

/**
	Encode the supplied header field.  This method uses the associated
	concrete CHeaderWriter object to do a conversion of the field into
	raw form from the generic internal representation.

	@param aHeader (in) A proxy for the header field to be encoded
	@leave KErrNotSupported if a codec that supports encoding this header
		   cannot be found
*/
	//##ModelId=3C4C1885018C
	IMPORT_C void EncodeHeaderL(RHeaderField& aHeader) const;

/**
	Decode the supplied header field.  This method uses the associated
	concrete CHeaderReader object to do a conversion of the field into
	the generic internal representation from the raw form.

	@param aHeader (in) A proxy for the header field to be decoded
	
	@leave KErrNotSupported if a codec that supports decoding this header
   		   cannot be found

*/
	//##ModelId=3C4C1885018A
	IMPORT_C void DecodeHeaderL(RHeaderField& aHeader) const;

/**
	Intended Usage: Concrete header codec classes must implement this method to
	indicate to the framework whether their concrete CHeaderWriter is
	capable of encoding the named header field.
	@param aHeaderField	(in) A proxy for the header field to be encoded
	@return A flag indicating ETrue if the field can be encoded.
*/
	//##ModelId=3C4C18850181
	virtual TBool CanEncode(RStringF aHeaderField) const = 0;

/**
	Intended Usage: Concrete header codec classes must implement this method to
					indicate to the framework whether their concrete CHeaderReader is
					capable of decoding the named header field.
	@param			aHeaderField		(in) A proxy for the header field to be encoded
	@return			A flag indicating ETrue if the field can be decoded.
 */
	//##ModelId=3C4C18850178
	virtual TBool CanDecode(RStringF aHeaderField) const = 0;

/**
	Intended Usage: Concrete header codecs must be implement this method if they wish to
					delegate the encoding/decoding of particular header fields to a
					different codec.  
					This would be done if the codec doesn't have the ability itself to
					do the encode/decode but can locate an alternative that does. This 
					function may leave with a Standard Symbian OS error code. eg. KErrNoMemory

					The caller takes ownership of the returned codec.
	@param aHeaderField (in) A proxy for the header field to be encoded
	@return A pointer to the new CHeaderCodec, or NULL if one couldn't be found.
	@leave KErrNoMemory, Not enough memory to create object.	
*/
	//##ModelId=3C4C18850176
	virtual CHeaderCodec* FindDelegateCodecL(RStringF aHeaderField) const = 0;

protected: // methods

/**
	Default constructor.
 */
	//##ModelId=3A914DF20273
	IMPORT_C CHeaderCodec();

/**
	Second phase construction in which any necessary allocation is done
	Implementations of this interface may leave with standard erros like KErrNoMemory if there is
	insufficient memory for allocation in the second phase.
 */
	//##ModelId=3C4C1885016E
	IMPORT_C void ConstructL();

protected: // attributes

	/** The owned header writer object that does actual encoding of header fields.
	*/
	//##ModelId=3C4C18850164
	CHeaderWriter* iWriter;

	/** The owned header reader object that does actual decoding of header fields.
	*/
	//##ModelId=3C4C1885015A
	CHeaderReader* iReader;

private: // methods

/**	
	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C1885016D
	inline virtual void Reserved1();

/**	
	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C1885016C
	inline virtual void Reserved2();

private: // attributes

	/** The most recently-obtained delegate codec
	*/
	//##ModelId=3C4C18850150
	mutable CHeaderCodec* iDelegateCodec;
	};


//##ModelId=3C4C188601D1
class CHeaderWriter : public CBase
/**
An abstract HTTP header encoder.  CHeaderWriter provides an interface used by its
owning codec to do conversion of header data from the generic internal header
representation to the raw representation used for a particular protocol/transport.
Specific sub-classes of CHeaderWriter are associated with specific protocol handlers.
@publishedAll
@released
*/
	{
public: // methods

/** 
	Intended Usage:	Destructor - cleans up and release resources to the system.
*/
	//##ModelId=3C4C188601FC
	IMPORT_C virtual ~CHeaderWriter();

/**
	Intended Usage:	Encodes the supplied header field.  This method does a conversion
					of the field into raw form from the generic internal representation.

					Implementations of this interface may leave with any of KErrNotSupported, KErrHttpEncodeDoWWWAuthenticate,
					KErrHttpEncodeAuthorization, KErrHttpEncodeDoAge, KErrHttpEncodeDoVary, KErrHttpEncodeDoContentLanguage.

					Specific header writer sub-classes must implement this method.
	@param			aHeader		(in) A proxy for the header field to be encoded
 */
	//##ModelId=3C4C188601FA
	virtual void EncodeHeaderL(RHeaderField& aHeader) = 0;

protected: // methods

/**
	Default constructor.
 */
	IMPORT_C CHeaderWriter();

/**
	Second phase construction in which any necessary allocation is done
	Implementations of this interface may leave with standard errors like KErrNoMemory.
 */
	//##ModelId=3C4C188601F9
	IMPORT_C void ConstructL();

private: // methods

/**	
	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C188601F1
	inline virtual void Reserved1();

/**	
	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C188601F0
	inline virtual void Reserved2();
	};


//##ModelId=3C4C188602FE
class CHeaderReader : public CBase
/**
An abstract HTTP header decoder.  CHeaderReader provides an interface used by its
owning codec to do conversion of header data from the raw representation used for
a particular protocol/transport to the generic internal header representation.
Specific sub-classes of CHeaderWriter are associated with specific protocol
handlers.
@publishedAll
@released
*/
	{
public: // methods

/** 
	Intended Usage:	Destructor - cleans up and release resources to the system.
*/
	//##ModelId=3C4C1886031E
	IMPORT_C virtual ~CHeaderReader();

/**
	Intended Usage:	Decodes the supplied header field.  This method does a conversion
	of the field from the generic internal representation into raw form.

	Specific header reader sub-classes must implement this method.

	Implementations of this interface may leave with any of KErrHttpDecodeAccept, KErrHttpDecodeAcceptCharset, 
	KErrHttpDecodeAcceptLanguage, KErrHttpDecodeAcceptEncoding, KErrNotSupported.

	@param aHeader (in) A proxy for the header field to be decoded
	
*/
	//##ModelId=3C4C1886031C
	virtual void DecodeHeaderL(RHeaderField& aHeader) = 0;

protected: // methods

/**
	Default constructor.
 */
	IMPORT_C CHeaderReader();

/**
	Second phase construction in which any necessary allocation is done
	Implementations of this interface may leave with standard errors like KErrNoMemory.
 */
	//##ModelId=3C4C18860315
	IMPORT_C void ConstructL();

private: // methods

/**	
	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C18860314
	inline virtual void Reserved1();

/**	
	Intended Usage:	Reserve a slot in the v-table to preserve future BC
 */
	//##ModelId=3C4C18860313
	inline virtual void Reserved2();
	};


inline void CHeaderCodec::Reserved1()
	{}
inline void CHeaderCodec::Reserved2()
	{}
inline void CHeaderWriter::Reserved1()
	{}
inline void CHeaderWriter::Reserved2()
	{}
inline void CHeaderReader::Reserved1()
	{}
inline void CHeaderReader::Reserved2()
	{}

#endif /* __CHEADERCODEC_H__ */

