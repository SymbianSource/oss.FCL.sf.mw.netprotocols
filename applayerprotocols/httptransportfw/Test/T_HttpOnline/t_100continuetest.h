// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// T_100CONINUETEST.H
// 
//

#ifndef	__T_100CONINUETEST_H__
#define	__T_100CONINUETEST_H__

#include <http.h>
#include <http/mhttpfiltercreationcallback.h>
#include <http/mhttpauthenticationcallback.h>
#include <chttpformencoder.h>
#include "HttpTestCore.h"

// User Include
#include "TestScripts.h"

// Literals used in the file
_LIT8(KHttpUrl,		"http://search.apache.org");
_LIT8(KBody,		"keyword=asdf");

class CHttpTimer;

class MTestClient
	{
	protected:
		RHTTPTransaction iTrans;
		
	friend class CHttpTimer;	
	};



class CTestClient: public CHttpTestTransBase,
							public MHTTPTransactionCallback 
													
	{
public:
	static CTestClient* NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	virtual ~CTestClient();
	// to implement in this class' member function definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;

protected:
	CTestClient(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	
	TBool GetNextDataPart(TPtrC8& aDataPart);
	void ReleaseData();
	TInt OverallDataSize();
	TInt Reset();

private:
	CScriptFile* iIniSettingsFile;
	
protected:
	TInt iFailureError;
	TInt iTestNumber;
	};



class CTestClient_2: public CHttpTestTransBase,
							public MHTTPTransactionCallback 
													
	{
public:
	static CTestClient_2* NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	virtual ~CTestClient_2();
	// to implement in this class' member function definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;

protected:
	CTestClient_2(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();

	TBool GetNextDataPart(TPtrC8& aDataPart);
	void ReleaseData();
	TInt OverallDataSize();
	TInt Reset();

private:
	CScriptFile* iIniSettingsFile;
	
protected:
	TInt iFailureError;
	TInt iTestNumber;
	};
	
	
	
class CTestClient_3: public CHttpTestTransBase,
							public MHTTPTransactionCallback,
							public MTestClient
													
	{
public:
	static CTestClient_3* NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	virtual ~CTestClient_3();
	// to implement in this class' member function definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;

protected:
	CTestClient_3(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();

	TBool GetNextDataPart(TPtrC8& aDataPart);
	void ReleaseData();
	TInt OverallDataSize();
	TInt Reset();

private:
	CScriptFile* iIniSettingsFile;
	
protected:
	TInt iFailureError;
	TInt iTestNumber;
	};
	
	
	
class CTestClient_4: public CHttpTestTransBase,
							public MHTTPTransactionCallback,
							public MTestClient
													
	{
public:
	static CTestClient_4* NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	virtual ~CTestClient_4();
	// to implement in this class' member function definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;

protected:
	CTestClient_4(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();

	TBool GetNextDataPart(TPtrC8& aDataPart);
	void ReleaseData();
	TInt OverallDataSize();
	TInt Reset();

private:
	CScriptFile* iIniSettingsFile;
	
protected:
	TInt iFailureError;
	TInt iTestNumber;
	};	
	

class CTestClient_5: public CHttpTestTransBase,
							public MHTTPTransactionCallback 
													
	{
public:
	static CTestClient_5* NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	virtual ~CTestClient_5();
	// to implement in this class' member function definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;

protected:
	CTestClient_5(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	
	TBool GetNextDataPart(TPtrC8& aDataPart);
	void ReleaseData();
	TInt OverallDataSize();
	TInt Reset();

private:
	CScriptFile* iIniSettingsFile;
	
protected:
	TInt iFailureError;
	TInt iTestNumber;
	};


	
class CHttpTimer: public CActive
	{
	public:
		CHttpTimer(MTestClient& aClient);
		~CHttpTimer();
		void After(TTimeIntervalMicroSeconds32 anInterval);
		void DoCancel();
		void RunL();
	private:
		RTimer iTimer;
		MTestClient& iClient;		
	};
#endif // __T_100CONINUETEST_H__
