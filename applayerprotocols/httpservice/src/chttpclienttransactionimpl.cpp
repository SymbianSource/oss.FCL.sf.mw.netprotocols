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

#include <uri8.h>
#include "chttpclienttransaction.h"
#include "chttpclienttransactionimpl.h"
#include "chttpservice.h"
#include "chttpdatareceiver.h"
#include "chttpdatasender.h"
#include "CHeaders.h"

CHttpClientTransactionImpl* CHttpClientTransactionImpl::NewL(CHttpService& aClient, CHttpClientTransaction& aTrans, const TDesC8& aMethod, const TDesC8& aUri)
	{
	CHttpClientTransactionImpl* impl = new(ELeave) CHttpClientTransactionImpl(aTrans, aClient.Session().StringPool());
	CleanupStack::PushL(impl);
	impl->ConstructL(aClient, aMethod, aUri);
	CleanupStack::Pop(); // impl
	return impl;
	}
	
CHttpClientTransactionImpl::~CHttpClientTransactionImpl()
	{
	iTransaction.Close();
	delete iDataSender;
	delete iDataReceiver;
	}

void CHttpClientTransactionImpl::ConstructL(CHttpService& aClient, const TDesC8& aMethod, const TDesC8& aUri)
	{
	TUriParser8 parser;
	User::LeaveIfError(parser.Parse(aUri));
	RHTTPSession sess = aClient.Session();
	RStringF method = iStringPool.OpenFStringL(aMethod);
	CleanupClosePushL(method);
	iTransaction = sess.OpenTransactionL(parser, *this, method);
	CleanupStack::PopAndDestroy(); // method	
	}

	
void CHttpClientTransactionImpl::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
    __ASSERT_DEBUG (iTransaction == aTransaction, User::Invariant());
    switch(aEvent.iStatus)
        {
        case THTTPEvent::EGotResponseHeaders:
            {
            // Parse the headers
            if(aTransaction.Response().GetHeaderCollection().iImplementation->ConvertAllHeadersToParsedFormat() != KErrNone)
                {
                iClientTrans.OnError(KErrCorrupt); // Change the error code.
                return;
                }
            iClientTrans.OnResponseHeaders();
            if(iDataReceiver)
                {
                iDataReceiver->SetDataSupplier(aTransaction.Response().Body());
                }
            }
        break;
        
        case THTTPEvent::EGotResponseBodyData:
            {
            iDataReceiver->DataAvailable();
            }
        break;
        
        case THTTPEvent::ERedirectedTemporarily:
        case THTTPEvent::ERedirectedPermanently:
             {
            if(!iClientTrans.OnRedirection())
                {  
                aTransaction.Cancel();
                }
              }
        break;
        case THTTPEvent::ERedirectRequiresConfirmation:
            {
            aTransaction.Cancel();
            if(iClientTrans.OnRedirection())
                {  
                aTransaction.Submit();
                }
            // Otherwise no need to do anything. Response will complete by itself
            }
        break;
        
        case THTTPEvent::EResponseComplete:
            // Do nothing
            break;
        
        case THTTPEvent::ESucceeded:
            {
            iClientTrans.OnCompletion();
            }
        break;
        
        case THTTPEvent::EFailed:
            {
            // Cancel the transaction. The failure would have been indicated to the
            // client already.
            iTransaction.Cancel();
            }
        break;

        case THTTPEvent::ESendTimeOut:
            {            
            }
            break;
            
        case THTTPEvent::EReceiveTimeOut:
            {
            iTransaction.Cancel();
            iClientTrans.OnError(KErrHttpResponseNotReceived);
            }
            break;
                   
        default:
            {
            iClientTrans.OnError(aEvent.iStatus);
            }
        break;
        }
	}
	
TInt CHttpClientTransactionImpl::MHFRunError(TInt aError, RHTTPTransaction /* aTransaction */, const THTTPEvent& /* aEvent */)
	{
	iClientTrans.OnError(aError);
	return KErrNone;
	}


TInt CHttpClientTransactionImpl::SetDynamicContentSource(MHttpContentSource* aSource)
    {
    delete iDataSender;
    iDataSender = NULL;
    iDataSender = CHttpDataSender::New(iTransaction, iClientTrans, aSource);
    if(iDataSender == NULL)
        return KErrNoMemory;
    iTransaction.Request().SetBody(*iDataSender);
    return KErrNone;
    }

TInt CHttpClientTransactionImpl::SetDynamicContentSink(MHttpContentSink* aSink)
    {
    if(!iDataReceiver)
        {
        iDataReceiver = new CHttpDataReceiver(iTransaction, iClientTrans);
        if(iDataReceiver == NULL)
            return KErrNoMemory;
        }
    
    iDataReceiver->SetDynamicSink(aSink);
    return KErrNone;
    }

TInt CHttpClientTransactionImpl::SetStaticContentSource(RFile& aSource)
    {
    delete iDataSender;
    iDataSender = NULL;
    iDataSender = CHttpDataSender::New(iTransaction, iClientTrans, aSource);
    if(iDataSender == NULL)
         return KErrNoMemory;
     iTransaction.Request().SetBody(*iDataSender);
     return KErrNone;
    }

TInt CHttpClientTransactionImpl::SetStaticContentSink(RFile& aSink)
    {
    if(!iDataReceiver)
         {
         iDataReceiver = new CHttpDataReceiver(iTransaction, iClientTrans);
         if(iDataReceiver == NULL)
             return KErrNoMemory;
         }
    return iDataReceiver->SetFile(aSink);
    }

void CHttpClientTransactionImpl::SetDynamicContentSourceLength(TInt aLen)
    {
    if(iDataSender)
        iDataSender->SetDynamicContentLen(aLen);
    }

