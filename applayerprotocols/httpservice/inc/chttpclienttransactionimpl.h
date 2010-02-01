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

#ifndef __CHTTPCLIENTTRANSACTIONIMPL_H__
#define __CHTTPCLIENTTRANSACTIONIMPL_H__

#include <http.h>
#include <e32base.h>
#include <f32file.h>  
#include <stringpool.h>

class CHttpClientTransaction;
//class MHTTPContentSource;
class MHTTPContentSink;
class CHttpService;
class CHttpDataSender;
class CHttpDataReceiver;
class CHttpClientTransactionImpl : public CBase, public MHTTPTransactionCallback
	{

	public:
	static CHttpClientTransactionImpl* NewL(CHttpService& aClient, CHttpClientTransaction& aTrans, const TDesC8& aMethod, const TDesC8& aUri);
	~CHttpClientTransactionImpl();
	
	void SetDynamicContentSourceLength(TInt aLen);
	TInt SetDynamicContentSource(MHttpContentSource* aSource);
	TInt SetDynamicContentSink(MHttpContentSink* aSink);
	TInt SetStaticContentSource(RFile& aSource);
	TInt SetStaticContentSink(RFile& aSink);
	inline RHTTPTransaction Transaction();
	inline RStringPool StringPool();
	
	private:
	inline CHttpClientTransactionImpl(CHttpClientTransaction& aTrans, RStringPool aStringPool);
	void ConstructL(CHttpService& aClient, const TDesC8& aMethod, const TDesC8& aUri);
	
	// From MHTTPTransactionCallback
	
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);


	
	private:
	CHttpClientTransaction&    iClientTrans;
	CHttpDataSender*           iDataSender;
	CHttpDataReceiver*         iDataReceiver;
	RHTTPTransaction		   iTransaction;	
	RStringPool				   iStringPool; 		// Not owned.
	};

inline CHttpClientTransactionImpl::CHttpClientTransactionImpl(CHttpClientTransaction& aTrans, RStringPool aStringPool)
: iClientTrans(aTrans),
iStringPool(aStringPool)
    {
    
    }
	
inline RHTTPTransaction CHttpClientTransactionImpl::Transaction()
	{
	return iTransaction;
	}

inline RStringPool CHttpClientTransactionImpl::StringPool()
	{
	return iStringPool;
	}

#endif // __CHTTPCLIENTTRANSACTIONIMPL_H__
