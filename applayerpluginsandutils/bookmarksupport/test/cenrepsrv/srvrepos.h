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

#ifndef SRVREPOS_H
#define SRVREPOS_H

#include <e32base.h>
#include <s32file.h>
#include "shrepos.h"
#include "inifile.h"
#include "setting.h"
#include "clientrequest.h"
#include "sessnotf.h"

class MObserver;
class CIniFileIn;

class CServerRepository : public CRepositoryTransactor
	{
public:
	~CServerRepository();
	
	void OpenL(TUid aUid, MObserver& aObserver, TBool aFailIfNotFound = ETrue);
	void HandleOpenMergeL();
	void Close(MObserver& aObserver);
	void RestoreConsistencyL();
	TInt ResetRepositoryL();
	static void ResetRepositoriesL();

	TInt ReadIniFileL(CSharedRepository*& aRepository,TCentRepLocation aLocation);

	inline TInt StartTransaction(TInt aMode)
		{
		return iRepository->StartTransaction(*this, aMode);
		}
	inline TInt CommitTransaction(TUint32& aKeyInfo)
		{
		return iRepository->CommitTransaction(*this, aKeyInfo);
		}
	inline void CancelTransaction() // serves as both rollback and async cancel
		{
		iRepository->CancelTransaction(*this);
		}
	void CleanupCancelTransactionPushL();
	inline TInt FailTransaction(TInt aError, TUint32 aErrorKey)
		{
		// returns aError to allow "return FailTransaction(error, errorKey);" - error written once
		return iRepository->FailTransaction(*this, aError, aErrorKey);
		}

	inline void FailAllTransactions()
		{
		iRepository->FailAllTransactions(NULL);
		}
	/** must currently be in active Read transaction. Does not fail
	transaction here if promotion to read/write failed.
	@return KErrNone if promoted, KErrLocked if not
	*/
	inline TInt AttemptPromoteTransactionToReadWrite()
		{
		ASSERT(IsInActiveReadTransaction());
		return iRepository->AttemptPromoteTransactionToReadWrite(*this);
		}

	inline TServerSetting* GetPersistentSetting(TUint32 aId)
		{
		return iRepository->iSettings.Find(aId);
		}
	inline TServerSetting* GetTransactionSetting(TUint32 aId)
		{
		return iTransactionSettings.Find(aId);
		}
	inline TServerSetting* GetSetting(TUint32 aId)
		{
		// try to be most efficient when no transaction changes
		if ((iTransactionSettings.Count() > 0) && IsInActiveReadWriteTransaction())
			{
			TServerSetting* s = GetTransactionSetting(aId);
			if (s)
				{
				return (s);
				}
			}
		return GetPersistentSetting(aId);
		}

	template <class T> 
	TInt TransactionCreateL(TUint32 aKey, const T& aVal, TUint32 aMeta);

	inline TInt TransactionDeleteL(TUint32 aId)
		{
		// all write operations now done in a transaction
		ASSERT(IsInActiveReadWriteTransaction());
		TServerSetting* ts=GetTransactionSetting(aId);
		if (ts)
			{
			//If already marked as Deleted just return KErrNotFound
			if (ts->IsDeleted())
				return KErrNotFound;
			//otherwise reset and mark as delete
			else
				{
				ts->Reset();
				ts->SetDeleted();	
				}
			}
		else
			{
			//cannot find transaction settings, go find persistent settings
			TServerSetting* ps=GetPersistentSetting(aId);
			//if also cannot find persistent settings just return KErrNotFound
			if (!ps)
				return KErrNotFound;
			// Create a placeholder and set as deleted
			TServerSetting newSetting(aId);
			newSetting.SetDeleted();
			iTransactionSettings.OrderedInsertL(newSetting);
			}
		return KErrNone;
		}

	// note this is non-const ONLY to use non-const helper function GetSetting
	template <class T>
	inline TInt Get(TUint32 aId, T& aVal)
		{
		const TServerSetting* s = GetSetting(aId);
		if (!s || s->IsDeleted())
			{
			return KErrNotFound;
			}
		return s->AssignValueTo(aVal);
		}

	template <class T>
	TInt TransactionSetL(TUint32 aKey, const T& aVal);
	
	TInt CheckReadPermissions(RSettingPointerArray& aSettings, const TClientRequest& aMessage, const char *aDiagnostic);

	TInt TransactionDeleteRangeL(const TClientRequest& aMessage, TUint32& aErrId);
	TInt TransactionMoveL(const TClientRequest& aMessage, TUint32& aErrorKey);

	inline TInt FindPersistentSettings(TUint32 aPartialId, TUint32 aIdMask, RSettingPointerArray& aMatches) const
		{
		// guarantees to reset RSettingPointerArray in case of error
		return iRepository->iSettings.Find(aPartialId, aIdMask, aMatches);
		}
	inline TInt FindTransactionSettings(TUint32 aPartialId, TUint32 aIdMask, RSettingPointerArray& aMatches) const
		{
		// guarantees to reset RSettingPointerArray in case of error
		return iTransactionSettings.Find(aPartialId, aIdMask, aMatches);
		}
	TInt FindSettings(TUint32 aPartialId, TUint32 aIdMask, RSettingPointerArray& aMatches) const;

	/** Fills aFindResult with the list of keys in aSettings matching or not matching the supplied
	value, checking read policies as it goes. If any read policy is not satisfied, fails and returns
	KErrPermissionDenied, otherwise KErrNone.
	guarantees to reset aFindResult in case of error
	IMPORTANT PLATSEC NOTE:
	This method is called by the FindEqL and FindNeqL methods.
	MUST return KErrPermissionDenied if read policy of ANY setting in the search range not passed.
	MUST NOT merely check read policy of matching entries, otherwise it is possible to determine
	secret values by brute force: Using single-value ranges, cycling through the possible values and
	confirming a match when it returns KErrPermissionDenied rather than KErrNotFound.
	*/
	template <class T>
	inline TInt MakeComparisonFindResultsCheckingReadPolicies(const RSettingPointerArray& aSettings,
		const TClientRequest& aMessage, const char *aDiagnostic, const T& aVal, TComparison aEqual, RArray<TUint32>& aFindResult) const
		{
		aFindResult.Reset();
		const TInt numSettings = aSettings.Count();
		TInt error = KErrNone;
		for (TInt i = 0; i < numSettings; i++)
			{
			ASSERT(aSettings[i]);
			const TServerSetting& setting = *(aSettings[i]);
			if (!aMessage.CheckPolicy(GetReadAccessPolicy(setting), aDiagnostic))
				{
				aFindResult.Reset();
				return KErrPermissionDenied;
				}
			else
				{
				ASSERT(!setting.IsDeleted());
				if(aEqual && setting==aVal || !aEqual && setting!=aVal)
					{
					error = aFindResult.Append(setting.Key());
					if (error != KErrNone)
						{
						aFindResult.Reset();
						return error;
						}
					}
				}
			}
		return error;
		}

	inline void CommitChangesL() const
		{
		iRepository->CommitChangesL();
		}

	TInt ResetL(TUint32 aId);
	TInt ResetAllL();

	inline const TSecurityPolicy& GetFallbackReadAccessPolicy(TUint32 aId) const
		{
		return iRepository->GetFallbackReadAccessPolicy(aId);
		}
	inline const TSecurityPolicy& GetReadAccessPolicy(const TServerSetting& aSetting) const
		{
		return iRepository->GetReadAccessPolicy(aSetting);
		}
	inline const TSecurityPolicy& GetReadAccessPolicy(TUint32 aId) const
		{
		return iRepository->GetReadAccessPolicy(aId);
		}
	inline const TSecurityPolicy& GetDefaultReadAccessPolicy() const
		{
		return iRepository->GetDefaultReadAccessPolicy();
		}

	inline const TSecurityPolicy& GetFallbackWriteAccessPolicy(TUint32 aId) const
		{
		return iRepository->GetFallbackWriteAccessPolicy(aId);
		}
	inline TSettingsAccessPolicy* GetFallbackAccessPolicy(TUint32 aId) const
		{
		return iRepository->GetFallbackAccessPolicy(aId);
		}
	inline const TSecurityPolicy& GetWriteAccessPolicy(const TServerSetting& aSetting) const
		{
		return iRepository->GetWriteAccessPolicy(aSetting);
		}
	inline const TSecurityPolicy& GetWriteAccessPolicy(TUint32 aId) const
		{
		return iRepository->GetWriteAccessPolicy(aId);
		}
	inline const TSecurityPolicy& GetDefaultWriteAccessPolicy() const
		{
		return iRepository->GetDefaultWriteAccessPolicy();
		}

	void HandleSWIUpdateL(TUid aUid, TTime aModified, CSessionNotifier& aNotifier);
	void HandleSWIDeleteL(TUid aUid, CSessionNotifier& aNotifier);

	inline TUid Owner(){return iRepository->Owner();} ;

	void StoreRepositoryContentsL(CStreamStore & aStore, TStreamId & aSettingStreamId, TStreamId & aDeletedSettingsStreamId) const;
	void RestoreRepositoryContentsL(CStreamStore& aStore, TStreamId aSettingStreamId, TStreamId aDeletedSettingsStreamId = KNullStreamId);
	void ResetFromIniFileL(TUint32 aId,TCentRepLocation aLocation,TBool& aKeyFound);
	
	static inline void RemoveRepository(CSharedRepository* aRepository)
		{
		TInt index = iOpenRepositories.Find(aRepository);
		
		if (index>=0)		
			{
			iOpenRepositories.Remove(index);
			}
		}

#ifdef CACHE_OOM_TESTABILITY
	static void CloseiOpenRepositories() 
		{
		iOpenRepositories.Close();
		}
	TInt SizeiRepository()
		{
		return iRepository->Size();
		}
	TBool iTrapOOMOnOpen;
#endif	
private:
	void LoadRepositoryLC(TUid aUid, TBool aFailIfNotFound, CSharedRepository*& aRepository, CIniFileIn::TIniFileOpenMode aIniFileOpenMode);
	TInt CreateRepositoryL(CSharedRepository*& aRepository, TCentRepLocation aLocation);
	TInt CreateRepositoryL(CSharedRepository*& aRepository, CIniFileIn::TIniFileOpenMode aIniFileOpenMode);
	TInt FindRepository(TUid aUid) const;
	TInt ReadSettingsL(CIniFileIn* aIniFileIn, CSharedRepository* aRep);
	template <class T> TInt CreateL(TUint32 aKey, const T& aVal, TUint32 aMeta);	
private:
	static RPointerArray<CSharedRepository> iOpenRepositories;
	CSharedRepository* iRepository;
	};

template <class T>
TInt CServerRepository::TransactionSetL(TUint32 aKey, const T& aVal)
	{
	// all write operations now done in a transaction
	ASSERT(IsInActiveReadWriteTransaction());
	TServerSetting* s = iTransactionSettings.Find(aKey);		
	TServerSetting* orig_s = s ? NULL : iRepository->iSettings.Find(aKey);
	if (!s && !orig_s)
		{
		return CreateL(aKey, aVal, 0);
		}
	TInt error = KErrNone;
    if (s && s->IsDeleted())
    	{
 		// replace the deleted entry with the new values
		s->CopyValueL(aVal);
		iRepository->SetMetaDataOnCreate(*s, 0);
		s->SetAccessPolicy(GetFallbackAccessPolicy(aKey));
		}		   
	else if (s)
		{
		// replace value of setting already in transaction - fail if type is different
		error = s->AssignValueFromL(aVal);
		s->SetMeta(s->Meta() & (~KMetaDefaultValue));
		}
	else
		{
		ASSERT(orig_s);
		// insert a setting with the new value (must be same type) into the transaction settings
		if (!orig_s->IsType(aVal))
			{
			error = KErrArgument;
			}
		else
			{
			error = CreateL(aKey, aVal, orig_s->Meta() & (~KMetaDefaultValue));
			}
		}

	if (error != KErrNone)
		{
		FailTransaction(error, aKey);
		}
	return error;
	}

template <class T>
TInt CServerRepository::TransactionCreateL(TUint32 aKey, const T& aVal, TUint32 aMeta)
	{
	// all write operations now done in a transaction
	ASSERT(IsInActiveReadWriteTransaction());
	TServerSetting* s = GetSetting(aKey);
	if (s && !s->IsDeleted())
		{
		return FailTransaction(KErrAlreadyExists, aKey);
		}
	if (s)
		{
		// replace the deleted entry with the new values
		s->CopyValueL(aVal);
		iRepository->SetMetaDataOnCreate(*s, aMeta);
		s->SetAccessPolicy(GetFallbackAccessPolicy(aKey));
		}
	else
		{
		CreateL(aKey, aVal, aMeta);
		}
	return KErrNone;
	}

template <class T>
TInt CServerRepository::CreateL(TUint32 aKey, const T& aVal, TUint32 aMeta)
	{
	TServerSetting newSetting(aKey);
	newSetting.CopyValueL(aVal);
	iRepository->SetMetaDataOnCreate(newSetting, aMeta);
	newSetting.SetAccessPolicy(GetFallbackAccessPolicy(aKey));
	newSetting.PushL(); // only needed for strings
	iTransactionSettings.OrderedInsertL(newSetting);
	newSetting.Pop();	// only needed for strings

	return KErrNone;
	}

#endif // SRVREPOS_H


