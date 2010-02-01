// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPCONNECTFILTER_H__
#define __CHTTPCONNECTFILTER_H__

#include <e32base.h>
#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <stringpool.h>

#include "mhttpconnectobserver.h"
#include "thttpconnectcallback.h"

class CHttpConnectFilter : public CBase, 
						   public MHTTPFilter,
						   public MHttpConnectObserver
	{
public:

	static CHttpConnectFilter* NewL(RHTTPSession aSession);
	virtual ~CHttpConnectFilter();

private:	// methods from MHTTPFilterBase

	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

private:	// methods from MHTTPFilter

	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);

private:	// methods from MHttpConnectObserver

	virtual void ConnectCompleteL(TInt aConnectId);
	virtual void ConnectErrorL(TInt aConnectId);
	virtual void FailPendingTransactions(TInt aConnectId);	

private:

	class TPendingTransaction
		{
	public:

		inline TPendingTransaction();
		inline TPendingTransaction(RHTTPTransaction aTransaction, TInt aId);

	public:
	
		RHTTPTransaction	iPendingTransaction;
		TInt				iConnectId;

		};
	
private:

	CHttpConnectFilter(RHTTPSession aSession);
	void ConstructL();

	TInt FindConnectRequest(RStringF aTunnelHost, RStringF aProxy);
	TInt CreateConnectTransactionL(RStringF aTunnelHost, RStringF aProxy, RStringF aUserAgent, RStringF aProfile);
	void CloseConnectTransaction(TInt aConnectId);

private:

	TInt			iLoadCount;
	RHTTPSession	iSession;
	const TStringTable& iStringTable;
	RStringPool		iStringPool;

	THttpConnectCallback	iCallback;

	RArray<RHTTPTransaction>	iConnectTransactions;
	RArray<TPendingTransaction>	iPendingTransactions;
	
	RHTTPTransaction	iNewConnectTransaction;
	TBool				iCloseConnectTransaction;

	};

inline CHttpConnectFilter::TPendingTransaction::TPendingTransaction()
	{
	}

inline CHttpConnectFilter::TPendingTransaction::TPendingTransaction(
																   RHTTPTransaction	aTransaction, 
																   TInt				aId
																   )
: iPendingTransaction(aTransaction), iConnectId(aId)
	{
	}

#endif	//__CHTTPCONNECTFILTER_H_ 
