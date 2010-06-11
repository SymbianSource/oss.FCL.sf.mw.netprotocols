// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <http/framework/cheadercodec.h>

// Local includes
#include "CHeaders.h"

// Class signature
#include "CHeaderField.h"


CHeaderField* CHeaderField::NewL(RStringF aHeaderFieldName, CHeaders& aOwner)
	{
	return new(ELeave)CHeaderField(aHeaderFieldName, aOwner);
	}

CHeaderField* CHeaderField::New(RStringF aHeaderFieldName, CHeaders& aOwner)
    {
    return new CHeaderField(aHeaderFieldName, aOwner);
    }


CHeaderField* CHeaderField::NewL(RStringF aHeaderFieldName, CHeaders& aOwner, THTTPHdrVal aVal)
	{
	CHeaderField* me = new(ELeave)CHeaderField(aHeaderFieldName, aOwner);
	CleanupStack::PushL(me);
	CHeaderFieldPart* part = CHeaderFieldPart::NewL(aVal);
	CleanupStack::PushL(part);
	me->AddPartL(part);
	CleanupStack::Pop(2);
	return me;
	}


CHeaderField* CHeaderField::NewL(RStringF aHeaderFieldName, CHeaders& aOwner, const TDesC8& aRawData)
	{
	CHeaderField* me = new(ELeave)CHeaderField(aHeaderFieldName, aOwner);
	CleanupStack::PushL(me);
	me->ConstructL(aRawData);
	CleanupStack::Pop(me);
	return me;
	}


/** D'tor
*/

CHeaderField::~CHeaderField()
	{
	iName.Close();
	ClearParsedData();
	ClearRawData();
	iElementArray.Close();	
	iParameterArray.Close();
	}




TInt CHeaderField::NumPartsL()
	{
	// Convert, if necessary, to the parsed format
	ConvertToParsedFormatL();

	// Check the array exists first
	return iParts.Count();
	}


THeaderFieldPartIter CHeaderField::PartsL()
	{
	// Convert, if necessary, to the parsed format
	ConvertToParsedFormatL();

	// create an iterator initialised for this header
	return THeaderFieldPartIter(this);
	}

TInt CHeaderField::Part(TInt aIndex, CHeaderFieldPart*& aPart)
    {
    TInt err = KErrNone;
    // Check, is already in parsed format
    if (iDataParseState == EDataInRawForm)
        {
        // Convert, to the parsed format
        TRAP(err, ConvertToParsedFormatL());        
        }        

    if(err == KErrNone && aIndex < iParts.Count() && (aIndex >= 0))
        {
        aPart = iParts[aIndex];
        }
    return err;
    }


CHeaderFieldPart* CHeaderField::PartL(TInt aIndex)
	{
	// Convert, if necessary, to the parsed format
	ConvertToParsedFormatL();

	// Check the part is in range, if so return it
	if ((aIndex < NumPartsL()) && (aIndex >= 0))
		return iParts[aIndex];
	else
		User::Leave(KErrNotFound);
	
	return NULL;
	}

EXPORT_C
void CHeaderField::RawDataL(TPtrC8& aRawData)
	{
	ConvertToRawFormatL();
	aRawData.Set(*iRawData);
	}


/** Setters
*/

void CHeaderField::SetPartL(CHeaderFieldPart* aPart, TInt aIndex)
	{
	if (aIndex < iParts.Count())
		{
		CHeaderFieldPart* oldPart = iParts[aIndex];
		iParts[aIndex] = aPart;
		delete oldPart;
		}
	else
		User::LeaveIfError(iParts.Append(aPart));
	}

void CHeaderField::BeginRawDataL(TInt aChunkSize)
	{
	iRawChunkSize = aChunkSize;

	// clear out any existing Raw data, but don't reallocate if not needed
	if (!iRawData)
		iRawData = HBufC8::NewL(iRawChunkSize);
	else
		{
		TPtr8 rawPtr = iRawData->Des();
		rawPtr.Zero();
		}
	}


void CHeaderField::WriteRawDataL(const TDesC8& aData)
	{
    // Check to see if the buffer needs to grow
	TInt maxLength  = iRawData->Des().MaxLength();
	TInt reqdLength = iRawData->Length() + aData.Length();
	if (reqdLength > maxLength)
		{
		// Yes - allocate a new buffer of a larger size, and copy the contents
		// of the old one over. This may need more than one extra chunk. Check
		// for partial chunks - add an extra one if necessary.
		TInt numChunksReqd = (reqdLength - maxLength)/iRawChunkSize;
		 if ( (reqdLength - maxLength)%iRawChunkSize > 0)
			++numChunksReqd;
		HBufC8* largerBuffer = iRawData->ReAllocL(maxLength + iRawChunkSize*numChunksReqd);

		// Switch buffers. The old one was removed by ReAlloc.
		iRawData = largerBuffer;
		}

	// Can now append confidently
	iRawData->Des().Append(aData);
	}


void CHeaderField::WriteRawDataL(TChar aData)
	{
	// Check to see if the buffer descriptor needs to grow
	TInt maxLength  = iRawData->Des().MaxLength();
	TInt reqdLength = iRawData->Length() + 1;
	if (reqdLength > maxLength)
		{
		// Yes - allocate a new buffer of a larger size, and copy the contents
		// of the old one over. Since a single character is being added, one
		// chunk will be enough.
		HBufC8* largerBuffer = iRawData->ReAllocL(maxLength + iRawChunkSize);

		// Switch buffers. The old one was removed by ReAlloc.
		iRawData = largerBuffer;
		}

	// Can now append confidently
	iRawData->Des().Append(aData);
	}


void CHeaderField::CommitRawData()
	{
	// Change state and remove the parsed form of the header data
	iDataParseState = EDataInRawForm;
	ClearParsedData();
	}


void CHeaderField::AddPartL(CHeaderFieldPart* aPart)
	{
	// Convert, if necessary, to the parsed format
	ConvertToParsedFormatL();

	// Append the item
	User::LeaveIfError(iParts.Append(aPart));
	}

TInt CHeaderField::AddPart(CHeaderFieldPart* aPart)
    {
    TInt err = KErrNone;
    if (iDataParseState == EDataInRawForm)
        {
        TRAP(err, AddPartL(aPart));
        }
    if(err == KErrNone)
        {
        err = iParts.Append(aPart);
        }
    return err;
    }

void CHeaderField::InsertPartL(CHeaderFieldPart* aPart, TInt aIndex)
	{
	// Convert, if necessary, to the parsed format
	ConvertToParsedFormatL();

	// If the array isn't yet constructed, or is empty, then use 'AddPart'
	if (!NumPartsL()) 
		AddPartL(aPart);
	else
		User::LeaveIfError(iParts.Insert(aPart, aIndex));
	}


TInt CHeaderField::RemovePartL(CHeaderFieldPart* aPart)
	{
	// Convert, if necessary, to the parsed format
	ConvertToParsedFormatL();

	// Sanity check - if no parts, then the part can't be removed...
	if (iParts.Count() == 0)
		return KErrNotFound;

	// Find the part.  
	TInt idx = iParts.Find(aPart);

	// Now remove by index
	iParts.Remove(idx);
	iParts.Compress();
	delete aPart;
	return KErrNone;
	}


TInt CHeaderField::RemovePartL(TInt aIndex)
	{
	// Convert, if necessary, to the parsed format
	ConvertToParsedFormatL();

	// Sanity check - if no array exists or if the index supplied is out of the bounds
	// of the array, then the part can't be removed...
	if ((aIndex < 0) || (aIndex >= NumPartsL()))
		return KErrNotFound;

	// Note that the array doesn't actually delete the part itself. Also the array must be
	// compressed to close the 'gap' left by the deleted part
	CHeaderFieldPart* part = PartL(aIndex);
	iParts.Remove(aIndex);
	iParts.Compress();
	delete part;
	return KErrNone;
	}


/** Constructors and second-phase construction
*/

void CHeaderField::ConvertToRawFormatL()
	{
	if (iDataParseState != EDataParsedOK)
		return;
	RHeaderField rf(*this);
	// State change and removal of parsed data  is done when the Raw data is committed by the codec when it is encoded
	iCodec.EncodeHeaderL(rf);
	}

void CHeaderField::ConvertToParsedFormatL()
	{
	if (iDataParseState != EDataInRawForm)
		return;
	RHeaderField rf(*this);
	iCodec.DecodeHeaderL(rf);

	// If we haven't left, then it must have succeeded. Change state and remove the Raw data
	iDataParseState = EDataParsedOK;
	ClearRawData();
	}

TInt CHeaderField::ConvertToParsedFormat()
    {
    if (iDataParseState != EDataInRawForm)
        return KErrNone;

    RHeaderField rf(*this);
    TRAPD(err, iCodec.DecodeHeaderL(rf));
    if(err != KErrNone)
        return err;

    // If we haven't left, then it must have succeeded. Change state and remove the Raw data
    iDataParseState = EDataParsedOK;
    ClearRawData();

    return KErrNone;
    }

TInt CHeaderField::SplitValueAndParameterArray()
    {
    const TUint count = iParts.Count();
    CHeaderFieldPart* part = NULL;
    TInt ret = KErrNone;
    for(TUint i = 0; (i < count) && (ret == KErrNone); ++i)
        {
        part = iParts[i];
        THTTPHdrVal val = part->Value();        
        if(((val.Type() == THTTPHdrVal::KStrVal) && val.Str().DesC().Length() > 0) || 
                ((val.Type() == THTTPHdrVal::KStrFVal) && val.StrF().DesC().Length() > 0))
            {
            ret = iElementArray.Append(part);
            }
        else
            {
            const TUint numParams = part->NumParameters();
            if(numParams > 0)  // Push only if it has some parameters.
                {
                for(TUint j = 0; j < numParams && ret == KErrNone; ++j)
                    {
                    ret = iParameterArray.Append(part->Parameter(j));
                    }
                }
            else
                {
                ret = iElementArray.Append(part);
                }
            }        
        }
    return ret;
    }

CHeaderField::CHeaderField(RStringF aHeaderFieldName, CHeaders& aOwner)
	: iCodec(aOwner.Codec()), iDataParseState(ENoDataSupplied), iOwner(&aOwner) 
	{
	// Ensure that we maintain string references correctly with the header field name
	iName = aHeaderFieldName.Copy();
	iDataParseState = EDataParsedOK;
	}

void CHeaderField::ConstructL(const TDesC8& aRawData)
	{
	// Copy the data.  Set the state.
	iRawData = aRawData.AllocL();
	iDataParseState = EDataInRawForm;
	}

void CHeaderField::ClearParsedData()
	{
	// Clear and destroy parts array and its contents
	iParts.ResetAndDestroy();
	}

void CHeaderField::ClearRawData()
	{
	// Destroy any Raw data
	delete iRawData;
	iRawData = NULL;
	}


/** Implementation of CHeaderFieldPart class *************************************************
*/


/** Getter's
*/

CHeaderFieldParam* CHeaderFieldPart::Parameter(RStringF aParamName) const
	{
	// Iterate the parameter list looking for a name match
	THeaderFieldParamIter it(this);
	CHeaderFieldParam* retVal = NULL;
	const CHeaderFieldParam* nextParam = NULL;
	TBool done = EFalse;
	while (!done && !it.AtEnd())
		{
		nextParam = it();
		if (nextParam && (nextParam->Name() == aParamName))
			{
			retVal = CONST_CAST(CHeaderFieldParam*, nextParam);
			done = ETrue;
			}
		++it;
		}
	return retVal;
	}



/** Setter's
*/


void CHeaderFieldPart::SetValue(THTTPHdrVal aVal)
	{
	// We must check for an existing string value since it will have to be closed
	if (iValue.Type() == THTTPHdrVal::KStrFVal)
		iValue.StrF().Close();
	if (iValue.Type() == THTTPHdrVal::KStrVal)
		iValue.Str().Close();
	iValue = aVal.Copy();
	}



TInt CHeaderFieldPart::RemoveParam(CHeaderFieldParam* aParam)
	{
	// Sanity check - if no array exists, then the part can't be removed...
	if (iParams.Count() == 0)
		return KErrNotFound;

	// Find the param. 
	TInt idx = iParams.Find(aParam);

	// Now remove by index. Note that the array doesn't actually delete the param itself. Also
	// the array must be compressed to close the 'gap' left by the deleted param
	if (idx != KErrNotFound)
		{
		iParams.Remove(idx);
		iParams.Compress();
		delete aParam;
		idx = KErrNone;
		}
	return idx;
	}


