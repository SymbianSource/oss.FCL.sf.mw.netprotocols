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

#ifndef __MPIPELININGTESTCASE_H__
#define __MPIPELININGTESTCASE_H__

#include <e32base.h>
#include <http.h>

/*
The MPipeliningTestCase class is a pure virtual class used to derive/create test cases
for use with the t_httppipeliningtest test harness.

To write a test case for the t_httppipeliningtest test harness, it is a simple task of
creating a C class that is derived from the pure virtual class MPipeliningTestCase.
The test case class must implement the methods detailed below in order for the test case
to be executed. Writing a test case assumes knowledge of the HTTP Transport Framework
and how it is supposed to behave. The input data is all provided by the test case as well
as the server responses and the expected behaviour.

The test harness provides a simple HTTP client and server that query the test case on how
it should behave based on various parameters. The description is provided below with each
of the pure virtual methods in this class.

To use the test server functionality all requests must be directed to http://127.0.0.1 on
port 80.

Once the test case class has been implemented, the class must be added to the test engine
in CPipeliningTestEngine::DoTestsL(). To do this the following code must be added...

  CTestCaseExample* testCase = CTestCaseExample::NewL(*iTestUtils);
  CleanupStack::PushL(testCase);
  RunTestL(*testCase);
  CleanupStack::PopAndDestroy(testCase);

The construction parameters iTestUtils is optional, but is simply a reference to the
CHTTPTestUtils class to provide standard HTTP logging functionality.
*/
class MPipeliningTestCase
	{
public:
	/*
	This method should return a descriptor that contains the display name of the test case.
	*/
	virtual const TDesC& TestCaseName() const = 0;

	/*
	This method should return the total number of transactions that are opened and involved
	in the test case. This number can be dynamic during the running of the test, an example
	of this would be if transactions are cancelled and therefore those should removed from
	the count. The test HTTP client uses this value so it knows when all the transactions
	have completed.
	*/
	virtual TInt TotalTransactionCount() const = 0;

	/*
	This method should return the number of transactions on a giving connection index. For
	tests that open a number of connections on a server, this method should return the
	number of transactions on a connection identified by its index (ie starting from 0).
	Tests that don't use multiple conenctions, this should simply return the same value as
	TotalTransactionCount(0).
	*/
	virtual TInt TransactionCount(TInt aConnectionIndex) const = 0;

	/*
	This method should create the RHTTPTransaction that will be used in the test. The index
	value passed in indicates what transaction should be created (based on the
	TotalTransactionCount() value and will be called that many times. The HTTP Session and
	the client transaction callback that should be used is also provided in the parameters.
	*/
	virtual RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient) = 0;

	/*
	This method allows the test case to set an option into how the transactions are
	submitted. If this method returns ETrue, the transaction will be batched where they are
	all opened first and then submitted all together, or else return EFalse so transactions
	are submitted as they are opened.
	*/
	virtual TBool BatchTransactions() const = 0;

	/*
	When the client receives a transaction event, the error code can be checked to verify
	what (if any) error code is expected. The transaction is supplied so that the test case
	can identify the transaction by Id(). If no error codes are expected, this method should
	return KErrNone.
	*/
	virtual TInt ExpectedError(RHTTPTransaction aTrans) = 0;

	/*
	This method should return the raw request data that is expected to be reeived by the
	server for a giving connection and transaction on that connection. The connection
	is indentified by its index and then the transaction index on that connection. For
	instance, for the 3rd transaction on the 2nd connection will result in aConnectionIndex = 1
	and aTransIndex = 2.
	*/
	virtual const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex) = 0;

	/*
	This method should return the raw response data that the server should return for a
	given transaction. This is similar to GetRawRequestL() with the parameter values.
	*/
	virtual const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex) = 0;

	/*
	This method should return the number of connections that is expected to open with the
	test server in the test case. If the test server is not being used (ie. transactions are
	going to 'real' servers) then this should return 0.
	*/
	virtual TInt ConnectionCount() const = 0;

	/*
	This method should return the number of expected connections opened with real servers. 
	This information is stored by the protocol handler in debug mode only and checked.
	
	When using the test server this should return the same as ConnectionCount()
	*/
	virtual TInt RealExpectedConnectionCount() const = 0;

	/*
	This method will indicate whether to enable batching (request buffering). The test engine
	will use this to enable/disable batching by setting the appropriate session property.
	*/
	virtual TBool EnableBatching() const = 0;
	
	/*
	This method is called when the EGotResponseHeaders event is received and gives the 
	opportunity for the test case to process the headers. 
	*/
	virtual void ProcessHeadersL(RHTTPTransaction aTrans) = 0;
	
	virtual TInt ErrorVal() const = 0;
	};
	
#endif // __MPIPELININGTESTCASE_H__
