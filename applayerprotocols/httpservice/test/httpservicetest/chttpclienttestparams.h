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

#ifndef __CHTTPCLIENTTESTPARAMS_H__
#define __CHTTPCLIENTTESTPARAMS_H__

#include <e32base.h>
#include <f32file.h>
class CTestStep;  

class CHeaderFieldParamInfo : public CBase
    {
public:
    ~CHeaderFieldParamInfo()
        {
        delete iParamName;
        delete iParamValue;
        }
    
    HBufC8*     iParamName;
    HBufC8*     iParamValue;
    };

class CHeaderFieldInfo : public CBase
    {
public:
    ~CHeaderFieldInfo()
        {
        delete iFieldValue;
        iFieldParamInfos.ResetAndDestroy();
        }
    HBufC8*                                 iFieldValue;
    RPointerArray<CHeaderFieldParamInfo>    iFieldParamInfos;
    };

class CHeaderInfo : public CBase
    {
    
    public:
    ~CHeaderInfo()
        {
        delete iFieldName;
        iFieldInfos.ResetAndDestroy();
        iParamInfos.ResetAndDestroy();
        }
    
    HBufC8*                             iFieldName;
    RPointerArray<CHeaderFieldInfo>     iFieldInfos;
    RPointerArray<CHeaderFieldParamInfo>     iParamInfos;
    };


class CFileSender : public CBase
    {
    friend class CHttpClientTestParams;
public:    
    static CFileSender* NewL(const TDesC& aFileName);
    ~CFileSender();
    const TDesC8& Read();
    const TDesC8& Read(TInt aSize);
    TInt Size();
    TBool   MoreToRead() const;
    const TDesC& FileName() const;
    
private:
    CFileSender();
    void ConstructL(const TDesC& aFileName);
private:
    RFs     iFs;
    RFile   iFile;
    RBuf8   iBuffer;
    TInt    iFileSize;
    HBufC*  iFileName;
    };

class CFileReceiver : public CBase
    {
friend class CHttpClientTestParams;    
public:
    static CFileReceiver* NewL(const TDesC& aFileName);
    TInt Write(const TDesC8& aData);
    ~CFileReceiver();
    RFile& File();
    const TDesC& FileName() const;
    TInt Size()
        {
        TInt size;
        iFile.Size(size);
        return size;
        }
private:
    void ConstructL(const TDesC& aFileName);
private:
    RFs iFs;
    RFile   iFile;
    HBufC*  iFileName;    
    };

class THttpHeaderValueVariant;

class CHttpClientTestParams : public CBase
	{
	public:
	static CHttpClientTestParams* NewL(CTestStep& aTestCase); 
	~CHttpClientTestParams();
	
	
	TInt StatusCode() const;
	const TDesC8& Method() const;
	const TDesC8& Uri() const;
	const TDesC8& RawRequest() const;
	const TDesC8& RawResponse() const;
	const TDesC8& EncodedAuthenticationRequest() const;
	void SetAuthReqRes();
	const TDesC8& ResponseBody() const;
	const RPointerArray<CHeaderInfo>& HeaderInfos() const
	{
	return iResponseHeaderInfos;
	}

	const RPointerArray<CHeaderInfo>& RequestHeaderInfos() const
	   {
	   return iRequestHeaderInfos;
	   }

    TBool CheckVariantValue(const THttpHeaderValueVariant& aVariant, const TDesC8& aValueToMatch);
    CHeaderInfo* FindHeaderInfo(const TDesC8& aHeaderName);     
    
    TBool   IsFileSending() const;
    TBool   IsPendingRead() const;
    const TDesC8& ReadFromFile();
    TInt WriteToFile(const TDesC8& aData);
    
    RFile&  SinkFile();
    RFile&  SourceFile();
    TBool   MatchFileContent();
    TBool   MatchPostFileContent();
    TBool   NeedDisconnection() const;
    TInt ExpectedError() const;
    TBool NoRetryOnDisconnect() const;
    TBool ResponseTimeoutEnable() const; 
    TBool IsRedirecting() const;
    TBool TransferProgress() const;
    TBool IsAuthenticationReqd() const;
    
    const TDesC8& RequestBody() const
    {
    if(iRequestBody)
        return *iRequestBody;
    return KNullDesC8();
    }
    TInt SendingFileSize()
        {
        return iFileSender->Size();
        }
    TInt ReceivingFileSize()
        {
        return iFileReceiver->Size();
        }
    TBool ResponseDynamicAndStatic()
        {
        return iResponseDynamicAndStatic;
        }
    TBool OnlineTest()
        {
        return iOnlineTest;
        }
    
    const TDesC8& ProxyAddress() const
        {
        if(iProxyAddress)
            return *iProxyAddress;
        return KNullDesC8();
        }
private:
    void ConstructL(CTestStep& aTestCase);
       
    
private:
    CFileSender*    iFileSender;
    CFileReceiver*  iFileReceiver;
    TBool           iConnDisconnect;
	TInt    iStatusCode;
	HBufC8*	iMethod;
	HBufC8*	iUri;
	HBufC8* iRawRequest;
	HBufC8* iRawResponse;
	HBufC8* iEncodedAuthenticationRequest;
	HBufC8* iFinalResponse;
	HBufC8*	iResponseBody;
	HBufC8* iRequestBody;
	TInt    iExpectedError;
	TBool    iNoRetryOnDisconnect;
	RPointerArray<CHeaderInfo>     iResponseHeaderInfos;
	RPointerArray<CHeaderInfo>     iRequestHeaderInfos;
	TBool          iResponseDynamicAndStatic;
	TBool          iOnlineTest;
	TBool          iResponseTimeout; 
	TBool          iRedirection;
	TBool          iNotifyTransferProgress;
	TBool          iSetAuthentication;
	HBufC8*        iProxyAddress;
	};

class CHttpClientTestParamArray : public CBase
    {
public:
    CHttpClientTestParamArray();
    ~CHttpClientTestParamArray();
    void AddTestParams(CHttpClientTestParams* aParams);
    CHttpClientTestParams* MatchingParam(const TDesC8& aData);
private:
    RPointerArray<CHttpClientTestParams> iTestParamArray;   
    };
        
#endif // __CHTTPCLIENTTESTPARAMS_H__
