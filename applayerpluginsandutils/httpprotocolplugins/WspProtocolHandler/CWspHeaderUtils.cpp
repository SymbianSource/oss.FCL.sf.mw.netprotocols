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

// System includes
#include <wspstringconstants.h>
#include <wspdecoder.h>
#include <wsperror.h>

// User includes
#include "wsppanic.h"

// Class signature
#include "cwspheaderutils.h"

// Constants used in this file
const TInt KDefaultSessionHeadersBufferSize		= 512;
const TInt KByteLength							= 1;

const TUint8 KNullTerminator					= 0x00;
const TUint8 KConvertToShortInt					= 0x80;

LOCAL_C void StringArrayCleanup(TAny* aStringArray);

CWspHeaderUtils* CWspHeaderUtils::NewL(CWspHeaderCodec& aCodec)
	{
	return new(ELeave) CWspHeaderUtils(aCodec);
	}

CWspHeaderUtils::CWspHeaderUtils(CWspHeaderCodec& aCodec)
	:iCodec(aCodec)
	{
	}

CWspHeaderUtils::~CWspHeaderUtils()
	{
	}

HBufC8* CWspHeaderUtils::EncodeHeadersL(RStringPool aStringPool, RHTTPHeaders aHeaders)
	{
	// Create a buffer for the encoded headers
	HBufC8* buf = HBufC8::NewL(KDefaultSessionHeadersBufferSize);
	CleanupStack::PushL(buf);

	// Encode the content-type header if it exists
	TBool hasContentType = EncodeContentTypeL(aStringPool, aHeaders, buf);

	// Go through the headers
	THTTPHdrFieldIter fields = aHeaders.Fields();
	fields.First();
	while( fields.AtEnd() == EFalse )
		{
		// Get the field value
		RStringF headerField = aStringPool.StringF(fields());

		// Was there a content-type header present? Make sure that it is not
		// appended a second time.
		TInt headerFieldValue = iCodec.EncodeFieldName(headerField);
		if( !hasContentType || headerFieldValue != WSP::EContentType )
			{
			// Encode the header field
			EncodeHeaderFieldL(aHeaders, headerField, buf);
			}
		// Next header field
		++fields;
		}
	// Cleanup...
	CleanupStack::Pop(buf);
	return buf;
	}

HBufC8* CWspHeaderUtils::EncodeNoTrailerHeadersL(RStringPool aStringPool, RHTTPHeaders aHeaders, HBufC8*& aTrailerData)
	{
	// Need an array of RStringFs for the headers in the trailers - need to put 
	// on the cleanup stack
	RArray<RStringF> trailerHeaders;
	TCleanupItem arrayCleaner = TCleanupItem(&StringArrayCleanup, (TAny*) &trailerHeaders);
	CleanupStack::PushL(arrayCleaner);

	// Need to find out what headers are in the trailers
	THTTPHdrVal trailerHeaderVal;
	TInt index =0;
	while( aHeaders.GetField(
							aStringPool.StringF(WSP::ETrailer, WSP::Table),
							index,		// Zero index -> first part
							trailerHeaderVal
							) != KErrNotFound )
		{
		__ASSERT_DEBUG( trailerHeaderVal.Type() == THTTPHdrVal::KStrFVal, Panic(KWspPanicBadTrailerHeader) );

		// Got a trailer header - append to the list
		RStringF header = trailerHeaderVal.StrF();
		User::LeaveIfError(trailerHeaders.Append(header));

		// Need to increment the reference count of this string
		header.Copy();

		// Next...
		++index;
		}
	__ASSERT_DEBUG( trailerHeaders.Count() > 0, Panic(KWspPanicNoTrailerHeaders) );

	// Create a buffer for the encoded headers
	HBufC8* bufHdrs = HBufC8::NewL(KDefaultSessionHeadersBufferSize);
	CleanupStack::PushL(bufHdrs);

	// Encode the content-type header if it exists
	TBool hasContentType = EncodeContentTypeL(aStringPool, aHeaders, bufHdrs);

	// Ok, now encode all the headers bar the ones in trailerHeaders
	// Go through the headers
	THTTPHdrFieldIter fields = aHeaders.Fields();
	fields.First();
	while( fields.AtEnd() == EFalse )
		{
		// Get the header field 
		RStringF headerField = aStringPool.StringF(fields());

		// Was there a content-type header present? Make sure that it is not
		// appended a second time, and only append if this is a trailer header.
		TInt headerFieldValue = iCodec.EncodeFieldName(headerField);
		if( !IsTrailerHeader(trailerHeaders, headerField) && 
			(!hasContentType || headerFieldValue != WSP::EContentType) )
			{
			// Encode the field
			EncodeHeaderFieldL(aHeaders, headerField, bufHdrs);
			}
		// Next header field
		++fields;
		}
	// Now, need to encode the trailer headers. Create a buffer for the trailer 
	// headers
	HBufC8* bufTrls = HBufC8::NewL(KDefaultSessionHeadersBufferSize);
	CleanupStack::PushL(bufTrls);

	TInt count = trailerHeaders.Count();
	for( TInt i=0; i < count; ++i)
		{
		// Encode the header field
		EncodeHeaderFieldL(aHeaders, trailerHeaders[i], bufTrls);
		}
	// Pass back the encoded trailer headers and other headers
	aTrailerData = bufTrls->ReAllocL(bufTrls->Des().Length());

	// Cleanup...
	CleanupStack::Pop(2, bufHdrs);
	CleanupStack::PopAndDestroy(&trailerHeaders);

	return bufHdrs;
	}

void CWspHeaderUtils::DecodeReplyHeadersL(RStringPool aStringPool, const TDesC8& aEncodedData, RHTTPHeaders& aHeaders)
	{
	// Ok, the data should have a content-type header at the start, but without
	// the header field name - straight onto the field value. The type of the 
	// first byte will give the length of the field value.
	TWspPrimitiveDecoder decoder = TWspPrimitiveDecoder(aEncodedData);

	TInt dataLength = 0;
	switch( decoder.VarType() )
		{
	case TWspPrimitiveDecoder::ELengthVal:
		{
		// The content-type header follows the BNF - 
		//
		// content-type-value = content-general-form = value-length media-type

		// The length of the data has been specified - extra it and include the
		// length of value-length
		TInt consumed = decoder.LengthVal(dataLength);
		dataLength += consumed;
		} break;
	case TWspPrimitiveDecoder::EString:
		{
		// The content-type header follows the BNF - 
		//
		// content-type-value = constrained-media = extension-media = *TEXT end-of-string
		
		// The end of the string is give by a NULL terminator
		TInt endPos = aEncodedData.Locate(KNullTerminator);
		if( endPos == KErrNotFound )
			{
			// The header data is corrupt
			User::Leave(KErrCorrupt);
			}
		dataLength = endPos + 1;
		} break;
	case TWspPrimitiveDecoder::E7BitVal:
		{
		// The content-type header follows the BNF - 
		//
		// content-type-value = constrained-media = short-integer = OCTET

		// The header data is a single byte long
		dataLength = KByteLength;
		} break;
	default:
		// The header data is corrupt
		User::Leave(KErrCorrupt);
		break;
		}
	// Set the content type field value
	TPtrC8 contentTypeValue = aEncodedData.Left(dataLength);

	// Set the raw data in the header object
	TBuf8<KByteLength> contentTypeToken;
	contentTypeToken.Append((TUint8)WSP::EContentType);
	aHeaders.SetRawFieldL(aStringPool.StringF(WSP::EContentType, WSP::Table), contentTypeValue, contentTypeToken);

	// Skip past the content-type field value
	TPtrC8 encodedData = aEncodedData.Mid(dataLength);

	// Segment the remaining data
	DecodeHeadersL(aStringPool, encodedData, aHeaders);
	}

void CWspHeaderUtils::DecodeHeadersL(RStringPool aStringPool, const TDesC8& aEncodedData, RHTTPHeaders& aHeaders)
	{
	// Use a segmentor to slice-up the data buffer
	TWspHeaderSegmenter segmentor = TWspHeaderSegmenter(aStringPool, WSP::Table, aEncodedData);

	// Slice..
	TBool done = EFalse;
	while( !done )
		{
		// Get the next field
		TWspField field;
		TInt error = segmentor.NextL(field);

		// Add the field name to the cleanup stack
		CleanupClosePushL(field.iHdrName);

		// Check for corrupt data
		if( error == KErrCorrupt )
			{
			// Data is corrupt
			User::Leave(KErrCorrupt);
			}

		// Was there a field found?
		if( error == KErrNotFound )
			{
			// No more data - stop
			done = ETrue;
			}
		else
			{
			// Set-up the field in the headers object
			TInt headerToken = field.iHdrName.Index(WSP::Table);
			if( headerToken == KErrNotFound )
				{
				// No token value so add as straight text with a NULL terminator
				HBufC8* tokenText = HBufC8::NewLC(field.iHdrName.DesC().Length() + 1);
				TPtr8 tokenTextBuffer(tokenText->Des());
				tokenTextBuffer.Append(KNullTerminator);
				aHeaders.SetRawFieldL(field.iHdrName, field.iValBuffer, *tokenText);
				CleanupStack::PopAndDestroy(tokenText);
				}
			else
				{
				// Encoded token available so use binary token as separator
				headerToken += KConvertToShortInt;
				TBuf8<KByteLength> headerTokenDes;
				headerTokenDes.Append((TUint8)headerToken);
				aHeaders.SetRawFieldL(field.iHdrName, field.iValBuffer, headerTokenDes);
				}
			}

		// Cleanup the field variable
		CleanupStack::PopAndDestroy(&field.iHdrName);

		}
	}

TBool CWspHeaderUtils::EncodeContentTypeL(RStringPool aStringPool, RHTTPHeaders aHeaders, HBufC8*& aBuf)
	{
	// Get the TPtr8 from the buffer
	TPtr8 encodedHdrs = aBuf->Des();

	// Get the content-type header, if it exists
	RStringF headerField = aStringPool.StringF(WSP::EContentType, WSP::Table);
	TPtrC8 encodedData;
	TInt foundContentType = aHeaders.GetRawField(headerField, encodedData);
	TBool hasContentType = foundContentType != KErrNotFound;

	// If the content-type header exists, append to the start of the buffer 
	// without the field name.
	if( hasContentType )
		{
		// Append the encoded field value - check for space first
		while( encodedHdrs.Length() + encodedData.Length() > encodedHdrs.MaxLength() )
			{
			aBuf = aBuf->ReAllocL(encodedHdrs.MaxLength() + KDefaultSessionHeadersBufferSize);
			encodedHdrs = aBuf->Des();
			}
		// Append the field value
		encodedHdrs.Append(encodedData);
		}
	return hasContentType;
	}

void CWspHeaderUtils::EncodeHeaderFieldL(RHTTPHeaders aHeaders, RStringF aHeaderField, HBufC8*& aBuf)
	{
	// Check to see if the header field is well-known
	TInt headerFieldValue = iCodec.EncodeFieldName(aHeaderField);

	TBool wellKnownHeader = headerFieldValue != KErrNotFound;

	// Calculate the header field length
	TInt fieldNameLength = KByteLength;
	if( !wellKnownHeader )
		{
		// The header is not well-known - need to encode the header field 
		// name as token text, which is NULL-terminated.
		fieldNameLength = aHeaderField.DesC().Length() + 1;
		}
	// Need to add the header field name and then the encoded header value.
	// First get the OTA format of the field value
	TPtrC8 encodedData;
	TInt err = aHeaders.GetRawField(aHeaderField, encodedData);

	// Was there any error encoding the header?
	if( err != KErrNone )
		{
		// Header isn't here - something has gone wrong
		User::Leave(KWspErrMissingHeader);
		}

	// Ensure there is enough space in the buffer.
	TPtr8 encodedHdrs = aBuf->Des();
	while( encodedHdrs.Length() + encodedData.Length() + fieldNameLength > encodedHdrs.MaxLength() )
		{
		aBuf = aBuf->ReAllocL(encodedHdrs.MaxLength() + KDefaultSessionHeadersBufferSize);
		encodedHdrs = aBuf->Des();
		}
	// Append the field name - check to see if well-known
	if( wellKnownHeader )
		{
		// Append as an encoded byte-value - the top bit must be set, since well-known
		// header field names are encoded as short-int ([WSP] Sect. 8.4.2.6 "Header")
		encodedHdrs.Append(headerFieldValue | KConvertToShortInt);
		}
	else
		{
		// Append as token text
		encodedHdrs.Append(aHeaderField.DesC());
		encodedHdrs.Append(KNullTerminator);
		}
	// Append the encoded field value
	encodedHdrs.Append(encodedData);
	}

TBool CWspHeaderUtils::IsTrailerHeader(RArray<RStringF> aTrailerHeaders, RStringF aHeaderField)
	{
	// Search the list of trailer headers to see if aHeaderField is in it.
	TInt index = 0;
	TInt count = aTrailerHeaders.Count();
	TBool found = EFalse;
	while( !found && index < count )
		{
		// Make the comparison
		RStringF header = aTrailerHeaders[index];
		found = header == aHeaderField;
		++index;
		}
	return found;
	}

LOCAL_C void StringArrayCleanup(TAny* aStringArray)
	{
	RArray<RStringF> array = *REINTERPRET_CAST(RArray<RStringF>*, aStringArray);

	TInt count = array.Count();
	for( TInt i = 0; i < count; ++i)
		{
		// Close the string
		array[i].Close();
		}
	array.Close();
	}
