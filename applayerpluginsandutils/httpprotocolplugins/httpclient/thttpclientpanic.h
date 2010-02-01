// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __THTTPCLIENTPANIC_H__
#define __THTTPCLIENTPANIC_H__

#include <e32std.h>

// @todo remove these if possible - affects headers filter and codec
_LIT8(KSpace, " ");
_LIT8(KFieldSeparator, "\n");

class THttpClientPanic
	{
public:
	
	enum THttpClientPanicCode
		{
		EInvalidHeaderValueType					= 0,
		EFullUrlRequired						= 1,
		EUriMissingAuthority					= 2,
		EInvalidHeaderForHTTP10					= 3,
		EInvalidProxySetting					= 4,
		ENoBodyPartInDataSupplier				= 5,
		EInvalidBatchingSetting					= 6
		};
		
	static void Panic(THttpClientPanicCode aPanic);
	
	};

#endif	// __THTTPCLIENTPANIC_H__

