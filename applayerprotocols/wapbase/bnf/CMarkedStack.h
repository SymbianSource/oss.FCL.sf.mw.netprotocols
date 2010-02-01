// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Template class to implement a marked stack. Conatins a stack itself of marks 
// providing basic marking functionality.
// 
//

#ifndef __CMARKEDSTACK_H__
#define __CMARKEDSTACK_H__

// includes
#include <e32base.h>
#include <cstack.h>

//##ModelId=3B666BCD0001

template <class T, TBool Owner>
class CMarkedStack : public CStack<T, Owner>
/** Provides a templated stack that allows items in the stack to be marked.

The class allows each mark to have an associated TInt value, which allows different 
types of mark to be used on the same stack.

Template parameter T specifies the type of object on the stack. Owner should be set to 
ETrue if the object's destructor should delete the objects on the stack.
    @publishedAll
    @deprecated
 
*/
{
protected:
class TMarkPoint
/** Represents a mark.
 */
{
public:
	/** Represents a mark. */	
	TMarkPoint(TInt aMarkType
		   , TInt aIndex)
	  : iMarkType(aMarkType)
	  , iStackIndex(aIndex)
	{}
	/** Mark type. */
	TInt iMarkType;
	/** Index of the marked stack item. */
	TInt iStackIndex;
};
/** Defines a stack of marks. */
typedef CStack<TMarkPoint, ETrue> CMarks;
public:
	//##ModelId=3B666BCD0048
	inline virtual ~CMarkedStack();

	//##ModelId=3B666BCD0041
	inline void MarkL(TInt aMarkType);
	//##ModelId=3B666BCD003F
	inline TInt RemoveMark(TInt aMarkType);
	//##ModelId=3B666BCD003D
	inline void DeleteToMark(TInt aMarkType);
	//##ModelId=3B666BCD0033
	inline void ResetToMark(TInt aMarkType);
protected:
	/** A stack of marks.

	Items are added and removed from this stack by MarkL(), RemoveMark() etc.
	*/
	//##ModelId=3B666BCD002D
	CMarks iMarks;
};

#include <cmarkedstack.inl>

#endif // __CMARKEDSTACK_H__
