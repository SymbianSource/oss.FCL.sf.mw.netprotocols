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
// The context for the unit tests upon the CWspHeaderReader class methods.
// 
//

/**
 @file WspHeaderReaderUnitTestContext.inl
*/

// User includes
#include "WspHeaderReaderTransitions.h"
#include "cheaders.h"
#include "cheaderfield.h"

// System includes
#include <wspstringconstants.h>

_LIT8(KTxtTestHeader, "TestHeader");

// ______________________________________________________________________________
//
inline CWspHeaderReader_UnitTestContext::CWspHeaderReader_UnitTestContext(CDataLogger& aDataLogger,
												MStateAccessor& aStateAccessor,
												MTransitionObserver& aObserver,
												const TStringTable& aStrTable)
: CUnitTestContext(aDataLogger, aStateAccessor, aObserver), iStrTable(aStrTable)
	{
	//Do nothing
	}

inline CWspHeaderReader_UnitTestContext::~CWspHeaderReader_UnitTestContext()
	{
	delete iExpectedData;
	delete iOutputData;
	delete iTestHeader;
	delete iHeaderField;
	delete iHeader;
	delete iCodec;
	iStrPool.Close();
	}

inline void CWspHeaderReader_UnitTestContext::ConstructL(TInt aHeaderName)
	{
	iHeaderToken = aHeaderName;
	iStrPool.OpenL(iStrTable);
	iCodec = CWspHeaderCodec::NewL(iStrPool, iStrTable);
	REINTERPRET_CAST(CWspHeaderCodec*, iCodec)->SetWspVersion(CWspHeaderCodec::EVersion1_4);
	iHeader = CHeaders::NewL(*iCodec);
	if( iHeaderToken == KErrNotFound )
		{
		RStringF testHeader = iStrPool.OpenFStringL(KTxtTestHeader());
		CleanupClosePushL(testHeader);
		iHeaderField = CHeaderField::NewL(testHeader, *iHeader);
		CleanupStack::PopAndDestroy(&testHeader);
		}
	else
		{
		iHeaderField = CHeaderField::NewL(iStrPool.StringF(aHeaderName, iStrTable), *iHeader);
		}
	iTestHeader = new(ELeave) RHeaderField(*iHeaderField);
	}

inline void CWspHeaderReader_UnitTestContext::SetOutputL()
	{
	_LIT8(KPartSeparator, ",");
	_LIT8(KParamSeparator, ";");
	_LIT8(KParamValueSeparator, "=");
	// "Sun, 06 Nov 1994 08:49:37 GMT"  ; RFC 822, updated by RFC 1123
	_LIT(KDateFormatRfc1123,"%*E,%D%*N%Y %1 %2 %3 %H:%T:%S GMT");

	TBuf8<512> data;
	THeaderFieldPartIter iter  = iTestHeader->PartsL();
	iter.First();
	TInt partCount = 0;
	while( !(iter.AtEnd()) )
		{
		if( partCount > 0 )
			data.Append(KPartSeparator);

		const CHeaderFieldPart* fieldPart = iter();
		THTTPHdrVal hdrVal = fieldPart->Value();
		switch( hdrVal.Type() )
			{
			case THTTPHdrVal::KStrFVal:
				{
				data.Append(hdrVal.StrF().DesC());
				} break;
			case THTTPHdrVal::KStrVal:
				{
				data.Append(hdrVal.Str().DesC());
				} break;
			case THTTPHdrVal::KTIntVal:
				{
				TBuf8<25> convertNum;
				convertNum.Num(hdrVal.Int());
				data.Append(convertNum);
				} break;
			case THTTPHdrVal::KDateVal:
				{
				// Formate the date string in GMT
				TBuf<40> dateTimeStringRfc1123;
				TTime t(hdrVal.DateTime());
				t.FormatL(dateTimeStringRfc1123,KDateFormatRfc1123);
				// convert from unicode
				TBuf8<40> dateTimeString8;
				dateTimeString8.Copy(dateTimeStringRfc1123);
				data.Append(dateTimeString8);
				} break;
			default:
				break;
			}
		++iter;
		++partCount;

		// Check how many parameters there are on this part
		TInt numParams = fieldPart->NumParameters();
		// loop through all the paramters and them to the descriptor
		for( TInt i=0; i<numParams; ++i )
			{
			data.Append(KParamSeparator);
			CHeaderFieldParam* fieldParam = fieldPart->Parameter(i);
			data.Append(fieldParam->Name().DesC());
			if(fieldParam->Value().Type() == THTTPHdrVal::KStrFVal)
				{
				if(fieldParam->Value().StrF().DesC().Length()!=0)
					data.Append(KParamValueSeparator);
				}
			else
				data.Append(KParamValueSeparator);

			THTTPHdrVal paramVal = fieldParam->Value();
			switch( paramVal.Type() )
				{
				case THTTPHdrVal::KStrFVal:
					{
					data.Append(paramVal.StrF().DesC());
					} break;
				case THTTPHdrVal::KStrVal:
					{
					data.Append(paramVal.Str().DesC());
					} break;
				case THTTPHdrVal::KTIntVal:
					{
					TBuf8<25> convertNum;
					convertNum.Num(paramVal.Int());
					data.Append(convertNum);
					} break;
				case THTTPHdrVal::KDateVal:
					{
					// Formate the date string in GMT
					TBuf<40> dateTimeStringRfc1123;
					TTime t(paramVal.DateTime());
					t.FormatL(dateTimeStringRfc1123,KDateFormatRfc1123);
					// convert from unicode
					TBuf8<40> dateTimeString8;
					dateTimeString8.Copy(dateTimeStringRfc1123);
					data.Append(dateTimeString8);
					} break;
				default:
					break;
				}
			}
		}

	// We have the data converted to text form so now add to the context
	iOutputData = data.AllocL();
	}

inline void CWspHeaderReader_UnitTestContext::SetExpectedL(const TDesC8& aExpectedData)
	{
	iExpectedData = aExpectedData.AllocL();
	}

inline void CWspHeaderReader_UnitTestContext::SetTrapOutputL()
	{
	CHeaderField* headerField = CHeaderField::NewL(iStrPool.StringF(iHeaderToken, iStrTable), *iHeader);
	CleanupStack::PushL(headerField);
	RHeaderField testHeader(*headerField);
	testHeader.BeginRawDataL();
	TPtrC8 rawData;
	iTestHeader->RawDataL(rawData);
	testHeader.WriteRawDataL(rawData);
	testHeader.CommitRawData();
	TRAPD(err, testHeader.PartsL());
	CleanupStack::PopAndDestroy(headerField);
	TBuf8<25> convertNum;
	convertNum.Num(err);
	iOutputData = convertNum.AllocL();
	}
