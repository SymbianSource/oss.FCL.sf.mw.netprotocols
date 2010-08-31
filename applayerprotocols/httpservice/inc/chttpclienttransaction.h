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


#ifndef __CHTTPCLIENTTRANSACTION_H__
#define __CHTTPCLIENTTRANSACTION_H__

#include <e32base.h>
#include <f32file.h>  
#include <http.h>
#include <httpheaderiter.h>
#include <mhttpcontentsource.h>
#include <mhttpcontentsink.h>

class CHttpService;
class CHttpClientTransactionImpl;

/**
 * In HTTP terms a single transaction is one request and its equivalent response. CHttpClientTransaction 
 * encapsulates the information for the transaction. The client transaction is associated with the client 
 * session. Client should derive from this class and implement the concrete use cases. [for ex; download, 
 * upload etc..]
 * 
 * @publishedAll
 * @prototype
 * 
 */
class CHttpClientTransaction : public CBase
	{
	public:
	
	IMPORT_C TInt Create(CHttpService& aClient, const TDesC8& aMethod, const TDesC8& aUri);
	
	IMPORT_C virtual ~CHttpClientTransaction();
		
	IMPORT_C TInt StatusCode() const;
	IMPORT_C const TDesC8& StatusText() const;
	
	IMPORT_C void DisablePipelining();
	IMPORT_C void SetNoRetry();
	
	IMPORT_C TInt SetContentSource(MHttpContentSource& aSource);
	IMPORT_C TInt SetContentSource(RFile& aFile);
	
	IMPORT_C TInt SetContentSink(MHttpContentSink& aSink);
	IMPORT_C TInt SetContentSink(RFile& aFile);
	
	IMPORT_C void SetRequestTimeout(TInt aValue);
	IMPORT_C void SetResponseTimeout(TInt aValue);

	IMPORT_C TInt AddRequestHeader(TInt aHeaderId, const THttpHeaderValueVariant& aHeaderValue);
	IMPORT_C TInt AddRequestHeader(TInt aHeaderId, const THttpHeaderValueVariant& aHeaderValue, TInt aParamId, const THttpHeaderValueVariant& aParamValue); 
    IMPORT_C TInt AddRequestHeader(const TDesC8& aHeaderName, const THttpHeaderValueVariant& aHeaderValue);
    IMPORT_C TInt AddRequestHeader(const TDesC8& aHeaderName, const THttpHeaderValueVariant& aHeaderValue, const TDesC8& aParamName, const THttpHeaderValueVariant& aParamValue); 
    IMPORT_C TInt AddRequestHeader(const TDesC8& aHeaderName, const TDesC8& aParamName, const THttpHeaderValueVariant& aParamValue);    

	
	IMPORT_C TInt AddCustomRequestHeader(const TDesC8& aHeaderName, const TDesC8& aHeaderValue);
	
	
	IMPORT_C THttpHeaderIterator         ResponseHeaders() const;
	IMPORT_C THttpHeaderElementIterator 	ResponseHeader(TInt aHeaderId);
	IMPORT_C THttpHeaderElementIterator 	ResponseHeader(const TDesC8& aHeaderName);


	IMPORT_C TInt SendRequest();
	IMPORT_C void CancelRequest();
	
	/**
	virtual functions
	*/
	virtual void OnResponseHeaders() =0;
	IMPORT_C virtual void OnTransferProgress(TInt aTotal, TInt aTransffered);
	IMPORT_C virtual TBool OnRedirection();
	virtual void OnCompletion() =0;
	virtual void OnError(TInt aError) =0;
	IMPORT_C virtual void OnWarning(TInt aWarning);
	
	private:
	// Reserved for future use.
	inline virtual void Reserved1();
	inline virtual void Reserved2();
	
	protected:
	IMPORT_C CHttpClientTransaction();
	
	
	private:		
	CHttpClientTransactionImpl* iHttpClientTrans;
	};

inline void CHttpClientTransaction::Reserved1()
    {
    }

inline void CHttpClientTransaction::Reserved2()
	{
	}
	
#endif // __CHTTPCLIENTTRANSACTION_H__
