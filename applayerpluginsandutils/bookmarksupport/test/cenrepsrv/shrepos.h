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

#ifndef SHREPOS_H
#define SHREPOS_H

#include <e32base.h>
#include "obsrvr.h"
#include "srvres.h"
#include "srvdefs.h"
#include "setting.h"
#include "clientrequest.h"
#include "panic.h"
#include "transstate.h"
#include "inifile.h"

typedef RPointerArray<TServerSetting> RSettingPointerArray;

enum TComparison {ENotEqual, EEqual};
enum TMergeType {EInternalizeMerge, ETransactionMerge,ERestoreMerge, ESWIUpgradeMerge, ESWIDowngradeMerge};

/** Uses private inheritance, delegating only those methods that are needed, to remove the possibility
of callers breaking invariants (such as non-repeating and strictly-increasing keys) by using
modifiers like Append directly.
*/
class RSettingsArray : private RArray<TServerSetting>
	{
public:
	inline RSettingsArray();
	inline ~RSettingsArray();
	inline TServerSetting* Find(TInt aKey) const;
	inline TInt FindIndex(const TServerSetting& aSetting) const;
	inline void OrderedInsertL(const TServerSetting& aEntry);
	inline TInt OrderedInsert(const TServerSetting& aEntry);
	inline TInt Count() const;
	inline const TServerSetting &operator[](TInt aIndex) const;
	inline TServerSetting &operator[](TInt aIndex);
	inline void Remove(TInt aIndex);
	inline void Reset();
	inline void AdoptL(RSettingsArray& aSrc);
	
	inline void WriteBackupStream (RWriteStream& aStream) const;
	inline void ExternalizeL(RWriteStream& aStream) const;
	inline void InternalizeL(RReadStream& aStream) ;

	TInt Find(TUint32 aPartialId, TUint32 aMask, RSettingPointerArray& aMatches) const;
	static TInt Merge(RSettingPointerArray& aDst, const RSettingPointerArray& aSrc);
	inline TBool IsDefault() const;
	inline void SetIsDefault(TBool aIsDefault);
#ifdef SYMBIAN_CENTRALREPOSITORY_PARANOID_CHECKING
	void ValidateInvariantProperties() const;
	static void ValidateInvariantProperties(const RSettingPointerArray& aPtrArray);
#endif
 	TInt MergeArray(RSettingsArray& aChanges, RArray<TUint32>& aDeletedSettings, TMergeType aMergeType, TBool aKeyOnly = EFalse);

private:
	enum {KGranularity=8};
	inline void DeleteElement(TInt aIndex);
	inline void RemoveElement(TInt aIndex);
	TBool iIsDefault;
	};
/**
@internalTechnology
This is a wrapper for TSecurityPolicies specified for a range of settings,
mian purpose is to provide nice search trough the range */
class RRangePolicyArray : public RArray<TSettingsAccessPolicy>
	{
public:
	inline RRangePolicyArray();
	inline ~RRangePolicyArray();
	inline TSettingsAccessPolicy* Find(TInt aKey) const;
	inline void OrderedInsertL(const TSettingsAccessPolicy& aEntry);

	inline void ExternalizeL(RWriteStream& aStream) const;
	inline void InternalizeL(RReadStream& aStream) ;
	};
	
/**
@internalTechnology
This is a wrapper for default metadata specified for ranges of settings,
mian purpose is to provide nice search trough the range */
class RDefaultMetaArray : public RArray<TSettingsDefaultMeta>
	{
public:
	inline RDefaultMetaArray();
	inline ~RDefaultMetaArray();
	inline TSettingsDefaultMeta* Find(TInt aKey) const;
	inline void OrderedInsertL(const TSettingsDefaultMeta& aEntry);
	inline void ExternalizeL(RWriteStream& aStream) const;
	inline void InternalizeL(RReadStream& aStream) ;
	};

/**
@internalTechnology
Base class for CServerRepository and other objects that may be in transactions with the
shared repository. Contains a double queue link so the shared repository can build a queue
of them. In order to use transactions in the shared repository, the object needs to supply a
CRepositoryTransactor - either itself or a member variable. This makes it possible to
prevent one transactor from closing another's transaction.
Stores transaction state and changes made during the transaction.
Also stores RMessage2 while pending async start or commit, and handles its completion.
*/
class CRepositoryTransactor : public CBase
	{
	// shared repository may set private transaction state, but not derived classes
	friend class CSharedRepository;
public:

	inline TInt TransactionState() const
		{
		return iTransactionState;
		}
	inline TInt TransactionMode() const
		{
		return iTransactionState & EAllTransactionModeBits;
		}
	inline TBool IsInTransaction() const
		{
		return iTransactionState != ENoTransaction;
		}
	inline TBool IsInActiveTransaction() const
		{
		// transaction is active if no bits apart from the (non-zero) mode are set
		return (iTransactionState != ENoTransaction) && (TransactionMode() == iTransactionState);
		}
	inline TBool IsInActiveReadTransaction() const
		{
		return iTransactionState == EReadTransaction;
		}
	inline TBool IsInActiveConcurrentReadWriteTransaction() const
		{
		return (iTransactionState == EConcurrentReadWriteTransaction);
		}
	inline TBool IsInActiveExclusiveReadWriteTransaction() const
		{
		return (iTransactionState == EReadWriteTransaction);
		}
	inline TBool IsInActiveReadWriteTransaction() const
		{
		return (iTransactionState == EReadWriteTransaction)
			|| (iTransactionState == EConcurrentReadWriteTransaction);
		}
	inline TBool IsInFailedTransaction() const
		{
		return (iTransactionState & EFailedBit) != 0;
		}

protected:
	inline void ClearSettings()
		{
		iTransactionSettings.Reset();
		}

private:
	inline void SetFailed(TInt aError, TUint32 aErrorKey)
		{
		// don't want to fail more than once
		ASSERT(IsInActiveTransaction());
		ASSERT(aError != KErrNone); // must fail for a reason
		iTransactionState |= EFailedBit;
		ClearSettings();
		iTransactionResult = aError;
		iTransactionErrorKey = aErrorKey;
		}
	inline void PromoteToExclusiveReadWrite()
		{
		ASSERT(iTransactionState == EReadTransaction);
		iTransactionState = EReadWriteTransaction;
		ClearSettings();
		}
	inline void AddToQueue(TDblQue<CRepositoryTransactor>& aTransactors, TInt aMode)
		{
		ASSERT(!IsInTransaction());
		// if the following ASSERT fails, transactor is already in a queue
		ASSERT(iLink.iNext == NULL);
		aTransactors.AddLast(*this);
		iTransactionState = aMode;
		// check adding to queue in an active state
		ASSERT(IsInActiveTransaction());
		ClearSettings();
		// clear failure reasons
		iTransactionResult = KErrNone;
		iTransactionErrorKey = KUnspecifiedKey;
		}
	inline void Deque()
		{
		ASSERT(IsInTransaction());
		// check transactor is in a queue
		ASSERT(iLink.iNext != NULL);
		iLink.Deque();
		iTransactionState = ENoTransaction;
		ClearSettings();
		}
protected:
	// derived classes are free to change transaction settings.
	// these can be made persistent by committing write transaction
	RSettingsArray iTransactionSettings;

private:
	// A queue link used by the CSharedRepository the CTransactor is working on
	TDblQueLink iLink;
	// integer encoding transaction state using definition of TTransactionState
	TInt iTransactionState;
	// result to be returned by commit if transaction failed earlier
	TInt iTransactionResult;
	// if transaction failed earlier, key or partial key involved in the original error,
	// or KUnspecifiedKey if could not be attributed to just one.
	TUint32 iTransactionErrorKey;
	};

class CSharedRepository : public CObservable
	{
	friend class CServerRepository;
	friend class TConvToolTester;

public:
	CSharedRepository(TUid aUid);
	~CSharedRepository();

	TUid Uid() const;
	TInt CommitChanges();
	void CommitChangesL()
		{
		User::LeaveIfError(CommitChanges());
		}

	TInt CreateL(TServerSetting& s, TSettingsAccessPolicy* &aPolicy, TBool aSingleMetaFound=EFalse);
	
	void SetMetaDataOnRead(TServerSetting& aSetting, TBool aSingleMetaFound);
	
	void SetMetaDataOnCreate(TServerSetting& newSetting, TUint32 aMeta)
		{
		if(aMeta)
			{
			newSetting.SetMeta(aMeta);
			}
		else
			{
			// No metadata specified. First check for a matching "range" default
			// metadata setting
			TSettingsDefaultMeta *defaultMeta = iRangeMeta.Find(newSetting.Key());
			if (defaultMeta)
				{
				newSetting.SetMeta(defaultMeta->GetDefaultMetadata());
				}
			else
				{
				// Range value not found, try for a repository default
				newSetting.SetMeta(iDefaultMeta);
				}
			}
		}

	TInt DeleteAndPersist(TUint32 aId);
	TInt DeleteNoPersist(TUint32 aId);

	TInt ResetNoPersistL(TServerSetting& aSetting);
	void ResetAndPersistL(TServerSetting& aSetting);
	TInt ResetAllNoPersistL(CSharedRepository& aRepository);
	
	const TSecurityPolicy& GetFallbackReadAccessPolicy(TUint32 aId);
	inline const TSecurityPolicy& GetReadAccessPolicy(const TServerSetting& aSetting);
	inline const TSecurityPolicy& GetReadAccessPolicy(TUint32 aId);
	inline const TSecurityPolicy& GetDefaultReadAccessPolicy();

	const TSecurityPolicy& GetFallbackWriteAccessPolicy(TUint32 aId);
	inline const TSecurityPolicy& GetWriteAccessPolicy(const TServerSetting& aSetting);
	inline const TSecurityPolicy& GetWriteAccessPolicy(TUint32 aId);
	inline const TSecurityPolicy& GetDefaultWriteAccessPolicy();

	TSettingsAccessPolicy* GetFallbackAccessPolicy(TUint32 aId);
															
	inline RPointerArray<TSettingsAccessPolicy>& SinglePolicyArray();
	
	void CheckPolicyL(const RMessage2& msg,const TSecurityPolicy& aPolicy,
					const char *aDiagnostic);

	static TBool SinglePolicyMatchOnKey(TSettingsAccessPolicy const &aSinglePolicy, TSettingsAccessPolicy const &aSinglePolicyIndexItem);
	static TBool CompareKeyIds(TSettingsAccessPolicy const &aSinglePolicy, TSettingsAccessPolicy const &aSinglePolicyIndexItem);

	TInt StartTransaction(CRepositoryTransactor& aTransactor, TInt aMode);
	TInt CommitTransaction(CRepositoryTransactor& aTransactor, TUint32& aKeyInfo);
	void CancelTransaction(CRepositoryTransactor& aTransactor);
	TInt FailTransaction(CRepositoryTransactor& aTransactor, TInt aError, TUint32 aErrorKey);
	void FailAllTransactions(const CRepositoryTransactor* aExcludeTransactor);
	TInt AttemptPromoteTransactionToReadWrite(CRepositoryTransactor& aTransactor);
	inline TBool IsTransactionActive();

	void	MergeL(CSharedRepository& aInstallRep, TMergeType aMergeType);
	
	TInt FindLocationForFileL(TCentRepLocation& aLocation,TUid aUid,const TCentRepFileType aType) const;
	
	TInt	InstallFileTimeStampL(TTime& aInstallFileTimeStamp);

	void	HandleUpdateMergeL(TTime aInstallFileTimeStamp, CSharedRepository& aInstallRep);
	void	HandleDeleteMergeL(CSharedRepository& aRomRep);
	TTime	InstallTime();
	void	SetInstallTime(TTime aInstallTime);

	inline TUid Owner() {return iOwner ;} ;
	inline void WriteBackupStream(RWriteStream& aStream) const;
	inline void WriteDeletedSettingsStream(RWriteStream& aStream) const;
	inline void ExternalizeL(RWriteStream& aStream) const;
	inline void InternalizeL(RReadStream& aStream) ;
	void ExternalizeCre(RWriteStream& aStream) const;
	void InternalizeCreL(RReadStream& aStream) ;
	TInt ReloadContentL(CIniFileIn& aIniFile);
	void DoCommitChangesToIniFileL(const TDesC& aOutFileName);
	inline void SetSize(TInt aSize);
	inline TInt Size() const;

private:
	void 	DoCommitChangesL();
	void	ResetContent();
	void	RestoreConsistencyL();
	void	DoRestoreConsistencyL();
	TInt	ReadSettingSavePolicyL(CIniFileIn& aFile,TServerSetting& aSetting, TSettingsAccessPolicy* &aPolicy, TBool& aSingleMetaFound);
	TInt	CreateRepositoryFromCreFileL(TCentRepLocation aLocation);
	void	CreateRepositoryFromCreFileL(TDesC& aFilePath);

	void ReleaseTransactionLock(CRepositoryTransactor& aTransactor);
	TInt DoCommitTransactionSettings(CRepositoryTransactor& aTransactor, TUint32& aKeyInfo);
	void SetAllDefaultMetaFlags();
	
private:
	enum {KGranularity=8};
	//RSettingsArray<TServerSetting> iSettings;
	RSettingsArray iSettings;
	TUid iUid;
	TUid iOwner;
	
	TTime iTimeStamp ;
	//
	RPointerArray<TSettingsAccessPolicy> iSinglePolicies;
	RRangePolicyArray iRangePolicies;
	//
	TSecurityPolicy iDefaultReadPolicy;
	TSecurityPolicy iDefaultWritePolicy;
	
	TSettingsAccessPolicy iDefaultPolicy;
	TUint32 iDefaultMeta ;
	RDefaultMetaArray iRangeMeta ;
	TBool iPersistsIniFileExists;

	// Keep track of deleted keys so that deletions can be propagated across
	// a Backup/Restore cycle when backup data is merged with initialisation
	// file during restore process.
	RArray<TUint32> iDeletedSettings ;

	// all transactors in the order added
	TDblQue<CRepositoryTransactor> iTransactors;
	// combined read/write lock for pessimistic transactions.
	// -1 if in EReadWriteTransaction, otherwise non-negative number of EReadTransactions.
	TInt iPessimisticTransactionLockCount;
	// count of active concurrent read/write transactions:
	TInt iNumActiveConcurrentReadWriteTransactions;

	//iInconsistentData data flag will be set at the beginning of CommitChangesL()
	//and reset at the end of it. The flag is used to implement lazy-load approach for the
	//repository - if CommitChangesL() fails, the in-memory representation of the repository 
	//won't match the updated by CommitChangesL() repository (ini) file. The repository
	//consistency has to be restored before any next call of CSharedRepository methods.
	TBool iInconsistentData;
	
	TInt32 iSize; //approximate size(in memory) of the repository after internalisation
	};

#include "shrepos.inl"

#endif // SHREPOS_H
