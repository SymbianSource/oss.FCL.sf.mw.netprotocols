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

#include "CCatalogue.h"										// This module

//-----------------------------------------------------------------------------

CCatalogue *CCatalogue::NewL()
{
CCatalogue *self = NewLC();
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCatalogue* CCatalogue::NewLC()
{
CCatalogue *self = new (ELeave) CCatalogue();
CleanupStack::PushL(self);
self->ConstructL();
return self;
}

//-----------------------------------------------------------------------------

CCatalogue::~CCatalogue()
{
iCatalogue->ResetAndDestroy();
delete iCatalogue;
iCatalogue = NULL;
}

//-----------------------------------------------------------------------------
//	Note: this constructs ONLY CLabeledText objects!

void CCatalogue::ConstructL()
{
iCatalogue = new (ELeave) CArrayPtrSeg<CLabeledText>(64);
}

//-----------------------------------------------------------------------------
//	create a label with supplied data then append...

void CCatalogue::AddL(const TDesC& aLabel, const TDesC& aValue)
{
CLabeledText *text = CLabeledText::NewLC(aLabel, aValue);
AddL(text);
CleanupStack::Pop();
}

//-----------------------------------------------------------------------------
//	directly append a label object

void CCatalogue::AddL(CLabeledText *aText)
{
iCatalogue->AppendL(aText);
}

//-----------------------------------------------------------------------------

TBool CCatalogue::Delete( const TDesC& aLabel )
{
TInt  i;
TInt  c = iCatalogue->Count();
for ( i = 0; i < c; i++ )
	{
	CLabeledText* text = iCatalogue->At( i );
	if ( aLabel.Compare(text->Label()) == 0 )
		{
		iCatalogue->Delete( i );
		delete text;
		return ETrue;
		}
	} // end-for

return EFalse;
}

//-----------------------------------------------------------------------------

TInt CCatalogue::Count( )
{
return iCatalogue->Count();
}

//-----------------------------------------------------------------------------

CLabeledText *CCatalogue::At( TInt aIndex ) const
{
return iCatalogue->At( aIndex );
}

//-----------------------------------------------------------------------------

CLabeledText *CCatalogue::operator[]( TInt aIndex ) const
{
return iCatalogue->At( aIndex );
}

//-----------------------------------------------------------------------------

CLabeledText *CCatalogue::Text( const TDesC& aLabel ) const
{
TInt i;
TInt c = iCatalogue->Count();
for ( i = 0; i < c; i++ )
	{
	CLabeledText *text = iCatalogue->At( i );
	if ( aLabel.Compare(text->Label()) == 0 ) 
		return text;
	}

return NULL;
}

//-----------------------------------------------------------------------------

#else

//-----------------------------------------------------------------------------

#include "CCatalogue.h"										// This module

//-----------------------------------------------------------------------------

template <class T>
void CCatalogue<T>::AddL( const TDesC& aLabel, const TDesC& aValue )
{
T* text = T::NewLC( aLabel, aValue );
iCatalog->AppendL( text );
CleanupStack::Pop();
}

//-----------------------------------------------------------------------------

template <class T>
void CCatalogue<T>::AddL(T* aText )
{
iCatalog->AppendL( aText );
}

//-----------------------------------------------------------------------------

template <class T>
TBool CCatalogue<T>::Delete( const TDesC& aLabel )
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



