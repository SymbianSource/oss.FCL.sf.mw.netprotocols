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
* Description:  Declaration of class CookieManagerSession
*               
*
*/


#ifndef __COOKIEMANAGER_SESSION_H__
#define __COOKIEMANAGER_SESSION_H__

// INCLUDE FILES
	// System includes
#include <e32base.h>
#include <e32def.h>

	// User includes
#include "CookieClientPanic.h"

// FORWARD DECLARATIONS

class CCookie;
class CCookieArray;
class CCookieManagerServer;
class CCookieServer;
class RMessage;
class RThread;
class TCookiePacker;
class TUriC8;

// CLASS DECLARATION

/**
*
*/
NONSHARABLE_CLASS( CCookieManagerSession ) : public CSession2
	{
	public:
		/**
		*
		*/
		static CCookieManagerSession* NewL( CCookieManagerServer& aServer );

		/**
		*
		*/
		~CCookieManagerSession();

	public :	// from CSession2
		/**
		*
		*/
		virtual void ServiceL( const RMessage2& aMessage );

	private :	// construction

		/**
		*
		*/
		CCookieManagerSession( CCookieManagerServer& aServer );

		/**
		* Second-phase constructor.
		*/
		void ConstructL();

	private :	// internal methods

		/**Clear all Cookies from file system belongs to a Client
		*
		*/
		TInt ClearAllCookies( const RMessage2& aMessage );

		/** Clear all Cookies from File System based on appuid 
        *
        */
        TInt ClearAllAppUidCookies( const RMessage2& aMessage );

		/**
		*
		*/
		TInt DoClearAllCookies( const RMessage2& aMessage );

        /**
        *
        */
        TInt DoClearAllAppUidCookies( const RMessage2& aMessage );

		/**Gets Cookie Size for a Url
		*
		*/
		TInt DoGetCookieSize( const RMessage2& aMessage );

		/** Gets Cookie info for a Url
		*
		*/
		TInt DoGetCookies( const RMessage2& aMessage );

		/**Stores cookie info for a specific url.
		*
		*/
		void DoStoreCookieL( const RMessage2& aMessage );

		/**
		*
		*/
		TInt GetCookieSize( const RMessage2& aMessage );

		/**
		*
		*/
		TInt GetCookies( const RMessage2& aMessage );

		/**
		*
		*/
		void PanicClient( const RMessage2& aMessage, 
                          TCookieClientPanic aPanic ) const;

		/**
		*
		*/
		TInt StoreCookie( const RMessage2& aMessage );

		/**Sets the appuid for a request
		*
		*/
		TInt SetAppUidL(const RMessage2& aMessage );
		
		/*Delets cookie from Server memory and writes the cookie Info in to a file 
		 * 
		 */
		TInt DestroyCookies( const RMessage2& aMessage );
		
		/*
		 * 
		 */
		TInt DoDestroyCookies( const RMessage2& aMessage );
		
		/*
		 * 
		 */
        TInt GetCookieSharableFlag( const RMessage2& aMessage );

        /*
         * 
         */
	    TInt DoGetCookieSharableFlag( const RMessage2& aMessage );

	private:
		CCookieManagerServer&   iCookieServer;
		// TODO : RStringPool* --> RStringPool&
		RStringPool*            iStringPool;	///< does not own it,
												///< reads from the server

		TCookiePacker iCookiePacker;

		// As a 'get cookies' request consists of two steps (size inquiry and
		// passing cookies), we have to store the resulting cookie list
		// temporarily between the two calls.
		RPointerArray<CCookie> iGetCookieList;

		// The size of the temporary cookie list in bytes
		TInt iGetCookieListSize;

	};

#endif //__COOKIEMANAGER_SESSION_H__
