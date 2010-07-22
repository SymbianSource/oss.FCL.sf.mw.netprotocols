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

#include "chttpclienttestparams.h"
#include <testexecuteserverbase.h>
#include "thttpheadervaluevariant.h"
#include <escapeutils.h>
#include <tinternetdate.h>

_LIT(KMethod, "Method");
_LIT(KUri, "Uri");
_LIT(KRawRequest, "RawRequest");
_LIT(KRawResponse, "RawResponse");
_LIT(KRawEncodedAuthenticationRequest, "RawEncodedAuthenticationRequest");
_LIT(KRawFinalResponse, "RawFinalResponse");
_LIT(KResponseBody, "ResponseBody");
_LIT(KStatusCode, "StatusCode");
_LIT(KResponseFieldCount, "ResponseFieldCount");
_LIT(KHeader, "ResponseHeader%d");
_LIT(KHeaderValue, "ResponseHeader%d.Value%d");
_LIT(KHeaderValueCount, "ResponseHeader%dValueCount");
_LIT(KFieldParamCount, "ResponseHeader%d.Field%dParamCount");
_LIT(KFieldParamName, "ResponseHeader%d.Field%d.ParamName%d");
_LIT(KFieldParamValue, "ResponseHeader%d.Field%d.ParamValue%d");
_LIT(KHeaderParamCount, "ResponseHeader%dParamCount");
_LIT(KHeaderParamName, "ResponseHeader%d.ParamName%d");
_LIT(KHeaderParamValue, "ResponseHeader%d.ParamValue%d");
_LIT(KResponseFileName, "ResponseFileName");
_LIT(KResponseFileNameStore, "ResponseFileNameStore");
_LIT(KRequestFileName, "RequestFileName");
_LIT(KRequestFileNameStore, "RequestFileNameStore");


_LIT(KRequestFieldCount, "RequestFieldCount");
_LIT(KRequestHeader, "RequestHeader%d");
_LIT(KRequestHeaderValueCount, "RequestHeader%dValueCount");
_LIT(KRequestHeaderValue, "RequestHeader%d.Value%d");
_LIT(KRequestHeaderValueParamCount, "RequestHeader%d.Value%dParamCount");
_LIT(KRequestHeaderValueParamName, "RequestHeader%d.Value%d.ParamName%d");
_LIT(KRequestHeaderValueParamValue, "RequestHeader%d.Value%d.ParamValue%d");

_LIT(KRequestBody, "RequestBody");
_LIT(KConnDisconnect, "ConnDisconnect");
_LIT(KExpectedError, "ExpectedError");
_LIT(KNoRetryOnDisconnect, "NoRetryOnDisconnect");
_LIT(KResponseDynamicAndStatic, "ResponseDynamicAndStatic");
_LIT(KOnlineTest, "OnlineTest");
_LIT(KProxyAddress, "ProxyAddress");
_LIT(KResponseTimeout, "ResponseTimeout");  
_LIT(KRedirection, "Redirection"); 
_LIT(KNotifyTransferProgress, "NotifyTransferProgress");
_LIT(KSetAuthentication, "SetAuthentication");

_LIT8(KScriptLF8,   "\\n");
_LIT8(KReplaceLF8,  "\n");
_LIT8(KScriptCR8,   "\\r");
_LIT8(KReplaceCR8,  "\r");


void ParseAndReplaceCRLF(HBufC8* aHBufPtr)
    {
    TInt crPos = 0;
    TInt lfPos = 0;
    do
      {
      crPos = ((aHBufPtr->Des()).Find(KScriptCR8()));
      if (crPos != KErrNotFound)
        {
        (aHBufPtr->Des()).Replace(crPos,2, KReplaceCR8());
        }           
      }while (crPos != KErrNotFound );


    do
      {
      lfPos = ((aHBufPtr->Des()).Find(KScriptLF8()));
      if (lfPos != KErrNotFound)
        {
        (aHBufPtr->Des()).Replace(lfPos,2, KReplaceLF8());
        }            
      }while (lfPos != KErrNotFound );    
    }

CHttpClientTestParams* CHttpClientTestParams::NewL(CTestStep& aTestCase)
    {
    CHttpClientTestParams* self = new(ELeave) CHttpClientTestParams;
    CleanupStack::PushL(self);
    self->ConstructL(aTestCase);
    CleanupStack::Pop();
    return self;
    } 



void CHttpClientTestParams::ConstructL(CTestStep& aTestCase)
    {
    TPtrC method;
    TPtrC uri;
    TPtrC rawRequest;
    TPtrC rawResponse;
    TPtrC rawEncodedAuthenticationRequest;
    TPtrC rawFinalResponse;
    if(!aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KMethod(), method) || 
            !aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KUri(), uri) ||
             !aTestCase.GetIntFromConfig(aTestCase.ConfigSection(), KStatusCode(), iStatusCode))
        {
        User::Leave(KErrCorrupt);
        }
    aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KRawRequest(), rawRequest);
    aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KRawResponse(), rawResponse);
    aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KRawEncodedAuthenticationRequest(), rawEncodedAuthenticationRequest);
    aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KRawFinalResponse(), rawFinalResponse);
    
    iMethod = EscapeUtils::ConvertFromUnicodeToUtf8L(method);
    iUri = EscapeUtils::ConvertFromUnicodeToUtf8L(uri);
    iRawRequest = EscapeUtils::ConvertFromUnicodeToUtf8L(rawRequest);
    iRawResponse = EscapeUtils::ConvertFromUnicodeToUtf8L(rawResponse);
    iEncodedAuthenticationRequest = EscapeUtils::ConvertFromUnicodeToUtf8L(rawEncodedAuthenticationRequest);
    iFinalResponse = EscapeUtils::ConvertFromUnicodeToUtf8L(rawFinalResponse);
       
    ParseAndReplaceCRLF(iRawRequest);
    ParseAndReplaceCRLF(iRawResponse);
    ParseAndReplaceCRLF(iEncodedAuthenticationRequest);
    ParseAndReplaceCRLF(iFinalResponse);

    
    
    TPtrC responseBody;
    if(aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KResponseBody(), responseBody))
        {
        iResponseBody = EscapeUtils::ConvertFromUnicodeToUtf8L(responseBody);
        }
    
    TPtrC requestBody;
    if(aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KRequestBody(), requestBody))
        {
        iRequestBody = EscapeUtils::ConvertFromUnicodeToUtf8L(requestBody);
        }
    
    
    // Now read the headers
    // Read the number of headers
    TInt responseHeaderCount = 0;
    aTestCase.GetIntFromConfig(aTestCase.ConfigSection(), KResponseFieldCount(), responseHeaderCount);
    for(TInt i = 0; i < responseHeaderCount; ++i)
        {
        CHeaderInfo* headerInfo = new(ELeave) CHeaderInfo;
        CleanupStack::PushL(headerInfo);
        TBuf<32> headerFormat;
        headerFormat.Format(KHeader(), i + 1);
        TPtrC headerName;
        aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), headerFormat, headerName);
        headerInfo->iFieldName = EscapeUtils::ConvertFromUnicodeToUtf8L(headerName);
        TInt headerValueCount = 0;        
        TBuf<32> headerValueCountStr;
        headerValueCountStr.Format(KHeaderValueCount(), i + 1);        
        aTestCase.GetIntFromConfig(aTestCase.ConfigSection(), headerValueCountStr, headerValueCount);        
        for(TInt j = 0; j < headerValueCount; ++j)
            {
            TBuf<32> headerValueFormat;
            headerValueFormat.Format(KHeaderValue(), i + 1, j+ 1);
            TPtrC headerValue;
            aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), headerValueFormat, headerValue);
            CHeaderFieldInfo* fieldInfo = new(ELeave) CHeaderFieldInfo;
            CleanupStack::PushL(fieldInfo);
            fieldInfo->iFieldValue = EscapeUtils::ConvertFromUnicodeToUtf8L(headerValue);
            
            // Check this field has params.
            TBuf<32> headerFieldParamCount;
            headerFieldParamCount.Format(KFieldParamCount(), i + 1, j + 1);
            TInt paramCount = 0;
            aTestCase.GetIntFromConfig(aTestCase.ConfigSection(), headerFieldParamCount, paramCount);
            for(TInt k = 0; k < paramCount; ++k)
                {
                TBuf<48> fieldParamName;
                TBuf<48> fieldParamValue;
                fieldParamName.Format(KFieldParamName, i + 1, j + 1, k + 1);
                fieldParamValue.Format(KFieldParamValue, i + 1, j + 1, k + 1);             
                TPtrC paramName;
                TPtrC paramValue;
                aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), fieldParamName, paramName);
                aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), fieldParamValue, paramValue);
                CHeaderFieldParamInfo* paramInfo = new(ELeave) CHeaderFieldParamInfo;
                CleanupStack::PushL(paramInfo);
                paramInfo->iParamName = EscapeUtils::ConvertFromUnicodeToUtf8L(paramName);
                paramInfo->iParamValue = EscapeUtils::ConvertFromUnicodeToUtf8L(paramValue);                
                fieldInfo->iFieldParamInfos.AppendL(paramInfo);
                CleanupStack::Pop(); // paramInfo                
                }
            
            headerInfo->iFieldInfos.AppendL(fieldInfo); 
            CleanupStack::Pop(); // fieldInfo
            }        
        // Check if the header has some params
        TBuf<48> headerParamCount;
        headerParamCount.Format(KHeaderParamCount(), i + 1);
        TInt hdrParamCount = 0;
        aTestCase.GetIntFromConfig(aTestCase.ConfigSection(), headerParamCount, hdrParamCount);   
        for(TInt x = 0; x < hdrParamCount; ++x)
            {
            TBuf<48> headerParamName;
            TBuf<48> headerParamValue;
            headerParamName.Format(KHeaderParamName(), i + 1, x + 1);
            headerParamValue.Format(KHeaderParamValue(), i + 1, x + 1);
            TPtrC paramName;
            TPtrC paramValue;
            aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), headerParamName, paramName);
            aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), headerParamValue, paramValue);
            CHeaderFieldParamInfo* paramInfo = new (ELeave) CHeaderFieldParamInfo;
            CleanupStack::PushL(paramInfo);
            paramInfo->iParamName = EscapeUtils::ConvertFromUnicodeToUtf8L(paramName);
            paramInfo->iParamValue = EscapeUtils::ConvertFromUnicodeToUtf8L(paramValue);
            headerInfo->iParamInfos.AppendL(paramInfo);
            CleanupStack::Pop(); // paramInfo
            }
        iResponseHeaderInfos.AppendL(headerInfo);
        CleanupStack::Pop(); // headerInfo
        }

    TPtrC fileName;
    aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KResponseFileName(), fileName);
    if(fileName.Length() > 0)
        {
        iFileSender = CFileSender::NewL(fileName);        
        aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KResponseFileNameStore(), fileName);
        if(fileName.Length() == 0)
            {
            User::Leave(KErrCorrupt);
            }
        iFileReceiver = CFileReceiver::NewL(fileName);
        }

    
    TPtrC ulFileName;
    aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KRequestFileName(), ulFileName);
    if(ulFileName.Length() > 0)
        {
        iFileSender = CFileSender::NewL(ulFileName);
        aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KRequestFileNameStore(), ulFileName);
        if(ulFileName.Length() == 0)
             {
             User::Leave(KErrCorrupt);
             }
         iFileReceiver = CFileReceiver::NewL(ulFileName);        
        }
    
    // Read request headers 
     TInt reqFieldCount = 0;
    aTestCase.GetIntFromConfig(aTestCase.ConfigSection(), KRequestFieldCount(), reqFieldCount);
    
    for(TInt ii = 0; ii < reqFieldCount; ++ii)
        {
        // Read the request header
        TBuf<64> reqHeaderStr;
        reqHeaderStr.Format(KRequestHeader(), ii + 1);
        TPtrC reqHeaderName;
        aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), reqHeaderStr, reqHeaderName);
        CHeaderInfo* headerInfo = new(ELeave) CHeaderInfo;
        CleanupStack::PushL(headerInfo);
        headerInfo->iFieldName = EscapeUtils::ConvertFromUnicodeToUtf8L(reqHeaderName);
        
        // Now read how many fields are present
        reqHeaderStr.Format(KRequestHeaderValueCount(), ii + 1);
        TInt reqHdrValueCount = 0;
        aTestCase.GetIntFromConfig(aTestCase.ConfigSection(), reqHeaderStr, reqHdrValueCount);
        for(TInt jj = 0; jj < reqHdrValueCount; ++jj)
            {
            TPtrC reqHeaderVal;
            reqHeaderStr.Format(KRequestHeaderValue(), ii + 1, jj + 1);
            aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), reqHeaderStr, reqHeaderVal);
            CHeaderFieldInfo* info = new (ELeave) CHeaderFieldInfo;
            CleanupStack::PushL(info);
            info->iFieldValue = EscapeUtils::ConvertFromUnicodeToUtf8L(reqHeaderVal);
            reqHeaderStr.Format(KRequestHeaderValueParamCount, ii + 1, jj + 1);
            TInt paramCount = 0;
            aTestCase.GetIntFromConfig(aTestCase.ConfigSection(), reqHeaderStr, paramCount);
            for(TInt kk = 0; kk < paramCount; ++kk)
                {
                reqHeaderStr.Format(KRequestHeaderValueParamName(), ii + 1, jj + 1, kk + 1);
                TPtrC name;
                TPtrC value;
                aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), reqHeaderStr, name);
                reqHeaderStr.Format(KRequestHeaderValueParamValue(), ii + 1, jj + 1, kk + 1);
                aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), reqHeaderStr, value);
                
                CHeaderFieldParamInfo* paramInfo = new (ELeave) CHeaderFieldParamInfo;
                CleanupStack::PushL(paramInfo);
                paramInfo->iParamName = EscapeUtils::ConvertFromUnicodeToUtf8L(name);
                paramInfo->iParamValue = EscapeUtils::ConvertFromUnicodeToUtf8L(value);
                info->iFieldParamInfos.AppendL(paramInfo);
                CleanupStack::Pop(); // CHeaderFieldParamInfo
                }
            headerInfo->iFieldInfos.AppendL(info);
            CleanupStack::Pop(); // info
            }
        
        iRequestHeaderInfos.AppendL(headerInfo);
        CleanupStack::Pop();      
        }         
    
    iConnDisconnect = EFalse;
    aTestCase.GetBoolFromConfig(aTestCase.ConfigSection(), KConnDisconnect(), iConnDisconnect);
    
    iExpectedError = KErrNone;    
    aTestCase.GetIntFromConfig(aTestCase.ConfigSection(), KExpectedError(), iExpectedError);
    
    iNoRetryOnDisconnect = EFalse;
    aTestCase.GetBoolFromConfig(aTestCase.ConfigSection(), KNoRetryOnDisconnect(), iNoRetryOnDisconnect);
    
    iResponseDynamicAndStatic = EFalse;
    aTestCase.GetBoolFromConfig(aTestCase.ConfigSection(), KResponseDynamicAndStatic(), iResponseDynamicAndStatic);
    
    iOnlineTest = EFalse;
    aTestCase.GetBoolFromConfig(aTestCase.ConfigSection(), KOnlineTest(), iOnlineTest);
        
    iRedirection = EFalse;
    aTestCase.GetBoolFromConfig(aTestCase.ConfigSection(), KRedirection(), iRedirection);
   
    iResponseTimeout = EFalse;
    aTestCase.GetBoolFromConfig(aTestCase.ConfigSection(), KResponseTimeout(), iResponseTimeout);
    
    iNotifyTransferProgress = EFalse;
    aTestCase.GetBoolFromConfig(aTestCase.ConfigSection(), KNotifyTransferProgress(), iNotifyTransferProgress);
    
    iSetAuthentication = EFalse;
    aTestCase.GetBoolFromConfig(aTestCase.ConfigSection(), KSetAuthentication(), iSetAuthentication);
    
    TPtrC proxyAddress;
    if(aTestCase.GetStringFromConfig(aTestCase.ConfigSection(), KProxyAddress, proxyAddress))
        {
        iProxyAddress = EscapeUtils::ConvertFromUnicodeToUtf8L(proxyAddress);
        }
    }


CHttpClientTestParams::~CHttpClientTestParams()
    {
    delete iFileSender;
    delete iMethod;
    delete iUri;
    delete iRawRequest;
    delete iEncodedAuthenticationRequest;
    delete iFinalResponse;
    delete iRawResponse;
    delete iResponseBody;
    iResponseHeaderInfos.ResetAndDestroy();
    }

TInt CHttpClientTestParams::StatusCode() const
    {
    return iStatusCode;
    }

const TDesC8& CHttpClientTestParams::Method() const
    {
    return *iMethod;
    }

const TDesC8& CHttpClientTestParams::Uri() const
    {
    return *iUri;
    }

const TDesC8& CHttpClientTestParams::RawRequest() const
    {
    return *iRawRequest;
    }

const TDesC8& CHttpClientTestParams::RawResponse() const
    {
    return *iRawResponse;
    }

const TDesC8& CHttpClientTestParams::EncodedAuthenticationRequest() const
    {
    return *iEncodedAuthenticationRequest;
    }

void CHttpClientTestParams::SetAuthReqRes() 
    {
    if (iRawRequest)
        {
        delete iRawRequest;
        iRawRequest = NULL;
        }        
    if (iRawResponse)
        {
        delete iRawResponse;
        iRawResponse = NULL;
        }
    iRawRequest = iEncodedAuthenticationRequest;
    iRawResponse = iFinalResponse;
    
    iEncodedAuthenticationRequest = NULL;
    iFinalResponse = NULL;
    }

const TDesC8& CHttpClientTestParams::ResponseBody() const
    {
    if(iResponseBody)
        return *iResponseBody;
    return KNullDesC8();
    }

TBool   CHttpClientTestParams::IsFileSending() const
    {
    return (iFileSender != NULL);
    }

const TDesC8& CHttpClientTestParams::ReadFromFile()
    {
    return iFileSender->Read();
    }

TBool   CHttpClientTestParams::IsPendingRead() const
    {
    return iFileSender->MoreToRead();
    }

TInt CHttpClientTestParams::WriteToFile(const TDesC8& aData)
    {
    return iFileReceiver->Write(aData);
    }

RFile&  CHttpClientTestParams::SourceFile()
    {
    return iFileSender->iFile;
    }

RFile&  CHttpClientTestParams::SinkFile()
    {
    return iFileReceiver->iFile;
    }

TBool   CHttpClientTestParams::NeedDisconnection() const
    {
    return iConnDisconnect;
    }

TInt CHttpClientTestParams::ExpectedError() const
    {
    return iExpectedError;
    }

TBool CHttpClientTestParams::NoRetryOnDisconnect() const
    {
    return iNoRetryOnDisconnect;
    }

TBool CHttpClientTestParams::ResponseTimeoutEnable() const
    {
    return iResponseTimeout;
    }

TBool CHttpClientTestParams::IsRedirecting() const
    {
    return iRedirection;
    }

TBool CHttpClientTestParams::TransferProgress() const
    {
    return iNotifyTransferProgress;
    }

TBool CHttpClientTestParams::IsAuthenticationReqd() const   
    {
    return iSetAuthentication;
    }

TBool CHttpClientTestParams::CheckVariantValue(const THttpHeaderValueVariant& aVariant, const TDesC8& aValueToMatch)
    {
    TBool ret = EFalse;
    switch(aVariant.Type())
        {
        case THttpHeaderValueVariant::EIntType:
            {
            _LIT8(KIntType, "%d");
            TBuf8<16>  buf;
            buf.Format(KIntType(), aVariant.Int());
            ret = (buf.CompareF(aValueToMatch) == 0);                    
            }
        break;
        
        case THttpHeaderValueVariant::EStrType:
            {
            ret = (aVariant.Str().CompareF(aValueToMatch) == 0);
            }
        break;
        
        case THttpHeaderValueVariant::EDateTimeType:
            {
            TInternetDate internetDate(aVariant.DateTime());
            HBufC8* dateTimeStr = internetDate.InternetDateTimeL(TInternetDate::ERfc1123Format);
            ret = aValueToMatch.CompareF(*dateTimeStr) == 0;
            delete dateTimeStr;
            }
        break;
        
        default:
        User::Invariant();
        }    
    return ret;
    }

CHeaderInfo* CHttpClientTestParams::FindHeaderInfo(const TDesC8& aHeaderName)
    {
    const TUint count = iResponseHeaderInfos.Count();
    for(TInt i = 0; i < count; ++i)
        {
        if(aHeaderName.CompareF(*iResponseHeaderInfos[i]->iFieldName) == 0)
            {
            return iResponseHeaderInfos[i];
            }
        }
    return NULL;        
    }

TBool CHttpClientTestParams::MatchPostFileContent()
    {
    CFileSender* sender = CFileSender::NewL(iFileSender->FileName());
    CleanupStack::PushL(sender);
    CFileSender* sender2 = CFileSender::NewL(iFileReceiver->FileName());
    CleanupStack::PushL(sender2);
    sender2->Read(RawRequest().Length());
    TBool ret = ETrue;
    while(sender->MoreToRead())
        {
        if(!sender2->MoreToRead())
            {
            ret = EFalse;
            break;
            }
        
        if(sender->Read().CompareF(sender2->Read()) != 0)
            {
            ret = EFalse;
            break;
            }
        }
    CleanupStack::Pop(2); // sender/sender2
    return ret;     
    }

TBool   CHttpClientTestParams::MatchFileContent()
    {
    CFileSender* sender = CFileSender::NewL(iFileSender->FileName());
    CleanupStack::PushL(sender);
    CFileSender* sender2 = CFileSender::NewL(iFileReceiver->FileName());
    CleanupStack::PushL(sender2);
    TBool ret = ETrue;
    while(sender->MoreToRead())
        {
        if(!sender2->MoreToRead())
            {
            ret = EFalse;
            break;
            }
        
        if(sender->Read().CompareF(sender2->Read()) != 0)
            {
            ret = EFalse;
            break;
            }
        }
    CleanupStack::Pop(2); // sender/sender2
    return ret;    
    }

CHttpClientTestParamArray::CHttpClientTestParamArray()
    {
    
    }

CHttpClientTestParamArray::~CHttpClientTestParamArray()
    {
    iTestParamArray.ResetAndDestroy();
    }

void CHttpClientTestParamArray::AddTestParams(CHttpClientTestParams* aParams)
    {
    iTestParamArray.Append(aParams);
    }

CHttpClientTestParams* CHttpClientTestParamArray::MatchingParam(const TDesC8& aData)
    {
    CHttpClientTestParams* param = NULL;
    TInt count = iTestParamArray.Count();
    for(TUint i = 0; i < count; ++i)
        {
        TInt startPos = aData.Locate(' ');
        TInt reversePos = aData.LocateReverse(' ') - 1;
        TPtrC8 ptr = aData.Mid(startPos + 1, reversePos - startPos);         
         if (iTestParamArray[i]->Uri().FindF(ptr) != KErrNotFound)
             {
             param = iTestParamArray[i];             
             break;
             }
        }
    return param;            
    }

//Parse Replace \n & \r
void ParseReplaceCRLF(HBufC8* aHBufPtr)
    {
    TInt crPos = 0;
    TInt lfPos = 0;
    do
        {

        crPos = ((aHBufPtr->Des()).Find(KScriptCR8()));
        if (crPos != KErrNotFound)
            {
            (aHBufPtr->Des()).Replace(crPos,2, KReplaceCR8());
            }
            
        }
    while (crPos != KErrNotFound );


    do
        {

        lfPos = ((aHBufPtr->Des()).Find(KScriptLF8()));
        if (lfPos != KErrNotFound)
            {
            (aHBufPtr->Des()).Replace(lfPos,2, KReplaceLF8());
            }
            
        }
    while (lfPos != KErrNotFound );
    
    }


CFileSender* CFileSender::NewL(const TDesC& aFileName)
    {
    CFileSender* self = new(ELeave) CFileSender;
    CleanupStack::PushL(self);
    self->ConstructL(aFileName);
    CleanupStack::Pop(); // self
    return self;
    }


void CFileSender::ConstructL(const TDesC& aFileName)
    {
    iFileName = aFileName.AllocL();
    User::LeaveIfError(iFs.Connect());
    iBuffer.CreateL(16 * 1024);
    User::LeaveIfError(iFile.Open(iFs, aFileName, EFileShareReadersOnly));
    User::LeaveIfError(iFile.Size(iFileSize));
    }

CFileSender::~CFileSender()
    {
    delete iFileName;
    iBuffer.Close();
    iFile.Close();
    iFs.Close();
    }

const TDesC8& CFileSender::Read(TInt aSize)
    {
    iBuffer.Zero();
    iFile.Read(iBuffer, aSize);
    iFileSize -= iBuffer.Length();
    return iBuffer;    
    }

const TDesC8& CFileSender::Read()
    {
    iBuffer.Zero();
    iFile.Read(iBuffer);
    iFileSize -= iBuffer.Length();
    return iBuffer;
    }

TInt CFileSender::Size()
    {
    TInt size;
    iFile.Size(size);
    return size;
    }

TBool   CFileSender::MoreToRead() const
    {
    return iFileSize > 0;
    }

CFileSender::CFileSender()
    {
    
    }

const TDesC& CFileSender::FileName() const
    {
    return *iFileName;
    }

CFileReceiver* CFileReceiver::NewL(const TDesC& aFileName)
    {
    CFileReceiver* self = new(ELeave) CFileReceiver;
    CleanupStack::PushL(self);
    self->ConstructL(aFileName);
    CleanupStack::Pop(); // self
    return self;
    }

 TInt CFileReceiver::Write(const TDesC8& aData)
     {
     return iFile.Write(aData);
     }
 
 CFileReceiver::~CFileReceiver()
     {
     iFile.Close();
     iFs.Close();
     delete iFileName;
     }
 
 void CFileReceiver::ConstructL(const TDesC& aFileName)
     {
     iFileName = aFileName.AllocL();
     User::LeaveIfError(iFs.Connect());
     if(iFile.Open(iFs, aFileName, EFileWrite) != KErrNone)
         {
         User::LeaveIfError(iFile.Create(iFs, aFileName, EFileWrite));
         }
     }
 
 RFile& CFileReceiver::File()
     {
     return iFile;
     }

 const TDesC& CFileReceiver::FileName() const
     {
     return *iFileName;
     }
