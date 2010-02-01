// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined (__CTEXTMODETESTPERSISTENTCONN_H__)
#define __CTEXTMODETESTPERSISTENTCONN_H__

#include <http.h>
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

/**
 *  Derived test case class : Test a sequence of transactions that share a persistent connection
 */
class CUri8;
class CTextModeTestPersistentConn : public CHttpTestTransBase, public MHTTPTransactionCallback
	{
public:
	static CTextModeTestPersistentConn* NewL(const TDesC& aHostName, const TDesC& aPath, TInt aNumTrans);
	virtual ~CTextModeTestPersistentConn();
public: 
	// methods inherited from MHTTPTransactionCallback
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	// methods inherited from MHTTPDataSupplier
	virtual TBool GetNextDataPart(TPtrC8& aDataChunk);
	void ReleaseData();
	TInt OverallDataSize();
protected:
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
private:
	CTextModeTestPersistentConn(TInt aNumTrans);
	void ConstructL(const TDesC& aHostName, const TDesC& aPath);
private:
	TInt iNumTrans;
	TInt iTestIdx;
	TInt iReqBodyChunkCount;
	CUri8* iTestUrl;
	TInt iFailureError;
	TBool iRetVal;
	};


#endif // !defined __CTEXTMODETESTPERSISTENTCONN_H__
