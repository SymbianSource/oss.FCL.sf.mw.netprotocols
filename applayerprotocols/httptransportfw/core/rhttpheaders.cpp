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
#include <e32std.h>
#include <http/thttphdrval.h>
#include <httperr.h>
#include <http/rhttpheaders.h>// Class signature


// Local includes
#include "CHeaders.h"


EXPORT_C THTTPHdrVal::THTTPHdrVal()
	{
	iType = KNoType;
	iMoreSpace = 0;
	iVal = 0;
	}

EXPORT_C
THTTPHdrVal::THTTPHdrVal(TInt aIntVal)
	{
	SetInt(aIntVal);
	}

EXPORT_C
THTTPHdrVal::THTTPHdrVal(RStringF aStrVal)
	{
	SetStrF(aStrVal);
	}
EXPORT_C
THTTPHdrVal::THTTPHdrVal(RString aStrVal)
	{
	SetStr(aStrVal);
	}

EXPORT_C
THTTPHdrVal::THTTPHdrVal(TDateTime aDateVal)
	{
	SetDateTime(aDateVal);
	}

//
// Clone this header value
//

EXPORT_C
THTTPHdrVal THTTPHdrVal::Copy() const
	{
	THTTPHdrVal ret = *this;
	if (iType == KStrVal)
		Str().Copy();
	if (iType == KStrFVal)
		StrF().Copy();
	return ret;
	}


//
// Accessors
//

EXPORT_C
THTTPHdrVal::THTTPValType THTTPHdrVal::Type() const
	{
	return iType;
	}

EXPORT_C
TInt THTTPHdrVal::Int() const
	{
	__ASSERT_DEBUG(iType == KTIntVal, HTTPPanic::Panic(HTTPPanic::EHeaderInvalidType));
	return iVal;
	}
	
EXPORT_C
RStringF THTTPHdrVal::StrF() const
	{
	__ASSERT_DEBUG(iType == KStrFVal, HTTPPanic::Panic(HTTPPanic::EHeaderInvalidType));
	return *reinterpret_cast<const RStringF*>(&iVal);
	}
EXPORT_C
RString THTTPHdrVal::Str() const
	{
	__ASSERT_DEBUG(iType == KStrVal, HTTPPanic::Panic(HTTPPanic::EHeaderInvalidType));
	return *reinterpret_cast<const RString*>(&iVal);
	}

EXPORT_C
TDateTime THTTPHdrVal::DateTime() const
	{
	__ASSERT_DEBUG(iType == KDateVal, HTTPPanic::Panic(HTTPPanic::EHeaderInvalidType));
	return reinterpret_cast<const TTime*>(&iVal)->DateTime();
	}

EXPORT_C
void THTTPHdrVal::SetInt(TInt aIntVal)
	{
	iMoreSpace = 0;
	iVal  = aIntVal;
	iType = KTIntVal;
	}

EXPORT_C
void THTTPHdrVal::SetStr(RString aStrVal)
	{
	iMoreSpace = 0;
	*reinterpret_cast<RString*>(&iVal) = aStrVal;
	iType = KStrVal;
	}
EXPORT_C
void THTTPHdrVal::SetStrF(RStringF aStrVal)
	{
	iMoreSpace = 0;
	*reinterpret_cast<RStringF*>(&iVal) = aStrVal;
	iType = KStrFVal;
	}

EXPORT_C
void THTTPHdrVal::SetDateTime(TDateTime aDateVal)
	{
	iMoreSpace = 0;
	*reinterpret_cast<TTime*>(&iVal) = aDateVal;
	iType	  = KDateVal;
	}

EXPORT_C
TBool THTTPHdrVal::operator==(THTTPHdrVal aVal) const
	{
	TBool ret = EFalse;
	if (aVal.iType == iType)
		{
		switch (iType)
			{
		case KTIntVal:
			ret = (aVal.iVal == iVal);
			break;
		case KStrVal:
		case KStrFVal:
		case KDateVal:
			ret = (aVal.iVal == iVal && aVal.iMoreSpace == iMoreSpace);
			break;
		default:
			HTTPPanic::Panic(HTTPPanic::EHeaderInvalidType);
			break;
			}
		}
	return ret;
	}

EXPORT_C
TBool THTTPHdrVal::operator!=(THTTPHdrVal aVal) const
	{
	return !operator==(aVal);
	}




EXPORT_C
void RHTTPHeaders::SetFieldL(RStringF aFieldName, THTTPHdrVal aFieldValue)
	{
	iImplementation->SetFieldL(aFieldName, aFieldValue);
	}

EXPORT_C TInt RHTTPHeaders::SetField(RStringF aFieldName, THTTPHdrVal aFieldValue)
    {
    TRAPD(err, iImplementation->SetFieldL(aFieldName, aFieldValue));
	return err;
    }

EXPORT_C
void RHTTPHeaders::SetFieldL(RStringF aFieldName, THTTPHdrVal aFieldValue,
						RStringF aParamName, THTTPHdrVal aParamValue)
	{
	iImplementation->SetFieldL(aFieldName, aFieldValue, aParamName, aParamValue);
	}

EXPORT_C
TInt RHTTPHeaders::SetField(RStringF aFieldName, THTTPHdrVal aFieldValue, RStringF aParamName, THTTPHdrVal aParamValue)
    {
    // Implementation need to be changed to make it non-leaving. TODO..
    TRAPD(err, iImplementation->SetFieldL(aFieldName, aFieldValue, aParamName, aParamValue));
    return err;
    }

EXPORT_C
void RHTTPHeaders::SetRawFieldL(RStringF aFieldName, const TDesC8& aOtaFieldData, const TDesC8& aFieldSeparator)
	{
	iImplementation->SetRawFieldL(aFieldName, aOtaFieldData, aFieldSeparator);
	}

EXPORT_C 
TInt RHTTPHeaders::SetRawField(RStringF aFieldName, const TDesC8& aRawFieldData, const TDesC8& aFieldSeparator)
    {
    // Implementation need to be changed to make it non-leaving. TODO..
    TRAPD(err, iImplementation->SetRawFieldL(aFieldName, aRawFieldData, aFieldSeparator));
    return err;
    }

EXPORT_C
TInt RHTTPHeaders::RemoveField(RStringF aFieldName)
	{
	return iImplementation->RemoveField(aFieldName);
	}


EXPORT_C TInt RHTTPHeaders::RemoveFieldPart(RStringF aFieldName, TInt aIndex)
	{
	TRAPD(err,iImplementation->RemoveFieldPartL(aFieldName, aIndex));
	return err;
	}

EXPORT_C void RHTTPHeaders::RemoveAllFields()
	{
	iImplementation->RemoveAllFields();
	}


EXPORT_C 
TInt RHTTPHeaders::FieldPartsL(RStringF aFieldName) const
	{
	return iImplementation->FieldPartsL(aFieldName);
	}

EXPORT_C TInt RHTTPHeaders::GetField(RStringF aFieldName, TInt aPartIdx, 
									 THTTPHdrVal& aHeaderValue) const
	{
	return iImplementation->GetField(aFieldName, aPartIdx, aHeaderValue);
	}


EXPORT_C
TInt RHTTPHeaders::GetRawField(RStringF aFieldName, TPtrC8& aRawFieldData) const
	{
	return iImplementation->GetRawField(aFieldName, aRawFieldData);
	}

EXPORT_C void RHTTPHeaders::GetRawFieldL(RStringF aFieldName, TPtrC8& aRawFieldData) const
    {
    iImplementation->GetRawFieldL(aFieldName, aRawFieldData);
    }

EXPORT_C
TInt RHTTPHeaders::GetParam(RStringF aFieldName, RStringF aParamName, 
							THTTPHdrVal& aValue, TInt aPartIdx) const
	{
	return iImplementation->GetParam(aFieldName, aParamName, aValue, aPartIdx);
	}

EXPORT_C void RHTTPHeaders::SetParamL(RStringF aFieldName, RStringF aParamName, 
									  THTTPHdrVal aParamValue, TInt aPartIdx)
	{
	iImplementation->SetParamL(aFieldName, aParamName, aParamValue, aPartIdx);
	}

EXPORT_C
THTTPHdrFieldIter RHTTPHeaders::Fields() const
	{
	return iImplementation->Fields();
	}



EXPORT_C
THTTPHdrFieldIter::~THTTPHdrFieldIter()
	{
	// does nothing
	}

EXPORT_C
void THTTPHdrFieldIter::First()
	{
	// Check existence of the field name array in CHeaders. If it hasn't been created yet
	// then we must start off 'at the end'.  This is indicated by the position index set
	// to KErrNotFound
	iPosIdx = 0;
	CheckInvalidation();
	}

EXPORT_C
TBool THTTPHdrFieldIter::AtEnd() const
	{
	return (iPosIdx == KErrNotFound);
	}

EXPORT_C
void THTTPHdrFieldIter::operator++()
	{
	// Do nothing if we're already at the end
	if (iPosIdx >= 0)
		{
		++iPosIdx;
		CheckInvalidation();
		}
	}

EXPORT_C
RStringTokenF THTTPHdrFieldIter::operator()()
	{
	CheckInvalidation();
	if (iPosIdx > KErrNotFound)
		return iHeaders->iFields[iPosIdx].iFieldName;
	else
		return RStringF();
	}

void THTTPHdrFieldIter::CheckInvalidation()
	{
	// iPosIdx may have been valid after the last use of operator() but
	// if an item was subsequently removed from the collection the iterator
	// may have gone off the end.
	if (iPosIdx >= iHeaders->iFields.Count())
		iPosIdx = KErrNotFound; // Hit the end
	}

