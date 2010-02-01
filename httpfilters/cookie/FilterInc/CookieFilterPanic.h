/*
* Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/


#ifndef __COOKIEFILTERPANIC_H__
#define __COOKIEFILTERPANIC_H__

// INCLUDE FILES
	// System includes
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION


/**
*
*/
class TCookieFilterPanic
	{
	public :	// public enums
		enum TCookieFilterPanicCode
			{
			EInvalidFilterState,
			EGetCookiesArrayNotEmpty,
       		ECookieListItemsAlreadyOwned,
	        ECookieListItemsAlreadyReleased
			};

	public :	// public methods
		static void Panic( TCookieFilterPanicCode aPanic );
	};

inline void TCookieFilterPanic::Panic
						( TCookieFilterPanic::TCookieFilterPanicCode aPanic )
	{
	_LIT( KCookiePanicString, "HTTP-COOKIE" );
	User::Panic( KCookiePanicString, aPanic );
	}

#endif // __COOKIEFILTERPANIC_H__