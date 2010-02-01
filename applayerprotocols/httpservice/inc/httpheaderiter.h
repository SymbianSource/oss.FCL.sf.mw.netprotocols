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

#ifndef __HTTPHEADERITER_H__
#define __HTTPHEADERITER_H__

#include <e32base.h>
#include <thttpheadervaluevariant.h>
class CHeaders;
class CHeaderField;
class CHeaderFieldPart;
class CHeaderFieldParam;

class THttpHeaderParamIterator
{
friend class THttpHeaderElementIterator;
friend class THttpHeaderIterator;

public:
IMPORT_C THttpHeaderParamIterator operator++ (TInt);
IMPORT_C THttpHeaderParamIterator& operator++ ();
IMPORT_C TBool	AtEnd() const;
IMPORT_C const TDesC8& Name() const;
IMPORT_C THttpHeaderValueVariant Value() const;
IMPORT_C THttpHeaderParamIterator();
private:
    IMPORT_C THttpHeaderParamIterator(const RPointerArray<CHeaderFieldParam>* aArray, TInt aIndex = 0);

private:
const RPointerArray<CHeaderFieldParam>*     iParameterArray; 
TInt iPostIdx;
};

class THttpHeaderElementIterator
{
friend class THttpHeaderIterator;
friend class CHttpClientTransaction;
public:
IMPORT_C THttpHeaderElementIterator operator++ (TInt);
IMPORT_C THttpHeaderElementIterator& operator++ ();
IMPORT_C TBool AtEnd() const;
IMPORT_C THttpHeaderValueVariant Value() const;
IMPORT_C THttpHeaderParamIterator Parameter() const;
IMPORT_C THttpHeaderElementIterator();
private:
    IMPORT_C THttpHeaderElementIterator(const RPointerArray<CHeaderFieldPart>* aArray, TInt aIndex = 0);

private:
const RPointerArray<CHeaderFieldPart>*     iElementArray; 
TInt iPostIdx;
};

class THttpHeaderIterator
{
friend class CHttpClientTransaction;
public:
IMPORT_C THttpHeaderIterator operator++ (TInt);
IMPORT_C THttpHeaderIterator& operator++ ();
IMPORT_C TBool AtEnd() const;
IMPORT_C const TDesC8& Name() const;
IMPORT_C THttpHeaderElementIterator Elements() const;
IMPORT_C THttpHeaderParamIterator Parameters() const;
IMPORT_C THttpHeaderIterator();
private:
IMPORT_C THttpHeaderIterator(const CHeaders* aHeaders, TInt aIndex = 0);

private:
const CHeaders* iHttpHeaders;
TInt iPostIdx;
};
#endif // __HTTPHEADERITER_H__

