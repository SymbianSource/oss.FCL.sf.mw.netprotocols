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

#include "chttpheaderwriter.h"

#include <tinternetdate.h>
#include <httpstringconstants.h>
#include <http/rhttpsession.h>
#include <httperr.h>

#include "CHeaderField.h"

_LIT8(KCommaSpaceSep,", ");
_LIT8(KSemiSpaceSep,"; ");

const TInt KMaxNumContentLengthDigits = 10;
const TInt KMaxNumQDigits = 5;

CHttpHeaderWriter::CHttpHeaderWriter(RStringPool aStringPool)
: CHeaderWriter(), iStringPool(aStringPool), iStringTable(RHTTPSession::GetTable())
	{
	}

void CHttpHeaderWriter::EncodeGenericNumberHeaderL(RHeaderField& aHeader, TInt aErrorCodeToUseOnFailure) const
	{
	// header = <header> ":" 1*DIGIT
	THeaderFieldPartIter iter1 = aHeader.PartsL();
	iter1.First();
	if (!iter1.AtEnd())
		{
		const CHeaderFieldPart* part = iter1();
		if (part)
			{
			THTTPHdrVal pt1val = part->Value();
			if (pt1val.Type() == THTTPHdrVal::KTIntVal)
				{
				TBuf8<KMaxNumContentLengthDigits> lenBuf;
				lenBuf.Num(pt1val.Int());
				aHeader.BeginRawDataL();
				aHeader.WriteRawDataL(lenBuf);
				aHeader.CommitRawData();
				return;
				}
			}
		}
	User::Leave(aErrorCodeToUseOnFailure);
	}

void CHttpHeaderWriter::EncodeGenericDateL(RHeaderField& aHeader, TInt aErrorCodeToUseOnFailure) const
	{
	// <header> = <header> ":" HTTP-date

	THeaderFieldPartIter iter1 = aHeader.PartsL();
	iter1.First();
	if (iter1.AtEnd())
		User::Leave(aErrorCodeToUseOnFailure);
	const CHeaderFieldPart* part = iter1();
	if (part == NULL)
		User::Leave(aErrorCodeToUseOnFailure);
	THTTPHdrVal value = part->Value();
	if (value.Type() != THTTPHdrVal::KDateVal)
		User::Leave(aErrorCodeToUseOnFailure);


	
	TInternetDate date(value);
	HBufC8* internetDate = date.InternetDateTimeL(TInternetDate::ERfc1123Format);
	CleanupStack::PushL(internetDate);
	aHeader.BeginRawDataL();
	aHeader.WriteRawDataL(*internetDate);
	aHeader.CommitRawData();
	CleanupStack::PopAndDestroy(internetDate);
	}


// Helper to DoAcceptL(), DoAcceptCharsetL(), DoAcceptLanguageL() and DoAcceptEncodingL() for comma-separated
// lists incorporating q-values..
void CHttpHeaderWriter::DoGeneralAcceptHeaderL(TInt aNormalHeaderLen, RHeaderField& aHeader, RStringF /*aFieldType*/, TInt aErrorCode) const
	{
	THeaderFieldPartIter iter = aHeader.PartsL();
	iter.First();
	if (iter.AtEnd())
		User::Leave(aErrorCode);

	// Any header field-specific variations may be made by checking aFieldType...

	aHeader.BeginRawDataL(aNormalHeaderLen);
	do
		{
		// Write raw data for the StringF component in the next part
		const CHeaderFieldPart* part = WriteRawStrFPartL(aHeader, iter, aErrorCode);

		// Check for parameters
		THeaderFieldParamIter iter2 = part->Parameters();
		iter2.First();
		while (!iter2.AtEnd())
			{
			// Got a parameter - write a semicolon separator
			aHeader.WriteRawDataL(KSemiSpaceSep);
			const CHeaderFieldParam* param = iter2();
			if (!param)
				User::Leave(aErrorCode);

			// If it's the 'q' parameter we need to decode the q-value
			RStringF qStr = iStringPool.StringF(HTTP::EQ, iStringTable);
			THTTPHdrVal paramVal = param->Value();
			if (param->Name() == qStr)
				{
				// Get the q value
				if (paramVal.Type() != THTTPHdrVal::KTIntVal)
					User::Leave(KErrHttpEncodeQValue);
				TInt val = paramVal.Int();
				THTTPHdrVal::TQConv q(val);
				TBuf8<KMaxNumQDigits> qDesC;
				TRealFormat realFt(3); // allow 3 decimal places
				// always use a decimal separator rather than the one supplied 
				// by the current locale
				realFt.iPoint = TChar('.'); //define decimal separator char
				qDesC.Num(q, realFt);
				aHeader.WriteRawDataL('q');
				aHeader.WriteRawDataL('=');
				aHeader.WriteRawDataL(qDesC);
				}
			else
				{
				// Extension parameter - handle strings only
				if (paramVal.Type() != THTTPHdrVal::KStrFVal)
					User::Leave(aErrorCode);
				RStringF paramName = iStringPool.StringF(param->Name());
				aHeader.WriteRawDataL(paramName.DesC());
				aHeader.WriteRawDataL('=');
				aHeader.WriteRawDataL(paramVal.StrF().DesC());
				}

			// Move onto the next parameter,
			++iter2;
			}

		// If there's another media field we need a separator
		++iter;
		if (!iter.AtEnd())
			aHeader.WriteRawDataL(KCommaSpaceSep);
		}while (!iter.AtEnd());

	// Done
	aHeader.CommitRawData();
	}

// Helper to DoConnectionL(), DoContentEncodingL(), DoTransferEncodingL(), etc., which take  comma-separated
// token lists.
void CHttpHeaderWriter::DoTokenCsvListHeaderL(RHeaderField& aHeader, TInt aErrorCode) const
	{
	DoTokenListHeaderL(aHeader, KCommaSpaceSep(), aErrorCode);
	}

// Helper to DoUserAgentL(), etc., which take token lists with arbitrary separators
void CHttpHeaderWriter::DoTokenListHeaderL(RHeaderField& aHeader, const TDesC8& aSep, TInt aErrorCode) const
	{
	THeaderFieldPartIter iter = aHeader.PartsL();
	iter.First();
	if (iter.AtEnd())
		User::Leave(aErrorCode);

	aHeader.BeginRawDataL();
	do
		{
		// Write raw data for the StringF component in the next part
		(void)WriteRawStrFPartL(aHeader, iter, KErrHttpEncodeConnection);

		// If there's another media field we need a separator
		++iter;
		if (!iter.AtEnd())
			{
			aHeader.WriteRawDataL(aSep);
			(void)WriteRawStrFPartL(aHeader, iter, KErrHttpEncodeConnection);
			++iter;
  		    if(!iter.AtEnd())
			    aHeader.WriteRawDataL(aSep);			
			}
		}while (!iter.AtEnd());

	// Done
	aHeader.CommitRawData();
	}

const CHeaderFieldPart* CHttpHeaderWriter::WriteRawStrFPartL(RHeaderField& aHeader, THeaderFieldPartIter& aIter, TInt aErrorCode) const
	{
	const CHeaderFieldPart* part = aIter();
	if (part == NULL)
		User::Leave(aErrorCode);
	THTTPHdrVal ptVal = part->Value();
	if (ptVal.Type() != THTTPHdrVal::KStrFVal)
		User::Leave(aErrorCode);
	aHeader.WriteRawDataL(ptVal.StrF().DesC());
	return part;
	}
