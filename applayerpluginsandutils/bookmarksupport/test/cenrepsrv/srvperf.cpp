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

#include "srvperf.h"

#ifdef __CENTREP_SERVER_PERFTEST__

#include "srvreqs.h"

TUint32 TCentRepPerfTest::iEntries[KCentRepPerfTestArraySize];
TUint   TCentRepPerfTest::iCount;
TUint   TCentRepPerfTest::iActiveSessionCount;
TUint   TCentRepPerfTest::iLastCompleteAccess;
TUint   TCentRepPerfTest::iEndOfBootCount;

// SessionClose
// Decrement reference count. If no more active sessions
// then advance iLastCompleteAccess
void TCentRepPerfTest::SessionClose()
	{
	iActiveSessionCount--;
	// There are clients who never close their sessions. Hence
	// cannot use iActiveSessionCount == 0 as indication to copy
	// iCount to iLastCompleteAccess.

	if (!IsFinished())
		{
		iLastCompleteAccess = iCount;
		}
	}

// Append
// Add the integer if iEntries array is not full yet.
TInt TCentRepPerfTest::Append(TUint32 aEntry)
	{
	if (iCount < KCentRepPerfTestArraySize)
		{
		iEntries[iCount++] = aEntry;
		return KErrNone;
		}
	return KErrOverflow;
	}

#endif //__CENTREP_SERVER_PERFTEST__

