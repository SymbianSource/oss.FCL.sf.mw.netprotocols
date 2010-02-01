// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef	__T_UAPROF_H__
#define	__T_UAPROF_H__


// System Includes
#include <http.h>
#include <http/mhttpfiltercreationcallback.h>

// User Includes
#include "HttpTestCore.h"
#include <http/mhttpsessioneventcallback.h>

class CTestCpiManager;

/**
 *  Derived test case class : test UAProf
 */
class CTestUAProf: public CHttpTestTransBase,
				   public MHTTPTransactionCallback,
				   public MHTTPSessionEventCallback
	{
friend class CTestCpiManager;

public: // methods

	static CTestUAProf* NewL(TInt aTestNumber, const TDesC8& aProxyAddr, const TDesC8& aHostName);
	virtual ~CTestUAProf();


public: // methods derived from MHTTPTransactionCallback and MHTTPSessionEventCallback

	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);


private: // methods

	CTestUAProf(TInt aTestNumber);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void ConstructL(const TDesC8& aProxyAddr, const TDesC8& aHostName);

	void DoUAProfTest1L();
	void DoUAProfTest2L();
	void DoUAProfTest3L();
	void DoUAProfTest4L();
	void DoUAProfTest5L();
	void DoUAProfTest6L();
	void DoGenericUAProfTestL();

	void SessionConnectL();
	void CreateTransactionL();
	void SessionDisconnectL();


private: // attributes

	HBufC8* iProxyAddr;
	HBufC8* iHostName;
	TInt iFailureError;
	CTestCpiManager* iCpiManager;
	TInt iTestNumber;
	TInt iOpenTransactionCount;
	};

#endif // __T_UAPROF_H__
