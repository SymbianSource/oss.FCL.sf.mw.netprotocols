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
// T_BEU-4YJFSK.h
// 
//

#ifndef __T_INC053172_H__
#define __T_INC053172_H__


#include <e32test.h>
#include <http.h>

#include "HttpTestCore.h"

class CTestFixINC053172 : public CHttpTestBase, 
	public MHTTPTransactionCallback
	{
public:
	static CTestFixINC053172* NewLC();
	~CTestFixINC053172();
	virtual const TDesC& TestName();
	void DoRunL();
	void DoCancel();

protected:
	// From MHTTPTransactionCallback
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

 protected:
	TInt iCount;
	TInt iError;
	RHTTPSession iSession;
	RHTTPTransaction iTransaction;
	};

#endif // __T_INC053172_H__
