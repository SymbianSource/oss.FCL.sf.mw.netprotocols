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
* Description:  Implementation of the DLL Entry Point.   
*      
*
*/


// INCLUDE FILES

#include <s32std.h>
#include <ecom/implementationproxy.h>
#include "uaproffilter.h"

/**
* Implementation proxy data for this dll (instantiation method per UID).
*/
const TImplementationProxy KImplementationTable[] = 
    {
        { { 0x10200A8A }, (TProxyNewLPtr) CHttpUAProfFilter::InstallFilterL }
    };

/**
* Get implementation proxy data (exported ECOM dll interface).
* @param aTableCount Table row count is returned here.
* @return Implementation proxy data.
*/
EXPORT_C const TImplementationProxy* ImplementationGroupProxy
    ( TInt& aTableCount )
    {
    aTableCount = sizeof(KImplementationTable) / sizeof(TImplementationProxy);
    return KImplementationTable;
    }
