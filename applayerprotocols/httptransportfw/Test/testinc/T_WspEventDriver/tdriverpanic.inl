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

#ifndef __TDRIVERPANIC_INL__
#define __TDRIVERPANIC_INL__


void TDriverPanic::Panic(TDriverPanicCode aPanic)
	{
	User::Panic(KDriverPanicType(), aPanic);
	}


#endif // __TDRIVERPANIC_INL__