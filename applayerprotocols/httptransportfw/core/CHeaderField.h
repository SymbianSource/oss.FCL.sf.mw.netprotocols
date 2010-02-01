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
// Declaration of CHeaderField, CHeaderFieldPart, THeaderFieldPartIter, CHeaderFieldParam,
// and THeaderFieldParamIter classes.
// 
//

#ifndef __CHEADERFIELD_H__
#define __CHEADERFIELD_H__

#include <e32base.h>
#include <http/thttphdrval.h>
#include <http/rhttppropertyset.h>
#include <http/rhttpheaders.h>



// The default chunk size for Raw data should be set to the average length of a header field when encoded in
// plaintext
const TInt KHttpDefaultRawChunkSize= 32;


class CHeaders;
class CHeaderFieldPart;
class THeaderFieldPartIter;
class CHeaderFieldParam;
class THeaderFieldParamIter;
class CHeaderCodec;
class RHTTPPropertySet;


/** Generalised internal representation of a single HTTP header field.  The field's
	value may be contained as raw data as received by HTTP, or
	in a object-oriented parsed form, which is accessible to the client via
	the class API.

	Each field has a type and a (potentially complex) value.  The type must
	be chosen at construction time, and remains the same during the
	header's lifetime.

	Simple headers are represented using a single 'part'.  Several factory
	methods are provided for each type the part can take; e.g.
	
	@verbatim
	Max-Forwards: 3							-- a single integer value
	Location: http://ayg.com/				-- a single string token value
	Expires: Mon, 15 Jan 2001 10:30:25 GMT	-- a single date/time value
	@endverbatim
	
	More complex headers are possible (see the RFC2116 for many more examples)
	which are built up from several parts; e.g.
	
	@verbatim
	Accept: text/wml, application/vnd.wap.wmlc	-- a header with two string token parts
	Allow: GET, POST, HEAD						-- a header with three string token parts
	@endverbatim
	
	Multiple parts are supported by the CHeaderFieldPart class.

	Furthermore, some headers allow parameters to be supplied in the form of attribute-value
	pairs.   These may be present on any part of the header, according to the syntax defined
	in the RFC; e.g.
	
	@verbatim
	Accept-Charset: utf-8; q=0.8, us-ascii; q=0.2
	Cache-Control: maxage=6000
	@endverbatim
	
	Parameters are supported by the CHeaderFieldParam class.	

    The classes CHeaderField, CHeaderFieldPart and CHeaderFieldParam are internal header representations
	and will only be manipulated by protocol handlers and filters.  Only the RHTTPHeaders and THTTPHdrVal
	classes are intended for use by the Client.
	@see RHTTPHeaders
	@see THTTPHdrVal
	@see CHeaderFieldPart
	@see CHeaderFieldParam
	*/

//##ModelId=3C4C186D030C
class CHeaderField : public CBase
	{
friend class THeaderFieldPartIter;
friend class RHeaderField;
friend class THttpHeaderElementIterator;
friend class THttpHeaderIterator;
friend class CHttpClientTransaction;
public:
	//
	// Getters
	//

	/** Obtain the name of the header field, e.g. Accept
		@return The header field name as a string token
	*/
	//##ModelId=3C4C186E008D
	inline RStringF Name() const;

	/** Access the header value parts via an iterator
		@return An iterator constructed to point at the first part
		@leave KErrNoMemory - insufficient free memory for object creation when decoding parts
		@leave KErrNotSupported - if decoding when counting parts fails
	*/
	//##ModelId=3C4C186E008C
	THeaderFieldPartIter PartsL();

	/** Get the Raw header representation.  This will flip the internal state of the header,
		if it's not already in Raw form. Requires a Codec to be set.
		@param The 8-bit data buffer representing the header in Raw form
		@leave KErrNoMemory - insufficient free memory for object creation when decoding parts
		@leave KErrNotSupported - if decoding when counting parts fails
	*/
	IMPORT_C
	//##ModelId=3C4C186E0082
	void RawDataL(TPtrC8& aRawData);

	//
	// Setters
	//

	/** Set a part in the header. It replaces any existing parts at the specified index.
		Note this is for use only by the header codec, when converting Raw data into parsed header parts from parsed Raw data.
		This function may leave with any of the system wide error codes, if the attempt to append this part to the internal list
		fails.
		@param aPart The part to be added.  The header takes ownership of the new part.
		@param aIndex The index of the part you are replacing
	*/
	//##ModelId=3C4C186E006F
	void SetPartL(CHeaderFieldPart* aPart, TInt aIndex);

	/** Start setting Raw data in the header.  Leaves if initial Raw buffer allocation fails. The
		optional parameter specifies the size to be used for Raw data chunk allocation.
		Note this is for use only by the header codec. 
		@leave KErrNoMemory - insufficient free memory buffer allocation
	*/
	//##ModelId=3C4C186E0066
	void BeginRawDataL(TInt aChunkSize = KHttpDefaultRawChunkSize);

	/** Add more Raw data, supplied as a descriptor.  The buffer is extended as necessary.
		Leaves if buffer allocation fails.
		Note this is for use only by the header codec. 
		@leave KErrNoMemory - insufficient free memory buffer allocation
	*/
	//##ModelId=3C4C186E005A
	void WriteRawDataL(const TDesC8& aData);

	/** Add more Raw data, supplied as a single character.  The buffer is extended as necessary.
		Leaves if buffer allocation fails.
		Note this is for use only by the header codec. 
		@leave KErrNoMemory - insufficient free memory buffer allocation
	*/
	//##ModelId=3C4C186E0064
	void WriteRawDataL(TChar aData);

	/** Commit the Raw data.  The parsed header parts and parameters are removed.
		Note this is for use only by the header codec.
	*/
	//##ModelId=3C4C186E0051
	void CommitRawData();



	/** Obtain the number of parts in this header
		@return The number, as an integer
		@leave KErrNoMemory - insufficient free memory for object creation when decoding parts
		@leave KErrNotSupported - if decoding when counting parts fails
	*/
	//##ModelId=3C4C186E0050
	TInt NumPartsL();


	/** Create a new header field that has no value yet.
		@param aHeaderFieldName The header field type.
		@param aOwner The header fields collection that this field belongs to
		@leave KErrNoMemory - insufficient free memory to create an instance of the class.
	*/
	//##ModelId=3C4C186E0015
	static CHeaderField* NewL(RStringF aHeaderFieldName, CHeaders& aOwner);
	static CHeaderField* New(RStringF aHeaderFieldName, CHeaders& aOwner);
	
	/** Create a new header field with a single part, whose HTTP value is specified.
		@param aHeaderFieldName The header field type.
		@param aOwner The header fields collection that this field belongs to
		@param aVal The header field value. A copy is taken when storing.
		@leave KErrNoMemory - insufficient free memory to create an instance of the class, or any classes needed to create subclasses for parsing
		@leave KErrNotSupported - if decoding for this header field can't be made
	*/
	//##ModelId=3C4C186E0028
	static CHeaderField* NewL(RStringF aHeaderFieldName, CHeaders& aOwner, THTTPHdrVal aVal);

	/** Create a new header field from the supplied Raw data using the specified transaction codec.
		@param aHeaderFieldName The header field type.
		@param aOwner The header fields collection that this field belongs to
		@param aRawData The raw data buffer, as a descriptor
		@leave KErrNoMemory - insufficient free memory to create an instance of the class, or any classes needed to create subclasses for parsing
		@leave KErrNotSupported - if decoding for this header field can't be made
	*/
	//##ModelId=3C4C186E003C
	static CHeaderField* NewL(RStringF aHeaderFieldName, CHeaders& aOwner, const TDesC8& aRawData);

	// Class destructor
	//##ModelId=3C4C186E0014
	virtual ~CHeaderField();
	/** Access the header value parts by index
		@param aIndex The index of the desired part. 0 references the first one.
		@return The indexed part, or NULL if the index was invalid.
		@leave KErrNoMemory - insufficient free memory for object creation when decoding parts
		@leave KErrNotSupported - if decoding when counting parts fails
	*/
	//##ModelId=3C4C186E000A
	CHeaderFieldPart* PartL(TInt aIndex);
	
	TInt Part(TInt aindex, CHeaderFieldPart*& aPart);

	/** Add a part to the header. It is appended after any existing parts. Simple headers
		have just one part;  they could be constructed using the 'single part' NewL() methods.
		@param aPart The part to be added.  The header takes ownership of the new part.
		@leave KErrNoMemory - insufficient free memory to create an instance of the class, or any classes needed to create subclasses for parsing
		@leave KErrNotSupported - if decoding for this header field can't be made
	*/
	//##ModelId=3C4C186E0000
	void AddPartL(CHeaderFieldPart* aPart);
	TInt AddPart(CHeaderFieldPart* aPart);
	
	/** Insert a part into the header. It is inserted at the specified index - i.e. to insert
		a new part at the start of the header, aIndex should be set to 0.
		@param aPart The part to be inserted.  The header takes ownership of the new part.
		@param aIndex The index at which the part is to be inserted.
		@leave KErrNoMemory - insufficient free memory to create an instance of the class, or any classes needed to create subclasses for parsing
		@leave KErrNotSupported - if decoding for this header field can't be made
	*/
	//##ModelId=3C4C186D03D4
	void InsertPartL(CHeaderFieldPart* aPart, TInt aIndex);

	
	/** Remove the specified part from the header.
		@param aPart The part to be removed.  The header deletes the removed part.
		@return An error code; KErrNone if the removal succeeds or KErrNotFound if it didn't.
		@leave KErrNoMemory - insufficient free memory to create an instance of the class, or any classes needed to create subclasses for parsing
		@leave KErrNotSupported - if decoding for this header field can't be made
	*/
	//##ModelId=3C4C186D03C2
	TInt RemovePartL(CHeaderFieldPart* aPart);


	/** Remove the indexed part from the header
		@param aIndex An index to the part to be removed.  The header deletes the removed part.
		@return An error code; KErrNone if the removal succeeds or KErrNotFound if it didn't.
		@leave KErrNoMemory - insufficient free memory to create an instance of the class, or any classes needed to create subclasses for parsing
		@leave KErrNotSupported - if decoding for this header field can't be made
	*/
	//##ModelId=3C4C186D03CB
	TInt RemovePartL(TInt aIndex);
	
	TInt ConvertToParsedFormat();
	TInt SplitValueAndParameterArray();
private: // methods

	//
	// Internal format conversion
	//

	/** Create an Raw encoding of the header's field name and value. This is done
		using the Codec, which must be supplied prior to this call.	The part data is removed
		after conversion to the Raw format.
		@leave KErrNotSupported if a codec that supports encoding this header cannot be found
	*/
	//##ModelId=3C4C186D03C1
	void ConvertToRawFormatL();

	/** Create the part objects that represent the header field's value, by parsing the 
		Raw data received by HTTP.  The Raw data is removed after conversion completes
		successfully.
		@leave KErrNotSupported if a codec that supports encoding this header cannot be found
	*/
	//##ModelId=3C4C186D03C0
	void ConvertToParsedFormatL();	

	/** Standard constructor.  Used when client constructs headers.
		@param aHeaderFieldName A string table reference to the header field's name.
		@param aOwner The header fields collection that this field belongs to
		@return The constructed instance of CHeaderField
	*/
	//##ModelId=3C4C186D03AC
	CHeaderField(RStringF aHeaderFieldName, CHeaders& aOwner);

	/** Phase Two construction. This variant copies-to-keep the supplied Raw data buffer.
		@param aRawData An 8-bit buffer containing raw header data received by HTTP.
	*/
	//##ModelId=3C4C186D03A3
	void ConstructL(const TDesC8& aRawData);

	// Empty and destroy contents of the 'parts' array.
	//##ModelId=3C4C186D03A2
	void ClearParsedData();

	// Empty and destroy any Raw data held
	//##ModelId=3C4C186D0398
	void ClearRawData();

private: // types and enums

	/** Enumeration of the states in which this header's value data can exist.
	*/
	enum TDataState
		{
		ENoDataSupplied	= 0x00, /**< the header object is not populated with data after construction*/
		EDataInRawForm	= 0x01, /**< the data is in it's raw form, i.e. not accessible by part*/
		EDataParsedOK	= 0x02, /**< the data is in it's client representation, i.e. parsed into parts.	*/
		EDataParseError	= 0x03  /**< the data could not be parsed so remains in Raw form*/
		};

private: // attributes

	/** Size of data chunk used when allocating and extending the Raw data buffer
	*/
	//##ModelId=3C4C186D0323
	TInt iRawChunkSize;

	/** Header data - in Raw format.
		\todo Is it worth this being a union?
	*/
	//##ModelId=3C4C186D0355
	HBufC8* iRawData;

	/** Codec used to convert this header
	*/
	//##ModelId=3C4C186D032C
	const CHeaderCodec& iCodec;

	/**  Name of this header (a reference into the string table) 
	*/
	//##ModelId=3C4C186D037C
	RStringF iName;

	/** Has this header been parsed yet? did it parse correctly?
	*/
	//##ModelId=3C4C186D0372
	TDataState iDataParseState;

	/**  Headers collection that owns this field
	*/
	//##ModelId=3C4C186D0366
	CHeaders* iOwner;

	/** Header data - in parsed format.
		\todo Is it worth this being a union?
	*/
	//##ModelId=3C4C186D034A
	RPointerArray<CHeaderFieldPart> iParts;	
	
	RPointerArray<CHeaderFieldPart>  iElementArray;
	RPointerArray<CHeaderFieldParam>  iParameterArray;
	};




/** Generalised representation of a single HTTP header value part.  The part takes a
	value of one of the following three types:

	- integer, signed (i.e. can be -ve or +ve)
	- string token, which can represent free text (i.e. text with no special meaning; ETags; MD5 hashes;
	  MIME types, etc.), enumerations (words with special meanings such as 'private', 'close', ') or URLs
	- Date/Time

	In addition the part may have one or more parameters.  These take the form of
	attribute-value pairs.  They are implemented by the CHeaderFieldParam class.
*/
//##ModelId=3C4C186B034F
class CHeaderFieldPart : public CBase
	{
friend class THeaderFieldParamIter;
friend class THttpHeaderParamIterator;
friend class THttpHeaderElementIterator;

friend class CHeaderField;
public:
	/** Create a new part, for the supplied HTTP value 
		@param aVal The HTTP value for this part of the header field. A copy is taken when storing.
		@return The constructed instance of CHeaderFieldPart.
	*/
	
	//##ModelId=3C4C186B03D3
	static inline CHeaderFieldPart* NewL(THTTPHdrVal aVal);
	static inline CHeaderFieldPart* New(THTTPHdrVal aVal);
	// Class destructor
	//##ModelId=3C4C186B03D2
	inline ~CHeaderFieldPart();

	// Makes a property set from this object. (Property sets are implemented as
	// CHeaderFieldPart objects)
	//##ModelId=3C4C186B03CA
	inline void SetPropertySet(RHTTPPropertySet& aPropertySet);
	//##ModelId=3C4C186B03C8
	inline static void ClosePropertySet(RHTTPPropertySet& aPropertySet);

	//
	// Getters
	//

	/** Obtain the value held in this field part.  The caller must call Copy() on the returned THTTPHdrVal
		if it is going to be stored.  This is needed in order to correctly maintain string references.
		@return The field part's HTTP value
	*/
	
	//##ModelId=3C4C186B03C7
	inline THTTPHdrVal Value() const;

	/** Obtain the number of parameters in this header
		@return The number, as an integer
	*/ 
	
	//##ModelId=3C4C186B03BE
	inline TInt NumParameters() const;

	/** Access the parameters in this header part via an iterator
		@return An iterator constructed to point at the first parameter
	*/
	
	//##ModelId=3C4C186B03BD
	inline THeaderFieldParamIter Parameters() const;

	/** Access the header parameters by index
		@param aIndex The index of the desired parameter. 0 references the first one.
		@return The indexed parameter, or NULL if the index was invalid.
	*/
	//##ModelId=3C4C186B03B3
	inline CHeaderFieldParam* Parameter(TInt aIndex) const;

	/** Access the header parameters by parameter name
		@param aParamName The name of the desired part.
		@return The named parameter, or NULL if it doesn't exist.
	*/
	//##ModelId=3C4C186B03B5
	CHeaderFieldParam* Parameter(RStringF aParamName) const;

	//
	// Setters
	//

	/** Change the value held in this field part
		@param  The HTTP value. It is copied in order to store it.
	*/
	//##ModelId=3C4C186B03AC
	void SetValue(THTTPHdrVal aVal);

	/** Add a parameter to the header part. It is appended after any existing
		parameters.
		@param aParam The parameter to be added.  The header takes ownership of the
					  new parameter.
	*/
	
	//##ModelId=3C4C186B03AA
	inline void AddParamL(CHeaderFieldParam* aParam);
	
	inline TInt AddParam(CHeaderFieldParam* aParam);
	/** Remove the specified parameter from the header part.
		@param aParam The parameter to be removed.  The header deletes the removed
					  parameter.
		@return An error code; KErrNone if the removal succeeds or KErrNotFound if
				it didn't.
	*/
	//##ModelId=3C4C186B03A1
	TInt RemoveParam(CHeaderFieldParam* aParam);

private: // methods

	// Default constructor. 
	//##ModelId=3C4C186B039F
	inline CHeaderFieldPart(THTTPHdrVal aVal);

private: // attributes

	// The value of this part of the header field
	//##ModelId=3C4C186B0397
	THTTPHdrVal iValue;

	// Parameters for this part.
	//##ModelId=3C4C186B038D
	RPointerArray<CHeaderFieldParam> iParams;
	};


/** Generalised representation of a single HTTP header parameter.  The parameter has
	a name and takes a value using one of the three types supported in CHeaderFieldPart.

	Special case:
	- an unnamed parameter with an assigned value

	Examples of the use for this special case are:

	@verbatim
	Cache-Control: max-stale [=delta-seconds]
	Expect: expectation1=value1; expectation2=value2
	Content-Range: bytes 0-1023/4096
	@endverbatim

	Cache-Control's max-stale directive can take an optional value; hence the client
	should create a one-part header where the part is an enumeration value 'max-stale',
	and assign to that part an un-named parameter for the delta-seconds value if it is
	desired.

	Expect's sets of expectation expressions should each be added by the client as a
	separate part, each of which is given an unnamed parameter that takes the
	expectation	value.

	Content-Range should have one part added containing the enumeration 'bytes'.  Three
	parameters should be defined for that part: 'first', 'last' and 'total' which take
	the values in the byte-range.  This is, of course, application-specific and similar
	guidelines might be adopted by the parser to represent other headers.
	*/

//##ModelId=3C4C186F00B6
class CHeaderFieldParam : public CBase
	{
public: // methods

	/** Create a new header parameter that has a descriptor value. 
		@param aParamName The name of the parameter, as a string table
						  reference. Can be an empty string.
		@param aVal The HTTP value of the parameter. A copy is taken when storing.
		@return The constructed instance of CHeaderFieldParam
	*/
	
	//##ModelId=3C4C186F00F2
	static inline CHeaderFieldParam* NewL(RStringF aParamName, THTTPHdrVal aVal);
	
	static inline CHeaderFieldParam* New(RStringF aParamName, THTTPHdrVal aVal);

	// Class destructor
	//##ModelId=3C4C186F00EB
	inline ~CHeaderFieldParam();

	//
	// Getters
	//

	/** Obtain the name of the parameter, e.g. q
	    @return String table reference to the name
	*/
	
	//##ModelId=3C4C186F00EA
	inline RStringF Name() const;

	/** Obtain the value of the parameter.  The caller must call Copy() on the returned THTTPHdrVal
		if it is going to be stored.  This is needed in order to correctly maintain string references.
		@return The parameter's HTTP value
	*/
	
	//##ModelId=3C4C186F00E9
	inline THTTPHdrVal Value() const;

	//
	// Setters
	//

	/** Change the value held in this field parameter
		@param  The HTTP value
	*/
	//##ModelId=3C4C186F00E1
	inline void SetValue(THTTPHdrVal aVal);

private: // methods

	/** Default constructor. 
	*/
	//##ModelId=3C4C186F00DE
	inline CHeaderFieldParam(RStringF aName, THTTPHdrVal aVal);

private: // attributes

	// A string table reference for the name of this parameter
	//##ModelId=3C4C186F00D6
	RStringF iName;

	// The value of this part of the header field
	//##ModelId=3C4C186F00CC
	THTTPHdrVal iValue;
	};

/** Iterator class to iterate the parts contained within a header field. Requires friendship with
	CHeaderField and as such is tightly bound to that class.
*/
//##ModelId=3C4C187D005C
class THeaderFieldPartIter
	{
public:
	/** Construct an iterator for the parts of the supplied header.
		@param aHeader The header whose parts we want to iterate
	*/
	//##ModelId=3C4C187D0098
	inline THeaderFieldPartIter(const CHeaderField* aHeader);
	/** Class destructor
	*/

	/** Reset iterator to point at the first part of the header
	*/
	
	//##ModelId=3C4C187D0091
	inline void First();
	/** Check if the iterator is at the end of the list. If so, further calls to
	    operator() will return NULL.
		@return True if the iterator has hit the end of the parts list
	*/
	
	//##ModelId=3C4C187D0090
	inline TBool AtEnd();

	/** Advance the iterator.
	*/
	
	//##ModelId=3C4C187D008F
	inline void operator++();
	/** Obtain the header part currently pointed at by the iterator.
		@return The header part;  NULL if the iterator has gone off the end of the list
	*/
	
	//##ModelId=3C4C187D008E
	inline const CHeaderFieldPart* operator()();

private:

	/** Check the iterator state for invalidity following deletions in the list
	*/
	//##ModelId=3C4C187D0084
	inline void CheckInvalidation();

	/** The header whose parts we are iterating.
	*/
	//##ModelId=3C4C187D007C
	const CHeaderField* iHeader;

	/** The index of the part in the header that is currently pointed at by the iterator
	*/
	//##ModelId=3C4C187D0072
	TInt iPosIdx;
	};

/** Iterator class to iterate the parameters contained within a header part. Requires friendship with
	CHeaderFieldPart and as such is tightly bound to that class.
*/
//##ModelId=3C4C187C0105
class THeaderFieldParamIter
	{
public:
	/** Construct an iterator for the parameterss of the supplied header part.
		@param aHeaderPart The header part whose parameters we want to iterate
	*/
	//##ModelId=3C4C187C0144
	inline THeaderFieldParamIter(const CHeaderFieldPart* aHeaderPart);
	/** Class destructor
	*/
	
	//##ModelId=3C4C187C0143
	inline ~THeaderFieldParamIter();

	/** Reset iterator to point at the first part of the header
	*/
	
	//##ModelId=3C4C187C0142
	inline void First();
	/** Check if the iterator is at the end of the list. If so, further calls to
	    operator() will return NULL.
	*/
	
	//##ModelId=3C4C187C0141
	inline TBool AtEnd();

	/** Advance the iterator.
		@return True if the iterator still points at a valid parameter after advancing.
	*/
	
	//##ModelId=3C4C187C0139
	inline void operator++();
	/** Obtain the header parameter currently pointed at by the iterator.
		@return The header parameter;  NULL if the iterator has gone off the end of the list
	*/
	
	//##ModelId=3C4C187C0138
	inline  const CHeaderFieldParam* operator()();

private:

	/** Check the iterator state for invalidity following deletions in the list
	*/
	//##ModelId=3C4C187C0137
	inline void CheckInvalidation();

	/** The header part whose parameters we are iterating.
	*/
	//##ModelId=3C4C187C0125
	const CHeaderFieldPart* iHeaderPart;
	/** The index of the parameter in the header part that is currently pointed at by the iterator
	*/
	//##ModelId=3C4C187C011B
	TInt iPosIdx;
	};

inline void CHeaderFieldPart::SetPropertySet(RHTTPPropertySet& aPropertySet)
	{
	aPropertySet.iImplementation = this;
	}

inline void CHeaderFieldPart::ClosePropertySet(RHTTPPropertySet& aPropertySet)
	{
	delete aPropertySet.iImplementation;
	aPropertySet.iImplementation = 0;
	}

inline TInt CHeaderFieldPart::AddParam(CHeaderFieldParam* aParam)
    {
    return iParams.Append(aParam);
    }

inline CHeaderFieldParam* CHeaderFieldParam::New(RStringF aParamName, THTTPHdrVal aVal)
    {
    return new CHeaderFieldParam(aParamName, aVal);
    }

inline RStringF CHeaderField::Name() const
    {
    return iName;
    }

inline CHeaderFieldPart* CHeaderFieldPart::NewL(THTTPHdrVal aVal)
    {
    return new(ELeave)CHeaderFieldPart(aVal);
    }

inline CHeaderFieldPart* CHeaderFieldPart::New(THTTPHdrVal aVal)
    {
    return new CHeaderFieldPart(aVal);
    }

inline THTTPHdrVal CHeaderFieldPart::Value() const
    {
    return iValue;
    }

inline TInt CHeaderFieldPart::NumParameters() const
    {
    // Check the array exists first
    return iParams.Count();
    }


inline THeaderFieldParamIter CHeaderFieldPart::Parameters() const
    {
    // create an iterator initialised for this part
    return THeaderFieldParamIter(this);
    }


inline CHeaderFieldParam* CHeaderFieldPart::Parameter(TInt aIndex) const
    {
    // Check the parameter is in range, if so return it
    if (aIndex < NumParameters())
        return (CHeaderFieldParam*)iParams[aIndex];
    else
        return NULL;
    }

inline void CHeaderFieldPart::AddParamL(CHeaderFieldParam* aParam)
    {
    // Append the item
    User::LeaveIfError(iParams.Append(aParam));
    }

inline CHeaderFieldPart::CHeaderFieldPart(THTTPHdrVal aVal)
    : iValue(aVal)
    {
    // Ensure that we've copied the HdrVal properly - strings must be Copy()d in order to
    // correctly maintain string references
    iValue = aVal.Copy();
    }

inline CHeaderFieldPart::~CHeaderFieldPart()
    {
    // Remove the parameter list.  Parent class clears the part value.
    iParams.ResetAndDestroy();

    // If the part's value is a string, then it must be closed.
    if (iValue.Type() == THTTPHdrVal::KStrFVal)
        iValue.StrF().Close();
    if (iValue.Type() == THTTPHdrVal::KStrVal)
        iValue.Str().Close();
    }


inline CHeaderFieldParam* CHeaderFieldParam::NewL(RStringF aParamName, THTTPHdrVal aVal)
    {
    return new(ELeave)CHeaderFieldParam(aParamName, aVal);
    }



/** Getter's
*/

inline RStringF CHeaderFieldParam::Name() const
    {
    return iName;
    }

inline THTTPHdrVal CHeaderFieldParam::Value() const
    {
    return iValue;
    }

/** Setter's
*/

inline void CHeaderFieldParam::SetValue(THTTPHdrVal aVal)
    {
    // We must check for an existing string value since it will have to be closed
    if (iValue.Type() == THTTPHdrVal::KStrFVal)
        iValue.StrF().Close();
    if (iValue.Type() == THTTPHdrVal::KStrVal)
        iValue.Str().Close();
    iValue = aVal.Copy();
    }

/** c'tor
*/
inline CHeaderFieldParam::CHeaderFieldParam(RStringF aName, THTTPHdrVal aVal)
    : iValue(aVal)
    {
    // Ensure that we've copied the arguments properly - strings must be Copy()d in order to
    // correctly maintain string references
    iName = aName.Copy();
    iValue = aVal.Copy();
    }

inline CHeaderFieldParam::~CHeaderFieldParam()
    {
    // Close the parameter name and value, if it is a string.
    iName.Close();
    if (iValue.Type() == THTTPHdrVal::KStrVal)
        iValue.Str().Close();
    if (iValue.Type() == THTTPHdrVal::KStrFVal)
        iValue.StrF().Close();
    }


inline THeaderFieldPartIter::THeaderFieldPartIter(const CHeaderField* aHeader)
    : iHeader(aHeader)
    {
    // to begin at the beginning...
    First();
    }

/** d'tor
*/


/** Reset the iterator to point at the first element
*/


inline void THeaderFieldPartIter::First()
    {
    // Check existence of the array in CHeaderField. If it hasn't been created yet then
    // we must start off 'at the end'.  This is indicated by the position index set to KErrNotFound
    iPosIdx = 0;
    CheckInvalidation();
    }

/** Check if the iterator is at the end of the list it traverses
*/


inline TBool THeaderFieldPartIter::AtEnd()
    {
    return (iPosIdx == KErrNotFound);
    }

/** Advance the iterator
*/

inline void THeaderFieldPartIter::operator++()
    {
    // Do nothing if we're already at the end
    if (iPosIdx >= 0)
        {
        ++iPosIdx;
        CheckInvalidation();
        }
    }

/** Obtain the element at the iterator's current position
*/

inline const CHeaderFieldPart* THeaderFieldPartIter::operator()()
    {
    CheckInvalidation();
    if (iPosIdx > KErrNotFound)
        return iHeader->iParts.operator[](iPosIdx);
    else
        return NULL;
    }

inline void THeaderFieldPartIter::CheckInvalidation()
    {
    // iPosIdx may have been valid after the last use of operator() but
    // if an item was subsequently removed from the collection the iterator
    // may have gone off the end.
    if (iPosIdx >= iHeader->iParts.Count())
        iPosIdx = KErrNotFound; // Hit the end
    }

inline THeaderFieldParamIter::THeaderFieldParamIter(const CHeaderFieldPart* aHeaderPart)
    : iHeaderPart(aHeaderPart)
    {
    // to begin at the beginning...
    First();
    }

/** d'tor
*/


inline THeaderFieldParamIter::~THeaderFieldParamIter()
    {
    // does nothing
    }

/** Reset the iterator to point at the first element
*/


inline void THeaderFieldParamIter::First()
    {
    // Check existence of the array in CHeaderFieldPart. If it hasn't been created yet then
    // we must start off 'at the end'.  This is indicated by the position index set to KErrNotFound
    iPosIdx = 0;
    CheckInvalidation();
    }

/** Check if the iterator is at the end of the list it traverses
*/


inline TBool THeaderFieldParamIter::AtEnd()
    {
    return (iPosIdx == KErrNotFound);
    }

/** Advance the iterator
*/


inline void THeaderFieldParamIter::operator++()
    {
    // Do nothing if we're already at the end
    if (iPosIdx >= 0)
        {
        ++iPosIdx;
        CheckInvalidation();
        }
    }

/** Obtain the element at the iterator's current position
*/

inline const CHeaderFieldParam* THeaderFieldParamIter::operator()()
    {
    CheckInvalidation();
    if (iPosIdx > KErrNotFound)
        return iHeaderPart->iParams.operator[](iPosIdx);
    else
        return NULL;
    }

inline void THeaderFieldParamIter::CheckInvalidation()
    {
    // iPosIdx may have been valid after the last use of operator() but
    // if an item was subsequently removed from the collection the iterator
    // may have gone off the end.
    if (iPosIdx >= iHeaderPart->iParams.Count())
        iPosIdx = KErrNotFound; // Hit the end
    }


#endif // __CHEADERFIELD_H__
