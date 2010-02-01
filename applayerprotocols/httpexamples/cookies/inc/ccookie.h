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

#ifndef		__CCOOKIE_H__
#define		__CCOOKIE_H__

#include <e32std.h>
#include <stringpool.h>
#include <http/thttphdrval.h>

/** Class to store the data that makes up a cookie
*/
class CCookie : public CBase
	{
public:
	/** Enumeration of all the elements of data that make up a cookie
	*/
	enum TCookieAttributeName
		{
		EName,			// Name of the cookie - mandatory
		EValue,			// Value associated with the name - mandatory
		EComment,		// Comment which can be displayed to the user
		ECommentURI,	// A URI which can be visited for more information
		EDiscard,		// Flag indicating that the cookie should be discarded
		EDomain,		// Domain which this cookie applies to
		EMaxAge,		// Age after which this cookie should be deleted
		EPath,			// Path which this cookie applies to
		EPort,			// Port numbers that this cookie applies to
		ESecure,		// Flag indicating this cookie should only be used for secure transactions
		EVersion,		// Version of style of cookie
		EExpires,		// Time after which this cookie should be deleted - Netscape cookies only
		};

	/** Standard C class construction method
		@param	aStringPool	A string pool to be used by this cookie
		@param	aName		Name attribute for this cookie
		@param	aValue		Value attribute for this cookie
		@param	aCookie2	Flag indicating if this cookie was received in a SetCookie2 header
		@return	Pointer to a fully constructed CCookie
	*/
	static CCookie* NewL(RStringPool aStringPool, RString aName, RString aValue, TBool aCookie2);

	/** Standard destructor
	*/
	~CCookie();
	
	/** SetAttributeL sets an attribute on the cookie. 
		Adding an attribute where one exists will replace the existing one
		This function may leave with a system wide error code if the type of the value is not correct
		or if the attempt to append the attribute to an internal list fails.
		@param aAttributeName identifies the attribute 
		@param aAttributeVal the value being set
		@leave KErrNoMemory if there is not enough memory
	*/
	void SetAttributeL(TCookieAttributeName aAttributeName, THTTPHdrVal aAttributeVal);
	
	/** Attribute retrieves the value of an attribute
		@param	aAttributeName	Identifies the attribute 
		@param	aAttributeVal	The value being retrieved
		@return	An error code. KErrNotFound will indicate that the cookie doesn't 
		contain the specified attribute.
	*/
	TInt Attribute(TCookieAttributeName aAttributeName, THTTPHdrVal& aAttributeVal) const;

	/** Did this cookie come from a SetCookie2 header?
		@return	Flag indicating if this cookie was received in SetCookie2 header (rather 
				than a SetCookie header).
	*/
	TBool FromCookie2() const;

private:
	/** Class to store the name and value of an individual cookie attribute
	*/
	class TCookieAttribute
		{
	public:
	/** Constructor that initialises name and value of attribute
		@param	aName	Identifies the attribute to be set
		@param	aHdrVal	The value of the attribute
	*/
		TCookieAttribute(TCookieAttributeName aName, THTTPHdrVal aHdrVal);

	/** Default constructor
	*/
		TCookieAttribute();
	public:
		/** The cookie attribute
		*/
		TCookieAttributeName iName;

		/** The value of this attribute
		*/
		THTTPHdrVal iValue;
		};

	/** Constructor for CCookie
		@param	aStringPool	String pool to be used by this cookie
		@param	aCookie2	Flag indicating if this cookie was received in a SetCookie2 header
	*/
	CCookie(RStringPool aStringPool, TBool aCookie2);

	/** Standard C class second phase construction method
		@param aName		Name attribute for this cookie
		@param aValue		Value attribute for this cookie
	*/
	void ConstructL(RString aName, RString aValue);

	/** Closes a cookie attribute - used to ensure string attributes are
		properly closed on destruction
		@param aCookieAttribute	The attribute to be closed
	*/
	void CloseAttribute(TCookieAttribute aCookieAttribute);

	/** Finds the specified attribute in this cookie
		@param aAttribute		
		@param aAttributeName	
		@return The index in iAttributes of the specified attribute. KErrNotFound indicates 
		that the cookie does not contain the attribute.
	*/
	TInt FindAttribute(TCookieAttributeName aAttributeName, TCookieAttribute& aAttribute) const;

private:
	/** String pool that can be used by this cookie
	*/
	RStringPool iStringPool;
	
	/** Array of the attributes of this cookie
	*/
	RArray<TCookieAttribute> iAttributes;

	/** Flag indicating whether this cookie was received in a SetCookie2 header
	*/
	TBool iSetCookie2;
	};

#include "ccookie.inl"

#endif //	__CCOOKIE_H__

