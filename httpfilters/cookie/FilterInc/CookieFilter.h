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


#ifndef __HTTPCOOKIE_FILTER_H__
#define __HTTPCOOKIE_FILTER_H__

// INCLUDE FILES
	// System includes
#include "cookie.h"
#include "cookiemanagerclient.h"
#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>

	// User includes
#include "cookiefilterinterface.h"

// CLASS DECLARATIONS

/**
* CCookieFilter is used to trap outgoing requests, to add cookies to them,
* and incoming responses, to extract cookies from them.
*/
class CCookieFilter : public CHttpCookieFilter, public MHTTPFilter
	{
	public :
		/**
		* Installs the cookie filter, supplying it with this callback
		* interface.
		* @param aSession the session to install the filter into.
		* @exception KErrNoMemory There was not enough memory.
		*/
		static CHttpCookieFilter* InstallFilterL( TAny* aFilterParams );

		/**
		* Standard destructor
		*/
		~CCookieFilter();

	public :	// from MHTTPFilter
		/**
		* @see MHTTPFilter
		*/
		virtual void MHFRunL( RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent );

		/**
		* @see MHTTPFilter
		*/
		virtual TInt MHFRunError( TInt aError, 
								 RHTTPTransaction aTransaction,
								 const THTTPEvent& aEvent );

		/**
		* @see MHTTPFilter
		*/
		virtual void MHFUnload( RHTTPSession aSession, 
							   THTTPFilterHandle aHandle );

		/**
		* @see MHTTPFilter
		*/
		virtual void MHFLoad( RHTTPSession aSession, 
							 THTTPFilterHandle aHandle );

	private :	// construction
		/**
		* Standard constructor
		*/
		CCookieFilter( RHTTPSession aSession );

		/**
		* EPOC second-phase constructor
		*/
		void ConstructL();

	private :	// internal methods
		/**
		* Check the response headers of the specified transaction and extract
		* any cookies from them for storage.
		* @param aTransaction The transaction to handle the response headers.
		*/
		void HandleResponseHeadersL( RHTTPTransaction aTransaction );

		/**
		* Find any cookies which are appropriate to this request and add them
		* to the outgoing request headers.
		* @param aTransaction The transaction to handle the request headers.
		*/
		void HandleRequestHeadersL( RHTTPTransaction aTransaction );

        /**
        * @return ETrue if cookie usage enabled.
        */
        TBool IsCookieUsageEnabledL( RHTTPTransaction aTransaction );

    private:    // debug utilities

        /**
        * In debug build, log headers. In release build, do nothing.
        * @param aHeaders Headers to log.
        */
        void LogHeaders( RHTTPHeaders aHeaders );

	private :	// data members
		/**
		* Count of how many times this filter has been loaded - used to ensure 
		* destruction happens at the correct time
		*/
		TInt iLoadCount;

		/** String pool to be used by this cookie
		*/
		RStringPool iStringPool;

		/** The current http session
		*/
		RHTTPSession iSession;

		/** The manager which is used for storage and retrieval of cookies
		*/
		RCookieManager iCookieManager;

		/** The string table which is used for storage of all strings
		*/
		const TStringTable& iStringTable;

        /** The string that is used to check cookie usage transaction property.
        */
        RStringF iCookieUsage;
	};

#endif //__HTTPCOOKIE_FILTER_H__