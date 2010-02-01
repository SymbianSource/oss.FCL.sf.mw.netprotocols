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

#ifndef __T_KHN4Z7DT7_H__
#define __T_KHN4Z7DT7_H__


#include <e32test.h>
#include <http.h>

#include "HttpTestCore.h"
#include "csrvaddrval.h"

class CTestFixKHN4Z7DT7 : public CHttpTestBase, 
	public MHTTPTransactionCallback
	{
public:
	static CTestFixKHN4Z7DT7* NewLC(CScriptFile* aIniSettingsFile);
	~CTestFixKHN4Z7DT7();
	virtual const TDesC& TestName();
	void DoRunL();
	void DoCancel();

protected:
	// From MHTTPTransactionCallback
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

private:
	CTestFixKHN4Z7DT7(CScriptFile* aIniSettingsFile);

private:
	CScriptFile* iIniSettingsFile;

protected:
	TInt iCount;
	TInt iError;
	RHTTPSession iSession;
	RHTTPTransaction iTransaction;
	};

#endif // __T_KHN4Z7DT7_H__
