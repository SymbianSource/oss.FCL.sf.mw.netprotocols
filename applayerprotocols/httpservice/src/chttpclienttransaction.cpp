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

#include "chttpclienttransaction.h"
#include "chttpclienttransactionimpl.h"
#include "httpclientutils.h"
#include "CHeaders.h"
#include "CHeaderField.h"
#include "mhttpdatareceiver.h"
#include "mhttpdatasender.h"

EXPORT_C THttpContentSinkOp::THttpContentSinkOp()
: iDataReceiver(NULL)
    {
    
    }

EXPORT_C TBool THttpContentSinkOp::GetData(TPtrC8& aData)
    {
    return iDataReceiver->GetData(aData);
    }

EXPORT_C void THttpContentSinkOp::Release()
    {
    iDataReceiver->Release();
    }

EXPORT_C void THttpContentSinkOp::Store() // Store into a file if a sink is provided.
    {
    iDataReceiver->Store();
    }

THttpContentSinkOp::THttpContentSinkOp(MHttpDataReceiver& aReceiver)
: iDataReceiver(&aReceiver)
    {    
    }
// --------------------------------------------------------------------------------------------

EXPORT_C THttpContentSourceOp::THttpContentSourceOp()
: iDataSender(NULL)
    {
    
    }

EXPORT_C void THttpContentSourceOp::Notify(const TDesC8& aData, TBool aLast /* = EFalse */)
    {    
    iDataSender->Notify(aData, aLast);
    }    

THttpContentSourceOp::THttpContentSourceOp(MHttpDataSender* aSender)
: iDataSender(aSender)
    {
    }

// --------------------------------------------------------------------------------------------

/**
 * Create the HTTP transaction by specifiying the method [GET, POST etc..] and a URI
 * 
 * @param aClient - CHttpService instance
 * @param aMethod - HTTP method. [GET, POST etc...]
 * @param aUri - URI
 * 
 * @return KErrNoMemory incase of failure otherwise KErrNone for success
 * 
 */

EXPORT_C TInt CHttpClientTransaction::Create(CHttpService& aClient, const TDesC8& aMethod, const TDesC8& aUri)
	{
	TRAPD(err, iHttpClientTrans = CHttpClientTransactionImpl::NewL(aClient, *this, aMethod, aUri));
	return err;
	}

/**
 * Destructor
 */
EXPORT_C CHttpClientTransaction::~CHttpClientTransaction()
	{
	delete	iHttpClientTrans;
	}

/**
 * Returns HTTP response status code
 * 
 */
EXPORT_C TInt CHttpClientTransaction::StatusCode() const
	{
	return iHttpClientTrans->Transaction().Response().StatusCode();	
	}
	
/**
 * Returns HTTP response status text
 * 
 */
EXPORT_C const TDesC8& CHttpClientTransaction::StatusText() const
	{
	return iHttpClientTrans->Transaction().Response().StatusText().DesC();
	}

/**
 * Disable the pipelining for “this” transaction. The transaction will be send on a 
 * free TCP connection and no other transactions will be send via that TCP connection 
 * till the response for “this” transaction is received
 */
EXPORT_C void CHttpClientTransaction::DisablePipelining()
	{
	RStringPool sp = iHttpClientTrans->StringPool();
	RStringF property = sp.StringF(HTTP::EHttpPipelining, RHTTPSession::GetTable());
	THTTPHdrVal disablePipelining = sp.StringF(HTTP::EDisablePipelining, RHTTPSession::GetTable());
	iHttpClientTrans->Transaction().PropertySet().SetProperty(property, disablePipelining);	
	}

/**
 * Disable automatic retry on a TCP connection failure. On TCP connection failure the error 
 * will be notified via OnError function
 * 
 */
EXPORT_C void CHttpClientTransaction::SetNoRetry()
	{
	RStringPool sp = iHttpClientTrans->StringPool();
	RStringF property = sp.StringF(HTTP::ENotifyOnDisconnect, RHTTPSession::GetTable());
	THTTPHdrVal disconnectNotify = sp.StringF(HTTP::EEnableDisconnectNotification, RHTTPSession::GetTable());
	iHttpClientTrans->Transaction().PropertySet().SetProperty(property, disconnectNotify);	
	}

/**
 * Set the dynamic HTTP content source.
 */
EXPORT_C TInt CHttpClientTransaction::SetContentSource(MHttpContentSource& aSource)
	{
	return iHttpClientTrans->SetDynamicContentSource(&aSource);
	}
	
/**
 * Set the static HTTP content source.
 * 
 * @param aFile - An opened file
 */
EXPORT_C TInt CHttpClientTransaction::SetContentSource(RFile& aFile)
	{
	return iHttpClientTrans->SetStaticContentSource(aFile);
	}

/**
 * Set the dynamic HTTP content sink
 * 
 */
EXPORT_C TInt CHttpClientTransaction::SetContentSink(MHttpContentSink& aSink)
	{
	return iHttpClientTrans->SetDynamicContentSink(&aSink);
	}

/**
 * Set the static HTTP content sink
 * 
 * @param aFile - An opened file
 */
EXPORT_C TInt CHttpClientTransaction::SetContentSink(RFile& aFile)
	{
	return iHttpClientTrans->SetStaticContentSink(aFile);
	}	
/**
 * Set the HTTP request timeout. If the HTTP request didn't send in the specified 
 * time OnError will be called with an error code
 * 
 * @param aValue - Request timeout value
 */
EXPORT_C void CHttpClientTransaction::SetRequestTimeout(TInt aValue)
	{
	RStringPool sp = iHttpClientTrans->StringPool();
	RStringF property = sp.StringF(HTTP::ESendTimeOutValue, RHTTPSession::GetTable());
	THTTPHdrVal requestTimeout(aValue);
	iHttpClientTrans->Transaction().PropertySet().SetProperty(property, requestTimeout);			
	}

/**
 * Set the HTTP response timeout. If the HTTP response didn't arrive in the specified 
 * time OnError will be called with an error code
 */
EXPORT_C void CHttpClientTransaction::SetResponseTimeout(TInt aValue)
	{
	RStringPool sp = iHttpClientTrans->StringPool();
	RStringF property = sp.StringF(HTTP::EReceiveTimeOutValue, RHTTPSession::GetTable());
	THTTPHdrVal responseTimeout(aValue);
	iHttpClientTrans->Transaction().PropertySet().SetProperty(property, responseTimeout);			
	}

EXPORT_C TInt CHttpClientTransaction::AddRequestHeader(TInt aHeaderId, const THttpHeaderValueVariant& aHeaderValue)
	{
    RHTTPHeaders requestHeaders = iHttpClientTrans->Transaction().Request().GetHeaderCollection();;
    RStringPool sp = iHttpClientTrans->StringPool();
    THTTPHdrVal val = HttpClientUtils::CopyHttpHdrVal(aHeaderValue, sp);
    if(val.Type() == THTTPHdrVal::KNoType)
        return KErrNoMemory;
    TInt err = requestHeaders.SetField(sp.StringF(aHeaderId, RHTTPSession::GetTable()), val);
    HttpClientUtils::CloseString(val);
    return err;	
	}

EXPORT_C TInt CHttpClientTransaction::AddRequestHeader(const TDesC8& aHeaderName, const THttpHeaderValueVariant& aHeaderValue)
    {
    RHTTPHeaders requestHeaders = iHttpClientTrans->Transaction().Request().GetHeaderCollection();;
    RStringPool sp = iHttpClientTrans->StringPool();
    THTTPHdrVal val = HttpClientUtils::CopyHttpHdrVal(aHeaderValue, sp);
    if(val.Type() == THTTPHdrVal::KNoType)
        return KErrNoMemory;
    RStringF str;
    TInt err = HttpClientUtils::OpenStringF(aHeaderName, sp, str); 
    if(err != KErrNone)
        {
        HttpClientUtils::CloseString(val);
        return err;
        }
    err = requestHeaders.SetField(str, val);
    HttpClientUtils::CloseString(val);
    str.Close();
    return err;    
    }

EXPORT_C TInt CHttpClientTransaction::AddRequestHeader(TInt aHeaderId, const THttpHeaderValueVariant& aHeaderValue, TInt aParamId, const THttpHeaderValueVariant& aParamValue)
	{
    RHTTPHeaders requestHeaders = iHttpClientTrans->Transaction().Request().GetHeaderCollection();;
    RStringPool sp = iHttpClientTrans->Transaction().Session().StringPool();
    THTTPHdrVal val = HttpClientUtils::CopyHttpHdrVal(aHeaderValue, sp);
    if(val.Type() == THTTPHdrVal::KNoType)
        return KErrNoMemory;

    THTTPHdrVal val2 = HttpClientUtils::CopyHttpHdrVal(aParamValue, sp);
    if(val2.Type() == THTTPHdrVal::KNoType)
        {
        HttpClientUtils::CloseString(val);
        return KErrNoMemory;
        }
    TInt err = requestHeaders.SetField(sp.StringF(aHeaderId, RHTTPSession::GetTable()), val,
                                        sp.StringF(aParamId, RHTTPSession::GetTable()), val2);
    HttpClientUtils::CloseString(val);
    HttpClientUtils::CloseString(val2);
    return err;
	}

EXPORT_C TInt CHttpClientTransaction::AddRequestHeader(const TDesC8& aHeaderName, const THttpHeaderValueVariant& aHeaderValue, const TDesC8& aParamName, const THttpHeaderValueVariant& aParamValue)
    {
    RHTTPHeaders requestHeaders = iHttpClientTrans->Transaction().Request().GetHeaderCollection();;
    RStringPool sp = iHttpClientTrans->Transaction().Session().StringPool();
    THTTPHdrVal val = HttpClientUtils::CopyHttpHdrVal(aHeaderValue, sp);
    if(val.Type() == THTTPHdrVal::KNoType)
        return KErrNoMemory;

    THTTPHdrVal val2 = HttpClientUtils::CopyHttpHdrVal(aParamValue, sp);
    if(val2.Type() == THTTPHdrVal::KNoType)
        {
        HttpClientUtils::CloseString(val);
        return KErrNoMemory;
        }
    RStringF str;
    TInt err = HttpClientUtils::OpenStringF(aHeaderName, sp, str); 
    if(err != KErrNone)
        {
        HttpClientUtils::CloseString(val);
        HttpClientUtils::CloseString(val2);
        return err;
        }

    RStringF str2;
    err = HttpClientUtils::OpenStringF(aParamName, sp, str2); 
    if(err != KErrNone)
        {
        HttpClientUtils::CloseString(val);
        HttpClientUtils::CloseString(val2);
        str.Close();
        return err;
        }
    err = requestHeaders.SetField(str, val, str2, val2);
    HttpClientUtils::CloseString(val);
    HttpClientUtils::CloseString(val2);
    str.Close();
    str2.Close();
    return err;  
    }

EXPORT_C TInt CHttpClientTransaction::AddCustomRequestHeader(const TDesC8& aHeaderName, const TDesC8& aHeaderValue)
	{
    _LIT8(KFieldSeparator, "\n");
    RHTTPHeaders requestHeaders = iHttpClientTrans->Transaction().Request().GetHeaderCollection();;   
    RStringPool sp = iHttpClientTrans->StringPool();
    RStringF str;
    if(HttpClientUtils::OpenStringF(aHeaderName, sp, str) != KErrNone)
        {
        return KErrNoMemory;
        }
    TInt err = requestHeaders.SetRawField(str, aHeaderValue, KFieldSeparator);
    str.Close();
    return err;
	}


EXPORT_C THttpHeaderIterator CHttpClientTransaction::ResponseHeaders() const
	{
	CHeaders* responseHeaders = iHttpClientTrans->Transaction().Response().GetHeaderCollection().iImplementation;
    return THttpHeaderIterator(responseHeaders);
	}
	
EXPORT_C THttpHeaderElementIterator CHttpClientTransaction::ResponseHeader(TInt aHeaderId)
	{
	RStringPool sp = iHttpClientTrans->StringPool();
	RHTTPHeaders responseHeaders = iHttpClientTrans->Transaction().Response().GetHeaderCollection();
	CHeaderField* field = responseHeaders.iImplementation->LookupField(sp.StringF(aHeaderId, RHTTPSession::GetTable()));
	if(field == NULL)
	    {
	    return THttpHeaderElementIterator(NULL);
	    }
	return THttpHeaderElementIterator(&field->iElementArray);
	}
	
EXPORT_C THttpHeaderElementIterator CHttpClientTransaction::ResponseHeader(const TDesC8& aHeaderName)
	{
    RStringPool sp = iHttpClientTrans->StringPool();
	RHTTPHeaders responseHeaders = iHttpClientTrans->Transaction().Response().GetHeaderCollection();
	CHeaderField* field = NULL;
	RStringF str;
	if(HttpClientUtils::OpenStringF(aHeaderName, sp, str) == KErrNone)
	    {
	    field = responseHeaders.iImplementation->LookupField(str);
	    str.Close();
	    }
	if(field == NULL)
	    {
	    return THttpHeaderElementIterator(NULL);
	    }
	return THttpHeaderElementIterator(&field->iElementArray);
	}

EXPORT_C TInt CHttpClientTransaction::SendRequest()
	{
    RStringF name = iHttpClientTrans->StringPool().StringF(HTTP::EContentLength, RHTTPSession::GetTable());
    RHTTPHeaders requestHeaders = iHttpClientTrans->Transaction().Request().GetHeaderCollection();
    THTTPHdrVal value;
    if(requestHeaders.GetField(name, 0, value) == KErrNone)
        {
        iHttpClientTrans->SetDynamicContentSourceLength(value.Int());
        }
	
	return iHttpClientTrans->Transaction().Submit();
	}
	
EXPORT_C void CHttpClientTransaction::CancelRequest()
	{
	iHttpClientTrans->Transaction().Cancel();
	}

/**
virtual functions
*/
EXPORT_C void CHttpClientTransaction::OnTransferProgress(TInt /* aTotal */, TInt /* aTransffered */)
	{
	
	}
	
EXPORT_C TBool CHttpClientTransaction::OnRedirection()
	{
	return ETrue;
	}

EXPORT_C void CHttpClientTransaction::OnWarning(TInt /* aWarning */)
    {
    
    }

EXPORT_C CHttpClientTransaction::CHttpClientTransaction()
: iHttpClientTrans(NULL)
	{
	}
