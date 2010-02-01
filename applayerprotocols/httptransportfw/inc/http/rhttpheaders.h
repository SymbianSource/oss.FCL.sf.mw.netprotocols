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

#ifndef	__RHTTPHEADERS_H__
#define	__RHTTPHEADERS_H__

// System includes
#include <http/thttphdrfielditer.h>
#include <http/thttphdrval.h>


//##ModelId=3C4C1880001A
class RHTTPHeaders
/** 
The collection of headers (or more correctly, header fields)
associated with a message. Header (fields) can be created, read and
modified. They may be composed of several parts (by repeated
invocations of API methods, see below) and may be assigned one or
more parameters.  Individual field parts and parameters take
values described using THTTPHdrVal.
@publishedAll
@released
@see RHTTPMessage
@see THTTPHdrVal
*/
	{
 public:
	/** Default constructor
	*/
	//##ModelId=3C4C18800088
	inline RHTTPHeaders();

	/**Getter methods
		Methods for reading data.
	*/
	//@{
	/** Obtain the number of parts in the named header field's value,
		Simple headers are created with a single part following one
		call to SetFieldL. Subsequent calls to SetFieldL create
		additional parts if the field exists already.
		@leave KErrNoMemory
		@param aFieldName The header name
		@return The number of parts, or zero if the field does not exist.  */
	//##ModelId=3C4C18800081
	IMPORT_C TInt FieldPartsL(RStringF aFieldName) const;

	/** Obtain the named header field's value. The index
		of a part within the field must be specified. Parts are indexed
		from 0 and fields with only one part return the entire field for index 0
		@param aFieldName The header name
		@param aPartIdx The index of the part
		@param aHeaderValue The header field value
		@return An error condition. Returns KErrNotFound if there is not  a field with the 
				specifed field name
	*/
	//##ModelId=3C4C18800079
	IMPORT_C TInt GetField(RStringF aFieldName, 
								  TInt aPartIdx, THTTPHdrVal& aHeaderValue) const;
	
	/** Obtain an Raw representation of the named header
		field's value.  Note that general client use of this method is
		strongly discouraged since it exposes the Raw representation of particular headers.  
		However it may be needed for some cases where received headers could not be
		decoded by HTTP.  It will normally be used internally when
		preparing header data to be transmitted with a request.
		Note that this API may not return the field values for all the headers.
		@param aFieldName The field name, e.g, 'Content-Type'
		@param aRawFieldData The field's data content, in an appropriate Raw form 
		@return An error condition. Returns KErrNotFound if there is not  a field with the 
				specifed field name
	*/
	//##ModelId=3C4C18800076
	IMPORT_C TInt GetRawField(RStringF aFieldName, 
							   TPtrC8& aRawFieldData) const;
	
	IMPORT_C void GetRawFieldL(RStringF aFieldName, TPtrC8& aRawFieldData) const;
	
	/** Obtain the value of a named parameter, associated with the
		named header field.  An optional index to a part within the
		header field may be supplied, if not it is assumed that it is
		the first part.
		@param aFieldName The header name
		@param aParamName The parameter name
		@param aReturn The returned value. Note that this
		must be Copy()d by the caller, if it wants to keep the value.
		@param aPartIdx The optional index of the part
		@return An error condition. Returns KErrNotFound if there is not  a field with the 
				specifed field name  */
	//##ModelId=3C4C1880006C
	IMPORT_C TInt GetParam(RStringF aFieldName, 
								  RStringF aParamName, 
								  THTTPHdrVal& aReturn,
								  TInt aPartIdx = 0) const;

	/** Access the fields within this header collection, via an
		iterator.  Each application of the iterator returns the name
		of the next field type.  This may then be accessed via
		RHTTPHeaders methods.
		@return The iterator.  */
	//##ModelId=3C4C1880006B
	IMPORT_C THTTPHdrFieldIter Fields() const;
	//@}

	/** Setter Methods
		Methods for writing data.
	*/
	//@{
	/** Set a named field in the header.  On the first instance that
		this API method is used for a given field name, the first will
		be created.  On subsequent calls, the same field will be
		extended to have several parts, with a new part created to
		hold the supplied value.
		@param aFieldName The field name, e.g, 'Content-Type'
		@param aFieldValue The field value, e.g. 'text/html' */
	//##ModelId=3C4C18800060
	IMPORT_C void SetFieldL(RStringF aFieldName, THTTPHdrVal aFieldValue);
	IMPORT_C TInt SetField(RStringF aFieldName, THTTPHdrVal aFieldValue);
	
	/** Set a named field in the header, and associate with it the
		supplied parameter. If the field doesn't already exist it will
		be created along with a parameter; if it does exist, then a
		new part will be created along with the parameter.
		@param aFieldName The field name, e.g. 'Accept'
		@param aFieldValue The field value. e.g. 'text/plain'
		@param aParamName The parameter name, e.g. 'q'
		@param aParamValue The parameter value, e.g. '0.3' */
	//##ModelId=3C4C18800063
	IMPORT_C void SetFieldL(RStringF aFieldName, THTTPHdrVal aFieldValue,
							RStringF aParamName, THTTPHdrVal aParamValue);
	IMPORT_C TInt SetField(RStringF aFieldName, THTTPHdrVal aFieldValue,
                            RStringF aParamName, THTTPHdrVal aParamValue);
	/** Set a parameter in an existing header. 
		@param aFieldName The field name, e.g. 'Accept'
		@param aPartIdx The part of the header to add the parameter to
		@param aParamName The parameter name, e.g. 'q'
		@param aParamValue The parameter value, e.g. '0.3'
		@leave KErrNotFoud if the field, or the part within the field doesn't exist
	*/
	//##ModelId=3C4C18800058
	IMPORT_C void SetParamL(RStringF aFieldName, RStringF aParamName, THTTPHdrVal aParamValue, TInt aPartIdx);

	/** Set a named field in the header to contain the supplied Raw header
		data. If the header already exists then a LF and the new data will be added to the existing data. This is used to
		indicate that there are multiple instances of this header


		Note that general client use of this method is strongly
		discouraged since it exposes the raw representation of particular headers.  
		However it may be needed for some cases where headers to be transmitted have no encoding known
		to HTTP.  It will normally be used internally when receiving data from a service provider.
		@param aFieldName The field name, e.g, 'Content-Type'
		@param aRawFieldData The field's data content, in a raw form
		@param aFieldSeparator The header field separator
	*/
	//##ModelId=3C4C1880004F
	IMPORT_C void SetRawFieldL(RStringF aFieldName, 
							   const TDesC8& aRawFieldData,
							   const TDesC8& aFieldSeparator);
	
	IMPORT_C TInt SetRawField(RStringF aFieldName, const TDesC8& aRawFieldData, const TDesC8& aFieldSeparator);
	/** Remove, entirely, the named header field from the header
		collection. All its parts and associated parameters (where
		they exist) are also removed.
		@param aFieldName The field name.
		@return KErrNone if the removal is successful; KErrNotFound if
		the field didn't exist within the headers.  */
	//##ModelId=3C4C1880004D
	IMPORT_C TInt RemoveField(RStringF aFieldName);

	/** Remove a single part of a header field. Just the part and any associated paramters are removed. If this 
		results in no parts being present in the header then it will also be removed
		@param aFieldName The header name
		@param aIndex The particular part of the field to be removed
		@return KErrNone if the removal is sucessful; KErrNotFound if the header didn't exist. No exception is raised if 
		the particular value is not found as part of that header */
	//##ModelId=3C4C18800043
	IMPORT_C TInt RemoveFieldPart(RStringF aFieldName, TInt aIndex);

	/** Remove all the fields of this header collection
	*/
	//##ModelId=3C4C18800042
	IMPORT_C void RemoveAllFields();

	//@}


private:
	friend class CHeaders;
	friend class CHttpClientTransaction;
	friend class CHttpClientTransactionImpl;
	//##ModelId=3C4C1880003A
	CHeaders* iImplementation;
	};


inline RHTTPHeaders::RHTTPHeaders()
		: iImplementation(NULL)
	{
	}


#endif  // __RHTTPHEADERS_H__
