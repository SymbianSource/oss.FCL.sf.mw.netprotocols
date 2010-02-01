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

#ifndef __UAPROFPANIC_H__
#define __UAPROFPANIC_H__

// System Includes
#include <e32std.h>


class UAProfPanic
	{
public:
	enum TUAProfPanic
		{
		EInvalidEventReceived
		};
	inline static void Panic(TUAProfPanic aPanic);
	};
	
inline void UAProfPanic::Panic(UAProfPanic::TUAProfPanic aPanic)
	{
	_LIT(KUAProfPanic, "UAProf");
	User::Panic(KUAProfPanic, aPanic);
	}
	
#endif // __UAPROFPANIC_H__