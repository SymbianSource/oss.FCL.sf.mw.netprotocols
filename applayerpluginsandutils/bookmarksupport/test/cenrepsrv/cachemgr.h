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

#ifndef CACHEMGR_H
#define CACHEMGR_H

#include <e32base.h>
//#include <cinidata.h> //Commented and added the below line as part of TEF2 to TEF3 migration
#include <iniparser.h>
#include "log.h"
#include "panic.h"
#include "srvdefs.h"

_LIT(KCacheMgrIniFile, "centrep.ini");

class CSharedRepository;

/**
@internalTechnology
This is the class which manages Coarse-Grained cache operations including delayed-unloading of
repositories and forced eviction of repositories under OOM conditions.
*/
class CRepositoryCacheManager : public CTimer
	{
	friend class TRepositoryCacheManagerTester;
public:
	static CRepositoryCacheManager* NewLC();
	~CRepositoryCacheManager();
	void ConstructL();
	
	void EnableCache();
	void DisableCache();
	void EnableCache(TInt aDefaultTimeout, TInt aCacheSize);
	
	inline TBool Enabled();

	TBool StartEviction(CSharedRepository*& aRepository);
	
	void RemoveIdleRepository(CSharedRepository* aRepository);	
	void FlushCache();

protected:
	/**
	@internalTechnology
	This is the class/structure which keeps eviction-related cache data 
	*/
	class TRepositoryCacheInfo
		{
	public:
		TTime iCacheTime;
		CSharedRepository* iSharedRepository;
		};

	void RunL();
		
private:
	inline CRepositoryCacheManager(); 

	void Evict(TInt aIdleRepIndex);
	
	static TInt ForcedEvictionSortOrder(const TRepositoryCacheInfo &aRepository1, const TRepositoryCacheInfo &aRepository2);
	static TInt TimerEvictionSortOrder(const TRepositoryCacheInfo &aRepository1, const TRepositoryCacheInfo &aRepository2);	
	
private:
	TBool iEnabled; 
	TInt  iTotalCacheUsage; 
	RArray<TRepositoryCacheInfo> iIdleRepositories;
	TTimeIntervalMicroSeconds32 iDefaultTimeout;
	TInt  iCacheSize;
	};

#include "cachemgr.inl"

#endif // CACHEMGR_H
