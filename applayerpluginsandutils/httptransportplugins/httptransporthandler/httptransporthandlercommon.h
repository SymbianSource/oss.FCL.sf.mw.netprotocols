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

#ifndef __HTTPTRANSPORTHANDLERCOMMON_H__
#define __HTTPTRANSPORTHANDLERCOMMON_H__

#include <e32std.h>

const TInt KIpv6MaxAddrSize		= 39;

/**	A define that causes a leave to occur in OOM testing.
*/
#ifdef _DEBUG
	#define __OOM_LEAVE_TEST		{ HBufC* _a=HBufC::NewL(4);delete _a; }
#else
	#define __OOM_LEAVE_TEST
#endif

#endif	// __HTTPTRANSPORTHANDLERCOMMON_H__
