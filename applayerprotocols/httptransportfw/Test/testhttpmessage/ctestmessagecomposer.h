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

#ifndef __CTESTMESSAGECOMPOSER_H__
#define __CTESTMESSAGECOMPOSER_H__

#include <e32base.h>
#if !defined HTTP_H
#include <http.h>
#endif
#include "rhttpmessagecomposer.h"
#include "mhttpmessagecomposerobserver.h"

#if !defined MHTTPTRANSACTIONCALLBACK_H__
#include <http/mhttptransactioncallback.h>
#endif

#if !defined __MTESTOBSERVER_H__
#include "mtestobserver.h"
#endif


class CTestMessageComposer : public CActive,
							   public MHttpMessageComposerObserver,public MHTTPTransactionCallback
	{
	
public:	// methods

	static CTestMessageComposer* NewL(MTestObserver& aTestObserver);

	virtual ~CTestMessageComposer();
	void SetHeaderL(TInt aNumberofHeaders);
	void StartRequest();

private:	// methods from CActive

	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);

private:	// methods from MHttpMessageComposerObserver

	virtual void StartLineL(TPtrC8& aToken1, TPtrC8& aToken2, TPtrC8& aToken3);
	virtual TInt NextHeaderL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue);
	virtual TInt NextTrailerL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue);
	virtual MHTTPDataSupplier* HasBodyL();
	virtual void MessageComplete();
	virtual void MessageDataReadyL();

	virtual TInt HandleComposeError(TInt aError);

	virtual void Reserved_MHttpMessageComposerObserver();
	
private:
	//
	// methods from MHTTPTransactionCallback
	//
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

private:	// enums

	enum TMessageState
		{
		EPendingStartLine			= 0,
		EPendingNextHeader,
		EPendingHasBody,
		EPendingNextTrailer,
		EPendingMessageComplete,
		EDone
		};
private:	// helper classes

	class THeaderField
		{
	public:	// methods

		THeaderField(const TDesC8& aName, const TDesC8& aValue) : iName(aName), iValue(aValue) {};

	public:	// attributes

		TPtrC8		iName;
		TPtrC8		iValue;

		};
private:	// methods

	CTestMessageComposer(MTestObserver& aObserver);
	void ConstructL();

	void CompleteSelf();
	void DoReset();
	
private:

	RStringPool iStringPool;
	RHTTPSession iSession;
	RHTTPTransaction iTransaction; // only used to create headers
	RHTTPHeaders iHTTPHeaders;
	RArray<THeaderField>	iHeaders;
	TInt					iHeaderIndex;
	MTestObserver&		iObserver;
	RHttpMessageComposer	iMessageComposer;
	RHTTPRequest			iRequest;
	TMessageState			iState;
	TBool iFlag;
	};

#endif  // __CTestMessageComposer_H__
