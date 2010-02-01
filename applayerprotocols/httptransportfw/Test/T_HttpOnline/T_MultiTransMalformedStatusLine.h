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

#if !defined (__CTEXTMODETESTMULTITRANSMALFORMEDSTATUSLINE_H__)
#define __CTEXTMODETESTMULTITRANSMALFORMEDSTATUSLINE_H__

#include <http.h>
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

/**
 *  Derived test case class : test parallel transactions
 *	This test was designed for the defect fix EXT-5EBAHM
 */
class CTextModeTestMultiTransMalformedStatusLine: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
public:
	static CTextModeTestMultiTransMalformedStatusLine* NewL(const TDesC& aHostName, TInt aNumTrans);
	virtual ~CTextModeTestMultiTransMalformedStatusLine();
	// to implement in this class' member functs definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;

protected:
	CTextModeTestMultiTransMalformedStatusLine(TInt aNumTrans);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void CreateTransArrayL();
	void CreateSingleTransactionL(RStringF aGet);
	void ConstructL(const TDesC& aHostName);
protected:
	TInt iMaxNumTransactions;
	RArray<RHTTPTransaction> iTransArray;
	TInt iOpenTransactionCount;
	HBufC8* iHostName;
	TInt iFailureError;
	TInt iConnectFailureCount;
	};

#endif // !defined __CTEXTMODETESTMULTITRANSMALFORMEDSTATUSLINE_H__
