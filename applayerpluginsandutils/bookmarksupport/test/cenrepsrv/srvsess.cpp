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
#include "sessnotf.h"
#include "srvreqs.h"
#include "srvsess.h"
#include "cachemgr.h"
#include <centralrepository.h>

#include "backup.h"

using namespace NCentralRepositoryConstants;

#ifdef __CENTREP_SERVER_PERFTEST__
// Function EncodeEventAndData
// Put event Id in upper 8 bit and data in lower 24 bit.
inline
TUint32 EncodeEventAndData(TUint aEventId, TUint32 aData)
	{
	return ((aEventId & KEventIdMask) << KEventIdShiftBits) |
		   (aData & KDataMask);
	}
#endif //__CENTREP_SERVER_PERFTEST__

inline CSessionManager& CServerSession::Server()
	{return *static_cast<CSessionManager*>(const_cast<CServer2*>(CSession2::Server()));}

CServerSession::~CServerSession()
	{
	iFindResult.Close();

#ifdef __CENTREP_SERVER_PERFTEST__
	DoEventStart();
#endif

	iRepository.Close(iNotifier);

#ifdef __CENTREP_SERVER_PERFTEST__
	if (iInitialised)
		{
		DoEventEnd(ESessionClose);
		}
#endif
	}

// if ServiceL Leaves, execution resumes in this method.
// this allows us to panic clients using bad descriptors, to deal with OOM conditions
// and to fail transactions with the correct reason: OOM etc.
void CServerSession::ServiceError(const RMessage2 &aMessage, TInt aError)
	{
	// ensure any transaction is failed for the reason aError
	iRepository.FailTransaction(aError, KUnspecifiedKey);
	CSession2::ServiceError(aMessage, aError);
	}

void CServerSession::ServiceL(const RMessage2& aMessage)
	{
	const TClientRequest msg(aMessage);
	
	struct SAction
		{
		TServerRequest req;
		TInt (CServerSession::*groupFuncL)(const TClientRequest&, TServerFunc);
		TServerFunc funcL;
		};
		
	static const SAction actionTable[] =
		{
			{EInitialise,				&CServerSession::GeneralOperationsL,	&CServerSession::InitialiseL},
			{ECreateInt,				&CServerSession::WriteOperationsL,		&CServerSession::CreateIntL},
			{ECreateReal,				&CServerSession::WriteOperationsL,		&CServerSession::CreateRealL},
			{ECreateString,				&CServerSession::WriteOperationsL,		&CServerSession::CreateStringL},
			{EDelete,					&CServerSession::WriteOperationsL,		&CServerSession::DeleteL},
			{EGetInt,					&CServerSession::ReadOperationsL,		&CServerSession::GetIntL},
			{ESetInt,					&CServerSession::WriteOperationsL,		&CServerSession::SetIntL},
			{EGetReal,					&CServerSession::ReadOperationsL,		&CServerSession::GetRealL},
			{ESetReal,					&CServerSession::WriteOperationsL,		&CServerSession::SetRealL},
			{EGetString,				&CServerSession::ReadOperationsL,		&CServerSession::GetStringL},
			{ESetString,				&CServerSession::WriteOperationsL,		&CServerSession::SetStringL},
			{EFind,						&CServerSession::ReadOperationsL,		&CServerSession::FindL},
			{EFindEqInt,				&CServerSession::ReadOperationsL,		&CServerSession::FindEqIntL},
			{EFindEqReal,				&CServerSession::ReadOperationsL,		&CServerSession::FindEqRealL},
			{EFindEqString,				&CServerSession::ReadOperationsL,		&CServerSession::FindEqStringL},
			{EFindNeqInt,				&CServerSession::ReadOperationsL,		&CServerSession::FindNeqIntL},
			{EFindNeqReal,				&CServerSession::ReadOperationsL,		&CServerSession::FindNeqRealL},
			{EFindNeqString,			&CServerSession::ReadOperationsL,		&CServerSession::FindNeqStringL},
			{EGetFindResult,			&CServerSession::ReadOperationsL,		&CServerSession::GetFindResultL},
			{ENotifyRequestCheck,		&CServerSession::GeneralOperationsL,	&CServerSession::NotifyRequestCheck},
			{ENotifyRequest,			&CServerSession::GeneralOperationsL,	&CServerSession::NotifyRequest},
			{ENotifyCancel,				&CServerSession::GeneralOperationsL,	&CServerSession::NotifyCancel},
			{ENotifyCancelAll,			&CServerSession::GeneralOperationsL,	&CServerSession::NotifyCancelAll},
			{EGroupNotifyRequest,		&CServerSession::GeneralOperationsL,	&CServerSession::GroupNotifyRequest},
			{EGroupNotifyCancel,		&CServerSession::GeneralOperationsL,	&CServerSession::GroupNotifyCancel},
			{EReset,					&CServerSession::ResetOperationsL,		&CServerSession::ResetL},
			{EResetAll,					&CServerSession::ResetOperationsL,		&CServerSession::ResetAllL},
			{ETransactionStart,			&CServerSession::GeneralOperationsL,	&CServerSession::TransactionStart},
  			{ETransactionCommit,		&CServerSession::GeneralOperationsL,	&CServerSession::TransactionCommitL},
  			{ETransactionCancel,		&CServerSession::GeneralOperationsL,	&CServerSession::TransactionCancel},
			{EMove,						&CServerSession::WriteOperationsL,		&CServerSession::MoveL},
			{ETransactionState,			&CServerSession::GeneralOperationsL,	&CServerSession::TransactionStateL},
			{ETransactionFail,			&CServerSession::GeneralOperationsL,	&CServerSession::TransactionFail},
			{EDeleteRange,				&CServerSession::WriteOperationsL,		&CServerSession::DeleteRangeL}
		};
	 
	TInt r;
	TServerRequest fn = static_cast<TServerRequest>(aMessage.Function());

#if defined(__CENTREP_SERVER_PERFTEST__) || defined(__CENTREP_SERVER_MEMTEST__) || defined(__CENTREP_SERVER_CACHETEST__)
	if (fn == EGetSetParameters)
		{
		r = GetSetParameters(msg);
		aMessage.Complete(r);
		return;
		}
#endif
#ifdef __CENTREP_SERVER_PERFTEST__
    DoEventStart();
	if (EInitialise == fn)
		{
		iRepositoryUid = aMessage.Int0();
		}
#endif

	__ASSERT_ALWAYS(iInitialised || fn==EInitialise,
		PanicClient(ESessionNotInitialised, msg));

	if(fn>ELastInTable)
		{
		r = KErrNotSupported; // or panic client??
		}
	else
		{
		iRepository.RestoreConsistencyL();
		// plus need to check we are initialised
		r = (this->*actionTable[fn].groupFuncL)(aMessage, actionTable[fn].funcL);
		}
			
#ifdef __CENTREP_SERVER_PERFTEST__
	DoEventEnd(fn);
#endif

	if (r != KDontComplete)
		{
		aMessage.Complete(r);
		}
	}

//method allows transactions and notify requests to get through during backup/restore
//process to prevent them from being blocked.
//Transactions being blocked would lead clients to be panicked
//if they were trying to open a second transaction in the same session. 
// Notify cancels must be allowed through because they must always succeed.
TInt CServerSession::GeneralOperationsL(const TClientRequest& aMessage, TServerFunc aFuncL)
	{
	ASSERT(aFuncL != NULL);
	return (this->*aFuncL)(aMessage);
	}

// method allows read operations to share transaction-related tasks.
// Read operations are allowed only during backup process. 
// During restore it fails transactions with KErrLocked and returns KErrAbort
// and if it is a standalone read oparation it rejects it with KErrServerBusy.
TInt CServerSession::ReadOperationsL(const TClientRequest& aMessage, TServerFunc aFuncL)
	{
	TInt backupStatus = CRepositoryBackupClient::GetBackupStatus();
		
	if (iRepository.IsInFailedTransaction())
		{
		return KErrAbort;
		}
	else if (iRepository.IsInTransaction() && (backupStatus == ERestoreInProgress) )
	    {
	    iRepository.FailTransaction(KErrLocked,KUnspecifiedKey);
	    return KErrAbort;
	    }
	else if (backupStatus == ERestoreInProgress)
	    {
	    return KErrServerBusy;
	    }

	ASSERT(aFuncL != NULL);
	return (this->*aFuncL)(aMessage);
	}

// method allows write operations to share transaction-related tasks
// All write operations are not allowed either during backup or restore process.
// If backup or restore is in progress it fails transaction with KErrLocked, 
// returns KErrAbort or if it is a standalone operation it returns KErrServerBusy. 
TInt CServerSession::WriteOperationsL(const TClientRequest& aMessage, TServerFunc aFuncL)
	{
	TInt backupStatus = CRepositoryBackupClient::GetBackupStatus();
	
	if (iRepository.IsInFailedTransaction())
		{
		return KErrAbort;
		}
	else if (iRepository.IsInTransaction() && (backupStatus != ENoBackupActivty) )
	    {
	    iRepository.FailTransaction(KErrLocked,KUnspecifiedKey);
	    return KErrAbort;
	    }
    else if (CRepositoryBackupClient::GetBackupStatus() != ENoBackupActivty)
        {
        return KErrServerBusy;
        }
    
	// if not already in a transaction, create a temporary concurrent read/write transaction
	const TBool tempTransaction = !iRepository.IsInTransaction();
	if (tempTransaction)
		{
		// concurrent read/write transaction is guaranteed to start
		iRepository.StartTransaction(EConcurrentReadWriteTransaction);
		// to protect against Leaves:
		iRepository.CleanupCancelTransactionPushL();
		}
	else
		{
		if (iRepository.IsInActiveReadTransaction())
			{
			// must be a read/write transaction to continue
			iRepository.AttemptPromoteTransactionToReadWrite();
			// Note we don't check the return value of the above and return it here.
			// Instead we call the next level write function and expect it to have the
			// following code:
			//	if (iRepository.IsInActiveReadTransaction())
			//		{
			//		return iRepository.FailTransaction(KErrLocked, key);
			//		}
			// this ensures CommitTransaction reports the failing key. 
			}
		// Note ServiceError will fail the transaction if write operation leaves
		}

	// call the server function
	ASSERT(aFuncL != NULL);
	TInt result = (this->*aFuncL)(aMessage);
	
	// commit the temporary transaction
	if (tempTransaction)
		{
		CleanupStack::Pop(); // remove cleanup item from earlier
		// absorb result and keyInfo from commit of temporary transaction
		TUint32 tempKeyInfo;
		User::LeaveIfError(iRepository.CommitTransaction(tempKeyInfo));
		}
	return result;
	}

TInt CServerSession::ResetOperationsL(const TClientRequest& aMessage, TServerFunc aFuncL)
	{
	// reset operations are not currently supported in transactions
	if (iRepository.IsInTransaction())
		{
		// fail transaction otherwise client may be misled to believe operation worked
		return iRepository.FailTransaction(KErrNotSupported, KUnspecifiedKey);
		}
   //can't reset when backup or restore is in progress
   else if (CRepositoryBackupClient::GetBackupStatus() != ENoBackupActivty)
        {
        return KErrServerBusy;
        }		
	ASSERT(aFuncL != NULL);
	return (this->*aFuncL)(aMessage);
	}

TInt CServerSession::InitialiseL(const TClientRequest& aMessage)
	{
	__ASSERT_ALWAYS(!iInitialised,
		PanicClient(ESessionAlreadyInitialised, aMessage));
	// We let anyone to open a repository... 
	// it's not considered a breach of security to let people know
	// that a repository is there... 
	TUid repositoryUid = TUid::Uid(aMessage.Int0());
	iRepository.OpenL(repositoryUid, iNotifier);

	iInitialised = ETrue;

	return KErrNone;
	}

TInt CServerSession::CreateIntL(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();

	// cannot make changes in a read transaction - upgrade must have failed due to write lock being used
	if (iRepository.IsInActiveReadTransaction())
		{
		return iRepository.FailTransaction(KErrLocked, key);
		}

	if(KErrNone !=  CheckPolicy(aMessage,iRepository.GetWriteAccessPolicy(key),
			__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::CreateIntL - Attempt made to create a setting")))
		return iRepository.FailTransaction(KErrPermissionDenied, key);

	TInt val = aMessage.Int1();
		
	TInt r = iRepository.TransactionCreateL(key, val, 0);

	return r;
	}

TInt CServerSession::CreateRealL(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();

	// cannot make changes in a read transaction - upgrade must have failed due to write lock being used
	if (iRepository.IsInActiveReadTransaction())
		{
		return iRepository.FailTransaction(KErrLocked, key);
		}

	if(KErrNone != CheckPolicy(aMessage,iRepository.GetWriteAccessPolicy(key),
			__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::CreateRealL - Attempt made to create a setting")))
		return iRepository.FailTransaction(KErrPermissionDenied, key);

	TReal val;
	TPckg<TReal> p(val);
	aMessage.ReadL(1, p);
	
	TInt r = iRepository.TransactionCreateL(key, val, 0);

	return r;
	}
	
TInt CServerSession::CreateStringL(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();

	// cannot make changes in a read transaction - upgrade must have failed due to write lock being used
	if (iRepository.IsInActiveReadTransaction())
		{
		return iRepository.FailTransaction(KErrLocked, key);
		}

	// sometime: must ensure bad descriptor results in client being panic'd

	// check for descriptor-too-long was previously on the client side,
	// hence test code expects KErrArgument response before KErrPermissionDenied
	TInt length = aMessage.GetDesLengthL(1);
	if (length > KMaxBinaryLength)
		{
		return iRepository.FailTransaction(KErrArgument, key);
		}

	if(KErrNone != CheckPolicy(aMessage,iRepository.GetWriteAccessPolicy(key),
			__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::CreateStringL - Attempt made to create a setting")))
		return iRepository.FailTransaction(KErrPermissionDenied, key);

	TBuf8<KMaxBinaryLength> val;
	aMessage.ReadL(1, val);
	
	TInt error = iRepository.TransactionCreateL(key, val, 0);

	return error;
	}

TInt CServerSession::DeleteL(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();

	// cannot make changes in a read transaction - upgrade must have failed due to write lock being used
	if (iRepository.IsInActiveReadTransaction())
		{
		return iRepository.FailTransaction(KErrLocked, key);
		}

	if(KErrNone != CheckPolicy(aMessage,iRepository.GetWriteAccessPolicy(key),
			__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::DeleteL - Attempt made to delete a setting")))
		return iRepository.FailTransaction(KErrPermissionDenied, key);

	TInt r = iRepository.TransactionDeleteL(key);

	return r;
	}

TInt CServerSession::GetIntL(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();

	if(KErrNone != CheckPolicy(aMessage,iRepository.GetReadAccessPolicy(key),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::GetIntL - Attempt made to read a setting")))
		return KErrPermissionDenied;

	TInt val;
	TInt error = iRepository.Get(key, val);

	if (error == KErrNone)
		{
		TPckg<TInt> p(val);
		aMessage.WriteL(1, p);
		}

	return error;
	}

TInt CServerSession::SetIntL(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();

	// cannot make changes in a read transaction - upgrade must have failed due to write lock being used
	if (iRepository.IsInActiveReadTransaction())
		{
		return iRepository.FailTransaction(KErrLocked, key);
		}
	
	if(KErrNone != CheckPolicy(aMessage,iRepository.GetWriteAccessPolicy(key),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::SetIntL - Attempt made to write a setting")))
		return iRepository.FailTransaction(KErrPermissionDenied, key);
	
	TInt val = aMessage.Int1();
	TInt error = iRepository.TransactionSetL(key, val);

	return error;
	}

TInt CServerSession::GetRealL(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();

	if(KErrNone != CheckPolicy(aMessage,iRepository.GetReadAccessPolicy(key),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::GetRealL - Attempt made to read a setting")))
		return KErrPermissionDenied;
	
	TReal val;
	TInt error = iRepository.Get(key, val);

	if(error==KErrNone)
		{
		TPckg<TReal> p(val);
		aMessage.WriteL(1, p);
		}

	return error;
	}

TInt CServerSession::SetRealL(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();

	// cannot make changes in a read transaction - upgrade must have failed due to write lock being used
	if (iRepository.IsInActiveReadTransaction())
		{
		return iRepository.FailTransaction(KErrLocked, key);
		}
	
	if(KErrNone != CheckPolicy(aMessage,iRepository.GetWriteAccessPolicy(key),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::SetRealL - Attempt made to write a setting")))
		return iRepository.FailTransaction(KErrPermissionDenied, key);

	TReal val;
	TPckg<TReal> p(val);
	aMessage.ReadL(1, p);

	TInt error = iRepository.TransactionSetL(key, val);

	return error;
	}

TInt CServerSession::GetStringL(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();

	if(KErrNone != CheckPolicy(aMessage,iRepository.GetReadAccessPolicy(key),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::GetStringL - Attempt made to read a setting")))
		return KErrPermissionDenied;

	TBuf8<KMaxBinaryLength> val;

	TInt error = iRepository.Get(key, val);

	if(error==KErrNone)
		{
		TInt clientMaxDescriptorLength;
		TPckg<TInt> pInt (clientMaxDescriptorLength);
		aMessage.Read(2, pInt);
		
		TInt descriptorLength = val.Length();
		
		// write the descriptor length to aMessage			
		TPckg<TInt> p(descriptorLength);
		error = aMessage.Write(2, p);
		
		if(error==KErrNone)
			{
			if(descriptorLength > clientMaxDescriptorLength)
				{
				// if it is, fill the descriptor up to its max length
				error = aMessage.Write(1, val.Left(clientMaxDescriptorLength));

				// if client-side descriptor is too small to take the value, which it is, returns KErrOverflow
				if(error == KErrNone)
					{
					error = KErrOverflow;
					}
				}
			else
				{
				error = aMessage.Write(1, val);
				}
			}
			
		// if error is KErrOverflow should not failing transaction
		if ((error != KErrNone) && (error != KErrOverflow))
			{
			// ServiceError will fail transaction
			User::Leave(error);
			}
		}
	return error;
	}

TInt CServerSession::SetStringL(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();

	// cannot make changes in a read transaction - upgrade must have failed due to write lock being used
	if (iRepository.IsInActiveReadTransaction())
		{
		return iRepository.FailTransaction(KErrLocked, key);
		}

	// check for descriptor-too-long was previously on the client side,
	// hence test code expects KErrArgument response before KErrPermissionDenied
	TInt length = aMessage.GetDesLengthL(1);
	if (length > KMaxBinaryLength)
		{
		return iRepository.FailTransaction(KErrArgument, key);
		}
	
	if(KErrNone != CheckPolicy(aMessage,iRepository.GetWriteAccessPolicy(key),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::SetStringL - Attempt made to write a setting")))
		return iRepository.FailTransaction(KErrPermissionDenied, key);

	TBuf8<KMaxBinaryLength> val;
	aMessage.ReadL(1, val);  // no error if too long, truncated instead

	TInt error = iRepository.TransactionSetL(key, val);

	return error;
	}

TInt CServerSession::FindL(const TClientRequest& aMessage)
	{
// PLATSEC NOTE:
// There are no read policy checks on FindL.
// Client is returned the full list of keys whether they have read permission or not.
// They were able to find this out by brute force using FindL on single setting ranges
// anyway (because it would return KErrPermissionDenied if it existed, KErrNotFound otherwise).
// Revealing the existence of settings is not considered a breach of security.
	TInt partialId = aMessage.Int0();
	TInt idMask = aMessage.Int1();
	// we reset results first since we do not police GetResult
	// and this way we can guarantee no results are available if access not granted
	iFindResult.Reset();
	RSettingPointerArray settings;
	CleanupClosePushL(settings);
	TInt error = iRepository.FindSettings(partialId, idMask, settings);
	
	if(error == KErrNone)
		{
		const TInt numSettings = settings.Count();
		
		if (numSettings == 0)
			{
			error = KErrNotFound;
			}
		else
			{
			RArray<TUint32> settingsToSend;
			CleanupClosePushL(settingsToSend);
			const TInt numInitial = numSettings > KCentRepFindBufSize ? KCentRepFindBufSize : numSettings;
			const TInt numFinal = numSettings > KCentRepFindBufSize ? numSettings - KCentRepFindBufSize : 0;
			
			//reserve memory for everything that needs to be added to the array
			settingsToSend.Reserve(numSettings + 1);
			
			//first append the number of found settings
			settingsToSend.AppendL(numSettings);
			
			//now append up to KCentRepFindBufSize settings
			for(TInt i = 0; i < numInitial; i++)
				{
				ASSERT(settings[i]);
				// all settings flagged as deleted should have been removed by now, but just to be safe:
				ASSERT(!settings[i]->IsDeleted());
				settingsToSend.AppendL(settings[i]->Key());
				}
				
			//fill the iFindResult array with any remaining settings that we were not able to send
			//with the first ipc
			if(numFinal)
				{
				for(TInt i = numInitial; i < numSettings; i++)
					{
					ASSERT(settings[i]);
					// all settings flagged as deleted should have been removed by now, but just to be safe:
					ASSERT(!settings[i]->IsDeleted());
					iFindResult.AppendL(settings[i]->Key());
					}
				}
				
			//send:
			//1 - the count of total settings found
			//2 - the settings that fit in the buffer allocated for the first IPC 
			TPtrC8 p(reinterpret_cast<TUint8*>(&(settingsToSend[0])), (numInitial+1)*sizeof(TUint32));
			error = aMessage.Write(3, p);
			CleanupStack::PopAndDestroy(); //settingsToSend
			}
		}
	
	if(error != KErrNone)
		{
		iFindResult.Reset();
		if ((error != KErrNone) && (error != KErrNotFound))
			{
			CleanupStack::PopAndDestroy(); //settings
			// ServiceError will fail transaction
			User::Leave(error);
			}
		}
	CleanupStack::PopAndDestroy(); //settings
	return error;
	}

TInt CServerSession::FindEqIntL(const TClientRequest& aMessage)
	{
	TInt val = aMessage.Int2();
	// PlatSec check done in FindValueL
	return FindValueL(aMessage, val);
	}

TInt CServerSession::FindEqRealL(const TClientRequest& aMessage)
	{
	TReal val;
	TPckg<TReal> p(val);
	aMessage.Read(2, p);
	// PlatSec check done in FindValueL
	return FindValueL(aMessage, val);
	}

TInt CServerSession::FindEqStringL(const TClientRequest& aMessage)
	{
	TBuf8<KMaxBinaryLength> val;
	aMessage.ReadL(2, val);  // no error if too long, truncated instead
	// PlatSec check done in FindValueL
	return FindValueL(aMessage, val);
	}

TInt CServerSession::FindNeqIntL(const TClientRequest& aMessage)
	{
	TInt val = aMessage.Int2();
	// PlatSec check done in FindValueL
	return FindValueL(aMessage, val, ENotEqual);
	}

TInt CServerSession::FindNeqRealL(const TClientRequest& aMessage)
	{
	TReal val;
	TPckg<TReal> p(val);
	aMessage.Read(2, p);
	// PlatSec check done in FindValueL
	return FindValueL(aMessage, val, ENotEqual);
	}

TInt CServerSession::FindNeqStringL(const TClientRequest& aMessage)
	{
	TBuf8<KMaxBinaryLength> val;
	aMessage.ReadL(2, val);  // no error if too long, truncated instead
	// PlatSec check done in FindValueL
	return FindValueL(aMessage, val, ENotEqual);
	}

template <class T>
TInt CServerSession::FindValueL(const TClientRequest& aMessage, const T& aVal,
	TComparison aComparison)
	{
	// IMPORTANT PLATSEC NOTE:
	// MUST return KErrPermissionDenied if read policy of ANY setting in the search range not passed.
	// MUST NOT merely check read policy of matching entries, otherwise it is possible to determine
	// secret values by brute force: Using single-value ranges, cycling through the possible values and
	// confirming a match when it returns KErrPermissionDenied rather than KErrNotFound.
	TInt partialId = aMessage.Int0();
	TInt idMask = aMessage.Int1();
	// we reset results first since we do not police GetResult
	// and this way we can guarantee no results are available if access not granted
	iFindResult.Reset();
	RSettingPointerArray settings;
	CleanupClosePushL(settings);
	TInt error = iRepository.FindSettings(partialId, idMask, settings);
	if (error == KErrNone)
		{
		error = iRepository.MakeComparisonFindResultsCheckingReadPolicies(settings, aMessage,
			__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::FindValueL - Attempt made to search settings"),
			aVal, aComparison, iFindResult);
			
		if ((error == KErrNone) && (iFindResult.Count() == 0))
			{
			error = KErrNotFound;
			}
			
		if(error == KErrNone)
			{
			const TInt numSettings = iFindResult.Count();
			const TInt numInitial = numSettings > KCentRepFindBufSize ? KCentRepFindBufSize : numSettings;
			RArray<TUint32> settingsToSend;
			CleanupClosePushL(settingsToSend);
			
			//reserve memory for everything that needs to be added to the array
			settingsToSend.Reserve(numInitial);
			
			//first append the number of found settings
			settingsToSend.AppendL(numSettings);
			
			//now append up to KCentRepFindBufSize settings
			for(TInt i = 0; i < numInitial; i++)
				{
				settingsToSend.AppendL(iFindResult[0]);
				iFindResult.Remove(0);
				}
				
			if(iFindResult.Count() == 0)
				{
				iFindResult.Reset();
				}
				
			//send:
			//1 - the count of total settings found
			//2 - the settings that fit in the buffer allocated for the first IPC 
			TPtrC8 p(reinterpret_cast<TUint8*>(&(settingsToSend[0])), (numInitial+1)*sizeof(TUint32));
			error = aMessage.Write(3, p);
			CleanupStack::PopAndDestroy(); //settingsToSend
			}
		}
		
	if ((error != KErrNone) && (error != KErrNotFound) && (error != KErrPermissionDenied))
		{
		iFindResult.Reset();
		CleanupStack::PopAndDestroy(); //settings
		
		// ServiceError will fail transaction
		User::Leave(error);
		}
		
	CleanupStack::PopAndDestroy(); //settings
	return error;
	}

TInt CServerSession::GetFindResultL(const TClientRequest& aMessage)
	{
	TInt n = iFindResult.Count();
	if (n == 0)
		{
		// should never call this if it's zero
		return KErrNotFound;
		}
  	TPtrC8 p(reinterpret_cast<TUint8*>(&(iFindResult[0])), n*sizeof(TUint32));
  	TInt error = aMessage.Write(0, p);
  	// Free up iFindResult - it's no longer needed
  	iFindResult.Reset();
	// ServiceError will fail transaction
	return User::LeaveIfError(error);
	}

TInt CServerSession::NotifyRequestCheck(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();
	if(KErrNone != CheckPolicy(aMessage,iRepository.GetReadAccessPolicy(key),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::NotifyRequestCheck - Attempt made to check Notify request")))
		return KErrPermissionDenied;

	return iRepository.GetPersistentSetting(key) ? KErrNone : KErrNotFound;
	}

TInt CServerSession::NotifyRequest(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();
	if(KErrNone != CheckPolicy(aMessage,iRepository.GetReadAccessPolicy(key),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::NotifyRequest - Attempt made to register for Notify")))
		return KErrPermissionDenied;
	TInt r = iNotifier.AddRequest(key, aMessage);
	return r==KErrNone ? KDontComplete : r;
	}

TInt CServerSession::NotifyCancel(const TClientRequest& aMessage)
	{
	TUint32 key = aMessage.Int0();
	return iNotifier.CancelRequest(key);
	}

TInt CServerSession::NotifyCancelAll(const TClientRequest& /*aMessage*/)
	{
	return iNotifier.CancelAllRequests();
	}

TInt CServerSession::GroupNotifyRequest(const TClientRequest& aMessage)
	{
	TUint32 partialId = aMessage.Int0();
	TUint32 idMask = aMessage.Int1();
	RSettingPointerArray settings;
	TInt error = iRepository.FindPersistentSettings(partialId, idMask, settings);
	if (error == KErrNone)
		{
		error = iRepository.CheckReadPermissions(settings, aMessage,
			__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::GroupNotifyRequest - Attempt made to register for group Notify"));
		}
	settings.Reset();
	if (error != KErrNone)
		{
		return KErrPermissionDenied;
		}

	error = iNotifier.AddRequest(partialId, idMask, aMessage);
	return error==KErrNone ? KDontComplete : error;
	}

TInt CServerSession::GroupNotifyCancel(const TClientRequest& aMessage)
	{
	const TUint32 partialId = aMessage.Int0();
	const TUint32 idMask = aMessage.Int1();
	RSettingPointerArray settings;
	TInt error = iRepository.FindPersistentSettings(partialId, idMask, settings);
	settings.Reset();
	if (error != KErrNone)
		{
		return error;
		}

	return iNotifier.CancelRequest(partialId, idMask);
	}

TInt CServerSession::ResetL(const TClientRequest& aMessage)
	{
	// individual setting reset is not yet supported in transactions
	ASSERT(!iRepository.IsInTransaction());
	TUint32 key = aMessage.Int0();
	if(KErrNone != CheckPolicy(aMessage,iRepository.GetWriteAccessPolicy(key),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::ResetL - Attempt made to reset a setting")))
		return iRepository.FailTransaction(KErrPermissionDenied, key);

	TInt r = iRepository.ResetL(key);

	return r;
	}

TInt CServerSession::ResetAllL(const TClientRequest& aMessage)
	{
	// factory reset operation is not yet supported in transactions
	ASSERT(!iRepository.IsInTransaction());
	if(KErrNone != CheckPolicy(aMessage,iRepository.GetDefaultWriteAccessPolicy(),
				__PLATSEC_DIAGNOSTIC_STRING("CenRep: CServerSession::ResetAllL - Attempt made to reset repository")))
		return KErrPermissionDenied;
	
	iNotifier.IdReportingOff();
	TInt r = iRepository.ResetAllL();
	iNotifier.IdReportingOn();
	
	if(r==KErrNone)
	iRepository.CommitChangesL();  // temporary measure

	return r;
	}

TInt CServerSession::MoveL(const TClientRequest& aMessage)
	{
	if (iRepository.IsInActiveReadTransaction())
		{
		// could not promote to read/write: fail & give the source partialKey as the error key
		TUint32 sourcePartialKey = aMessage.Int0();
		return iRepository.FailTransaction(KErrLocked, sourcePartialKey);
		}
	TUint32 errorKey = KUnspecifiedKey;
	TInt result = iRepository.TransactionMoveL(aMessage, errorKey);
	if (result != KErrNone)
		{
		TPckg<TUint32> p(errorKey);
		aMessage.WriteL(3, p);
		}
	return result;
	}
	
TInt CServerSession::TransactionStart(const TClientRequest& aMessage)
	{
	// panic client if attempting to start a transaction when already in one
	__ASSERT_ALWAYS(!iRepository.IsInTransaction(), PanicClient(EStartAlreadyInTransaction, aMessage));

	const TInt mode = aMessage.Int0();
	return iRepository.StartTransaction(mode);
	}

// serves as both rollback and async cancel
TInt CServerSession::TransactionCancel(const TClientRequest& /*aMessage*/)
	{
	iRepository.CancelTransaction();
	return KErrNone;
	}

TInt CServerSession::TransactionCommitL(const TClientRequest& aMessage)
	{
	__ASSERT_ALWAYS(iRepository.IsInTransaction(), PanicClient(ECommitNotInTransaction, aMessage));

	TUint32 keyInfo = KUnspecifiedKey;
	TInt result = iRepository.CommitTransaction(keyInfo);

	TPckg<TUint32> p(keyInfo);
	aMessage.WriteL(0, p);

	return result;
	}

TInt CServerSession::DeleteRangeL(const TClientRequest& aMessage)
	{
	if (iRepository.IsInActiveReadTransaction())
		{
		// could not promote to read/write: fail & give the partialKey as the error key
		TUint32 partialKey = aMessage.Int0();
		return iRepository.FailTransaction(KErrLocked, partialKey);
		}
	TUint32 errorKey = KUnspecifiedKey;
	TInt result = iRepository.TransactionDeleteRangeL(aMessage, errorKey);
	if (result != KErrNone)
		{
		TPckg<TUint32> p(errorKey);
		aMessage.WriteL(2, p);
		}
	return result;
	}	

TInt CServerSession::TransactionStateL(const TClientRequest& aMessage)
	{
	TInt iState = iRepository.TransactionState();
	
	TPckg<TInt> p(iState);
	aMessage.WriteL(0, p);
	
	return KErrNone;
	}
	
TInt CServerSession::TransactionFail(const TClientRequest& /*aMessage*/)
	{
	iRepository.FailTransaction(KErrAbort, KUnspecifiedKey);
	return KErrNone;
	}

/**
@internalTechnology
Check the security policy against a RMessage.
*/
TInt CServerSession::CheckPolicy(const TClientRequest& msg,
								 const TSecurityPolicy& aPolicy, 
								 const char *aDiagnostic)
	{
	return msg.CheckPolicy(aPolicy,aDiagnostic) ? KErrNone : KErrPermissionDenied;
	}

#ifdef __CENTREP_SERVER_PERFTEST__
// DoEventStart
// Save initial values of timer and MEM allocated.
void CServerSession::DoEventStart()
	{
	if(iPerfData.IsFinished())
		return;
	
	iStartTick = User::FastCounter();
	}

// DoEventEnd
// store CPU and MEM usage, time of event, etc. in performance
// array.
void CServerSession::DoEventEnd(TUint aEventId)
	{
	if (iPerfData.IsFinished())
		return;

	TUint32 endTick = User::FastCounter();

	// Performance data has 3 parts. First: time spent to
	// service the request. 2nd if event is open/close/evict
	// time of the event. 3rd, if open/close/evict repository UID

	// First part: event ID and CPU time spent servicing the request
	TUint32 entry = EncodeEventAndData(aEventId, endTick - iStartTick);
	iPerfData.Append(entry);

	if (aEventId == EInitialise)
		iPerfData.SessionOpen();

	TBool repOpenCloseEvict = aEventId == EInitialise || aEventId == ESessionClose || aEventId == EEvict;

	// Third part: repository UID if event is open/close/evict.
	if (repOpenCloseEvict)
		{
		iPerfData.Append(iStartTick);
		iPerfData.Append(iRepositoryUid);  // Append will check if array IsFull.
		}

	// One last thing to do: check if all concurrent sessions close.
	if (aEventId == ESessionClose)
		{
		iPerfData.SessionClose();

		// This is a temporary solution to detect end-of-boot until SS code is 
		// modified to return end-of-boot state and Centrep becomes SSA 
		if (iRepositoryUid == 0xCCCCCC00)
			{
			iPerfData.IncEndOfBoot();
			}
		}
	}
#endif // __CENTREP_SERVER_PERFTEST__


#if defined(__CENTREP_SERVER_PERFTEST__) || defined (__CENTREP_SERVER_MEMTEST__) || defined(__CENTREP_SERVER_CACHETEST__)
// GetSetParameters
// The function code EGetSetParameters is a generic msg reserved
// for testing purpose. Int0 specifies the function to perform.
TInt CServerSession::GetSetParameters(const TClientRequest& aMessage)
	{
	TServerGetSetParametersSubCmd cmd = static_cast<TServerGetSetParametersSubCmd>(aMessage.Int0());

#ifdef __CENTREP_SERVER_PERFTEST__
	if (cmd == EGetPerfResults)
		{
		TInt desSize = aMessage.GetDesMaxLength(1);
		TInt numVals = desSize / sizeof(TUint32);
		if (numVals < KCentRepPerfTestArraySize)
			{
			return KErrOverflow;
			}
		TPtrC8 p(reinterpret_cast<const TUint8*>(iPerfData.Entries()),
				   KCentRepPerfTestArraySize * sizeof(TUint32));
		TInt ret = aMessage.Write(1, p);
		if (ret == KErrNone)
			{
			TUint lastCompleteAccess = iPerfData.LastCompleteAccess();
			TPckg<TUint> p2(lastCompleteAccess);
			ret = aMessage.Write(2, p2);
			}
		return ret;
		}
	else if (cmd == ERestartPerfTests)
		{
		iPerfData.Reset();
		return KErrNone;
		}
	else if (cmd == EStopPerfTests)
		{
		iPerfData.Stop();
		return KErrNone;
		}
#endif // __CENTREP_SERVER_PERFTEST__

#ifdef __CENTREP_SERVER_MEMTEST__
	if(cmd == EGetMemResults)
		{
		TInt count = TServerResources::iMemTestDataCount;
		TPckg<TInt> pCount(count);
		
		TInt err = aMessage.Write(1, pCount);
		if(err == KErrNone && count > 0)
			{
			TPtrC8 pBuf(reinterpret_cast<TUint8*>(TServerResources::iMemTestData), (TServerResources::iMemTestDataCount)*sizeof(TInt32));
			err = aMessage.Write(2, pBuf);
			}
		// Stop recording results
		TServerResources::StopRecordTimerResult();
		return err;
		}
#endif // __CENTREP_SERVER_MEMTEST__

#ifdef __CENTREP_SERVER_CACHETEST__
	if (cmd == EEnableCache)
		{
		// First parameter is Timer Interval, second is cache size
		TServerResources::iCacheManager->EnableCache(aMessage.Int1(), aMessage.Int2());
		return KErrNone;
		}
	
	if (cmd == EDisableCache)
		{
		TServerResources::iCacheManager->DisableCache();
		return KErrNone;
		}
#endif  // __CENTREP_SERVER_CACHETEST__
	return KErrNotSupported;
	}
#endif // __CENTREP_SERVER_PERFTEST__ || __CENTREP_SERVER_MEMTEST__ || __CENTREP_SERVER_CACHETEST__
