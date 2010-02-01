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



// System includes
//
#include <wspdecoder.h>
#include <chttpresponse.h>

// Wap Logging
//
#include <waplog.h>
#include <logdef.h>

const TUint8 KWapQuote = 0x7F;
const TUint8 KTop3BitSet = 0x70;
const TUint8 KCarryBitMask = 0x80;

// Default MIME type is text/plain if we can't identify any other
//
static const TText8* const defaultType = _S8("application/octet-stream");

// Implementation of CHTTPResponse class
//


// Factory method to construct this class.
//
// Rtn: a new CHTTPResponse object, by ptr. Ownership is transferred to the
//      caller.
//

EXPORT_C CHTTPResponse* CHTTPResponse::NewL()
	{
	CHTTPResponse* me = new(ELeave)CHTTPResponse();
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}


// Destructor for this class. Removes this object and releases memory held
// by it
//

EXPORT_C CHTTPResponse::~CHTTPResponse()
    {
    Reset();
	__CLOSE_LOG;
    }


// Clean out the fields buffer
//

EXPORT_C void CHTTPResponse::Reset()
    {
	__LOG_ENTER(_L("CHTTPResponse::Reset"));
    delete iResponse;
    iResponse = NULL;
	__LOG_RETURN;
    }


// Set the fields buffer with the response received from the WAP Stack
//
// In:
//  aResponse - an 8-bit descriptor field containing the origin server's
//				WSP-encoded response header. Ownership is transferred to
//				this class.
//

EXPORT_C void CHTTPResponse::AddResponse(HBufC8* aResponse)
    {
	__LOG_ENTER(_L("CHTTPResponse::AddResponse"));
    delete iResponse;
    iResponse = aResponse;
#ifdef _DEBUG
	DumpToLog(*aResponse);
#endif
	__LOG_RETURN;
    }


// Accessor to the HTTP response fields buffer
//
// Rtn: a reference to the response. Ownership is _NOT_ transferred
//
// NOTE THIS SHOULD RETURN CONST - BUT CAN'T BE CHANGED SINCE IT WOULD
// BREAK BC.

EXPORT_C HBufC8& CHTTPResponse::Response() const
    {
	__LOG_ENTER(_L("CHTTPResponse::Response"));
	__LOG_RETURN;
    return *iResponse;
    }


// Accessor to the HTTP status code (e.g. 400, 300, 200, 500)
//
// Rtn: the status code - series number only.
//

EXPORT_C THttpStatusCode CHTTPResponse::StatusCode() const
    {
	__LOG_ENTER(_L("CHTTPResponse::StatusCode"));
	__LOG_RETURN;
    return iStatusCode;
    }


// Accessor to the HTTP detailed status code (e.g. 404, 304, 200, 501)
//
// Rtn: the status code - series and specific code value
//

EXPORT_C THttpStatusCode CHTTPResponse::DetailedStatusCode() const
    {
	__LOG_ENTER(_L("CHTTPResponse::DetailedStatusCode"));
	__LOG_RETURN;
    return iDetailedStatusCode;
    }


// Accessor to set the HTTP response status.
//
// In:
//  aCode - the WSP-encoded status code
//

EXPORT_C void CHTTPResponse::SetStatusCode(TInt aCode)
    {
//	__LOG_ENTER(_L("CHTTPResponse::SetStatusCode"));
//	__LOG1(_L("CHTTPResponse::SetStatusCode : 'aCode' = %d"), aCode);
    
	// Set iDetailedStatusCode to a default
    iDetailedStatusCode = EHttpUnknown;

    // Magic no.s come from the WAP WSP specification, Appendix A, Table 36
    switch (aCode)
        {
    case 0x10:
        iStatusCode = EHttpContinue;
        break;
    case 0x11:
        iDetailedStatusCode = EHttpSwitchingProtocols;
        iStatusCode = EHttpContinue;
        break;

    case 0x20:
        iStatusCode = EHttpOK;
        break;
    case 0x21:
        iDetailedStatusCode = EHttpCreated;
        iStatusCode = EHttpOK;
        break;
    case 0x22:
        iDetailedStatusCode = EHttpAccepted;
        iStatusCode = EHttpOK;
        break;
    case 0x23:
        iDetailedStatusCode = EHttpNonAuthorativeInformation;
        iStatusCode = EHttpOK;
        break;
    case 0x24:
        iDetailedStatusCode = EHttpNoContent;
        iStatusCode = EHttpOK;
        break;
    case 0x25:
        iDetailedStatusCode = EHttpResetContent;
        iStatusCode = EHttpOK;
        break;
    case 0x26:
        iDetailedStatusCode = EHttpPartialContent;
        iStatusCode = EHttpOK;
        break;

    case 0x30:
        iStatusCode = EHttpMultipleChoices;
        break;
    case 0x31:
        iDetailedStatusCode = EHttpMovedPermanently;
        iStatusCode = EHttpMultipleChoices;
        break;
    case 0x32:
        iDetailedStatusCode = EHttpMovedTemporarily;
        iStatusCode = EHttpMultipleChoices;
        break;
    case 0x33:
        iDetailedStatusCode = EHttpSeeOther;
        iStatusCode = EHttpMultipleChoices;
        break;
    case 0x34:
        iDetailedStatusCode = EHttpNotModified;
        iStatusCode = EHttpMultipleChoices;
        break;
    case 0x35:
        iDetailedStatusCode = EHttpUseProxy;
        iStatusCode = EHttpMultipleChoices;
        break;

    case 0x40:
        iStatusCode = EHttpBadRequest;
        break;
    case 0x41:
        iDetailedStatusCode = EHttpUnauthorized;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x42:
        iDetailedStatusCode = EHttpPaymentRequired;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x43:
        iDetailedStatusCode = EHttpForbidden;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x44:
        iDetailedStatusCode = EHttpNotFound;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x45:
        iDetailedStatusCode = EHttpMethodNotAllowed;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x46:
        iDetailedStatusCode = EHttpNotAcceptable;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x47:
        iDetailedStatusCode = EHttpProxyAuthenticationRequired;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x48:
        iDetailedStatusCode = EHttpRequestTimeout;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x49:
        iDetailedStatusCode = EHttpConflict;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x4a:
        iDetailedStatusCode = EHttpGone;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x4b:
        iDetailedStatusCode = EHttpLengthRequired;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x4c:
        iDetailedStatusCode = EHttpPreconditionFailed;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x4d:
        iDetailedStatusCode = EHttpRequestEntityTooLarge;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x4e:
        iDetailedStatusCode = EHttpRequestURITooLong;
        iStatusCode = EHttpBadRequest;
        break;
    case 0x4f:
        iDetailedStatusCode = EHttpUnsupportedMediaType;
        iStatusCode = EHttpBadRequest;
        break;

    case 0x60:
        iStatusCode = EHttpInternalServerError;
        break;
    case 0x61:
        iDetailedStatusCode = EHttpNotImplemented;
        iStatusCode = EHttpInternalServerError;
        break;
    case 0x62:
        iDetailedStatusCode = EHttpBadGateway;
        iStatusCode = EHttpInternalServerError;
        break;
    case 0x63:
        iDetailedStatusCode = EHttpServiceUnavailable;
        iStatusCode = EHttpInternalServerError;
        break;
    case 0x64:
        iDetailedStatusCode = EHttpGatewayTimeout;
        iStatusCode = EHttpInternalServerError;
        break;
    case 0x65:
        iDetailedStatusCode = EHttpHTTPVersionNotSupported;
        iStatusCode = EHttpInternalServerError;
        break;

    default:
        iStatusCode = EHttpUnknown;
        break;
        }
    if (iDetailedStatusCode == EHttpUnknown)
        iDetailedStatusCode = iStatusCode;
//	__LOG1(_L("CHTTPResponse::SetStatusCode : status code = %d"), iStatusCode);
//	__LOG1(_L("CHTTPResponse::SetStatusCode : detailed status code = %d"), iDetailedStatusCode);
//	__LOG_RETURN;
    }


// Method to find a named field, that returns null terminated 
// WSP text strings. Note that there is no checking that it is a text string
// which follows.
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

EXPORT_C TBool CHTTPResponse::FindField(THttpHeaderField aField,
							   TPtrC8& aDesc,
							   TInt aStartIndex) const
    {
//	__LOG_ENTER(_L("CHTTPResponse::FindField (string)"));
//	__LOG1(_L("CHTTPResponse::FindField : searching for field type = %d"), aField);
    TInt index = LocateField(aField, aStartIndex);
    if (index >0)
        {
        TInt count = 0;
        for (count = index; (count < iResponse->Length()) &&
			(iResponse->Des()[count] != 0); count++) {};
        if (count <= iResponse->Length())
            {
            aDesc.Set(&(iResponse->Des()[index]), count-index);
//			__LOG(_L("CHTTPResponse::FindField : found string:"));
#ifdef _DEBUG
			DumpToLog(aDesc);
#endif
//			__LOG_RETURN;
            return ETrue;
            }
        }
//	__LOG(_L("CHTTPResponse::FindField : nothing found"));
//	__LOG_RETURN;
    return EFalse;
    }


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

EXPORT_C TBool CHTTPResponse::FindBinaryDescField(THttpHeaderField aField,
										 TPtrC8& aDesc,
										 TInt aStartIndex) const
    {
    TInt index = LocateField(aField, aStartIndex);
    if (index >= 0)
        {
		TInt offset = 0;
		TInt fieldLength = iResponse->Des()[index];	// assume the length is in
													// a short integer
		if(fieldLength == 31)
			{
			// Nope : Code 31 indicates that the following bytes make a
			// UIntVar,  which indicates the number of data octets after it. 
			// The UIntVar itself could be composed of upto 5 bytes
			// Copy a full 5 bytes from the header
			// Note that actually fewer might have been used; 
			// the UIntVar to Int converter function returns the exact number 
			// that were used.
			TInt consumed = ParseUIntVar(iResponse->Des().Mid(index + 1), fieldLength);

			__ASSERT_DEBUG( consumed >= KErrNone, User::Invariant() );
			
			offset += consumed;			
			}
        else if (fieldLength > 127)
			{
 			// Oops be sneaky and reuse this single byte
 			// Because this is a special code
 			fieldLength = 1;		
 			offset = -1;
 			}

		if(fieldLength)
            {
            aDesc.Set(&(iResponse->Des()[index + offset + 1]), fieldLength);
#ifdef _DEBUG
			DumpToLog(aDesc);
#endif
            return ETrue;
            }
        }
    return EFalse;
    }


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

EXPORT_C TBool CHTTPResponse::FindField(THttpHeaderField aField,
							   TTime& aTime,
							   TInt aStartIndex) const
    {
	__LOG_ENTER(_L("CHTTPResponse::FindField (time)"));
	TBool result = EFalse;
    TInt index = LocateField(aField, aStartIndex);
    if (index > 0)
        {
 		TPtr8 respChars  = iResponse->Des();
   		ExtractFieldDateValue(respChars,index,aTime);
		result = ETrue;
        }
	__LOG_RETURN;
    return result;
    }


// Method to find a named field within the Cache Control header
//
// In:
//  aField		- the field type
//
// Out:
//
// Rtn: TBool - set to ETrue if the field was found, EFalse otherwise
//

EXPORT_C TBool CHTTPResponse::FindCacheControlFieldValue(TCacheControlFieldValue aField) const
	{
	TPtrC8 cacheControl;
	return FindCacheControlFieldValue(aField,cacheControl) != KErrNotFound;
	}


// Method to find a named field within the Cache Control header, 
// that returns an EPOC date/time structure.
//
// In:
//  aField		- the field type
//
// Out:
//  aTime - a structure containing the time (and date) found in the header
//			field
//
// Rtn: TBool - set to ETrue if the field was found, EFalse otherwise
//

EXPORT_C TBool CHTTPResponse::ExtractCacheControlTime(TCacheControlFieldValue aField,
											 TTime& aTime) const
	{
	__LOG_ENTER(_L("CHTTPResponse::ExtractCacheControlTime"));
	__ASSERT_DEBUG(aField == ECacheCtrlMaxAge || aField == ECacheCtrlMaxStale
						|| aField == ECacheCtrlMinFresh, User::Invariant());
	TBool result = EFalse;
	TPtrC8 cacheControl;
	aTime = 0;
	TInt index = FindCacheControlFieldValue(aField, cacheControl);
	if(index != KErrNotFound)
		{
		// Have the cache control and the field position
		// Now we need to extract the field's delta-secs value
		index++;
		TInt time = 0;
		TPtrC8 integerSource = cacheControl.Mid(index);

		if(integerSource[0] >= 0x80)			// Short integer value
			time = integerSource[0] & 0x7F;
		else								// Otherwise its multi octet
			ExtractMultiOctetInteger(time, integerSource);
		
		TTimeIntervalSeconds timeSeconds(time);
		aTime += timeSeconds;			// Store the seconds in the time field
		result = ETrue;
		}
	__LOG_RETURN;
	return result;
	}


// Method to search for the content type encoded in the response header
//
// Out:
//  aDesc - a pointer-type descriptor into the appropriate element of an
//			array prefilled with all the content types that have WSP
//			encodings. e.g. "text/vnd.wap.wml".  The contents of the
//			descriptor must NOT be modified.
//

EXPORT_C void CHTTPResponse::ContentType(TPtrC8& aDesc) const
    {
	// Decode the content-type data as per the WSP bnf...
	// Note - There is no provision available here to handle content-type parameters
	// so parameters are ignored here.
	TInt error = LocateField(EHttpContentType);
	TInt token = 0;
	TBool isString = EFalse;
	if (error != KErrNotFound)
		{
		TPtrC8 respChars(*iResponse);
		TWspPrimitiveDecoder wspDecoder(respChars);
		TWspPrimitiveDecoder::TWspHeaderType type = wspDecoder.VarType();
		switch(type)
			{
			case TWspPrimitiveDecoder::E7BitVal:
				{
				// 128-255 - encoded 7 bit value, this header has no more data
				TUint8 byteCode = 0;
				error = wspDecoder.Val7Bit(byteCode); // error code
				token = static_cast<TInt>(byteCode);
				} break;
			case TWspPrimitiveDecoder::EString:
				{
				// 32-127 - value is a text string, terminated by a '\0' 
				// Content type is embedded as a text string
				error = wspDecoder.String(aDesc); // error code
				isString = ETrue;
				} break;
			case TWspPrimitiveDecoder::ELengthVal:
				{
				// 0-31 -  octet is a value length
				TInt dataLength = 0;
				error = wspDecoder.LengthVal(dataLength);
				if( error >= KErrNone )
					{
					type = wspDecoder.VarType();
					if( type == TWspPrimitiveDecoder::E7BitVal || type == TWspPrimitiveDecoder::ELengthVal )
						{
						TUint32 contentTypeToken = 0;
						error = wspDecoder.Integer(contentTypeToken);
						token = static_cast<TInt>(contentTypeToken);
						}
					else if( type == TWspPrimitiveDecoder::EString )
						{
						error = wspDecoder.String(aDesc);
						isString = ETrue;
						}
					}

				} break;
			default:
				{
				error = KErrNotFound;
				} break;
			}
		}

	if(error < KErrNone)
		token = KErrNotFound;

	// Look up the appropriate content type, provided an error hasn't occurred or the string
	// has not already been set
	if (token == KErrNotFound || !isString)
		{
		// Convert the content type string to the supplied descriptor
		const TText8* type = ContentType(token);
		aDesc.Set(TPtrC8(type));
//		__LOG1(_L("CHTTPResponse::ContentType : contentIndex = %d"), contentIndex);
		}

	}


// Method to search for the realm encoded in the response header, when the
// response challenges the client for HTTP authentication (code 401)
//
// Out:
//  aDesc - a pointer-type descriptor into the response header buffer
//			positioned at the realm string within the challenge. The
//			contents of the descriptor must NOT be modified.
//
// Rtn: TBool - set to ETrue if a WWWAuthenticate header was found, EFalse
//				otherwise
//

EXPORT_C TBool CHTTPResponse::FindRealm(TPtrC8& aRealm) const
    {
	__LOG_ENTER(_L("CHTTPResponse::FindRealm"));
	// Search for the WWWAuthenticate field
	TPtrC8 realmPtr(aRealm);
    TBool retVal = FindField(EHttpWWWAuthenticate, realmPtr, 0);
    if (retVal)
        {
		// realmPtr now points to the WWWAuthentication field value. This contains the Authentication scheme, realm
		// value and optional parameters. Check authentication is Basic (encoded as 0x80). This is stored in the
		// second byte of the header value (i.e. index [1]).
		if (realmPtr[1] == 0x80)
			{
			// Set the realm descriptor passed in
            aRealm.Set(realmPtr.Mid(2));
#ifdef _DEBUG
			// In debug builds, convert the 8-bit realm to 16-bit UNICODE in order to log it.
			HBufC16* aRealm16 = HBufC16::New(aRealm.Length());
			if(aRealm16!=NULL)
				{
				TPtr16 aRealm16_Ptr = aRealm16->Des();
				aRealm16_Ptr.Copy(aRealm);
				__LOG1(_L("CHTTPResponse::FindRealm : found realm string: %S"), &aRealm16_Ptr);
				delete aRealm16;
				}
#endif
			}
		else
			{
			__LOG(_L("CHTTPResponse::FindRealm : nothing found"));
			retVal = EFalse;
			}
		}
	__LOG_RETURN;
    return retVal;
    }


// Method to search for the character set encoded in the Content-Type field of
// the response header
//
// Out:
//  aDesc - a pointer-type descriptor into the appropriate element of an
//			array prefilled with all the character sets that have WSP
//			encodings. e.g. "utf-8".  The contents of the descriptor must
//			NOT be modified.
//
// Rtn: TBool - set to ETrue if a character set was found, EFalse if not
//

EXPORT_C TBool CHTTPResponse::CharSet(TPtrC8& aDesc) const
    {
//	__LOG_ENTER(_L("CHTTPResponse::CharSet"));
	// Find the byte index in the header for the content type value
    TInt index = LocateField(EHttpContentType);

    TUint8 byteCode = 0;
	TInt paramByteCode = KErrNotFound;
	TInt valueByteCode1 = KErrNotFound;
	TInt charsetCode = 0;
	// Read the byte code, unless KErrNotFound was returned
	if (index != KErrNotFound)
		{
		TPtr8 respChars = iResponse->Des();
		TInt respLength = iResponse->Length();

		// If the byteCode is in the range 0-30 then a range of bytes is
		// indicated: the following byte gives the content type and the
		// remainder are arranged as a series of parameter attribute-value
		// pairs. This method checks for the presence of a 'charset' parameter.
		byteCode = respChars[index];
//		__LOG1(_L("CHTTPResponse::CharSet : found bytecode = %d"), byteCode);

		// Check valid range ... note that a range of zero could not contain a charset
		// parameter anyway, so exclude it...
		if ((byteCode > 0) && (byteCode <= 30))
			{
			// Check for overrun... if this occurs it should be an error.  Note that
			// corruption _could_ occur in this response buffer - some gateways, which
			// don't return error decks (e.g. AnyTime GW) send a response buffer 1 byte
			// long, containing only the value 0x01 - which is invalid WSP.
			// Be conservative and safe here - we can't overrun.  Use the value of byte-
			// -Code (which should be the WSP encoding of how many bytes follow), or the
			// total length of the response - whichever is smaller.
			if (index + byteCode < respLength)
				{
				// e,g, header to illustrate use of offsets in this code:
				// 03 94 81 84 : Content-Type: application/vnd.wap.wmlc; charset=iso-8859-1
				// +0 +1 +2 +3 : 03 = no. bytes in Content-Type header
				//			   : 94 = 14 | 80 = application/vnd.wap.wmlc
				//			   : 81 = 01 | 80 = Charset parameter
				//			   : 84 = 04 | 80 = iso-8859-1
				paramByteCode = respChars[index + 2];

				if ((paramByteCode & 0x7f) == EHttpCharset)
					{
					// We have a charset
					paramByteCode &= 0x7f;
					valueByteCode1 = respChars[index + 3];

					if (valueByteCode1 & 0x80)
						{
						// A short one-byte value
						charsetCode = valueByteCode1 & 0x7f;
						}
					else
						{
						// A multibyte value
						ExtractMultiOctetInteger(charsetCode, 
												 respChars.Mid(index + 3));
						}
					}
				}
			else
				{
				index = KErrNotFound;
				}
			}
		}

	// If a parameter-value pair was found, determine whether it encodes a
	// charset
	if ( (index != KErrNotFound) && (paramByteCode == EHttpCharset) )
		{
		// Look up the value from the charset table.
		const TText8* chset;
		chset = CharSet(charsetCode);

		// Convert the charset string to the supplied descriptor
		if (chset)
			aDesc.Set(TPtrC8(chset));
		else
			index = KErrNotFound; // We've found a charset but we don't recognise it
		}
	else	// Either no content-type header (hence no charset) or a content-type
			// header with a parameter other than charset
		{
		index = KErrNotFound;
		}

//	__LOG1(_L("CHTTPResponse::CharSet : CharSet = %S"), &aDesc);
//	__LOG_RETURN;
	return (index !=KErrNotFound);
    }


// Normal constructor - do non-allocating creation of this class
//

EXPORT_C CHTTPResponse::CHTTPResponse()
	: iStatusCode(EHttpUnknown), iDetailedStatusCode(EHttpUnknown)
    {
	// Does nothing here
    }


// Second phase construction - any allocation for this class must take place
// here. Sets up the resources required by an HTTP Response.
//

EXPORT_C void CHTTPResponse::ConstructL()
    {
    // Does nothing
	__OPEN_LOG(__LOG_WAP_FILE_NAME); 
    }


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

EXPORT_C TInt CHTTPResponse::LocateField(THttpHeaderField aField,
								TInt aStartIndex) const
    {
	// Content-Type is a special case; it appears to always be at the first
	// byte of the header, and doesn't have any encoding of the field name -
	// just straight into the Field Value at byte 0.  This is an assumption
	// however, since the WSP spec is not explicit - could it possibly be just
	// the NWSS GW's implementation of WSP that does this?
	if ( (aStartIndex == 0) && (aField == EHttpContentType) )
		{
		return aStartIndex; // the content-type field value position - ie. 0
		}
	
	// Deal with other Field Names, (Possibly including Content-Type if the
	// start index is offset into the header? Note that this is not likely to
	// occur though, with the abbreviated encoding.)
	TInt respLength = iResponse->Length();
	TPtr8 respChars  = iResponse->Des();
    for (TInt index = aStartIndex; index < respLength; index++)
        {
		// Examine the byte at this position in the header
        TUint8 byteCode = respChars[index];

		// Expect byteCode to be a Field Name code (unless the search is at
		// position zero, which has a missing content-type field name). Check
		// for the search field, remembering to clear the top bit
        if ( ( (byteCode & 0x7f) == aField) && (index != 0) )
            {
			// Got it - return the next position to locate the field value,
			// checking for potential overrun
			if (index < respLength - 1)
				{
				// Advance 1 to the header field value
				++index;
				return index;
				}
			else
				{
				return KErrNotFound;
				}
            }
        else
            {
			// Check that we aren't dealing with the Content-Type field
			// (expected at position 0), since it doesn't use a field type
			if (index != 0)
				{
				// WSP Spec Section 8.4.1.1 - Field Names
				//
				// If the byte is an alphanumeric, then it must be a field name that doesn't have
				// a WSP encoding.  In this circumstance, we can't handle the field, and must
				// therefore skip over it
				if ((byteCode >= 32) && (byteCode <= 127))
					{
					// Hit the start of a Header Name string - this will be assumed
					// continuous until the NUL is found or until the end
					// of the header is hit (which would be an error)
					while ( (respChars[index] != 0) && (index < respLength - 1) )
						++index;
					}

				// WSP Spec Section 8.4.1.2 - Field Values
				//
				// Now examine the field value by advancing one place.  If that advance takes us off
				// the end of the buffer, then (a) the WSP is invalid, and (b) the field is not found!
				++index;
				if (index == respLength)
					return KErrNotFound;
				}

			// Read the next byte at this position in the header
            byteCode = respChars[index];

			// Codes 0-30 represent that number of following data octets, so
			// they should be skipped
			if (byteCode == 0)			// 0 data octets follow !???? : (Strange but true)
				{
				// __DEBUGGER();
				}
            else if (byteCode <= 30)
				{
                index += byteCode;
				}
            else
                {
				// Code 31 indicates that the following bytes make a UIntVar,
				// which indicates the number of data octets after it. The
				// UIntVar itself could be composed of upto 5 bytes
                if (byteCode == 31)
					{
					// Copy a full 5 bytes from the header - note that actually
					// fewer might have been used; the UIntVar to Int
					// converter function returns the exact number that were
					// used.
					TInt value = 0;
					TInt consumed = ParseUIntVar(respChars.Mid(index + 1), value);
					
					if( consumed < KErrNone )
						return KErrCorrupt;

					// Advance to the last byte of data in this header
                    index += consumed + value;
					}
                else
					// Codes 32-127 are alphanumerics representing a text
					// string, up to a NUL termination
                    if (byteCode <= 127)
						// Hit the start of a string - this will be assumed
						// continuous until the NUL is found or until the end
						// of the header is hit (which would be an error)
                        while ( (respChars[index] != 0) && (index < respLength - 1) )
							++index;
                }
            }
        }

	// This return only occurs if the search ran off the end of the header
    return KErrNotFound;
    }


// Perform a look-up of content type given a WSP encoding value, used as
// an index.
//
// In:
//  aIndex - the WSP encoding value
//
// Rtn: const TText* - the required content type text - NOT to be changed
//

EXPORT_C const TText8* CHTTPResponse::ContentType(TInt aContentTypeCode) const
    {

    if ((aContentTypeCode >= 0) && (aContentTypeCode < KHttpNumContentTypes))
        return KHttpContentTypes[aContentTypeCode];
    else
        return defaultType;
    }


// Perform a look-up of character set given a WSP encoding value, used as
// an index.
//
// In:
//  aCharsetCode - the index into the content types table/
//
// Rtn: const TText8* - the required 8-bit character set text - NOT to be
//						changed by the caller
//

EXPORT_C const TText8* CHTTPResponse::CharSet(TInt aCharSetCode) const
    {
	// Search for an index for the supplied charset code
	TInt charSetIdx = KErrNotFound;
	for (TInt index = 0; ((index < KHttpNumCharacterSets) &&
									(charSetIdx == KErrNotFound)); index++)
		{
		if (KHttpCharacterSetCodes[index] == aCharSetCode)
			{
			charSetIdx = index;
			}
		}

	// If something was found, return the corresponding charset name
	if (charSetIdx != KErrNotFound)
		return KHttpCharacterSetNames[charSetIdx];
	else
		return NULL;
    }


// Do a conversion from 32-bit UIntVar encoding into 32-bit integer
//
TInt CHTTPResponse::ParseUIntVar(const TDesC8& aBuffer, TInt& aVal) const
	{
	// Is there any buffer?
	const TInt length = aBuffer.Length();
	if( length == 0 )
		return KErrCorrupt;

	// initialize return val
	aVal = 0;

	// maximum length for a uintvar is 5
	TInt lenLeft = Min(length, 5);

	// get the first octet
	TInt index = 0;
	TUint8 byte = aBuffer[index++];
	TInt numBytes = 1;

	--lenLeft;	

	// Check if any of the top 3 bits, ignoring the very top 'continue' bit, are set.  
	// Later if we see that this is a 5 byte number - we'll know it is corrupt.  
	// Encoding uses 7 bits/number 7x5=35 and we only support a maxiumum number 
	// of 32 bits.
	TBool topThreeBitsSet = byte & KTop3BitSet; 
	
	// copy over data from the byte into our return value (the top bit is a carry bit)
	aVal = byte & KWapQuote;

	// while the 'continue' bit is set and we have more data
	while ((byte & KCarryBitMask) && (lenLeft > 0))
		{
		// shift our last value up
		aVal <<= 7;
		// get the next byte
		byte = aBuffer[index++];
		// copy it over to the lowest byte
		aVal |= byte & KWapQuote;
		--lenLeft;
		++numBytes;
		} 

	// last octet has continue bit set ... NOT allowed Or
	// this was encoded wrong - can't have a number bigger than 32 bits
	if ((byte & KCarryBitMask) || (numBytes == 5 && topThreeBitsSet))
		return KErrCorrupt;

	// number of bytes read
	return numBytes;
	}


// Extract a WSP encoded MultiOctet Integer encoding into 32-bit integer
//
// In:
//  aSource	- the source Multi-Octet integer
//
// Out:
//  aInt		- the 32-bit resulting integer
//
void CHTTPResponse::ExtractMultiOctetInteger(TInt& aInt, const TPtrC8& aSource) const
 	// Extract a WSP encoded integer from the source descriptor
	{
	__LOG_ENTER(_L("CHTTPResponse::ExtractMultiOctetInteger"));
	// Get num bytes encoding the integer - 
	// we are positioned at that location in the source descriptor
	TUint8 numBytes = aSource[0];
	aInt = 0;
	if (numBytes <= 30)
 		{
 		__ASSERT_DEBUG(numBytes <= aSource.Length(), User::Invariant());
 		// Loop over the source, taking each byte and shifting it in to the count.
 		for (TInt count = 1; (count <= numBytes); count++)
 		        aInt = (aInt << 8) + aSource[count];
 		}
 	else if (numBytes & 0x80) //  check top bit is set
 		aInt = numBytes & 0x7f;
 	// anything between 30 and 127 is not handled...
	__LOG_RETURN;
	}


// Method to find a named field within the Cache Control header
//
// In:
//  aSource		- the descriptor containing the date value
//	aFrom		- The position in the descriptor to start from
//
// Out:
//  aTime - a structure containing the time (and date) found in the descriptor
//
// NOTE THIS METHOD WAS EXPORTED FOR TESTING OF THE CACHE. IT SHOULDN'T BE
// NOW, BUT CAN'T BE CHANGED SINCE IT WOULD AFFECT BC.
void CHTTPResponse::ExtractFieldDateValue(const TPtrC8& aSource,
										  TInt aFrom,
										  TTime& aTime) const
	{
	__LOG_ENTER(_L("CHTTPResponse::ExtractFieldDateValue"));
	// Get num bytes encoding the date - 
	// we are positioned at that location in the source descriptor
    TInt time = 0;
	TPtrC8 integerSource = aSource.Mid(aFrom);
	ExtractMultiOctetInteger(time, integerSource);
	// The WSP Date encoding is the number of seconds since the start of the
	// UNIX epoch (00:00:00.000, 01-Jan-1970), as a long integer
	TDateTime unixEpocDT(1970, EJanuary, 0, 0, 0, 0, 0);
	TTime unixEpoch(unixEpocDT);
    TTimeIntervalSeconds timeSeconds(time);
    aTime = unixEpoch + timeSeconds;
	__LOG_RETURN;
	}


// Method to find a named field within the Cache Control header
//
// In:
//  aField		- the field type
//
// Out:
//	the found aCacheControl string
//
// Rtn: TInt - set to KErrNotFound if the field was not found,
//		otherwise the position in the cache control descriptor that the field
//		was found
//
TInt CHTTPResponse::FindCacheControlFieldValue(TCacheControlFieldValue aField,
											   TPtrC8& aCacheControl) const
// Find a named field within the Cache Control header
	{
	__LOG_ENTER(_L("CHTTPResponse::FindCacheControlFieldValue"));
	TInt pos = KErrNotFound;
	TInt index = LocateField(EHttpCacheControl, 0);
    if (index >0)
        {
		// Have the cache control descriptor
		// Now we need to search for the field

		// The following rules are used to encode cache control values.
		// Cache-control-value	=	No-cache | No-store | Max-stale |
		//							Only-if-cached | Private | Public |
		//							No-transform | Must-revalidate | 
		//							Proxy-revalidate | Cache-extension | 
		//							Value-length Cache-directive
		// Cache-directive	=	No-cache 1*(Field-name) | 
		//						Max-age Delta-second-value |
		//						Max-stale Delta-second-value |
		//						Min-fresh Delta-second-value |
		//						Private 1*(Field-name) |
		//						Cache-extension Parameter
		TUint8 byteCode = iResponse->Des()[index];		// check the first byte for a recognised value
		if((byteCode >= 32) && (byteCode <= 127))
			{
			// Hit the start of a Header Name string - this will be assumed
			// continuous until the NUL is found or until the end
			// of the header is hit (which would be an error)
			// - not supported
			return pos;				
			}
		switch (byteCode)
			{
			case ECacheControlNoCache:		// "no-cache"
			case ECacheCtrlNoStore:			// "no-store"
			case ECacheCtrlMaxStale:		// "max-stale"
			case ECacheCtrlOnlyIfCached:	// "only-if-cached"
			case ECacheCtrlPublic:			// "public"
			case ECacheCtrlPrivate:			// "private"
			case ECacheCtrlNoTransform:		// "no-transform"
			case ECacheCtrlMustRevalidate:	// "must-revalidate"
			case ECacheCtrlProxyRevalidate:	// "proxy-revalidate"
				if( aField == byteCode )
					pos = index;			// Right here (right now).
				break;
			case ECacheCtrlCacheExtension:	// "cache-extension":
				break;						// Not handled
			default:
				{
			// Value-length Cache-directive
				if(FindBinaryDescField(EHttpCacheControl,aCacheControl))
					{
					TInt respLength = aCacheControl.Length();
					TUint8 byteCode = 0;
					for (TInt count = 0; count < respLength; count++)
						{
						byteCode = aCacheControl[count];
						if(aField == byteCode)
							{
							// Found the field we are looking for
							pos = count;
							break;
							}
						else if(count < (respLength - 1))	// Check for overrun... if this occurs it should be an error
							{
							if (byteCode <= 30)
								{
								// Codes 0-30 represent that number of following data
								// octets, check the cache directive field after the length
								if(aField == aCacheControl[count + 1])
									{
									// Found the one we want
									pos = count + 1;
									break;
									}
								else if(byteCode)
									{
									// so the following data octets should be skipped
									count += byteCode;
									}
								else
									{
									__DEBUGGER();
									count++;	// 0 data octets follow !???? : (Strange but true)
									}
								}
							else if (byteCode == 31)
								{
								// Code 31 indicates that the following bytes make a
								// UIntVar,  which indicates the number of data octets
								// after it. 
								// The UIntVar itself could be composed of upto 5 bytes
								// Copy a full 5 bytes from the header
								// Note that actually fewer might have been used; 
								// the UIntVar to Int converter function returns the exact
								// number that were used.
								TInt value = 0;
								TInt consumed = ParseUIntVar(aCacheControl.Mid(count + 1), value);
								
								if( consumed < KErrNone )
									return KErrCorrupt;
																
								if(aField == aCacheControl[count + 1 + consumed])
									{
									// Found the one we want
									pos = count + 1 + consumed;
									break;
									}
								else
									{
									// so the following data octets should be skipped
									count += 1 + consumed + value;
									}
								}
							}
						}
					}
				}
				break;
			}
		}
	__LOG_RETURN;
	return pos;
	}


// Panic method
//
// In:
//  aPanicCode - a standard HTTP panic code (see <HttpStd.h>)
//
void CHTTPResponse::Panic(THttpPanicCode aPanicCode) const
	{
	_LIT(KWapCHTTPResponse, "CHTTPResp");
	User::Panic(KWapCHTTPResponse, aPanicCode);
	}


#ifdef _DEBUG
// Debug method to dump to log the response header's binary content
//
void CHTTPResponse::DumpToLog(const TDesC8& aData) const
    {
//	__LOG_ENTER(_L("CHTTPResponse::DumpToLog"));

	// Iterate the supplied block of data in blocks of 16 bytes
	__LOG(_L("CHTTPResponse::DumpToLog : START"));
	TInt pos = 0;
	TBuf<KMaxLogEntrySize> logLine;
	TBuf<KMaxLogEntrySize> anEntry;
	while (pos < aData.Length())
		{
		anEntry.Format(TRefByValue<const TDesC>_L("%04x : "), pos);
		logLine.Append(anEntry);

		// Hex output
		for (TInt offset = 0; offset < 16; offset++)
			{
			if (pos + offset < aData.Length())
				{
				TInt nextByte = aData[pos + offset];
				anEntry.Format(TRefByValue<const TDesC>_L("%02x "), nextByte);
				logLine.Append(anEntry);
				}
			else
				{
				anEntry.Format(TRefByValue<const TDesC>_L("   "));
				logLine.Append(anEntry);
				}
			}
			anEntry.Format(TRefByValue<const TDesC>_L(": "));
			logLine.Append(anEntry);

		// Char output
		for (TInt offset = 0; offset < 16; offset++)
			{
			if (pos + offset < aData.Length())
				{
				TInt nextByte = aData[pos + offset];
				if ((nextByte >= 32) && (nextByte <= 127))
					{
					anEntry.Format(TRefByValue<const TDesC>_L("%c"), nextByte);
					logLine.Append(anEntry);
					}
				else
					{
					anEntry.Format(TRefByValue<const TDesC>_L("."));
					logLine.Append(anEntry);
					}
				}
			else
				{
				anEntry.Format(TRefByValue<const TDesC>_L(" "));
				logLine.Append(anEntry);
				}
			}
			__LOG1(_L("%S"), &logLine);
			logLine.Zero();

		// Advance to next 16 byte segment
		pos += 16;
		}
	__LOG(_L("CHTTPResponse::DumpToLog : END"));
//	__LOG_RETURN;
    }
#endif


// Spare methods for future BC. Const- and non-const versions to assist
// the caller in preserving const-ness. IMPORT_C ensures they reserve a
// slot in the vtbl, which is essential to preseve future BC.
//
EXPORT_C TAny* CHTTPResponse::Extend_CHTTPResponse(TAny* aArgs)
	{
	Panic(EHttpReservedForFutureExpansion);
	return (TAny*)aArgs;
	}
EXPORT_C TAny* CHTTPResponse::Extend_CHTTPResponse_const(TAny* aArgs) const
	{
	Panic(EHttpReservedForFutureExpansion);
	return (TAny*)aArgs;
	}
