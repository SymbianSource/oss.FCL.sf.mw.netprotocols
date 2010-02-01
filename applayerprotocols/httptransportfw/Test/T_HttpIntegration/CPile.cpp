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
// Noticed lots of classes want to 'pile' up objects (as in stack 'em) but
// all create their own 'class'. Stuff that for a game of soldiers...
// here's a templated class that serves all!
// mjd, mark@mjdss.com, july 2002
// 
//

#include "CPile.h"

//-----------------------------------------------------------------------------
//	the NewL calls default values (e.g. 16) in ConstructL

template <class T>
CPile<T> * CPile<T>::NewL()
{
CPile* self = NewLC();
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------
//	the NewL calls passes desired default size to ConstructL

template <class T>
CPile<T> * CPile<T>::NewL(const TInt &aElements)
{
CPile *self = NewLC(aElements);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

template <class T>
CPile<T> * CPile<T>::NewLC(const TInt &aElements)
{
CPile *self = new (ELeave) CPile();
CleanupStack::PushL(self);
self->ConstructL(aElements);
return self;
}

//-----------------------------------------------------------------------------
//	destroy the pile of T!
//	not forgetting to delete the stacks contents first!

template <class T>
CPile<T>::~CPile()
{
iPile->ResetAndDestroy();
delete iPile;
iPile = NULL;
}

//-----------------------------------------------------------------------------
//	would be nice to be able to easily specify in the constructor
//	the size of the initial stack...
//	mjd: added in the aElements which is now passed (via default) from NewL

template <class T>
void CPile<T>::ConstructL(const TInt &aElements)
{
iPile = new (ELeave) CArrayPtrSeg<T>(aElements);
}

//-----------------------------------------------------------------------------
//	ok, push an object of class T onto the stack

template <class T>
void CPile<T>::PushL(T *aT)
{
iPile->AppendL(aT);
}

//-----------------------------------------------------------------------------
//	pop an object of class T off the top of the pile (stack) or
//	NULL if nothing to get...
//	note: Count is always 1 more than actual contents of stack...

template <class T>
T *CPile<T>::Pop()
{
TInt count = Count();
if (count > 0)
  {
  T *t = At(--count);
  iPile->Delete(count);
  return t;
  }
else
  return NULL;
}

//-----------------------------------------------------------------------------
//	zap the objcet of class T sitting at the top of the stack

template <class T>
void CPile<T>::Skim()
{
T *t = Pop();
delete t;
}

//-----------------------------------------------------------------------------
//	zap all objects of class T sitting on the stack

template <class T>
void CPile<T>::Empty()
{
while (iPile->Count())
  Skim();
}

//-----------------------------------------------------------------------------
//	How many do we have - this SHOULD be a property!

template <class T>
TInt CPile<T>::Count() const
{
return iPile->Count();
}

//-----------------------------------------------------------------------------
//	get the n'th item in the list 
//	indexed from 0 so [0] is valid.

template <class T>
T *CPile<T>::At(const TInt &aIndex) const
{
if ((aIndex >= 0) || (aIndex < Count()))
	return iPile->At(aIndex);
return NULL;
}

//-----------------------------------------------------------------------------
//	get the [n]'th item in the list 

template <class T>
T *CPile<T>::operator[](const TInt &aIndex) const
{
return At(aIndex);
}

//-----------------------------------------------------------------------------

template <class T>
T *CPile<T>::Peek()
{
TInt count = Count();
if (count > 0)
  {
  T *t = At(--count);
  return t;
  }

return NULL;
}
//-----------------------------------------------------------------------------
//	Template Specialization requirements: CLogFile
//-----------------------------------------------------------------------------

CPile<CLogFile> * CPile<CLogFile>::NewL()
{
CPile *self = NewLC();
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CPile<CLogFile> *CPile<CLogFile>::NewLC(const TInt &aElements)
{
CPile *self = new (ELeave) CPile();
CleanupStack::PushL(self);
self->ConstructL(aElements);
return self;
}

//-----------------------------------------------------------------------------

void CPile<CLogFile>::ConstructL(const TInt &aElements)
{
iPile = new (ELeave) CArrayPtrSeg<CLogFile>(aElements);
}

//-----------------------------------------------------------------------------

void CPile<CLogFile>::PushL(CLogFile *aT)
{
iPile->AppendL(aT);
}

//-----------------------------------------------------------------------------

CLogFile *CPile<CLogFile>::Pop()
{
TInt count = Count();
if (count > 0)
  {
  CLogFile *t = At(--count);
  iPile->Delete(count);
  return t;
  }
else
  return NULL;
}

//-----------------------------------------------------------------------------

CLogFile *CPile<CLogFile>::At(const TInt &aIndex) const
{
if ((aIndex >= 0) || (aIndex < Count()))
	return (CLogFile *)iPile->At(aIndex);
	//return (CLogFile *) &iPile[aIndex];
return NULL;
}

//-----------------------------------------------------------------------------

TInt CPile<CLogFile>::Count() const
{
return iPile->Count();
}

//-----------------------------------------------------------------------------

CPile<CLogFile>::~CPile()
{
iPile->ResetAndDestroy();
delete iPile;
iPile = NULL;
}


//-----------------------------------------------------------------------------
//	Template Specialization requirements:	CCmdFile
//-----------------------------------------------------------------------------

CPile<CCmdFile> *CPile<CCmdFile>::NewL()
{
CPile *self = NewLC();
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CPile<CCmdFile> *CPile<CCmdFile>::NewLC(const TInt &aElements)
{
CPile *self = new (ELeave) CPile();
CleanupStack::PushL(self);
self->ConstructL(aElements);
return self;
}

//-----------------------------------------------------------------------------

CPile<CCmdFile>::~CPile()
{
iPile->ResetAndDestroy();
delete iPile;
iPile = NULL;
}

//-----------------------------------------------------------------------------

void CPile<CCmdFile>::ConstructL(const TInt &aElements)
{
iPile = new (ELeave) CArrayPtrSeg<CCmdFile>(aElements);
}

//-----------------------------------------------------------------------------

TInt CPile<CCmdFile>::Count() const
{
return iPile->Count();
}

//-----------------------------------------------------------------------------

CCmdFile *CPile<CCmdFile>::At(const TInt &aIndex) const
{
if ((aIndex >= 0) || (aIndex < Count()))
	return (CCmdFile *)iPile->At(aIndex);
	//return (CCmdFile *) &iPile[aIndex];
return NULL;
}

//-----------------------------------------------------------------------------

void CPile<CCmdFile>::PushL(CCmdFile *aT)
{
iPile->AppendL(aT);
}

//-----------------------------------------------------------------------------

CCmdFile *CPile<CCmdFile>::Pop()
{
TInt count = Count();
if (count > 0)
  {
  CCmdFile *t = At(--count);
  iPile->Delete(count);
  return t;
  }
else
  return NULL;
}

//-----------------------------------------------------------------------------

void CPile<CCmdFile>::Skim()
{
CCmdFile *t = Pop();
delete t;
}

//-----------------------------------------------------------------------------

void CPile<CCmdFile>::Empty()
{
while (iPile->Count())
  Skim();
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Template Specialization requirements:	CIFControl
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CPile<CIFControl> *CPile<CIFControl>::NewL()
{
CPile *self = NewLC();
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CPile<CIFControl> *CPile<CIFControl>::NewLC(const TInt &aElements)
{
CPile *self = new (ELeave) CPile();
CleanupStack::PushL(self);
self->ConstructL(aElements);
return self;
}

//-----------------------------------------------------------------------------

void CPile<CIFControl>::ConstructL(const TInt &aElements)
{
iPile = new (ELeave) CArrayPtrSeg<CIFControl>(aElements);
}

//-----------------------------------------------------------------------------

CIFControl *CPile<CIFControl>::At(const TInt &aIndex) const
{
if ((aIndex >= 0) || (aIndex < Count()))
	return (CIFControl *)iPile->At(aIndex);
return NULL;
}

//-----------------------------------------------------------------------------

void CPile<CIFControl>::PushL(CIFControl *aT)
{
iPile->AppendL(aT);
}

//-----------------------------------------------------------------------------

TInt CPile<CIFControl>::Count() const
{
return iPile->Count();
}

//-----------------------------------------------------------------------------

CIFControl *CPile<CIFControl>::Pop()
{
TInt count = Count();
if (count > 0)
  {
  CIFControl *t = At(--count);
  iPile->Delete(count);
  return t;
  }

return NULL;
}

void CPile<CIFControl>::Skim()
{
	CIFControl *t = Pop();
	delete t;
}

//-----------------------------------------------------------------------------

CIFControl *CPile<CIFControl>::Peek()
{
TInt count = Count();
if (count > 0)
  {
  CIFControl *t = At(--count);
  return t;
  }

return NULL;
}


//-----------------------------------------------------------------------------

CPile<CIFControl>::~CPile()
{
iPile->ResetAndDestroy();
delete iPile;
iPile = NULL;
}

//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------
