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
// mjd, mark@mjdss.com, july 2002
// During implementation found a 'known' bug in the MSVC++ v6.0 compiler.
// The C2955 bug. 
// 
//

#ifndef __CPILE_H__
#define __CPILE_H__

//-----------------------------------------------------------------------------

#include <e32base.h>
#include <e32cons.h>

//-----------------------------------------------------------------------------

#include "CLogFile.h"
#include "CCmdFile.h"
#include "CIFControl.h"

//-----------------------------------------------------------------------------

template <class T>
class CPile : public CBase
{
public:     // Constructors and destructors

	static CPile *NewL ();		//	these were both static - why???
	static CPile *NewL (const TInt &);		//	these were both static - why???
	static CPile *NewLC(const TInt & = 16);
	~CPile();

protected:    

	//	this may be required by derived classes...
	virtual void ConstructL(const TInt &);

private:

	//	this remains private
	CPile() {};

public:     // New functions

	// Push to pile.
	virtual void PushL(T *);

	// Pop from pile. Returns NULL is none.
	virtual T *Pop();

	// Pop from pile (skim) and delete (empty).
	virtual void Skim();		//	skim off top
	virtual void Empty();		//	Empty the stack completely
	virtual T *Peek();			//	Peek at the current (top) item

	// Get count and the it'h T, legal i assumed.
	virtual TInt Count() const;
	virtual T *At(const TInt &) const;
	virtual T *operator[](const TInt &) const;

private:    // Data

	// Pile (stack) of T.
	CArrayPtrSeg<T> *iPile;
	//RPointerArray<T> *iPile;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	For the specializations

class CLogFile;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEMPLATE_SPECIALIZATION class CPile<CLogFile> : public CBase
{
public:     // New functions

	static CPile *NewL ();
	static CPile *NewLC(const TInt & = 16);
	~CPile();

protected:    

	//	this may be required by derived classes...
	virtual void ConstructL(const TInt &);

public:     // New functions

	// Push to pile.
	void PushL(CLogFile *);

	// Pop from pile. Returns NULL is none.
	CLogFile *Pop();

	// Get count and the it'h T, legal i assumed.
	TInt Count() const;

	CLogFile *At(const TInt &) const;

private:    // Data

	CArrayPtrSeg<CLogFile> *iPile;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	For the specializations

TEMPLATE_SPECIALIZATION class CPile<CCmdFile> : public CBase
{
public:     // New functions

	static CPile *NewL ();
	static CPile *NewLC(const TInt & = 16);
	~CPile();

protected:    

	//	this may be required by derived classes...
	virtual void ConstructL(const TInt &);

public:     // New functions

	// Push to pile.
	void PushL(CCmdFile *);

	// Pop from pile. Returns NULL is none.
	CCmdFile *Pop();

	// Get count and the it'h T, legal i assumed.
	TInt Count() const;

	void Skim();		//	skim off top
	void Empty();		//	Empty the stack completely

	CCmdFile *At(const TInt &) const;

private:    // Data

	CArrayPtrSeg<CCmdFile> *iPile;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	For the specializations

TEMPLATE_SPECIALIZATION class CPile<CIFControl> : public CBase
{
public:     // New functions

	static CPile *NewL ();
	static CPile *NewLC(const TInt & = 16);
	~CPile();

protected:    

	//	this may be required by derived classes...
	virtual void ConstructL(const TInt &);

public:     // New functions

	// Push to pile.
	void PushL(CIFControl *);

	// Pop from pile. Returns NULL is none.
	CIFControl *Pop();
	void Skim();		//	skim off top

	// Get count and the it'h T, legal i assumed.
	TInt Count() const;

	//	return the items At(x)
	CIFControl *At(const TInt &) const;

	//	Peek at the current (top) item
	CIFControl *Peek();	

private:    // Data

	CArrayPtrSeg<CIFControl> *iPile;
};

//-----------------------------------------------------------------------------

#endif	//	__CPILE_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------

