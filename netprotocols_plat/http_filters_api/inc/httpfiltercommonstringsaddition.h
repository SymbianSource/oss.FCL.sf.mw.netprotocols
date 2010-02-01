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
* Description:  Additional common string tables for CCC
*
*/



#ifndef HTTPFILTERCOMMONSTRINGSADDITION_H
#define HTTPFILTERCOMMONSTRINGSADDITION_H

//  INCLUDES
#include <stringpool.h>
#include <stringtablesupport.h>
#include <httpstringconstants.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Additional string tables used by both the filters and their clients
*
*  @lib httpcommon.lib
*  @since 2.0
*/
class HttpFilterCommonStringsAddition
    {
    public:   // Constructors and destructor


    public:  // New functions

        /**
        * Get the string table containing strings used by both filters and their clients.
        * @since 2.0
        * @param 
        * @return The string table
        */
        IMPORT_C static const TStringTable& GetTable();

    public:      // Data
        // Enumeration of the strings in the string table
        enum TStrings
        {
            EProfile,
            EXWapProfile
        };

        static const TStringTable iTable;
    };

#endif      // HTTPFILTERCOMMONSTRINGSADDITION_H

// End of File
