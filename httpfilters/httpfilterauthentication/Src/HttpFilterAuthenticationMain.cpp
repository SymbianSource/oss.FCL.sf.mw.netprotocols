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



// INCLUDE FILES
#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "httpfilterauthenticationinterface.h"
#include "HttpFilterAuthentication.h"
#include "HttpFilterDigestAuthentication.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
// These constants are already defined in XXXInterface.h separatly
// but we need them here together.
// It's very rarely case but do not forget to change them
// here as well if you change them there.
const TInt KInstallDigestAuthenticationUIDValue = 0x101F85F0;
const TInt KInstallAuthenticationUIDValue = 0x101F8497;

const TImplementationProxy KImplementationTable[] =
    {
    {{KInstallAuthenticationUIDValue}, (TProxyNewLPtr) CHttpFilterAuthentication::InstallFilterL},
    {{KInstallDigestAuthenticationUIDValue}, (TProxyNewLPtr) CHttpFilterDigestAuthentication::InstallFilterL}
    };

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// Returns the filters implemented in this DLL
// Returns: The filters implemented in this DLL
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( KImplementationTable ) / sizeof( TImplementationProxy );
    return KImplementationTable;
    }


//  End of File
