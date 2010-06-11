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

#include "ctestclienthttpget.h"
#include <httpstringconstants.h>
#include "mlocaltestserverobserver.h"
#include "chttpclienttestparams.h"
#include <tinternetdate.h>
_LIT8(KAcceptHeader, "Accept");
_LIT8(KAcceptValue, "text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5");
_LIT(KTransferProgress, "File size: %d kb , Downloaded so far: %d kb");

CTestClientHttpSimpleGet* CTestClientHttpSimpleGet::NewL(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHttpService& aClient, const TDesC8& aUri, CHTTPTestUtils* aUtils)
    {
    CTestClientHttpSimpleGet* self = new(ELeave) CTestClientHttpSimpleGet(aParams, aObserver, aUtils);
    CleanupStack::PushL(self);
    self->ConstructL(aClient, aUri);
    CleanupStack::Pop(); // self
    return self;
    }  

CTestClientHttpSimpleGet::~CTestClientHttpSimpleGet()
    {
    }

CTestClientHttpSimpleGet::CTestClientHttpSimpleGet(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHTTPTestUtils* aUtils)
: iObserver(aObserver),
iParams(aParams),
iTestUtils(*aUtils)
    {
    
    }

void CTestClientHttpSimpleGet::ConstructL(CHttpService& aClient, const TDesC8& aUri)
    {
    TPtrC8 method;
    aClient.String(HTTP::EGET, method);
    CHttpClientTransaction::Create(aClient, method, aUri);   
    }

// From CHTTPClientTransaction
void CTestClientHttpSimpleGet::OnResponseHeaders()
    {
    // Check whether the status code matches
    if(iParams.StatusCode() != StatusCode())
        {
        iObserver.EndTest(KErrCorrupt);
        }
    
    // IF we are receiving a file. just sink the content to the file.
    // We will not check 
    if(iParams.IsFileSending())
        {
        CHttpClientTransaction::SetContentSink(iParams.SinkFile());
        return;
        }
    else
        {
        CHttpClientTransaction::SetContentSink(*this);
        }    
    TInt i = 0;
    const RPointerArray<CHeaderInfo>& hdrInfos = iParams.HeaderInfos();
    
    THttpHeaderIterator iter = ResponseHeaders();
    while(!iter.AtEnd())
        {
        CHeaderInfo* info = iParams.FindHeaderInfo(iter.Name());
        if(info == NULL)
            {
            iObserver.EndTest(KErrCorrupt);
            return;
            }
        
        TInt j = 0;
        THttpHeaderElementIterator elementIter = iter.Elements();
        while(!elementIter.AtEnd())
            {
            CHeaderFieldInfo* fieldInfo = info->iFieldInfos[j];
            THttpHeaderValueVariant value = elementIter.Value();
            if(!iParams.CheckVariantValue(value, *fieldInfo->iFieldValue))
                {
                iObserver.EndTest(KErrCorrupt);
                return;
                }
            
            // Check the params
            THttpHeaderParamIterator paramIter = elementIter.Parameter();
            TInt k = 0;
            while(!paramIter.AtEnd())
                {
                CHeaderFieldParamInfo* paramInfo = fieldInfo->iFieldParamInfos[k];
                // Check the param name and value
                if(paramInfo->iParamName->CompareF(paramIter.Name()) != 0)
                    {
                    iObserver.EndTest(KErrCorrupt);
                    return;
                    }
                THttpHeaderValueVariant value = paramIter.Value();
                if(!iParams.CheckVariantValue(value, *paramInfo->iParamValue) != 0)
                    {
                    iObserver.EndTest(KErrCorrupt);
                    return;
                    }                
                ++k;
                ++paramIter;                
                }
            ++j;
            ++elementIter;
            }
        TInt x = 0;
        THttpHeaderParamIterator hdrParamIter = iter.Parameters();
        while(!hdrParamIter.AtEnd())
            {
            CHeaderFieldParamInfo* paramInfo = info->iParamInfos[x];
            if(paramInfo->iParamName->CompareF(hdrParamIter.Name()) != 0)
                {
                iObserver.EndTest(KErrCorrupt);
                return;
                }
            THttpHeaderValueVariant value = hdrParamIter.Value();
            if(!iParams.CheckVariantValue(value, *paramInfo->iParamValue) != 0)
                {
                iObserver.EndTest(KErrCorrupt);
                return;
                }                
            ++x;
            ++hdrParamIter;
            }
        ++iter;
        ++i;
        }
    }

void CTestClientHttpSimpleGet::OnCompletion()
    {
    if(iParams.IsFileSending())
        {
        iParams.SinkFile().Close(); // Close the file
        TBool ret = ETrue;
        TRAPD(err, ret = iParams.MatchFileContent());
        if(err == KErrNone && ret)
            iObserver.EndTest(KErrNone);
        else
            iObserver.EndTest(KErrCorrupt);
        }
    else
        {
        iObserver.EndTest(KErrNone);
        }
    }

void CTestClientHttpSimpleGet::OnError(TInt aError)
    {
    if(aError == iParams.ExpectedError())
        iObserver.EndTest(KErrNone);
    else
        iObserver.EndTest(aError);    
    }

void CTestClientHttpSimpleGet::OnTransferProgress(TInt aTotal, TInt aTransffered)
    {
    if(iParams.TransferProgress())
        {
        iTestUtils.LogIt(KTransferProgress(),aTotal,aTransffered);
        }    
    }

TBool CTestClientHttpSimpleGet::OnRedirection()
    {
    if(iParams.IsRedirecting())
        return ETrue;
    else
        return EFalse;
    }

void CTestClientHttpSimpleGet::OnData(THttpContentSinkOp& aData)
    {
    if(iParams.ResponseDynamicAndStatic())
        {
        aData.Store();
        }
    else
        {
        TPtrC8 ptr;
        aData.GetData(ptr);
        if(ptr.CompareF(iParams.ResponseBody()) != 0)
            {
            iObserver.EndTest(KErrCorrupt);
            return;
            }
        aData.Release();
        }
    }


CTestClientHttpOnlineGet* CTestClientHttpOnlineGet::NewL(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHttpService& aClient, const TDesC8& aUri, CHTTPTestUtils* aUtils)
    {
    CTestClientHttpOnlineGet* self = new(ELeave) CTestClientHttpOnlineGet(aParams, aObserver, aUtils);
     CleanupStack::PushL(self);
     self->ConstructL(aClient, aUri);
     CleanupStack::Pop(); // self
     return self;    
    }

CTestClientHttpOnlineGet::CTestClientHttpOnlineGet(CHttpClientTestParams& aParams, MLocalTestServerObserver& aObserver, CHTTPTestUtils* aUtils)
: CTestClientHttpSimpleGet(aParams, aObserver, aUtils)
    {
    
    }

void CTestClientHttpOnlineGet::ConstructL(CHttpService& aClient, const TDesC8& aUri)
    {
    CTestClientHttpSimpleGet::ConstructL(aClient, aUri);
    _LIT8(KLocalHost, "127.0.0.1");
    if(aUri.FindF(KLocalHost()) == KErrNotFound)
        {
        CHttpClientTransaction::AddCustomRequestHeader(KAcceptHeader(), KAcceptValue());
        THttpHeaderValueVariant variant;    
        variant.SetInt(300);
        CHttpClientTransaction::AddRequestHeader(HTTP::EKeepAlive, variant);
        _LIT8(KEnUs, "en-us");
        variant.SetStr(KEnUs);
        CHttpClientTransaction::AddRequestHeader(HTTP::EAcceptLanguage, variant);
        _LIT8(KEn, "en");
        _LIT8(KQVal, "0.9");
        
        //The following functions SetDateTime() and DateTime() have been used to avoid the code coverage issues.
        TDateTime dateTime(2009, EDecember, 0, 0, 0, 0, 0);
        variant.SetDateTime(dateTime);
        
        TInternetDate internetDate(variant.DateTime());
    
        TLex8 lex(KQVal);
        TReal64 value = 0.;
        lex.Val(value);             
        THTTPHdrVal::TQConv q(value);
        TInt val2 = q;
        THttpHeaderValueVariant variant2(val2);
        variant.SetStr(KEn);
        CHttpClientTransaction::AddRequestHeader(HTTP::EAcceptLanguage, variant, HTTP::EQ, variant2);
        
        //The following two temporary objects have been created to avoid the code coverage issues.
        THttpContentSinkOp httpContentSinkOp;
        THttpContentSourceOp httpContentSourceOp;
        // Disable the pipelining
        DisablePipelining();
        }
    else
        {
        _LIT8(KAcceptAll, "*/*");
        CHttpClientTransaction::AddCustomRequestHeader(KAcceptHeader(), KAcceptAll());        
        }
    }

CTestClientHttpOnlineGet::~CTestClientHttpOnlineGet()
    {
    
    }

void CTestClientHttpOnlineGet::OnResponseHeaders()
    {
    // Check whether the status code matches
     if(iParams.StatusCode() != StatusCode())
         {
         iObserver.EndTest(KErrCorrupt);
         return;
         }
     _LIT8(KStatusText, "Ok");
     const TDesC8& statusText = StatusText();
     
     if(statusText.CompareF(KStatusText) != 0)
         {
         // we do not fail here. But comparison is failed. print that.
         }
     
     // Iterate the response headers
     THttpHeaderIterator iter;
     iter = ResponseHeaders();
     while(!iter.AtEnd())
         {
         iter++;
         }
    
     THttpHeaderElementIterator it;
     it = ResponseHeader(HTTP::ECacheControl);
     while(!it.AtEnd())
         {
         it++;
         }
     _LIT8(KContentType, "Content-Type");
     it = ResponseHeader(KContentType());
     while(!it.AtEnd())
         {
         THttpHeaderParamIterator it2;
         it2 = it.Parameter();
         while(!it2.AtEnd())
             {
             it2++;
             }
         it++;
         }     
     CHttpClientTransaction::SetContentSink(*this);
    }

void CTestClientHttpOnlineGet::OnCompletion()
    {
    iObserver.EndTest(KErrNone);
    }
 
void CTestClientHttpOnlineGet::OnData(THttpContentSinkOp& aData)
    {
    // Just release the data
    aData.Release();
    }
