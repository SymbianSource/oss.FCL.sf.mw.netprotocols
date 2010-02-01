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
// Internal class for the repository access base class.
// @internalComponent
// 
//

#include "repository.h"

CRepositoryAccessor::CRepositoryAccessor()
	: iStatus(EStatusCreating)
	{
	}

void CRepositoryAccessor::SetRepository(CRepository& aRepository)
	{
	iRepository = &aRepository;
	}

void CRepositoryAccessor::CommitL()
	{
	TUint32 indexBase = IndexBase();

	switch (iStatus)
		{
	case EStatusCreating:
		TransactionL(ETransNew);
		break;
	case EStatusDirty:
		TransactionL(ETransSave);
		break;
	case EStatusDeleted:
		TransactionL(ETransRemove);
		break;
	case EStatusClean:
	default:
		// Do nothing
		break;
		}
	iStatus = EStatusClean;
	}

void CRepositoryAccessor::SetClean()
	{
	iStatus = EStatusClean;
	}

void CRepositoryAccessor::SetDirty()
	{
	if (iStatus == EStatusClean)
		{
		iStatus = EStatusDirty;
		}
	}

void CRepositoryAccessor::DeleteL()
	{
	iStatus = EStatusDeleted;
	}

Bookmark::TItemId CRepositoryAccessor::NextIndexL()
	{
	TInt nextIndex = 0;
	User::LeaveIfError(iRepository->Get(KRepNextIndex, nextIndex));
	if (nextIndex > Bookmark::KMaxBookmarkEntries)
		{
		User::Leave(Bookmark::KErrNoMoreSpace);
		}

	Bookmark::TItemId id = nextIndex;
	++nextIndex;
	User::LeaveIfError(iRepository->Set(KRepNextIndex, nextIndex));

	return id;
	}

void CRepositoryAccessor::TransactionL(TTransType aTransType)
	{
	// Loads are not performed in transactions. This is because property entries 
	// need to be filled as they go along. The alternative would be to fill some type of 
	// dynamic array and that pass this to a second method that will fill in the objects data members.
	if (aTransType == ETransLoad)
		{
		TransLoadL();
		return;
		}

	TUint32 failedIndex;
	// start a transaction
	TInt result = KErrLocked;
	do
		{
		iRepository->StartTransaction(CRepository::EConcurrentReadWriteTransaction);
		iRepository->CleanupCancelTransactionPushL();
		
		switch(aTransType)
			{
		case ETransNew:
			TransNewL();
			break;
		case ETransSave:
			TransSaveL();
			break;
		case ETransRemove:
			TransRemoveL();
			break;
		default:
			ASSERT(0);
			break;
			}
		
		// commit the transaction
		result = iRepository->CommitTransaction(failedIndex);
		CleanupStack::PopAndDestroy();
		} while (result == KErrLocked);
		
	User::LeaveIfError(result);
	}
