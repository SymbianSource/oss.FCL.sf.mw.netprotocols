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

#ifndef __CPIPELININGTESTPARAMS_H__
#define __CPIPELININGTESTPARAMS_H__

#include <e32base.h>


class CHTTPTestUtils;



typedef struct TransInfo_
{
	/*Array for Header types to add to transactions, presently only size 3, 
	index 0 - HTTP & index 1 for Connection, 2 for Transaction - to cancel
	0 in the index means default header ( http 1.1 & no connection close),
	if there is a value other than 0, it means there is information
	in the TransHeaderInfo array at the value index,
	eg: value 4 in index 1 of TransHeaderType means 
	Connection header has some value in the 4th index of the TransHeaderInfo array
	*/
	TInt					TransType;
	RArray < TInt >			TransHeaderTypeArr; 
	RPointerArray < HBufC > TransHeaderInfoPtrArr;
} TransInfo;

enum TArrayType
	{
	ERequestArray = 1,
	EResponseArray,
	ETransactionArray
	};

/*	Pipelining parameters - configured from ini file, and passed to testCase constructor
*/
class CPipeliningTestParams : public CBase
	{
public:
	static CPipeliningTestParams* NewL(CHTTPTestUtils& aTestUtils);
		
	//Parse "\n", "\r" with '\r', \n' and " with null
	void RemoveQuotes(HBufC16* aHBuf16Ptr);
	void RemoveQuotes(HBufC8* aHBuf8Ptr);
	
	void ParseReplaceCRLF(HBufC8* aHBuf8Ptr);
	void ParseReplaceCRLF(HBufC16* aHBuf16Ptr);
	
	//Run Test
	TBool	RunTest() const;
	void SetRunTest(const TDesC& aRunTest);
	
	//Batching Enabled
	TBool BatchingEnabled() const;
	void EnableBatching(const TDesC& aIsBatchingEnabled);
	
	//Pipelining Enabled
	TBool PipeliningEnabled() const;			
	void EnablePipelining(const TDesC& aIsPipeliningEnabled);
	
	//Buffer Size
	TInt	BufferSize() const;
	void SetBufferSize(const TDesC& aBufferSize);
	
	//Uri
	TDesC8&	UriL(TInt aUriIndex) const;
	TInt UriCount() const;
	void SetUriL(TInt aUriIndex, const TDesC& aUri);
	
	//Error
	TInt Error() const;
	void SetErrorL(const TDesC& aError);

	//Connection Count
	TInt ConnectionCount() const;
	void SetConnectionCount(const TDesC& aConnectionCount);
	
	//Request Method Type
	void SetReqMethodL(const TDesC& aReqMethod);
	TDesC16& ReqMethod();
	
	//Total TransactionCount
	TInt TotalTransactionCount() const;
	void SetTotalTransactionCount(const TDesC& aTotalTransCount);
	void DecreaseTotalTransactionCount() const;
	
	//TransactionCount - returns the size of the connReqArray
	TInt TransactionCount(TInt aConnIndex) const;
	
	//TestCase Name
	TDesC&	TestCaseName() const;
	void SetTestCaseNameL(const TDesC& aTestCaseName );
	
	//Common to request and response arrays
	//Input TInt aInt : Use 1 for requestarr, 2 for responsearr, 3 for transactionarr
	void SetSequenceArrL(const TDesC& aArrAsStr, TInt aInt);
	TInt TransactionSequenceArray(TInt aIndex) const;
	
	//Raw Request
	TDesC8&	RawRequest(TInt aConnIndex, TInt aTransIndex) const; // returns based on the connection index after checking 2d array
	TDesC8& RawRequest(TInt aRawRequestBufArrIndex ) const; //returns value in the rarray based on index
	void SetRawRequestL(TInt aConnIndex, const TDesC& aRawRequest);	
	
	
	//Raw Response
	TDesC8&	RawResponse(TInt aConnIndex, TInt aTransIndex) const;
	TDesC8&	RawResponse( TInt aRawResponseBufArrIndex ) const;
	void SetRawResponseL(TInt aConnIndex, const TDesC& aRawResponse);
	
	
	//Transaction Types
	TransInfo& TransactionType(TInt transTypeNum) const; //Returns a long string like transInfo in SetTransType()
	
	//1. Creates a new instance of TransInfo struct,
	//2. Sets all information of TransInfo by
	//	a. Sets the TransInfo->TransType to transTypeNumber passed by function
	//	b. Parsing the given string for ',' demiliter
	//	c. Parsing string returned by step 2.a for ':' delimiter
	//	d. sets the second part of string returned by 2.b in the string to TransInfo->TransHeaderInfoPtrArr
	//		checking if first part of string returned by 2.b is HTTP or Connection
	//		if HTTP, then sets TransInfo->TransHeaderTypeArr[0] to count of TransInfo->TransHeaderInfoPtrArr
	//		else sets TransInfo->TransHeaderTypeArr[1]
	//
	// Returns the index of the TransType created in iTransTypesInfoPtrArr
	TInt SetTransactionTypeL(TInt transTypeNumber, TDesC& transInfoDesc);
	
	//Header to Process for test cases cheaderdecode, cinc052596, cinc037440, cpipeliningconfigtest
	TBool	ProcessHeader()	const;
	TInt NumHdrs() const;
	TDesC8&	 HeaderToProcess(TInt aHdrIdx) const;
	void SetHeaderToProcessL(const TInt aHdrIdx, const TDesC& aHdrName );		
	
	//Value expected for the header
	TInt NumberOfHeaderVals(const TInt aCtr) const;
	TDesC8& HeaderVal(const TInt aHdrCtr, const TInt aHdrValCtr ) const;
	void SetHdrValL(const TInt aHdrCtr, const TInt aHdrValIdx, const TDesC& aHdrVal);
	
	//Validate: To check if the params are configured correctly.
	//If no, leave with error number defined in TestParamsErr.h
	void ValidateL() const;
	
	//Destructor	
	~CPipeliningTestParams();
	
private:
	CPipeliningTestParams(CHTTPTestUtils& aTestUtils);
	CPipeliningTestParams(const CPipeliningTestParams& aPipeliningTestParams);
	
private:
	TBool							iRunTest;
	HBufC*							iTestCaseNameBuf;
	TBool							iBatchingEnabled;
	TBool							iPipeliningEnabled;
	TInt							iBufferSize;
	RArray < HBufC8* >				iUriBufArr;
	TInt							iConnectionCount;
	HBufC*							iReqMethod;
	TInt							iError;
	mutable TInt					iTotalTransactionCount;
	RArray < TInt >					iTransArr;
	RPointerArray < TransInfo >		iTransTypesInfoPtrArr;
	RArray < HBufC8* >				iRawRequestBufArr;	
	RArray < HBufC8* >				iRawResponseBufArr;
	RArray < RArray < TInt 	>	>	iConnReq2DArr;
	RArray < RArray < TInt 	>	>	iConnResp2DArr;
	RArray < TBool >				iProcessHdrArr;
	TBool			 				iProcessHdr;
	RArray < HBufC8* >				iHeader2ProcessArr;
	RArray < RArray < HBufC8* > >	iHdrVal2DArr;
	
	CHTTPTestUtils&			iTestUtils;
	
	};

#endif // __CPIPELININGTESTPARAMS_H__
