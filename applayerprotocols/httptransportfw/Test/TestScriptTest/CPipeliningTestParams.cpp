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

#include "CPipeliningTestParams.h"
#include "httptestutils.h"
#include "CPipeliningTestCase.h"


const TUint KDoubleQuote ('"');
_LIT(KScriptLF, 	"\\n");
_LIT(KReplaceLF,	"\n");
_LIT8(KScriptLF8, 	"\\n");
_LIT8(KReplaceLF8,	"\n");

_LIT(KScriptCR, 	"\\r");
_LIT(KReplaceCR,	"\r");
_LIT8(KScriptCR8, 	"\\r");
_LIT8(KReplaceCR8,	"\r");




TInt err(0);
//Helper function to convert TPtr to TInt
static TInt Ptr2Int(const TDesC& aPtr)
	{
	TInt aInt;
	TLex aLex(aPtr);
	aLex.Val(aInt);
	return aInt;
	}

void CheckSetHeaderValueL(const TDesC& aTransInfoBuf, const TDesC& aHeader2Check, TransInfo& aTransInfo, const TInt aLocationInHeaderArr)
	{
	TChar commaChar (',');
	TChar colonChar (':');
	TInt locationOfDes = aTransInfoBuf.Match(aHeader2Check);
	
	if (locationOfDes != KErrNotFound)//the header has been found in the input string
		{
	 	TPtrC subString = aTransInfoBuf.Mid(locationOfDes);
	  	TInt commaDelimterPos = subString.Locate(commaChar);
	  	TPtrC headerFieldValuePairString = subString.Mid(locationOfDes, (commaDelimterPos != KErrNotFound ? commaDelimterPos:subString.Length()));		
	 	TInt colonDelimterPos = headerFieldValuePairString.Locate(colonChar);
	 	TPtrC headerVal = headerFieldValuePairString.Mid(colonDelimterPos+1 );
	 	HBufC *headerValueBuf = HBufC::NewLC(headerVal.Size());
	 	(headerValueBuf->Des()).Copy(headerVal);
	 	User::LeaveIfError(aTransInfo.TransHeaderInfoPtrArr.Append(headerValueBuf));
	 	User::LeaveIfError(aTransInfo.TransHeaderTypeArr.Insert(((aTransInfo.TransHeaderInfoPtrArr.Count()) - 1),aLocationInHeaderArr));
	 	CleanupStack::Pop(headerValueBuf);
	 	}
	 else//header is not present in the string
	 	{
	 	aTransInfo.TransHeaderTypeArr.Insert( ( -1 ), aLocationInHeaderArr );
	 	}
	}
	

CPipeliningTestParams* CPipeliningTestParams::NewL(CHTTPTestUtils& aTestUtils)
	{
	CPipeliningTestParams* self = new (ELeave) CPipeliningTestParams(aTestUtils);
	return self;
	}

CPipeliningTestParams::CPipeliningTestParams(CHTTPTestUtils& aTestUtils)
:iTestUtils(aTestUtils)
	{
	//Only Initializing integers, for the strings we need to know the size.
	iBufferSize = -1;
	iConnectionCount = -1;
	iTotalTransactionCount = -1;
	}

CPipeliningTestParams::~CPipeliningTestParams()
	{
	delete iTestCaseNameBuf;
	delete iReqMethod;
	TInt count;
	
	count = iUriBufArr.Count();
	for (TInt ii=0; ii < count ; ++ii )
		{
		delete iUriBufArr[ii];
		}	
	iUriBufArr.Close();
	
	count = iRawRequestBufArr.Count();
	for (TInt ii=0; ii < count ; ++ii )
		{
		delete iRawRequestBufArr[ii];
		}
	iRawRequestBufArr.Close();
	
	count = iRawResponseBufArr.Count();
	for (TInt ii=0; ii < count; ++ii )
		{
		delete iRawResponseBufArr[ii];	
		}
	iRawResponseBufArr.Close();
	
	
	count = iConnReq2DArr.Count();
	for (TInt ii=0; ii < count; ++ii )
		{
		iConnReq2DArr[ii].Close();
		}
	iConnReq2DArr.Close();
	
	count = iConnResp2DArr.Count();
	for (TInt ii=0; ii < count; ++ii )
		{
		iConnResp2DArr[ii].Close();	
		}
	iConnResp2DArr.Close();
	
	TInt temp = iTransTypesInfoPtrArr.Count();
	for ( TInt ii=0; ii < temp; ++ii)
		{
		iTransTypesInfoPtrArr[ii]->TransHeaderTypeArr.Close();
		iTransTypesInfoPtrArr[ii]->TransHeaderInfoPtrArr.ResetAndDestroy();
		delete iTransTypesInfoPtrArr[ii];
		}
	iTransTypesInfoPtrArr.Close();
	
	count = iHeader2ProcessArr.Count();

	for (TInt ii=0; ii < count; ++ii )
		{
		delete iHeader2ProcessArr[ii];
		}
	
	count = iHdrVal2DArr.Count();
	for (TInt ii=0; ii < count; ++ii )
		{
		TInt hdrValCount = iHdrVal2DArr[ii].Count();
		for (TInt jj=0; jj < hdrValCount; ++jj)
			{
			delete iHdrVal2DArr[ii][jj];
			}
		iHdrVal2DArr[ii].Close();
		}			
	iHdrVal2DArr.Close();
	iHeader2ProcessArr.Close();
	iTransArr.Close();
	}


//RunTest
void CPipeliningTestParams::SetRunTest(const TDesC& aRunTest)
	{
	iRunTest = Ptr2Int(aRunTest);
	}

TBool	CPipeliningTestParams::RunTest() const
	{
	return iRunTest;
	}

//Batching Enabled
void CPipeliningTestParams::EnableBatching(const TDesC& aIsBatchingEnabled)
	{
	iBatchingEnabled = Ptr2Int(aIsBatchingEnabled);
	}

TBool CPipeliningTestParams::BatchingEnabled() const
	{
	return iBatchingEnabled;
	}

//Pipelining Enabled
void CPipeliningTestParams::EnablePipelining(const TDesC& aIsPipeliningEnabled)
	{
	iPipeliningEnabled = Ptr2Int(aIsPipeliningEnabled);	
	}
	
TBool	CPipeliningTestParams::PipeliningEnabled() const
	{
	return iPipeliningEnabled;
	}
	

//Buffer Size
void CPipeliningTestParams::SetBufferSize(const TDesC& aBufferSize)
	{
	iBufferSize = Ptr2Int(aBufferSize);
	}

TInt	CPipeliningTestParams::BufferSize() const
	{
	return iBufferSize;
	}



//Error	
void CPipeliningTestParams::SetErrorL(const TDesC& aError)
	{
	HBufC* iErrorBuf = HBufC::NewLC(aError.Size());
	(iErrorBuf->Des()).Copy(aError);
	RemoveQuotes(iErrorBuf)	;
	ParseReplaceCRLF(iErrorBuf) ;
	
	if(iErrorBuf->Des().Compare(_L("KErrDisconnected")) == 0)
		{
		iError = KErrDisconnected;
		}
		
	else if (iErrorBuf->Des().Compare(_L("KErrCancel")) == 0)
		{
		iError = KErrCancel;
		}
	else
		{
		iError = KErrNone;				
		}	
	
	CleanupStack::PopAndDestroy(iErrorBuf);
	}

TInt CPipeliningTestParams::Error() const
	{
	return iError;
	}

//Connection Count	
void CPipeliningTestParams::SetConnectionCount(const TDesC& aConnectionCount)
	{
	iConnectionCount = Ptr2Int(aConnectionCount);
	}

TInt CPipeliningTestParams::ConnectionCount() const
	{
	return iConnectionCount;
	}
	
//Request Method
void CPipeliningTestParams::SetReqMethodL(const TDesC& aReqMethod)	
	{
	iReqMethod = HBufC::NewL(aReqMethod.Size());
	(iReqMethod->Des()).Copy(aReqMethod);
	RemoveQuotes(iReqMethod);
	}

TDesC16& CPipeliningTestParams::ReqMethod()
	{
	return (*(iReqMethod));
	}

//Transaction Count
void CPipeliningTestParams::SetTotalTransactionCount(const TDesC& aTotalTransCount)
	{
	iTotalTransactionCount = Ptr2Int(aTotalTransCount);
	}

void CPipeliningTestParams::DecreaseTotalTransactionCount() const
	{
		--iTotalTransactionCount;
	}

TInt CPipeliningTestParams::TotalTransactionCount() const
	{
	return iTotalTransactionCount;
	}

//Transaction Count
TInt CPipeliningTestParams::TransactionCount(TInt aConnIndex) const
	{
	
	TInt outerSize = iConnReq2DArr.Count();
	
	for (TInt x=0; x < outerSize; ++x )
	{
	TInt innerSize = iConnReq2DArr[x].Count();
	}
	
	__ASSERT_ALWAYS( ((aConnIndex < iConnReq2DArr.Count()) && (aConnIndex >= 0)), User::Invariant());
	
	return iConnReq2DArr[aConnIndex].Count();
	}
	

//Test Case Name
void CPipeliningTestParams::SetTestCaseNameL(const TDesC& aTestCaseName ) 
	{
	iTestCaseNameBuf = HBufC::NewL(aTestCaseName.Size());
	(iTestCaseNameBuf->Des()).Copy(aTestCaseName);
	RemoveQuotes(iTestCaseNameBuf);
	}

TDesC& CPipeliningTestParams::TestCaseName() const
	{
	return (*(iTestCaseNameBuf));
	}


//Uri
void CPipeliningTestParams::SetUriL(TInt aUriIndex, const TDesC& aUri)
	{
	if (aUriIndex == 0)
		{
		iUriBufArr.Reset();
		}

	TInt uriSize = aUri.Size();

	HBufC8* tmpUri;
	tmpUri = HBufC8::NewLC(((uriSize%2) == 0)? (uriSize/2):((uriSize/2)+1));
	User::LeaveIfError(iUriBufArr.Append(tmpUri));
	((iUriBufArr[aUriIndex])->Des()).Copy(aUri);

	RemoveQuotes(iUriBufArr[aUriIndex]);
	CleanupStack::Pop(tmpUri);
	}



TInt CPipeliningTestParams::UriCount() const
	{
	return iUriBufArr.Count();
	}




TDesC8&	CPipeliningTestParams::UriL(TInt aUriIndex) const
	{
	__ASSERT_ALWAYS( ((aUriIndex < iUriBufArr.Count()) && (aUriIndex >= 0)), User::Leave(KErrArgument));
	
	return (*(iUriBufArr[aUriIndex]));
	}




//Raw Request
//store the raw request strings
void CPipeliningTestParams::SetRawRequestL(TInt aTransIndex, const TDesC& aRawRequest)
	{
	TInt rawRequestSize = aRawRequest.Size();
	HBufC8* tmpRequest = HBufC8::NewLC(((rawRequestSize%2) == 0)? (rawRequestSize/2):((rawRequestSize/2)+1));
	User::LeaveIfError(iRawRequestBufArr.Append(tmpRequest));
	((iRawRequestBufArr[aTransIndex])->Des()).Copy(aRawRequest);

	RemoveQuotes(iRawRequestBufArr[aTransIndex])	;
	ParseReplaceCRLF(iRawRequestBufArr[aTransIndex]) ;
	CleanupStack::Pop(tmpRequest);
	}
	
//return the raw request based on the request array (rawrequest[(iConnRequest2DArr[connIndex][transIndex])]
TDesC8&	CPipeliningTestParams::RawRequest(TInt aConnIndex, TInt aTransIndex) const
	{
	__ASSERT_ALWAYS( ((aConnIndex < iConnReq2DArr.Count()) && (aConnIndex >= 0)), User::Invariant());
	__ASSERT_ALWAYS( ((aTransIndex < iConnReq2DArr[aConnIndex].Count()) && (aTransIndex >= 0)), User::Invariant());

	TInt returnIndex = iConnReq2DArr[aConnIndex][aTransIndex];
	return (*( iRawRequestBufArr[ returnIndex - 1 ] ) );
	}

TDesC8&	CPipeliningTestParams::RawRequest( TInt aRawRequestBufArrIndex ) const
	{
	__ASSERT_ALWAYS( ((aRawRequestBufArrIndex < iRawRequestBufArr.Count()) && (aRawRequestBufArrIndex >= 0)), User::Invariant());

	return (*(iRawRequestBufArr[aRawRequestBufArrIndex]));
	}
	
//Raw Response
void CPipeliningTestParams::SetRawResponseL(TInt aTransIndex, const TDesC& aRawResponse)
	{
	TInt rawResponseSize = aRawResponse.Size();
	HBufC8* tmpResponse = HBufC8::NewLC(((rawResponseSize%2) == 0)? (rawResponseSize/2):((rawResponseSize/2)+1));
	User::LeaveIfError(iRawResponseBufArr.Append(tmpResponse));
	((iRawResponseBufArr[aTransIndex])->Des()).Copy(aRawResponse);

	RemoveQuotes(iRawResponseBufArr[aTransIndex])	;
	ParseReplaceCRLF(iRawResponseBufArr[aTransIndex]) ;
	CleanupStack::Pop(tmpResponse);
	}





TDesC8&	CPipeliningTestParams::RawResponse(TInt aConnIndex, TInt aTransIndex) const
	{
	__ASSERT_ALWAYS( ((aConnIndex < iConnResp2DArr.Count()) && (aConnIndex >= 0)), User::Invariant());
	__ASSERT_ALWAYS( ((aTransIndex < iConnResp2DArr[aConnIndex].Count()) && (aTransIndex >= 0)), User::Invariant());
		
	TInt returnIndex = iConnResp2DArr[aConnIndex][aTransIndex];
	return (*(iRawResponseBufArr[ returnIndex - 1 ]));
	}





TDesC8&	CPipeliningTestParams::RawResponse( TInt aRawResponseBufArrIndex ) const
	{
	__ASSERT_ALWAYS( ((aRawResponseBufArrIndex < iRawResponseBufArr.Count()) && (aRawResponseBufArrIndex >= 0)), User::Invariant());
	
	return (*(iRawResponseBufArr[aRawResponseBufArrIndex]));
	}




//Returns a TransInfo instance
TransInfo& CPipeliningTestParams::TransactionType(TInt atransTypeNum) const
	{
	TInt transTypeIndex = -1;
	for (TInt x=0; x < iTransTypesInfoPtrArr.Count(); ++x)
		{
		if (iTransTypesInfoPtrArr[x]->TransType == atransTypeNum)
			{
			transTypeIndex = x;
			}
		}
	__ASSERT_ALWAYS((transTypeIndex >= 0),User::Invariant());
	return (*(iTransTypesInfoPtrArr[transTypeIndex]));
	}




TInt CPipeliningTestParams::SetTransactionTypeL(TInt aTransTypeNumber, TDesC& aTransInfoDesc)
	{
	HBufC* transInfoBuf = HBufC::NewL(aTransInfoDesc.Size());
	CleanupStack::PushL( transInfoBuf );
	(transInfoBuf->Des()).Copy(aTransInfoDesc);

	RemoveQuotes(transInfoBuf)	;
	ParseReplaceCRLF(transInfoBuf) ;

	//Create an instance of transInfo structure on heap
	TransInfo* transInfo = new (ELeave) TransInfo;
	iTransTypesInfoPtrArr.Append(transInfo);
	
	//testing
	transInfo->TransHeaderTypeArr.Insert( -1 , 0 );
	
	//Set the transaction type number
	transInfo->TransType = aTransTypeNumber;
	
	//Parse the string transInfoBuf at comma's
	//Get a bunch of substrings delimited by comma	

	CheckSetHeaderValueL(*transInfoBuf, _L("*HTTP*"), *transInfo, EHttpHeader);
	CheckSetHeaderValueL(*transInfoBuf, _L("*Connection*"), *transInfo, EConnectionHeader);
	CheckSetHeaderValueL(*transInfoBuf, _L("*Batching*"), *transInfo, EBatchingHeader);
	CheckSetHeaderValueL(*transInfoBuf, _L("*Transaction*"), *transInfo, ETransactionHeader);
	
	CleanupStack::PopAndDestroy( transInfoBuf );
	
	return 0;												
	}





TBool CPipeliningTestParams::ProcessHeader() const
	{
	return iProcessHdr;
	}


void CPipeliningTestParams::SetHeaderToProcessL(const TInt aHdrIdx, const TDesC& aHeaderName ) 
	{	
	__ASSERT_ALWAYS((((aHdrIdx -1) >= 0) && ((aHdrIdx -1) == iHeader2ProcessArr.Count()  )), User::Invariant());
	
	TInt aHeaderNameSize = aHeaderName.Size();
	HBufC8* tmpHdr = HBufC8::NewLC(((aHeaderNameSize%2) == 0)? (aHeaderNameSize/2):((aHeaderNameSize/2)+1));
	User::LeaveIfError(iHeader2ProcessArr.Append(tmpHdr));
	((iHeader2ProcessArr[aHdrIdx-1])->Des()).Copy(aHeaderName);

	RemoveQuotes(iHeader2ProcessArr[aHdrIdx - 1])	;
	ParseReplaceCRLF(iHeader2ProcessArr[aHdrIdx - 1]) ;
	
	iProcessHdr = ETrue;
	CleanupStack::Pop(tmpHdr);
	}


// Number of headers for this transaction
TInt CPipeliningTestParams::NumHdrs() const
	{
	return iHeader2ProcessArr.Count();
	}


TDesC8& CPipeliningTestParams::HeaderToProcess(TInt aHdrIdx) const
	{
	__ASSERT_ALWAYS( (((aHdrIdx) < iHeader2ProcessArr.Count()) && ((aHdrIdx) >= 0)), User::Invariant());
	
	return (*(iHeader2ProcessArr[aHdrIdx]));
	}




//Value expected for the header
//1. Number of header values for this header
TInt CPipeliningTestParams::NumberOfHeaderVals(const TInt aHdrIdx) const
	{
	__ASSERT_ALWAYS( ((aHdrIdx < iHeader2ProcessArr.Count()) && (aHdrIdx >= 0)), User::Invariant());
	
	return iHdrVal2DArr[aHdrIdx].Count();
	}

TDesC8& CPipeliningTestParams::HeaderVal(const TInt aHdrCtr, const TInt aHdrValCtr ) const
	{
	__ASSERT_ALWAYS( ((aHdrCtr-1 < iHeader2ProcessArr.Count()) && (aHdrCtr-1 >= 0)), User::Invariant());		
	__ASSERT_ALWAYS( ((aHdrValCtr-1 < iHdrVal2DArr[aHdrCtr-1].Count()) && (aHdrValCtr-1 >= 0)), User::Invariant());
	
	return (*(iHdrVal2DArr[aHdrCtr - 1][aHdrValCtr - 1]));
	
	}

void CPipeliningTestParams::SetHdrValL(const TInt aHdrIdx, const TInt aHdrValIdx, const TDesC& aHdrVal)
	{
	__ASSERT_ALWAYS( (((aHdrIdx-1) < iHeader2ProcessArr.Count()) && ((aHdrIdx-1) >= 0)), User::Invariant());		


	TInt aHdrValSize = aHdrVal.Size();
	RArray < HBufC8 * > y ;

	
	if ( iHdrVal2DArr.Count() <= (aHdrIdx-1) )
		{
		User::LeaveIfError(iHdrVal2DArr.Append(y));
		}
	else
		{
		y = iHdrVal2DArr[aHdrIdx-1];
		}
		
	HBufC8* tmpHdr = HBufC8::NewLC(((aHdrValSize%2) == 0)? (aHdrValSize/2):((aHdrValSize/2)+1));
	User::LeaveIfError(y.Append(tmpHdr));
	y[aHdrValIdx-1]->Des().Copy(aHdrVal);		
	
	iHdrVal2DArr.Remove ( aHdrIdx - 1 );
	iHdrVal2DArr.Insert ( y, aHdrIdx - 1 );

	HBufC8* xyz = y[aHdrValIdx-1];
	HBufC8* xyz1 = iHdrVal2DArr[aHdrIdx-1][aHdrValIdx-1];
	
	TPtr8 ptr = xyz->Des();
	TPtr8 ptr1 = xyz1->Des();

	RemoveQuotes(iHdrVal2DArr[aHdrIdx - 1][aHdrValIdx - 1])	;
	ParseReplaceCRLF(iHdrVal2DArr[aHdrIdx - 1][aHdrValIdx - 1]) ;
	
	CleanupStack::Pop(tmpHdr);
	}
//Parse Replace to replace double quotes
void CPipeliningTestParams::RemoveQuotes(HBufC16* aHBufPtr) 
	{
	if (((*aHBufPtr)[0]) == KDoubleQuote)
		{
		(aHBufPtr->Des()).Delete(0,1);
		}
		
	TInt lastChar = ((aHBufPtr->Des()).Length());
	--lastChar;
	
	if ((*aHBufPtr)[lastChar] == KDoubleQuote)
		{
		(aHBufPtr->Des()).Delete(lastChar,1);
		}
		
	}




void CPipeliningTestParams::RemoveQuotes(HBufC8* aHBufPtr) 
	{
	if ((( *aHBufPtr )[0]) == KDoubleQuote)
		{
		( aHBufPtr->Des() ).Delete(0,1);
		}	
		
	TInt lastChar = ((aHBufPtr->Des()).Length());
	--lastChar;
	
	if (( *aHBufPtr )[lastChar] == KDoubleQuote)
		{
		( aHBufPtr->Des() ).Delete(lastChar,1);
		}
		
	}




//Parse Replace \n & \r
void CPipeliningTestParams::ParseReplaceCRLF(HBufC8* aHBufPtr)
	{
	TInt crPos = 0;
	TInt lfPos = 0;
	do
		{

		crPos = ((aHBufPtr->Des()).Find(KScriptCR8()));
		if (crPos != KErrNotFound)
			{
			(aHBufPtr->Des()).Replace(crPos,2, KReplaceCR8());
			}
			
		}
	while (crPos != KErrNotFound );


	do
		{

		lfPos = ((aHBufPtr->Des()).Find(KScriptLF8()));
		if (lfPos != KErrNotFound)
			{
			(aHBufPtr->Des()).Replace(lfPos,2, KReplaceLF8());
			}
			
		}
	while (lfPos != KErrNotFound );
	
	}




void CPipeliningTestParams::ParseReplaceCRLF(HBufC16* aHBufPtr)
	{
	TInt crPos = 0;
	TInt lfPos = 0;
	do
		{

		crPos = ((aHBufPtr->Des()).Find(KScriptCR()));
		if (crPos != KErrNotFound)
			{
			(aHBufPtr->Des()).Replace(crPos,2, KReplaceCR());
			}
			
		}
	while (crPos != KErrNotFound );


	do
		{

		lfPos = ((aHBufPtr->Des()).Find(KScriptLF()));
		if (lfPos != KErrNotFound)
			{
			(aHBufPtr->Des()).Replace(lfPos,2, KReplaceLF());
			}
			
		}
	while (lfPos != KErrNotFound );
	
	}
	
	
	
	

	
//parse and set the request or Response array 
void CPipeliningTestParams::SetSequenceArrL(const TDesC& aRawRequestOrResponseArr, TInt aInt)
	{
	TInt jj;
	TInt ii;
	TInt arrMemberInt = -1;
	RArray < TInt > aArray;
	CleanupClosePushL(aArray);
	ii = 0;
	jj = 0;
	if (aRawRequestOrResponseArr[jj] == ' ')
		{
		++jj;
		}		
	
	TInt arrSize = aRawRequestOrResponseArr.Length();
	for ( ii=0; ii < arrSize  ; ++ii )
		{
		if ( ( aRawRequestOrResponseArr[ii] == ',') ) //one integer, so process it
			{
			TPtrC intCharPtr = aRawRequestOrResponseArr.Mid ( jj, (ii - jj) );
			TLex	intLex2SlicePtr( intCharPtr );
			arrMemberInt = -1;
			intLex2SlicePtr.Val(arrMemberInt);
			
			if ( aInt == ETransactionArray )
				{
				User::LeaveIfError(iTransArr.Append(arrMemberInt));
				}
			else
				{
				User::LeaveIfError(aArray.Append(arrMemberInt));
				}
				
			jj = ii + 1;
			if ( aRawRequestOrResponseArr[jj] == ' ' )
				{
					++jj;
				}			
			}			
		}
		
	TInt lastInt;
	TPtrC lastCharPtr = aRawRequestOrResponseArr.Mid ( jj, (aRawRequestOrResponseArr.Length() - jj ) );
	TLex	lastIntLex2SlicePtr( lastCharPtr );
	lastIntLex2SlicePtr.Val(lastInt);
	
	if ( aInt == ETransactionArray )
		{
		User::LeaveIfError(iTransArr.Append( lastInt ));
		}
	else
		{
		User::LeaveIfError(aArray.Append( lastInt ));
		}
		
	
	if ( aInt == ERequestArray )
		{
		User::LeaveIfError(iConnReq2DArr.Append( aArray ));
		}		
	else if ( aInt == EResponseArray )
		{
		User::LeaveIfError(iConnResp2DArr.Append( aArray ));
		}
	CleanupStack::Pop();
	}





//return the reqarr value
TInt CPipeliningTestParams::TransactionSequenceArray(TInt aIndex) const
	{
	__ASSERT_ALWAYS( ((aIndex < iTransArr.Count()) && (aIndex >= 0)), User::Invariant());

	return iTransArr[aIndex];
	}
	
	
	
//Need to write this up
void CPipeliningTestParams::ValidateL() const
	{
	TInt err = KErrNone;
	TBool localUri = EFalse;
	
	// Are all mandatory fields present?
	// TestCaseName
	__ASSERT_ALWAYS((iTestCaseNameBuf->Des().Length() > 0), User::Leave(KErrGeneral));
	
	// Uri/Uri(n)
	__ASSERT_ALWAYS(iUriBufArr.Count() > 0, User::Leave(KErrGeneral));

	localUri = ((iUriBufArr.Count() == 1) && (((iUriBufArr[0]->Des()).Compare(_L8("http://127.0.0.1")) == 0) || ((iUriBufArr[0]->Des()).Compare(_L8("http://127.0.0.1/")) == 0)));
	
	// Pipelining or Batching
	__ASSERT_ALWAYS((iPipeliningEnabled) || (iBatchingEnabled), User::Leave(KErrGeneral));	

	// TotalTransactionCount
	__ASSERT_ALWAYS(iTotalTransactionCount > 0, User::Leave(KErrGeneral));
		
	// TransArr
	__ASSERT_ALWAYS(iTransArr.Count() == (iTotalTransactionCount), User::Leave(KErrGeneral));
	
	// ConnectionCount
	__ASSERT_ALWAYS(((!(localUri) && (iConnectionCount == 0)) || (iConnectionCount > 0)), User::Leave(KErrGeneral));
	
	// RawRequest
	// Greater than 0
	__ASSERT_ALWAYS(iRawRequestBufArr.Count() > 0, User::Leave(KErrGeneral));
	
	// RawResponse
	// Need not be there if connecting to external uri
	// Single external uri 
	__ASSERT_ALWAYS((localUri) || (iRawResponseBufArr.Count() > 0), User::Leave(KErrGeneral));
/*			
	if (localUri && iConnectionCount > 0 && !(iTestCaseNameBuf->Des().Compare(_L("Defect Fix INC036954")) == 0))
		{
		__ASSERT_ALWAYS(iConnReq2DArr.Count() == iConnectionCount, User::Leave(KErrGeneral));	
		}	
	// Are all optional fields correctly present?
*/	
	User::LeaveIfError( err );
	}
