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

#ifndef __THTTPHEADERVALUEVARIANT_H__
#define __THTTPHEADERVALUEVARIANT_H__

#include <http.h>
class RStringPool;

/**
 * THttpHeaderValueVariant represents a value found in the header element. The value of the header element 
 * can be of the various datatype like integer, string and datetime hence THttpHeaderValueVariant is a container
 * for them.  
 * 
 * @publishedAll
 * @prototype
 */

class THttpHeaderValueVariant
	{
friend class THttpHeaderParamIterator;
friend class THttpHeaderElementIterator;
friend class THttpHeaderIterator;
friend class CHttpClient;
friend class CHttpClientTransaction;
public:
    /**
     * Enumeration type.
     * 
     */
	typedef enum
			{
			ENoType 			= 0x00,  // No concrete datatype
			EIntType			= 0x01,  // Integer datatype
			EStrType 			=	0x02, // String datatype
			EDateTimeType	= 0x03 // Datetime datatype
			}THttpVariantType;
public:
	/**
	 * Default constructor
	 */
	inline THttpHeaderValueVariant();
	/**
	 * Constructor 
	 */
	inline THttpHeaderValueVariant(TInt aIntVariant);
	inline THttpHeaderValueVariant(const TDesC8& aStrVariant);
	inline THttpHeaderValueVariant(TDateTime aDateVariant);
	/**
	 * Set the integer value
	 * @param aIntVariant Integer value
	 */
	inline void SetInt(TInt aIntVariant);
	/**
	 * Set the string value
	 * @param aStrVariant String value
	 */
	inline void SetStr(const TDesC8& aStrVariant);
	/**
	 * Set the datettime value
	 * @param aDateVariant Datettime value
	 */
	inline void SetDateTime(TDateTime aDateVariant);
	
	/**
	 * Return the type that is represented by "this" object
	 * 
	 */
	inline THttpVariantType Type() const;
	
	/**
	 * Returns the contained integer. Will panic if the contained
	 * datatype is not an ineger
	 */
	inline TInt Int() const;
    
	/**
     * Returns the contained string. Will panic if the contained
     * datatype is not an string
     */	
	inline TPtrC8 Str() const;
	
    /**
     * Returns the contained datetime value. Will panic if the contained
     * datatype is not a datettime value
     */ 
	inline TDateTime DateTime() const;

private:	
	THttpVariantType	               iVariantType;
	TInt                               iVariantVal;
	TInt							   iMoreSpace;
	};


inline THttpHeaderValueVariant::THttpHeaderValueVariant()
: iVariantVal(0),
iVariantType(ENoType),
iMoreSpace(0)
	{
	}

inline THttpHeaderValueVariant::THttpHeaderValueVariant(TInt aInt)
: iVariantVal(aInt),
iVariantType(EIntType),
iMoreSpace(0)
	{
	}

inline THttpHeaderValueVariant::THttpHeaderValueVariant(const TDesC8& aStr)
: iVariantVal(reinterpret_cast<TInt>(const_cast<TUint8*>(aStr.Ptr()))),
iVariantType(EStrType),
iMoreSpace(aStr.Length())
	{
	}

inline THttpHeaderValueVariant::THttpHeaderValueVariant(TDateTime aDateTime)
: iVariantType(EDateTimeType),
	iMoreSpace(0)
	{
	*reinterpret_cast<TTime*>(&iVariantVal) = aDateTime;
	}
	
inline void THttpHeaderValueVariant::SetInt(TInt aInt)
	{
	iVariantVal = aInt;
	iMoreSpace = 0;
	iVariantType = EIntType;
	}
		
inline void THttpHeaderValueVariant::SetStr(const TDesC8& aStr)
	{	
	iVariantVal = reinterpret_cast<TInt>(const_cast<TUint8*>(aStr.Ptr()));	
	iMoreSpace = aStr.Length();	
	iVariantType = EStrType;	
	}
	
inline void THttpHeaderValueVariant::SetDateTime(TDateTime aDateTime)
	{
	*reinterpret_cast<TTime*>(&iVariantVal) = aDateTime;
	iVariantType = EDateTimeType;
	iMoreSpace = 0;
	}
	
inline THttpHeaderValueVariant::THttpVariantType THttpHeaderValueVariant::Type() const
	{
	return iVariantType;
	}

inline TInt THttpHeaderValueVariant::Int() const
	{
	__ASSERT_DEBUG(iVariantType == EIntType, User::Invariant());
	return iVariantVal;
	}

inline TPtrC8 THttpHeaderValueVariant::Str() const
	{
	__ASSERT_DEBUG(iVariantType == EStrType, User::Invariant());
	
	TUint8* actualAddr = reinterpret_cast<TUint8*>(iVariantVal);
	TPtrC8 ptr;
	ptr.Set(actualAddr, iMoreSpace);	
	return ptr;
	}
	
inline TDateTime THttpHeaderValueVariant::DateTime () const
	{
	__ASSERT_DEBUG(iVariantType == EDateTimeType, User::Invariant());
	return reinterpret_cast<const TTime*>(&iVariantVal)->DateTime();	
	}
#endif
