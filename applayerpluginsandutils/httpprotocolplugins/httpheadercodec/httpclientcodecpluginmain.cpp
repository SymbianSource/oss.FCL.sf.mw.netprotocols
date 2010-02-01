// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "chttpclientheadercodec.h"
#include "chttpgeneralheadercodec.h"
#include "chttpdefaultheadercodec.h"
#include "chttpserverheadercodec.h"

const TInt KUidHttpClientHeaderCodec  = 0x10272E5D;
const TInt KUidHttpGeneralHeaderCodec = 0x10272E5E;
const TInt KUidHttpDefaultHeaderCodec = 0x10272E5F;
const TInt KUidHttpServerHeaderCodec  = 0x10272E62;

// Map the interface UIDs to implementation factory functions
const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(KUidHttpClientHeaderCodec,	CHttpClientHeaderCodec::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KUidHttpGeneralHeaderCodec,	CHttpGeneralHeaderCodec::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KUidHttpDefaultHeaderCodec,	CHttpDefaultHeaderCodec::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KUidHttpServerHeaderCodec,	CHttpServerHeaderCodec::NewL)
	};

// Exported proxy for instantiation method resolution
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
	
#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}
#endif // EKA2
