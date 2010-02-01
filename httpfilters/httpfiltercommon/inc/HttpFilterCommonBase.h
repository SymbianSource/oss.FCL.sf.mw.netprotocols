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



#ifndef HTTPFILTERCOMMONBASE_H
#define HTTPFILTERCOMMONBASE_H

//  INCLUDES
#include <http/mhttpfilter.h>


// CONSTANTS

// MACROS

// DATA TYPES
typedef struct
    {
    RHTTPSession* iSession;
    TBool iIsProxy;
    }
TAuthenticationParams;


// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Base class for Authentication Filter. Contains offset to the UID.
*
*  @lib cannot link with Ecom plugins
*  @since 2.0
*/
class CHttpFilterCommonBase : public CBase, public MHTTPFilter
    {
    public:  // New functions

        /**
        * Install the filter.
        * @since 2.0
        * @param aFilterParams The initialization parameters for the Filter
        * @return ?description
        */
        static CHttpFilterCommonBase* InstallFilterL(TAny* aFilterParams);

    public:  // New functions

        /**
        * Reserved. 
        * @since 2.0
        * @param 
        * @return reserved
        */
        virtual TInt Reserved() = 0;

    public:      // Data

        // The Ecom interface ID
        TUid iEcomDtorID;
    };

#endif      // HTTPFILTERCOMMONBASE_H

// End of File
