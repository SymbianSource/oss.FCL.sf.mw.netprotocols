// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//


#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "redirect.h"
#include "ValidationFilter.h"

#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}
#endif // EKA2

const TImplementationProxy KImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x101F446F, CRedirectFilter::InstallFilterL),
		IMPLEMENTATION_PROXY_ENTRY(0x101F447B, CValidationFilter::InstallFilterL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(KImplementationTable) / sizeof(TImplementationProxy);
	return KImplementationTable;
	}
