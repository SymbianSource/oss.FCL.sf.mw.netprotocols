/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Authentication filter and cache filter.
*
*/



#ifndef HTTPFILTERAUTHENTICATION_H
#define HTTPFILTERAUTHENTICATION_H

//  INCLUDES
#include <http/mhttpfilter.h>
#include <httpstringconstants.h>
#include "httpfilterauthenticationinterface.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Authentication Filter implementation class.
*
*  @lib cannot link with Ecom plugins
*  @since 2.0
*/

class CHttpFilterAuthentication : public CHttpFilterAuthenticationInterface, public MHTTPFilter
    {

    public:     // Constructors and destructor

        /**
        * Two-phased constructor. This function replaces NewL
        */
        static CHttpFilterAuthentication* InstallFilterL( TAny* aAuthenticationParams );

        /**
        * Destructor.
        */
        virtual ~CHttpFilterAuthentication();

    public:    // Functions from base classes


        /**
        * From MHTTPFilterBase Process a transaction event.
        * @since 2.0
        * @param aTransaction The transaction for which an event occured
        * @param aEvent The event to be processed
        * @return void
        */
        virtual void MHFRunL( RHTTPTransaction aTransaction, const THTTPEvent& aEvent );

        /**
        * From MHTTPFilterBase Process a session event.
        * @since 2.0
        * @param aEvent The event to be processed
        * @return void
        */
        virtual void MHFSessionRunL( const THTTPSessionEvent& aEvent );

        /**
        * From MHTTPFilterBase Process an error that occured while processing the transaction.
        * @since 2.0
        * @param aError The error that occured
        * @param aTransaction The transaction for which the error occured
        * @param aEvent The event during which the error occured
        * @return KErrNone
        */
        virtual TInt MHFRunError( TInt aError,
                                  RHTTPTransaction aTransaction,
                                  const THTTPEvent& aEvent );

        /**
        * From MHTTPFilterBase Called when MHFRunL leaves from a session event.
        * @since 2.0
        * @param aError The error that occured
        * @param aEvent The event during which the error occured
        * @return KErrNone
        */
        virtual TInt MHFSessionRunError( TInt aError, const THTTPSessionEvent& aEvent );

        /**
        * From MHTTPFilter Called when the filter is being removed from a session's filter queue.
        * @since 2.0
        * @param aSession The session it's being removed from
        * @param aHandle The filter handle.
        * @return void
        */
        virtual void MHFUnload( RHTTPSession aSession, THTTPFilterHandle aHandle );

        /**
        * From MHTTPFilter Called when the filter is being added to the session's filter queue.
        * @since 2.0
        * @param aSession The session it's being added to.
        * @param aHandle The filter handle.
        * @return void
        */
        virtual void MHFLoad( RHTTPSession aSession, THTTPFilterHandle aHandle );

        /**
        * Reserved for future extensions
        * @since 2.0
        * @param 
        * @return 0
        */
        virtual TInt Reserved()
            {
            return 0;
            }

    protected:

        /**
        * C++ default constructor.
        */
        CHttpFilterAuthentication( RHTTPSession* aSession, TBool aIsProxy );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( RHTTPSession aSession );

        /**
        * Function to handle Submit events. We need to remove any
        * username and password from the URI.
        */
        void SubmitL( RHTTPTransaction aTransaction );

        /**
        * Provides a central point for all Tranaction handling.
        */
        void HandleTransactionL( RHTTPTransaction aTransaction );

        /**
        * Search list for matching credentials.
        */
        TInt FindCredentials( const RString& aRealm, const TUriC8& aURI ) ;

        /**
        * Searches list for credentials that can be used with this URI
        * without knowing the realm.
        */
        TInt FindCredentialsForURI( const TUriC8& aURI ) ;

        /**
        * Search list for matching credentials for a given realm.
        */
        TInt FindCredentialsForRealm( const RString& aRealm, TBool aCheckProxy = ETrue ) ;

        /**
        * Search list for matching credentials based on proxy used.
        */
        TInt FindCredentialsForProxy();

        /**
        * Find the Authentication header.
        */
        TInt FindHeaderPartToUseL( RHTTPTransaction aTransaction ) ;

        /**
        * Set basic auth header
        */
        void EncodeBasicAuthL( const RString& aUsername,
                               const RString& aPW,
                               RHTTPTransaction& aTransaction );

        /**
        * Find credentials from the transaction's URI.
        */
        void GetCredentialsFromURIL( RHTTPTransaction& aTransaction );

        /**
        * Find credentials from the transaction's properties.
        */
        void GetCredentialsFromPropertiesL( RHTTPTransaction& aTransaction );

        /**
        * Remove credentials from the list.
        */
        void RemoveCredentialsFromList( TInt aCredId );

        /**
        * Store credentials in the list.
        */
        void AddCredentialsToListL( RString aUsernameStr,
                                    RString aPasswordStr,
                                    RString aRealmStr,
                                    RStringF aUriStr );

        /**
        * Remove white spaces from the left.
        */
        void TrimLWSLeft( TPtrC8& aPtr );

        /**
        * Remove white spaces from the right.
        */
        void TrimLWSRight( TPtrC8& aPtr );

        /**
        * Find a character in a buffer, but not in a quoted string.
        */
        TInt FindCharNotInQoutes( TPtrC8& aPtr, TUint8 aCharToFind );

        /**
        * Find a substring in a buffer, but not in a quoted string.
        */
        TInt FindCharNotInQoutes( TPtrC8& aPtr, const TPtrC8& aCharsToFind );

        /**
        * Find the next token in the header.
        */
        TInt FindNextTokenInHeaderFieldL( TPtrC8& aHeaderPtr, TPtrC8& aTokenPtr );

        /**
        * Find the proxy realm in the response header and add it to the transaction's properties.
        */
        void AddProxyRealmToTransactionL( RHTTPTransaction aTransaction );

        /**
        * Get proxy name from the HTTP session.
        */
        TBool GetProxyName( RStringF &aProxyName );

        /**
        * Set property of the transaction
        */
        void SetPropertyL( RHTTPTransaction& aTransaction,
                                             RStringF aPropertyName, 
                                             const TDesC8& aToken );


        /**
        * Sets property Basic that indicates to engine that
        * the current authentication is Basic.
        */
        void SetBasicProperty( RHTTPTransaction aTransaction );

    protected:       // Data
        // Container for holding an internal storage of credentials

        class TCredentials
            {

            public:
                RStringToken iRealm; ///< Realm used - there may be multiple for each host
                RStringToken iUser;  ///< username
                RStringToken iPassword; ///< password
                RStringTokenF iURI;  ///< uri
                TInt iID;    ///< to associate credentials with transactions
            };

        // Internal list of valid credentials
        RArray<TCredentials> iCredentials;

        // The string pool to use
        RStringPool iStringPool;

        // Counter for how amny times the filter was loaded
        TInt iLoadCount;

        // The next free credential ID
        TInt iNextID;

        // The HTTP Session for which the filter was loaded
        RHTTPSession* iSession;

        // Flag if this Filter is used for proxy authentication or www authentication.
        TBool iIsProxy;

        // The following data members are initialized to the corresponding string
        // depending on the useage of the filter, for proxy authentication or
        // www authentication
        HTTP::TStrings iHttpStringAuthorization;
        HTTP::TStrings iHttpStringAuthenticate;
        TInt iStatusCode;
        RStringF iRealmStr;
        RStringF iUsernameStr;
        RStringF iPasswordStr;

    };

#endif      // HTTPFILTERAUTHENTICATION_H

// End of File
