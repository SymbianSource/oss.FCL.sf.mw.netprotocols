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

#ifndef __CPIPELININGTESTCLIENT_H__
#define __CPIPELININGTESTCLIENT_H__

#include <e32base.h>
#include <http.h>
#include <in_sock.h>

class CHTTPTestUtils;
class MPipeliningTestCase;
class MPipeliningTestObserver;

class CHttpTestTimer : public CActive
    {
    public:
    static CHttpTestTimer* NewL();
    
    void RunL();
    void DoCancel();
    void StartTimerAndWait();
    private:
    CHttpTestTimer();
    void ConstructL();
    
    private:
    RTimer iTimer;
    };

class CPipeliningTestClient : public CBase,
							  public MHTTPTransactionCallback
	{
public:
	static CPipeliningTestClient* NewL(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver);
	~CPipeliningTestClient();
	
	RHTTPSession& GetSession();
	void AddTransactionL(RHTTPTransaction aTransaction);
	void StartClientL();
	void SetTestCase(MPipeliningTestCase* aTestCase);
	TInt LastError();
	
protected:
	CPipeliningTestClient(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver);
	void ConstructL();
    void EndTestL(TInt aErrorCode);
    
    // From MHTTPTransactionCallback
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	void StartAndSetNetworkToHttp();
protected:
	CHTTPTestUtils&				iTestUtils;
	MPipeliningTestObserver&	iObserver;
	RHTTPSession				iSession;
	RArray<RHTTPTransaction>	iTransArray;
	MPipeliningTestCase*		iTestCase;		// Not owned
	TInt						iCurrentTrans;
	TInt						iLastError;
    RSocketServ             iSocketServ;
    RConnection             iNetworkConnection;    
	};
	
#endif // __CPIPELININGTESTCLIENT_H__
