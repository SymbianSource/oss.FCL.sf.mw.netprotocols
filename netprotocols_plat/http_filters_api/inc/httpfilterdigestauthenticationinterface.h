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
* Description:  Authetication Filter 
*
*/


#ifndef CHTTPFILTERAUTHENTICATIONINTERFACE_H
#define CHTTPFILTERAUTHENTICATIONINTERFACE_H

// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <http/rhttpsession.h>

/**
 @public
 @class   CHttpFilterAuthenticationInterface
 @since   2.0
 @dependencies CBase, REComSession, RHTTPSession
 @lib   euser.lib ecom.lib
 The ECOM interface definition for an HTTP authentication filter.  Implementations
 of a authentication filter must inherit this class.
 */

// CONSTANTS
const TInt KDigestAuthenticationUIDValue = 0x101F85F0;
const TUid KUidDigestAuthenticationFilter = {KDigestAuthenticationUIDValue};

typedef struct
    {
    RHTTPSession* iSession;
    TBool iIsProxy;
    }
TAuthenticationParams;

class CHttpFilterAuthenticationInterface : public CBase
    {

    public:   // Methods
        /**
        @fn    InstallFilterL(RHTTPSession& aSession)
        Intended Usage: This method is used to install the authentication filter to the filter queue.
        @since   2.0
        @param   aSession A handle to the transport session
        @pre    The session had already been setup
        @post   The filter(s) have been installed
        */
        inline static void InstallFilterL( RHTTPSession& aSession , TBool aIsProxy );

        /**
        @fn    ~CHttpAuthenticationFilterInterface()
        Intended Usage: Virtual destructor
        @since   2.0
        @pre    The object identified by the destructor key in iEcomDtorID exists
        @post   The object is destroyed
        */
        inline ~CHttpFilterAuthenticationInterface();

    public:   // Attributes
        /// The ECom destructor key identifier
        TUid iEcomDtorID;
    };

//----------------------------------------------------------------------------------------

inline void CHttpFilterAuthenticationInterface::InstallFilterL( RHTTPSession& aSession, TBool aIsProxy )
    {
    TAuthenticationParams authParams;
    authParams.iSession = &aSession;
    authParams.iIsProxy = aIsProxy;
    REComSession::CreateImplementationL( KUidDigestAuthenticationFilter,
                                         _FOFF( CHttpFilterAuthenticationInterface, iEcomDtorID ), &authParams );
    }

inline CHttpFilterAuthenticationInterface::~CHttpFilterAuthenticationInterface()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }

#endif // CHTTPFILTERAUTHENTICATIONINTERFACE_H
