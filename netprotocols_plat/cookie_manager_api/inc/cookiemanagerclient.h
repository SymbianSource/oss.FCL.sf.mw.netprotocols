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
* Description:  Declaration of class RCookieManager
*
*
*/



#ifndef __COOKIEMANAGER_CLIENT_H__
#define __COOKIEMANAGER_CLIENT_H__

// INCLUDE FILES
	// System includes
#include <e32base.h>
#include <http.h>

	// User includes
#include <cookie.h>
#include <cookieipc.h>

//FORWARD Class Declaration
class CCookieClientDataArray;
// CLASS DECLARATION
	
/** 
* It is the client interface to the Cookie Server.
*/
class RCookieManager : public RSessionBase
	{
	public :	// exported methods
		/**
		*
		*/
		IMPORT_C RCookieManager( RStringPool aStringPool );

		/**
		*
		*/
		IMPORT_C TInt Connect();

		/** 
		* Store a cookie. This will get called with each cookie that is
		* encountered in a transaction response.
		* @param aCookie The cookie to be stored. Ownership of aCookie is
		* passed to this class.
		* @param aUri The uri that sent this cookie. This is required to 
		* complete any missing attributes of the cookie and to ensure the
		* cookie is valid.
		* @return Error code indicating the result of the call.
		*/
		IMPORT_C TInt StoreCookie( const CCookie& aCookie,
									const TUriC8& aUri );

		/** 
		* Get the relevant cookies for a transaction
		* @param aUri The URI of the current HTTP request.
		* @param aCookieList An array which will be filled with the cookies
		* for aTransaction. The array should be empty when this function is
		* called. Ownership of the cookies remains with the manager.
		* @param aCookie2Reqd Flag indicating whether a Cookie2 header should
		* be added to the outgoing request headers
		*/
		IMPORT_C void GetCookiesL( const TDesC8& aUri,
								   RPointerArray<CCookie>& aCookies, 
								   TBool& aCookie2Reqd );

		/**
		* Clear all cookies.
		* @param aDeletedCookies On return, it contains the number of deleted
		* cookies.
		* @return Error code indicating the result of the call.
		*/
		IMPORT_C TInt ClearCookies( TInt& aDeletedCookies );
		
		/**Sets the AppUidL
		* @param aAppUid the application UID.
		* @return Error code indicating the result of the call.
		*/		
		IMPORT_C TInt SetAppUidL(const TUint32& aAppUid );
        IMPORT_C void Close();
        IMPORT_C TInt StoreCookie( const CCookie& aCookie,
                                            const TUriC8& aUri,TUint32& aAppUid);
        IMPORT_C void GetCookiesL( const TDesC8& aUri,
                                   RPointerArray<CCookie>& aCookies, 
                                   TBool& aCookie2Reqd,TUint32& aAppUid );
        IMPORT_C TInt ClearAllAppUidCookies(const TUint32& aAppUid);


	private :	// internal methods
		/**
		* Gets the sum of cookie sizes for a given request URI. After this
		* method call there will be available the set of cookies on server-side
		* that have been collected by the URI in question.
		* @param aRequestUri The HTTP transaction request's URI,
		* @param aSizePkg It is an integer that will contain the overall size
		* of all cookies that have been selected by the given URI after the
		* method returns. It is a package : it is prepared for client-server
		* communications.
		* @return Error code indicating the result of the call.
		*/
		TInt DoGetCookieSize( const TDesC8& aRequestUri,
							TPckg<TInt>& aPkgSize ) const;
							
	    TInt DoGetCookieSize( const TDesC8& aRequestUri,
							TPckg<TInt>& aPkgSize,TDesC& aAppUidPtr ) const;

		/**
		* Puts those cookies in the buffer that have been previously selected
		* on server-side as a result of the GetCookieSize method.
		* @param aBuffer Buffer to be filled with appropriate cookies.
		* @return Error code indicating the result of the call.
		*/
		TInt DoGetCookies( TDes8& aBuffer ) const;

		/**
		*
		*/
		TInt DoStoreCookie( const TDesC8& aPackedCookie,
							const TDesC8& aUri,TDesC& aAppUidPtr ) const;

		/**
		*
		*/
		TVersion Version() const;
		
		TInt DestroyCookiesFromMemory( TInt& aDeleteStatus );
		
		void StoreCookieAtClientSide( const CCookie* aCookie, const TDesC8& aUri,TUint32 aWidgetUid =0);
		
		TInt GetClientSideCookies( const TDesC8& aRequestUri,RPointerArray<CCookie>& aCookies
		        ,TBool& aFound, TUint32 aWidgetUid );
		TInt GetCookieSharableFlagFromServer(TBool& aCookieSharableFlag ) const;
		
	private :	// data members
	    //internal data structure for supporting Client side caching.
        class TCookieMgrInternalStruct
               {
               public:
               /* Constructor
                * 
                */    
               TCookieMgrInternalStruct(RStringPool aStringPool)
               : iCookiePacker(aStringPool),
               iCookieClientDataArray(NULL)
               {          
               }
               
               /* Destructor
                * 
                */
               ~TCookieMgrInternalStruct();
               
               /* Get Cookiepacker instance
                * 
                */
               inline TCookiePacker& GetCookiePacker(){return iCookiePacker;}
               
               /* Get Client Data Array Instance
                * 
                */
               inline CCookieClientDataArray* GetCookieClientDataArray(){return iCookieClientDataArray;}
               
               /* Initialization method for Cookie Client Data Array
                * 
                */
               TInt Init();
               
               private:
               TCookiePacker iCookiePacker;
               CCookieClientDataArray* iCookieClientDataArray;
               };

     RStringPool iStringPool;
     TCookieMgrInternalStruct* iCookieMgrData;  
		
	};

#endif //__COOKIEMANAGER_CLIENT_H__
