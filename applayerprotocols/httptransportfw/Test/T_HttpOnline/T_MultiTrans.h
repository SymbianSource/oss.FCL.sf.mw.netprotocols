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

#if !defined (__CTEXTMODETESTMULTITRANS_H__)
#define __CTEXTMODETESTMULTITRANS_H__

#include <http.h>
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

/**
 *  Derived test case class : test parallel transactions
 */
class CTextModeTestMultiTrans: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
public:
	static CTextModeTestMultiTrans* NewL(const TDesC& aHostName, TInt aNumTrans);
	virtual ~CTextModeTestMultiTrans();
	// to implement in this class' member functs definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;

protected:
	CTextModeTestMultiTrans(TInt aNumTrans);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void CreateTransArrayL();
	void ValidateBodyL(RHTTPTransaction aTransaction);
	void ConstructL(const TDesC& aHostName);
protected:
	TInt iMaxNumTransactions;
	RArray<RHTTPTransaction> iTransArray;
	TInt iOpenTransactionCount;
	HBufC8* iHostName;
	TInt iFailureError;
	};

#endif // !defined __CTEXTMODETESTMULTITRANS_H__
