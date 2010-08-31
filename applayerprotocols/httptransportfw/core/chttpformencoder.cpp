// CHTTPFormEncoder.cpp

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
#include <chttpformencoder.h> // Class signature
#include <httperr.h>

// Local includes


const TInt KFormEncoderBufferIncrementSize = 256;

EXPORT_C CHTTPFormEncoder* CHTTPFormEncoder::NewL()
	{
	CHTTPFormEncoder* self = new (ELeave) CHTTPFormEncoder();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CHTTPFormEncoder::~CHTTPFormEncoder()
	{
	__ASSERT_DEBUG(iState != ESupplyingData,HTTPPanic::Panic(HTTPPanic::EHTTPFormDataUnreleased));
	delete iData;
	}

CHTTPFormEncoder::CHTTPFormEncoder() 
	{}

void CHTTPFormEncoder::ConstructL()
	{
	iData = HBufC8::NewL(KFormEncoderBufferIncrementSize);
	}


TBool CHTTPFormEncoder::GetNextDataPart(TPtrC8& aDataPart)
	{
	aDataPart.Set(*iData);
	iState = ESupplyingData;
	return ETrue;
	}


 void CHTTPFormEncoder::ReleaseData()
	{
	iState = EDataReleased;
	}

	
TInt CHTTPFormEncoder::OverallDataSize()
	{
	iState = ESuppliedSize;
	return iData->Size();
	}
	
TInt CHTTPFormEncoder::Reset()
	{
    iState = EAllowAppendData;
	return KErrNone;
	}

EXPORT_C void CHTTPFormEncoder::AddFieldL(const TDesC8& aFieldName, const TDesC8& aFieldValue)
	{
	__ASSERT_DEBUG(iState==EAllowAppendData, HTTPPanic::Panic(HTTPPanic::EHTTPFormSupplyingData));
	if (!aFieldName.Length())
		User::Leave(KErrArgument);

	// The maximum size is if every single character would have to be encoded as its hex equivalent. 
	// That is %HH. Also need to add room for the '=' and '&' field separator
	TInt reqSize = (aFieldName.Length() + aFieldValue.Length())*3 +2;

	if (!iData)
		iData = HBufC8::NewL(reqSize);
	else
		{
		TInt currentLength = iData->Length();
		if (reqSize + currentLength > iData->Des().MaxLength())
			{
			TInt amountToGrow = reqSize < KFormEncoderBufferIncrementSize? KFormEncoderBufferIncrementSize : reqSize;
			iData = iData->ReAllocL(amountToGrow+currentLength);
			}
		}

	if (iData->Length() >0)
		iData->Des().Append('&');
	AppendData(aFieldName);
	iData->Des().Append('=');
	AppendData(aFieldValue);
	}


TBool CHTTPFormEncoder::IsAlphaNumeric(TUint8 aChar)
	{
	return (aChar >= 'a' && aChar <= 'z') ||
		(aChar >= 'A' && aChar <= 'Z') ||
		(aChar >= '0' && aChar <='9');
	}


/** Appends a non alpha numeric character by converting it to %HH where HH is the Hexadecmial representation
of the character
*/
void CHTTPFormEncoder::AppendSpecialChar(TUint8 aChar)
	{
	TPtr8 dataPtr = iData->Des();
	dataPtr.Append('%');

	TUint hexChar = aChar/16;
	hexChar = hexChar > 9 ? hexChar + 55 : hexChar + 48;
	dataPtr.Append(hexChar);

	hexChar= aChar%16;
	hexChar = hexChar > 9 ? hexChar + 55 : hexChar + 48;
	dataPtr.Append(hexChar);
	}

void CHTTPFormEncoder::AppendData(const TDesC8& aData)
	{
	TBool hadCR  = EFalse;                                
	TInt length = aData.Length();
	for (TInt ii=0; ii<length; ++ii)
		{
		TPtr8 dataPtr = iData->Des();
		TUint8 charToProcess = aData[ii];

		if (charToProcess == 0x0d)	// turn CR into CR/LF
			{
			hadCR = ETrue;
			AppendSpecialChar(charToProcess);
			AppendSpecialChar(0x0a);
			continue;
			}

		if (hadCR)
			{
			hadCR = EFalse;
			if (charToProcess == 0x0a) // LF after a CR, so skip as already appended
				continue;
			}

		// handle spaces -> become +
		if (charToProcess == ' ')
			dataPtr.Append('+');
		// handle normal character
		else if (IsAlphaNumeric(charToProcess))
			dataPtr.Append((TChar)charToProcess);
		else 
			AppendSpecialChar(charToProcess); // encode to %HH
		//
		}
	}
