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

#include "panic.h"
#include "srvdefs.h"
#include "srvres.h"
#include "shrepos.h"
#include "srvrepos.h"
#include "srvparams.h"

#ifdef SYMBIAN_CENTRALREPOSITORY_PARANOID_CHECKING

/** Validates that the required invariants hold: (1) ordered by ascending key value (2) key uniqueness.
Panics upon violation.
*/
void RSettingsArray::ValidateInvariantProperties() const
	{
	if (Count() > 0)
		{
		const TServerSetting* prev = &( operator[](0) );
		const TServerSetting* curr = prev + 1;
		const TServerSetting* upperBound = prev + Count() - 1;
		while(curr <= upperBound)
			{
			// future: should replace with diagnostic panic code
			ASSERT(curr->Key() > prev->Key());
			prev = curr;
			++curr;
			}
		}
	}

/** Validates that the required invariants hold for the given setting pointer array: 
(1) ordered by ascending key value (2) key uniqueness. Panics upon violation.
*/
void RSettingsArray::ValidateInvariantProperties(const RSettingPointerArray& aPtrArray)
	{
	if (aPtrArray.Count() > 0)
		{
		const TServerSetting** prev = const_cast<const TServerSetting**>( &(aPtrArray[0]) );
		const TServerSetting** curr = prev + 1;
		const TServerSetting** upperBound = prev + aPtrArray.Count() - 1;
		while(curr <= upperBound)
			{
			// future: should replace with diagnostic panic code
			ASSERT((*curr)->Key() > (*prev)->Key());
			prev = curr;
			++curr;
			}
		}
	}

#endif

/** Returns pointers to all settings whose key matches all bits of the target id also included
in the bitwise mask.

@param	aTargetKey Bit pattern to match
@param	aMask Bitwise mask. Where bits are set the corresponding bit in aTargetKey is significant, ie
		AND behaviour conventional with masking
@param	aMatches Returns pointers to all settings with keys matching the mask, in ascending key order. The
		caller should ensure it is appropriately constructed for the likely growth pattern.
@return	Error code. aMatches may be modified even upon error return but the contents must not be relied upon.
*/
TInt RSettingsArray::Find(TUint32 aTargetKey, TUint32 aMask, RSettingPointerArray& aMatches) const
	{
#ifdef SYMBIAN_CENTRALREPOSITORY_PARANOID_CHECKING
	ValidateInvariantProperties();
#endif

	aMatches.Reset();
	// Find the first setting with key >= the lowest possible match
	const TInt upperIdx = Count() - 1;
	const TUint32 maskedTarget = aTargetKey & aMask;
	TInt lowerIdx;
	FindInUnsignedKeyOrder(TServerSetting(maskedTarget), lowerIdx);
	if(lowerIdx > upperIdx)
		{
		return KErrNone;
		}
	
	// this simply traverses the settings from the first possible match to the last. For masks with
	// only with higher bits this would be wasteful - in principle could jump to next possible match start
	// with another Find...() call. However logic more complex and counterproductive in some cases; would
	// need some heuristic such as probing N settings ahead of current point and only invoking the refind
	// if that still falls below the next range start, where N would allow for the cost of the binsearch
	// and other book-keeping (for real smartness construct a new RArray from the current point onwards,
	// so binsearch excludes the already known infertile preceding settings). Only worth pursuing after
	// profiling.
	const TUint32 upperMatchingKey = aTargetKey | ~aMask;
	const TServerSetting* setting = &( operator[](lowerIdx) );
	const TServerSetting* upperBound = setting + (upperIdx - lowerIdx);
	while(setting <= upperBound && setting->Key() <= upperMatchingKey)
		{
		if((setting->Key() & aMask) == maskedTarget)
			{
			TInt err = aMatches.Append(setting);
			// Small optimisation of not testing err in while() condition, presuming
			// that Append() far less common than key tests, ie repeatedly testing err
			// is wasteful. However (arguably) makes code more complex
			if(err != KErrNone)
				{
				aMatches.Reset();
				return err;
				}
			}
		++setting;
		}
	return KErrNone;
	}

/** Given source and destination arrays of settings, replaces the destination with the union of the
arrays. The properties of ascending key order and key uniqueness are preserved: where a key is common
to both arrays the source setting is kept, excepting where the source setting is flagged as deleted, which
simply deletes the destination

@param	aDst destination settings array
@param	aSrc source settings array
@return Error code. If not KErrNone then the contents of aDst are may have been modified and must be
		disregarded
@post	aDst contains the union and matches the setting array invariant
*/
TInt RSettingsArray::Merge(RSettingPointerArray& aDst, const RSettingPointerArray& aSrc)
	{
	const TServerSetting** srcBound;
	const TServerSetting** srcPtr;
	TInt count = aSrc.Count();
	if(count > 0)
		{
		srcPtr = const_cast<const TServerSetting**>( &(aSrc[0]) );
		srcBound = srcPtr + count;
		}
	else
		{
		srcPtr = NULL;
		srcBound = NULL;
		}

	TInt dstIdx = 0;
	const TServerSetting** dstPtr=NULL;
	const TServerSetting** dstBound=NULL;

	// The approach taken here of inserting entries one at a time is necessarily inefficient
	// given the RPointerArray interface, but may be adequate for the projected (small) use. For 
	// greater efficiency the ability to insert ranges is required, ie probably custom array code
	while(srcPtr < srcBound)
		{
		if (aDst.Count() > 0)
			{
			dstBound = const_cast<const TServerSetting**>( &(aDst[0]) ) + aDst.Count();
			dstPtr=const_cast<const TServerSetting**>( &(aDst[0]) )+ dstIdx;
			}
		const TUint32 srcKey = (*srcPtr)->Key();
		while(dstPtr < dstBound && srcKey > (*dstPtr)->Key())
			{
			++dstPtr;
			++dstIdx;
			}
		// Three possibilities: end of dst reached (append src), src and dst match (replace dst), 
		// and dst now greater than src (insert src)
		if((dstPtr == dstBound || (*dstPtr)->Key() > srcKey))
			{
			if(!(*srcPtr)->IsDeleted())	// drop deleted records
				{
				TInt ret = aDst.Insert(*srcPtr, dstIdx);	// presumes insert at end == append
				if(ret != KErrNone)
					{
					return ret;
					}
				++dstIdx;
				}
			}
		else
			{
			// Src key matches dst, replace or delete dst
			if(!(*srcPtr)->IsDeleted())
				{
				*dstPtr= *srcPtr;
				++dstIdx;
				}
			else
				{
				// may be more efficient ways of handling remove
				aDst.Remove(dstIdx);
				--dstBound;
				}
			}
		++srcPtr;
		}
#ifdef SYMBIAN_CENTRALREPOSITORY_PARANOID_CHECKING
	RSettingsArray::ValidateInvariantProperties(aDst);
#endif
	return KErrNone;
	}

CSharedRepository::CSharedRepository(TUid aUid) :
	iSettings(), iUid(aUid),
	iSinglePolicies(KGranularity),
	iTransactors(_FOFF(CRepositoryTransactor, iLink))
	{
	iPersistsIniFileExists = ETrue;
	
	// debug check that CRepository::TTransactionMode modes match those used internally
	// from CRepositoryTransactor: internal state logic relies on this
	// there should be a better location for these checks...
	ASSERT(CRepository::EReadTransaction == EReadTransaction);
	ASSERT(CRepository::EConcurrentReadWriteTransaction == EConcurrentReadWriteTransaction);
	ASSERT(CRepository::EReadWriteTransaction == EReadWriteTransaction);
	}
	
CSharedRepository::~CSharedRepository()
	{
	// sanity check that no transactions are active
	ASSERT(!IsTransactionActive());
	// check transactor queue is empty - note this includes failed transactions
	ASSERT(NULL == (CRepositoryTransactor*)TDblQueIter<CRepositoryTransactor>(iTransactors));

	iSinglePolicies.ResetAndDestroy();
	iDeletedSettings.Close();
	}

TUid CSharedRepository::Uid() const
	{
	return iUid;
	}

/**
Stores the repository in-memory content to the related repository file on drive C.
If the operation fails, the in-memory content won't match the content of 
the repository file (which will be kept as it was before the CommitChangesL() call).
In order to keep the consistency, the in-memory repository content is deleted now
and restored later, on the next repository operation.
*/
TInt CSharedRepository::CommitChanges()
	{
	TRAPD(error, DoCommitChangesL());
	if (error != KErrNone)
		{
		ResetContent();
		}
	return error;
	}

/** Merges the aChanges settings into array. Is used for
	Transaction merges
	Restore merges
	SWI merges

	Works as follows:
(a) EDeleted settings in aChanges destroy the corresponding setting in this array (if any)
(b) Where matching settings exist in both arrays, data from aChanges replaces originals. 
    Matching depends on aKeyOnly parameter's value. If true, matching is only
    based on key. If false, settings values will be compared
    For SWI merges this replacement is also dependent on whether the setting has been modified
(c)	aMergeType is the merge type
Future: All descriptors are deeply copied; must change to use shallow copy for greater efficiency.
@post Following a requirement that notifications are only made if values have changed:
1. Settings flagged as deleted without a counterpart in the persistent array are removed from aChanges.
2. Where the new value and the existing value are the same, the setting is removed from aChanges.
*/
TInt RSettingsArray::MergeArray(RSettingsArray& aChanges, RArray<TUint32> & aDeletedSettings, TMergeType aMergeType, TBool aKeyOnly)
	{
	TInt error = KErrNone;
	TInt numChanges;
	TInt i;
		
	// For downgrade remove SWI only keys
	if(aMergeType==ESWIDowngradeMerge)
		{
		numChanges = Count();
		i = 0;
		while ((i < numChanges) && (KErrNone == error))
			{
			TServerSetting source = operator[](i);
			// find index of item in array matching the index of this change
			TInt targetIndex = aChanges.FindIndex(source);
			if (targetIndex == KErrNotFound)
				{
				// If key is default and wasn't in the ROM, it was an install key
				// so remove it
				if(source.IsClean())
					{
					DeleteElement(i);
					--numChanges;
					--i;
					}
				}
			++i;
			}
		}

	numChanges = aChanges.Count();
	i=0;
	
	while ((i < numChanges) && (KErrNone == error))
		{
		const TServerSetting& source = aChanges[i];
		// find index of item in array matching the index of this change
		TInt targetIndex = FindIndex(source);
		if (targetIndex == KErrNotFound)
			{
			if (source.IsDeleted())
				{
				// remove setting that is flagged as deleted to prevent notification
				aChanges.DeleteElement(i);
				i--;
				numChanges--;
				}
			else
				{
				// add the new setting
				TServerSetting newSetting(source.Key());
				error = newSetting.Replace(source);
				if (error == KErrNone)
					{
					error = OrderedInsert(newSetting);
					TInt deletedSetting = aDeletedSettings.FindInUnsignedKeyOrder(source.Key());
					if (KErrNotFound != deletedSetting)
						{
						aDeletedSettings.Remove(deletedSetting) ;
						}
					if (error != KErrNone)
						{
						newSetting.Reset();
						}
					}
				}
			}
		else
			{
			if (source.IsDeleted())
				{
				// Retain "deleted element" state for settings marked for backup so that
				// we can persist state correctly across a backup/restore cycle.
				if (operator[](targetIndex).Meta() & KMetaBackup)
					{
						aDeletedSettings.InsertInUnsignedKeyOrder(source.Key()) ;
					}
				DeleteElement(targetIndex);
				}
			else
				{
				TServerSetting& target = operator[](targetIndex);
				if ((target == source) && (aKeyOnly==EFalse))
					{
					// value not changing: remove setting to prevent notification
					// Note that for SWI merges, settings are not deleted from 
					// aChanges unless they are marked with EDeleted.
					aChanges.DeleteElement(i);
					i--;
					numChanges--;
					}
				else
					{						
					if( (aMergeType==ESWIUpgradeMerge) || (aMergeType==ESWIDowngradeMerge))
						{
						if(target.IsClean())
							{
							error = target.Replace(source);
							}
						else
							{
							// value not changing: remove setting to prevent notification
							aChanges.DeleteElement(i);
							i--;
							numChanges--;
							}
						}
					else
						{
						error = target.Replace(source);
						}
					}
				}
			}
		i++;
		}
	aDeletedSettings.Compress();
	return error;
	}

// merge transaction settings (which may include entries flagged as deleted), persist and notify
// private method relies on calling code to ensure it is permitted to make changes here.
// if this method is committing any changes, it cancels all other sessions' transactions
TInt CSharedRepository::DoCommitTransactionSettings(CRepositoryTransactor& aTransactor, TUint32& aKeyInfo)
	{
	aKeyInfo = KUnspecifiedKey;
	if (0 == aTransactor.iTransactionSettings.Count())
		{
		aKeyInfo = 0; // == number of settings modified
		return KErrNone; // nothing to do
		}
 	TInt error = iSettings.MergeArray(aTransactor.iTransactionSettings, iDeletedSettings, ETransactionMerge, EFalse);
	TInt numChanges = aTransactor.iTransactionSettings.Count();
	if (numChanges == 0)
		{
		if (error == KErrNone)
			{
			aKeyInfo = 0; // no changes
			}
		// no changes were made, so the internal cache is still valid.
		// This could be because there were no changes: empty list, only deletes on
		// non-existent items (a setting created and deleted in the transaction),
		// or because of error, such as failure of an initial realloc failure.
		return error;
		}
	if (error != KErrNone)
		{
		// the repository is corrupted. Dump it for lazy loading later
		ResetContent();
		}
	if (error == KErrNone)
		{
		// changes have been made: fail all other sessions' transactions so we can commit
		FailAllTransactions(/*aExcludeTransactor=*/&aTransactor);
		error = CommitChanges(); // this already calls ResetContent() in case of failure
		}
	if (error == KErrNone)
		{
		// settings are now persistent on disk: we can now notify about the changes
		// following will notify about objects that are created and deleted in the transaction
		// this could be made faster by having a multiple Notify method.
		// That would also allow Notify messages to be more descriptive - ranges of Keys
		for (TInt i = 0; i < numChanges; i++)
			{
			Notify(aTransactor.iTransactionSettings[i].Key());
			}
		aKeyInfo = /*reinterpret_cast<TUint32>*/numChanges;
		}
	return error;
	}

void CSharedRepository::SetMetaDataOnRead(TServerSetting& aSetting, TBool aSingleMetaFound)
	{
	TInt isMetaFlagSet = aSetting.Meta() & KMetaDefaultValue;
	
	if(!aSingleMetaFound)
		// No single metadata set for this key
		{
		// First check for a matching "range" default metadata
		// setting
		TSettingsDefaultMeta *defaultMeta = iRangeMeta.Find(aSetting.Key());
		if (defaultMeta)
			{
			if (isMetaFlagSet)
				//sets a default meta data
				//also sets the flag back to indicate that it is a default setting from ROM 
				//or previous install so it can be replaced later with a new one. 
				aSetting.SetMeta(defaultMeta->GetDefaultMetadata() | KMetaDefaultValue);
			else
				aSetting.SetMeta(defaultMeta->GetDefaultMetadata());
			}
		else
			{
			// Range value not found, try for a repository default
			if (isMetaFlagSet)	
				aSetting.SetMeta(iDefaultMeta | KMetaDefaultValue) ;
			else
				aSetting.SetMeta(iDefaultMeta) ;
			}
		}
	}

TInt CSharedRepository::CreateL(TServerSetting& aSetting, TSettingsAccessPolicy* &aPolicy, TBool aSingleMetaFound)
	{
	if(iSettings.Find(aSetting.Key()))
		return KErrAlreadyExists;
	
	SetMetaDataOnRead( aSetting, aSingleMetaFound);
		
	TUint32 key = aSetting.Key();
	aSetting.SetAccessPolicy(aPolicy);

	iSettings.OrderedInsertL(aSetting);
	Notify(aSetting.Key());
	return KErrNone;
	}

// deletes an individual setting in the shared repository and makes it persistent
// if changes are made, all sessions' transactions are failed
TInt CSharedRepository::DeleteAndPersist(TUint32 aId)
	{
	TServerSetting* s = iSettings.Find(aId);
	if(!s)
		return KErrNotFound;

	iSettings.Remove(aId);
	// removed a setting, so must fail all sessions' transactions before commit possible
	FailAllTransactions(/*aExcludeTransactor=*/NULL);
	TInt error = CommitChanges();
	if (error == KErrNone)
		{
		Notify(aId);
		}
	return error;
	}

// deletes an individual setting without making it persistent
// must not be called while any sessions are in transactions
TInt CSharedRepository::DeleteNoPersist(TUint32 aId)
	{
	// should only be calling this if no transactions are active
	ASSERT(!IsTransactionActive());

	TServerSetting* s = iSettings.Find(aId);
	if(!s)
		return KErrNotFound;

	iSettings.Remove(aId);
	Notify(aId);
	return KErrNone;
	}

TInt CSharedRepository::ResetNoPersistL(TServerSetting& aSetting)
	{
	TServerSetting* s = iSettings.Find(aSetting.Key());
	if ((!s) || (*s != aSetting))
		{
		if (s)
			{
			// save access policy of setting
			TSettingsAccessPolicy* policy=s->AccessPolicy();
			s->Transfer(aSetting);
			// restore access policy of setting
			s->SetAccessPolicy(policy);
			}
		else
			{
			TServerSetting setting;
			setting.Transfer(aSetting);
			setting.SetAccessPolicy(GetFallbackAccessPolicy(setting.Key()));
			setting.PushL();
			iSettings.OrderedInsertL(setting);
			setting.Pop();
			}
		}
	else
		{
		return KErrGeneral;
		}
	return KErrNone;
	}

// if changes are made, all sessions' transactions are failed
void CSharedRepository::ResetAndPersistL(TServerSetting& aSetting)
	{
	if (ResetNoPersistL(aSetting) == KErrNone)
		{
		// changed a setting, so must fail all sessions' transactions
		// before commit possible
		FailAllTransactions(/*aExcludeTransactor=*/NULL);
		CommitChangesL();
		Notify(aSetting.Key());
		}
	}

TInt CSharedRepository::ResetAllNoPersistL(CSharedRepository& aNewContent)
	{
	// mark cache as inconsistent in case Reset fails, so it is reloaded.
	iInconsistentData = ETrue;

	// should not change repository while transactions in progress: should fail them first
	ASSERT(!IsTransactionActive());
	TInt newCount = aNewContent.iSettings.Count();
	TInt count = iSettings.Count();

	TInt newIndex = 0;
	TInt index = 0;

	while(newIndex<newCount && index<count)
		{
		const TServerSetting& newSetting = aNewContent.iSettings[newIndex];
		const TServerSetting& setting = iSettings[index];

		TUint32 newKey = newSetting.Key();
		TUint32 key = setting.Key();

		if(newKey<key)
			{
			Notify(newKey);
			newIndex++;
			}
		else if(newKey==key)
			{
			if(newSetting!=setting)
				Notify(key);
			newIndex++;
			index++;
			}
		else if(newKey>key)
			{
			Notify(key);
			index++;
			}
		}

	while(newIndex<newCount)
		Notify(aNewContent.iSettings[newIndex++].Key());

	while(index<count)
		Notify(iSettings[index++].Key());

	// Replace current settings with settings read from ROM, this 
	// will leave settings pointing to new single policies
	iSettings.AdoptL(aNewContent.iSettings);

	// Reset policy pointers to point at this repositories policies
	newCount=iSettings.Count();
	for(TInt i=0; i<newCount;i++)
		{
		iSettings[i].SetAccessPolicy(NULL);
		TUint32 key = iSettings[i].Key();
		iSettings[i].SetAccessPolicy(GetFallbackAccessPolicy(key));
		}	

	iInconsistentData = EFalse;
	return KErrNone;
	}
	
// Comparison relation to allow single policies to be inserted in order
TBool CSharedRepository::CompareKeyIds(TSettingsAccessPolicy const &aSinglePolicy, TSettingsAccessPolicy const &aSinglePolicyIndexItem)
	{
	if(aSinglePolicy.iLowKey==aSinglePolicyIndexItem.iLowKey)
		return 0;
	return (aSinglePolicy.iLowKey < aSinglePolicyIndexItem.iLowKey)?-1:1;
	}

// Identity relation to allow single policy for a given key to be found
TBool CSharedRepository::SinglePolicyMatchOnKey(TSettingsAccessPolicy const &aSinglePolicy, TSettingsAccessPolicy const &aSinglePolicyIndexItem)
	{
	return aSinglePolicy.iLowKey==aSinglePolicyIndexItem.iLowKey;
	}

// returns the read security policy used if there is no per-setting policy at aId
const TSecurityPolicy& CSharedRepository::GetFallbackReadAccessPolicy(TUint32 aId)
	{
	return *(GetFallbackAccessPolicy(aId)->GetReadAccessPolicy());
	}

// returns the write security policy used if there is no per-setting policy at aId
const TSecurityPolicy& CSharedRepository::GetFallbackWriteAccessPolicy(TUint32 aId)
	{
	return *(GetFallbackAccessPolicy(aId)->GetWriteAccessPolicy());
	}

// Get pointer to security policy that applies to a given setting
TSettingsAccessPolicy* CSharedRepository::GetFallbackAccessPolicy(TUint32 aId)
	{
	// Check for single policy
	TSettingsAccessPolicy policy(aId);
	TIdentityRelation<TSettingsAccessPolicy> identity(SinglePolicyMatchOnKey);
	TInt index = iSinglePolicies.Find(&policy, identity);
	if(KErrNotFound != index)
		return iSinglePolicies[index];
	
	// check if the aId falls into any range specified in the ini file
	TSettingsAccessPolicy* rangePolicy = iRangePolicies.Find(aId);
	if(rangePolicy)
		return rangePolicy;
	
	// If no single policy or range policy, return default policy
	return &iDefaultPolicy;
	}


TInt CSharedRepository::ReadSettingSavePolicyL(CIniFileIn& aFile,TServerSetting& aSetting, TSettingsAccessPolicy* &aPolicy, TBool& aSingleMetaFound)
	{
	TBool singleReadPolicyFound;
	TBool singleWritePolicyFound;
	TSecurityPolicy singleReadPolicy;
	TSecurityPolicy singleWritePolicy;

	TInt err=aFile.ReadSettingL(aSetting,singleReadPolicy, singleWritePolicy, singleReadPolicyFound, singleWritePolicyFound, aSingleMetaFound);
	if(err!=KErrNone)
		return err;
	
	// Set up single policies
	if(!singleReadPolicyFound)
		singleReadPolicy=GetDefaultReadAccessPolicy();
	if(!singleWritePolicyFound)
		singleWritePolicy=GetDefaultWriteAccessPolicy();

	aSetting.PushL();
	if(singleReadPolicyFound || singleWritePolicyFound)
		{
		aPolicy=new (ELeave) TSettingsAccessPolicy(singleReadPolicy,singleWritePolicy,aSetting.Key());
		CleanupStack::PushL(aPolicy);
		TLinearOrder<TSettingsAccessPolicy> order(&CSharedRepository::CompareKeyIds);
		SinglePolicyArray().InsertInOrderL(aPolicy, order);
		CleanupStack::Pop(aPolicy);
		}
	else
		{
		// check if the aId falls into any range specified in the ini file
		// otherwise set policy to default policy
		TSettingsAccessPolicy* rangePolicy = iRangePolicies.Find(aSetting.Key());
		if(rangePolicy)
			aPolicy=rangePolicy;
		else
			aPolicy=&iDefaultPolicy;
		}
		
	aSetting.Pop();
	return err;
	}	

// Merge settings in this->iSettings with the iSettings of aMergeRep
// During an intsall/upgrade event aMergeRep will be created from the installed file
// During an upinstall event aMergeRep will be created from the ROM file
void CSharedRepository::MergeL(CSharedRepository& aMergeRep, TMergeType aMergeType)
	{								
	// Process settings from main section - this updates values only
	User::LeaveIfError(iSettings.MergeArray(aMergeRep.iSettings, iDeletedSettings, aMergeType));
		
	// Update all access policies and meta
	for(TInt i=0; i<iSettings.Count();i++)
		{
		TServerSetting& setting= iSettings[i];
		setting.SetAccessPolicy(GetFallbackAccessPolicy(setting.Key()));
		}
	}

// Get timestamp of installed file
TTime CSharedRepository::InstallTime()
	{
	return iTimeStamp;
	}
	
// Save timestamp of installed file 
void CSharedRepository::SetInstallTime(TTime aInstallTime)
	{
	iTimeStamp = aInstallTime;
	}
    
// Return if this repository's installed file exists and if it does
// return the file time stamp as well
TInt CSharedRepository::InstallFileTimeStampL(TTime& aModified)
	{
	// Ensure that there is an install directory
	if( !TServerResources::iInstallDirectory)
		{
		return KErrNotFound;
		}
	
	// Get name for open repository
	// Look for .cre file first
	HBufC* fullPath(NULL);
	TCentRepLocation location=EInstall;
    TServerResources::CreateRepositoryFileNameLC(fullPath, iUid, location, ECre);
		
	// Look in install directory to see if this repository has been hit
	TEntry entry;
	TInt r=TServerResources::iFs.Entry(fullPath->Des(),entry);
	if( r == KErrNone)
		{
		aModified=entry.iModified;
		}
	else if( r== KErrNotFound)
		{
		// Look for txt file
    	TServerResources::CreateRepositoryFileNameLC(fullPath, iUid, location, EIni);
		r=TServerResources::iFs.Entry(fullPath->Des(),entry);
		if( r == KErrNone)
			{
			aModified=entry.iModified;
			}	
		}
	return r;
	}
	
// Handle creation or upgrade of file in install directory
void CSharedRepository::HandleUpdateMergeL(TTime aInstallFileTimeStamp, CSharedRepository& aInstallRep)
	{			
	MergeL(aInstallRep, ESWIUpgradeMerge);
	
	SetInstallTime(aInstallFileTimeStamp);	// Set merge timestamp		
	CommitChangesL();						// Commit changes to write C: file

	// settings are now persistent on disk: we can now notify about the changes
	for (TInt i = 0; i < aInstallRep.iSettings.Count(); i++)
		{
		Notify(aInstallRep.iSettings[i].Key());
		}
	}
	
// Handle merge activity due to an uninstall
void CSharedRepository::HandleDeleteMergeL(CSharedRepository& aRomRep)
	{
	MergeL(aRomRep, ESWIDowngradeMerge);
	
	SetInstallTime(0);						// Reset timestamp			
	CommitChangesL();						// Commit changes to write C: file
	
	// settings are now persistent on disk: we can now notify about the changes
	for (TInt i = 0; i < aRomRep.iSettings.Count(); i++)
		{
		Notify(aRomRep.iSettings[i].Key());
		}
	}
	

/**
Statement "iInconsistentData = ETrue;" must be the first statement in the method,
"iInconsistentData = EFalse;" must be the last. It is used for lasy-load implementation
for the repository and solves the problem that if CommitChangesL() fails the in-memory
repository data won't match the repository data, stored in the file.
This routine is being retained for testing purposes
*/
void CSharedRepository::DoCommitChangesToIniFileL(const TDesC& aOutFileName)
	{
	iInconsistentData = ETrue;

	// should not be committing while transactions are still active
	ASSERT(!IsTransactionActive());

	CIniFileOut* out = CIniFileOut::NewLC();

	out->WriteHeaderL();
	out->WriteOwnerSectionL(iOwner);
	out->WriteTimeStampL(iTimeStamp);
	out->WriteMetaDataL(iDefaultMeta, iRangeMeta);
	out->WritePlatSecL(iDefaultReadPolicy, iDefaultWritePolicy, iRangePolicies);

	out->WriteMainSectionHeaderL();
	for(TInt i=0; i<iSettings.Count(); i++)
		{
		const TServerSetting& setting = iSettings[i];
		if (setting.HasAccessPolicy() && (iSinglePolicies.Find(setting.AccessPolicy()) != KErrNotFound))
			{
			out->WriteSettingL(setting, *setting.AccessPolicy());
			}
		else
			{
			out->WriteSettingL(setting);
			}
		}

	out->CommitL(aOutFileName);
	CleanupStack::PopAndDestroy(out);//out
	iInconsistentData = EFalse;
	}


/**
Statement "iInconsistentData = ETrue;" must be the first statement in the method,
"iInconsistentData = EFalse;" must be the last. It is used for lasy-load implementation
for the repository and solves the problem that if CommitChangesL() fails the in-memory
repository data won't match the repository data, stored in the file.
*/
void CSharedRepository::DoCommitChangesL() 
	{
	iInconsistentData = ETrue;
	
	// should not be committing while transactions are still active
	ASSERT(!IsTransactionActive());
	
	TCentRepLocation location = EPersists;
	HBufC* persistsTrnsFilePath(NULL);
    //allocates memory on the heap
    TServerResources::CreateRepositoryFileNameLC(persistsTrnsFilePath,iUid,location,ETmp);
	// Create file store
	CDirectFileStore* store = CDirectFileStore::ReplaceLC(TServerResources::iFs, *persistsTrnsFilePath,
															  (EFileWrite | EFileShareExclusive));
	const TUid uid2	 = KNullUid ;														 
	store->SetTypeL(TUidType(KDirectFileStoreLayoutUid, uid2, KServerUid3)) ; 

	// Write the stream index/dictionary as root stream within the store
	// so we can access it when we do a restore later on
	RStoreWriteStream rootStream ;
	TStreamId rootStreamId = rootStream.CreateLC(*store) ;
	ExternalizeCre(rootStream) ;
	rootStream.CommitL() ;
		
	CleanupStack::PopAndDestroy(&rootStream) ;
	store->SetRootL(rootStreamId);
	store->CommitL();
	CleanupStack::PopAndDestroy(store) ;	
	
	HBufC* persistsFilePath(NULL);	   
	TServerResources::CreateRepositoryFileNameLC(persistsFilePath,iUid,location,ECre);
	TEntry entry;
	if(TServerResources::iFs.Entry(*persistsFilePath,entry)==KErrNone)
		{
		User::LeaveIfError(TServerResources::iFs.Replace(*persistsTrnsFilePath, *persistsFilePath));
		}
	else
		{
		User::LeaveIfError(TServerResources::iFs.Rename(*persistsTrnsFilePath, *persistsFilePath));
		}
	
	CleanupStack::PopAndDestroy(persistsFilePath);
	CleanupStack::PopAndDestroy(persistsTrnsFilePath);		
	iInconsistentData = EFalse;		
	
	// Delete any exitsing ini file on 1st sucessful externalizing of repository
	if(iPersistsIniFileExists)
		{
		HBufC* iniFileName(NULL);	   
	    TServerResources::CreateRepositoryFileNameLC(iniFileName,iUid,location,EIni);	
		TServerResources::iFs.Delete(*iniFileName);
		CleanupStack::PopAndDestroy(iniFileName);
		iPersistsIniFileExists=EFalse;
		}
	}

/**
The method reloads the repository content from a repository file.
The current repository must be emptied (or must be empty already) before the call is made.
@param aIniFile A reference to CIniFileIn object, which will be used to load
				the repository content.
@return KErrCorrupt Corrupted repository file.
		KErrNone	The repository content was seccessfully loaded into memory.
		KErrNotFound Setting not found in the file.
@leave System-wide error codes.
@leave KErrGeneral It's probably a programmer's mistake - current CSharedRepository 
				   object is partially initialised.
*/
TInt CSharedRepository::ReloadContentL(CIniFileIn& aIniFile)
	{
	// Preconditions - CSharedRepository object should be an empty one.
	if(iSettings.Count() != 0 || iRangeMeta.Count() != 0 ||
	   iSinglePolicies.Count() != 0 || iRangePolicies.Count() != 0)
		{
		User::Leave(KErrGeneral);
		}
	// Look for an "owner" section
	TUint32 uidValue(KNullUid.iUid);
	TInt err = aIniFile.ReadOwnerSectionL(uidValue);
	if(err == KErrCorrupt)
		{
		return err;
		}
	iOwner.iUid = uidValue;
	// Timestamp
	TTime timeStamp (0);
	err = aIniFile.ReadTimeStampSectionL(timeStamp);
	if(err == KErrCorrupt)
		{
		return err;
		}
	if(timeStamp.Int64() != 0)
		{
		iTimeStamp = timeStamp;
		}

	// Metadata
	err = aIniFile.ReadDefaultMetaSecSectionL(iDefaultMeta, iRangeMeta);
	if(err == KErrCorrupt)
		{
		return err;
		}
	// Default read/write policies
	TBool gotDefaultReadPolicy;
	TBool gotDefaultWritePolicy;
	err = aIniFile.ReadPlatSecSectionL(iDefaultReadPolicy, gotDefaultReadPolicy,
									   iDefaultWritePolicy, gotDefaultWritePolicy,
									   iRangePolicies);
	if(err == KErrCorrupt)
		{
		return err;
		}
		
	iDefaultPolicy=TSettingsAccessPolicy(iDefaultReadPolicy,iDefaultWritePolicy, KUnspecifiedKey);

	// Settings
	TServerSetting setting;
	TSettingsAccessPolicy* policy;
	TBool singleMetaFound;
	while((err = ReadSettingSavePolicyL(aIniFile, setting, policy, singleMetaFound)) == KErrNone)
		{
		setting.PushL();
		if(iSettings.IsDefault())
			{
			setting.SetClean();			
			}
		User::LeaveIfError(CreateL(setting, policy, singleMetaFound));
		setting.Pop();
		}
	if(err == KErrNotFound)			
		{
		return KErrNone;
		}
	return err;
	}

/**
Resets current repository data - actually all of them, which may be loaded from
the related ini file.
The iUid data member value is kept as it was at the moment of creation of 
CSharedRepository object.
*/
void CSharedRepository::ResetContent()
	{
	iSettings.Reset();
	iOwner = KNullUid;
	iTimeStamp = TTime(0);

	for (TInt i=0;i<iSinglePolicies.Count();i++)
		{
		delete iSinglePolicies[i];
		}
	iSinglePolicies.Reset();
	iRangePolicies.Reset();
	iDefaultReadPolicy = TSecurityPolicy();
	iDefaultWritePolicy = TSecurityPolicy();
	iDefaultPolicy=TSettingsAccessPolicy(iDefaultReadPolicy,iDefaultWritePolicy, KUnspecifiedKey);
	iDefaultMeta = 0;
	iRangeMeta.Reset();
	}

/**
This function is used to restore the notification, which was temporary disabled
when making RestoreConsistencyL() call.
@param aNotificationState It points to CObservable::iNotificationState data member, which 
						 controls the notification state - active or disabled.
@internalComponent
*/
static void RestoreNotification(void* aNotificationState)
	{
	TBool* notificationState = static_cast <TBool*> (aNotificationState);
	*notificationState = ETrue;
	}

/**
The method reloads the repository content from the related ini file if previous
CommitChangesL() has not completed successfully.
*/
void CSharedRepository::RestoreConsistencyL()
	{
	//Do nothing if previous CommitChangesL() completed successfully.
	if(!iInconsistentData)
		{
		return;
		}
	//Reset current repository data	
	ResetContent();
	//Disable notifications
	TCleanupItem restoreNotification(&RestoreNotification, &iNotificationState);
	CleanupStack::PushL(restoreNotification);
	iNotificationState = EFalse;
	//Reload the repository content from the related ini file
	DoRestoreConsistencyL();
	//Activate notifications
	CleanupStack::PopAndDestroy();//restoreNotification
	
	 TCentRepLocation location = EPersists;
	HBufC* persistsTmpFilePath(NULL);
    //allocates memory on the heap
    TServerResources::CreateRepositoryFileNameLC(persistsTmpFilePath,iUid,location,ETmp);
	// Remove any .tmp file
	TServerResources::iFs.Delete(*persistsTmpFilePath);
	CleanupStack::PopAndDestroy(persistsTmpFilePath);
	
	iInconsistentData = EFalse;	
	}

/**
This method looks for and sets a location for a given repository.
It is based on EAuto mode thus it goes through all locations in the
same order (EPersists - EInstall - ERom)  

@param aLocation - returns a location for a repository
@param aUid - id of a repository which location should be found
@param aType - repository file type (.txt or .cre) 
@return KErrNone if aLocation succesfully set for a given repository,
		KErrNotFound if a repository was not found in any locations.

@internalTechnology
*/
TInt CSharedRepository::FindLocationForFileL(TCentRepLocation& aLocation,TUid aUid,const TCentRepFileType aType) const
    { 		
	if(TServerResources::CentrepFileExistsL(aUid, EPersists, aType))
	    {
	     aLocation = EPersists;
		 return KErrNone;
	    }
	    
	if(TServerResources::CentrepFileExistsL(aUid, EInstall, aType))
		{
	     aLocation = EInstall;
		 return KErrNone;
	    }
	    
	if(TServerResources::CentrepFileExistsL(aUid, ERom, aType))
		{
		aLocation = ERom;
		return KErrNone;
		}

	return KErrNotFound;
    }
    
/**
The method reloads the repository content from the related cre or ini file.
@leave System-wide error codes
*/	
void CSharedRepository::DoRestoreConsistencyL()
	{
	 TCentRepLocation location;
	
	TInt err = FindLocationForFileL(location,iUid,ECre);
	if (err != KErrNotFound)
		{
		User::LeaveIfError(CreateRepositoryFromCreFileL(location));
		return;
		}
		
	User::LeaveIfError(FindLocationForFileL(location,iUid,EIni));
	
	HBufC* fileName(NULL);
    TServerResources::CreateRepositoryFileNameLC(fileName,iUid,location,EIni);	
 
	CIniFileIn* iniFile = NULL;
	err = CIniFileIn::NewLC(iniFile,fileName,location);
	User::LeaveIfError(err);
	

	err = ReloadContentL(*iniFile);
	User::LeaveIfError(err);
	
	CleanupStack::PopAndDestroy(iniFile); //iniFile 
	CleanupStack::PopAndDestroy(fileName);	//fileName
	fileName=0;
	}
    
TInt CSharedRepository::CreateRepositoryFromCreFileL( TCentRepLocation aLocation)
	{
	// Get file path name from location
    HBufC* filePath(NULL);
    TServerResources::CreateRepositoryFileNameLC(filePath,iUid, aLocation,ECre);
    // Trap errors from repository creation so we can delete corrupt repositories
	TRAPD(error, CreateRepositoryFromCreFileL(*filePath));
	if(error!=KErrNone && error!=KErrNotFound && error!=KErrNoMemory)
		{
		error=KErrCorrupt;
		// store wasn't quite what we were expecting - can't return an error, can't leave
		// so all we can do is close the file, tidy up as best we can, and return corrupt 
		if (aLocation != ERom)
			{
			TServerResources::iFs.Delete(*filePath) ;
			}
		}
	else if( error==KErrNoMemory)
		{
		User::Leave(KErrNoMemory);
		}
	CleanupStack::PopAndDestroy(filePath);
	return error;
	}
    
void CSharedRepository::CreateRepositoryFromCreFileL(TDesC& aFilePath )
	{	

	TEntry entry;
	TInt e = TServerResources::iFs.Entry(aFilePath, entry);	
	if(e==KErrNotFound || e==KErrPathNotFound)
		{
		User::Leave( KErrNotFound);
		}

	CDirectFileStore* store = 
	    CDirectFileStore::OpenLC (TServerResources::iFs,aFilePath, EFileRead|EFileShareReadersOnly);
	if(store->Type()[0] != KDirectFileStoreLayoutUid)
		{
		User::Leave(KErrCorrupt);
		}

	// Get the root stream and attempt to read the index from it
	TStreamId rootStreamId = store->Root() ;
	RStoreReadStream rootStream ;
	rootStream.OpenLC(*store, rootStreamId);
	// Internalize the repository
	InternalizeCreL(rootStream);
	CleanupStack::PopAndDestroy(&rootStream);
	CleanupStack::PopAndDestroy(store);
	}

/** Attempts to start a transaction.
Guaranteed to succeed (return KErrNone) for EConcurrentReadWriteTransaction mode only.
@param aTransactor transactor attempting to start transaction
@param aMode type of transaction to be started
@pre transactor is not in a transaction
@return KErrNone if the transaction is started, KErrLocked if read/write locks prevented that
type of transaction from starting now, and KErrArgument for invalid aMode.
@post On returning KErrNone, transaction is started and read/write locks are obtained for it
in the shared repository. Any other return: transaction has not started.
*/
TInt CSharedRepository::StartTransaction(CRepositoryTransactor& aTransactor, TInt aMode)
	{
	// session can only be in one transaction
	ASSERT(!aTransactor.IsInTransaction());
	switch (aMode)
		{
		case EConcurrentReadWriteTransaction:
			// can always start this type of transaction
			iNumActiveConcurrentReadWriteTransactions++;
			break;
		case EReadTransaction:
			// negative lock means there is an active EReadWriteTransaction
			if (iPessimisticTransactionLockCount < 0)
				{
				ASSERT(iPessimisticTransactionLockCount == -1); // sanity check
				return KErrLocked;
				}
			// when non-negative lock equals number of active EReadTransactions.
			iPessimisticTransactionLockCount++;
			break;
		case EReadWriteTransaction:
			// lock is zero if there are no active pessimistic transactions
			if (iPessimisticTransactionLockCount != 0)
				{
				return KErrLocked;
				}
			// lock value of -1 means the exclusive EReadWriteTransaction is active
			iPessimisticTransactionLockCount = -1;
			break;
		default:
			// not a valid transaction mode
			return KErrArgument;
		}
	aTransactor.AddToQueue(iTransactors, aMode);
	return KErrNone;
	}

/**	Commit transaction
@return KErrNone on success, or error code.
@param aKeyInfo 
	on success (return KErrNone): aKeyInfo returns number of modified settings;
	on failure (other error code): KUnspecifiedKey
@pre transactor is in a transaction.
@post transactor is not in a transaction
*/
TInt CSharedRepository::CommitTransaction(CRepositoryTransactor& aTransactor, TUint32& aKeyInfo)
	{
	// calling code should have panicked the client if not in a transaction
	ASSERT(aTransactor.IsInTransaction());
	TInt result = aTransactor.iTransactionResult;
	if (aTransactor.IsInFailedTransaction())
		{
		ASSERT(result != KErrNone);
		aKeyInfo = aTransactor.iTransactionErrorKey;
		}
	else
		{
		ASSERT(result == KErrNone);
		ASSERT(aTransactor.iTransactionErrorKey == KUnspecifiedKey);
		aKeyInfo = 0;
		// must release locks otherwise shared repository will not commit changes
		// failed transactions have already released their locks
		ReleaseTransactionLock(aTransactor);
		}
	
	// transactions that haven't made any changes can be closed at any time
	if (aTransactor.IsInActiveReadWriteTransaction() &&
		(aTransactor.iTransactionSettings.Count() > 0))
		{
		result = DoCommitTransactionSettings(aTransactor, aKeyInfo);
		}

	// transaction is complete - remove from queue
	aTransactor.Deque();

	return result;
	}

/**	Cancels the transaction, discarding changes.
@post Not in a transaction
*/
void CSharedRepository::CancelTransaction(CRepositoryTransactor& aTransactor)
	{
	if (aTransactor.IsInTransaction())
		{
		ReleaseTransactionLock(aTransactor);
		aTransactor.Deque();
		}
	}

TInt CSharedRepository::FailTransaction(CRepositoryTransactor& aTransactor, TInt aError, TUint32 aErrorKey)
	{
	ASSERT(aError != KErrNone); // must fail for a reason
	if (aTransactor.IsInActiveTransaction())
		{
		// locks cannot be removed from a failed transaction, so release before failing
		ReleaseTransactionLock(aTransactor);
		aTransactor.SetFailed(aError, aErrorKey);
		}
	return aError; // to allow "return FailTransaction(error, errorKey);" - error written once
	}

/** Fails all active transactions - except for the optional aExcludeTransactor, releasing locks.
All transactions are failed with reason "KErrLocked" meaning they are "locked out".
This should only be done to allow another agent to change values in the repository.
Beware that all concurrent read/write transactions that are failed with KErrLocked are
expected to retry the transactions straight afterwards - must be careful to allow their
retry strategy to be successful.
*/
void CSharedRepository::FailAllTransactions(const CRepositoryTransactor* aExcludeTransactor)
	{
		TDblQueIter<CRepositoryTransactor> transIter(iTransactors);
		CRepositoryTransactor* transactor;
		while ((transactor = transIter++) != NULL)
			{
			if (transactor != aExcludeTransactor)
				{
				FailTransaction(*transactor, KErrLocked, KUnspecifiedKey);
				}
			}
	}

/** must currently be in active Read transaction. Does not fail
transaction here if promotion to read/write failed.
@return KErrNone if promoted, KErrLocked if not
*/
TInt CSharedRepository::AttemptPromoteTransactionToReadWrite(CRepositoryTransactor& aTransactor)
	{
	// transactor should currently be in an active read transaction
	ASSERT(aTransactor.IsInActiveReadTransaction());
	// sanity check: must only be pessimistic reads active
	ASSERT(iPessimisticTransactionLockCount > 0);
	// can promote only if there are no other active read transactions:
	if (1 == iPessimisticTransactionLockCount)
		{
		// may only promote to exclusive read/write as it has the same commit semantics
		// as Read transaction: concurrent R/W must wait for reads to finish first.
		aTransactor.PromoteToExclusiveReadWrite();
		// lock value of -1 means the exclusive EReadWriteTransaction is active
		iPessimisticTransactionLockCount = -1;
		return KErrNone;
		}
	return KErrLocked;
	}

/** Private helper method which releases any read/write locks held in the shared repository
by this transactor. Caller must set transactor's state or remove from queue as appropriate.
@param aTransactor transactor whose read/write locks are to be released.
@post Any read/write locks held by transactor are released.
*/
void CSharedRepository::ReleaseTransactionLock(CRepositoryTransactor& aTransactor)
	{
	if (aTransactor.IsInActiveConcurrentReadWriteTransaction())
		{
		iNumActiveConcurrentReadWriteTransactions--;
		ASSERT(iNumActiveConcurrentReadWriteTransactions >= 0); // sanity check
		}
	else if (aTransactor.IsInActiveReadTransaction())
		{
		iPessimisticTransactionLockCount--;
		ASSERT(iPessimisticTransactionLockCount >= 0); // sanity check
		}
	else if (aTransactor.IsInActiveExclusiveReadWriteTransaction())
		{
		// can only be one exclusive read/write transaction active (lock value -1)
		ASSERT(iPessimisticTransactionLockCount == -1);
		iPessimisticTransactionLockCount = 0;
		}
	}
	
void CSharedRepository::ExternalizeCre(RWriteStream& aStream) const
	{
	aStream << TServerResources::iPersistsVersion;
	aStream << iUid ;
	aStream << iOwner ;
	
	TUint32 count=iSinglePolicies.Count();
	aStream << count;
	for(TInt i=0; i<count;i++)
		{
		aStream << *iSinglePolicies[i];
		}
	
	aStream << iRangePolicies ;
	aStream << iDefaultReadPolicy.Package() ;
	aStream << iDefaultWritePolicy.Package() ;
	
	aStream << iDefaultMeta ;
	aStream << iRangeMeta ;
	aStream << iTimeStamp.Int64() ;

	aStream << iSettings ;
	
	// Deleted settings
	count = iDeletedSettings.Count() ;
	aStream << count ;
	for (TInt i=0; i<count; i++)
		{
		aStream << iDeletedSettings[i];
		}
	}

void CSharedRepository::InternalizeCreL(RReadStream& aStream)
	{
	TUint8 version;
	aStream >> version;
	aStream >> iUid ;
	aStream >> iOwner ;
	
	TUint32 count;
	aStream >> count;
	for(TInt i=0; i<count;i++)
		{
		TSettingsAccessPolicy* singlePolicy = new(ELeave) TSettingsAccessPolicy;
		CleanupStack::PushL(singlePolicy);
		aStream >> *singlePolicy;
		iSinglePolicies.AppendL(singlePolicy);
		CleanupStack::Pop(singlePolicy);
		}
	
	iRangePolicies.Reset();		
	aStream >> iRangePolicies ;
	
	HBufC8* securityPolicyPackage ;
	securityPolicyPackage = HBufC8::NewLC(aStream, 10000) ;
	iDefaultReadPolicy.Set(securityPolicyPackage->Des()) ;
	CleanupStack::PopAndDestroy(securityPolicyPackage) ;
	securityPolicyPackage = HBufC8::NewLC(aStream, 10000) ;
	iDefaultWritePolicy.Set(securityPolicyPackage->Des()) ;
	CleanupStack::PopAndDestroy(securityPolicyPackage) ;
	
	iDefaultPolicy=TSettingsAccessPolicy(iDefaultReadPolicy,iDefaultWritePolicy, KUnspecifiedKey);
	
	aStream >> iDefaultMeta ;
	
	iRangeMeta.Reset();
	aStream >> iRangeMeta ;
	
	TInt64 timeStampInt ;
	aStream >> timeStampInt ;
	iTimeStamp = timeStampInt ;

	iSettings.Reset() ;
	aStream >> iSettings ;
	
	if (version >= KPersistFormatSupportsDeletedSettings)
		{
		// Deleted Settings 
		// Deleted settings
		aStream >> count  ;
		for (TInt i=0; i<count; i++)
			{
			TUint32 keyValue ;
			aStream >> keyValue ;
			iDeletedSettings.InsertInUnsignedKeyOrder(keyValue);
			}
		}
	// Set up access policies
	TInt32 numElements = iSettings.Count();
	for (TInt32 count = 0; count < numElements; count++)
		{
		TServerSetting* setting= &iSettings[count];
		TUint32 key=setting->Key();
		setting->SetAccessPolicy(GetFallbackAccessPolicy(key));
		}
	}
	
void CSharedRepository::SetAllDefaultMetaFlags()	
    {
    TInt y = iSettings.Count();
    for (TInt i=0; i < iSettings.Count(); i++)
        {
        TUint32 meta = iSettings.operator[](i).Meta();
        meta |= KMetaDefaultValue;
        iSettings.operator[](i).SetMeta(meta);
        }
    }
