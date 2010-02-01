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

#ifndef __CEVENTCALLBACKFILTER_H__
#define __CEVENTCALLBACKFILTER_H__

// System includes
//
#include <e32base.h>
#include <http/mhttpsessioneventcallback.h>
#include <http/mhttptransactioncallback.h>

// Local includes
//
#include "itemstore.h"
#include "chattriggers.h"

class CEventCallbackFilter;

class TSessionEventCallback : public MHTTPSessionEventCallback
	{
public:	// Methods

	TSessionEventCallback(CEventCallbackFilter* aCallback);

	TSessionEventCallback();

private:	// Methods from MHTTPFilterBase

/**
	@fn				HFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	@see			MHTTPFilterBase
 */
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

/**
	@fn				MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	@see			MHTTPFilterBase
 */
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

/**
	@fn				MHFSessionRunL(const THTTPSessionEvent& aEvent)
	@see			MHTTPFilterBase
 */
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);

/**
	@fn				MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent)
	@see			MHTTPFilterBase
 */
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

private:	// Attributes

	CEventCallbackFilter*	iCallback;
	};

class CDriverTrans;
class MTransConverter
	{
public:
	virtual CDriverTrans& LookUpTransaction(RHTTPTransaction) = 0;
	};

class TTransactionEventCallback : public MHTTPTransactionCallback
	{
public:	// Methods

	TTransactionEventCallback(CEventCallbackFilter* aCallback);

	TTransactionEventCallback();

	void SetTransConverter(MTransConverter& aTrConv);

private:	// Methods from MHTTPTransactionCallback

/**
	@fn				HFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	@see			MHTTPFilterBase
 */
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

/**
	@fn				MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	@see			MHTTPFilterBase
 */
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

private:	// Attributes

	CEventCallbackFilter*	iCallback;
	MTransConverter* iTransConv;
	};

class MTransConverter;
class CDriverTrans;
class CEventCallbackFilter : public CBase, public MChatItemStore
	{
public:	// Methods

	IMPORT_C ~CEventCallbackFilter();

/**
	@fn				SessionCallback()
	Intended Usage	:	Provides the callback filter for session events. This
						function is not exported as it is not intended for use
						outside of the dll.
	@since			7.0
	@return			A reference to the session events callback filter.
 */
	MHTTPSessionEventCallback& SessionCallback();

/**
	@fn				TransactionCallback()
	Intended Usage	:	Provides the callback filter for transaction events. 
						This function is not exported as it is not intended for
						use outside of the dll.
	@since			7.0
	@return			A reference to the transaction events callback filter.
 */
	MHTTPTransactionCallback& TransactionCallback();

	IMPORT_C void SetTransConverter(MTransConverter& aConv);

public:	// Pure virtual functions for derived class

	virtual void SessionRunL(const THTTPSessionEvent& aEvent) =0;

	virtual TInt SessionRunError(TInt aError, const THTTPSessionEvent& aEvent) =0;

	virtual void TransactionRunL(CDriverTrans& aTransaction, const THTTPEvent& aEvent) =0;

	virtual TInt TransactionRunError(TInt aError, CDriverTrans& aTransaction, const THTTPEvent& aEvent) =0;

protected:	// Methods

	IMPORT_C CEventCallbackFilter();

	/** Look-up in the item store an item with the supplied ID
	*/
	IMPORT_C TInt FetchFromItemStore(const TDesC& aItemName, TStoredItemBase& aItem) const;

	/** Item store clean-up - removes all items from the store, deleting those that the stub owns
	*/
	IMPORT_C void FlushItemStore();

protected:	// Methods from MChatItemStore

	IMPORT_C virtual void StoreItemL(const TDesC& aItemName, TStoredItemBase aItem);
	IMPORT_C virtual TStoredItemBase CreateItemL(const TDesC& aItemName, const TDesC& aStubClass);
	IMPORT_C virtual TBool LookUpItem(TStoredItemBase aItem, TPtrC& aItemName) const;
	IMPORT_C virtual void MCIS_Reserved();

protected:	// Methods to be implemented in sub-classes

	virtual TStoredItemBase DoCreateItemLC(const TDesC& aStubClass) = 0;

private:	// Methods

	/** Helper for storage of items in the item store - adds a flag indicating whether the item is owned by the stub
		(and hence must be deleted by the stub)
	*/
	void DoStoreItemL(const TDesC& aItemName, TStoredItemBase aItem, TBool aOwnedByStub);

private: // type definitions

	/** Simple structure that pairs a decriptor (owned elsewhere) with a store item and a flag indicating ownership
	*/
	class TStoredItemPair
		{
	public:
		TStoredItemPair();
		TPtrC iName;
		TStoredItemBase iItem;
		TBool iOwnedByDriver;
		};


private:	// Attributes

	TSessionEventCallback		iSessionCallback;

	TTransactionEventCallback	iTransactionCallback;

	MTransConverter* iConv;

	/** The item store
	*/
	RArray<TStoredItemPair> iItemStore;
	};

#endif // __CEVENTCALLBACKFILTER_H__