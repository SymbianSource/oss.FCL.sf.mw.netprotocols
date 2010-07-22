// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CTESTHTTPCLIENTSTEP_H__
#define __CTESTHTTPCLIENTSTEP_H__


#include <testexecuteserverbase.h>
#include "mlocaltestserverobserver.h"

class CHttpService;
class CActiveScheduler;
class CHttpClientTransaction;
class CLocalTestServer;
class CHTTPTestUtils;
class CHttpClientTestParamArray;
class CHttpNetworkConnection;
class CTestHttpServiceAuthentication;

class CTestHttpClientStep : public CTestStep, public MLocalTestServerObserver
    {
public:
    CTestHttpClientStep();
    ~CTestHttpClientStep();
      
    // TEF virtuals
    virtual TVerdict doTestStepPreambleL();

    // TEF pure virtual
    virtual TVerdict doTestStepL();
    
    void EndTest(TInt aError);
    
private:
    CHttpService*        iHttpClient;
    CActiveScheduler*   iActiveScheduler;
    CHttpClientTransaction* iHttpTrans;
    CHTTPTestUtils*     iTestUtils;
    CLocalTestServer*   iTestServer;
    CHttpClientTestParamArray* iTestParamArray;
    CTestHttpServiceAuthentication* iTestHttpServiceAuthentication;
    CHttpNetworkConnection* iHttpConnInfo;
    };

#endif // __CTESTHTTPCLIENTSTEP_H__
