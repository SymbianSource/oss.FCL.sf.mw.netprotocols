/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/
#ifndef __CTESTCLIENTHTTPPOST_H__
#define __CTESTCLIENTHTTPPOST_H__

#include "chttpservice.h"
#include "chttpclienttransaction.h"
#include "httptestutils.h"

class MLocalTestServerObserver;
class CHttpClientTestParams;

class CTestClientHttpSimplePost : public CHttpClientTransaction, public MHttpContentSource
    {
public:    
    static CTestClientHttpSimplePost* NewL(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHttpService& aClient, const TDesC8& aUri, CHTTPTestUtils& aUtils);
    ~CTestClientHttpSimplePost();
    
private:
    CTestClientHttpSimplePost(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHTTPTestUtils& aUtils);
    void ConstructL(CHttpService& aClient, const TDesC8& aUri);
    // From CHTTPClientTransaction
    virtual void OnResponseHeaders();
    virtual void OnCompletion();
    virtual void OnError(TInt aError);    
    virtual void OnTransferProgress(TInt aTotal, TInt aTransffered);
    // From MHTTPContentSource
    void OnData(THttpContentSourceOp& aData);
    
    MLocalTestServerObserver&   iObserver;
    CHttpClientTestParams&      iParams;
    CHTTPTestUtils&             iTestUtils;
    };
 
#endif // __CHTTPCLIENTHTTPGET_H__
