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

// System Includes
#include <wspstringconstants.h>
#include <wspparamconstants.h>
#include <wspcontenttypes.h>
#include <wspregcontenttypes.h>
#include <wspcharactersets.h>
#include <wspstdconstants.h>
#include <wsplanguages.h>
#include <wsptypeconstants.h>

// User Includes
#include "cheaderfield.h"
#include "cwspheaderreader.h"

// Constants
const TInt KMaxNumQDigits = 5;	// Maximum number of digits for a 'Q' value
const TUint32 KRegisteredContentTypesOffset = 0x0201; // Registered content types offset
const TUint8 KTopBitMask = 0x80; // Bit-wise mask for setting top bit to 1

CWspHeaderReader::~CWspHeaderReader()
	{
	}

CWspHeaderReader* CWspHeaderReader::NewL(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec)
	{
	return new(ELeave)CWspHeaderReader(aStrPool, aStrTable, aCodec);
	}

CWspHeaderReader* CWspHeaderReader::NewLC(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec)
	{
	CWspHeaderReader* self = CWspHeaderReader::NewL(aStrPool, aStrTable, aCodec);
	CleanupStack::PushL(self);
	return self;
	}

CWspHeaderReader::CWspHeaderReader(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec)
	: iStrPool(aStrPool), iStrTable(aStrTable), iCodec(aCodec)
	{
	}

void CWspHeaderReader::DecodeHeaderL(RHeaderField& aHeader)
	{
	switch( aHeader.Name().Index(iStrTable) )
		{
		case WSP::EContentType:
			DecodeContentTypeL(aHeader);
			break;
		case WSP::EDate:
		case WSP::ELastModified:
			DecodeGenericDateValueL(aHeader);
			break;
		case WSP::EContentLocation:
		case WSP::ELocation:
		case WSP::EServer:
		case WSP::EVia:
		case WSP::EXWapInitiatorURI:
		case WSP::EUpgrade:
			DecodeGenericNewStringValueL(aHeader);
			break;
		case WSP::EPragma:
			DecodePragmaL(aHeader);
			break;
		case WSP::EVary:
		case WSP::ETrailer:
			DecodeGenericFieldNameL(aHeader);
			break;
		case WSP::EWWWAuthenticate:
		case WSP::EProxyAuthenticate:
			DecodeGenericChallengeL(aHeader);
			break;
		case WSP::ESetCookie:
			DecodeSetCookieL(aHeader);
			break;
		case WSP::EEncodingVersion:
			DecodeEncodingVersionL(aHeader);
			break;
		case WSP::EAcceptRanges:
			DecodeAcceptRangesL(aHeader);
			break;
		case WSP::EContentEncoding:
			DecodeContentEncodingL(aHeader);
			break;
		case WSP::EContentLanguage:
			DecodeContentLanguageL(aHeader);
			break;
		case WSP::EContentMD5:
			DecodeContentMD5L(aHeader);
			break;
		case WSP::ERetryAfter:
			DecodeRetryAfterL(aHeader);
			break;
		case WSP::EContentRange:
		case WSP::EContentRangeDep:
			DecodeContentRangeL(aHeader);
			break;
		case WSP::EXWapApplicationId:
			DecodeXWapApplicationIdL(aHeader);
			break;
		case WSP::EPushFlag:
			DecodePushFlagL(aHeader);
			break;
		case WSP::EAllow:
			DecodeAllowL(aHeader);
			break;
		case WSP::EWarning:
			DecodeWarningL(aHeader);
			break;
		case WSP::EProfileWarning:
		case WSP::EProfileWarningDep:
			DecodeProfileWarningL(aHeader);
			break;
		default:
			User::Leave(KErrNotSupported);
			break;
		}
	}


void CWspHeaderReader::DecodeContentTypeL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);

	if( !(CheckNullDesPartL(aHeader, rawData, 0)) )
		{
		TWspPrimitiveDecoder wspDecoder(rawData);
		switch( wspDecoder.VarType() )
			{
			case TWspPrimitiveDecoder::E7BitVal:
				{
				TUint8 contentTypeToken = 0;
				User::LeaveIfError(wspDecoder.Val7Bit(contentTypeToken));
				RStringF contentType = iStrPool.StringF(contentTypeToken, WSPContentTypes::Table);
				SetFStringPartL(aHeader, 0, contentType);
				} break;
			case TWspPrimitiveDecoder::EString:
				{
				AddNewDecoderStringPartL(aHeader, wspDecoder);
				} break;
			case TWspPrimitiveDecoder::ELengthVal:
				{
				TInt dataLength = 0;
				TInt err = wspDecoder.LengthVal(dataLength);
				User::LeaveIfError(err);
				TInt bytesProcessed = err;
				TWspPrimitiveDecoder::TWspHeaderType type = wspDecoder.VarType();
				CHeaderFieldPart* fieldPart = NULL;
				if( type == TWspPrimitiveDecoder::E7BitVal || type == TWspPrimitiveDecoder::ELengthVal )
					{
					TUint32 contentTypeToken = 0;
					err = wspDecoder.Integer(contentTypeToken);
					User::LeaveIfError(err);
					bytesProcessed += err;
					RStringF contentTypeStr;
					if( contentTypeToken < KRegisteredContentTypesOffset )
						contentTypeStr = iStrPool.StringF(contentTypeToken, WSPContentTypes::Table);
					else
						contentTypeStr = iStrPool.StringF((contentTypeToken-KRegisteredContentTypesOffset), WSPRegContentTypes::Table);
					
					fieldPart = &(SetFStringPartL(aHeader, 0, contentTypeStr));
					}
				else if( type == TWspPrimitiveDecoder::EString )
					{
					TPtrC8 contentType;
					err = wspDecoder.String(contentType);
					User::LeaveIfError(err);
					bytesProcessed += err;
					fieldPart = &(SetNewFStringPartL(aHeader, 0, contentType));
					}
				else
					User::Leave(KErrCorrupt);

				// Check if paramters exist
				while( bytesProcessed < rawData.Length() )
					{
					TPtrC8 parameterBlock(rawData.Mid(bytesProcessed));
					bytesProcessed += DecodeGenericSingleParameterL(parameterBlock, *fieldPart);
					}
				} break;
			default:
				User::Leave(KErrCorrupt);
			}
		}
	}

void CWspHeaderReader::DecodePragmaL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);

	// Check for <Octet 128> which indicates "No-Cache" string
	if( rawData[0] == 128 )
		{
		SetFStringPartL(aHeader, 0, iStrPool.StringF(WSPStdConstants::ENoCache, WSPStdConstants::Table));
		}
	else
		{
		TWspPrimitiveDecoder wspDecoder(rawData);
		if( wspDecoder.VarType() == TWspPrimitiveDecoder::ELengthVal )
			{
			TInt dataLength = 0;
			TInt bytesProcessed = wspDecoder.LengthVal(dataLength);
			User::LeaveIfError(bytesProcessed);
			TPtrC8 rawParamBlock(rawData.Mid(bytesProcessed));
			// Check that there is only one parameter block
			if( dataLength != rawParamBlock.Length() )
				User::Leave(KErrCorrupt);
			CHeaderFieldPart& fieldPart = SetNewFStringPartL(aHeader, 0, KNullDesC8());
			bytesProcessed += DecodeGenericSingleParameterL(rawParamBlock, fieldPart);
			if( bytesProcessed != rawData.Length() )
				User::Leave(KErrCorrupt);
			}
		}
	}

void CWspHeaderReader::DecodeSetCookieL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 2);
	TWspPrimitiveDecoder wspDecoder(rawData);
	if( wspDecoder.VarType() != TWspPrimitiveDecoder::ELengthVal )
		User::Leave(KErrCorrupt);
	TInt valueLength = 0;
	TInt bytesProcessed = wspDecoder.LengthVal(valueLength);
	User::LeaveIfError(bytesProcessed);

	// Get the first part which is the cookie-version
	TInt err = 0;
	if( CheckForNullStringL(rawData, bytesProcessed, wspDecoder) )
		{
		SetNewFStringPartL(aHeader, 0, KNullDesC8());
		}
	else
		{
		RStringF version;
		err = wspDecoder.VersionL(iStrPool, version);
		User::LeaveIfError(err);
		bytesProcessed += err;
		CleanupClosePushL(version);
		SetFStringPartL(aHeader, 0, version);
		CleanupStack::PopAndDestroy(&version);
		}

	// Loop through the next two 
	// Get the second part which is the cookie-name
	// Get the third part which is the cookie-value
	CHeaderFieldPart* fieldPart = NULL;
	for(TInt ii=1; ii<3; ++ii)
		{
		TPtrC8 cookiePart(KNullDesC8());
		if( !(CheckForNullStringL(rawData, bytesProcessed, wspDecoder)) )
			{
			err = wspDecoder.String(cookiePart);
			User::LeaveIfError(err);
			bytesProcessed += err;
			}
		fieldPart = &(SetNewFStringPartL(aHeader, ii, cookiePart));
		}

	// Check if there are any parameters. if so add them to the 3rd part
	if( fieldPart != NULL ) 
		{
		while( bytesProcessed < rawData.Length() )
			{
			TPtrC8 parameterBlock(rawData.Mid(bytesProcessed));
			bytesProcessed += DecodeGenericSingleParameterL(parameterBlock, *fieldPart);
			}
		}
	}

TBool CWspHeaderReader::CheckForNullStringL(TPtrC8& aRawData, TInt& aBytesRead,
											TWspPrimitiveDecoder& aDecoder) const
	{
	// Check that the buffer is not out of range
	if( aBytesRead >= aRawData.Length() )
		User::Leave(KErrCorrupt);

	// Check if the data at the indicated position is a NULL terminator
	if( aRawData[aBytesRead] == 0 )
		{
		TUint8 value = 0;
		TInt err = aDecoder.Val7Bit(value);
		User::LeaveIfError(err);
		aBytesRead += err;
		return ETrue;
		}
	return EFalse;
	}

void CWspHeaderReader::DecodeEncodingVersionL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);
	TWspPrimitiveDecoder wspDecoder(rawData);
	
	if( !(CheckNullDesPartL(aHeader, rawData, 0)) )
		{
		// Check type and decode accordingly
		switch( wspDecoder.VarType() )
			{
			case TWspPrimitiveDecoder::ELengthVal:
				{
				TInt valLength = 0;
				TInt bytesRead = 0;
				TInt err = wspDecoder.LengthVal(valLength);
				User::LeaveIfError(err);
				bytesRead += err;
				if( wspDecoder.VarType() == TWspPrimitiveDecoder::E7BitVal )
					{
					TUint8 codePage = 0;
					err = wspDecoder.Val7Bit(codePage);
					User::LeaveIfError(err);
					bytesRead += err;
					SetNewIntegerPartL(aHeader, 0, codePage);
					if( bytesRead < rawData.Length() )
						{
						// Check for Null string first
						if( rawData[bytesRead] == 0 )
							SetNewFStringPartL(aHeader, 1, KNullDesC8());
						else
							DecodeGenericVersionL(aHeader, wspDecoder, 1);
						}
					}
				else
					User::Leave(KErrCorrupt);
				} break;
			case TWspPrimitiveDecoder::EString:
			case TWspPrimitiveDecoder::E7BitVal:
				{
				DecodeGenericVersionL(aHeader, wspDecoder, 0);
				} break;
			default:
				User::Leave(KErrCorrupt);
				break;
			}
		}
	}

void CWspHeaderReader::DecodeGenericVersionL(RHeaderField& aHeader, TWspPrimitiveDecoder& aDecoder, TInt aIndex) const
	{
	RStringF versionValue;
	CleanupClosePushL(versionValue);
	User::LeaveIfError(aDecoder.VersionL(iStrPool, versionValue));
	SetFStringPartL(aHeader, aIndex, versionValue);
	CleanupStack::PopAndDestroy(&versionValue);
	}

void CWspHeaderReader::DecodeAcceptRangesL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);
	TInt acceptRangeIndex = KErrNotFound;
	switch( rawData[0] )
		{
		case 128: // This is the token for 'None'
			{
			acceptRangeIndex = WSPStdConstants::ENone;
			} break;
		case 129: // This is the token for 'Bytes'
			{
			acceptRangeIndex = WSPStdConstants::EBytes;
			} break;
		default: // Must be token text
			{
			TWspPrimitiveDecoder wspDecoder(rawData);
			// Check that the data is of string type
			if( wspDecoder.VarType() == TWspPrimitiveDecoder::EString )
				{
				// Extract and add the string as a part
				AddNewDecoderStringPartL(aHeader, wspDecoder);
				}
			else
				User::Leave(KErrCorrupt);
			} break;
		}

	if(acceptRangeIndex!=KErrNotFound)
		SetFStringPartL(aHeader, 0, iStrPool.StringF(acceptRangeIndex, WSPStdConstants::Table));
	}

void CWspHeaderReader::DecodeContentEncodingL(RHeaderField& aHeader) const
	{
	TPtrC8 rawHeaderData;
	aHeader.RawDataL(rawHeaderData);
	TInt startPos = 0;
	TInt separatorPos = 0;
	TInt ii = 0;
	// Loop through all the parts separated by the header field token name
	do
		{
		TPtrC8 rawData(rawHeaderData.Mid(startPos));
		separatorPos = rawData.Locate((TUint8)(WSP::EContentEncoding + KTopBitMask));
		if(separatorPos!=KErrNotFound)
			rawData.Set(rawHeaderData.Mid(startPos, separatorPos));

		CheckLengthL(rawData, 1);
		TInt contentEncodingIndex = KErrNotFound;
		switch( rawData[0] )
			{
			case 128: // This is the token for 'GZip'
				{
				contentEncodingIndex = WSPStdConstants::EGzip;
				} break;
			case 129: // This is the token for 'Compress'
				{
				contentEncodingIndex = WSPStdConstants::ECompress;
				} break;
			case 130: // This is the token for 'Deflate'
				{
				contentEncodingIndex = WSPStdConstants::EDeflate;
				} break;
			default: // Must be token text
				{
				TWspPrimitiveDecoder wspDecoder(rawData);
				// Check that the data is of string type
				if( wspDecoder.VarType() == TWspPrimitiveDecoder::EString )
					{
					// Extract and add the string as a part
					AddNewDecoderStringPartL(aHeader, wspDecoder, ii);
					}
				else
					User::Leave(KErrCorrupt);
				} break;
			}

		if(contentEncodingIndex!=KErrNotFound)
			SetFStringPartL(aHeader, ii, iStrPool.StringF(contentEncodingIndex, WSPStdConstants::Table));
		++ii;
		startPos += (separatorPos + 1);
		} while( separatorPos != KErrNotFound );
	}

void CWspHeaderReader::DecodeContentLanguageL(RHeaderField& aHeader) const
	{
	TPtrC8 rawHeaderData;
	aHeader.RawDataL(rawHeaderData);
	TInt startPos = 0;
	TInt separatorPos = 0;
	TInt ii = 0;
	// Loop through all the parts separated by the header field name
	do
		{
		TPtrC8 rawData(rawHeaderData.Mid(startPos));
		separatorPos = rawData.Locate((TUint8)(WSP::EContentLanguage + KTopBitMask));
		if(separatorPos!=KErrNotFound)
			rawData.Set(rawHeaderData.Mid(startPos, separatorPos));

		CheckLengthL(rawData, 1);
		// First check for any language ('*') encoded as octet 128
		if( rawData[0] == 128 )
			SetFStringPartL(aHeader, ii, iStrPool.StringF(WSPLanguages::EAnyLanguage, WSPLanguages::Table));
		else
			{
			// Otherwise the language is encoded as a short int, long int or token text
			TWspPrimitiveDecoder wspDecoder(rawData);
			switch( wspDecoder.VarType() )
				{
				case TWspPrimitiveDecoder::E7BitVal: // short int
				case TWspPrimitiveDecoder::ELengthVal: // long int
					{
					TUint32 languageToken = 0;
					User::LeaveIfError(wspDecoder.Integer(languageToken));
					// Check if the language token is short or long int as long requires an offset
					if( languageToken >= 128 )
						// Language token is long int so apply the offset
						--languageToken;
					SetFStringPartL(aHeader, ii, iStrPool.StringF(languageToken, WSPLanguages::Table));
					} break;
				case TWspPrimitiveDecoder::EString: // token text
					{
					// Extract the token text and set the part
					AddNewDecoderStringPartL(aHeader, wspDecoder, ii);
					} break;
				default:
					User::Leave(KErrCorrupt);
					break;
				}
			}

		++ii;
		startPos += (separatorPos + 1);
		} while( separatorPos != KErrNotFound );
	}

void CWspHeaderReader::DecodeContentMD5L(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);
	TWspPrimitiveDecoder wspDecoder(rawData);
	if( wspDecoder.VarType() == TWspPrimitiveDecoder::ELengthVal )
		{
		TInt length = 0;
		TInt bytesRead = wspDecoder.LengthVal(length);
		User::LeaveIfError(bytesRead);
		TPtrC8 md5Data(rawData.Mid(bytesRead));
		if( md5Data.Length() != length )
			User::Leave(KErrCorrupt);

		SetNewStringPartL(aHeader, 0, md5Data);
		}
	else
		User::Leave(KErrCorrupt);
	}

void CWspHeaderReader::DecodeRetryAfterL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);
	TWspPrimitiveDecoder wspDecoder(rawData);
	if( wspDecoder.VarType() == TWspPrimitiveDecoder::ELengthVal )
		{
		TInt lengthVal = 0;
		TInt bytesRead = wspDecoder.LengthVal(lengthVal);
		User::LeaveIfError(bytesRead);
		switch( rawData[bytesRead] )
			{
			case 128: // This is an absolute time
				{
				TUint8 updateOffset = 0;
				User::LeaveIfError(wspDecoder.Val7Bit(updateOffset));
				TDateTime dateTime(1970, EJanuary, 0, 0, 0, 0, 0);
				User::LeaveIfError(wspDecoder.Date(dateTime));
				SetNewDatePartL(aHeader, 0, dateTime);
				} break;
			case 129: // This is a relative time
				{
				TUint8 updateOffset = 0;
				User::LeaveIfError(wspDecoder.Val7Bit(updateOffset));
				TUint32 relativeTime = 0;
				User::LeaveIfError(wspDecoder.Integer(relativeTime));
				SetNewIntegerPartL(aHeader, 0, relativeTime);
				} break;
			default:
				User::Leave(KErrCorrupt);
				break;
			}
		}
	else
		User::Leave(KErrCorrupt);
	}

void CWspHeaderReader::DecodeContentRangeL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);
	TWspPrimitiveDecoder wspDecoder(rawData);
	if( wspDecoder.VarType() == TWspPrimitiveDecoder::ELengthVal )
		{
		TInt lengthVal = 0;
		TInt bytesRead = wspDecoder.LengthVal(lengthVal);
		User::LeaveIfError(bytesRead);
		
		// Get the first part, first byte pos
		TUint32 uintvarValue = 0;
		TInt err = wspDecoder.UintVar(uintvarValue);
		User::LeaveIfError(err);
		bytesRead += err;
		SetNewIntegerPartL(aHeader, 0, uintvarValue);

		// Get the second part, entity length
		// Check for '*' token <octet 128> and the encoding version is greater than v1.2
		if( (rawData[bytesRead]) == 128 && (iCodec.GetWspVersion() > CWspHeaderCodec::EVersion1_2) )
			{
			RStringF unknownLenStr = iStrPool.StringF(WSPStdConstants::EAny, WSPStdConstants::Table);
			SetFStringPartL(aHeader, 1, unknownLenStr);
			}
		else
			{
			User::LeaveIfError(wspDecoder.UintVar(uintvarValue));
			SetNewIntegerPartL(aHeader, 1, uintvarValue);
			}
		}
	else
		User::Leave(KErrCorrupt);
	}

void CWspHeaderReader::DecodeXWapApplicationIdL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);
	TWspPrimitiveDecoder wspDecoder(rawData);
	switch( wspDecoder.VarType() )
			{
			case TWspPrimitiveDecoder::E7BitVal:
			case TWspPrimitiveDecoder::ELengthVal:
				{
				if( rawData[0] != 0 )
					{
					TUint32 applicationID = 0;
					User::LeaveIfError(wspDecoder.Integer(applicationID));
					SetNewIntegerPartL(aHeader, 0, applicationID);
					}
				} break;
			default:
				DecodeGenericNewStringValueL(aHeader);
				break;
			}
	}

void CWspHeaderReader::DecodePushFlagL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);
	TWspPrimitiveDecoder wspDecoder(rawData);
	// Check that the value is a short integer
	if( wspDecoder.VarType() == TWspPrimitiveDecoder::E7BitVal )
		{
		TUint8 pushFlagVal = 0;
		User::LeaveIfError(wspDecoder.Val7Bit(pushFlagVal));
		SetNewIntegerPartL(aHeader, 0, pushFlagVal);
		}
	else
		User::Leave(KErrCorrupt);
	}

void CWspHeaderReader::DecodeAllowL(RHeaderField& aHeader) const
	{
	TPtrC8 rawHeaderData;
	aHeader.RawDataL(rawHeaderData);
	TInt startPos = 0;
	TInt separatorPos = 0;
	TInt ii = 0;
	// Loop through all the parts separated by the header field name
	do
		{
		TPtrC8 rawData(rawHeaderData.Mid(startPos));
		separatorPos = rawData.Locate((TUint8)(WSP::EAllow + KTopBitMask));
		if(separatorPos!=KErrNotFound)
			rawData.Set(rawHeaderData.Mid(startPos, separatorPos));
		
		CheckLengthL(rawData, 1);
		TWspPrimitiveDecoder wspDecoder(rawData);
		// Check that the value is a short integer
		if( wspDecoder.VarType() == TWspPrimitiveDecoder::E7BitVal )
			{
			TUint8 pushFlagVal = 0;
			User::LeaveIfError(wspDecoder.Val7Bit(pushFlagVal));
			// Get the correct string for the integer value from the WSP Type Constants table
			TInt stringTableIndex = KErrNotFound;
			switch(pushFlagVal)
				{
				case 0x40: // Get
					stringTableIndex = WSPTypeConstants::EGet;
					break;
				case 0x41: // Options
					stringTableIndex = WSPTypeConstants::EOptions;
					break;
				case 0x42: // Head
					stringTableIndex = WSPTypeConstants::EHead;
					break;
				case 0x43: // Delete
					stringTableIndex = WSPTypeConstants::EDelete;
					break;
				case 0x44: // Trace
					stringTableIndex = WSPTypeConstants::ETrace;
					break;
				case 0x60: // Post
					stringTableIndex = WSPTypeConstants::EPost;
					break;
				case 0x61: // Put
					stringTableIndex = WSPTypeConstants::EPut;
					break;
				default: // Not found so leave
					User::Leave(KErrNotSupported);
					break;
				}
			RStringF allowValue = iStrPool.StringF(stringTableIndex, WSPTypeConstants::Table);
			SetFStringPartL(aHeader, ii, allowValue);
			}
		else
			User::Leave(KErrCorrupt);
		
		++ii;
		startPos += (separatorPos + 1);
		} while( separatorPos != KErrNotFound );
	}

void CWspHeaderReader::DecodeWarningL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);
	TWspPrimitiveDecoder wspDecoder(rawData);
	switch( wspDecoder.VarType() )
		{
		case TWspPrimitiveDecoder::E7BitVal: // This is a short int warn code
			{
			DecodeGenericWarnCodeL(aHeader, wspDecoder);
			} break;
		case TWspPrimitiveDecoder::ELengthVal: // This is value-length warning value
			{
			// Get the first part - warn-code (short int)
			TInt length = 0;
			TInt bytesProcessed = wspDecoder.LengthVal(length);
			User::LeaveIfError(bytesProcessed);
			DecodeGenericWarnCodeL(aHeader, wspDecoder);
			++bytesProcessed;
			
			// Get the second part - warn-agent (text string)
			if(CheckForNullStringL(rawData, bytesProcessed, wspDecoder))
				SetNewFStringPartL(aHeader, 1, KNullDesC8());
			else
				bytesProcessed += AddNewDecoderStringPartL(aHeader, wspDecoder, 1);

			// Get the third part - warn-text (text string)
			if(CheckForNullStringL(rawData, bytesProcessed, wspDecoder))
				SetNewFStringPartL(aHeader, 2, KNullDesC8());
			else
				{
				AddNewDecoderStringPartL(aHeader, wspDecoder, 2);
				}
			} break;
		default:
			User::LeaveIfError(KErrCorrupt);
			break;
		}
	}
	
void CWspHeaderReader::DecodeProfileWarningL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);
	TWspPrimitiveDecoder wspDecoder(rawData);
	switch (wspDecoder.VarType())
		{
		case TWspPrimitiveDecoder::E7BitVal: // This is a short int profile warn-code
			{
			DecodeGenericWarnCodeL(aHeader, wspDecoder);
			} break;
		case TWspPrimitiveDecoder::ELengthVal:
			{
			TInt length = 0;
			TInt bytesRead = wspDecoder.LengthVal(length);
			User::LeaveIfError(bytesRead);
			
			// Get the first part - profile warn-code (short int)
			DecodeGenericWarnCodeL(aHeader, wspDecoder);
			++bytesRead;
			
			// Get the second part - warn-target
			if(CheckForNullStringL(rawData, bytesRead, wspDecoder))
				SetNewFStringPartL(aHeader, 1, KNullDesC8());
			else
				bytesRead += AddNewDecoderStringPartL(aHeader, wspDecoder, 1);

			// Get the warn-date if it exists
			if( bytesRead < rawData.Length() )
				{
				TDateTime dateTime(1970, EJanuary, 0, 0, 0, 0, 0);
				User::LeaveIfError(wspDecoder.Date(dateTime));
				SetNewDatePartL(aHeader, 2, dateTime);
				}
			} break;
		default:
			User::Leave(KErrCorrupt);
			break;
		}
	}


// Generic encoding methods

void CWspHeaderReader::DecodeGenericWarnCodeL(RHeaderField& aHeader, TWspPrimitiveDecoder& aDecoder) const
	{
	TUint8 warnCode = 0;
	User::LeaveIfError(aDecoder.Val7Bit(warnCode));
	// Convert the code
	TInt convertedCode = 0;
	switch( warnCode )
		{
		case 10: // 110 - Response stale
		case 11: // 111 - Revalidation failed
		case 12: // 112 - Disconnected operation
		case 13: // 113 - Heuristic expiration
		case 99: // 199 - Miscellaneous warning
			convertedCode = warnCode + 100;
			break;
		case 14: // 214 - Tranformation applied
			convertedCode = warnCode + 200;
			break;
		// The following warning codes are UAProf specific
		case 16: // 100 - OK
		case 17: // 101 - Used stale profile
		case 18: // 102 - Not used profile
			convertedCode = warnCode + 84;
			break;
		case 32: // 200 - Not applied
		case 33: // 201 - Content selection applied
		case 34: // 202 - Content generation applied
		case 35: // 203 - Transformation applied
			convertedCode = warnCode + 168;
			break;
		default:
			convertedCode = warnCode;
			break;
		}
	// Add the value as an integer part
	SetNewIntegerPartL(aHeader, 0, convertedCode);
	}
	
void CWspHeaderReader::DecodeGenericNewStringValueL(RHeaderField& aHeader) const
	{
	TPtrC8 rawHeaderData;
	aHeader.RawDataL(rawHeaderData);

	TInt startPos = 0;
	TInt separatorPos = 0;
	TInt ii = 0;
	// Loop through all the parts separated by the header field name
	do
		{
		TPtrC8 rawData(rawHeaderData.Mid(startPos));
		separatorPos = rawData.Locate((TUint8)(aHeader.Name().Index(WSP::Table) + KTopBitMask));
		if(separatorPos!=KErrNotFound)
			rawData.Set(rawHeaderData.Mid(startPos, separatorPos));

		CheckLengthL(rawData, 1);
		TWspPrimitiveDecoder wspDecoder(rawData);
		if( !(CheckNullDesPartL(aHeader, rawData, ii)) )
			{
			AddNewDecoderStringPartL(aHeader, wspDecoder, ii);
			}

		++ii;
		startPos += (separatorPos + 1);
		} while( separatorPos != KErrNotFound );
	}

void CWspHeaderReader::DecodeGenericDateValueL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 1);
	TWspPrimitiveDecoder wspDecoder(rawData);
	if( wspDecoder.VarType() == TWspPrimitiveDecoder::ELengthVal )
		{
		TDateTime dateTime(1970, EJanuary, 0, 0, 0, 0, 0);
		User::LeaveIfError(wspDecoder.Date(dateTime));
		SetNewDatePartL(aHeader, 0, dateTime);
		}
	else
		User::Leave(KErrCorrupt);
	}

void CWspHeaderReader::DecodeGenericFieldNameL(RHeaderField& aHeader) const
	{
	TPtrC8 rawHeaderData;
	aHeader.RawDataL(rawHeaderData);
	TInt startPos = 0;
	TInt separatorPos = 0;
	TInt ii = 0;
	// Loop through all the parts separated by the header field name
	do
		{
		TPtrC8 rawData(rawHeaderData.Mid(startPos));
		separatorPos = rawData.Locate((TUint8)(aHeader.Name().Index(WSP::Table) + KTopBitMask));
		if(separatorPos!=KErrNotFound)
			rawData.Set(rawHeaderData.Mid(startPos, separatorPos));

		CheckLengthL(rawData, 1);
		TWspPrimitiveDecoder wspDecoder(rawData);
		switch( wspDecoder.VarType() )
			{
			case TWspPrimitiveDecoder::EString:
				{
				AddNewDecoderStringPartL(aHeader, wspDecoder, ii);
				} break;
			case TWspPrimitiveDecoder::E7BitVal:
				{
				TUint8 fieldNameToken = 0;
				User::LeaveIfError(wspDecoder.Val7Bit(fieldNameToken));
				RStringF fieldNameStr = iStrPool.StringF(fieldNameToken, iStrTable);
				SetFStringPartL(aHeader, ii, fieldNameStr);
				} break;
			default:
				User::Leave(KErrCorrupt);
				break;
			}

		++ii;
		startPos += (separatorPos + 1);
		} while( separatorPos != KErrNotFound );
	}

TInt CWspHeaderReader::DecodeGenericSingleParameterL(TPtrC8& aRawParamBlock, CHeaderFieldPart& aHeaderFieldPart) const
	{
	TWspPrimitiveDecoder wspDecoder(aRawParamBlock);
	TInt bytesRead = 0;
	switch( wspDecoder.VarType() )
		{
		// This is a typed parameter
		case TWspPrimitiveDecoder::ELengthVal:
		case TWspPrimitiveDecoder::E7BitVal:
			{
			DecodeWellKnownParamTokenL(wspDecoder, bytesRead, aRawParamBlock, aHeaderFieldPart);
			} break;
		// This is an untyped parameter
		case TWspPrimitiveDecoder::EString:
			{
			DecodeUntypedParamL(wspDecoder, bytesRead, aRawParamBlock, aHeaderFieldPart);
			} break;
		default:
			User::Leave(KErrCorrupt);
			break;
		}
	return bytesRead;
	}

void CWspHeaderReader::DecodeWellKnownParamTokenL(TWspPrimitiveDecoder& aDecoder, TInt& aBytesRead,
												  TPtrC8& aRawParamBlock, CHeaderFieldPart& aHeaderFieldPart) const
	{
	TInt err = 0;
	TUint32 parameterToken = 0;
	aBytesRead = aDecoder.Integer(parameterToken);
	THTTPHdrVal paramValue;
	RStringF paramDesValue;
	CleanupClosePushL(paramDesValue);
	RStringF paramName = iStrPool.StringF(parameterToken, WSPParam::Table);
	switch( parameterToken )
		{
		case WSPParam::EQ:
			{
			// Decode Q value
			TUint32 qIntValue = 0;
			err = aDecoder.UintVar(qIntValue);
			User::LeaveIfError(err);
			aBytesRead += err;
			TReal q;
			TInt numDecimals = 0;
			TBuf8<KMaxNumQDigits> qDesC;
			if( qIntValue > 100 )
				{
				// Value is -100 and then divide by 1000
				qIntValue -= 100;
				q = ((TReal)(qIntValue/1000.));
				numDecimals = 3;
				}
			else
				{
				// Value is -1 and then divide by 100
				--qIntValue;
				if( qIntValue%10 ==0 )
					numDecimals = 1;
				else
					numDecimals = 2;
				q = ((TReal)(qIntValue/100.));
				}
			TRealFormat realFt(KMaxNumQDigits,numDecimals); // set max width and 3 decimal places
			// always use a decimal separator rather than the one supplied 
			// by the current locale
			realFt.iPoint = TChar('.'); 
			qDesC.Num(q, realFt);
			paramDesValue = iStrPool.OpenFStringL(qDesC);
			paramValue.SetStrF(paramDesValue);
			} break;
		case WSPParam::ECharset:
			{
			if( aRawParamBlock[aBytesRead] == 128 )
				{
				paramDesValue = iStrPool.StringF(WSPStdConstants::EAny, WSPStdConstants::Table);
				paramValue.SetStrF(paramDesValue);
				// Need to call Integer to update offset in WSP Decoder
				TUint8 updateDecoder =  0;
				err = aDecoder.Val7Bit(updateDecoder);
				User::LeaveIfError(err);
				aBytesRead += err;
				}
			else
				{
				switch( aDecoder.VarType() )
					{
					case TWspPrimitiveDecoder::E7BitVal:
					case TWspPrimitiveDecoder::ELengthVal:
						{
						TUint32 value = 0;
						err = aDecoder.Integer(value);
						User::LeaveIfError(err);
						aBytesRead += err;
						GetCharacterSetFromValueL(value, paramDesValue);
						paramValue.SetStrF(paramDesValue);
						} break;
					default:
						User::Leave(KErrCorrupt);
						break;
					}
				}
			} break;
		case WSPParam::ELevel:
			{
			// This is a version value
			err = aDecoder.VersionL(iStrPool,paramDesValue);
			User::LeaveIfError(err);
			aBytesRead += err;
			paramValue.SetStrF(paramDesValue);
			} break;
		case WSPParam::EType:
		case WSPParam::ESize:
		case WSPParam::EPadding:
		case WSPParam::ESEC:
		case WSPParam::EMaxAge:
			{
			TUint32 integerValue = 0;
			err = aDecoder.Integer(integerValue);
			User::LeaveIfError(err);
			aBytesRead += err;
			paramValue.SetInt(integerValue);
			} break;
		case WSPParam::ECreationDate:
		case WSPParam::EModificationDate:
		case WSPParam::EReadDate:
			{
			TDateTime dateTime(1970, EJanuary, 0, 0, 0, 0, 0);
			err = aDecoder.Date(dateTime);
			User::LeaveIfError(err);
			aBytesRead += err;
			paramValue.SetDateTime(dateTime);
			}
		case WSPParam::ENameDep:
		case WSPParam::EFilenameDep:
		case WSPParam::EStartDep:
		case WSPParam::EStartInfoDep:
		case WSPParam::ECommentDep:
		case WSPParam::EDomainDep:
		case WSPParam::EPathDep:
			{
			TPtrC8 textString;
			err = aDecoder.String(textString);
			User::LeaveIfError(err);
			aBytesRead += err;
			paramDesValue = iStrPool.OpenFStringL(textString);
			paramValue.SetStrF(paramDesValue);
			} break;
		case WSPParam::EMAC:
		case WSPParam::EName:
		case WSPParam::EFilename:
		case WSPParam::EStart:
		case WSPParam::EStartInfo:
		case WSPParam::EComment:
		case WSPParam::EDomain:
		case WSPParam::EPath:
			{
			// Check if the string has <no-value> ie <octet 0>
			if( aRawParamBlock[aBytesRead] == 0 )
				{
				paramDesValue = iStrPool.OpenFStringL(KNullDesC8());
				paramValue.SetStrF(paramDesValue);
				// Need to call Integer to update offset in WSP Decoder
				TUint32 updateDecoder =  0;
				err = aDecoder.Integer(updateDecoder);
				User::LeaveIfError(err);
				}
			else
				{
				TPtrC8 textString;
				err = aDecoder.String(textString);
				User::LeaveIfError(err);
				paramDesValue = iStrPool.OpenFStringL(textString);
				paramValue.SetStrF(paramDesValue);
				}
			aBytesRead += err;
			} break;
		case WSPParam::EDifferences:
			{
			aBytesRead += DecodeGenericParamTokenL(aDecoder, iStrTable, paramValue, paramDesValue);
			} break;
		case WSPParam::EContentTypeType:
			{
			aBytesRead += DecodeGenericParamTokenL(aDecoder, WSPContentTypes::Table, paramValue, paramDesValue);
			} break;
		case WSPParam::ESecure:
			{
			// <octet 0> no-value
			paramDesValue = iStrPool.OpenFStringL(KNullDesC8());
			paramValue.SetStrF(paramDesValue);
			// Need to call Integer to update offset in WSP Decoder
			TUint32 updateDecoder =  0;
			err = aDecoder.Integer(updateDecoder);
			User::LeaveIfError(err);
			aBytesRead += err;
			} break;
		default:
			User::Leave(KErrCorrupt);
			break;
		}

	// Add the parameter name and value
	TPtrC8 paramDes(paramName.DesC());
	SetNewParamL(aHeaderFieldPart, paramDes, paramValue);
	CleanupStack::PopAndDestroy(&paramDesValue);
	}

void CWspHeaderReader::DecodeUntypedParamL(TWspPrimitiveDecoder& aDecoder, TInt& aBytesRead,
										   TPtrC8& aRawParamBlock, CHeaderFieldPart& aHeaderFieldPart) const
	{
	TPtrC8 paramName;
	aBytesRead = aDecoder.String(paramName);
	User::LeaveIfError(aBytesRead);
	TWspPrimitiveDecoder paramValueDecoder(aRawParamBlock.Mid(aBytesRead));
	TWspPrimitiveDecoder::TWspHeaderType type = paramValueDecoder.VarType();
	// Check if the first octet is NULL, if it is then the value is a <no-value> string
	if( aRawParamBlock[aBytesRead] == 0 )
		type = TWspPrimitiveDecoder::EString;

	switch( type )
		{
		case TWspPrimitiveDecoder::ELengthVal:
		case TWspPrimitiveDecoder::E7BitVal:
			{
			TUint32 paramIntValue = 0;
			aBytesRead += paramValueDecoder.Integer(paramIntValue);
			SetNewParamL(aHeaderFieldPart, paramName, paramIntValue);
			} break;
		case TWspPrimitiveDecoder::EString:
		case TWspPrimitiveDecoder::EQuotedString:
			{
			RStringF paramValueStr;
			CleanupClosePushL(paramValueStr);
			// Check if the parameter has a value
			if( aRawParamBlock[aBytesRead] == 0 )
				{
				paramValueStr = iStrPool.OpenFStringL(KNullDesC8());
				++aBytesRead;
				}
			else
				{
				TPtrC8 paramStrValue;
				TInt err = paramValueDecoder.String(paramStrValue);
				paramValueStr = iStrPool.OpenFStringL(paramStrValue);
				User::LeaveIfError(err);
				aBytesRead += err;
				}
			SetNewParamL(aHeaderFieldPart, paramName, paramValueStr);
			CleanupStack::PopAndDestroy(&paramValueStr);
			} break;
		default:
			User::Leave(KErrCorrupt);
			break;
		}
	}

TInt CWspHeaderReader::DecodeGenericParamTokenL(TWspPrimitiveDecoder& aDecoder, const TStringTable& aStrTable,
											THTTPHdrVal& aParamValue, RStringF& aParamDesValue) const
	{
	TInt err = 0;
	switch( aDecoder.VarType() )
		{
		case TWspPrimitiveDecoder::EString:
			{
			TPtrC8 fieldNameString;
			err = aDecoder.String(fieldNameString);
			User::LeaveIfError(err);
			aParamDesValue = iStrPool.OpenFStringL(fieldNameString);
			aParamValue.SetStrF(aParamDesValue);
			} break;
		case TWspPrimitiveDecoder::E7BitVal:
			{
			TUint8 fieldNameToken = 0;
			err = aDecoder.Val7Bit(fieldNameToken);
			User::LeaveIfError(err);
			aParamDesValue = iStrPool.StringF(fieldNameToken, aStrTable);
			aParamValue.SetStrF(aParamDesValue);
			} break;
		default:
			User::Leave(KErrCorrupt);
			break;
		}
	return err;
	}

void CWspHeaderReader::DecodeGenericChallengeL(RHeaderField& aHeader) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	CheckLengthL(rawData, 3);
	TWspPrimitiveDecoder wspDecoder(rawData);
	if( wspDecoder.VarType() != TWspPrimitiveDecoder::ELengthVal )
		User::Leave(KErrCorrupt);

	TInt lengthValue = 0;
	// Check if the first byte is a length-quote
	TInt bytesProcessed = wspDecoder.LengthVal(lengthValue);

	// An error may have occured
	User::LeaveIfError(bytesProcessed);

	// Get the challenge data
	TInt err = 0;
	TBool basic = EFalse;
	TPtrC8 challangeData = rawData.Mid(bytesProcessed);
	// The if the first byte is <Octet 128> then it is the Basic scheme otherwise
	// the scheme is in token text
	if( challangeData[0] == 128 )
		{
		basic = ETrue;
		SetFStringPartL(aHeader, 0, iStrPool.StringF(WSPStdConstants::EBasic, WSPStdConstants::Table));
		// Need to call Integer to update offset in WSP Decoder
		TUint32 updateDecoder =  0;
		err = wspDecoder.Integer(updateDecoder);
		User::LeaveIfError(err);
		}
	else
		err = AddNewDecoderStringPartL(aHeader, wspDecoder);

	bytesProcessed += err;

	// Get the second part which should be a text string
	// First octet can be '0' ie No-value
	CHeaderFieldPart* fieldPart = NULL;
	if( rawData[bytesProcessed] == 0 )
		{
		fieldPart = &(SetNewFStringPartL(aHeader, 1, KNullDesC8()));
		// Need to call Integer to update offset in WSP Decoder
		TUint32 updateDecoder =  0;
		err = wspDecoder.Integer(updateDecoder);
		User::LeaveIfError(err);
		}
	else
		{
		TPtrC8 realmValue;
		err = wspDecoder.String(realmValue);
		User::LeaveIfError(err);
		fieldPart = &(SetNewFStringPartL(aHeader, 1, realmValue));
		}
	bytesProcessed += err;

	// Check for parameters and process them
	if( fieldPart == NULL || !basic )
		{
		while( bytesProcessed < rawData.Length() )
			{
			TPtrC8 parameterBlock(rawData.Mid(bytesProcessed));
			bytesProcessed += DecodeGenericSingleParameterL(parameterBlock, *fieldPart);
			}
		}
	}

void CWspHeaderReader::GetCharacterSetFromValueL(TInt aValue, RStringF& aCharSetStr) const
	{
	TInt charSetToken = 0;
	const TStringTable& strTable = WSPCharacterSets::Table;
	switch( aValue )
		{
		case 0x07EA:
			charSetToken = WSPCharacterSets::EBig5;
			break;
		case 0x03E8:
			charSetToken = WSPCharacterSets::EIso10646ucs2;
			break;
		case 0x04:
			charSetToken = WSPCharacterSets::EIso88591;
			break;
		case 0x05:
			charSetToken = WSPCharacterSets::EIso88592;
			break;
		case 0x06:
			charSetToken = WSPCharacterSets::EIso88593;
			break;
		case 0x07:
			charSetToken = WSPCharacterSets::EIso88594;
			break;
		case 0x08:
			charSetToken = WSPCharacterSets::EIso88595;
			break;
		case 0x09:
			charSetToken = WSPCharacterSets::EIso88596;
			break;
		case 0x0A:
			charSetToken = WSPCharacterSets::EIso88597;
			break;
		case 0x0B:
			charSetToken = WSPCharacterSets::EIso88598;
			break;
		case 0x0C:
			charSetToken = WSPCharacterSets::EIso88599;
			break;
		case 0x11:
			charSetToken = WSPCharacterSets::EShiftJIS;
			break;
		case 0x03:
			charSetToken = WSPCharacterSets::EUsAscii;
			break;
		case 0x6A:
			charSetToken = WSPCharacterSets::EUtf8;
			break;
		default:
			User::Leave(KErrCorrupt);
			break;
		}
	aCharSetStr = iStrPool.StringF(charSetToken, strTable);
	}

// Generic methods

CHeaderFieldPart& CWspHeaderReader::SetNewPartL(RHeaderField& aHeader, TInt aPartIndex, THTTPHdrVal& aPartVal) const
	{
	CHeaderFieldPart* part = CHeaderFieldPart::NewL(aPartVal);
	CleanupStack::PushL(part);
	aHeader.SetPartL(part, aPartIndex);
	CleanupStack::Pop(part);
	return *part;
	}

CHeaderFieldPart& CWspHeaderReader::SetNewIntegerPartL(RHeaderField& aHeader, TInt aPartIndex, TInt aValue) const
	{
	THTTPHdrVal partVal(aValue);
	CHeaderFieldPart& part = SetNewPartL(aHeader, aPartIndex, partVal);
	return part;
	}

CHeaderFieldPart& CWspHeaderReader::SetNewFStringPartL(RHeaderField& aHeader, TInt aPartIndex, TPtrC8 aValue) const
	{
	RStringF partStr = iStrPool.OpenFStringL(aValue);
	CleanupClosePushL(partStr);
	THTTPHdrVal partVal(partStr);
	CHeaderFieldPart& part = SetNewPartL(aHeader, aPartIndex, partVal);
	CleanupStack::PopAndDestroy(&partStr);
	return part;
	}

CHeaderFieldPart& CWspHeaderReader::SetNewStringPartL(RHeaderField& aHeader, TInt aPartIndex, TPtrC8 aValue) const
	{
	RString partStr = iStrPool.OpenStringL(aValue);
	CleanupClosePushL(partStr);
	THTTPHdrVal partVal(partStr);
	CHeaderFieldPart& part = SetNewPartL(aHeader, aPartIndex, partVal);
	CleanupStack::PopAndDestroy(&partStr);
	return part;
	}

CHeaderFieldPart& CWspHeaderReader::SetFStringPartL(RHeaderField& aHeader, TInt aPartIndex, RStringF aStrVal) const
	{
	// Check if a string was found
	if( aStrVal.DesC().Length() == 0 )
		User::Leave(KErrCorrupt);

	THTTPHdrVal partVal(aStrVal);
	CHeaderFieldPart& part = SetNewPartL(aHeader, aPartIndex, partVal);
	return part;
	}

CHeaderFieldPart& CWspHeaderReader::SetNewDatePartL(RHeaderField& aHeader, TInt aPartIndex, TDateTime& aDate) const
	{
	THTTPHdrVal partVal(aDate);
	CHeaderFieldPart& part = SetNewPartL(aHeader, aPartIndex, partVal);
	return part;
	}

CHeaderFieldParam& CWspHeaderReader::SetNewParamL(CHeaderFieldPart& aHeaderPart, TPtrC8 aParamName, THTTPHdrVal aParamValue) const
	{
	RStringF paramNameStr = iStrPool.OpenFStringL(aParamName);
	CleanupClosePushL(paramNameStr);
	CHeaderFieldParam* param = CHeaderFieldParam::NewL(paramNameStr, aParamValue);
	CleanupStack::PushL(param);
	aHeaderPart.AddParamL(param);
	CleanupStack::Pop(param);
	CleanupStack::PopAndDestroy(&paramNameStr);
	return *param;
	}

TBool CWspHeaderReader::CheckNullDesPartL(RHeaderField& aHeader, TPtrC8& aRawData, TInt aPartIndex) const
	{
	TUint8 firstByte = aRawData[0];
	if( firstByte == 0 )
		{
		SetNewFStringPartL(aHeader, aPartIndex, KNullDesC8());
		return ETrue;
		}
	return EFalse;
	}

TInt CWspHeaderReader::AddNewDecoderStringPartL(RHeaderField& aHeader, TWspPrimitiveDecoder& aDecoder, TInt aIndex) const
	{
	TPtrC8 buffer;
	TInt err = aDecoder.String(buffer);
	User::LeaveIfError(err);
	SetNewFStringPartL(aHeader, aIndex, buffer);
	return err;
	}

// Implementation of CWspDefaultHdrReader
//-------------------------------------------------------------------------

CWspDefaultHdrReader::~CWspDefaultHdrReader()
	{
	}

CWspDefaultHdrReader* CWspDefaultHdrReader::NewL(RStringPool aStrPool)
	{
	return new(ELeave)CWspDefaultHdrReader(aStrPool);
	}

CWspDefaultHdrReader* CWspDefaultHdrReader::NewLC(RStringPool aStrPool)
	{
	CWspDefaultHdrReader* self = CWspDefaultHdrReader::NewL(aStrPool);
	CleanupStack::PushL(self);
	return self;
	}

CWspDefaultHdrReader::CWspDefaultHdrReader(RStringPool aStrPool)
	: iStrPool(aStrPool)
	{
	}

void CWspDefaultHdrReader::DecodeHeaderL(RHeaderField& aHeader)
	{
	// Get and store the header field name
	TPtrC8 headerField(aHeader.Name().DesC());

	// Decode the header as a text-string
	TPtrC8 rawHeaderData;
	aHeader.RawDataL(rawHeaderData);

	TInt startPos = 0;
	TInt separatorPos = 0;
	TInt ii = 0;
	// Loop through all the parts separated by the header field name
	do
		{
		TPtrC8 rawData(rawHeaderData.Mid(startPos));
		separatorPos = rawData.FindF(headerField);
		if(separatorPos!=KErrNotFound)
			rawData.Set(rawHeaderData.Mid(startPos, separatorPos));

		// Check that the length of the data is at least 1
		if( rawData.Length() < 1 )
			User::Leave(KErrCorrupt);

		// Check if the data is an empty string which should only have a NULL terminator
		// otherwise extract the text-string from the primitive decoder
		TUint8 firstByte = rawData[0];
		TWspPrimitiveDecoder wspDecoder(rawData);
		TPtrC8 buffer;
		if( firstByte == 0 )
			buffer.Set(KNullDesC8());
		else
			User::LeaveIfError(wspDecoder.String(buffer));

		// Create a header part from the decoded buffer and add the part to the header field
		RStringF partStr = iStrPool.OpenFStringL(buffer);
		CleanupClosePushL(partStr);
		THTTPHdrVal partVal(partStr);
		CHeaderFieldPart* part = CHeaderFieldPart::NewL(partVal);
		CleanupStack::PushL(part);
		aHeader.SetPartL(part, ii);
		CleanupStack::Pop(part);
		CleanupStack::PopAndDestroy(&partStr);

		++ii;
		startPos += (separatorPos + headerField.Length() + 1);
		} while( separatorPos != KErrNotFound );
	}
