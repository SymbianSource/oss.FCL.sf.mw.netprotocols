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

#ifndef __CWSPHEADERCODEC_H__
#define __CWSPHEADERCODEC_H__

// System Includes
#include <e32std.h>
#include <http.h>
#include <http/framework/cheadercodec.h>


/**
@internalAll
@since			7.0
This class is the WSP specific header codec that is required to encode and decode
WSP headers between a usable form and the form required for transport. The codec
derives from CHeaderCodec interface and provided the WSP specific encoding and
decoding of header fields following the specifications stated in the WSP specs
available from http://www.wapforum.org and the RFC2616.
*/
//##ModelId=3C4C41B30305
class CWspHeaderCodec : public CHeaderCodec
	{
public: // enums
	/**
	Enumeration defining the different WSP encoding versions that are supported
	@since			7.0
	*/
	enum TWspVersion
		{
		EVersion1_1, // Version 1.1
		EVersion1_2, // Version 1.2
		EVersion1_3, // Version 1.3
		EVersion1_4  // Version 1.4
		};

public: // methods

	/**
	Standard factory 2 phase construction
	@leave			KErrNoMemory
	@since			7.0
	@param			aStrPool A handle to an open string pool with the string table 
					that contains the header field names already open
	@param			aStrTable The string table that contains the header field names
	@return			A pointer to the newly created object
	@pre 			None
	@post			Fully constructed and initialized object
	*/
	//##ModelId=3C4C41B3037D
	static CWspHeaderCodec* NewL(RStringPool aStrPool, const TStringTable& aStrTable);

	/**
	Standard virtual destructor
	@since			7.0
	@post			Object has been fully destroyed
	*/
	//##ModelId=3C4C41B30375
	virtual ~CWspHeaderCodec();
 
	/**
	This method returns whether the header field passed in can
					be encoded or not.
	@since			7.0
	@param			aHeaderField The header field to enquire about
	@return			ETrue if the header can be encoded otherwise EFalse
	*/
	//##ModelId=3C4C41B30373
	virtual TBool CanEncode(RStringF aHeaderField) const;

	/**
	This method returns whether the header field passed in can
					be decoded or not.
	@since			7.0
	@param			aHeaderField The header field to enquire about
	@return			ETrue if the header can be decoded otherwise EFalse
	*/
	//##ModelId=3C4C41B3036A
	virtual TBool CanDecode(RStringF aHeaderField) const;
 
	/**
	This method should be used to find an alternative codec for the
					header field name if this codec cannot handle the header field.
					The caller takes ownership of the returned codec.
	@since			7.0
	@param			aHeaderField The header field name string
	@return			A pointer to the newly created codec if one is found, otherwise
					NULL is returned if a codec cannot be found. The caller takes
					ownership of the returned codec.	
	*/
	//##ModelId=3C4C41B30360
	virtual CHeaderCodec* FindDelegateCodecL(RStringF aHeaderField) const;

	/**
	This method gets the current encoding version that is being
					used.
	@since			7.0
	@return			The encoding version enumeration
	*/
	//##ModelId=3C4C41B3035F
	TWspVersion GetWspVersion() const;

	/**
	This method sets the encoding version to use. The encoding
					version defaults to 1.2 if this method is not called.
	@since			7.0
	@param			aWspVersion The encoding version enumeration
	@post			The encoding version is set to the version passed in
	*/
	//##ModelId=3C4C41B30357
	void SetWspVersion(TWspVersion aWspVersion);

	/**
	This method encodes a header field name to its correct encoded
					token value. For WSP the returned value must be then be encoded
					to a short integer by flipping the first bit to a 1, this is NOT
					done in this method.
	@since			7.0
	@param			aFieldName The header field name string
	@return			The encoded token value of the header field name. KErrNotFound if
					no encoding value could be found for the header field name
	*/
	//##ModelId=3C4C41B30355
	TInt EncodeFieldName(RStringF aFieldName) const;

protected: // methods

	/**
	Standard constructor
	@since			7.0
	@param			aStrPool A handle to an opened string pool with the string table 
					that contains the header field names already open
	@param			aStrTable The string table that contains the header field names
	@pre 			None
	@post			Object is constructed
	*/
	//##ModelId=3C4C41B30342 
	CWspHeaderCodec(RStringPool aStrPool, const TStringTable& aStrTable);

	/**
	Standard 2nd phase construction
	@leave		KErrNoMemory
	@since			7.0
	@pre 			Object is constructed
	@post			Object is initialized and all the required string tables have
					been opened in the string pool
	*/
	//##ModelId=3C4C41B30341
	void ConstructL();

private: // methods

	/**
	Standard default private constructor
	@since			7.0
	@pre 			None
	@post			Object is constructed
	*/
	//##ModelId=3C4C41B3034D
	CWspHeaderCodec();

private: // attributes

	/** 
		Handle to an opened string pool with the header field names table open 
	*/
	//##ModelId=3C4C41B30339 
	RStringPool iStrPool;

	/** 
		The string table that contains the header field names 
	*/
	//##ModelId=3C4C41B3032D
	const TStringTable& iStrTable;

	/**
		The current encoding version that is to be used, defaults to 1.2 
	*/
	//##ModelId=3C4C41B3031C
	TWspVersion iWspVersion;
	};

/**
@internalAll
@since			7.0
This class is the default WSP Header Codec that is used if no codec can encode/
decode a particular header. This codec will encode/decode headers with the default
behaviour of treating the data as a text-string as defined in the WSP specifications
at www.wapforum.org.
*/
class CWspDefaultHeaderCodec : public CHeaderCodec
	{
public:

	/**
	Standard factory 2 phase construction
	@leave		KErrNoMemory
	@since			7.0
	@param			aStrPool A handle to an open string pool with the string table 
					that contains the header field names already open
	@return			A pointer to the newly created object
	@pre 			None
	@post			Fully constructed and initialized object
	*/
	static CWspDefaultHeaderCodec* NewL(RStringPool aStrPool);

	/**
	Standard virtual destructor
	@since			7.0
	@post			Object has been fully destroyed
	*/
	virtual ~CWspDefaultHeaderCodec();

	/**
	This method returns whether the header field passed in can
					be encoded or not. Always returns true because it will
					handle anything by design!
	@since			7.0
	@param			aHeaderField The header field to enquire about
	@return			ETrue if the header can be encoded otherwise EFalse. In this
					case ETrue is always returned
	*/
	virtual TBool CanEncode(RStringF aHeaderField) const;

	/**
	This method returns whether the header field passed in can
					be decoded or not. Always returns true because it will
					handle anything by design!
	@since			7.0
	@param			aHeaderField The header field to enquire about
	@return			ETrue if the header can be decoded otherwise EFalse. In this
					case ETrue is always returned
	*/
	virtual TBool CanDecode(RStringF aHeaderField) const;

	/**
	Find a delegate codec... always returns NULL, since this codec
					handles anything by design.
	@since			7.0
	@param			aHeaderField The header field name string
	@return			A pointer to the newly created codec if one is found, otherwise
					NULL is returned if a codec cannot be found. The caller takes
					ownership of the returned codec. In this case NULL is always
					returned
	*/
	virtual CHeaderCodec* FindDelegateCodecL(RStringF aHeaderField) const;

protected: // methods

	/**
	Standard constructor
	@since			7.0
	@param			aStrPool A handle to an opened string pool with the string table 
					that contains the header field names already open
	@pre 			None
	@post			Object is constructed
	*/
	CWspDefaultHeaderCodec(RStringPool aStrPool);

	/**
	Standard 2nd phase construction
	@leave		KErrNoMemory
	@since			7.0
	@pre 			Object is constructed
	@post			Object is initialized and all the required string tables have
					been opened in the string pool
	*/
	void ConstructL();

private:// methods

	/**
	Standard default private constructor
	@since			7.0
	@pre 			None
	@post			Object is constructed
	*/
	CWspDefaultHeaderCodec();

private: // attributes

	/** 
		Handle to an opened string pool 
	*/
	RStringPool iStrPool;
	};

#endif // __CWSPHEADERCODEC_H__
