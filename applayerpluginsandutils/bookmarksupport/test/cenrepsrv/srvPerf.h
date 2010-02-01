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

#ifndef SRVPERF_H
#define SRVPERF_H

#include <e32base.h>

#ifdef __CENTREP_SERVER_PERFTEST__
#include "srvreqs.h"

enum TPseudoRequest
	{
	ESessionClose = EGetSetParameters+1,
	EEvict = EGetSetParameters+2
	};

const TInt KEventIdShiftBits = 24;
const TUint32 KEventIdMask = 0xFF;
const TUint32 KDataMask = 0x00FFFFFF;

// Only record CPU used by CentRep server.
const TUint32 KCentRepPerfTestArraySize = 3600;

// class TCentRepPerfTest manages the performance data
// collected by CentRep server.
class TCentRepPerfTest
	{
public:
	inline void SessionOpen()
		{ iActiveSessionCount++; }
	inline TBool IsFinished() const // Special assumed end-of-boot condition 
		{ return (iCount >= KCentRepPerfTestArraySize)||(iEndOfBootCount==9); }
	inline TUint LastCompleteAccess() const
		{ return iLastCompleteAccess; }
	inline const TUint32* Entries() const
		{ return iEntries; }
	inline void IncEndOfBoot()
		{ iEndOfBootCount++; }
	inline void Reset()
		{
		iActiveSessionCount = 0;
		iLastCompleteAccess = 0;
		iEndOfBootCount = 0;
		iCount = 0;
		}
	inline void Stop()
		{
		iCount = KCentRepPerfTestArraySize;
		}

	void SessionClose();
	TInt Append(TUint32 aEntry);

private:
	static TUint32 iEntries[KCentRepPerfTestArraySize];
	static TUint   iCount;
	static TUint   iActiveSessionCount;
	static TUint   iLastCompleteAccess;
	static TUint   iEndOfBootCount;
	};

#endif //__CENTREP_SERVER_PERFTEST__

#ifdef __CENTREP_SERVER_MEMTEST__
	enum TMemTestLocationIdentifier
		{
		EMemLcnRepositoryOpen //used to measure heap usage for a repository when
							  //called from CServerRepository::OpenL
		};
	const TInt32 KMemBufMaxEntry = 150; //make sure to allocate for 2 more than required.
									    //The additional 2 are used to mark when server
									    //tries to save more entries then max allowed
	const TUint32 KMagicMemTestOutOfBounds = 0xDEADBEEF;
	#define RECORD_HEAP_SIZE(aLocation, aIdentifier) TServerResources::RecordTimerResult(aLocation, aIdentifier)
#else //__CENTREP_SERVER_MEMTEST__
	#define RECORD_HEAP_SIZE(aLocation, aIdentifier)
#endif //__CENTREP_SERVER_MEMTEST__

#endif //SRVPERF_H
