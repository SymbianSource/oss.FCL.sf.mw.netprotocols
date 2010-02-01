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

#include <httpstringconstants.h>

inline CCookie* CCookie::NewL(RStringPool aStringPool, RString aName, RString aValue, TBool aCookie2)
	{
	CCookie* cookie = new(ELeave)CCookie(aStringPool, aCookie2);
	CleanupStack::PushL(cookie);
	cookie->ConstructL(aName, aValue);
	CleanupStack::Pop(cookie);
	return cookie;
	}

inline CCookie::CCookie(RStringPool aStringPool, TBool aCookie2) :
	iStringPool(aStringPool),
	iSetCookie2(aCookie2)
	{
	}

inline CCookie::~CCookie()
	{
	// go through each attribute closing any attribute strings	
	const TInt numAttributes= iAttributes.Count();
	for (TInt ii=0; ii<numAttributes; ++ii)
		CloseAttribute(iAttributes[ii]);
	iAttributes.Reset();
	}

inline void CCookie::CloseAttribute(CCookie::TCookieAttribute aCookieAttribute)
	{
	THTTPHdrVal attributeVal = aCookieAttribute.iValue;
	if (attributeVal.Type() == THTTPHdrVal::KStrVal)
		attributeVal.Str().Close();
	else if (attributeVal.Type() == THTTPHdrVal::KStrFVal)
		attributeVal.StrF().Close();	
	}

inline void CCookie::ConstructL(RString aName, RString aValue)
	{
	THTTPHdrVal value(aName);
	SetAttributeL(EName, value);
	value.SetStr(aValue);
	SetAttributeL(EValue, value);
	}

inline void CCookie::SetAttributeL(CCookie::TCookieAttributeName aAttributeName, 
									 THTTPHdrVal aAttributeVal)
	{
	TCookieAttribute attribute;
	TInt index = FindAttribute(aAttributeName, attribute);
	if(index != KErrNotFound)
		{
		CloseAttribute(attribute);
		iAttributes.Remove(index);
		}

	TCookieAttribute newAttribute(aAttributeName, aAttributeVal);
	User::LeaveIfError(iAttributes.Append(newAttribute));
	}

inline TInt CCookie::Attribute(CCookie::TCookieAttributeName aAttributeName, 
								 THTTPHdrVal& aAttributeVal) const
	{
	TCookieAttribute attribute;
	if(FindAttribute(aAttributeName, attribute) != KErrNotFound)
		{
		aAttributeVal = attribute.iValue;
		return KErrNone;
		}

	return KErrNotFound;
	}

inline TInt CCookie::FindAttribute(TCookieAttributeName aAttributeName, TCookieAttribute& aAttribute) const
	{
	const TInt numAttributes = iAttributes.Count();
	for (TInt index = 0; index<numAttributes; ++index)
		{
		if (iAttributes[index].iName == aAttributeName)
			{
			aAttribute = iAttributes[index];
			return index;
			}
		}

	return KErrNotFound;
	}

inline TBool CCookie::FromCookie2() const
	{
	return iSetCookie2;
	}

inline CCookie::TCookieAttribute::TCookieAttribute(TCookieAttributeName aName, THTTPHdrVal aHdrVal) :
iName(aName)
	{
	iValue = aHdrVal.Copy();
	}

inline CCookie::TCookieAttribute::TCookieAttribute()
	{
	}

