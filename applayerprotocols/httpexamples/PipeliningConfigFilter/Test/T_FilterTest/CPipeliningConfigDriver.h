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

#include "HttpTestCore.h"
#include <http.h>
#include <http/mhttpauthenticationcallback.h>

class CScriptFile;
class CPipeliningConfigFilterTest;

class CPipeliningConfigDriver :  public CHttpTestBase,
										MHTTPAuthenticationCallback,
										MHTTPTransactionCallback
	{
public:
	static CPipeliningConfigDriver* NewL(CScriptFile& aScriptFile, TInt aSectionIndex);
	~CPipeliningConfigDriver();
	
	// Methods from CHttpTestBase
	virtual const TDesC& TestName();
	virtual void DoRunL();
	virtual void DoCancel();

	// Methods from MHTTPAuthenticationCallback
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
								  RStringF aAuthenticationType,
								  RString& aUsername, 
								  RString& aPassword);
	
	// Methods from MHTTPTransactionCallback						  
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
			  
private:
	CPipeliningConfigDriver(CScriptFile& aScriptFile, TInt aSectionIndex);
	void ConstructL();
	
	void ResetHttpSessionL();
	void ReadScriptL();
	void CreateTransactionsL();
	void SetHttpVersionL();
	HBufC8* ConvertStringTo8BitL(const TDesC& aDes);
	TInt ConvertDesToInt(const TDesC& aDes);
	
private:
	RHTTPSession iSession;
	CScriptFile& iScriptFile;
	TInt iSectionIndex;
	HBufC8* iUri;
	HBufC8* iUsername;
	HBufC8* iPassword;
	RArray<RHTTPTransaction> iTransArray;
	TInt iTotalTrans;
	TInt iCurrentTrans;
	TBool iShouldPipeline;
	TPtrC iVersion;
	CPipeliningConfigFilterTest* iFilter; // Not owned
	TPtrC iTestName;
	};
