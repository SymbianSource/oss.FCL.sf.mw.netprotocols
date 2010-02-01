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

#include "srvrepos.h"
#include "cachemgr.h"

_LIT(KCacheLit, "CoarseGrainedCache");
_LIT(KDefaultCacheSizeLit, "size");
_LIT(KDefaultEvictionTimeoutLit, "timeout");
    
CRepositoryCacheManager* CRepositoryCacheManager::NewLC()
	{
	CRepositoryCacheManager* self = new(ELeave) CRepositoryCacheManager;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CRepositoryCacheManager::~CRepositoryCacheManager()
	{
	DisableCache();
	}

void CRepositoryCacheManager::ConstructL()
	{
	CTimer::ConstructL();
		
	CIniData* iniData = NULL;
	TInt res = KErrNone;
	TBuf<KMaxFileName> iniFile;
	
	iniFile.Copy( *TServerResources::iInstallDirectory );
	iniFile.Append( KCacheMgrIniFile );	
	TRAP(res, iniData = CIniData::NewL(iniFile));
	if(res==KErrNotFound)
		{
		// if RomDirectory exists
		if (TServerResources::iRomDirectory)
			{
			iniFile.Copy( *TServerResources::iRomDirectory );
			iniFile.Append( KCacheMgrIniFile );	
			TRAP(res, iniData = CIniData::NewL(iniFile));
			}
		if(res==KErrNotFound)
			{
			__CENTREP_TRACE1("CENTREP: Central Repository Cache Parameters ini file %S not found. Default values will be used.", &KCacheMgrIniFile);
			return;
			}
		}
	if (res != KErrNone)
		{
		User::Leave(res);
		}
		
	CleanupStack::PushL(iniData);
	
	// process the file here
	if(!iniData->FindVar(KCacheLit(),KDefaultCacheSizeLit(), iCacheSize))
		{
		iCacheSize = KDefaultCacheSize;
		}

	TInt tempTimeout;
	if(iniData->FindVar(KCacheLit(),KDefaultEvictionTimeoutLit(), tempTimeout))
		{
		iDefaultTimeout = tempTimeout;
		}
	
	CleanupStack::PopAndDestroy();		
	};

void CRepositoryCacheManager::EnableCache(TInt aDefaultTimeout, TInt aCacheSize)
	{
	if (aDefaultTimeout>0)
		{
		iDefaultTimeout = aDefaultTimeout;
		}
	if (aCacheSize>0)
		{
		iCacheSize = aCacheSize;
		}
	
	EnableCache();
	}

void CRepositoryCacheManager::EnableCache()
	{
	// If disabled, enable
	if (!iEnabled)
		{
		iEnabled = ETrue;
		__CENTREP_TRACE2("CENTREP: Cache Enabled. Size:%d Default Timeout:%d", iCacheSize, iDefaultTimeout.Int());
		}
	}
		
void CRepositoryCacheManager::DisableCache()
	{
	// If enabled, disable
	if (iEnabled)
		{
		FlushCache();
		
		iEnabled = EFalse;
		__CENTREP_TRACE("CENTREP: Cache Disabled");
		}
	}

void CRepositoryCacheManager::RemoveIdleRepository(CSharedRepository* aRepository)
	{
	if (iEnabled)
		{
		TInt i;
		for(i=iIdleRepositories.Count()-1; i>=0; i--)
			{
			if(iIdleRepositories[i].iSharedRepository==aRepository)
				{
				break;
				}
			}
		
		// Idle repository might not be found in the list if multiple clients try to open the same 
		// repository at the same time. First client will remove it and second one will not find it
		if(i>=0)
			{
			__CENTREP_TRACE1("CENTREP: Cache Hit when opening repository %x", aRepository->Uid().iUid);

			iTotalCacheUsage -= iIdleRepositories[i].iSharedRepository->Size();		
			iIdleRepositories.Remove(i);
			
			// If this was the first repository on the list, it means its timer is still ticking. 
			// We have to stop it and ...
			if (i==0)
				{
				Cancel();
				// if there's still other repositories in the list, reschedule the timer with the
				// new top-of-the-list  
				if (iIdleRepositories.Count())
					{
					AtUTC(iIdleRepositories[0].iCacheTime);
					}
				}
			}
		else
			{
			__CENTREP_TRACE1("CENTREP: Multiple Client Hit when opening repository %x", aRepository->Uid().iUid);			
			}
		}
	}

#ifdef CACHE_OOM_TESTABILITY
  	// This code is only for tesing and doesn't go into MCL
	// Hence hide the leave in a macro instead of making StartEvictionL
#define TEST_CODE_LEAVE(x) User::Leave(x)
#endif	

TBool CRepositoryCacheManager::StartEviction(CSharedRepository*& aRepository)
	{
	// Execute the forced eviction algorithm only if it will make sense
	// The eviction makes sense if:
	// - there's anything in the cache to evict
	// - the repository we're trying to cache can fit in the cache after evictions
	if (iIdleRepositories.Count() && (aRepository->Size() < iCacheSize))
		{
		// Check to see if current cache size + the current repository size is overshooting the limit
		if (iTotalCacheUsage + aRepository->Size() > iCacheSize)
			{
			// Forced eviction
			__CENTREP_TRACE3("CENTREP: Cache Size Exceeded: Current(%d)+Size(%d)>Cache(%d)", iTotalCacheUsage, aRepository->Size(), iCacheSize);
			
			// Sort in the forced eviction order
			TLinearOrder<TRepositoryCacheInfo> forcedSortOrder(CRepositoryCacheManager::ForcedEvictionSortOrder);
			iIdleRepositories.Sort(forcedSortOrder);
			
			// Evict one by one until there's enough cache space or we run out of idle reps
			do
				{
				__CENTREP_TRACE1("CENTREP: Forced Eviction of repository %x", iIdleRepositories[0].iSharedRepository->Uid().iUid);			
				iTotalCacheUsage -= iIdleRepositories[0].iSharedRepository->Size();
				Evict(0);
				iIdleRepositories.Remove(0);		
				} while (iIdleRepositories.Count() && (iTotalCacheUsage + aRepository->Size() > iCacheSize));
			
#ifdef CENTREP_TRACE			
			if (!iIdleRepositories.Count())
				{
				__CENTREP_TRACE("CENREP: Cache Emptied by Forced Eviction");
				}
#endif				
			// Re-sort to timer order for normal operation
			TLinearOrder<TRepositoryCacheInfo> timerSortOrder(CRepositoryCacheManager::TimerEvictionSortOrder);
			iIdleRepositories.Sort(timerSortOrder);
			};
		}
	
	// See if there's enough space now
	if (iTotalCacheUsage + aRepository->Size() > iCacheSize)
		{
		return EFalse;
		}

	// Create new item for the cache and insert it in the list
	TRepositoryCacheInfo repInfo;
	
	repInfo.iCacheTime.UniversalTime();
	repInfo.iCacheTime += TTimeIntervalMicroSeconds32(iDefaultTimeout);
	repInfo.iSharedRepository = aRepository;
	
	TLinearOrder<TRepositoryCacheInfo> timerSortOrder(CRepositoryCacheManager::TimerEvictionSortOrder);
	// With the same timeout value assigned to all repositories, no two repositories can have the same 
	// timeout theoretically, so InsertInOrder is sufficient. But in practice, because of the poor 
	// resolution of the NTickCount() function used by TTime::UniversalTime(), InsertInOrderAllowRepeats 
	// should be used instead of InsertInOrder to allow for duplicate timer values caused by two 
	// repositories cached in quick succession (<1ms)
	TInt err = iIdleRepositories.InsertInOrderAllowRepeats(repInfo, timerSortOrder);
#ifdef CACHE_OOM_TESTABILITY
  	// This code is only for tesing and doesn't go into MCL
  	if (err == KErrNoMemory)	
  		{
  		delete aRepository;
  		aRepository = NULL;
  		// Should Leave here for the OOM tests to successfully complete. 
		TEST_CODE_LEAVE(err);
  		}
#endif	
	if (err!=KErrNone)
		{
		return EFalse;
		}

	iTotalCacheUsage += repInfo.iSharedRepository->Size();
	
	// reset timer to the new top-of-the-list
	Cancel();
	AtUTC(iIdleRepositories[0].iCacheTime);

	return ETrue;
	}

void CRepositoryCacheManager::FlushCache()
	{
	// cancel any outstanding timer
	Cancel();

	TInt idleRepCount = iIdleRepositories.Count();
	for(TInt repCount = idleRepCount - 1; repCount >= 0 ; repCount--)	
		{
		Evict(repCount);
		}
	
	// empty the list
	iIdleRepositories.Reset();
	
	iTotalCacheUsage = 0;
	__CENTREP_TRACE1("CENTREP: Cache Flush: %d repositories flushed", idleRepCount);
	}
	
// Evict only removes items from iOpenRepositories list, not from iIdleRepositories list
void CRepositoryCacheManager::Evict(TInt aIdleRepIndex)
	{
	// find and remove the idle repositories' pointers in the open repositories list 
	CServerRepository::RemoveRepository(iIdleRepositories[aIdleRepIndex].iSharedRepository);
	
	// delete the repository objects 
	delete iIdleRepositories[aIdleRepIndex].iSharedRepository;
	
	}
		
void CRepositoryCacheManager::RunL()
	{
	TTime now;

	now.UniversalTime();

	// Try to evict all the repositories which have expired. There might be more than one repository
	// destined to expire at the same time, or there are more than one repository with expiry times
	// between the scheduled expiry time and now (which theoretically should have been the same, but maybe
	// because of other procesor activity, the timer Active Object just got late a bit)
	while((iIdleRepositories.Count()) && (iIdleRepositories[0].iCacheTime<=now))
		{
		__CENTREP_TRACE1("CENTREP: Normal Eviction of repository %x", iIdleRepositories[0].iSharedRepository->Uid().iUid);			
		// Always remove from the top of the sorted list
		iTotalCacheUsage -= iIdleRepositories[0].iSharedRepository->Size();		
		Evict(0);
		iIdleRepositories.Remove(0);		
		};
		
	// reschedule to run again at the expiry date of next repository on the list, if any
	if (iIdleRepositories.Count())
		{
		AtUTC(iIdleRepositories[0].iCacheTime);
		}
	else
		{
		__CENTREP_TRACE("CENTREP: Cache Empty/Timer Disabled");			
		}
	}

TInt CRepositoryCacheManager::ForcedEvictionSortOrder(const TRepositoryCacheInfo &aRepository1, const TRepositoryCacheInfo &aRepository2)
	{
/*
   The code in the comments below is the original simple-to-read version of the algebraically
   simplified production code. 

	TTime now;

	now.UniversalTime();

	// we calculate the ages of the repositories by taking the difference between now and when
	// they were last became idle. When refactoring, the calculation of the absolute ages will be 
	// eleminated and the age difference between the repositories will be used in the formula instead
	
	TTimeIntervalMicroSeconds age1 = now.MicroSecondsFrom(aRepository1.iCacheTime);
	TTimeIntervalMicroSeconds age2 = now.MicroSecondsFrom(aRepository2.iCacheTime);
	
	// then divide the resulting numbers by conversion constant to get a number in a compatible unit
	// to the size. This operation reduces the microsecond-based values to having an approx. max
	// of 100K units

	TInt t1 = age1.Int64()/KTimeoutToSizeConversion;
	TInt t2 = age2.Int64()/KTimeoutToSizeConversion;
	
	// the resulting normalized time difference values are added to the size of the repository
	// resulting in an implicit %50 weight in the overall importance value 
	// An approx. maximum size of a repository is assumed to be around 100K
	
	TInt importance1 = t1+aRepository1.iSharedRepository->Size();
	TInt importance2 = t2+aRepository2.iSharedRepository->Size();
	
	// the difference between the importances of the repositories determine the sorting order

	return static_cast<TInt>(importance1-importance2);
*/	
	//	after refactoring, the resulting formula is this one
	return static_cast<TInt>(((aRepository1.iCacheTime.Int64()-aRepository2.iCacheTime.Int64())/KTimeoutToSizeConversion)+(aRepository1.iSharedRepository->Size()-aRepository2.iSharedRepository->Size()));	
	}

TInt CRepositoryCacheManager::TimerEvictionSortOrder(const TRepositoryCacheInfo &aRepository1, const TRepositoryCacheInfo &aRepository2)
	{
	return static_cast<TInt>(aRepository1.iCacheTime.Int64()-aRepository2.iCacheTime.Int64());
	}
