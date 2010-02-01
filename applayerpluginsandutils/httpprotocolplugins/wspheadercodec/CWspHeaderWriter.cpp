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
#include "cwspheaderwriter.h"

const TInt KOffset = 1;				// Offset as the WspLanguages string table does not have 0x80 in it
const TInt KShortIntLimit = 128;	// A short integer can be within the range of 0-127

CWspHeaderWriter* CWspHeaderWriter::NewL(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec)
	{
	return new(ELeave)CWspHeaderWriter(aStrPool, aStrTable, aCodec);
	}

CWspHeaderWriter* CWspHeaderWriter::NewLC(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec)
	{
	CWspHeaderWriter* self = CWspHeaderWriter::NewL(aStrPool, aStrTable, aCodec);
	CleanupStack::PushL(self);
	return self;
	}

CWspHeaderWriter::CWspHeaderWriter(RStringPool aStrPool, const TStringTable& aStrTable, CWspHeaderCodec& aCodec)
	: iStrPool(aStrPool), iStrTable(aStrTable), iCodec(aCodec)
	{
	}

CWspHeaderWriter::~CWspHeaderWriter()
	{
	}

void CWspHeaderWriter::EncodeHeaderL(RHeaderField& aHeader)
	{
	switch(aHeader.Name().Index(iStrTable))
		{
		case WSP::EAccept:
			{
			EncodeAcceptL(aHeader);
			} break;
		case WSP::EAcceptCharsetDep:
		case WSP::EAcceptCharset:
			{
			EncodeAcceptCharsetL(aHeader);
			} break;
		case WSP::EAcceptEncodingDep:
		case WSP::EAcceptEncoding:
			{
			EncodeAcceptEncodingL(aHeader);
			} break;
		case WSP::EAcceptLanguage:
			{
			EncodeAcceptLanguageL(aHeader);
			} break;
		case WSP::EAllow:
			{
			EncodeAllowL(aHeader);
			} break;
		case WSP::EAuthorization:
			{
			EncodeAuthorizationL(aHeader);
			} break;
		case WSP::EContentEncoding:
			{
			EncodeContentEncodingL(aHeader);
			} break;
		case WSP::EContentLanguage:
			{
			EncodeContentLanguageL(aHeader);
			} break;
		case WSP::EContentLocation:
		case WSP::EFrom:
		case WSP::EProfile:
		case WSP::EReferer:
		case WSP::EUpgrade:
		case WSP::EUserAgent:
		case WSP::EVia:
			{
			GenericEncodeTextStringL(aHeader);
			} break;
		case WSP::EContentMD5:
			{
			EncodeContentMD5L(aHeader);
			} break;
		case WSP::EContentRangeDep:
		case WSP::EContentRange:
			{
			EncodeContentRangeL(aHeader);
			} break;
		case WSP::EContentType:
			{
			EncodeContentTypeL(aHeader);
			} break;
		case WSP::ECookie:
			{
			EncodeCookieL(aHeader);
			} break;
		case WSP::EDate:
		case WSP::ELastModified:
			{
			GenericEncodeDateL(aHeader);
			} break;
		case WSP::EEncodingVersion:
			{
			EncodeEncodingVersionL(aHeader);
			} break;
		case WSP::EExpect:
			{
			EncodeExpectL(aHeader);
			} break;
		case WSP::EPragma:
			{
			EncodePragmaL(aHeader);
			} break;
		case WSP::EProfileDiff:
			{
			EncodeProfileDiffL(aHeader);
			} break;
		case WSP::ERange:
			{
			EncodeRangeL(aHeader);
			} break;
		case WSP::ETE:
			{
			EncodeTEL(aHeader);
			} break;
		case WSP::ETrailer:
			{
			EncodeTrailerL(aHeader);
			} break;
		case WSP::EWarning:
			{
			EncodeWarningL(aHeader);
			} break;
		case WSP::EXWapApplicationId:
			{
			EncodeXWapApplicationIdL(aHeader);
			} break;
		default:
			User::Leave(KErrNotSupported);
			break;
		}
	}


void CWspHeaderWriter::EncodeAcceptL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	partIter.First();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	TInt partCount = 0;
	TBool requireValueLength = EFalse;
	aHeader.BeginRawDataL();
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			TInt numParam = part->NumParameters();
			if (numParam > 0)
				{
				requireValueLength = ETrue;
				encoder->StartValueLengthL();
				}
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				TInt typeValue = EncodeContentTypeValue(value);
				if (typeValue < KShortIntLimit && typeValue > KErrNotFound)
					encoder->AddShortIntL((TUint8)typeValue);
				else if (typeValue == KErrNotFound)
					{
					TInt registeredValue = value.StrF().Index(WSPRegContentTypes::Table);
					if (registeredValue > KErrNotFound)
						{
						if (!requireValueLength)
							{
							encoder->StartValueLengthL();
							requireValueLength = ETrue;
							}
						const TInt KContentTypeOffset = 0x0201;	// Offset for the Registered Content Type value string table
						encoder->AddLongIntL(registeredValue + KContentTypeOffset);
						}
					else if (registeredValue == KErrNotFound)
						{
						User::LeaveIfError(CheckTextString(value.StrF()));
						encoder->AddTextStringL(value.StrF().DesC());
						}
					}

				THeaderFieldParamIter paramIter = part->Parameters();
				paramIter.First();
				while (!paramIter.AtEnd())
					{
					const CHeaderFieldParam* param = paramIter();
					if (param != NULL)
						EncodeParameterL(*param, *encoder);
					++paramIter;
					}
				if (requireValueLength)
					encoder->EndValueLengthL();
				}
			else
				User::Leave(KErrCorrupt);
			}

		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;
		// If there is more than one header part it is necessary to append the name of the
		// header to the buffer
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeAcceptCharsetL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	partIter.First();
	aHeader.BeginRawDataL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	TInt partCount = 0;
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		TBool requireValueLength = EFalse;
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			TInt numParam = part->NumParameters();
			if (numParam > 0)
				{
				requireValueLength = ETrue;
				encoder->StartValueLengthL();
				}
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				// Check for '*' (Any-charset) and that the WSP version is greater than 1.2
				if ((value.StrF().Index(WSPStdConstants::Table) == WSPStdConstants::EAny) 
					&& (iCodec.GetWspVersion() > CWspHeaderCodec::EVersion1_2))
					{
					encoder->AddTokenL(128);		// '*' is encoded as <Octet 128>
					}
				else
					{
					TInt charValue = GetCharacterSetValue(value.StrF());
					switch(charValue)
						{
						case KErrNotFound:
							{
							User::LeaveIfError(CheckTextString(value.StrF()));
							encoder->AddTextStringL(value.StrF().DesC());
							} break;
						default:
							{
							if (charValue < KShortIntLimit)
								encoder->AddShortIntL((TUint8)charValue);
							else
								{
								if (!requireValueLength)
									{
									encoder->StartValueLengthL();
									requireValueLength = ETrue;
									}
								encoder->AddLongIntL(charValue);
								}
							} break;
						}
					}

				// Check if we have any parameters
				THeaderFieldParamIter paramIter = part->Parameters();
				paramIter.First();
				while (!paramIter.AtEnd())
					{
					const CHeaderFieldParam* param = paramIter();
					THTTPHdrVal paramValue = param->Value();
					HBufC8* qValue = EncodeQValueL(paramValue.StrF());
					CleanupStack::PushL(qValue);
					if (qValue != NULL)
						encoder->AddDataL(*qValue);
					++paramIter;
					CleanupStack::PopAndDestroy(qValue);
					}
				}
			else
				User::Leave(KErrCorrupt);
			}

		// If we have a Q-value then we need to calculate the value-length
		if (requireValueLength)
			encoder->EndValueLengthL();
		
		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;		
		// If there is more than one header part it is necessary to append the name of the
		// header to the buffer
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeAcceptEncodingL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	partIter.First();
	TInt partCount = 0;
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());	
	aHeader.BeginRawDataL();
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		TBool requireValueLength = EFalse;
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			TInt numParam = part->NumParameters();
			if (numParam > 0)
				{
				requireValueLength = ETrue;
				encoder->StartValueLengthL();
				}
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				TInt encValue = value.StrF().Index(WSPStdConstants::Table);
				// Check for '*' (Any-encoding) and that the WSP version is greater than 1.2
				if ((encValue == WSPStdConstants::EAny) 
					&& (iCodec.GetWspVersion() > CWspHeaderCodec::EVersion1_2))
					{
					if (!requireValueLength)
						{
						encoder->StartValueLengthL();
						requireValueLength = ETrue;
						}
					encoder->AddTokenL(131);				// '*' is encoded as <Octet 131>
					}
				else
					{
					switch(encValue)
						{
						case WSPStdConstants::EGzip:		// Gzip is encoded as <Octet 128>
							{
							encoder->AddTokenL(128);
							} break;
						case WSPStdConstants::ECompress:	// Compress is encoded as <Octet 129>
							{
							encoder->AddTokenL(129);
							} break;
						case WSPStdConstants::EDeflate:		// Deflate is encoded as <Octet 130>
							{
							encoder->AddTokenL(130);
							} break;
						default:
							{
							encoder->AddTokenTextL(value.StrF().DesC());
							} break;
						}
					}
			
				THeaderFieldParamIter paramIter = part->Parameters();
				paramIter.First();
				while (!paramIter.AtEnd())
					{
					const CHeaderFieldParam* param = paramIter();
					THTTPHdrVal paramValue = param->Value();
					HBufC8* qValue = EncodeQValueL(paramValue.StrF());
					CleanupStack::PushL(qValue);
					if (qValue != NULL)
						encoder->AddDataL(*qValue);
					CleanupStack::PopAndDestroy(qValue);
					++paramIter;
					}
				}
			else
				User::Leave(KErrCorrupt);
			}

		if (requireValueLength)
			encoder->EndValueLengthL();
		
		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;	
		// If there is more than one header part it is necessary to append the name of the
		// header to the buffer
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeAcceptLanguageL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	partIter.First();
	TInt partCount = 0;
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	aHeader.BeginRawDataL();
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		TBool requireValueLength = EFalse;
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			TInt numParam = part->NumParameters();
			if (numParam > 0)
				{
				requireValueLength = ETrue;
				encoder->StartValueLengthL();
				}
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				TInt langValue = value.StrF().Index(WSPLanguages::Table);
				switch(langValue)
					{
					case WSPLanguages::EAnyLanguage:
						{
						encoder->AddTokenL(128);		// '*' (Any-language) is encoded as <Octet 128>
						} break;
					case KErrNotFound:
						{
						User::LeaveIfError(CheckTextString(value.StrF()));
						encoder->AddTextStringL(value.StrF().DesC());
						} break;
					default:
						{
						if (langValue < KShortIntLimit)
							encoder->AddShortIntL((TUint8)langValue);
						else
							{
							if (!requireValueLength)
								{
								encoder->StartValueLengthL();
								requireValueLength = ETrue;
								}
							encoder->AddLongIntL(langValue + KOffset);
							}
						} break;
					}

				THeaderFieldParamIter paramIter = part->Parameters();
				paramIter.First();
				while (!paramIter.AtEnd())
					{
					const CHeaderFieldParam* param = paramIter();
					THTTPHdrVal paramValue = param->Value();
					HBufC8* qValue = EncodeQValueL(paramValue.StrF());
					CleanupStack::PushL(qValue);
					if (qValue != NULL)
						encoder->AddDataL(*qValue);
					++paramIter;
					CleanupStack::PopAndDestroy(qValue);
					}
				}
			else
				User::Leave(KErrCorrupt);
			}

		if (requireValueLength)
			encoder->EndValueLengthL();
		
		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;	
		// If there is more than one header part it is necessary to append the name of the
		// header to the buffer
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeAllowL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	aHeader.BeginRawDataL();
	TInt partCount = 0;
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				TUint8 allowNumber = 0;
				TInt allowValue = value.StrF().Index(WSPTypeConstants::Table);
				switch(allowValue)
					{
					case WSPTypeConstants::EGet:
						{
						allowNumber = 64;		// Get is encoded as 0x40
						} break;
					case WSPTypeConstants::EOptions:
						{
						allowNumber = 65;		// Options is encoded as 0x41
						} break;
					case WSPTypeConstants::EHead:
						{
						allowNumber = 66;		// Head is encoded as 0x42
						} break;
					case WSPTypeConstants::EDelete:
						{
						allowNumber = 67;		// Delete is encoded as 0x43
						} break;
					case WSPTypeConstants::ETrace:
						{
						allowNumber = 68;		// Trace is encoded as 0x44
						} break;
					case WSPTypeConstants::EPost:
						{
						allowNumber = 96;		// Post is encoded as 0x60
						} break;
					case WSPTypeConstants::EPut:
						{
						allowNumber = 97;		// Put is encoded as 0x61
						} break;
					default:
						User::Leave(KErrNotFound);
						break;
					}
				encoder->AddShortIntL(allowNumber);
				}
			else
				User::Leave(KErrCorrupt);
			}
		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeAuthorizationL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				encoder->StartValueLengthL();
				// Check to see if the first part is 'Basic'
				if (value.StrF().Index(WSPStdConstants::Table) == WSPStdConstants::EBasic)
					{
					// If it is, then encode Basic as <Octet 128> and then iterate through the
					// following parts which should contain the 'User-ID' and 'Password'
					encoder->AddTokenL(128);
					++partIter;
					while (!partIter.AtEnd())
						{
						value = partIter()->Value();
						if(value.Type() == THTTPHdrVal::KStrFVal)
							{
							User::LeaveIfError(CheckTextString(value.StrF()));
							encoder->AddTextStringL(value.StrF().DesC());
							++partIter;
							}
						else
							User::Leave(KErrCorrupt);
						}
					}
				// Otherwise, the first part is an 'Authentication-scheme' with zero or more parameters
				else
					{
					encoder->AddTokenTextL(value.StrF().DesC());
					THeaderFieldParamIter paramIter = part->Parameters();
					paramIter.First();
					while (!paramIter.AtEnd())
						{
						const CHeaderFieldParam* param = paramIter();
						if (param != NULL)
							EncodeParameterL(*param, *encoder);
						++paramIter;
						}
					}
				encoder->EndValueLengthL();
				}
			else
				User::Leave(KErrCorrupt);
			}

		aHeader.BeginRawDataL();	
		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		aHeader.CommitRawData();
		}
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeContentEncodingL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	aHeader.BeginRawDataL();
	TInt partCount = 0;
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				TInt encValue = value.StrF().Index(WSPStdConstants::Table);
				switch(encValue)
					{
					case WSPStdConstants::EGzip:
						{
						encoder->AddTokenL(128);		// Gzip is encoded as <Octet 128>
						} break;
					case WSPStdConstants::ECompress:
						{
						encoder->AddTokenL(129);		// Compress is encoded as <Octet 129>
						} break;
					case WSPStdConstants::EDeflate:
						{
						encoder->AddTokenL(130);		// Deflate is encoded as <Octet 130>
						} break;
					default:
						{
						encoder->AddTokenTextL(value.StrF().DesC());
						} break;
					}
				}
			else
				User::Leave(KErrCorrupt);
			}

		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeContentLanguageL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	partIter.First();
	TInt partCount = 0;
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	aHeader.BeginRawDataL();
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				TInt langValue = value.StrF().Index(WSPLanguages::Table);
				switch(langValue)
					{
					case WSPLanguages::EAnyLanguage:
						{
						encoder->AddTokenL(128);		// '*' (Any-language) is encoded as <Octet 128>
						} break;
					case KErrNotFound:
						{
						encoder->AddTokenTextL(value.StrF().DesC());
						} break;
					default:
						{
						if (langValue < KShortIntLimit)
							encoder->AddShortIntL((TUint8)langValue);
						else
							encoder->AddLongIntL(langValue + KOffset);
						} break;
					}
				}
			else
				User::Leave(KErrCorrupt);
			}

		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeContentMD5L(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TBool requireValueLength = EFalse;
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				encoder->StartValueLengthL();
				encoder->AddDataL(value.StrF().DesC());
				requireValueLength = ETrue;
				}
			else
				User::Leave(KErrCorrupt);

			if (requireValueLength)
				encoder->EndValueLengthL();

			aHeader.BeginRawDataL();	
			HBufC8* buffer = encoder->EndHeaderL();
			CleanupStack::PushL(buffer);
			TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
			aHeader.WriteRawDataL(data);
			CleanupStack::PopAndDestroy(buffer);
			aHeader.CommitRawData();
			}
		}
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeContentRangeL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KTIntVal)
				{
				encoder->StartValueLengthL();
				encoder->AddUintVarL(value);
				++partIter;
				if (!partIter.AtEnd())
					{
					value = partIter()->Value();
					if (value.Type() == THTTPHdrVal::KStrFVal)
						{
						// Check for '*' (Any-encoding) and that the WSP version is greater than 1.2
						if ((value.StrF().Index(WSPStdConstants::Table) == WSPStdConstants::EAny)
							&& (iCodec.GetWspVersion() > CWspHeaderCodec::EVersion1_2))
							{
							encoder->AddTokenL(128);	// '*' is encoded as <Octet 128>
							}
						}
					if (value.Type() == THTTPHdrVal::KTIntVal)
						encoder->AddUintVarL(value);
					}
				encoder->EndValueLengthL();
				}
			else
				User::Leave(KErrCorrupt);

			aHeader.BeginRawDataL();	
			HBufC8* buffer = encoder->EndHeaderL();
			CleanupStack::PushL(buffer);
			TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
			aHeader.WriteRawDataL(data);
			CleanupStack::PopAndDestroy(buffer);
			aHeader.CommitRawData();
			}
		}
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeContentTypeL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	TBool requireValueLength = EFalse;
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			TInt numParam = part->NumParameters();
			if (numParam > 0)
				{
				requireValueLength = ETrue;
				encoder->StartValueLengthL();
				}
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				TInt typeValue = EncodeContentTypeValue(value);
				if (typeValue < KShortIntLimit && typeValue > KErrNotFound)
					encoder->AddShortIntL((TUint8)typeValue);
				else if (typeValue == KErrNotFound)
					{
					TInt registeredValue = value.StrF().Index(WSPRegContentTypes::Table);
					if (registeredValue > KErrNotFound)
						{
						if (!requireValueLength)
							{
							encoder->StartValueLengthL();
							requireValueLength = ETrue;
							}
						const TInt KContentTypeOffset = 0x0201;	// Offset for the Registered Content Type value string table
						encoder->AddLongIntL(registeredValue + KContentTypeOffset);
						}
					else if (registeredValue == KErrNotFound)
						{
						User::LeaveIfError(CheckTextString(value.StrF()));
						encoder->AddTextStringL(value.StrF().DesC());
						}
					}
				// check for params and append to temporary buffer
				THeaderFieldParamIter paramIter = part->Parameters();
				paramIter.First();
				while (!paramIter.AtEnd())
					{
					const CHeaderFieldParam* param = paramIter();
					if (param != NULL)
						EncodeParameterL(*param, *encoder);
					++paramIter;
					}

				if (requireValueLength)
					encoder->EndValueLengthL();

				aHeader.BeginRawDataL();	
				HBufC8* buffer = encoder->EndHeaderL();
				CleanupStack::PushL(buffer);
				TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
				aHeader.WriteRawDataL(data);
				CleanupStack::PopAndDestroy(buffer);
				aHeader.CommitRawData();
				}
			else
				User::Leave(KErrCorrupt);
			}
		}
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeCookieL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			THTTPHdrVal value = part->Value();
			// Encode the first part which is the Cookie-version value
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				encoder->StartValueLengthL();
				TUint8 versionValue = EncodeVersionValueL(value.StrF());
				if (versionValue == 0)
					{
					User::LeaveIfError(CheckTextString(value.StrF()));
					encoder->AddTextStringL(value.StrF().DesC());
					}
				else
					encoder->AddShortIntL(versionValue);

				// Then move to the next part and iterate through all the parts that follow, appending
				// the parameters if any exist
				++partIter;
				while (!partIter.AtEnd())
					{
					CWspHeaderEncoder* cookieEncoder = CWspHeaderEncoder::NewL();
					CleanupStack::PushL(cookieEncoder);
					value = partIter()->Value();	// The first part is the Cookie-Name
					if (value.Type() == THTTPHdrVal::KStrFVal)
						{
						cookieEncoder->StartHeaderL((TUint8)token);
						cookieEncoder->StartValueLengthL();
						User::LeaveIfError(CheckTextString(value.StrF()));
						cookieEncoder->AddTextStringL(value.StrF().DesC());
						++partIter;
						part = partIter();
						value = partIter()->Value();	// The second part is the Cookie-Value
						if (value.Type() == THTTPHdrVal::KStrFVal)
							{
							User::LeaveIfError(CheckTextString(value.StrF()));
							cookieEncoder->AddTextStringL(value.StrF().DesC());
							THeaderFieldParamIter paramIter = part->Parameters();	// Now check for any parameters
							paramIter.First();
							while (!paramIter.AtEnd())
								{
								const CHeaderFieldParam* param = paramIter();
								if (param != NULL)
									EncodeParameterL(*param, *cookieEncoder);
								++paramIter;
								}
							}
						else
							User::Leave(KErrCorrupt);

						cookieEncoder->EndValueLengthL();
						HBufC8* cookieBuffer = cookieEncoder->EndHeaderL();
						CleanupStack::PushL(cookieBuffer);
						// Remove the header name from the buffer and then check if 
						// the first byte is equal to 31 i.e. a length quote, and if so remove it
						TInt offset = 1;
						if (cookieBuffer->Des()[1] == 31)
							++offset;
						HBufC8* data = cookieBuffer->Mid(offset).AllocL();
						CleanupStack::PushL(data);
						encoder->AddDataL(*data);
						CleanupStack::PopAndDestroy(2, cookieBuffer);		// data. cookieBuffer
						}
					++partIter;
					part = partIter();
					CleanupStack::PopAndDestroy(cookieEncoder);
					}
				encoder->EndValueLengthL();
				}
			else
				User::Leave(KErrCorrupt);
			}	
		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
		aHeader.BeginRawDataL();
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		aHeader.CommitRawData();
		}
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeEncodingVersionL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)		// There is a version-value in the first part
				{
				TUint8 versionValue = EncodeVersionValueL(value.StrF());
				if (versionValue == 0)
					{
					User::LeaveIfError(CheckTextString(value.StrF()));
					encoder->AddTextStringL(value.StrF().DesC());
					}
				else
					encoder->AddShortIntL(versionValue);
				}
			else if (value.Type() == THTTPHdrVal::KTIntVal)	// The first part is NOT a version-value but an int
				{
				encoder->StartValueLengthL();
				if (value.Int() < KShortIntLimit)
					encoder->AddShortIntL((TUint8)value);
				else
					User::Leave(KErrCorrupt);

				++partIter;
				if (!partIter.AtEnd())
					{
					value = partIter()->Value();
					if (value.Type() == THTTPHdrVal::KStrFVal)
						{
						TUint8 versionValue = EncodeVersionValueL(value.StrF());
						if (versionValue == 0)
							{
							User::LeaveIfError(CheckTextString(value.StrF()));
							encoder->AddTextStringL(value.StrF().DesC());
							}
						else
							encoder->AddShortIntL(versionValue);
						}
					}
				encoder->EndValueLengthL();
				}
			else
				User::Leave(KErrCorrupt);

			aHeader.BeginRawDataL();	
			HBufC8* buffer = encoder->EndHeaderL();
			CleanupStack::PushL(buffer);
			TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
			aHeader.WriteRawDataL(data);
			CleanupStack::PopAndDestroy(buffer);
			aHeader.CommitRawData();
			}
		}
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeExpectL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				if (value.StrF().Index(WSPStdConstants::Table) == WSPStdConstants::E100Continue)
					encoder->AddTokenL(128);		// 100-continue is encoded as <Octet 128>
				else
					{
					encoder->StartValueLengthL();
					while (!partIter.AtEnd())
						{
						value = partIter()->Value();
						if(value.Type() == THTTPHdrVal::KStrFVal)
							{
							TPtrC8 valueDes(value.StrF().DesC());
							if (valueDes[0] == 0x22)	// Check if we have a quoted-string
								{
								User::LeaveIfError(CheckTextString(value.StrF()));
								encoder->AddTextStringL(value.StrF().DesC());
								}
							else
								encoder->AddTokenTextL(value.StrF().DesC());

							THeaderFieldParamIter paramIter = part->Parameters();
							paramIter.First();
							while (!paramIter.AtEnd())
								{
								const CHeaderFieldParam* param = paramIter();
								if (param != NULL)
									EncodeParameterL(*param, *encoder);
								++paramIter;
								}
							++partIter;
							part = partIter();
							}
						else
							User::Leave(KErrCorrupt);
						}
					encoder->EndValueLengthL();
					}
				}
			else
				User::Leave(KErrCorrupt);
			}

		aHeader.BeginRawDataL();	
		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		aHeader.CommitRawData();
		}
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodePragmaL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	partIter.First();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TBool requireValueLength = EFalse;
	TInt token = EncodeFieldName(aHeader.Name());
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				if (value.StrF().Index(WSPStdConstants::Table) == WSPStdConstants::ENoCache)
					encoder->AddTokenL(128);		// 'No-cache' is encoded as <Octet 128>
				else
					{
					THeaderFieldParamIter paramIter = part->Parameters();
					paramIter.First();
					while (!paramIter.AtEnd())
						{
						const CHeaderFieldParam* param = paramIter();
						if (param != NULL)
							{
							encoder->StartValueLengthL();
							EncodeParameterL(*param, *encoder);
							requireValueLength = ETrue;
							}
						++paramIter;
						}
					}
				if (requireValueLength)
					encoder->EndValueLengthL();

				aHeader.BeginRawDataL();	
				HBufC8* buffer = encoder->EndHeaderL();
				CleanupStack::PushL(buffer);
				TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
				aHeader.WriteRawDataL(data);
				CleanupStack::PopAndDestroy(buffer);
				aHeader.CommitRawData();
				}
			else
				User::Leave(KErrCorrupt);
			}
		}
	CleanupStack::PopAndDestroy(encoder);
	}
	
void CWspHeaderWriter::EncodeProfileDiffL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt partCount = 0;
	aHeader.BeginRawDataL();
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				encoder->StartValueLengthL();
				encoder->AddDataL(value.StrF().DesC());
				encoder->EndValueLengthL();
				}
			else
				User::Leave(KErrCorrupt);
			}
			
		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeRangeL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	TInt partCount = 0;
	aHeader.BeginRawDataL();
	partIter.First();
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				encoder->StartValueLengthL();
				TInt rangeValue = value.StrF().Index(WSPStdConstants::Table);
				switch (rangeValue)
					{
					case WSPStdConstants::EByteRange:
						{
						encoder->AddTokenL(128);	// 'Byte-range' is encoded as <Octet 128>
						} break;
					case WSPStdConstants::ESuffixByteRange:
						{
						encoder->AddTokenL(129);	// 'Suffix-byte-range' is encoded as <Octet 129>
						} break;
					default:
						User::Leave(KErrCorrupt);
					}
				// Iterate through the parts that follow
				++partIter;
				while (!partIter.AtEnd())
					{
					value = partIter()->Value();
					if(value.Type() == THTTPHdrVal::KTIntVal)
						{
						encoder->AddUintVarL(value);
						++partIter;
						}
					else if(value.Type() == THTTPHdrVal::KStrFVal)
						{
						// Found the next header
						break;
						}
					else
						User::Leave(KErrCorrupt);
					}
				encoder->EndValueLengthL();
				}
			else
				User::Leave(KErrCorrupt);			
			}

		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeTEL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	TBool requireValueLength = EFalse;
	aHeader.BeginRawDataL();
	TInt partCount = 0;
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			TInt numParam = part->NumParameters();
			if (numParam > 0)
				{
				encoder->StartValueLengthL();
				requireValueLength = ETrue;
				}
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				if (value.StrF().Index(WSPStdConstants::Table) == WSPStdConstants::ETrailers)
					encoder->AddTokenL(129);		// 'Trailers' is encoded as <Octet 129>
				else
					{
					if (!requireValueLength)
						{
						encoder->StartValueLengthL();
						requireValueLength = ETrue;
						}
					TInt encValue = value.StrF().Index(WSPStdConstants::Table);
					switch(encValue)
						{
						case WSPStdConstants::EChunked:
							{
							encoder->AddTokenL(130);		// 'Chunked' is encoded as <Octet 130>
							} break;
						case WSPStdConstants::EIdentity:
							{
							encoder->AddTokenL(131);		// 'Identity' is encoded as <Octet 131>
							} break;
						case WSPStdConstants::EGzip:
							{
							encoder->AddTokenL(132);		// 'Gzip' is encoded as <Octet 132>
							} break;
						case WSPStdConstants::ECompress:
							{
							encoder->AddTokenL(133);		// 'Compress' is encoded as <Octet 133>
							} break;
						case WSPStdConstants::EDeflate:
							{
							encoder->AddTokenL(134);		// 'Deflate' is encoded as <Octet 134>
							} break;
						default:
							{
							encoder->AddTokenTextL(value.StrF().DesC());
							} break;
						}

					THeaderFieldParamIter paramIter = part->Parameters();
					paramIter.First();
					// Encode any parameters that exist.
					// A parameter consists of a Q-token and a Q-value
					while (!paramIter.AtEnd())
						{
						const CHeaderFieldParam* param = paramIter();
						if (param != NULL)			
							EncodeParameterL(*param, *encoder);
						++paramIter;
						}
					if (requireValueLength)
						encoder->EndValueLengthL();
					}
				}
			else
				User::Leave(KErrCorrupt);
			}

		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeTrailerL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	TInt partCount = 0;
	aHeader.BeginRawDataL();
	partIter.First();
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				TInt headerNameVal = EncodeFieldName(value.StrF());
				if (headerNameVal < KShortIntLimit && headerNameVal > KErrNotFound)
					encoder->AddShortIntL((TUint8)headerNameVal);
				else if (headerNameVal > 127)
					encoder->AddLongIntL(headerNameVal);
				else
					encoder->AddTokenTextL(value.StrF().DesC());
				}
			else
				User::Leave(KErrCorrupt);
			}

		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeWarningL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KTIntVal)
				{
				TUint8 warningNumber = 0;
				switch(value)
					{
					case 110:
						{
						warningNumber = 10;		// Warning code 110 is encoded as <Octet 10>
						} break;
					case 111:
						{
						warningNumber = 11;		// Warning code 111 is encoded as <Octet 11>
						} break;
					case 112:
						{
						warningNumber = 12;		// Warning code 112 is encoded as <Octet 12>
						} break;
					case 113:
						{
						warningNumber = 13;		// Warning code 113 is encoded as <Octet 13>
						} break;
					case 199:
					case 299:
						{
						warningNumber = 99;		// Warning codes 199 and 299 are encoded as <Octet 99>
						} break;
					case 214:
						{
						warningNumber = 14;		// Warning code 214 is encoded as <Octet 14>
						} break;
					default:
						break;
					}

				++partIter;
				if (partIter.AtEnd())	// Check to see if we have only one part
					encoder->AddShortIntL(warningNumber);
				else
					{
					encoder->StartValueLengthL();			// If there is more than one part then it is
					encoder->AddShortIntL(warningNumber);	// necessary to work out the value-length
					while (!partIter.AtEnd())
						{
						value = partIter()->Value();
						if(value.Type() == THTTPHdrVal::KStrFVal)
							{
							User::LeaveIfError(CheckTextString(value.StrF()));
							encoder->AddTextStringL(value.StrF().DesC());
							++partIter;
							}
						else
							User::Leave(KErrCorrupt);
						}
					encoder->EndValueLengthL();
					}
				}
			else
				User::Leave(KErrCorrupt);
			}
		aHeader.BeginRawDataL();	
		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		aHeader.CommitRawData();
		}
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeXWapApplicationIdL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				User::LeaveIfError(CheckTextString(value.StrF()));
				encoder->AddTextStringL(value.StrF().DesC());
				}
			else if (value.Type() == THTTPHdrVal::KTIntVal)
				{
				if (value.Int() < KShortIntLimit)
					encoder->AddShortIntL((TUint8)(value.Int()));
				else
					encoder->AddLongIntL(value.Int());
				}
			else
				User::Leave(KErrCorrupt);

			HBufC8* buffer = encoder->EndHeaderL();
			CleanupStack::PushL(buffer);
			TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
			aHeader.BeginRawDataL();
			aHeader.WriteRawDataL(data);
			CleanupStack::PopAndDestroy(buffer);
			aHeader.CommitRawData();
			}
		}
	CleanupStack::PopAndDestroy(encoder);
	}


//
// Generic methods
//

void CWspHeaderWriter::GenericEncodeDateL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	partIter.First();
	if (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KDateVal)
				{
				encoder->AddDateL(value.DateTime());
				HBufC8* buffer = encoder->EndHeaderL();
				CleanupStack::PushL(buffer);
				TPtrC8 data(buffer->Mid(EncodeHeaderNameL(aHeader)));
				aHeader.BeginRawDataL();
				aHeader.WriteRawDataL(data);
				CleanupStack::PopAndDestroy(buffer);
				aHeader.CommitRawData();
				}
			else
				User::Leave(KErrCorrupt);
			}
		}
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::GenericEncodeTextStringL(RHeaderField& aHeader) const
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	aHeader.BeginRawDataL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt token = EncodeFieldName(aHeader.Name());
	TInt partCount = 0;
	partIter.First();
	while (!partIter.AtEnd())
		{
		if (token == KErrNotFound)
			encoder->StartHeaderL(aHeader.Name().DesC());
		else
			encoder->StartHeaderL((TUint8)token);
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				{
				User::LeaveIfError(CheckTextString(value.StrF()));
				encoder->AddTextStringL(value.StrF().DesC());
				}
			else
				User::Leave(KErrCorrupt);
			}

		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;
		if (partCount == 1)
			startPos = EncodeHeaderNameL(aHeader);
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}

void CWspHeaderWriter::EncodeParameterL(const CHeaderFieldParam& aHeaderFieldParam, CWspHeaderEncoder& aEncoder) const
	{
	THTTPHdrVal value = aHeaderFieldParam.Value();
	TInt paramIndex = EncodeParameterTokenValue(aHeaderFieldParam.Name());
	if (paramIndex == KErrNotFound)		// This is an untyped parameter
		{
		// Append the name as a string
		aEncoder.AddTokenTextL(aHeaderFieldParam.Name().DesC());

		// Append the param value
		switch(value.Type())
			{
			case THTTPHdrVal::KTIntVal:
				{
				if (value.Int() < KShortIntLimit)
					aEncoder.AddShortIntL((TUint8)(value.Int()));
				else
					aEncoder.AddLongIntL(value.Int());
				} break;
			case THTTPHdrVal::KStrFVal:
				{
				User::LeaveIfError(CheckTextString(value.StrF()));
				aEncoder.AddTextStringL(value.StrF().DesC());
				} break;
			default:
				{
				User::Leave(KErrCorrupt);
				} break;
			}
		}
	else	// This is a typed parameter
		{
		// Check encoding version and adjust token as neccessary
		if (iCodec.GetWspVersion() > CWspHeaderCodec::EVersion1_3)
			{
			switch (paramIndex)
				{
				case 0x05:
					paramIndex = 0x17;	// Name-token is encoded as 0x17 in WSP version 1.4
					break;
				case 0x06:
					paramIndex = 0x18;	// Filename-token is encoded as 0x18 in WSP version 1.4
					break;
				case 0x0A:
					paramIndex = 0x19;	// Start-token is encoded as 0x19 in WSP version 1.4
					break;
				case 0x0B:
					paramIndex = 0x1A;	// Start-info-token is encoded as 0x1A in WSP version 1.4
					break;
				case 0x0C:
					paramIndex = 0x1B;	// Comment-token is encoded as 0x1B in WSP version 1.4
					break;
				case 0x0D:
					paramIndex = 0x1C;	// Domain-token is encoded as 0x1C in WSP version 1.4
					break;
				case 0x0F:
					paramIndex = 0x1D;	// Path-token is encoded as 0x1D in WSP version 1.4
					break;
				default:
					break;
				}
			}
		// Append the name as an encoded value
		aEncoder.AddShortIntL((TUint8)paramIndex);
		
		// Append the param value
		switch(paramIndex)
			{
			case WSPParam::EQ:
				{
				HBufC8* qValue = EncodeQValueL(value.StrF());
				if (qValue != NULL)
					{
					CleanupStack::PushL(qValue);
					aEncoder.AddDataL(*qValue);
					CleanupStack::PopAndDestroy(qValue);
					}
				} break;
			case WSPParam::ECharset:
				{
				if (value.StrF().Index(WSPStdConstants::Table) == WSPStdConstants::EAny)
					aEncoder.AddTokenL(128);	// '*' (Any-charset) is encoded as <Octet 128> 
				else
					{
					TInt charValue = GetCharacterSetValue(value.StrF());
					User::LeaveIfError(charValue);
					if (charValue < KShortIntLimit)
						aEncoder.AddShortIntL((TUint8)charValue);
					else
						aEncoder.AddLongIntL(charValue);
					}
				} break;
			case WSPParam::ELevel:
				{
				TUint8 versionValue = EncodeVersionValueL(value.StrF());
				if (versionValue == 0)
					{
					User::LeaveIfError(CheckTextString(value.StrF()));
					aEncoder.AddTextStringL(value.StrF().DesC());
					}
				else
					aEncoder.AddShortIntL(versionValue);
				} break;
			case WSPParam::EType:
			case WSPParam::EContentTypeType:
			case WSPParam::ESize:
			case WSPParam::EPadding:
			case WSPParam::ESEC:
			case WSPParam::EMaxAge:
				{
				if (value.Int() < KShortIntLimit)
					aEncoder.AddShortIntL((TUint8)value.Int());
				else
					aEncoder.AddLongIntL(value.Int());
				} break;
			case WSPParam::ENameDep:
			case WSPParam::EFilenameDep:
			case WSPParam::EStartDep:
			case WSPParam::EStartInfoDep:
			case WSPParam::ECommentDep:
			case WSPParam::EDomainDep:
			case WSPParam::EPathDep:
			case WSPParam::EName:
			case WSPParam::EFilename:
			case WSPParam::EStart:
			case WSPParam::EStartInfo:
			case WSPParam::EComment:
			case WSPParam::EDomain:
			case WSPParam::EPath:
			case WSPParam::EMAC:
				{
				TPtrC8 valueDes(value.StrF().DesC());
				if ((iCodec.GetWspVersion() < CWspHeaderCodec::EVersion1_4) || 
					(valueDes.Length() == 0) ||
					(valueDes[0] == 0x22))		// Check if we have a quoted-string
					{
					User::LeaveIfError(CheckTextString(value.StrF()));
					aEncoder.AddTextStringL(valueDes);
					}
				else
					{
					User::LeaveIfError(CheckTokenText(value.StrF()));
					aEncoder.AddTokenTextL(valueDes);
					}
				} break;
			case WSPParam::EDifferences:
				{
				TInt headerNameVal = EncodeFieldName(value.StrF());
				if (headerNameVal < KShortIntLimit && headerNameVal > KErrNotFound)
					aEncoder.AddShortIntL((TUint8)headerNameVal);
				else
					aEncoder.AddTokenTextL(value.StrF().DesC());
				} break;
			case WSPParam::ECreationDate:
			case WSPParam::EModificationDate:
			case WSPParam::EReadDate:
				{
				aEncoder.AddDateL(value.DateTime());
				} break;
			case WSPParam::ESecure:
				{
				aEncoder.AddTokenL(0);
				} break;
			default:
				{
				aEncoder.AddTokenTextL(value.StrF().DesC());
				} break;
			}
		}
	}

TInt CWspHeaderWriter::GetCharacterSetValue(RStringF aCharSet) const
	{
	TInt value = KErrNotFound;
	switch(aCharSet.Index(WSPCharacterSets::Table))
		{
		case WSPCharacterSets::EBig5:
			{
			value =  0x07EA;		// Character set big5 is encoded as 0x07EA
			} break;
		case WSPCharacterSets::EIso10646ucs2:
			{
			value =  0x03E8;		// Character set iso-10646-ucs-2 is encoded as 0x03E8 
			} break;
		case WSPCharacterSets::EIso88591:
			{
			value =  0x04;		// Character set iso-8859-1 is encoded as 0x04
			} break;
		case WSPCharacterSets::EIso88592:
			{
			value =  0x05;		// Character set iso-8859-2 is encoded as 0x05
			} break;
		case WSPCharacterSets::EIso88593:
			{
			value =  0x06;		// Character set iso-8859-3 is encoded as 0x06
			} break;
		case WSPCharacterSets::EIso88594:
			{
			value =  0x07;		// Character set iso-8859-4 is encoded as 0x07
			} break;
		case WSPCharacterSets::EIso88595:
			{
			value =  0x08;		// Character set iso-8859-5 is encoded as 0x08
			} break;
		case WSPCharacterSets::EIso88596:
			{
			value =  0x09;		// Character set iso-8859-6 is encoded as 0x09
			} break;
		case WSPCharacterSets::EIso88597:
			{
			value =  0x0A;		// Character set iso-8859-9 is encoded as 0x0A
			} break;
		case WSPCharacterSets::EIso88598:
			{
			value =  0x0B;		// Character set iso-8859-8 is encoded as 0x0B
			} break;
		case WSPCharacterSets::EIso88599:
			{
			value =  0x0C;		// Character set iso-8859-9 is encoded as 0x0C
			} break;
		case WSPCharacterSets::EShiftJIS:
			{
			value =  0x11;		// Character set shift_JIS is encoded as 0x11
			} break;
		case WSPCharacterSets::EUsAscii:
			{
			value =  0x03;		// Character set us-ascii is encoded as 0x03
			} break;
		case WSPCharacterSets::EUtf8:
		case WSPCharacterSets::EGsmDefaultAlphabet:
			{
			value = 0x6A;		// Character sets utf-8 and gsm-default-alphabet are
			} break;			// encoded as 0x6A
		default:
			// Use default value of KErrNotFound
			break;
		}
	return value;
	}

HBufC8* CWspHeaderWriter::EncodeQValueL(RStringF aQValue) const
	{
	TInt decPointPos = aQValue.DesC().Locate('.');
	if (decPointPos != KErrNotFound)
		{
		if ((decPointPos + 1) >= aQValue.DesC().Length())
			User::Leave(KErrCorrupt);
		TPtrC8 decValue(aQValue.DesC().Mid(decPointPos + 1));
		TLex8 intConvertDes(decValue);
		TInt convertedInt = 0;
		if (intConvertDes.Val(convertedInt) != KErrNone)
			User::Leave(KErrCorrupt);
		switch(decValue.Length())
			{
			case 1:
				convertedInt *= 10;
			case 2:
				++convertedInt;
				break;
			case 3:
				convertedInt += 100;
				break;
			default:
				User::Leave(KErrCorrupt);
				break;
			}
		return TWspPrimitiveEncoder::UintVarL(convertedInt);
		}
	return NULL;
	}

TUint8 CWspHeaderWriter::EncodeVersionValueL(RStringF aVersionValue) const
	{
	TPtrC8 version(aVersionValue.DesC());
	TInt versionLength = version.Length();
	if (versionLength == 0)
		User::Leave(KErrCorrupt);
	TInt decimalPos = version.Locate('.');
	TUint8 majorVersion = 0;
	TUint8 minorVersion = 0x0F;
	if (decimalPos != KErrNotFound)
		{
		if ((decimalPos + 1) == versionLength)
			User::Leave(KErrCorrupt);
		TPtrC8 minorDes(version.Mid(decimalPos + 1));
		TLex8 convertMinor(minorDes);
		TInt convertedMinor = 0;
		if (convertMinor.Val(convertedMinor) != KErrNone)
			User::Leave(KErrCorrupt);
		if (convertedMinor > 14)	// The minor version number must be within the range 0-14
			return 0;
		else
			minorVersion = (TUint8)convertedMinor;
		}
	TPtrC8 majorDes;
	if (decimalPos == KErrNotFound)
		majorDes.Set(version);
	else
		majorDes.Set(version.Left(decimalPos));

	TLex8 convertMajor(majorDes);
	TInt convertedInt = 0;
	if (convertMajor.Val(convertedInt) != KErrNone)
		User::Leave(KErrCorrupt);
	if (convertedInt > 7)			// The major version has a maximum value of 7
		return 0;
	else if (convertedInt > 0)		// and a minimum value of 1
		majorVersion = (TUint8)convertedInt;
	else
		User::Leave(KErrCorrupt);
		
	majorVersion <<= 4;								// Shift the bits across by four places so that
													// they are the most significant bits
	return (TUint8)(majorVersion | minorVersion);
	}

TInt CWspHeaderWriter::EncodeFieldName(RStringF aFieldName) const
	{
	TInt fieldName = aFieldName.Index(iStrTable);
	const CWspHeaderCodec::TWspVersion wspVersion = iCodec.GetWspVersion();

	// Check WSP version against encoded ranges
	switch(wspVersion)
		{
		case CWspHeaderCodec::EVersion1_1:
			{
			if(fieldName > 0x2E) // Highest header encoding is 0x2E in v1.1
				fieldName = KErrNotFound;
			} break;
		case CWspHeaderCodec::EVersion1_2:
			{
			if(fieldName > 0x37) // Highest header encoding is 0x2E in v1.2
				fieldName = KErrNotFound;
			} break;
		case CWspHeaderCodec::EVersion1_3:
			{
			if(fieldName > 0x43) // Highest header encoding is 0x2E in v1.3
				fieldName = KErrNotFound;
			} break;
		default: // version is 1.4 that can encode everything
			break;
		}

	// Make any value changes based on WSP version
	TInt name = fieldName;
	switch(fieldName)
		{
		case WSP::EAcceptCharsetDep:
		case WSP::EAcceptCharset:
			{
			if (wspVersion < CWspHeaderCodec::EVersion1_3)
				name = 0x01;		// Version 1.1 of Accept-Charset is encoded as 0x01
			else
				name = 0x3B;		// The current version (1.3) of Accept-Charset is encoded as 0x3B
			} break;
		case WSP::EAcceptEncodingDep:
		case WSP::EAcceptEncoding:
			{
			if (wspVersion < CWspHeaderCodec::EVersion1_3)
				name = 0x02;		// Version 1.1 of Accept-Encoding is encoded as 0x02
			else
				name = 0x3C;		// The current version (1.3) of Accept-Encoding is encoded as 0x3C
			} break;
		case WSP::ECacheControlDep:
		case WSP::ECacheControlDep2:
		case WSP::ECacheControl:
			{
			if (wspVersion < CWspHeaderCodec::EVersion1_3)
				name = 0x08;		// Version 1.1 of Cache-Control is encoded as 0x08
			else if (wspVersion == CWspHeaderCodec::EVersion1_3)
				name = 0x3D;		// Version 1.3 of Cache-Control is encoded as 0x3D
			else
				name = 0x47;		// The current version (1.4) of Cache-Control is encoded as 0x47
			} break;
		case WSP::EContentRangeDep:
		case WSP::EContentRange:
			{
			if (wspVersion < CWspHeaderCodec::EVersion1_3)
				name = 0x10;		// Version 1.1 of Content-Range is encoded as 0x10
			else
				name = 0x3E;		// The current version (1.3) of Content-Range is encoded as 0x3E
			} break;
		case WSP::EContentDispositionDep:
		case WSP::EContentDisposition:
			{
			if (wspVersion < CWspHeaderCodec::EVersion1_4)
				name = 0x2E;		// Version 1.1 of Content-Disposition is encoded as 0x2E
			else
				name = 0x45;		// The current version (1.4) of Content-Disposition is encoded as 0x45
			} break;
		case WSP::EProfileWarningDep:
		case WSP::EProfileWarning:
			{
			if (wspVersion < CWspHeaderCodec::EVersion1_4)
				name = 0x37;		// Version 1.2 of Profile-Warning is encoded as 0x37
			else
				name = 0x44;		// The current version (1.4) of Profile-Warning is encoded as 0x44
			} break;
		default:
			// Use default return name of fieldName.
			break;
		}
	return name;
	}

TInt CWspHeaderWriter::EncodeHeaderNameL(RHeaderField& aHeader) const
	{
	TInt headerLength = 0;
	TInt headerNameVal = EncodeFieldName(aHeader.Name());
	if (headerNameVal == KErrNotFound)
		{
		User::LeaveIfError(CheckTokenText(aHeader.Name()));
		headerLength = aHeader.Name().DesC().Length();
		}
	return ++headerLength;
	}

TInt CWspHeaderWriter::CheckTokenText(RStringF aTokenText) const
	{
	const TUint8 KMinValidCharValue = 32;				// Minimum ASCII value for a valid character
	const TUint8 KMaxValidCharValue = 126;				// Maximum ASCII value for a valid character
	_LIT8(KTxtSeparators, "()<>@,;:\\\"/[]?={} ");		// Separator characters as defined in RFC2616
	
	// Go through each character in the string ensuring that it is within the acceptable ASCII range
	// and then check that the string does not contain any seperator characters.
	const TInt tokenTextLength = aTokenText.DesC().Length();
	for	(TInt ii = 0; ii < tokenTextLength; ++ii)
		{
		TUint8 currentChar = aTokenText.DesC()[ii];
		if ((currentChar < KMinValidCharValue) && (currentChar > KMaxValidCharValue))
			return KErrCorrupt;
		if (KTxtSeparators().Locate(currentChar) != KErrNotFound)
			return KErrCorrupt;
		}
	return KErrNone;
	}

TInt CWspHeaderWriter::CheckTextString(RStringF aTextString) const
	{
	const TUint KHorizTab = 9;
	const TUint KLinefeed = 10;
	const TUint KCarriageReturn = 13;
	const TUint KSpace = 32;
	const TInt textStringLength = aTextString.DesC().Length();
	for	(TInt ii = 0; ii < textStringLength; ++ii)
		{
		TUint8 currentChar = aTextString.DesC()[ii];
		// Check for an ASCII control character (octets 0-31) or
		// an ASCII space character (octet 32), but allow ASCII linear white spce (octets 9,
		// 10, 13 and 32)
		if (currentChar < 33 && !(currentChar == KHorizTab || currentChar == KLinefeed 
			|| currentChar == KCarriageReturn || currentChar == KSpace))
			{
			return KErrCorrupt;
			}
		// Check for an ASCII delete character (octet 127)
		if (currentChar == 127)
			return KErrCorrupt;
		}
	return KErrNone;
	}

TInt CWspHeaderWriter::EncodeContentTypeValue(THTTPHdrVal& aContentType) const
	{
	TInt encodedToken = aContentType.StrF().Index(WSPContentTypes::Table);
	if( encodedToken > KErrNotFound )
		{
		// Apply some WSP version control on encoded token
		switch (iCodec.GetWspVersion())
			{
			case CWspHeaderCodec::EVersion1_1:
				{
				if( encodedToken > 0x2D ) // Highest content-type encoding is 0x2D in v1.1
					encodedToken = KErrNotFound;
				} break;
			case CWspHeaderCodec::EVersion1_2:
				{
				if( encodedToken > 0x34 ) // Highest content-type encoding is 0x34 in v1.2
					encodedToken = KErrNotFound;
				} break;
			case CWspHeaderCodec::EVersion1_3:
				{
				if( encodedToken > 0x36 ) // Highest content-type encoding is 0x36 in v1.3
					encodedToken = KErrNotFound;
				} break;
			default: // version is 1.4 that can encode all content-types in string table
				break;
			}
		}
	return encodedToken;
	}

TInt CWspHeaderWriter::EncodeParameterTokenValue(RStringF aParameterName) const
	{
	TInt encodedToken = aParameterName.Index(WSPParam::Table);
	if( encodedToken > KErrNotFound )
		{
		// Apply some WSP version control on encoded token
		switch (iCodec.GetWspVersion())
			{
			// Check for maximum encoding token for current WSP version
			case CWspHeaderCodec::EVersion1_1:
				{
				if( encodedToken > 0x08 ) // Highest parameter token encoding is 0x08 in v1.1
					encodedToken = KErrNotFound;
				} break;
			case CWspHeaderCodec::EVersion1_2:
				{
				if( encodedToken > 0x0B ) // Highest parameter token encoding is 0x0b in v1.2
					encodedToken = KErrNotFound;
				} break;
			case CWspHeaderCodec::EVersion1_3:
				{
				if( encodedToken > 0x10 ) // Highest parameter token encoding is 0x10 in v1.3
					encodedToken = KErrNotFound;
				} break;
			default: // version is 1.4 that can encode all parameter tokens in string table
				break;
			}
		}
	return encodedToken;
	}

// Implementation of CWspDefaultHdrWriter
//-------------------------------------------------------------------------------

CWspDefaultHdrWriter* CWspDefaultHdrWriter::NewL(RStringPool aStrPool)
	{
	return new(ELeave)CWspDefaultHdrWriter(aStrPool);
	}

CWspDefaultHdrWriter* CWspDefaultHdrWriter::NewLC(RStringPool aStrPool)
	{
	CWspDefaultHdrWriter* self = CWspDefaultHdrWriter::NewL(aStrPool);
	CleanupStack::PushL(self);
	return self;
	}

CWspDefaultHdrWriter::CWspDefaultHdrWriter(RStringPool aStrPool)
	: iStrPool(aStrPool)
	{
	}

CWspDefaultHdrWriter::~CWspDefaultHdrWriter()
	{
	}

void CWspDefaultHdrWriter::EncodeHeaderL(RHeaderField& aHeader)
	{
	THeaderFieldPartIter partIter = aHeader.PartsL();
	CWspHeaderEncoder* encoder = CWspHeaderEncoder::NewL();
	CleanupStack::PushL(encoder);
	TInt partCount = 0;
	aHeader.BeginRawDataL();
	partIter.First();
	// Loop through all the parts and build up the raw data
	while (!partIter.AtEnd())
		{
		encoder->StartHeaderL(aHeader.Name().DesC());
		const CHeaderFieldPart* part = partIter();
		if (part != NULL)
			{
			++partCount;
			THTTPHdrVal value = part->Value();
			if (value.Type() == THTTPHdrVal::KStrFVal)
				encoder->AddTextStringL(value.StrF().DesC());
			else
				User::Leave(KErrCorrupt);
			}

		HBufC8* buffer = encoder->EndHeaderL();
		CleanupStack::PushL(buffer);
		TInt startPos = 0;
		// If this is the first part then don't include the header name
		if (partCount == 1)
			startPos = (aHeader.Name().DesC().Length()) + 1;
		TPtrC8 data(buffer->Mid(startPos));
		aHeader.WriteRawDataL(data);
		CleanupStack::PopAndDestroy(buffer);
		++partIter;
		}
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(encoder);
	}
