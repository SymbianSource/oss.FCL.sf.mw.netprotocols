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

#ifndef __CPIPELININGTESTCASE_H__
#define __CPIPELININGTESTCASE_H__

#include <e32base.h>
#include <http.h>

#include "MPipeliningTestCase.h"

/*
The CPipeliningTestCase is a simple class to inherit from MPipeliningTestCase to provide
default implementations for some of the functions required by MPipeliningTestCase. Many
methods in this class are sometimes not really required so this is class has been
introduced to provide some default implementations. All these methods are virtual so that
any of them can be overriden with new implementations.
*/
class CPipeliningTestCase : public CBase,
							public MPipeliningTestCase
	{
public:	
	~CPipeliningTestCase();
	
	

	/*
	This method should return the number of transactions on a giving connection index. Most
	test will only require a single connection so this method should return the same value 
	as MPipeliningTestCase::TotalTransactionCount() so this method will simply call that.
	*/
	virtual TInt TransactionCount(TInt aConnectionIndex) const;

	/*
	This method allows the test case to set an option into how the transactions are
	submitted. If this method returns ETrue, the transaction will be batched where they are
	all opened first and then submitted all together, or else return EFalse so transactions
	are submitted as they are opened. Most test cases will require that transaction are
	batched so the default implementation is to return ETrue.
	*/
	virtual TBool BatchTransactions() const;

	/*
	When the client receives a transaction event, the error code can be checked to verify
	what (if any) error code is expected. The transaction is supplied so that the test case
	can identify the transaction by Id(). Most test cases will not expect an error code so
	the default implementation is to simply return KErrNone.
	*/
	virtual TInt ExpectedError(RHTTPTransaction aTrans);

	/*
	This method should return the number of connections that is expected to open with the
	test server in the test case. If the test server is not being used (ie. transactions are
	going to 'real' servers) then this should return 0. For simplicity, the default
	implementation is to return 0.
	*/
	virtual TInt ConnectionCount() const;
	
	/*
	This method should return the number of expected connections opened with real servers. 
	This information is stored by the protocol handler in debug mode only and checked.
	
	When using the test server this should return the same as ConnectionCount()
	*/
	TInt RealExpectedConnectionCount() const;

	/*
	This method will indicate whether to enable batching (request buffering). Most pipelining
	test cases do not require batching to be enabled so the default implementation is to return
	EFalse.
	*/
	virtual TBool EnableBatching() const;
	
	void ProcessHeadersL(RHTTPTransaction aTrans);
	
	virtual TBool TransportHandlerPriority() const;

	
protected:
	CPipeliningTestCase();

	};
	
#endif // __CPIPELININGTESTCASE_H__
