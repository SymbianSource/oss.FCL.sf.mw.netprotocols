/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  inline implementation of class CCookie
*               
*
*/


// ---------------------------------------------------------
// CCookie::FromCookie2
// ---------------------------------------------------------
//
inline TBool CCookie::FromCookie2() const
	{
	return iSetCookie2;
	}

// ---------------------------------------------------------
// CCookie::FromNetscapeVersion
// ---------------------------------------------------------
//
inline TBool CCookie::FromNetscapeVersion() const
    {
    return iNetscape;
    }

// ---------------------------------------------------------
// CCookie::SetCookie2
// ---------------------------------------------------------
//
inline void CCookie::SetCookie2( TBool aFromCookie2 )
	{
	iSetCookie2 = aFromCookie2;
	}


// ---------------------------------------------------------
// CCookie::SetFromNetscape
// ---------------------------------------------------------
//
inline void CCookie::SetFromNetscape( TBool aFromNetscape )
	{
	iNetscape = aFromNetscape;
	}


// ---------------------------------------------------------
// CCookie::Size
// ---------------------------------------------------------
//
inline TInt CCookie::Size( TBool aSuppressDefaulted ) const
	{
	TInt size = KCookieMaxAttributeCount *
				   ( KCookieAttributePrefixLength + 
                     KCookieAttributeDefaultedLength + 
                     KCookieAttributeFoundLength ) 
                   + sizeof( TInt );

	for ( TInt i = 0; i < iAttributes.Count(); i++ )
		{
		if ( !( iAttributes[i].Defaulted() && aSuppressDefaulted ) )
			{
			size += iAttributes[i].Size();
			}
		}

	return size;
	}