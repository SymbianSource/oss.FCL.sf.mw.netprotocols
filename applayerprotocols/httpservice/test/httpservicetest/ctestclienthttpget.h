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

#ifndef __CTESTCLIENTHTTPGET_H__
#define __CTESTCLIENTHTTPGET_H__

#include "chttpservice.h"
#include "chttpclienttransaction.h"
#include "httptestutils.h"

class MLocalTestServerObserver;
class CHttpClientTestParams;

class CTestClientHttpSimpleGet : public CHttpClientTransaction, public MHttpContentSink
    {
public:    
    static CTestClientHttpSimpleGet* NewL(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHttpService& aClient, const TDesC8& aUri, CHTTPTestUtils* iUtils);
    ~CTestClientHttpSimpleGet();
      
protected:
    CTestClientHttpSimpleGet(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHTTPTestUtils* iUtils);
    void ConstructL(CHttpService& aClient, const TDesC8& aUri);
    // From CHTTPClientTransaction
    virtual void OnResponseHeaders();
    virtual void OnCompletion();
    virtual void OnError(TInt aError);    
    virtual void OnTransferProgress(TInt aTotal, TInt aTransffered); 
    virtual TBool OnRedirection(); 
    // From MHTTPContentSink
    void OnData(THttpContentSinkOp& aData);
protected:    
    MLocalTestServerObserver&   iObserver;
    CHttpClientTestParams&      iParams;
    CHTTPTestUtils&              iTestUtils;
    };

class CTestClientHttpOnlineGet : public CTestClientHttpSimpleGet
    {
public:    
    static CTestClientHttpOnlineGet* NewL(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHttpService& aClient, const TDesC8& aUri, CHTTPTestUtils* iUtils);
    ~CTestClientHttpOnlineGet();
private:
    void ConstructL(CHttpService& aClient, const TDesC8& aUri);
    
    CTestClientHttpOnlineGet(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHTTPTestUtils* iUtils);   
    virtual void OnResponseHeaders();
    virtual void OnCompletion();
    // From MHTTPContentSink
    void OnData(THttpContentSinkOp& aData);    
    };
#endif // __CHTTPCLIENTHTTPGET_H__
