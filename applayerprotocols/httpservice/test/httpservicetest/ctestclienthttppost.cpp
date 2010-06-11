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
#include "ctestclienthttppost.h"
#include <httpstringconstants.h>
#include "mlocaltestserverobserver.h"
#include "chttpclienttestparams.h"
_LIT(KTransferProgress, "File size: %d kb , Uploaded so far: %d kb"); 

CTestClientHttpSimplePost* CTestClientHttpSimplePost::NewL(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHttpService& aClient, const TDesC8& aUri, CHTTPTestUtils& aUtils)
    {
    CTestClientHttpSimplePost* self = new(ELeave) CTestClientHttpSimplePost(aParams, aObserver, aUtils);
    CleanupStack::PushL(self);
    self->ConstructL(aClient, aUri);
    CleanupStack::Pop(); // self
    return self;
    }

CTestClientHttpSimplePost::~CTestClientHttpSimplePost()
    {
    
    }

CTestClientHttpSimplePost::CTestClientHttpSimplePost(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHTTPTestUtils& aUtils)
: iObserver(aObserver),
iParams(aParams),
iTestUtils(aUtils)
    {
    
    } 

void CTestClientHttpSimplePost::ConstructL(CHttpService& aClient, const TDesC8& aUri)
    {
    TPtrC8 method;
    aClient.String(HTTP::EPOST, method);
    CHttpClientTransaction::Create(aClient, method, aUri);   
    if(iParams.IsFileSending())
        CHttpClientTransaction::SetContentSource(iParams.SourceFile());
    else
        CHttpClientTransaction::SetContentSource(*this);
    }

// From CHTTPClientTransaction
void CTestClientHttpSimplePost::OnResponseHeaders()
    {
    // Check whether the status code matches
    if(iParams.StatusCode() != StatusCode())
        {
        iObserver.EndTest(KErrCorrupt);
        }
    }

void CTestClientHttpSimplePost::OnCompletion()
    {
    if(iParams.RequestBody().Length() > 0)
        {
        iObserver.EndTest(KErrNone);
        return;
        }
    if(iParams.MatchPostFileContent())
        iObserver.EndTest(KErrNone);
    else
        iObserver.EndTest(KErrCorrupt);
    }

void CTestClientHttpSimplePost::OnError(TInt aError)
    {
    if(aError == iParams.ExpectedError())
        iObserver.EndTest(KErrNone);
    else
        iObserver.EndTest(aError);    
    }

void CTestClientHttpSimplePost::OnTransferProgress(TInt aTotal, TInt aTransffered)
    {
    if(iParams.TransferProgress())
        {
        iTestUtils.LogIt(KTransferProgress(),aTotal,aTransffered);
        }    
    }

void CTestClientHttpSimplePost::OnData(THttpContentSourceOp& aData)
    {
    aData.Notify(iParams.RequestBody(), ETrue);
    }


