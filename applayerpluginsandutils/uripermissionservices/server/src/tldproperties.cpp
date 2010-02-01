// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "tldproperties.h"

CTldProperties* CTldProperties::NewL ( InetUriList::TListType aListType )
	{
	CTldProperties* self = new ( ELeave ) CTldProperties ( aListType );
	return self;
	}

CTldProperties::CTldProperties ( InetUriList::TListType aListType )
: iListType ( aListType )
	{
		
	}


CTldProperties::~CTldProperties ()
	{
	Clear();
	}

/**
Returns the listtype of the URI
*/
InetUriList::TListType CTldProperties::ListType () const
	{
	return iListType;
	}

/**
Returns the Tldname. Returns KNullDesC8 
if no Tldname is set.
*/
const TDesC8& CTldProperties::TldName () const
	{
	return iTldName;
	}

/**
Returns the CharacterSet. Returns KNullDesC8 
if no CharacterSet is set.
*/
const TDesC8& CTldProperties::CharacterSet () const
	{
	return iCharacterSet;
	}

/**
Set the listtype for the URI
*/
void CTldProperties::SetListType ( InetUriList::TListType aListType )
	{
	iListType = aListType;
	}


/**
Sets the TLDName
*/
void CTldProperties::SetTldNameL ( const TDesC8& aTldName )
	{
	iTldName.Close();
	iTldName.CreateL(aTldName);
	}
	
/**
Sets the allowed/ un-allowed character set
*/
void CTldProperties::SetCharacterSetL ( const TDesC8& aCharacterSet )
	{
	iCharacterSet.Close();
	iCharacterSet.CreateL(aCharacterSet);
	}

/**
Clear the properties value and sets the default

*/
void CTldProperties::Clear ()
	{
	iTldName.Close();
	iCharacterSet.Close();
	}


