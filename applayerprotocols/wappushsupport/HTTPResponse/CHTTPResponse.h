// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Purpose:  This file provides the definition of the CHTTPResponse class.
// The HTTP Response class encodes HTTP response headers only. It
// contains the methods used to transcode from WSP->HTTP fields
// 
//

#ifndef __CHTTPRESPONSE_H__
#define __CHTTPRESPONSE_H__

// System includes
//
#include <e32base.h>
#include <logdef.h>
#include <httpstd.h>
#include <thttpfields.h>


// Forward class declarations
//
class CPushMessage;

// Class definition for CHTTPResponse
//
//##ModelId=3B712B9A0231
class CHTTPResponse : public CBase
/**
@publishedAll
@deprecated
*/
    {
public:	// Methods

	// Factory method to construct this class.
	//
	// Rtn: a new CHTTPResponse object, by ptr. Ownership is transferred to the
	//      caller.
	//
	//##ModelId=3B712B9B000A
	IMPORT_C static CHTTPResponse* NewL();

	// Destructor for this class. Removes this object and releases memory held
	// by it
	//
	//##ModelId=3B712B9B0000
	IMPORT_C virtual ~CHTTPResponse();

	// Clean out the fields buffer
	//
	//##ModelId=3B712B9A03D6
	IMPORT_C void Reset();

	// Set the fields buffer with the response received from the WAP Stack
	//
	// In:
	//  aResponse - an 8-bit descriptor field containing the origin server's
	//				WSP-encoded response header. Ownership is transferred to
	//				this class.
	//
	//##ModelId=3B712B9A03D4
	IMPORT_C void AddResponse(HBufC8* aResponse);

	// Accessor to the HTTP response fields buffer
	//
	// Rtn: a reference to the response. Ownership is _NOT_ transferred
	//
	// NOTE THIS SHOULD RETURN CONST - BUT CAN'T BE CHANGED SINCE IT WOULD
	// BREAK BC.
	//##ModelId=3B712B9A03CA
	IMPORT_C HBufC8& Response() const;

 	// Accessor to the HTTP status code (e.g. 400, 300, 200, 500)
	//
	// Rtn: the status code - series number only.
	//
	//##ModelId=3B712B9A03C1
	IMPORT_C THttpStatusCode StatusCode() const;

 	// Accessor to the HTTP detailed status code (e.g. 404, 304, 200, 501)
	//
	// Rtn: the status code - series and specific code value
	//
	//##ModelId=3B712B9A03C0
	IMPORT_C THttpStatusCode DetailedStatusCode() const;

	// Accessor to set the HTTP response status.
	//
	// In:
	//  aCode - the WSP-encoded status code
	//
	//##ModelId=3B712B9A03B6
	IMPORT_C void SetStatusCode(TInt aCode);

	// Method to find a named field, that returns null terminated 
	// WSP text strings.
	//
	// In:
	//  aField		- the field type
	//  aStartIndex	- the index to search from (defaults to the buffer start)
	//
	// Out:
	//  aDesc - a pointer-type descriptor into the response buffer at the
	//			position where the field was located. The caller must NOT
	//			modify the descriptor contents
	//
	// Rtn: TBool - set to ETrue if the field was found, EFalse otherwise
	//
	//##ModelId=3B712B9A0390
	IMPORT_C TBool FindField(THttpHeaderField aField
                             , TPtrC8& aDesc
                             , TInt aStartIndex = 0) const;

	// Method to find a named field, that returns 8-bit octet data (binary
	// or strings - not stipulated which).
	//
	// In:
	//  aField		- the field type
	//  aStartIndex	- the index to search from (defaults to the buffer start)
	//
	// Out:
	//  aDesc - a pointer-type descriptor into the response buffer at the
	//			position where the field was located. The caller must NOT
	//			modify the descriptor contents
	//
	// Rtn: TBool - set to ETrue if the field was found, EFalse otherwise
	//
	//##ModelId=3B712B9A0384
	IMPORT_C TBool FindBinaryDescField(THttpHeaderField aField 
							, TPtrC8& aDesc
							, TInt aStartIndex = 0) const;

	// Method to find a named field, that returns an EPOC date/time structure.
	//
	// In:
	//  aField		- the field type
	//  aStartIndex	- the index to search from (defaults to the buffer start)
	//
	// Out:
	//  aTime - a structure containing the time (and date) found in the header
	//
	// Rtn: TBool - set to ETrue if the field was found, EFalse otherwise
	//
	//##ModelId=3B712B9A03A2
	IMPORT_C TBool FindField(THttpHeaderField aField
                             , TTime& aTime
                             , TInt aStartIndex = 0) const;

	// Method to find a named field within the Cache Control header
	//
	// In:
	//  aField		- the field type
	//
	// Out:
	//
	// Rtn: TBool - set to ETrue if the field was found, EFalse otherwise
	//
	//##ModelId=3B712B9A0370
	IMPORT_C TInt FindCacheControlFieldValue(TCacheControlFieldValue aField) const;

	// Method to find a named field within the Cache Control header, 
	// that returns an EPOC date/time structure.
	//
	// In:
	//  aField		- the field type
	//
	// Out:
	//  aTime - a structure containing the time (and date) found in the header field
	//
	// Rtn: TBool - set to ETrue if the field was found, EFalse otherwise
	//
	//##ModelId=3B712B9A035C
	IMPORT_C TBool ExtractCacheControlTime(TCacheControlFieldValue aField, TTime& aTime) const;

	// Method to search for the content type encoded in the response header
	//
	// Out:
	//  aDesc - a pointer-type descriptor into the appropriate element of an
	//			array prefilled with all the content types that have WSP
	//			encodings. e.g. "text/vnd.wap.wml".  The contents of the
	//			descriptor must NOT be modified.
	//
	//##ModelId=3B712B9A0348
	IMPORT_C void ContentType(TPtrC8& aDesc) const;

	// Method to search for the realm encoded in the response header, when the
	// response challenges the client for HTTP authentication (code 401)
	//
	// Out:
	//  aDesc - a pointer-type descriptor into the response header buffer
	//			positioned at the realm string within the challenge. The
	//			contents of the descriptor must NOT be modified.
	//
	// Rtn: TBool - set to ETrue if a www-authenticate realm was found, EFalse
	//				otherwise
	//
	//##ModelId=3B712B9A033F
	IMPORT_C TBool FindRealm(TPtrC8& aRealm) const;

	// Method to search for the character set encoded in the Content-Type
	// field ofthe response header
	//
	// Out:
	//  aDesc - a pointer-type descriptor into the appropriate element of an
	//			array prefilled with all the character sets that have WSP
	//			encodings. e.g. "utf-8".  The contents of the descriptor must
	//			NOT be modified.
	//
	// Rtn: TBool - set to ETrue if a character set was found, EFalse if not
	//
	//##ModelId=3B712B9A032A
	IMPORT_C TBool CharSet(TPtrC8& aDesc) const;

protected: // Methods

	// Normal constructor - do non-allocating creation of this class
	//
    IMPORT_C CHTTPResponse();

	// Second phase construction - any allocation for this class must take place
	// here. Sets up the resources required by an HTTP Response.
	//
	//##ModelId=3B712B9A0322
	IMPORT_C void ConstructL();

	//friend class declaration - only need the following function
	friend class CPushMessage;
	// Method to locate a named field in the response header, starting at the
	// specified index position.
	//
	// In:
	//  aField		- the header field type
	//  aStartIndex - the (optional) position in the header to start searching
	//
	// Rtn: TInt - the index position of the required field _value_ (not the
	//             field name), or KErrNotFound otherwise.
	//
	//##ModelId=3B712B9A030D
	IMPORT_C TInt LocateField(THttpHeaderField aField, TInt aStartIndex = 0) const; 

	// Perform a look-up of content type given a WSP encoding value, used as
	// an index.
	//
	// In:
	//  aIndex - the WSP encoding value
	//
	// Rtn: const TText8* - the required content type text - NOT to be changed
	//
	//##ModelId=3B712B9A0352
	IMPORT_C const TText8* ContentType(TInt aContentTypeCode) const;

 	// Perform a look-up of character set given a WSP encoding value, used as
	// an index.
	//
	// In:
	//  aCharsetCode - the index into the content types table/
	//
	// Rtn: const TText8* - the required 8-bit character set text - NOT to be
	//						changed by the caller
	//
	//##ModelId=3B712B9A0334
	IMPORT_C const TText8* CharSet(TInt aCharsetCode) const;

private: // Attributes

	// The 8-bit buffer used to store the HTTP response fields
	//
	//##ModelId=3B712B9A02AA
    HBufC8* iResponse;

	// The HTTP Status code (coarse - ie. series only, e.g. 100, 200 etc.)
	//
	//##ModelId=3B712B9A0296
    THttpStatusCode iStatusCode;

	// The detailed HTTP Status code (within series, e.g. 102, 204, 401 etc.)
	//
	//##ModelId=3B712B9A0282
    THttpStatusCode iDetailedStatusCode;

	// Declare the CLogClient pointer
   	__DECLARE_LOG

private: // Methods

	// Do a conversion from 32-bit UIntVar encoding into 32-bit integer
	//
	TInt ParseUIntVar(const TDesC8& aBuffer, TInt& aVal) const;

	// Extract a WSP encoded MultiOctet Integer encoding into 32-bit integer
	//
	// In:
	//  aSource	- the source Multi-Octet integer
	//
	// Out:
	//  aInt		- the 32-bit resulting integer
	//
	//##ModelId=3B712B9A02F8
	void ExtractMultiOctetInteger(TInt& aInt, const TPtrC8& aSource) const;

	// Method to find a named field within the Cache Control header
	//
	// In:
	//  aSource		- the descriptor containing the date value
	//	aFrom		- The position in the descriptor to start from
	//
	// Out:
	//  aTime - a structure containing the time (and date) found in the descriptor
	//
	//##ModelId=3B712B9A02E4
	void ExtractFieldDateValue(const TPtrC8& aSource, TInt aFrom, TTime& aTime) const;

	// Method to find a named field within the Cache Control header
	//
	// In:
	//  aField		- the field type
	//
	// Out:
	//	the found aCacheControl string
	//
	// Rtn: TInt - set to KErrNotFound if the field was not found,
	//		otherwise the position in the cache control descriptor that the field was found
	//
	//##ModelId=3B712B9A0372
	TInt FindCacheControlFieldValue(TCacheControlFieldValue aField,TPtrC8& aCacheControl) const;

	//##ModelId=3B712B9A02DA
    void Panic(THttpPanicCode aPanicCode) const;

	// Spare methods for future BC. Const- and non-const versions to assist
	// the caller in preserving const-ness. IMPORT_C ensures they reserve a
	// slot in the vtbl, which is essential to preseve future BC.
	//
	//##ModelId=3B712B9A02D0
	IMPORT_C virtual TAny* Extend_CHTTPResponse(TAny* aArgs);
	//##ModelId=3B712B9A02C6
	IMPORT_C virtual TAny* Extend_CHTTPResponse_const(TAny* aArgs) const;

#if defined _DEBUG
	// Debug method definition to allow class invariant testing
	// Called by the standard macro __TEST_INVARIANT
	//
	//##ModelId=3B712B9A02BD
	void __DbgTestInvariant() const;

	//##ModelId=3B712B9A02B2
	void DumpToLog(const TDesC8& aData) const;
#endif

	};

#endif // __CHTTPRESPONSE_H__

