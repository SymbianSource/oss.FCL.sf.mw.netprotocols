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
// CCatalogue class carrying any number of labeled texts.
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Files  
// 
//

#include <e32base.h>
#include <e32cons.h>

//-----------------------------------------------------------------------------

#ifndef __CCATALOGUE_H__
#define __CCATALOGUE_H__

//-----------------------------------------------------------------------------

#include "CLabeledText.h"

//-----------------------------------------------------------------------------

#if !defined(TEMPLATED_CATALOGUE)

//-----------------------------------------------------------------------------

class CCatalogue : public CBase
{

public:     // Constructors and destructors

	static CCatalogue *NewL ( );
	static CCatalogue *NewLC( );
	~CCatalogue();

private:    // Constructors and destructors

	void ConstructL( );
	CCatalogue() {};

public:     // New functions

	// Add a new one (no duplicate checks).
	void AddL( const TDesC& aLabel, const TDesC& aValue );
	void AddL( CLabeledText* aLabeledText );

	// Delete the first text having the label. Returns ETrue
	// if was found and deleted, otherwise returns EFalse.
	TBool Delete( const TDesC& aLabel );

	// Count
	TInt Count( );

	// The i'th text, legal index assumed.
	CLabeledText *At( TInt aIndex ) const;
	CLabeledText *operator[]( TInt aIndex ) const;

	// Text by Label, return NULL if none.
	CLabeledText *Text( const TDesC& aLabel ) const;

private:    // Data

	// The catalog.
	CArrayPtrSeg<CLabeledText> *iCatalogue;

};

//-----------------------------------------------------------------------------

#else

//-----------------------------------------------------------------------------

#include "CPile.h"

//-----------------------------------------------------------------------------
//	CCatalogue class carrying any number of items

template <class T>
class CCatalogue : public CPile<T>, CBase
{

private:

protected:

	CCatalogue() : CPile<T> {};

public:     // New functions

	// Add a new one (no duplicate checks).
	void AddL(const TDesC& aLabel, const TDesC& aValue );
	void AddL(T *);

	// Delete the first text having the label. Returns ETrue
	// if was found and deleted, otherwise returns EFalse.
	TBool Delete( const TDesC&);
};

//-----------------------------------------------------------------------------

/*
template <class T>
class CArrayFixFlat : public CArrayFix<T>
	{
public:
	inline CArrayFixFlat(TInt aGranularity);
	inline void SetReserveL(TInt aCount);
	};
*/
/*
TEMPLATE_SPECIALIZATION class CCatalogue<CLabeledText> : public CPile<CLabeledText>
{
public:     // New functions

	CCatalogue<CLabeledText> {};
	//CCatalogue<CLabeledText> : CPile<CLabeledText> {};

	// Add a new one (no duplicate checks).
	void AddL(const TDesC& aLabel, const TDesC& aValue );
	void AddL(T *);

	// Delete the first text having the label. Returns ETrue
	// if was found and deleted, otherwise returns EFalse.
	TBool Delete( const TDesC&);
	
	//inline CArrayFixFlat(TInt aRecordLength,TInt aGranularity);
	//inline void SetReserveL(TInt aCount);
};
*/

#endif  // templated spec

//-----------------------------------------------------------------------------

#endif  // __CCATALOGUE_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------

