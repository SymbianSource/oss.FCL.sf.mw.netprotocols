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

#ifndef		__COREFILTERSPANIC_H__
#define		__COREFILTERSPANIC_H__

#include <e32std.h>

class TFCoreFiltersPanic
	{
public:

	enum TPanic
		{
		EInvalidRedirectCountProperty,
		EInvalidMaxRedirectProperty,
		EValidationFilterFailure
		};


	static void Panic(TInt aPanicCode);
	};


#endif //	__COREFILTERSPANIC_H__
