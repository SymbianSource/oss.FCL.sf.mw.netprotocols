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
* Description:  Digest Authentication filter.
*
*/



#ifndef HTTPFILTERDIGESTAUTHENTICATION_H
#define HTTPFILTERDIGESTAUTHENTICATION_H

//  INCLUDES
#include "HttpFilterAuthentication.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CMD5;

/**
*  Digest Authentication Filter implementation class.
*
*  @lib cannot link with Ecom plugins
*  @since 2.0
*/

class CHttpFilterDigestAuthentication : public CHttpFilterAuthentication
    {
    enum
        {
        KAlgUnknown,
        KAlgMd5,
        KAlgMd5Sess
        };

    enum
        {
        KQopNone,
        KQopAuth = 1,
        KQopAuthInt = 2
        };

    public:     // Constructors and destructor

        /**
        * Two-phased constructor. This function replaces NewL
        */
        static CHttpFilterDigestAuthentication* InstallFilterL( TAny* aAuthenticationParams );

        /**
        * Destructor.
        */
        virtual ~CHttpFilterDigestAuthentication();

    public:    // Functions from base classes


        /**
        * From MHTTPFilterBase Process a transaction event.
        * @since 2.0
        * @param aTransaction The transaction for which an event occured
        * @param aEvent The event to be processed
        * @return void
        */
        virtual void MHFRunL( RHTTPTransaction aTransaction, const THTTPEvent& aEvent );

    protected:

        /**
        * C++ default constructor.
        */
        CHttpFilterDigestAuthentication( RHTTPSession* aSession, TBool aIsProxy );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( RHTTPSession aSession );

        /**
        * Parse "Authentication-Info" field in response header.
        */
        void ParseAuthenticationInfoL( RHTTPTransaction aTransaction );

        /**
        * Provides a central point for all Tranaction handling.
        */
        void DHandleTransactionL( RHTTPTransaction aTransaction );

        /**
        * Function to handle Submit events. We need to remove any
        * username and password from the URI.
        */
        void DSubmitL( RHTTPTransaction aTransaction );

        /**
        * Search list for matching credentials.
        */
        TInt DFindCredentials( const RString& aRealm, const TUriC8& aURI ) ;

        /**
        * Searches list for credentials that can be used with this URI
        * without knowing the realm.
        */
        TInt DFindCredentialsForURI( const TUriC8& aURI ) ;

        /**
        * Find the Authentication header.
        */
        TInt DFindHeaderPartToUseL( RHTTPTransaction aTransaction ) ;

        /**
        * Find credentials from the transaction's properties.
        */
        void DGetCredentialsFromPropertiesL( RHTTPTransaction& aTransaction );

        /**
        * Remove credentials from the list.
        */
        void DRemoveCredentialsFromList( TInt aCredId );

        /**
        * Add new credential to list
        */
        void DAddCredentialsToListL( RString aUsernameStr,
                                     RString aPasswordStr,
                                     RString aRealmStr,
                                     RStringF aUriStr,
                                     RString aOpaque,
                                     RString aNonce,
                                     TInt aQop,
                                     TInt aAuthAlg);

        /**
        * Encodes digest authentication
        */
        void EncodeDigestAuthL( TInt aCred, RHTTPTransaction aTransaction );

        /**
	    * Calculate H(A1). See section 3.2.2.2
        */
	    TInt HA1L( int aAlgorithm,
                   const RString& aUsername,
                   const RString& aPW,
			       const RString& aRealm,
                   const RString& aNonceValue,
                   TDesC8& aCNonce,
                   TDes8& aResult);

	    
	    /**
	    * Calculate H(A2). See section 3.2.2.3
        */
	    TInt HA2L(const RStringF& aMethod, const RString& aRequestUri, TDes8& aResult);

	    /**
        * Calculate a hash and in the form of a 32 character hex result.
        */
	    void Hash(const TDesC8& aMessage, TDes8& aHash);

        /**
        * Generate a cnonce value
        */
	    void GenerateCNonce( TDes8& aNonce );

        /**
        * Returns the requested uri 
        * that can be used in uri field or request header
        *
        * @param aTransaction current transaction
        * @return requested URI
        */
        RString RequestUriL( RHTTPTransaction& aTransaction );
                       
        /**
        * Check and return QoP from the response header.
        * @param aHeader authentication header
        * @param aWwwAuthHeader authentication header name
        * @param aHeaderPart which authentication header field in the response header
        * @return one of KQopXXX enum. KQopAuth is prefered.
        */    
        TInt CheckQop( RHTTPHeaders& headers,
                       RStringF& aWwwAuthHeader,
                       TInt aHeaderPart );


    private:       // Data
        // Container for holding an internal storage of credentials

        class TDCredentials
            {
            public:
                RStringToken iUser;  ///< username
                RStringToken iPassword; ///< password
                RStringTokenF iURI;  ///< uri
                RStringToken iRealm; ///< Realm used - there may be multiple for each host
                RStringToken iOpaque;
                RStringToken iNonce;
                TInt iQop;
                TInt iAlgorithm;
                TInt iNc;   // nonce-count
            };

        // Internal list of valid credentials
        RArray<TDCredentials> iDCredentials;

        RStringF    iOpaqueStr;
        RStringF    iNonceStr;
        RStringF    iQopStr;
        RStringF    iStaleStr;
        RStringF    iAuthInfo;
        RStringF    iMd5Str;
        RStringF    iMd5SessStr;
        RStringF    iQopAuthStr;
        TInt64      iSeed;
        CMD5*       iMD5Calculator;
    };

#endif      // HTTPFILTERDIGESTAUTHENTICATION_H

// End of File
