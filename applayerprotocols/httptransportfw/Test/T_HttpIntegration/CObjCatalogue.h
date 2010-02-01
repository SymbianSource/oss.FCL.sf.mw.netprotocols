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
// CObjCatalogue class carrying any number of labeled texts.
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Files  
// 
//

#include <e32base.h>
#include <e32cons.h>

//-----------------------------------------------------------------------------

#ifndef __COBJCATALOGUE_H__
#define __COBJCATALOGUE_H__

//-----------------------------------------------------------------------------

#if !defined(TEMPLATED_CATALOGUE)

//-----------------------------------------------------------------------------

class CAnyObject : public CBase
{
public:     // Constructors and destructors

	static CAnyObject *NewL ();
	static CAnyObject *NewLC();
	static CAnyObject *NewL (const TBuf<32> aName, const TInt &aIndex, const TInt &aTag, TAny *aPtr);
	static CAnyObject *NewLC(const TBuf<32> aName, const TInt &aIndex, const TInt &aTag, TAny *aPtr);
	~CAnyObject();

private:    // Constructors and destructors

	void ConstructL();
	void ConstructL(const TBuf<32> aName, const TInt &aIndex, const TInt &aTag, TAny *aPtr);
	CAnyObject() {};

public:		//	methods

	TBuf<32> Name(void) const {return iName;}
	void SetName(const TBuf<32> &aName) {iName = aName;}
	
	TInt Tag(void) const {return iTag;}
	void SetTag(const TInt &aTag) {iTag = aTag;}
	
	TInt Index(void) const {return iIndex;}
	void SetIndex(const TInt &aIndex) {iIndex = aIndex;}
	
	TAny *Ptr(void) const {return iPtr;}
	void SetPtr(TAny *aPtr) {iPtr = aPtr;}

private:	//	data members

	TBuf<32> iName;
	TInt iIndex;
	TInt iTag;
	TAny *iPtr;
};

//-----------------------------------------------------------------------------

class CObjCatalogue : public CBase
{

public:     // Constructors and destructors

	static CObjCatalogue *NewL ();
	static CObjCatalogue *NewLC( );
	~CObjCatalogue();

private:    // Constructors and destructors

	void ConstructL();
	void ConstructL(const TDesC &aName, const TInt &aIndex, const TInt &aTag, TAny *aPtr);
	CObjCatalogue() {};

public:     // New functions

	// Add a new one (no duplicate checks).
	void AddL(const TDesC &aName, const TInt &aIndex, const TInt &aTag, TAny *aPtr);
	void AddL(CAnyObject *aObject);

	// Delete the first text having the label. Returns ETrue
	// if was found and deleted, otherwise returns EFalse.
	TBool Delete(const TDesC &aLabel);
	TBool DeleteIndex(const TInt &aIndex);
	TBool DeleteTag(const TInt &aTag);

	// Count
	TInt Count();

	// The i'th text, legal index assumed.
	TAny *At(TInt aIndex ) const;
	TAny *operator[](TInt aIndex ) const;

	// Pointer by Label, return NULL if none.
	TAny *Name(const TDesC &aLabel ) const;

	// Pointer by Label, return NULL if none.
	TAny *Name(const TDesC &aLabel, TInt &aIndex) const;

	// Pointer by Tag, return NULL if none.
	TAny *Tag(const TInt &aTag) const;
	TAny *Tag(const TInt &aTag, TInt &aIndex) const;

private:    // Data

	// The catalog.
	CArrayPtrSeg<CAnyObject> *iObjects;

};

//-----------------------------------------------------------------------------

#else

//-----------------------------------------------------------------------------

#include "CPile.h"

//-----------------------------------------------------------------------------
//	CObjCatalogue class carrying any number of items

template <class T>
class CObjCatalogue : public CPile<T>, CBase
{

private:

protected:

	CObjCatalogue() : CPile<T> {};

public:     // New functions

	// Add a new one (no duplicate checks).
	void AddL(const TDesC& aLabel, const TDesC& aValue );
	void AddL(T *);

	// Delete the first text having the label. Returns ETrue
	// if was found and deleted, otherwise returns EFalse.
	TBool Delete( const TDesC&);
};

//-----------------------------------------------------------------------------

#endif	//	templated spec

//-----------------------------------------------------------------------------

#endif  // __COBJCATALOGUE_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------

