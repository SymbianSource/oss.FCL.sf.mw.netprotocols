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
* Description:  Additional common string tables
*
*/



// INCLUDE FILES
#include    "httpfiltercommonstringsaddition.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// Enter your strings here
_STLIT8(K1, "profile");
_STLIT8(K2, "x-wap-profile");



const void * const KMoreStringPointers[] =
    {
    (const void*)&K1,
    (const void*)&K2
    };


const TStringTable HttpFilterCommonStringsAddition::iTable =
    {
    sizeof(KMoreStringPointers) / sizeof(void*), KMoreStringPointers, EFalse
    };





// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HttpFilterCommonStringsAddition::GetTable
// Get the string table containing strings used by both filters and their clients.
// -----------------------------------------------------------------------------
//
EXPORT_C const TStringTable& HttpFilterCommonStringsAddition::GetTable()
    {
    return iTable;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
