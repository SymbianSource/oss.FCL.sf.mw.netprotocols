// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "t_wspeventdriver/ceventcallbackfilter.h"
#include "t_wspeventdriver/TDriverPanic.h"
	
TSessionEventCallback::TSessionEventCallback(CEventCallbackFilter* aCallback)
: iCallback(aCallback)
	{
	}

TSessionEventCallback::TSessionEventCallback()
: iCallback(NULL)
	{
	}

void TSessionEventCallback::MHFRunL(RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	}

TInt TSessionEventCallback::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

void TSessionEventCallback::MHFSessionRunL(const THTTPSessionEvent& aEvent)
	{
	if (aEvent.iStatus == -999)
		{
		CActiveScheduler::Stop();
		return;
		}
	__ASSERT_DEBUG(iCallback, TDriverPanic::Panic(TDriverPanic::EWspEventDriverNotInitialised)); 
	iCallback->SessionRunL(aEvent);
	}

TInt TSessionEventCallback::MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent)
	{
	__ASSERT_DEBUG(iCallback, TDriverPanic::Panic(TDriverPanic::EWspEventDriverNotInitialised)); 
	return iCallback->SessionRunError(aError, aEvent);
	}



TTransactionEventCallback::TTransactionEventCallback(CEventCallbackFilter* aCallback)
: iCallback(aCallback), iTransConv(NULL)
	{
	}

TTransactionEventCallback::TTransactionEventCallback()
: iCallback(NULL), iTransConv(NULL)
	{
	}

void TTransactionEventCallback::SetTransConverter(MTransConverter& aConv)
	{
	iTransConv = &aConv;
	}

void TTransactionEventCallback::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	__ASSERT_DEBUG(iCallback, TDriverPanic::Panic(TDriverPanic::EWspEventDriverNotInitialised)); 
	__ASSERT_DEBUG(iTransConv, TDriverPanic::Panic(TDriverPanic::EWspEventDriverNotInitialised)); 
	iCallback->TransactionRunL(iTransConv->LookUpTransaction(aTransaction), aEvent);
	}

TInt TTransactionEventCallback::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	__ASSERT_DEBUG(iCallback, TDriverPanic::Panic(TDriverPanic::EWspEventDriverNotInitialised)); 
	return iCallback->TransactionRunError(aError, iTransConv->LookUpTransaction(aTransaction), aEvent);
	}

EXPORT_C CEventCallbackFilter::CEventCallbackFilter()
: CBase()
	{
	iSessionCallback = TSessionEventCallback(this);
	iTransactionCallback = TTransactionEventCallback(this);
	}

EXPORT_C CEventCallbackFilter::~CEventCallbackFilter()
	{
	FlushItemStore();
	}

EXPORT_C void CEventCallbackFilter::SetTransConverter(MTransConverter& aConv)
	{
	iConv = &aConv;
	iTransactionCallback.SetTransConverter(*iConv);
	}

MHTTPSessionEventCallback& CEventCallbackFilter::SessionCallback()
	{
	return iSessionCallback;
	}

MHTTPTransactionCallback& CEventCallbackFilter::TransactionCallback()
	{
	return iTransactionCallback;
	}

CEventCallbackFilter::TStoredItemPair::TStoredItemPair()
	: iName(KNullDesC()), iItem(NULL, NULL, NULL)
	{
	}

EXPORT_C
void CEventCallbackFilter::StoreItemL(const TDesC& aItemName, TStoredItemBase aItem)
	{
	// Store the item, noting that it is _not_ owned by the Driver
	DoStoreItemL(aItemName, aItem, EFalse);
	}

EXPORT_C
TStoredItemBase CEventCallbackFilter::CreateItemL(const TDesC& aItemName, const TDesC& aDriverClass)
	{
	// Determine whether we can create the requested item - obviously only a very limited functionality can be achieved via
	// a script like this. So far we can only do MHTTPDataSupplier, as implemented by CDriverDataSupplier.
	TStoredItemBase item = DoCreateItemLC(aDriverClass);

	// Store the item, noting that it _is_ owned by the driver and must be cleaned up later.
	DoStoreItemL(aItemName, item, ETrue);
	CleanupStack::Pop();
	return item;
	}

EXPORT_C
TBool CEventCallbackFilter::LookUpItem(TStoredItemBase aItem, TPtrC& aItemName) const
	{
	// Iterate the item store in search of the specified item
	TBool found = EFalse;
	TInt idx = 0;
	while (idx < iItemStore.Count())
		{
		TStoredItemPair stored = iItemStore[idx];
		if (stored.iItem == aItem)
			{
			aItemName.Set(stored.iName);
			found = ETrue;
			break;
			}
		++idx;
		}
	return found;
	}

void CEventCallbackFilter::DoStoreItemL(const TDesC& aItemName, TStoredItemBase aItem, TBool aOwnedByDriver)
	{
	// Create a pair and add to the array that forms the item store
	TStoredItemPair toStore;
	toStore.iName.Set(aItemName);
	toStore.iItem = aItem;
	toStore.iOwnedByDriver = aOwnedByDriver;
	User::LeaveIfError(iItemStore.Append(toStore));
	}

EXPORT_C
TBool CEventCallbackFilter::FetchFromItemStore(const TDesC& aItemName, TStoredItemBase& aItem) const
	{
	TInt numItems = iItemStore.Count();
	TBool found = EFalse;
	TInt idx = 0;
	while ((idx < numItems) && !found)
		{
		TStoredItemPair item = iItemStore[idx];
		if (!aItemName.Compare(item.iName))
			{
			found = ETrue;
			aItem = item.iItem;
			}
		++idx;
		}
	return found;
	}

EXPORT_C void CEventCallbackFilter::FlushItemStore()
	{
	while (iItemStore.Count() > 0)
		{
		// Get the item at the head of the list and, if it is owned by the Driver, delete it
		TStoredItemPair item = iItemStore[0];
		if (item.iOwnedByDriver)
			{
			// Clean up the item, in a similar manner to the cleanup stack
			item.iItem.CleanUp();
			}
		iItemStore.Remove(0);
		}
	iItemStore.Reset();
	}

EXPORT_C void CEventCallbackFilter::MCIS_Reserved()
	{
	}
