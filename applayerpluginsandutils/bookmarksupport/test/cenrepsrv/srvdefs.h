// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef SRVDEFS_H
#define SRVDEFS_H

#include <e32def.h>

const TUint32 KMetaBackup		= 0x01000000;
const TUint32 KMetaRfsValue		= 0x02000000; // Restore Factory Settings
const TUint32 KMetaDefaultValue = 0x80000000;
const TUint32 KMetaType			= 0x70000000;

#ifndef DEFAULT_CENTREP_CACHE_SIZE
	/** 
	The default size of the coarse-grained cache, used when there is no .ini setting defined.
	The default value is approximately the size needed to cache everything during device boot-up
	@internalComponent
	*/
	#define DEFAULT_CENTREP_CACHE_SIZE 100000
#else
	#if DEFAULT_CENTREP_CACHE_SIZE <= 0
		#error "DEFAULT_CENTREP_CACHE_SIZE macro value must be greater than 0"
	#endif
#endif

/** 
The default size of the coarse-grained cache. 
@internalComponent
*/
const TInt KDefaultCacheSize=DEFAULT_CENTREP_CACHE_SIZE;

/** 
The special cache size value used for enabling the cache without changing the current cache size value.
@internalComponent
*/
const TInt KInvalidCacheSize	= -1;

#ifndef DEFAULT_CENTREP_CACHE_EVICTION_TIMEOUT
	/** 
	The default timeout value of the coarse-grained cache, used when there is no .ini setting defined.
	The default value is approximately the timeout needed to keep everything in cache during device boot-up
	@internalComponent
	*/
	#define DEFAULT_CENTREP_CACHE_EVICTION_TIMEOUT 5050000
#else
	#if DEFAULT_CENTREP_CACHE_EVICTION_TIMEOUT <= 0
		#error "DEFAULT_CENTREP_CACHE_EVICTION_TIMEOUT macro value must be greater than 0"
	#endif
#endif

/** 
The default timeout value (in microseconds) for the coarse-grained cache repository eviction. 
@internalComponent
*/
const TInt KDefaultEvictionTimeout=DEFAULT_CENTREP_CACHE_EVICTION_TIMEOUT;

/** 
The constant used in forced evicition sorting algorithm for converting microsecond-based timeout 
values to an intermediary unit compatible with byte-based size values.
@internalComponent
*/
const TInt KTimeoutToSizeConversion=DEFAULT_CENTREP_CACHE_EVICTION_TIMEOUT/100000;

/** 
The special timeout value used for enabling the cache without changing the current timeout value.
@internalComponent
*/
const TInt KInvalidEvictionTimeout=-1;

#endif // SRVDEFS_H
