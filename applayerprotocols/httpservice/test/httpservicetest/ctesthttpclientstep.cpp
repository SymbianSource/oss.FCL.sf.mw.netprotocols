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

#include "ctesthttpclientstep.h"
#include "chttpservice.h"
#include "ctestclienthttpget.h"
#include "ctestclienthttppost.h"
#include <escapeutils.h>
#include <thttphdrval.h>
#include <commdbconnpref.h>
#include "httptestutils.h"
#include "clocaltestserver.h"
#include "chttpclienttestparams.h"
#include "ctesthttpserviceauthentication.h"
#include "chttpnetworkconnectioninfo.h"

_LIT(KTestCaseName, "TestCaseName");
_LIT8(KUserAgent, "HTTP Client API Test");
_LIT(KTestHttpClientAPITestTitle, "HTTP client API Unit Test Harness");
const TInt KMaxNoOfConnections = 6;
const TInt KMaxTransToPipeline = 5;

CTestHttpClientStep::CTestHttpClientStep()
    {
    
    } 

CTestHttpClientStep::~CTestHttpClientStep()
    {
    iTestServer->StopServer();
    delete iHttpTrans;
    delete iHttpClient;
    delete iTestServer;
    delete iTestUtils;    
    delete iTestParamArray;
    delete iActiveScheduler;
    delete iTestHttpServiceAuthentication;
    }

// TEF virtuals
TVerdict CTestHttpClientStep::doTestStepPreambleL()
    {
    iActiveScheduler = new (ELeave) CActiveScheduler();
    CActiveScheduler::Install(iActiveScheduler);
    iTestParamArray = new(ELeave) CHttpClientTestParamArray;
    iTestUtils = CHTTPTestUtils::NewL(KTestHttpClientAPITestTitle());
    
    
    iTestServer = CLocalTestServer::NewL(*iTestUtils, *this, *iTestParamArray);
    iHttpClient = CHttpService::NewL();
    TCommDbConnPref connPref;
    connPref.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
    connPref.SetIapId(1);
    connPref.SetNetId(0);
    iHttpConnInfo = iHttpClient->HttpNetworkConnection();
    User::LeaveIfError(iHttpConnInfo->Start());
    THttpHeaderValueVariant variant(KUserAgent());
    if(iHttpClient->AddRequestHeader(HTTP::EUserAgent, variant))
        {
        SetTestStepResult(EFail);
        return TestStepResult();
        }
    if(iHttpClient->MaxConnections() != KMaxNoOfConnections)
        {
        SetTestStepResult(EFail);
        return TestStepResult();
        }
    if(iHttpClient->MaxTransactionsToPipeline() != KMaxTransToPipeline)
        {
        SetTestStepResult(EFail);
        return TestStepResult();
        }

    iTestServer->StartServerL();
    return TestStepResult();
    }

// TEF pure virtual
TVerdict CTestHttpClientStep::doTestStepL()
    {
    TPtrC testStepName;
    if(!GetStringFromConfig(ConfigSection(), KTestCaseName(), testStepName))
        {
        User::Leave(KErrCorrupt);
        }
    SetTestStepName(testStepName);
    _LIT8(KGetMethod, "GET");
    CHttpClientTestParams* param = CHttpClientTestParams::NewL(*this);
    iTestParamArray->AddTestParams(param);
    const TDesC8& proxyAddress = param->ProxyAddress();
    if(proxyAddress.Length() > 0)
        {
        User::LeaveIfError(iHttpClient->SetProxy(proxyAddress));
        if(iHttpClient->ProxyAddress().CompareF(proxyAddress) != 0)
            {
            User::Leave(KErrCorrupt);
            }
        }
    
    if(param->IsAuthenticationReqd())
        {
        iTestHttpServiceAuthentication = new CTestHttpServiceAuthentication();
        User::LeaveIfError(iHttpClient->SetAuthentication(iTestHttpServiceAuthentication));
        }
  
    if(param->Method().CompareF(KGetMethod) == 0)
        {
        if(param->OnlineTest())
            {
            iHttpTrans = CTestClientHttpOnlineGet::NewL(*param, *this, *iHttpClient, param->Uri(), iTestUtils);
            }
        else
            {
            iHttpTrans = CTestClientHttpSimpleGet::NewL(*param, *this, *iHttpClient, param->Uri(), iTestUtils);
            }
        
        }
    else
        {
        iHttpTrans = CTestClientHttpSimplePost::NewL(*param, *this, *iHttpClient, param->Uri(), *iTestUtils);
        TInt len = param->RequestBody().Length();
        if(len > 0)
            {
            THttpHeaderValueVariant variant(len);
            iHttpTrans->AddRequestHeader(HTTP::EContentLength, variant);
            }
        }  
   
    if(param->NoRetryOnDisconnect())
        {
        iHttpTrans->SetNoRetry();
        }
    if(param->ResponseTimeoutEnable())
        {
        iHttpTrans->SetResponseTimeout(10);
        }
    const RPointerArray<CHeaderInfo>& info = param->RequestHeaderInfos();
    for(TInt i = 0; i < info.Count(); ++i)
        {
        CHeaderInfo* headerInfo = info[i];
        const TDesC8& headerName = *headerInfo->iFieldName;
        
        for(TInt j = 0; j < headerInfo->iFieldInfos.Count(); ++j)
            {
            CHeaderFieldInfo* headerFieldInfo = headerInfo->iFieldInfos[j];
            const TDesC8& headerFieldValue = *headerFieldInfo->iFieldValue;
            THttpHeaderValueVariant variant(headerFieldValue);
            if(headerFieldInfo->iFieldParamInfos.Count() == 0)
                {                
                // Set the header
                iHttpTrans->AddRequestHeader(headerName, variant);
                }
            else
                {
                for(TInt k = 0; k < headerFieldInfo->iFieldParamInfos.Count(); ++k)
                    {
                    CHeaderFieldParamInfo* paramInfo = headerFieldInfo->iFieldParamInfos[k];
                    const TDesC8& paramName = *paramInfo->iParamName;
                    const TDesC8& paramValue = *paramInfo->iParamValue;
                    TLex8 lex(paramValue);
                    TReal64 value = 0.;
                    lex.Val(value);             
                    THTTPHdrVal::TQConv q(value);
                    TInt val2 = q;
                    THttpHeaderValueVariant variant2(val2);
                    /// Set the header with the param
                    iHttpTrans->AddRequestHeader(headerName, variant, paramName, variant2);
                    }
                }
            }
        }
    
    iHttpTrans->SendRequest();
 
    CActiveScheduler::Start();
    return TestStepResult();    
    }

void CTestHttpClientStep::EndTest(TInt aError)
    {
    if(aError != KErrNone)
        {
        SetTestStepResult(EFail);
        }
    // Failed with error.
    iHttpTrans->CancelRequest();
    CActiveScheduler::Stop();
    }
