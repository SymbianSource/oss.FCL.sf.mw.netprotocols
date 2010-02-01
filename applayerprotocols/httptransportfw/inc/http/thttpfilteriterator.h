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

/**
 @file
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__THTTPFILTERITERATOR_H__
#define	__THTTPFILTERITERATOR_H__ 

// System includes
#include <e32std.h>
#include <stringpool.h>
#include <http/thttpfilterregistration.h>

// Forward declarations
class CHTTPSession;


//##ModelId=3C4C187A0152
class THTTPFilterIterator 
/** 
An iterator for RHTTPFilterCollection. The iterator can iterate
over a subset of the filters in the collection, depending on how it
was created. See RHTTPFilterCollection::Query for more details. In
the documentation here, 'the collection' always refers to the
filters that this iterator can see.
@publishedAll
@released
*/
	{
 public:
	/** Non-initialised constructor. Note, to get an initialised one
        that actualy points to a filter collection, use
        RHTTPFilterCollection::Query */
	//##ModelId=3C4C187A01A2
	inline THTTPFilterIterator();

	/** Moves the iterator to the first member of the collection */
	//##ModelId=3C4C187A019A
	inline void First();

	/** Returns ETrue if the iterator is at the end of the collection */
	//##ModelId=3C4C187A0199
	IMPORT_C TBool AtEnd() const;

	/** Moves on to the next item in the collection */
	//##ModelId=3C4C187A0198
	IMPORT_C THTTPFilterIterator& operator++();

	/** Returns the filter that the iterator is pointing at */
	//##ModelId=3C4C187A0190
	IMPORT_C const THTTPFilterRegistration& operator()() const;

	/** Returns the filter that the iterator is pointing at */
	//##ModelId=3C4C187A018F
	inline const THTTPFilterRegistration& operator*() const;

	/** Returns the filter that the iterator is pointing at */
	//##ModelId=3C4C187A018E
	inline const THTTPFilterRegistration& operator->() const;

 private:
	friend class RHTTPFilterCollection;

	//##ModelId=3C4C187A017C
	CHTTPSession* iImplementation;

	//##ModelId=3C4C187A0172
	TInt iPos;

	//##ModelId=3C4C187A0168
	RStringTokenF iName;
	};


inline THTTPFilterIterator::THTTPFilterIterator()
		: iImplementation(NULL), iPos(0)
	{
	}

inline void THTTPFilterIterator::First()
	{
	iPos = -1;
	operator++();
	}

inline const THTTPFilterRegistration& THTTPFilterIterator::operator*() const
	{
	return operator()();
	}

inline const THTTPFilterRegistration& THTTPFilterIterator::operator->() const
	{
	return operator()();
	}



#endif //	__THTTPFILTERITERATOR_H__
