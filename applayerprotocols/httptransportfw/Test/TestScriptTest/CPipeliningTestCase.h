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
#include <chttpformencoder.h>
 


enum THeaderPosition
	{
	ENoHeader = -1,
	EHttpHeader,
	EConnectionHeader,
	EBatchingHeader,
	ETransactionHeader		
	};

//Forward declarations
class CHTTPTestUtils;
class CPipeliningTestParams;

class CPipeliningTestCase :  public CBase, public MPipeliningTestCase, public MHTTPDataSupplier 
 

	{
public:
	static CPipeliningTestCase* NewL(CHTTPTestUtils& aTestUtils, CPipeliningTestParams& aPipeliningTestParams);
	~CPipeliningTestCase();
		
private:
	CPipeliningTestCase(CHTTPTestUtils& aTestUtils, CPipeliningTestParams& aPipeliningTestParams);

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TransactionCount(TInt aConnIndex) const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt ConnectionCount() const;
	void AddConnectionCloseHeaderL(RHTTPTransaction aTransaction);
	TInt RealExpectedConnectionCount() const;
	TInt ExpectedError(RHTTPTransaction aTrans);
	void ProcessHeadersL(RHTTPTransaction aTrans);
	TBool EnableBatching() const;
	TBool BatchTransactions() const;
	void FieldCompareL(TInt aStringPool, TInt aIniFile);
	void FieldValueCompareL(RHTTPHeaders headers, TInt xyz, TInt xx, RStringF headerStr);
	TInt ErrorVal() const;
	
	TBool GetNextDataPart(TPtrC8& aDataPart);
	void ReleaseData();
	TInt OverallDataSize();
	TInt Reset();
private:
	CHTTPTestUtils&			iTestUtils;
	CPipeliningTestParams&	iPipeliningTestParams;
	
	//for testcase 7
	RHTTPTransaction		iTransToCancel;
	TBool					iIsTransCancelled;
	TBool					iBatchingBufferSizeSet;
	CHTTPFormEncoder* 		iFormEncoder;
		
	};
	
#endif // __CPIPELININGTESTCASE_H__
