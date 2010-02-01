/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*	  Deflate main file.
*
*
*/


#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "DeflateFilter.h"


const TImplementationProxy KImplementationTable[] = 
	{
#ifdef __EABI__
		{{0x101F9713}, (TFuncPtr)CDeflateFilter::InstallFilterL}
#else
		{{0x101F9713}, CDeflateFilter::InstallFilterL}
#endif
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(KImplementationTable) / sizeof(TImplementationProxy);
	return KImplementationTable;
	}
