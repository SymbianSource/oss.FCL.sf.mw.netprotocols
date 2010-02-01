// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <charconv.h>

// #define __OUTPUT_WMLC	// Debug Stuff - uncomment to get wmlc output.

#include <wbconverter.h>
#include "WbToken.h"
#include <mwbxmlconverterobserver.h>
#include "WbEnum.h"
#include "ByteList.h"

#include <s32file.h>

// Wap specific error codes defined here
#include <wapengstd.h>

//
// Library routines
//

#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
	return (KErrNone);
}
#endif

GLDEF_C void Panic (TWbxmlPanic aPanic)
{
	_LIT(KWbPanic,"WbxmlConverter");
	User::Panic(KWbPanic ,aPanic);
}

//
// class CWbxmlConverter - external API
//

const TInt KOutputBufferLength=1024;

CWbxmlConverter::CWbxmlConverter(RFs& aFs)
: iFs(aFs)
{
	__DECLARE_NAME(_S("CWbxmlConverter"));
}

EXPORT_C CWbxmlConverter* CWbxmlConverter::NewL(RFs& aFs, MWbxmlConverterObserver* aObserver)
{
	CWbxmlConverter* instance = NewLC(aFs, aObserver);
	CleanupStack::Pop();
	return instance;
}

EXPORT_C CWbxmlConverter* CWbxmlConverter::NewLC(RFs& aFs, MWbxmlConverterObserver* aObserver)
{
	CWbxmlConverter* instance = new (ELeave) CWbxmlConverter(aFs);
	CleanupStack::PushL(instance);
	instance->ConstructL(aObserver, NULL);
	return instance;
}

EXPORT_C CWbxmlConverter* CWbxmlConverter::NewL(RFs& aFs, MWbxmlConverterObserver* aObserver, const TDesC* aCharacterEncoding)
{
	CWbxmlConverter* instance = new (ELeave) CWbxmlConverter(aFs);
	CleanupStack::PushL(instance);
	instance->ConstructL(aObserver, aCharacterEncoding);
	CleanupStack::Pop();
	return instance;
}

CWbxmlConverter::~CWbxmlConverter()
{
	delete[] iStringTable;
	delete iPublicIdStr;
	delete iByteList;
	delete iCharsetConverter;
	delete iOutputBuffer;

	if( iTagStack )
	{
		iTagStack->Reset();
		delete iTagStack;
	}
	
	if (iTagArray)
	{
		if (iTagArray->Count())
			iTagArray->ResetAndDestroy();
		delete iTagArray;
	}
	if (iAttArray)
	{
		if (iAttArray->Count())
			iAttArray->ResetAndDestroy();
		delete iAttArray;
	}
#ifdef __OUTPUT_WMLC
	iWmlcFile.Close();
	iWmlcFs.Close();
#endif
}

void CWbxmlConverter::ConstructL(MWbxmlConverterObserver* aObserver, const TDesC* aCharacterEncoding)
{
	iObserver = aObserver;
	iState = EConvertVersion;
	iByteList = CByteList::NewL();
	iParsing = EParseTag;
	iOpenQuote = EFalse;

	iTagStack = new CArrayFixFlat<TUint16>(7);

	iOutputBuffer = HBufC8::NewL(KOutputBufferLength);

	iCharsetConverter = CCnvCharacterSetConverter::NewL();
	if( aCharacterEncoding != NULL )
	{
		HBufC8* charset = HBufC8::NewLC(aCharacterEncoding->Length());
		charset->Des().Copy(*aCharacterEncoding);
		iWbxmlCharset = iCharsetConverter->ConvertStandardNameOfCharacterSetToIdentifierL(*charset, iFs);
		CleanupStack::PopAndDestroy();
	}
#ifdef __OUTPUT_WMLC
	_LIT(KWmlcFileName, "c:\\WmlcFile.wmlc");
	User::LeaveIfError(iWmlcFs.Connect());
	TInt error = iWmlcFile.Replace(iWmlcFs, KWmlcFileName, EFileWrite);
	if( error != KErrNone )
		User::Leave(EWapErrAccessViolation);
#endif
}

EXPORT_C void CWbxmlConverter::ProcessDataL(HBufC8& aBuffer)
{
#ifdef __OUTPUT_WMLC
	iWmlcFile.Write(aBuffer);
#endif
	iByteList->AddToListL(aBuffer);
	HandleDataL();
}

EXPORT_C void CWbxmlConverter::CWbxmlConverter_Reserved1()
{
	// Must exist for Binary Compatibility
}

EXPORT_C void CWbxmlConverter::CommitL()
{
	HandleDataL();

	// Document should be finished
	if (iState != EConvertFinished)
		User::Leave(EWapErrDocumentCorrupted);
}


EXPORT_C void MWbxmlConverterObserver::MWbxmlConverterObserver_Reserved1()
{
	// Must exist for Binary Compatibility
}


void CWbxmlConverter::SetNextStateL(TInt aResult, TConvertState aState)
{
	switch (aResult)
	{
	case KErrNone:
		iState = aState;
		break;
	case KErrNotFound:
		iByteList->RollBack();
		break;
	default:
		User::Leave(EWapErrDocumentCorrupted);
	}
	iByteList->Commit();
}

TInt CWbxmlConverter::ConvertWbxmlVersionL()
{
	if (iByteList->Byte(&iWbxmlVersion,TRUE) != KErrNone)
		return KErrNotFound;

	BufferAndOutputL(KWbXmlVersion10);

	return KErrNone;
}

TInt CWbxmlConverter::ConvertPublicIdValueL()
{
	TUint8 pubIDFirstByte = 0;
	if( iByteList->Byte(&pubIDFirstByte,EFalse) != KErrNone )
		return KErrNotFound;

	if( pubIDFirstByte == 0 )
	{
		// The public id is in string table, index follows
		iByteList->Inc(1);
		if( ExtractMultiByte(&iPublicIdIndex,0) != KErrNone )
			return KErrNotFound;
		iPublicId = 0;
	}
	else
	{
		// "Normal" publicId, a multibyte
		if (ExtractMultiByte(&iPublicId,0) != KErrNone)
			return KErrNotFound;
	}

	return KErrNone;
}

TInt CWbxmlConverter::ConvertCharsetL()
	{
	TUint32 charsetMIB = 0;
	// The chrset part of bytecode document was only introduced
	// in WML 1.1.
	if (iWbxmlVersion >= (TInt8) 0x01)
		{
		if (ExtractMultiByte(&charsetMIB,0) != KErrNone)
			return KErrNotFound;
		}

	// if the charset wasn't already provided by HTTP, we set it according
	// to the bytecode content charset
	if( !iWbxmlCharset )
		{
		if( charsetMIB != 0 )
			{
			iWbxmlCharset = iCharsetConverter->ConvertMibEnumOfCharacterSetToIdentifierL(charsetMIB, iFs);
			}

		if( iWbxmlCharset == 0 )
			{
			// Patch in the most common results
			switch(charsetMIB)
				{
				case 4:
					iWbxmlCharset = KCharacterSetIdentifierIso88591;
					break;
				case 5:
					iWbxmlCharset = KCharacterSetIdentifierIso88592;
					break;
				case 7:
					iWbxmlCharset = KCharacterSetIdentifierIso88594;
					break;
				case 8:
					iWbxmlCharset = KCharacterSetIdentifierIso88595;
					break;
				case 10:
					iWbxmlCharset = KCharacterSetIdentifierIso88597;
					break;
				case 12:
					iWbxmlCharset = KCharacterSetIdentifierIso88599;
					break;
				case 103:	// UTF7
					iWbxmlCharset = KCharacterSetIdentifierUtf7;
					break;
				case 104:
					iWbxmlCharset = KCharacterSetIdentifierImapUtf7;
					break;
				case 0:
					// Default case
				case 106:	// UTF8
					// If the charset from bytecode was 'unknown' (0) or the converter couldn't
					// match the mib number to known charset identifiers we use 
					// the default charset: UTF-8
					iWbxmlCharset = KCharacterSetIdentifierUtf8;
					break;
				default:
					return KErrNotSupported;
				}
			}
		// Now we need to inform our observer what charset we are using
		HBufC8* charSetName = iCharsetConverter->ConvertCharacterSetIdentifierToStandardNameL(iWbxmlCharset, iFs);
		CleanupStack::PushL(charSetName);
		iObserver->HandleWbxmlEncodingUpdateL(charSetName);
		CleanupStack::PopAndDestroy();		// charSetName
		}
	return KErrNone;
	}

TInt CWbxmlConverter::ConvertStringTableL()
{
	if (iStringTable == NULL)
	{
		// Need "static" instance variable for streaming iStringTable!
		if( ExtractMultiByte(&iContinue,0) != KErrNone )
			return KErrNotFound;

		if (iContinue == 0)
			return KErrNone;

		iStringTable = HBufC8::NewL(iContinue);
	}

	if (iContinue != 0)
	{
		// Note: String table is 8bit data
		TUint8 byte = 0;
		while (iContinue)
		{
			if ((iByteList->Byte(&byte,TRUE)) != KErrNone)
			{
				iByteList->Commit();
				return KErrNotFound;
			}

			iStringTable->Des().Append(byte);
			iContinue--;
		}
	}
	// Ensure that string table is zero terminated
	TInt length = iStringTable->Length();
	if( iStringTable->Des()[length-1] != 0 )
		{
		// Make table one character bigger, and make sure last character is zero terminator
		iStringTable = iStringTable->ReAllocL(length + 1);
		iStringTable->Des().Append(0);
		}
	return KErrNone;
}

TInt CWbxmlConverter::ConvertPublicIdNameL()
{
	HBufC* fileName = NULL;

	switch (iPublicId)
	{
	case KWbxmlIdIndex:
		// Meaning: Followed by string table index for public id
		User::Leave(EWapErrUnknownDocument);
		break;
	case KWbxmlIdUnknown:
		// Meaning: unknown or missing
		// WML spec says that unknown DTD is handled as well as possible
		// so we presume it's supposed to be WML 1.1
		fileName = FindTokenFileL(KWbTokenDatabaseWml11);
		CleanupStack::PushL(fileName);
		OpenTokenFileL(*fileName);
		CleanupStack::PopAndDestroy(); // fileName

		// Waste the publicID string received from the token file
		delete iPublicIdStr;
		// ...and replace it with the one from the string table
		{
			TPtrC8 remainderFromIndex = iStringTable->Des().Mid(iPublicIdIndex);
			TInt idLen = remainderFromIndex.Locate('\0');
			// We trust that the string table is properly constructed and
			// contains the terminating null...
			if( idLen == KErrNotFound )
				return KErrNotFound;		// Ah well, Our trust was misplaced!
			TPtrC8 publicIdString = remainderFromIndex.Left(idLen);
			HBufC16* convertedBuffer = CharsetConvertDataL(publicIdString);
			CleanupStack::PushL(convertedBuffer);
			if( convertedBuffer == NULL )
			{
			// Conversion failed!!!
				CleanupStack::PopAndDestroy(); // convertedBuffer
				return KErrCorrupt;
			}
			iPublicIdStr = HBufC8::NewL(convertedBuffer->Length());
			iPublicIdStr->Des().Copy(*convertedBuffer);
			CleanupStack::PopAndDestroy(); // convertedBuffer
		}
		break;
	case KWbxmlIdWta10:
		// Meaning: "-//WAPFORUM//DTD WTA 1.0//EN" (WTA Event 1.0)
		// WTA is not supported
		User::Leave(EWapErrUnknownDocument);
		break;
	case KWbxmlIdWml11:
		// Meaning: "-//WAPFORUM//DTD WML 1.1//EN" (WML 1.1)
		fileName = FindTokenFileL(KWbTokenDatabaseWml11);
		CleanupStack::PushL(fileName);
		OpenTokenFileL(*fileName);
		CleanupStack::PopAndDestroy(); // fileName
		break;
	case KWbxmlIdWml12:
		// Meaning: "-//WAPFORUM//DTD WML 1.1//EN" (WML 1.2)
		fileName = FindTokenFileL(KWbTokenDatabaseWml12);
		CleanupStack::PushL(fileName);
		OpenTokenFileL(*fileName);
		CleanupStack::PopAndDestroy(); // fileName
		break;
	case KWbxmlIdSI:
		// Meaning: "-//WAPFORUM//DTD SI 1.0//EN" (Push Service Indication 1.0)
		fileName = FindTokenFileL(KWbTokenDatabaseSI10);
		CleanupStack::PushL(fileName);
		OpenTokenFileL(*fileName);
		CleanupStack::PopAndDestroy(); // fileName
		break;
	case KWbxmlIdSL:
		// Meaning: "-//WAPFORUM//DTD SL 1.0//EN" (Push Service Load 1.0)
		fileName = FindTokenFileL(KWbTokenDatabaseSL10);
		CleanupStack::PushL(fileName);
		OpenTokenFileL(*fileName);
		CleanupStack::PopAndDestroy(); // fileName
		break;
	default:
		// Meaning: 0x05 - 0x7F recerved
		// WML spec says that unknown DTD is handled as well as possible
		// so we presume it's supposed to be WML 1.1
		fileName = FindTokenFileL(KWbTokenDatabaseWml11);
		CleanupStack::PushL(fileName);
		OpenTokenFileL(*fileName);
		CleanupStack::PopAndDestroy(); // fileName
		break;
	}

	switch (iPublicId)
		{
	case KWbxmlIdWml11:
		BufferAndOutputL(KWbPublicIdStartWml);
		BufferAndOutputL(*iPublicIdStr); // ...you get HBufC8&
		BufferAndOutputL(KWbPublicIdEnd11);
		break;
	case KWbxmlIdSI:
		BufferAndOutputL(KWbPublicIdStartSi);
		BufferAndOutputL(*iPublicIdStr); // ...you get HBufC8&
		BufferAndOutputL(KWbPublicIdEndSI10);
		break;
	case KWbxmlIdSL:
		BufferAndOutputL(KWbPublicIdStartSl);
		BufferAndOutputL(*iPublicIdStr); // ...you get HBufC8&
		BufferAndOutputL(KWbPublicIdEndSL10);
		break;
	case KWbxmlIdWml12:
	default:
		BufferAndOutputL(KWbPublicIdStartWml);
		BufferAndOutputL(*iPublicIdStr); // ...you get HBufC8&
		BufferAndOutputL(KWbPublicIdEnd12);
		break;
		}
	
	return KErrNone;
}


void CWbxmlConverter::HandleDataL()
{
	TInt result = KErrNone;

	do
	{
		switch (iState)
		{
		case EConvertVersion:
			result = ConvertWbxmlVersionL();
			SetNextStateL(result,EConvertPublicIdValue);
			break;
		case EConvertPublicIdValue:
			result = ConvertPublicIdValueL();
			SetNextStateL(result,EConvertCharset);
			break;
		case EConvertCharset:
			result = ConvertCharsetL();
			SetNextStateL(result,EConvertStringTable);
			break;
		case EConvertStringTable:
			result = ConvertStringTableL();
			SetNextStateL(result,EConvertPublicIdName);
			break;
		case EConvertPublicIdName:
			result = ConvertPublicIdNameL();
			SetNextStateL(result,EConvertData);
			break;
		case EConvertData:
			result = ConvertDataL();
			SetNextStateL(result,EConvertFinished);
			break;
		case EConvertFinished:
			if(iOutputBuffer!=NULL && iOutputBuffer->Length())
				{
				// Buffer still has some data : Lose ownership to the observer
				TInt returnValue = iObserver->HandleWbxmlOutputL(iOutputBuffer);	
				if( returnValue != KErrNone )
					User::Leave(returnValue);
				iOutputBuffer = NULL;
				}
			return;
		default:
			iState = EConvertError;
			return;
		}
	}
	while (result == KErrNone && iState != EConvertFinished);
}

TInt CWbxmlConverter::ConvertDataL()
{
	// Continue streaming in the middle of string
	if (iContinue != 0)
	{
		switch (iContinue)
		{
		case EGotoStreaming:
			if (ConvertDataStreamingL() != KErrNone)
				return KErrNotFound;
			break;
		case EGotoString:
			if (ConvertDataStringL() != KErrNone)
				return KErrNotFound;
			break;
		case EGotoOpaque:
			if (ConvertOpaqueDataL() != KErrNone)
				return KErrNotFound;
			break;
		}
		iContinue = 0;
	}

	TUint32 multiByte = 0;
	TUint8 byte = 0;

	FOREVER
	{
		byte = (TUint8) -1;
		if (iByteList->Byte(&byte,TRUE) != KErrNone)
		{
			// HACK: At the end of all data inc(1) fails and we get KErrNotFound
			if (iTagIndex != 1 || byte != 1)
			{
				// it was real error
				return KErrNotFound;
			}

		}

		switch (byte)
		{
		case 0x00: // SWITCH_PAGE
			// NOTE: not supported, not used in WML 1.1
			// bytecode SWITCH_PAGE not implemented
			iByteList->Commit();
			break;

		case 0x01: // END
			if (iParsing == EParseTag || iParsing == EParseNot)
			{
				if (iParsing == EParseNot)
				{
					iParsing = EParseTag;
				}

				if (iTagIndex)
				{
					iTagIndex--;
					TUint16 stackEntry = iTagStack->At(iTagIndex);
					if( stackEntry != KWbxmlSkipLiteralEndTagMagicToken )
					{
						BufferAndOutputL(KWbTagStartClose);
						BufferAndOutputL(*(iTagArray->At(stackEntry&~0xc0)->iBuf));
						BufferAndOutputL(KWbTagEndWithContent);
					}
					iTagStack->Delete(iTagIndex);

					// Done, get out
					if (iTagIndex == 0)
					{
						iByteList->Commit();
						return KErrNone;
					}
				}
				else
				{
					// Corrupted bytecode
					User::Leave(EWapErrDocumentCorrupted);
				}
			}
			else if (iParsing == EParseAttrs)
			{
				iParsing = EParseTag;

				if( !iRestrictedOutput )
				{
					if (iOpenQuote)
					{
						BufferAndOutputL(KWbQuote);
						iOpenQuote = EFalse;
					}
					BufferAndOutputL(hasContent?KWbTagEndWithContent():KWbTagEndWithoutContent());
				}
				else
					iRestrictedOutput = EFalse;
			}
			else
			{
				// default case to catch impossible errors
				// Corrupted bytecode
				User::Leave(EWapErrDocumentCorrupted);
			}
			iByteList->Commit();
			
			// This will be true when a message is defined with attributes
			// but no data
			if (iTagIndex == 0)
				{
				return KErrNone;
				}
			break;

		case 0x02: // ENTITY
			if (ExtractMultiByte(&multiByte,0) != KErrNone)
				return KErrNotFound;
			OutputL(multiByte);
			iByteList->Commit();
			break;

		case 0x40: // EXT_I_0
		case 0x41: // EXT_I_1
		case 0x42: // EXT_I_2
			iVariable = byte;
			BufferAndOutputL(KWbVariableStart);
			if (ConvertDataStreamingL() != KErrNone)
			{
				iContinue = EGotoStreaming;
				return KErrNotFound;
			}
			iByteList->Commit();
			break;

		case 0x03: // INLINE STRING
			if (iParsing == EParseTag)
			{
				iParsing = EParseNot;
			}

			if (ConvertDataStreamingL() != KErrNone)
			{
				iContinue = EGotoStreaming;
				return KErrNotFound;
			}
			iByteList->Commit();
			break;

		case 0x80: // EXT_T_0
		case 0x81: // EXT_T_1
		case 0x82: // EXT_T_2
			iVariable = byte;
			BufferAndOutputL(KWbVariableStart);
			if (ConvertDataStringL() != KErrNone)
			{
				iContinue = EGotoString;
				return KErrNotFound;
			}
			iByteList->Commit();
			break;

		case 0x83: // STR_T
			if (ConvertDataStringL() != KErrNone)
			{
				iContinue = EGotoString;
				return KErrNotFound;
			}
			iByteList->Commit();
			break;

		case 0x43: // PI
			// not supported, not used in WML 1.1
			// bytecode PI not implemented
			iByteList->Commit();
			break;

		// We will ignore all the uknown tags and their attributes encoded as literals
		// (LITERAL, LITERAL_C and LITERAL_CA). The content of these elements is, however,
		// treated normally (unless it contains literals, too :)
		case 0x04: // LITERAL
			iByteList->Commit();
			break;
		case 0xC3: // OPAQUE
			if (ConvertOpaqueDataL() != KErrNone)
			{
				iContinue = EGotoOpaque;
				return KErrNotFound;
			}
			iByteList->Commit();
			break;
		case 0x44: // LITERAL_C
		case 0xC4: // LITERAL_CA
			iRestrictedOutput = ETrue;
			// FLOWTHROUGH
		default:
			{
			if (iParsing == EParseTag || iParsing == EParseNot)
			{
				if (iParsing == EParseNot)
				{
					iParsing = EParseTag;
				}

				if( !iRestrictedOutput )
				{
					BufferAndOutputL(KWbTagStart);

					// tag name
					BufferAndOutputL(*(iTagArray->At(byte&0x3F)->iBuf));
				}

				hasAttrs = (TUint8) (byte & 0x80);
				hasContent = (TUint8) (byte & 0x40);

				if (hasContent)
				{
					iTagIndex++;
					if( iRestrictedOutput )
						iTagStack->AppendL( KWbxmlSkipLiteralEndTagMagicToken );
					else
						iTagStack->AppendL( TUint16(byte) );
				}

				if (hasAttrs)
					iParsing = EParseAttrs;
				else if( !iRestrictedOutput )
					BufferAndOutputL(hasContent ? KWbTagEndWithContent() : KWbTagEndWithoutContent() );

				if( iRestrictedOutput && !hasAttrs )
				// The content will be processed as usual. However, if we had attributes
				// we shall discard them, too
					iRestrictedOutput = EFalse;
			}

			// Attr names and values
			else if( !iRestrictedOutput )
			{
					

				// starting new attribute
				if (byte<128)
				{
					if (iOpenQuote)
					{
						BufferAndOutputL(KWbQuote);
						iOpenQuote = EFalse;
					}

					// attribute name requires starting space, value not
					BufferAndOutputL(KWbSpace);
				}

				if( byte >= iAttArray->Count() )
					User::Leave(EWapErrDocumentCorrupted);

				// AttrName can be either 'ALIGN="CENTER"' or 'ALIGN="'
				// format and followed by several AttrValues or strings.
				// We have to scan AttrName to know when quotes are open
				// so that we close them no more and no less than once.
				// NOTE: quotes in strings are data, not attr escaping.
				
				if (OutputCheckQuoteL(*(iAttArray->At(byte)->iBuf)))
					iOpenQuote = ETrue;
			}
			iByteList->Commit();
			break;
			}

		} // switch (byte)

	} // FOREVER
}

// Output Opaque data
TInt CWbxmlConverter::ConvertOpaqueDataL()
	{
	iByteList->Commit();

	TUint32 multiByte = 0;
	if (ExtractMultiByte(&multiByte,0) != KErrNone)
		return KErrNotFound;

	TUint32 count = multiByte;
	TUint8 byte = 0;
	const TInt KBufferSegment = 20;
	HBufC8* buffer = HBufC8::NewLC(KBufferSegment);

	while (count != 0)
	{
		if (iByteList->Byte(&byte,TRUE) != KErrNone)
		{
			CleanupStack::PopAndDestroy(); // buffer
			return KErrNotFound;
		}

		buffer->Des().Append(&byte,1);
		if( buffer->Des().MaxLength() == buffer->Length() )
		{
			buffer = buffer->ReAllocL(buffer->Length() + KBufferSegment);
			CleanupStack::Pop(); // the old buffer
			CleanupStack::PushL(buffer);
		}
		count--;
	}

	HBufC16* convertedBuffer = FormatDateStringL(*buffer);
	CleanupStack::PopAndDestroy(); // buffer
	CleanupStack::PushL(convertedBuffer);

	if( convertedBuffer == NULL )
	{
	// We could not convert the document!
	// but no one checks the return value
	// so I don't care... :(
		CleanupStack::PopAndDestroy();  // convertedBuffer
		iByteList->Commit();			// We can do nothing more with this data so dump it
		return KErrCorrupt;
	}

	buffer = HBufC8::NewL(convertedBuffer->Length());
	buffer->Des().Copy(*convertedBuffer);
	CleanupStack::PopAndDestroy(); // convertedBuffer
	CleanupStack::PushL(buffer);

	BufferAndOutputL(*buffer);

	iByteList->Commit();
	CleanupStack::PopAndDestroy(); // buffer

	FinishStringL();
	iByteList->Commit();
	return KErrNone;
	}

// OutputL bytes from streamed inline data
TInt CWbxmlConverter::ConvertDataStreamingL()
{
	iByteList->Commit();

	TUint8 byte = 0;
	const TInt KBufferSegment = 20;
	HBufC8* buffer = HBufC8::NewLC(KBufferSegment);

	// Todo: optimization check how much data is already available and OutputL all at the same time
	FOREVER
	{
		if (iByteList->Byte(&byte,TRUE) != KErrNone)
		{
			CleanupStack::PopAndDestroy(); // buffer
			return KErrNotFound;
		}

		if (byte == 0)
			break;

		buffer->Des().Append(&byte,1);
		if( buffer->Des().MaxLength() == buffer->Length() )
		{
			buffer = buffer->ReAllocL(buffer->Length() + KBufferSegment);
			CleanupStack::Pop(); // the old buffer
			CleanupStack::PushL(buffer);
		}
	}

	HBufC16* convertedBuffer = CharsetConvertDataL(*buffer);
	CleanupStack::PopAndDestroy(); // buffer
	CleanupStack::PushL(convertedBuffer);
	if( convertedBuffer == NULL )
	{
	// We could not convert the document!
	// but no one checks the return value
	// so I don't care... :(
		CleanupStack::PopAndDestroy();  // convertedBuffer
		iByteList->Commit();			// We can do nothing more with this data so dump it
		return KErrCorrupt;
	}

	convertedBuffer = ReplaceCharacterEntitiesL(convertedBuffer);
	CleanupStack::Pop(); // convertedBuffer, which might have been reallocated
	CleanupStack::PushL(convertedBuffer); // the new converted buffer

	buffer = HBufC8::NewL(convertedBuffer->Length());
	buffer->Des().Copy(*convertedBuffer);
	CleanupStack::PopAndDestroy(); // convertedBuffer
	CleanupStack::PushL(buffer);

	BufferAndOutputL(*buffer);

	iByteList->Commit();
	CleanupStack::PopAndDestroy(); // buffer

	FinishStringL();
	iByteList->Commit();
	return KErrNone;
}

// OutputL bytes from internal stringtable
TInt CWbxmlConverter::ConvertDataStringL()
{
	if( iStringTable == NULL )
		// There can not be string table references withing a document that hasn't got a string table!
		User::Leave(EWapErrDocumentCorrupted);

	iByteList->Commit();

	TUint32 multiByte = 0;
	if (ExtractMultiByte(&multiByte,0) != KErrNone)
		return KErrNotFound;

	TInt count = 0;
	while (iStringTable->Ptr()[multiByte + count] != 0)
		count++;

	if (!count)
		return KErrNone;

	if (TUint ((*iStringTable).Length()) < multiByte + count)
		return KErrCorrupt;

	TPtrC8 referedString = (*iStringTable).Mid(multiByte,count);
	HBufC16* convertedBuffer = CharsetConvertDataL(referedString);
	CleanupStack::PushL(convertedBuffer);
	if( convertedBuffer == NULL )
	{
	// We could not convert the document!
	// but no one checks the return value
	// so I don't care... :(
		CleanupStack::PopAndDestroy(); // convertedBuffer
		iByteList->Commit();			// We can do nothing more with this data so dump it
		return KErrCorrupt;
	}

	convertedBuffer = ReplaceCharacterEntitiesL(convertedBuffer);
	CleanupStack::Pop(); // convertedBuffer, which might have been reallocated
	CleanupStack::PushL(convertedBuffer); // the new converted buffer

	HBufC8* buffer = HBufC8::NewL(convertedBuffer->Length());
	buffer->Des().Copy(*convertedBuffer);	// to 8-bit buffer...
	CleanupStack::PopAndDestroy(); // convertedBuffer
	CleanupStack::PushL(buffer);
	BufferAndOutputL(*buffer);
	CleanupStack::PopAndDestroy(); // buffer

	FinishStringL();
	iByteList->Commit();

	return KErrNone;
}

TInt CWbxmlConverter::OpenTokenFileL(HBufC& aFileName)
{
	// Design note: we should ask library user to give us token file
	// but since this is used only by WML 1.1, it's not our problem.
	// Anyway, that's why interface is HBufC& aFileName.

	if (iTagArray)
	{
		if (iTagArray->Count())
			iTagArray->ResetAndDestroy();
		delete iTagArray;
		iTagArray = NULL;
	}
	if (iAttArray)
	{
		if (iAttArray->Count())
			iAttArray->ResetAndDestroy();
		delete iAttArray;
		iAttArray = NULL;
	}

	//

	RFileReadStream stream;
	if (stream.Open(iFs,aFileName,EFileShareReadersOnly) != KErrNone)
	{
		User::Leave(EWapErrUnknownDocument);
	}
	else
	{
		CleanupClosePushL(stream);

		HBufC8* strBuffer = HBufC8::NewL(256);
		CleanupStack::PushL(strBuffer);
		TInt i;
		CWbxmlToken* token;

		//
		// PublicId
		TRAPD(error,ReadTokenFileL(stream,*strBuffer));
		if (error != KErrNone)
			{
			User::Leave(EWapErrUnknownDocument);
			}

		iPublicIdStr = strBuffer->AllocL();

		//
		// Tag tokens
		iTagArray = new (ELeave) CArrayPtrFlat<CWbxmlToken>(0x40);
		for (i = 0; i < 0x40; i++)
		{
			TRAP(error,ReadTokenFileL(stream,*strBuffer));
			if (error == KErrNone)
			{
				token = new (ELeave) CWbxmlToken;
				CleanupStack::PushL(token);
				token->SetTextL(*strBuffer);
				iTagArray->AppendL(token);
				CleanupStack::Pop(token);
			}
			else
			{
				User::Leave(EWapErrUnknownDocument);
			}
		}
		
		//
		// Attribute names
		iAttArray = new (ELeave) CArrayPtrFlat<CWbxmlToken>(256);
		for (i = 0; i < 256; i++)
		{
			TRAP(error,ReadTokenFileL(stream,*strBuffer));
			if (error == KErrEof)
				break;
			if (error == KErrNone)
			{
				token = new (ELeave) CWbxmlToken;
				CleanupStack::PushL(token);
				token->SetTextL(*strBuffer);
				iAttArray->AppendL(token);
				CleanupStack::Pop(token);
			}
			else
			{
				User::Leave(EWapErrUnknownDocument);
			}
		}
		
		CleanupStack::PopAndDestroy(2); // strBuffer and stream
	}

	// reset iTagIndex, was used temporarily as offset variable
	iTagIndex = 0;

	return KErrNone;
}

void CWbxmlConverter::ReadTokenFileL(RFileReadStream& aStream, HBufC8& aBuffer)
	{
	// Read data from aFile into aBuffer. Skip linebreak(s), not into aBuffer
	TBuf8<1> data;
	TInt startOffset = iTagIndex;
	TPtr8 ptr = aBuffer.Des();
	
	// Note: Reusing class instance variable iTagIndex as file offset
	aStream.Source()->SeekL(MStreamBuf::ERead,EStreamBeginning,iTagIndex);
	FOREVER
		{
		aStream.ReadL(data,1);
		if (data[0] == '\r' || data[0] == '\n')
			{
			aStream.Source()->SeekL(MStreamBuf::ERead,EStreamBeginning,startOffset);
			aStream.ReadL(ptr,iTagIndex-startOffset);
			break;
			}
		iTagIndex++;
		}
	
// Consume linebreaks, which btw might be missing at last row
	FOREVER
		{
		aStream.ReadL(data,1);
		if (data[0] != '\r' && data[0] != '\n')
			break;
		iTagIndex++;
		}
	}

// Note: check wbxml specification for multibyte definition
TInt CWbxmlConverter::ExtractMultiByte(TUint32* aMultiByte, TInt aSkipHeader)
{
	if (iByteList->Inc(aSkipHeader) != KErrNone)
		return KErrNotFound;

	TUint8 byte = 0;
	*aMultiByte = 0;
	do
	{
		*aMultiByte <<= 7;
		if (iByteList->Byte(&byte,TRUE) != KErrNone)
			return KErrNotFound;

		*aMultiByte |= byte & ~0x80;
	} while (byte & 0x80);

	return KErrNone;
}

void CWbxmlConverter::FinishStringL()
{
	switch (iVariable)
	{
	case 0x00: // was not a variable
		break;
	case 0x40: // variable substitution - escaped
	case 0x80: // variable reference to string table - escaped
		BufferAndOutputL(KWbVariableEscaped);
		break;
	case 0x41: // variable substitution - unescaped
	case 0x81: // variable reference to string table - unescaped
		BufferAndOutputL(KWbVariableUnescaped);
		break;
	case 0x42: // variable substitution - no transformation
	case 0x82: // variable reference to string table - no transformation
		// FLOWTHROUGH
	default:
		BufferAndOutputL(KWbVariableNormal);
		break;
	}
	iVariable = 0;
}

HBufC16* CWbxmlConverter::FormatDateStringL(TDesC8& aData)
// Return value NULL indicates failed conversion!
// Ownership of the resulting string is passed up to caller.
{
	const TInt KDateTimePart = 7;
	enum TDateTimeParts {EYearTop, EYearBot, EMon, EDay, EHour, EMin, ESec};
	TInt dateData[KDateTimePart];

	for (TInt i = 0; i < KDateTimePart; i++)
		dateData[i] = 0;
	
	TInt offSet = 0;
	TInt dataLen = aData.Length();

	while (offSet < KDateTimePart && dataLen > 0)
		{
		dateData[offSet] = aData[offSet];
		offSet++;
		dataLen--;
		}

	_LIT(KDateFormat,"%2x%02x-%02x-%02xT%02x:%02x:%02xZ");
	const TInt KDateFormatLen = 20;

	TBuf16<KDateFormatLen> dateBuf;
	dateBuf.Format(KDateFormat, dateData[EYearTop], 
								dateData[EYearBot], 
								dateData[EMon], 
								dateData[EDay], 
								dateData[EHour], 
								dateData[EMin], 
								dateData[ESec]);
	return( dateBuf.AllocL() );	
}

HBufC16* CWbxmlConverter::CharsetConvertDataL(TDesC8& aData)
// Return value NULL indicates failed conversion!
// Ownership of the resulting string is passed up to caller.
{
	if( iCharsetConverter->PrepareToConvertToOrFromL(iWbxmlCharset,iFs) == CCnvCharacterSetConverter::ENotAvailable )
		return( NULL );

	HBufC* convertedString = HBufC::NewLC(aData.Length());
	TInt state = CCnvCharacterSetConverter::KStateDefault;
	TPtr16 convertedStringDes = convertedString->Des();
	TInt convertResult = iCharsetConverter->ConvertToUnicode(convertedStringDes, aData, state);
	CleanupStack::Pop(); // convertedString

	if (convertResult)
	{
		delete convertedString;
		convertedString = NULL;
	}

	return( convertedString );	
}


HBufC* CWbxmlConverter::ReplaceCharacterEntitiesL(HBufC* aString)
	{
	_LIT(KDollarRef, "$$");
	_LIT(KAmpersandRef, "&amp;");
	_LIT(KLesserRef, "&lt;");
	_LIT(KGreaterRef, "&gt;");
	_LIT(KQuoteRef, "&quot;");
	_LIT(KAposRef, "&apos;");
	_LIT(KNonBreakingSpaceRef, "&nbsp;");
	_LIT(KShyRef, "&shy;");
	
	// Calculate the need for extra characters to be allocated.
	// The additions to lengthExtension are subtracted by one, since the original
	// chacracter will be overwritten.
	TInt i=0;
	TInt lengthExtension = 0;	// The amount of extra characters needed
	const TText* stringPtr=aString->Ptr();
	const TDesC* ext=NULL;
	for( i=0; i < aString->Length(); i++)
		{
		switch( *stringPtr++ )
			{
			case '$':
				ext = &KDollarRef;
				break;
			case '&':
				ext = &KAmpersandRef;
				break;
			case '<':
				ext = &KLesserRef;
				break;
			case '>':
				ext = &KGreaterRef;
				break;
			case '"':
				ext = &KQuoteRef;
				break;
			case 39:
				ext = &KAposRef;
				break;
			case 160:
				ext = &KNonBreakingSpaceRef;
				break;
			case 173:
				ext = &KShyRef;
				break;
			default:
				// Normal character cause no added length
				break;
			}
		if(ext)
			{
			lengthExtension+=ext->Length()-1;
			ext=NULL;
			}
		}
	
	if( lengthExtension == 0 )
		return aString;
	
	if( aString->Des().MaxLength() < aString->Length()+lengthExtension )
		aString = aString->ReAllocL(aString->Length()+lengthExtension);
	
	// Do the replacement thing...

	stringPtr=aString->Ptr();
	ext=NULL;

	for( i=0; i < aString->Length(); i++, stringPtr++)
		{
		switch( *stringPtr )
			{
			case '$':
				ext=&KDollarRef;
				break;
			case '&':
				ext=&KAmpersandRef;
				break;
			case '<':
				ext=&KLesserRef;
				break;
			case '>':
				ext=&KGreaterRef;
				break;
			case '"':
				ext=&KQuoteRef;
				break;
			case 39:
				ext=&KAposRef;
				break;
			case 160:
				ext=&KNonBreakingSpaceRef;
				break;
			case 173:
				ext=&KShyRef;
				break;
			default:
				ext=NULL;
				break;
			}
		
		if( ext != NULL )
			{
			aString->Des().Delete(i,1);
			aString->Des().Insert(i,*ext);
			i += ext->Length()-1;  // one will be added to the counter at the end of the loop
			stringPtr += ext->Length()-1;  // one will be added to the counter at the end of the loop
			}
		}
	
	return( aString );
	}

// Entity output
void CWbxmlConverter::OutputL(TUint32 aSource) const
{
	TBuf8<4> buf;
	const TDesC8* data=NULL;
	// This is the dodgy bit : we need to buffer these correctly
	// We are WML specific, so we DO know about these special cases
	switch(aSource)
	{
	case '$':
		{
		// Can this be added here, too??? JOM 231199
		data = &KEntityDollar;
		}
		break;
	case '&': // ampersand &#38;&#38;
		{
		data = &KEntityAmp;
		}
		break;
	case '<': // less than &#38;&#60;
		{
		data = &KEntityLt;
		}
		break;
	case '>': // greater than &#62;
		{
		data = &KEntityGt;
		}
		break;
	case '"': // quotation mark &#34;
		{
		data = &KEntityQuot;
		}
		break;
	case 39: // apostrophe &#39;
		{
		data = &KEntityApos;
		}
		break;
	case 160: // non-breaking space &#160;
		{
		data = &KEntityNbsp;
		}
		break;
	case 173: // soft hyphen &#173;
		{
		data = &KEntityShy;
		}
		break;
	default:
		{
		buf.Append(aSource);
		data = &buf;
		}
	}
	if(data)
		BufferAndOutputL(*data);
}

TBool CWbxmlConverter::OutputCheckQuoteL(HBufC8& aSource) const
{
	TBool openQuote = EFalse;

	// Scan for quote symbol
	TInt len = aSource.Length();
	const TText8* stringPtr=aSource.Ptr();
	for (TInt i = 0; i < len; i++)
	{
		if (*stringPtr++ == '"')
			openQuote = !openQuote;
	}
	BufferAndOutputL(aSource);

	return openQuote;
}

void CWbxmlConverter::BufferAndOutputL(const TDesC8& aSource) const
	{
	// Append to buffer here and output when full
	if(aSource.Length() + iOutputBuffer->Length() > KOutputBufferLength)
		{
		// Buffer is full : Keep ownership but reset buffer length to zero
		// No point passing ownership as the observer copies the buffer
		// and then deletes it. Passing it by reference means the observer
		// copies it and we can reuse it
		TInt returnValue = iObserver->HandleWbxmlOutputL(*iOutputBuffer);
		if( returnValue != KErrNone )
			User::Leave(returnValue);
		TPtr8 ptr(iOutputBuffer->Des());
		ptr.SetLength(0);
		}
	// Check input source is smaller than buffer
	if(aSource.Length() >= KOutputBufferLength)
		{
		// We have to allocate a new buffer here and give ownership to the observer
		HBufC8* tmp = aSource.AllocLC();
		TInt returnValue = iObserver->HandleWbxmlOutputL(tmp);
		if( returnValue != KErrNone )
			User::Leave(returnValue);
		CleanupStack::Pop();
		return;
		}
	// Buffer the current output
	TPtr8 ptr = iOutputBuffer->Des();
	ptr.Append(aSource);
	}

HBufC* CWbxmlConverter::FindTokenFileL(const TDesC& aTokenFilePath) const
//
// Search drives for file specified by aTokenFilePath, and return the full path.
	{
	TFindFile finder = iFs;
	User::LeaveIfError(finder.FindByDir(aTokenFilePath, KNullDesC()));

	HBufC* fullPath = NULL;
	fullPath = finder.File().AllocL();

	return fullPath;
	}


//
// End of file
//
