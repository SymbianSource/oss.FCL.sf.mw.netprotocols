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

#ifndef __CHTTPREQUESTCOMPOSER_H__
#define __CHTTPREQUESTCOMPOSER_H__

#include <e32base.h>
#include <http/thttphdrfielditer.h>
#include <http/framework/ctxdata.h>
#include <http/framework/logging.h>

#include "mhttpmessagecomposerobserver.h"
#include "rhttpmessagecomposer.h"
#include "mhttprequest.h"

class MHttpRequestObserver;

class CHttpRequestComposer : public CTxData,
							 public MHttpRequest,
							 public MHttpMessageComposerObserver
	{
public:
	
	static CHttpRequestComposer* NewL(CProtTransaction& aProtTrans, MHttpRequestObserver& aObserver);
	virtual ~CHttpRequestComposer();
	
	void NotifyMoreRequestBodyData();	
	TBool RequestSent () const;
	TBool IsSuspendedRequest() const;
	void ResumeSuspendedRequest();
	void CancelWaitFor100Continue();
	
private:	// methods from MHTTPDataSupplier, via CTxData

	virtual TBool GetNextDataPart(TPtrC8& aDataPart);
	virtual void ReleaseData();
	virtual TInt OverallDataSize();
	virtual TInt Reset();
	
private:	// methods from MHttpRequest

	virtual void StartRequest();
	virtual void CancelRequest();
	virtual void RequestDataSent();
	virtual TBool CheckRequestPendingComplete();
	virtual TBool NeedDisconnectNotification();
	virtual MHttpDataOptimiser* HttpDataOptimiser(TBool& aBatchingEnabled);
	virtual void OnRequestSendTimeOut();
	virtual TInt SendTimeOutValue();

private:	// methods from MHttpMessageComposerObserver

	virtual void StartLineL(TPtrC8& aMethod, TPtrC8& aRequestUri, TPtrC8& aVersion);
	virtual TInt NextHeaderL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue);
	virtual MHTTPDataSupplier* HasBodyL();
	virtual TInt NextTrailerL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue);
	virtual void MessageComplete();

	virtual void MessageDataReadyL();

	virtual TInt HandleComposeError(TInt aError);

	virtual void Reserved_MHttpMessageComposerObserver();
	
private:

	CHttpRequestComposer(CProtTransaction& aProtTrans, MHttpRequestObserver& aObserver);
	void ConstructL();

	void InitHeadersL();
	TBool IsTrailerHeader(RStringF aHeader);
	
private:
    TBool                   iRequestHeaderSent;
    TBool                   iCorkingEnabled;
    TBool					iRequestSent;
	TBool					iSuspendRequest;
	const TStringTable&     iStringTable;
	MHttpRequestObserver&	iObserver;
	RStringPool				iStringPool;
	RHTTPRequest			iRequest;	
	THTTPHdrFieldIter		iFields;
	TPtrC8					iData;
	RHttpMessageComposer	iMessageComposer;
	RHTTPTransactionPropertySet	iPropertySet;
	RArray<RStringF>		iTrailerHeaders;

public:

	__FLOG_DECLARATION_MEMBER2
	
	};


#endif	// __CHTTPREQUESTCOMPOSER_H__
