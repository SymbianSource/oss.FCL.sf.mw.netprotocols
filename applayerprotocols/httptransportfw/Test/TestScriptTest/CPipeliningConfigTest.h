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

#ifndef 	__CPIPELININGCONFIGTEST_H__
#define 	__CPIPELININGCONFIGTEST_H__



#include "MPipeliningTestCase.h"

/* This is a configurable test used for testing the configuration of pipelining in the
   HTTP Framework
   
   This test harness does not support testing of proxies
   
   It is used by creating a test and then optionally setting the max number of transactions
   to pipeline and/or the Max Number of Transport Handlers (effectively the maximum number of connections)
*/
class CPipeliningConfigTest : public CBase, public MPipeliningTestCase
	{
public:
	static CPipeliningConfigTest* NewLC(const TDesC& aTestName);
	~CPipeliningConfigTest();
	
	void SetMaxNumberTransactionsToPipeline(TInt aMaxNumberTransactionsToPipeline);
	void SetMaxNumberTransportHandlers(TInt aMaxNumberTransportHandlers);
	void AddTransactionUrisL(const TDesC8& aUri, TInt aRepeatNumber = 1);
	void ClearTransactionUris();
	void SetExpectedNumberConnections(TInt aExpectedNumberConnections);
	
// from MPipeliningTestCase	
	virtual const TDesC& TestCaseName() const;
	virtual TInt TotalTransactionCount() const;
	virtual TInt TransactionCount(TInt aConnectionIndex) const;
	virtual RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, 
											 MHTTPTransactionCallback& aClient);
	virtual TBool BatchTransactions() const;
	virtual TInt ExpectedError(RHTTPTransaction aTrans);
	virtual const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	virtual const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	virtual TInt ConnectionCount() const;
	TInt RealExpectedConnectionCount() const;
	virtual TBool EnableBatching() const;
	virtual void ProcessHeadersL(RHTTPTransaction aTrans);
	virtual TInt ErrorVal() const;

private:
	CPipeliningConfigTest();
	void ConstructL(const TDesC& aTestName);
	
	void CheckAndSetSessionSettingsL(RHTTPSession aSession);

private:	
	HBufC*	iTestName;
	RPointerArray<HBufC8> iTransactionUris;	
	TInt iMaxNumberTransactionsToPipeline;
	TInt iMaxNumberTransportHandlers;
	TInt iExpectedNumberConnections;
	};


#endif  //	__CPIPELININGCONFIGTEST_H__
