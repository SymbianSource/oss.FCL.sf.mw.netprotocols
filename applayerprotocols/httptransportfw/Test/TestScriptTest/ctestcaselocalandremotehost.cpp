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

#include "ctestcaselocalandremotehost.h"
#include "httptestutils.h"

const TInt KInternalTransCount = 3;
CTestCaseLocalAndRemoteHost* CTestCaseLocalAndRemoteHost::NewL(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver)
    {
    CTestCaseLocalAndRemoteHost* self = new (ELeave) CTestCaseLocalAndRemoteHost(aTestUtils, aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;    
    }

CTestCaseLocalAndRemoteHost::~CTestCaseLocalAndRemoteHost()
    {
    }


void CTestCaseLocalAndRemoteHost::ConstructL()
    {
    CPipeliningTestClient::ConstructL();
    }

CTestCaseLocalAndRemoteHost::CTestCaseLocalAndRemoteHost(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver)
: CPipeliningTestClient(aTestUtils, aObserver)
    {
    
    }

RHTTPTransaction CTestCaseLocalAndRemoteHost::GetLocalTransactionL()
    {
    _LIT8(KTxtUri, "http://127.0.0.1");
    TUriParser8 uri; 
    uri.Parse(KTxtUri());    
    return iSession.OpenTransactionL(uri, *this);    
    }

RHTTPTransaction CTestCaseLocalAndRemoteHost::GetRemoteTransactionL()
    {
    _LIT8(KRemoteUri,   "http://10.29.23.12/");
    TUriParser8 uri; 
    uri.Parse(KRemoteUri());    
    return iSession.OpenTransactionL(uri, *this);    
    }

void CTestCaseLocalAndRemoteHost::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
    {
        
    switch (aEvent.iStatus)
        {
        case THTTPEvent::EGotResponseHeaders:
            {
            _LIT(KTxtSuccessful, "EGotResponse headers: %d.");
             iTestUtils.LogIt(KTxtSuccessful, aTransaction.Id());
              
            } break;
        case THTTPEvent::EGotResponseBodyData:
            {
            // Done with that bit of body data
            aTransaction.Response().Body()->ReleaseData();
            } break;
        case THTTPEvent::EResponseComplete:
            {
            } break;
        case THTTPEvent::ESucceeded:
            {
            ++iCurrentTrans;
            TInt transId = aTransaction.Id();
            aTransaction.Close();
            _LIT(KTxtSuccessful, "Client - Transaction %d, completed successfully.");
            iTestUtils.LogIt(KTxtSuccessful, transId);
            
            if(KInternalTransCount == iCurrentTrans)
                {   
                EndTestL(KErrNone);
                break;
                }
            if(iSentRemoteHostTrans == EFalse)
                {
                StartAndSetNetworkToHttp();
                // Create and submit the transaction to remote host
                GetRemoteTransactionL().SubmitL();
                iSentRemoteHostTrans = ETrue;
                }
            else
                {
                // Create and submit the transaction to local host
                GetLocalTransactionL().SubmitL();
                }            
            } break;
        case THTTPEvent::EFailed:
            {
            aTransaction.Close();
            EndTestL(KErrNotFound); // We must not fail. If we fail we fail the test case as well.
            } break;
        default:
            {
            CPipeliningTestClient::MHFRunL(aTransaction, aEvent);
            } break;
        }    
    }
    

