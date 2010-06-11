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

#include "ctestserverstreammanager.h"
#include <minputstream.h>
#include <moutputstream.h>
#include "httptestutils.h"
#include "chttpclienttestparams.h"
#include <escapeutils.h>


CLocalTestServerStreamManager* CLocalTestServerStreamManager::NewL(CHTTPTestUtils& aTestUtils, MInputStream* aInStream, MOutputStream* aOutStream, CHttpClientTestParamArray& aTestParamArray)
	{
	CLocalTestServerStreamManager* self = new(ELeave) CLocalTestServerStreamManager(aTestUtils, aInStream, aOutStream, aTestParamArray);
	CleanupStack::PushL(self);
	
	CleanupStack::Pop();	
	return self;
	} 
	
CLocalTestServerStreamManager::~CLocalTestServerStreamManager()
	{
    if(iInputStream != NULL)
        {
        iInputStream->Close();
        }
    if(iOutputStream != NULL)
        {
        iOutputStream->Close();
        }	
	delete iRequestStream;	
	delete iResponseStream;
	}
	
CLocalTestServerStreamManager::CLocalTestServerStreamManager(CHTTPTestUtils& aTestUtils, MInputStream* aInStream, MOutputStream* aOutStream, CHttpClientTestParamArray& aTestParamArray)
: iTestUtils(aTestUtils),
iInputStream(aInStream),
iOutputStream(aOutStream),
iTestParamArray(aTestParamArray)
	{
    iInputStream->Bind(*this);
    iOutputStream->Bind(*this);	
	}
	
// From MInputStreamObserver
void CLocalTestServerStreamManager::ReceivedDataIndL(const TDesC8& aBuffer)
	{
    _LIT(KTxtConnection, "Stream Manager - Data received.");
    iTestUtils.LogIt(KTxtConnection());
	
	if(iRequestStream == NULL)
	    {
	    iRequestStream = aBuffer.AllocL();
	    }
	else
	    {
	    if(aBuffer.CompareF(iCurrentTestParam->EncodedAuthenticationRequest()) != 0)
	        {
            // Check to see if the buffer needs to grow
            TInt maxLength  = iRequestStream->Des().MaxLength();
            TInt reqdLength = iRequestStream->Length() + aBuffer.Length();
            if (reqdLength > maxLength)
                {
                HBufC8* largerBuffer = iRequestStream->ReAllocL(maxLength + aBuffer.Length());
    
                // Switch buffers. The old one was removed by ReAlloc.
                iRequestStream = largerBuffer;
                }
    
            // Can now append confidently
            iRequestStream->Des().Append(aBuffer);   
            }
	    else
	        {
	        iRequestStream = aBuffer.AllocL();
	      	iCurrentTestParam->SetAuthReqRes();
	        }
	    }
	   iInputStream->ReceivedDataRes();	    
	ProcessRequestStreamL();
	}
	
void CLocalTestServerStreamManager::SecureServerCnf()
	{
	}
	
void CLocalTestServerStreamManager::InputStreamCloseInd(TInt /* aError */)
	{
	iInputStream = NULL;		
	}
	
void CLocalTestServerStreamManager::MInputStreamObserver_Reserved()
	{
	}
	
MHttpResponse* CLocalTestServerStreamManager::CurrentResponse()
	{
	return NULL;		
	}
	
void CLocalTestServerStreamManager::OnReceiveTimeOut()	
	{
	}
	
// From MOutputStreamObserver
void CLocalTestServerStreamManager::SendDataCnfL()
	{
   _LIT(KTxtSendData, "Stream Manager - Sending data.");
    iTestUtils.LogIt(KTxtSendData());
	
	if(iSendingBodyData)
	    {
	    SendDataL ();
	    }
	else
	    {
	    if(iCurrentTestParam->RequestBody().Length() > 0)
	        {
	        iInputStream->ShutdownReq();
	        return;
	        }
	    if(iRequestStream == NULL)
	        return;
	    // Remove the current request from the request stream.
	    TPtr8 ptrStream = iRequestStream->Des();
	    TInt lenToRemove = iCurrentTestParam->RawRequest().Length();
	    TPtrC8 dataWindow;
	    if(lenToRemove < iRequestStream->Length())
	        {
	        dataWindow.Set(iRequestStream->Mid(lenToRemove));
	        }
	    TPtrC8 dataToCompare(iRequestStream->Ptr(), lenToRemove);
	    if(!iCurrentTestParam->IsFileSending() && iCurrentTestParam->RawRequest().CompareF(dataToCompare) != 0)
	        {
	        User::Leave(KErrCorrupt);
	        }
	    delete iRequestStream;
        iRequestStream = NULL;
        iRequestStream = dataWindow.AllocL();
	        
	    if(iRequestStream->Length() > 0)
	        {
	        iCurrentTestParam = NULL;
	        iSendingBodyData = EFalse;
	        ProcessRequestStreamL();
	        }
	    }
	}
	
void CLocalTestServerStreamManager::SecureClientCnf()
	{
	}
	
void CLocalTestServerStreamManager::OutputStreamCloseInd(TInt /* aError */)
	{
	iOutputStream = NULL;		
	}
	
void CLocalTestServerStreamManager::MOutputStreamObserver_Reserved()
	{
	}

void CLocalTestServerStreamManager::OnSendTimeOut()
    {
    }

TInt CLocalTestServerStreamManager::SendTimeOutVal()
	{
	return 0;
	}

void CLocalTestServerStreamManager::SendDataL()
    {
    delete iResponseStream;
    iResponseStream = NULL;
    if(!iSendingBodyData)
        {
        iResponseStream = iCurrentTestParam->RawResponse().AllocL();
        iSendingBodyData = ETrue;
        }
    else
        {
        delete iResponseStream;
        iResponseStream = NULL;
        _LIT8(KPostMethod, "POST");
        if(iCurrentTestParam->RequestBody().Length() == 0 && iCurrentTestParam->Method().CompareF(KPostMethod()) == 0)
            {
            iCurrentTestParam->SinkFile().Close();
            iInputStream->ShutdownReq();
            return;
            }
        if(iCurrentTestParam->IsFileSending())
            {
            iResponseStream = iCurrentTestParam->ReadFromFile().AllocL();
            iSendingBodyData = iCurrentTestParam->IsPendingRead();
            }
        else
            {
            if(iCurrentTestParam->ResponseBody().Length() > 0)
                {
                iResponseStream = iCurrentTestParam->ResponseBody().AllocL();
                }
            iSendingBodyData = EFalse;
            }
        
        
        }
    if(iResponseStream)
        iOutputStream->SendDataReqL(*iResponseStream);
    }

void CLocalTestServerStreamManager::ProcessRequestStreamL()
    {
    if(iSendingBodyData)
        return;
    _LIT8(KResponseTimeoutURI, "GET /index17.html HTTP/1.1\r\nHost: 127.0.0.1\r\nUser-Agent: HTTP Client API Test\r\n\r\n");
    // First see whether we received a request. by checking whether the startline
    // exists.
    _LIT8(KLineEnd, "\r\n");
    TPtrC8 ptrStream(*iRequestStream);
    TInt pos = ptrStream.FindF(KLineEnd);
    if(pos != KErrNotFound)
        {
        // Ok now we have a URI that matches. Now check if the URI is a timeout URI.
        if(ptrStream.CompareF(KResponseTimeoutURI) != 0)
            {
            _LIT8(KPostMethod, "POST");
            TPtrC8 startLine = ptrStream.Left(pos);
            iCurrentTestParam = iTestParamArray.MatchingParam(startLine); 
            if(iCurrentTestParam && !iCurrentTestParam->NeedDisconnection() &&  iCurrentTestParam->Method().CompareF(KPostMethod) != 0
                    && (iCurrentTestParam->ResponseBody().Length() > 0 || iCurrentTestParam->IsFileSending()))
                {            
                SendDataL();
                }
            else if(iCurrentTestParam && iCurrentTestParam->NeedDisconnection())
                {
                iInputStream->ShutdownReq();
                } 
            else if(iCurrentTestParam && (ptrStream.Length() == iCurrentTestParam->RawRequest().Length()) && !iCurrentTestParam->IsFileSending())
                {
                SendDataL();
                }
            
            if(iCurrentTestParam && iCurrentTestParam->Method().CompareF(KPostMethod) == 0 && iCurrentTestParam->IsFileSending() && iCurrentTestParam->RequestBody().Length() == 0)
                {
                iWritingUploadFile = ETrue;
                }
            }
        }
    if(iWritingUploadFile)
        {
        iCurrentTestParam->WriteToFile(ptrStream);
        delete iRequestStream;
        iRequestStream = NULL;
        TInt reqLen =  iCurrentTestParam->RawRequest().Length();
        TInt totalLen = (reqLen + iCurrentTestParam->SendingFileSize());
        if(iCurrentTestParam->ReceivingFileSize() == totalLen)
            {
            SendDataL();
            }
        }
    }
