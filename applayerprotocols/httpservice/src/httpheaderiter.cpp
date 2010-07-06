// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "httpheaderiter.h"
#include "CHeaders.h"
#include "CHeaderField.h"
#include "httpclientutils.h"

// This class needs to be optimized.
EXPORT_C THttpHeaderParamIterator THttpHeaderParamIterator::operator++ (TInt)
    {
    THttpHeaderParamIterator it(iParameterArray, iPostIdx);
    if(iParameterArray && iPostIdx < iParameterArray->Count())
        {
        ++iPostIdx;
        }
    return it;    
    }

EXPORT_C THttpHeaderParamIterator& THttpHeaderParamIterator::operator++ ()
    {
    if(iParameterArray && iPostIdx < iParameterArray->Count())
         {
         ++iPostIdx;
         }
     return *this;
    }

EXPORT_C TBool  THttpHeaderParamIterator::AtEnd() const
    {
    return (!iParameterArray || iPostIdx >= iParameterArray->Count());  
    }

EXPORT_C const TDesC8& THttpHeaderParamIterator::Name() const
    {   
    if(iParameterArray && iPostIdx < iParameterArray->Count())
        {
        return (*iParameterArray)[iPostIdx]->Name().DesC();
        }
    return KNullDesC8();       
    }

EXPORT_C THttpHeaderValueVariant THttpHeaderParamIterator::Value() const
    {
    if(iParameterArray && iPostIdx < iParameterArray->Count())
        {
        return HttpClientUtils::CopyHttpHdrVal((*iParameterArray)[iPostIdx]->Value());
        }
    return THttpHeaderValueVariant();
    }

EXPORT_C THttpHeaderParamIterator::THttpHeaderParamIterator()
: iParameterArray(NULL),
iPostIdx(0)
    {    
    }

EXPORT_C THttpHeaderParamIterator::THttpHeaderParamIterator(const RPointerArray<CHeaderFieldParam>* aArray, TInt aIndex /* = 0 */)
: iParameterArray(aArray),
iPostIdx(aIndex)
    {
    
    }

// -----------------------------------------------------------------------------
EXPORT_C THttpHeaderElementIterator THttpHeaderElementIterator::operator++ (TInt)
    {
    THttpHeaderElementIterator it(iElementArray, iPostIdx);
    if(iElementArray && iPostIdx < iElementArray->Count())
        {
        ++iPostIdx;
        }
    return it;          
    }

EXPORT_C THttpHeaderElementIterator& THttpHeaderElementIterator::operator++ ()
    {
    if(iElementArray && iPostIdx < iElementArray->Count())
         {
         ++iPostIdx;
         }
     return *this;          
    }

EXPORT_C TBool THttpHeaderElementIterator::AtEnd() const
    {
    return (!iElementArray || iPostIdx >= iElementArray->Count());      
    }

EXPORT_C THttpHeaderValueVariant THttpHeaderElementIterator::Value() const
    {
    if(iElementArray && iPostIdx < iElementArray->Count())
        {
        return HttpClientUtils::CopyHttpHdrVal((*iElementArray)[iPostIdx]->Value());
        }
    return THttpHeaderValueVariant();
    }

EXPORT_C THttpHeaderParamIterator THttpHeaderElementIterator::Parameter() const
    {
    if(iElementArray == NULL)
        return THttpHeaderParamIterator();
    return THttpHeaderParamIterator(&(*iElementArray)[iPostIdx]->iParams);   
    }

EXPORT_C THttpHeaderElementIterator::THttpHeaderElementIterator()
: iElementArray(NULL),
iPostIdx(0)
    {
    
    }

EXPORT_C THttpHeaderElementIterator::THttpHeaderElementIterator(const RPointerArray<CHeaderFieldPart>* aArray, TInt aIndex /* = 0 */)
: iElementArray(aArray),
iPostIdx(aIndex)
    {
    
    }

// ----------------------------------------------------------------------
EXPORT_C THttpHeaderIterator THttpHeaderIterator::operator++ (TInt)
    {
    THttpHeaderIterator it(iHttpHeaders, iPostIdx);
     if(iHttpHeaders && iPostIdx < iHttpHeaders->iFields.Count())
         {
         ++iPostIdx;
         }
     return it;   
    }

EXPORT_C THttpHeaderIterator& THttpHeaderIterator::operator++ ()
    {
    if(iHttpHeaders && iPostIdx < iHttpHeaders->iFields.Count())
         {
         ++iPostIdx;
         }
     return *this;      
    }

EXPORT_C TBool THttpHeaderIterator::AtEnd() const
    {
    return (!iHttpHeaders || iPostIdx >= iHttpHeaders->iFields.Count());   
    }

EXPORT_C const TDesC8& THttpHeaderIterator::Name() const
    {
    if(iHttpHeaders && iPostIdx < iHttpHeaders->iFields.Count())
        {
        return iHttpHeaders->iFields[iPostIdx].iFieldName.DesC();
        }
    return KNullDesC8();
    }

EXPORT_C THttpHeaderElementIterator THttpHeaderIterator::Elements() const
    {
    if(iHttpHeaders)
        THttpHeaderElementIterator();
    
    return THttpHeaderElementIterator(&iHttpHeaders->iFields[iPostIdx].iField->iElementArray);       
    }

EXPORT_C THttpHeaderParamIterator THttpHeaderIterator::Parameters() const
    {
    if(iHttpHeaders)
        THttpHeaderParamIterator();
    
    return THttpHeaderParamIterator(&iHttpHeaders->iFields[iPostIdx].iField->iParameterArray);       
    }

EXPORT_C THttpHeaderIterator::THttpHeaderIterator()
: iHttpHeaders(NULL),
iPostIdx(0)
    {
    
    }

EXPORT_C THttpHeaderIterator::THttpHeaderIterator(const CHeaders* aHeaders, TInt aIndex /* = 0 */)
: iHttpHeaders(aHeaders),
    iPostIdx(aIndex)
    {

    }

