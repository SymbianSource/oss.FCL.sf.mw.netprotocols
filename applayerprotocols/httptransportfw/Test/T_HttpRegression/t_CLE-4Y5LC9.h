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
// Regression test for defect BEU-4YJFSK (EResponseComplete received without receiveing EGotResponseHeaders first)
// 
//

#ifndef __T_CLE4Y5LC9_H__
#define __T_CLE4Y5LC9_H__


#include <e32test.h>
#include <http.h>

#include "HttpTestCore.h"
#include "csrvaddrval.h"

class CTestFixCLE4Y5LC9Base : public CHttpTestBase, 
	public MHTTPTransactionCallback, public MHTTPDataSupplier
	{
public:
	void DoRunL();
	void DoCancel();
protected:
	// From MHTTPDataSupplier
	virtual TBool GetNextDataPart(TPtrC8& aDataChunk);
	virtual void ReleaseData();
	virtual TInt OverallDataSize();

	// From MHTTPTransactionCallback
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

protected:
	CTestFixCLE4Y5LC9Base(CScriptFile* aIniSettingsFile);
	
private:
	CScriptFile* iIniSettingsFile;

protected:
	TInt iError;
	TInt iByteCount;
	TInt iSubmitCount;
	RHTTPSession iSession;
	RHTTPTransaction iTransaction;
	};

class CTestFixCLE4Y5LC9_1 : public CTestFixCLE4Y5LC9Base
	{
 public:
	static CTestFixCLE4Y5LC9_1* NewLC(CScriptFile* aIniSettingsFile);
	~CTestFixCLE4Y5LC9_1();
	virtual const TDesC& TestName();

 protected:
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt Reset();
	
 private:
	CTestFixCLE4Y5LC9_1(CScriptFile* aIniSettingsFile);
	};

class CTestFixCLE4Y5LC9_2 : public CTestFixCLE4Y5LC9Base
	{
 public:
	static CTestFixCLE4Y5LC9_2* NewLC(CScriptFile* aIniSettingsFile);
	~CTestFixCLE4Y5LC9_2();
	virtual const TDesC& TestName();

 protected:
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt Reset();

 private:
	CTestFixCLE4Y5LC9_2(CScriptFile* aIniSettingsFile);

 private:
	TBool iFinished;
	};

#endif // __T_CLE4Y5LC9_H__
