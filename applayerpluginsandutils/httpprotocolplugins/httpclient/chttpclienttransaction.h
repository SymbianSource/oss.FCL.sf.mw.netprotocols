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

#ifndef __CHTTPCLIENTTRANSACTION_H__
#define __CHTTPCLIENTTRANSACTION_H__

#include <e32base.h>
#include <http/framework/cprottransaction.h>

class CHttpConnectionManager;

class CHttpClientTransaction : public CProtTransaction						   
	{
public:

	static CHttpClientTransaction* NewL(RHTTPTransaction aTransaction);
	virtual ~CHttpClientTransaction();
	
	void SetConnectionManager(CHttpConnectionManager& aConnectionManager);
	CHttpConnectionManager* ConnectionManager() const;
	void RemoveConnectionManager();

	void IncRetryCount ();
	TBool RetryNeeded ();
    TBool NeedDisconnectNotification () const;

public:	// methods from CProtTransaction

	virtual void CreateTxDataL();
	virtual void CreateRxDataL(MRxDataObserver& aObserver);
	
private:

	CHttpClientTransaction(RHTTPTransaction aTransaction);
	
private:

	CHttpConnectionManager* iConnectionManager;
	TInt					iRetryCount;
	const TStringTable&        iStringTable;
	};

#endif	// __CHTTPCLIENTTRANSACTION_H__
