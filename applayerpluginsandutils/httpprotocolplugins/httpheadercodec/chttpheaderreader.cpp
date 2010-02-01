// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "chttpheaderreader.h"

#include <inetprottextutils.h>
#include <tinternetdate.h>
#include <http/rhttpsession.h>
#include "CHeaderField.h"


CHttpHeaderReader::CHttpHeaderReader(RStringPool aStrPool)
	: iStrPool(aStrPool), iStringTable(RHTTPSession::GetTable())
	{
	}

/** Generic Decoding function that splits headers into parts on a character separator */
void CHttpHeaderReader::DecodeGenericL(RHeaderField& aHeader, const TDesC8& aSeparator) const
	{
	TPtrC8 rawData;
	aHeader.RawDataL(rawData);
	TInt remaining = rawData.Length();
	TPtrC8 token;
	TInt tokensFound =0;
	while (remaining > 0)
		{
		remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, token, aSeparator);
		SetNewFStringPartL(aHeader, tokensFound, token);
		++tokensFound;
		if(remaining > 0)
			{
			remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, token, aSeparator);
			SetNewFStringPartL(aHeader, tokensFound, token);
			++tokensFound;
			}
		}
	}

// Helper to DoUserAgentL(), etc., which take token lists with arbitrary separators
void CHttpHeaderReader::DecodeTokenListHeaderL(RHeaderField& aHeader, const TDesC8& aSep) const
	{
	TPtrC8 rawData;
	TPtrC8 token;

	aHeader.RawDataL(rawData);

	TUint remaining = rawData.Length();
	TInt tokensFound=0;
	while (remaining > 0)
		{
		remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, token, aSep);
		InetProtTextUtils::RemoveWhiteSpace(token, InetProtTextUtils::ERemoveBoth);
		SetNewFStringPartL(aHeader, tokensFound, token);
		tokensFound++;
		if(remaining > 0)
			{
			remaining -= InetProtTextUtils::ExtractNextTokenFromList(rawData, token, aSep);
			InetProtTextUtils::RemoveWhiteSpace(token, InetProtTextUtils::ERemoveBoth);
			SetNewFStringPartL(aHeader, tokensFound, token);
			tokensFound++;
			}
		}
	}

void CHttpHeaderReader::DecodeGenericNumberL(RHeaderField& aHeader) const
	{
	TPtrC8 buffer;
	aHeader.RawDataL(buffer);
	TInt number = KErrNotFound;
	
	TInt decimalPos = buffer.Locate('.');
	if(decimalPos == 0)
		{
		// first character is decimal. So, set the value as zero.
		SetNewIntegerPartL(aHeader, 0, 0);
		}
	else
	   	{
		// Search for '\n' separator. In the case when a duplicate header has been received,
		// only use the fist instance of the valid data.
		TInt newLinePos = buffer.Locate('\n');
		if (newLinePos != KErrNotFound)
			{
			buffer.Set(buffer.Left(newLinePos));
			}
		
		TInt value = KErrNotFound;
		TInt ret = InetProtTextUtils::ConvertDescriptorToInt(buffer, value);
		if ( ret > KErrNone ) 
			{
			// Extract an integer.  Do not permit terminators other than WS or EOL.
			InetProtTextUtils::ExtractIntegerValueL(buffer, number, EFalse);	
			}
		SetNewIntegerPartL(aHeader, 0, number); // part 0, i.e. the first (and only) part
	   	}
	}

CHeaderFieldPart* CHttpHeaderReader::SetNewIntegerPartL(RHeaderField& aHeader, TInt aPartIndex, TInt aValue) const
	{
	THTTPHdrVal partVal(aValue);
	CHeaderFieldPart* part = SetNewPartL(aHeader, aPartIndex, partVal);
	return part;
	}

CHeaderFieldPart* CHttpHeaderReader::SetNewDatePartL(RHeaderField& aHeader, TInt aPartIndex, TDateTime& aDate) const
	{
	THTTPHdrVal partVal(aDate);
	CHeaderFieldPart* part = SetNewPartL(aHeader, aPartIndex, partVal);
	return part;
	}

CHeaderFieldPart* CHttpHeaderReader::SetNewFStringPartL(RHeaderField& aHeader, TInt aPartIndex, TPtrC8 aValue) const
	{
	RStringF partStr = iStrPool.OpenFStringL(aValue);
	CleanupClosePushL(partStr);
	THTTPHdrVal partVal(partStr);
	CHeaderFieldPart* part = SetNewPartL(aHeader, aPartIndex, partVal);
	CleanupStack::PopAndDestroy(&partStr);
	return part;
	}
CHeaderFieldPart* CHttpHeaderReader::SetNewStringPartL(RHeaderField& aHeader, TInt aPartIndex, TPtrC8 aValue) const
	{
	RString partStr = iStrPool.OpenStringL(aValue);
	CleanupClosePushL(partStr);
	THTTPHdrVal partVal(partStr);
	CHeaderFieldPart* part = SetNewPartL(aHeader, aPartIndex, partVal);
	CleanupStack::PopAndDestroy(&partStr);
	return part;
	}

CHeaderFieldPart* CHttpHeaderReader::SetNewPartL(RHeaderField& aHeader, TInt aPartIndex, THTTPHdrVal& aPartVal) const
	{
	CHeaderFieldPart* part = CHeaderFieldPart::NewL(aPartVal);
	CleanupStack::PushL(part);
	aHeader.SetPartL(part, aPartIndex);
	CleanupStack::Pop(part);
	return part;
	}
	
void CHttpHeaderReader::SetParamNameAndValueL(CHeaderFieldPart& aHeaderPart, const TDesC8& aParam, TBool aQuotedValueAllowed) const
 	{
	// Extract the name and value of the parameter. 
	TPtrC8 paramName;
	TPtrC8 paramValue;

	TInt equalPos = aParam.Locate('=');
	if (equalPos == KErrNotFound)
		{
		// If there is no value then set an empty parameter
		paramName.Set(aParam);
		paramValue.Set(KNullDesC8);
		}
	else
		{
		paramName.Set(aParam.Left(equalPos));
		InetProtTextUtils::RemoveWhiteSpace(paramName, InetProtTextUtils::ERemoveBoth);		
		paramValue.Set(aParam.Right(aParam.Length() - (equalPos+1)));
		InetProtTextUtils::RemoveWhiteSpace(paramValue, InetProtTextUtils::ERemoveBoth);		
		}

	TPtrC8 unquotedParamValue;
	if (aQuotedValueAllowed && paramValue.Length() > 0 && paramValue[0] == '"')
		{
		InetProtTextUtils::ExtractQuotedStringL(paramValue, unquotedParamValue);
		}				
	else
		{
		unquotedParamValue.Set(paramValue);
		}
 
 
	SetNewFStringParamL(aHeaderPart, paramName, unquotedParamValue);
	}

CHeaderFieldParam* CHttpHeaderReader::SetNewFStringParamL(CHeaderFieldPart& aHeaderPart, TPtrC8 aParamName, TPtrC8 aParamValue) const
	{
	RStringF paramNameStr = iStrPool.OpenFStringL(aParamName);
	CleanupClosePushL(paramNameStr);
	RStringF paramValStr = iStrPool.OpenFStringL(aParamValue);
	CleanupClosePushL(paramValStr);
	THTTPHdrVal paramVal(paramValStr);
	CHeaderFieldParam* param = CHeaderFieldParam::NewL(paramNameStr, paramVal);
	CleanupStack::PushL(param);
	aHeaderPart.AddParamL(param);
	CleanupStack::Pop(param);
	CleanupStack::PopAndDestroy(2, &paramNameStr); // and paramValStr
	return param;
	}


CHeaderFieldParam* CHttpHeaderReader::SetNewStringParamL(CHeaderFieldPart& aHeaderPart, TPtrC8 aParamName, TPtrC8 aParamValue) const
	{
	RStringF paramNameStr = iStrPool.OpenFStringL(aParamName);
	CleanupClosePushL(paramNameStr);
	RString paramValStr = iStrPool.OpenStringL(aParamValue);
	CleanupClosePushL(paramValStr);
	THTTPHdrVal paramVal(paramValStr);
	CHeaderFieldParam* param = CHeaderFieldParam::NewL(paramNameStr, paramVal);
	CleanupStack::PushL(param);
	aHeaderPart.AddParamL(param);
	CleanupStack::Pop(param);
	CleanupStack::PopAndDestroy(2, &paramNameStr); // and paramValStr
	return param;
	}

CHeaderFieldParam* CHttpHeaderReader::SetNewIntegerParamL(CHeaderFieldPart& aHeaderPart, TPtrC8 aParamName, TPtrC8 aParamValue) const
    {
	RStringF paramNameStr = iStrPool.OpenFStringL(aParamName);
	CleanupClosePushL(paramNameStr);
	TInt intParamVal;
	InetProtTextUtils::ConvertDescriptorToInt(aParamValue, intParamVal);
	THTTPHdrVal paramVal(intParamVal);
	CHeaderFieldParam* param = CHeaderFieldParam::NewL(paramNameStr, paramVal);
	CleanupStack::PushL(param);
	aHeaderPart.AddParamL(param);
	CleanupStack::Pop(param);
	CleanupStack::PopAndDestroy(&paramNameStr);
	return param;
   } 

void CHttpHeaderReader::DecodeDateL(RHeaderField& aHeader) const
	{
	// RFC2616, section 14.18
	// Date  = "Date" ":" HTTP-date
	//
	// An example is
	// Date: Tue, 15 Nov 1994 08:12:31 GMT

	TPtrC8 buffer;
	aHeader.RawDataL(buffer);
	
    // Search for '\n' separator. In the case when a duplicate header has been received,
	// only use the fist instance of the valid data.
	TInt newLinePos = buffer.Locate('\n');
	if (newLinePos != KErrNotFound)
		{
		buffer.Set(buffer.Left(newLinePos));
		}
		
	TInternetDate date;
	date.SetDateL(buffer);
	TDateTime dateTime = date.DateTime();
	SetNewDatePartL(aHeader, 0, dateTime); // part 0, i.e. the first (and only) part
	}


TInt CHttpHeaderReader::GetParamNameAndValueL(TPtrC8& aBuffer, TPtrC8& aName, TPtrC8& aValue, TInt aErrorCode) const
	{
	TInt bytesConsumed=0;
			
	TInt equalsPos = (aBuffer.Locate('=') );
	if (equalsPos==KErrNotFound)
		User::Leave(aErrorCode);

	aName.Set(aBuffer.Left(equalsPos));
	InetProtTextUtils::RemoveWhiteSpace(aName, InetProtTextUtils::ERemoveBoth);

	TPtrC8 value(aBuffer.Mid(equalsPos+1));
	InetProtTextUtils::RemoveWhiteSpace(value, InetProtTextUtils::ERemoveBoth);

	// Obtain the parameter value. It is a string which
	// may or may not be quoted. 
	if (value.Length()==0)
		User::Leave(aErrorCode);

	if (value[0] == '"')
		{
		bytesConsumed += InetProtTextUtils::ExtractQuotedStringL(value, aValue);
		}
	else
		{
		aValue.Set(value);
		}

	return bytesConsumed;
	}
