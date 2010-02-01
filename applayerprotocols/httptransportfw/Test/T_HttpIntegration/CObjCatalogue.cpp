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
// $Header$
// we attempted to template the CCatalog class and failed!
// this is due to either a) incompetence or b) SymbianOS 
// can't really blame either since both complex and lack of
// experience!
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// 
//

#if !defined(TEMPLATED_CATALOGUE)

//-----------------------------------------------------------------------------

#include "CObjCatalogue.h"										// This module

//-----------------------------------------------------------------------------

const TInt KObjGranularity = 16;

//-----------------------------------------------------------------------------

CAnyObject *CAnyObject::NewL( )
{
CAnyObject *self = NewLC();
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CAnyObject* CAnyObject::NewLC( )
{
CAnyObject *self = new (ELeave) CAnyObject();
CleanupStack::PushL(self);
self->ConstructL();
return self;
}

//-----------------------------------------------------------------------------

CAnyObject *CAnyObject::NewL (const TBuf<32> aName, const TInt &aIndex, const TInt &aTag, TAny *aPtr)
{
CAnyObject *self = NewLC(aName, aIndex, aTag, aPtr);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CAnyObject *CAnyObject::NewLC(const TBuf<32> aName, const TInt &aIndex, const TInt &aTag, TAny *aPtr)
{
CAnyObject *self = new (ELeave) CAnyObject();
CleanupStack::PushL(self);
self->ConstructL(aName, aIndex, aTag, aPtr);
return self;
}

//-----------------------------------------------------------------------------

CAnyObject::~CAnyObject()
{
}

//-----------------------------------------------------------------------------
//	Note: this is the default (NULL) constructor

void CAnyObject::ConstructL()
{
iName = _L("");
iIndex = -1;
iTag = 0;
iPtr = NULL;
}

//-----------------------------------------------------------------------------
//	Note: this is the general constructor

void CAnyObject::ConstructL(const TBuf<32> aName, const TInt &aIndex, const TInt &aTag, TAny *aPtr)
{
iName = aName;
iIndex = aIndex;
iTag = aTag;
iPtr = aPtr;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//	Object Catalogue
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CObjCatalogue *CObjCatalogue::NewL()
{
CObjCatalogue *self = NewLC();
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CObjCatalogue* CObjCatalogue::NewLC( )
{
CObjCatalogue *self = new (ELeave) CObjCatalogue();
CleanupStack::PushL(self);
self->ConstructL();
return self;
}

//-----------------------------------------------------------------------------
//	Note: this constructs ONLY CLabeledText objects!

void CObjCatalogue::ConstructL( )
{
iObjects = new (ELeave) CArrayPtrSeg<CAnyObject>(KObjGranularity);
}


//-----------------------------------------------------------------------------

CObjCatalogue::~CObjCatalogue()
{
iObjects->ResetAndDestroy();
delete iObjects;
iObjects = NULL;
}

//-----------------------------------------------------------------------------

void CObjCatalogue::AddL(const TDesC &aName, const TInt &aIndex, const TInt &aTag, TAny *aPtr)
{
CAnyObject *obj = CAnyObject::NewLC();
obj->SetName(aName);
obj->SetIndex(aIndex);
obj->SetTag(aTag);
obj->SetPtr(aPtr);
AddL(obj);
CleanupStack::Pop();
}

//-----------------------------------------------------------------------------

void CObjCatalogue::AddL(CAnyObject *aObject)
{
iObjects->AppendL(aObject);
}

//-----------------------------------------------------------------------------

TBool CObjCatalogue::Delete(const TDesC& aLabel)
{
for (TInt i = 0; i < iObjects->Count(); ++i)
	{
	CAnyObject *obj = iObjects->At(i);
	if (aLabel.Compare(obj->Name()) == 0)
		{
		iObjects->Delete(i);
		delete obj;
		return ETrue;
		}
	} // end-for

return EFalse;
}

//-----------------------------------------------------------------------------

TBool CObjCatalogue::DeleteIndex(const TInt &aIndex)
{
for (TInt i = 0; i < iObjects->Count(); ++i)
	{
	CAnyObject *obj = iObjects->At(i);
	if (aIndex == obj->Index())
		{
		iObjects->Delete(i);
		delete obj;
		return ETrue;
		}
	} // end-for

return EFalse;
}


//-----------------------------------------------------------------------------

TBool CObjCatalogue::DeleteTag(const TInt &aTag)
{
for (TInt i = 0; i < iObjects->Count(); ++i)
	{
	CAnyObject *obj = iObjects->At(i);
	if (aTag == obj->Tag())
		{
		iObjects->Delete(i);
		delete obj;
		return ETrue;
		}
	} // end-for

return EFalse;
}

//-----------------------------------------------------------------------------

TInt CObjCatalogue::Count( )
{
return iObjects->Count();
}

//-----------------------------------------------------------------------------

TAny *CObjCatalogue::At(TInt aIndex) const
{
return iObjects->At(aIndex);
}

//-----------------------------------------------------------------------------

TAny *CObjCatalogue::operator[](TInt aIndex) const
{
return iObjects->At(aIndex);
//return iObjects[aIndex];
}

//-----------------------------------------------------------------------------

TAny *CObjCatalogue::Name(const TDesC& aLabel) const
{
for (TInt i = 0; i < iObjects->Count(); ++i)
	{
	CAnyObject *obj = iObjects->At(i);
	if (obj)
		{
		if (aLabel.Compare(obj->Name()) == 0) 
			return obj;
		}
	}

return NULL;
}

//-----------------------------------------------------------------------------

TAny *CObjCatalogue::Name(const TDesC& aLabel, TInt &aIndex) const
{
aIndex = -1;	//	invalid index so can use this to additionally indicate failure

for (TInt i = 0; i < iObjects->Count(); ++i)
	{
	CAnyObject *obj = NULL;
	obj = iObjects->At(i);
	if (obj != NULL) 
		{
		if (aLabel.Compare(obj->Name()) == 0) 
			{
			aIndex = obj->Index();
			return obj;
			}
		}
	}

return NULL;
}

//-----------------------------------------------------------------------------

TAny *CObjCatalogue::Tag(const TInt &aTag) const
{
for (TInt i = 0; i < iObjects->Count(); ++i)
	{
	CAnyObject *obj = iObjects->At(i);
	if (aTag == obj->Tag()) 
		return obj;
	}

return NULL;
}

//-----------------------------------------------------------------------------

TAny *CObjCatalogue::Tag(const TInt &aTag, TInt &aIndex) const
{
for (TInt i = 0; i < iObjects->Count(); ++i)
	{
	CAnyObject *obj = iObjects->At(i);
	if (aTag == obj->Tag()) 
		{
		aIndex = i;
		return obj;
		}
	}

return NULL;
}

//-----------------------------------------------------------------------------

#else

//-----------------------------------------------------------------------------

#include "CObjCatalogue.h"										// This module

//-----------------------------------------------------------------------------

template <class T>
void CObjCatalogue<T>::AddL( const TDesC& aLabel, const TDesC& aValue )
{
T* text = T::NewLC( aLabel, aValue );
iCatalog->AppendL( text );
CleanupStack::Pop();
}

//-----------------------------------------------------------------------------

template <class T>
void CObjCatalogue<T>::AddL(T* aText )
{
iCatalog->AppendL( aText );
}

//-----------------------------------------------------------------------------

template <class T>
TBool CObjCatalogue<T>::Delete( const TDesC& aLabel )
{
TInt  i;
TInt  c = iCatalog->Count();
for ( i = 0; i < c; i++ )
{
CLabeledText* text = iCatalog->At( i );
if ( aLabel.Compare(text->Label()) == 0 )
    {
    iCatalog->Delete( i );
    delete text;
    return ETrue;
    }
} // end-for

return EFalse;
}

#endif //	templated spec















//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------



