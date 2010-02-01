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

#if !defined (__CTEXTMODETESTPROXIES_H__)
#define __CTEXTMODETESTPROXIES_H__

#if !defined HTTP_H
#include <http.h>
#endif

#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

/**
 *  Derived test case class : Test a sequence of transactions that share a persistent connection
 */
class CTextModeTestProxies : public CHttpTestTransBase, public MHTTPTransactionCallback
	{
public:
	static CTextModeTestProxies* NewL(TInt aTestNum, const TDesC8& aProxyAddr, const TDesC8& aHostName);
	virtual ~CTextModeTestProxies();
public: 
	// methods inherited from MHTTPTransactionCallback
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	// methods inherited from MHTTPDataSupplier
	virtual TBool GetNextDataPart(TPtrC8& aDataChunk);
	void ReleaseData();
	TInt OverallDataSize();
protected:
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
private:
	CTextModeTestProxies(TInt aTestNum);
	void ConstructL(const TDesC8& aProxyAddr, const TDesC8& aHostName);
	//
	void Test1L();
	void Test2L();
	void Test3L();
	void Test4L();
	void Test5L();
	void Test6L();
	void Test7L();
	void Test8L();
	void Test9L();
	void Test10L();
	void DoTestMethodL(TBool aUseRelativeURL, const TDesC8& aHostVal, TBool aUseProxy);
	//
private:
	TInt iTestNum;
	HBufC8* iProxyAddr;
	HBufC8* iHostName;
	CUri8* iAbsUri;
	TInt iFailureError;
	};


#endif // !defined __CTEXTMODETESTPROXIES_H__
