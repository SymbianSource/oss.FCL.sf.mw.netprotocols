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

#ifndef CTESTMESSAGEPARSER_H_
#define CTESTMESSAGEPARSER_H_
#if !defined HTTP_H
#include <http.h>
#endif
#include <e32base.h>
#include <rhttpmessageparser.h>
#include <mhttpmessageparserobserver.h>
#include <http/rhttpheaders.h>

#if !defined __MTESTOBSERVER_H__
#include "mtestobserver.h"
#endif

class CTestMessageParser : public CActive,
							   public MHttpMessageParserObserver,public MHTTPTransactionCallback
	{
public: 
	static CTestMessageParser* NewL(MTestObserver& aTestObserver);
	virtual ~CTestMessageParser();
	void StartRequest(TInt MessageType);
	
private:	// methods from CActive

	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);

private:	// methods from MHttpMessageParserObserver

	virtual void GetDataPacket(TPtrC8& aData);
	virtual void ReleaseDataPacket();

	virtual void StartLineL(const TDesC8& aStartLine);
	virtual void HeaderL(const TDesC8& aFieldName, TDesC8& aFieldValue);
	virtual TInt BodySizeL();
	virtual void BodyChunkL(const TDesC8& aData);
	virtual void BodyCompleteL();
	virtual void MessageCompleteL(const TPtrC8& aExcessData);

	virtual TInt HandleParserError(TInt aError);

	virtual void Reserved_MHttpMessageParserObserver();	

private:
	//
	// methods from MHTTPTransactionCallback
	//
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

private:	// methods

	CTestMessageParser(MTestObserver& aObserver);
	void ConstructL();
	void CompleteSelf();
	void DoReset();
	
private:
	MTestObserver&		iObserver;
	RHttpMessageParser	iMessageParser;	
	RStringPool iStringPool;
	RHTTPSession iSession;
	RHTTPTransaction iTransaction; // only used to create headers
	RHTTPHeaders iHTTPHeaders;
	HBufC8*	iBuffer;
	TInt iMessageType;
	};							   
#endif /* CTESTMESSAGEPARSER_H_ */
