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
* Description:  Declaration of class CHttpUAProfFilterInterface.
*
*
*/


#ifndef HTTP_UAPROF_FILTER_INTERFACE_H
#define HTTP_UAPROF_FILTER_INTERFACE_H

// INCLUDES

#include <e32base.h>
#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <ecom/ecom.h>

// CONSTANTS

/**
* UserAgentClientId property name.
* Set UserAgentClientId transaction property to get customized User-Agent
* header (values from SysUtil.h). If transaction property is unset or is not a
* recognized value, default KUAClientBrowser is used.
*/
_LIT8( KUserAgentClientId, "UserAgentClientId" );

//The value belonging to KUserAgentClientId inidcating that MMS is using the filter
const TInt KUserAgentCliMMS = 1;


// CLASS DECLARATION

/**
* HTTP User Agent Profile Filter interface.
*/
class CHttpUAProfFilterInterface : public CBase
    {
    public: // Constructors and destructor

        /*
        * Install filter on session.
        * @param aSession Install on this session.
        */
        inline static void InstallFilterL( RHTTPSession& aSession );

        /*
        * Destructor.
        */
        inline virtual ~CHttpUAProfFilterInterface();

    protected:  // Constructors and destructor

        /*
        * Default constructor.
        */
        inline CHttpUAProfFilterInterface();

    private:    // Constructors and destructor

        /*
        * Copy constructor - unimplemented, forbidden.
        */
        CHttpUAProfFilterInterface
            ( const CHttpUAProfFilterInterface& aUAProfFilterInterface );

        /*
        * Assignment operator - unimplemented, forbidden.
        */
        CHttpUAProfFilterInterface& operator=
            ( const CHttpUAProfFilterInterface& aUAProfFilterInterface );

    private:    // data

        /*
        * The ECom destructor key identifier.
        */
        TUid iEcomDtorID;
    };

#include "uaproffilter_interface.inl"

#endif  // HTTP_UAPROF_FILTER_INTERFACE_H
