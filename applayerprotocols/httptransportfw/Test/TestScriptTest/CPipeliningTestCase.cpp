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

#include "CPipeliningTestCase.h"
#include "httptestutils.h"
#include "CPipeliningTestParams.h"

//Helper function to convert TPtr to TInt
//declared as extern as it is already defined in 
//CPipeliningTestCaseParams.cpp as a static function
static TInt Ptr2Int(const TDesC8& aPtr)
	{
	TInt aInt;
	TLex8 aLex(aPtr);
	aLex.Val(aInt);
	return aInt;
	}

CPipeliningTestCase* CPipeliningTestCase::NewL(CHTTPTestUtils& aTestUtils, CPipeliningTestParams& aPipeliningTestParams)
	{
	CPipeliningTestCase* self = new (ELeave) CPipeliningTestCase(aTestUtils, aPipeliningTestParams);
	return self;
	}



CPipeliningTestCase::CPipeliningTestCase(CHTTPTestUtils& aTestUtils, CPipeliningTestParams& aPipeliningTestParams)
: iTestUtils(aTestUtils), iPipeliningTestParams(aPipeliningTestParams)
	{
	}



CPipeliningTestCase::~CPipeliningTestCase()
	{
	}



const TDesC& CPipeliningTestCase::TestCaseName() const
	{
	return iPipeliningTestParams.TestCaseName();
	}



TInt CPipeliningTestCase::TransactionCount(TInt aConnectionIndex) const
	{
	return iPipeliningTestParams.TransactionCount(aConnectionIndex);
	}



TInt CPipeliningTestCase::TotalTransactionCount() const
	{
	return iPipeliningTestParams.TotalTransactionCount();
	}


	
TBool CPipeliningTestCase::BatchTransactions() const
	{
	return ETrue;
	}	



TInt CPipeliningTestCase::ConnectionCount() const
	{
	return iPipeliningTestParams.ConnectionCount();
	}
	
TInt CPipeliningTestCase::ErrorVal() const
	{
		return iPipeliningTestParams.Error();
	}	

RHTTPTransaction CPipeliningTestCase::GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient)
	{
	__ASSERT_ALWAYS( aIndex < iPipeliningTestParams.TotalTransactionCount(), User::Invariant());
	
	if(iPipeliningTestParams.TestCaseName().Compare(_L("IOP TEST CASE 6")) == 0)
		{
		_LIT8(KTxtUri, "http://127.0.0.1");
		TUriParser8 uri; 
		uri.Parse(KTxtUri());
		
		RStringF method = aSession.StringPool().StringF(HTTP::EPOST,RHTTPSession::GetTable());
		RHTTPTransaction trans = aSession.OpenTransactionL(uri, aClient, method);
		RHTTPHeaders headers = trans.Request().GetHeaderCollection();
			
			
		_LIT8(KTxtHeaderName0, "Expect");
		_LIT8(KTxtHeaderValue0, "100-Continue");
		RStringF headerStrName0 = aSession.StringPool().OpenFStringL(KTxtHeaderName0());
		CleanupClosePushL(headerStrName0);
		RStringF headerStrValue0 = aSession.StringPool().OpenFStringL(KTxtHeaderValue0());
		CleanupClosePushL(headerStrValue0);
		THTTPHdrVal headerValue0(headerStrValue0);
		headers.SetFieldL(headerStrName0, headerValue0);
		CleanupStack::PopAndDestroy(2, &headerStrName0);
		return trans;
		}
	
	else if((iPipeliningTestParams.TestCaseName().Compare(_L("Test Case INC078630")) == 0) || (iPipeliningTestParams.TestCaseName().Compare(_L("Performance Test Case 8")) == 0))
		{
		
		_LIT8(KTxtUri, "Microsoft-Server-ActiveSync?DeviceId=Foo&DeviceType=9999&Cmd=Ping");
		TUriParser8 uri; 
		uri.Parse(KTxtUri());
		
		
		RStringF method = aSession.StringPool().StringF(HTTP::EPOST,RHTTPSession::GetTable());
		RHTTPTransaction trans = aSession.OpenTransactionL(uri, aClient, method);
		RHTTPHeaders headers = trans.Request().GetHeaderCollection();
			
			
		_LIT8(KTxtHeaderName0, "Host");
		_LIT8(KTxtHeaderValue0,	 "127.0.0.1");
		RStringF headerStrName0 = aSession.StringPool().OpenFStringL(KTxtHeaderName0());
		CleanupClosePushL(headerStrName0);
		RStringF headerStrValue0 = aSession.StringPool().OpenFStringL(KTxtHeaderValue0());
		CleanupClosePushL(headerStrValue0);
		THTTPHdrVal headerValue0(headerStrValue0);
		headers.SetFieldL(headerStrName0, headerValue0);
		CleanupStack::PopAndDestroy(2, &headerStrName0);
		
		_LIT8(KTxtHeaderName, "Accept-Language");
		_LIT8(KTxtHeaderValue, "en-us");
		RStringF headerStrName = aSession.StringPool().OpenFStringL(KTxtHeaderName());
		CleanupClosePushL(headerStrName);
		RStringF headerStrValue = aSession.StringPool().OpenFStringL(KTxtHeaderValue());
		CleanupClosePushL(headerStrValue);
		THTTPHdrVal headerValue(headerStrValue);
		headers.SetFieldL(headerStrName, headerValue);
		CleanupStack::PopAndDestroy(2, &headerStrName);
		
		_LIT8(KTxtHeaderName1, "MS-ASProtocolVersion");
		_LIT8(KTxtHeaderValue1, "2.5");
		RStringF headerStrName1 = aSession.StringPool().OpenFStringL(KTxtHeaderName1());
		CleanupClosePushL(headerStrName1);
		RStringF headerStrValue1 = aSession.StringPool().OpenFStringL(KTxtHeaderValue1());
		CleanupClosePushL(headerStrValue1);
		THTTPHdrVal headerValue1(headerStrValue1);
		headers.SetFieldL(headerStrName1, headerValue1);
		CleanupStack::PopAndDestroy(2, &headerStrName1);
		
		return trans;

		}
	else if(iPipeliningTestParams.TestCaseName().Compare(_L("Performance Test Case 9")) == 0)
		{
		
		_LIT8(KTxtUri, "http://www.newlc.com/Symbian-OS-Error-Codes.html");
		TUriParser8 uri; 
		uri.Parse(KTxtUri());
		
		
		RStringF method = aSession.StringPool().StringF(HTTP::EPOST,RHTTPSession::GetTable());
		RHTTPTransaction trans = aSession.OpenTransactionL(uri, aClient, method);
		RHTTPHeaders headers = trans.Request().GetHeaderCollection();
			
		_LIT8(KTxtHeaderName3, "Host");
		_LIT8(KTxtHeaderValue3,	 "194.146.227.34");
		RStringF headerStrName3 = aSession.StringPool().OpenFStringL(KTxtHeaderName3());
		CleanupClosePushL(headerStrName3);
		RStringF headerStrValue3 = aSession.StringPool().OpenFStringL(KTxtHeaderValue3());
		CleanupClosePushL(headerStrValue3);
		THTTPHdrVal headerValue3(headerStrValue3);
		headers.SetFieldL(headerStrName3, headerValue3);
		CleanupStack::PopAndDestroy(2, &headerStrName3);
			
		_LIT8(KTxtHeaderName1, "Content-Type");
		_LIT8(KTxtHeaderValue1, "*/*");
		RStringF headerStrName1 = aSession.StringPool().OpenFStringL(KTxtHeaderName1());
		CleanupClosePushL(headerStrName1);
		RStringF headerStrValue1 = aSession.StringPool().OpenFStringL(KTxtHeaderValue1());
		CleanupClosePushL(headerStrValue1);
		THTTPHdrVal headerValue1(headerStrValue1);
		headers.SetFieldL(headerStrName1, headerValue1);
		CleanupStack::PopAndDestroy(2, &headerStrName1);
			
		//Set the body data
		//The Content-Length will be set to 14
		if (iFormEncoder)
			{
			delete iFormEncoder;
			iFormEncoder = NULL;
			}
			
		iFormEncoder = CHTTPFormEncoder::NewL(); 
		TBuf8<256> name8;
		TBuf8<256> value8;
		_LIT(KName, "recherche");
		_LIT(KValue, "asdf");
		name8.Copy(KName);
		value8.Copy(KValue);
		iFormEncoder->AddFieldL(name8, value8);
		
		MHTTPDataSupplier* dataSupplier = this;
		dataSupplier = iFormEncoder;
		trans.Request().SetBody(*dataSupplier);
	
		return trans;

		}
	

	TInt transType = iPipeliningTestParams.TransactionSequenceArray(aIndex);	
	
	// Check the transaction type to see if its a HTTP 1.0 transaction
	// if 1.0 transaction then
	// change the session properties for transaction 1
	
	// Check the value for the TransactionList header in the ini file if it carries a value 
	// other than 1 at the position described by aIndex for the particular test case
	if ( (transType != 1) && (aIndex == 0))
		{
		TransInfo thisTransInfo = iPipeliningTestParams.TransactionType(transType);
		
		TInt valueForHttpHdr = thisTransInfo.TransHeaderTypeArr[EHttpHeader];
		if (( valueForHttpHdr != (ENoHeader) ) )
			{
			if ((thisTransInfo.TransHeaderInfoPtrArr[valueForHttpHdr])->Des().Compare(_L("1.0")))
				{
				RHTTPConnectionInfo connInfo = aSession.ConnectionInfo();
				RStringPool strPool = aSession.StringPool();
				THTTPHdrVal http10( strPool.StringF( HTTP::EHttp10, RHTTPSession::GetTable() ) );
				connInfo.SetPropertyL( strPool.StringF( HTTP::EHTTPVersion,RHTTPSession::GetTable() ), http10 );
				}
			}
		}
	
	//Check the request method type 
	RStringF method;
	if(iPipeliningTestParams.ReqMethod().Compare(_L("HEAD")) == 0)
		{
		method = aSession.StringPool().StringF(HTTP::EHEAD,RHTTPSession::GetTable());
		}
	else if(iPipeliningTestParams.ReqMethod().Compare(_L("POST")) == 0)
		{
		method = aSession.StringPool().StringF(HTTP::EPOST,RHTTPSession::GetTable());
		}
	else if(iPipeliningTestParams.ReqMethod().Compare(_L("GET")) == 0)
		{
		method = aSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());
		}		
	else if(iPipeliningTestParams.ReqMethod().Compare(_L("OPTIONS")) == 0)
		{
		method = aSession.StringPool().StringF(HTTP::EOPTIONS,RHTTPSession::GetTable());
		}		
	TInt aTempIndex(0);
	TInt uriCount(iPipeliningTestParams.UriCount());
	aTempIndex = (uriCount > 1) ? aIndex : (uriCount-1);
	
	TUriParser8 uri;
	uri.Parse(iPipeliningTestParams.UriL(aTempIndex));

	if ((iPipeliningTestParams.BatchingEnabled() == 1) && (iBatchingBufferSizeSet == 1))
		{
		TInt bufferSize = iPipeliningTestParams.BufferSize();
		RHTTPConnectionInfo connInfo = aSession.ConnectionInfo();
		connInfo.SetPropertyL(aSession.StringPool().StringF(HTTP::EBatchingBufferSize,RHTTPSession::GetTable()), bufferSize);
		}

	if( (iPipeliningTestParams.Error() == KErrCancel) && (iPipeliningTestParams.TransactionSequenceArray(aIndex) == 2 ))
		{
		iTransToCancel = aSession.OpenTransactionL(uri, aClient, method);
		return iTransToCancel;
		}

	// Check the value for the TransactionList header in the ini file if it carries a value 
	// other than 1 at the position described by aIndex for the particular test case
	else if ( transType != 1 )
		{
		//Get the transaction info
		TransInfo thisTransInfo = iPipeliningTestParams.TransactionType(transType);
		
		//Check if there is a Cancel transaction
		TInt valueForTransactionHdr = thisTransInfo.TransHeaderTypeArr[ETransactionHeader];
		if (( valueForTransactionHdr != ENoHeader ) )
			{
			if ((thisTransInfo.TransHeaderInfoPtrArr[valueForTransactionHdr])->Des().Compare(_L("Cancel")))
				{
				iTransToCancel = aSession.OpenTransactionL(uri, aClient, method);
				return iTransToCancel;
				}
			}
		
		//Create a plain-vanilla transaction
		RHTTPTransaction trans = aSession.OpenTransactionL(uri, aClient, method);
		
		//Check if there is a connection header
		TInt valueForConnectionHdr = thisTransInfo.TransHeaderTypeArr[::EConnectionHeader];
		if (( valueForConnectionHdr != ENoHeader ) )
			{
			if ( ( thisTransInfo.TransHeaderInfoPtrArr[valueForConnectionHdr])->Des().Compare(_L("Close") ) )
				{
				AddConnectionCloseHeaderL(trans);
				}
			}
		
			
		TInt valueForBatchingHdr = thisTransInfo.TransHeaderTypeArr[EBatchingHeader];
		if (( valueForBatchingHdr != ENoHeader ) && (thisTransInfo.TransHeaderInfoPtrArr[valueForBatchingHdr])->Des().Compare(_L("True")) )//Transaction needs a large header
			{
			// Add a fairly large header
			
			RHTTPHeaders headers = trans.Request().GetHeaderCollection();
			
			_LIT8(KTxtHeaderName, "This_Is_A_Long_Header_Name");
			_LIT8(KTxtHeaderValue, "This_Is_A_Long_Header_Value_abcdefghijklmnopqrstuvwxyz1234567890");
			RStringF headerStrName = aSession.StringPool().OpenFStringL(KTxtHeaderName());
			CleanupClosePushL(headerStrName);
			RStringF headerStrValue = aSession.StringPool().OpenFStringL(KTxtHeaderValue());
			CleanupClosePushL(headerStrValue);
			THTTPHdrVal headerValue(headerStrValue);
			headers.SetFieldL(headerStrName, headerValue);
			CleanupStack::PopAndDestroy(2, &headerStrName);
				
			}
		return trans;
		}

	return aSession.OpenTransactionL(uri, aClient, method);
	}


TBool CPipeliningTestCase::GetNextDataPart(TPtrC8& /*aDataPart*/)
	{
	return 0;
	}

void CPipeliningTestCase::ReleaseData()
	{
	
	}
TInt CPipeliningTestCase::OverallDataSize()
	{
	return 0;
	}
TInt CPipeliningTestCase::Reset()
	{
	return 0;
	}
const TDesC8& CPipeliningTestCase::GetRawRequest(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<iPipeliningTestParams.TotalTransactionCount()) && (aConnectionIndex < iPipeliningTestParams.ConnectionCount()), User::Invariant());
	
	return iPipeliningTestParams.RawRequest(aConnectionIndex, aTransIndex);
	}



	
const TDesC8& CPipeliningTestCase::GetRawResponse(TInt aConnectionIndex, TInt aTransIndex)
	{
	__ASSERT_ALWAYS( (aTransIndex<iPipeliningTestParams.TotalTransactionCount()) && (aConnectionIndex < iPipeliningTestParams.ConnectionCount()), User::Invariant());

	return iPipeliningTestParams.RawResponse(aConnectionIndex, aTransIndex);
	}



	
void CPipeliningTestCase::AddConnectionCloseHeaderL(RHTTPTransaction aTransaction)
	{
	RStringF name = aTransaction.Session().StringPool().StringF(HTTP::EConnection, RHTTPSession::GetTable());
	THTTPHdrVal value = aTransaction.Session().StringPool().StringF(HTTP::EClose, RHTTPSession::GetTable());

	aTransaction.Request().GetHeaderCollection().SetFieldL(name, value);
	}



	
TInt CPipeliningTestCase::RealExpectedConnectionCount() const
	{
	TInt numUris = iPipeliningTestParams.UriCount();
	_LIT8(KTxtUri, "http://127.0.0.1");
	//Check if the number of URIs in the ini file is more than one for the particular test case
	if ( numUris != 1)
		{
		TInt numLocalConnections = 0;
		TInt numExternalConnections = 0;
		for (TInt ii = 0; ii < numUris ; ++ ii)
			{
			TPtrC8 thisUri;
			TRAPD(err,thisUri.Set(iPipeliningTestParams.UriL(ii)));
			if(err != KErrArgument)
				{
				if (thisUri.Compare(KTxtUri) == 0)
					{
					++numLocalConnections;
					}
				else
					{
					++numExternalConnections;
					}
				}
			}
		
		TInt totalConnections = numLocalConnections + ((numExternalConnections != 0) ? 1:0);
		return totalConnections + 1;
		}		
	else
		{
		TPtrC8 firstUri;
		TRAPD(err,firstUri.Set(iPipeliningTestParams.UriL(0)));
		if(err != KErrArgument)
			{
			if (firstUri.Compare(KTxtUri) == 0)
				{
				return iPipeliningTestParams.ConnectionCount();
				}
			else
				{
				return 1;
				}
			}
		}
	return 0;
	}


TInt CPipeliningTestCase::ExpectedError(RHTTPTransaction aTrans)
	{
	if (iPipeliningTestParams.Error() == KErrCancel)
		{
		if(!iIsTransCancelled)
			{
			if(aTrans.Id() == 3)
				{
				iTransToCancel.Cancel();
				iIsTransCancelled = ETrue;
				iPipeliningTestParams.DecreaseTotalTransactionCount();
				}
			}
		
		if( iTransToCancel.Id() == aTrans.Id() )
			{
			return KErrCancel;
			}
		}
	else if ( iPipeliningTestParams.Error() == KErrDisconnected)
		{
		return KErrDisconnected;
		}
	
	return KErrNone;
	}

void CPipeliningTestCase::ProcessHeadersL(RHTTPTransaction aTrans)
	{

	_LIT(KTxtFailed, "**TEST FAILED**");
	_LIT(KExpectedValue, "Expected Value: %S");
	_LIT(KActualValue, "Actual Value: %S");
	_LIT(KExpectedLength, "Expected Content Length: %d");
	_LIT(KActualLength, "Actual Content Length: %d");
											
					
	if ( iPipeliningTestParams.ProcessHeader() )
		{
		TInt numHdrs = iPipeliningTestParams.NumHdrs();
		
		for (TInt xx=0; xx < numHdrs; ++xx )
			{
			//Check if the header to process is WWW-Authenticate
			if((iPipeliningTestParams.HeaderToProcess(xx).Compare(_L8("WWW-Authenticate"))) == 0)
				{
				RStringPool stringPool = aTrans.Session().StringPool();
				RHTTPHeaders headers = aTrans.Response().GetHeaderCollection();
				RStringF headerStr = stringPool.StringF(HTTP::EWWWAuthenticate, aTrans.Session().GetTable());
				FieldCompareL(headers.FieldPartsL(headerStr), iPipeliningTestParams.NumberOfHeaderVals(xx));
				FieldValueCompareL(headers, headers.FieldPartsL(headerStr), xx, headerStr);
				}
			
			//Check if the header to process is Upgrade
			else if ( (iPipeliningTestParams.HeaderToProcess(xx).Compare(_L8("Upgrade"))) == 0 )
				{
				RStringPool stringPool = aTrans.Session().StringPool();
				RHTTPHeaders headers = aTrans.Response().GetHeaderCollection();
				RStringF headerStr = stringPool.StringF(HTTP::EUpgrade, aTrans.Session().GetTable());
				FieldCompareL(headers.FieldPartsL(headerStr), iPipeliningTestParams.NumberOfHeaderVals(xx));
				FieldValueCompareL(headers, headers.FieldPartsL(headerStr), xx, headerStr);
				}

			//Check if the header to process is Content-Language
			else if ( (iPipeliningTestParams.HeaderToProcess(xx).Compare(_L8("Content-Language"))) == 0 )
				{
				RStringPool stringPool = aTrans.Session().StringPool();
				RHTTPHeaders headers = aTrans.Response().GetHeaderCollection();
				RStringF headerStr = stringPool.StringF(HTTP::EContentLanguage, aTrans.Session().GetTable());
				FieldCompareL(headers.FieldPartsL(headerStr), iPipeliningTestParams.NumberOfHeaderVals(xx));
				FieldValueCompareL(headers, headers.FieldPartsL(headerStr), xx, headerStr);
				}

			//Check if the header to process is Server
			else if ( (iPipeliningTestParams.HeaderToProcess(xx).Compare(_L8("Server"))) == 0 )
				{
				RStringPool stringPool = aTrans.Session().StringPool();
				RHTTPHeaders headers = aTrans.Response().GetHeaderCollection();
				RStringF headerStr = stringPool.StringF(HTTP::EServer, aTrans.Session().GetTable());
				FieldCompareL(headers.FieldPartsL(headerStr), iPipeliningTestParams.NumberOfHeaderVals(xx));
				FieldValueCompareL(headers, headers.FieldPartsL(headerStr), xx, headerStr);
				}	
			
			//Check if the header to process is Host
			else if ( (iPipeliningTestParams.HeaderToProcess(xx).Compare(_L8("Host"))) == 0 )
				{
				RStringPool stringPool = aTrans.Session().StringPool();
				RHTTPHeaders headers = aTrans.Request().GetHeaderCollection();
				THTTPHdrVal hdrVal;
				TBuf<100> buf1, buf2;
				RStringF headerStr = stringPool.StringF(HTTP::EHost, aTrans.Session().GetTable());
				FieldValueCompareL(headers, 1, xx, headerStr);
				}		
			
			//Check if the header to process is Content-Length
			else if ( (iPipeliningTestParams.HeaderToProcess(xx).Compare(_L8("Content-Length"))) == 0 )
				{
				RStringPool stringPool = aTrans.Session().StringPool();
				RHTTPHeaders headers = aTrans.Response().GetHeaderCollection();
				THTTPHdrVal hdrVal;

				//Check Content-Length header parameter is stored correctly
				RStringF headerStr = stringPool.StringF(HTTP::EContentLength, aTrans.Session().GetTable());
				User::LeaveIfError(headers.GetField(headerStr,0,hdrVal));
				
				FieldCompareL(headers.FieldPartsL(headerStr), iPipeliningTestParams.NumberOfHeaderVals(xx));	
	
				if (hdrVal.Int() != Ptr2Int(iPipeliningTestParams.HeaderVal(xx+1,1)) )
					{
					iTestUtils.LogIt(KExpectedLength, hdrVal.Int());
					iTestUtils.LogIt(KActualLength, Ptr2Int(iPipeliningTestParams.HeaderVal(xx + 1,1)));			
					iTestUtils.LogIt(KTxtFailed);
					User::Leave(KErrArgument);
					}
				iTestUtils.LogIt(KExpectedLength, hdrVal.Int());
				iTestUtils.LogIt(KActualLength, Ptr2Int(iPipeliningTestParams.HeaderVal(xx + 1,1)));	
				}
				
			//Check if the header to process is Content-Type
			else if ( (iPipeliningTestParams.HeaderToProcess(xx).Compare(_L8("Content-Type"))) == 0 )
				{
				RStringPool stringPool = aTrans.Session().StringPool();
				RHTTPHeaders headers = aTrans.Response().GetHeaderCollection();
				THTTPHdrVal hdrVal;
				TBuf<100> buf1, buf2;
				
				RStringF headerStr = stringPool.StringF(HTTP::EContentType, aTrans.Session().GetTable());
				
				FieldCompareL(headers.FieldPartsL(headerStr), iPipeliningTestParams.NumberOfHeaderVals(xx));
				
				for ( TInt mn =0; mn < headers.FieldPartsL(headerStr); ++mn)
					{
					User::LeaveIfError(headers.GetField(headerStr,mn,hdrVal));
					TPtrC8 ptrValue1 ( hdrVal.StrF ().DesC() );   
					if (TInt mno = ptrValue1.Compare(iPipeliningTestParams.HeaderVal(xx+1,mn+1)) != 0)
						{
						buf1.Copy(ptrValue1);
						buf2.Copy(iPipeliningTestParams.HeaderVal(xx + 1,mn + 1));
						iTestUtils.LogIt(KExpectedValue, &buf1);
						iTestUtils.LogIt(KActualValue, &buf2);			
						iTestUtils.LogIt(KTxtFailed);
						User::Leave(KErrArgument);
						}
					buf1.Copy(ptrValue1);
					buf2.Copy(iPipeliningTestParams.HeaderVal(xx + 1,mn + 1));
					iTestUtils.LogIt(KExpectedValue, &buf1);
					iTestUtils.LogIt(KActualValue, &buf2);
					
					/*
					RStringF param = stringPool.StringF(HTTP::ECharset, aTrans.Session().GetTable());
					THTTPHdrVal paramval;
					User::LeaveIfError(headers.GetParam(headerStr, param, paramval, mn));
					TPtrC8 paramval1(paramval.StrF().DesC());
					TBuf<100> buf3;
					buf3.Copy(paramval1);
					_LIT(KTxtParam, "Parameter Charset= %S");
					iTestUtils.LogIt(KTxtParam, &buf3);
					*/
					}
				}
			
			//Check if the header to process is Date
			else if ( (iPipeliningTestParams.HeaderToProcess(xx).Compare(_L8("Date"))) == 0 )
				{
				RStringPool stringPool = aTrans.Session().StringPool();
				RHTTPHeaders headers = aTrans.Response().GetHeaderCollection();
				THTTPHdrVal hdrVal;
												
				RStringF headerStr = stringPool.StringF(HTTP::EDate, aTrans.Session().GetTable());
				
				if( headers.FieldPartsL(headerStr) != iPipeliningTestParams.NumberOfHeaderVals(xx))
					{	
					User::Leave(KErrArgument);
					}
					
				for ( TInt mn =0; mn < headers.FieldPartsL(headerStr); ++mn)
					{
					User::LeaveIfError(headers.GetField(headerStr,mn,hdrVal));
					TDateTime rdt = hdrVal.DateTime();
					}
				}	
			
			//Check if the header to process is MsgBody
			else if ( (iPipeliningTestParams.HeaderToProcess(xx).Compare(_L8("MsgBody"))) == 0 )
				{
				RStringPool stringPool = aTrans.Session().StringPool();
				THTTPHdrVal hdrval;
										
				if(aTrans.Response().HasBody())
					{
					_LIT(KStrHasBody, "Response stream has a body");
					iTestUtils.LogIt(KStrHasBody);	
					}
				else
					{
					_LIT(KStrNoBody, "Response stream has no body");
					iTestUtils.LogIt(KStrNoBody);	
					}	
				
				if ( iPipeliningTestParams.HeaderVal(xx+1, 1).Compare(_L8("Null")) == 0)
					{
					TBool hasBody = aTrans.Response().HasBody();
					if ( hasBody )
						{
						_LIT(KTxtExpectedMsg, "Expected message body: NULL");
						iTestUtils.LogIt(KTxtExpectedMsg);
						iTestUtils.LogIt(KTxtFailed);
						User::Leave(KErrArgument);
						}
					else
						{
						_LIT(KTxtHasBody, "Message Body: NULL");
						iTestUtils.LogIt(KTxtHasBody);
						}	
					}
				}
			}	
		}
	}

TBool CPipeliningTestCase::EnableBatching() const
	{
	return iPipeliningTestParams.BatchingEnabled();
	}

/**
Compares the number fields in the String pool with those provided in the ini file
@param aStringPool
	The String Pool
@param aIniFile
	The fields from the ini file
@leave
	KErrArgument if the number of fields in the ini file do not match those from the string pool
*/
void CPipeliningTestCase::FieldCompareL(TInt aStringPool, TInt aIniFile)
	{
	
	_LIT(KExpectedFields, "Expected number of fields: %d");
	_LIT(KActualFields, "Actual number of fields: %d");
	_LIT(KTxtFailed, "**TEST FAILED**");
	
	if( aStringPool != aIniFile)
		{
		iTestUtils.LogIt(KExpectedFields,aStringPool );
		iTestUtils.LogIt(KActualFields, aIniFile);
		iTestUtils.LogIt(KTxtFailed);
		User::Leave(KErrArgument);
		}
	iTestUtils.LogIt(KExpectedFields,aStringPool);
	iTestUtils.LogIt(KActualFields, aIniFile);
	
	}

/**
Compares the values provided for each header in the ini file with those obtained from
the string pool 
@param headers
	Collection of headers from the response/request
@param xyz
	Number of fields/headers present in the string pool
@param xx
	Index value the header that needs to be processed
@param headerStr
	Holds the header name that is obtained from the string pool
@leave
	KErrArgument if the field values do not match
*/
void CPipeliningTestCase::FieldValueCompareL(RHTTPHeaders headers, TInt xyz, TInt xx, RStringF headerStr)
	{
	
	_LIT(KTxtFailed, "**TEST FAILED**");
	_LIT(KExpectedValue, "Expected Value: %S");
	_LIT(KActualValue, "Actual Value: %S");
	
	TBuf<100> buf1, buf2;
	THTTPHdrVal hdrVal;
	
	for ( TInt mn =0; mn < xyz; ++mn)
		{
		User::LeaveIfError(headers.GetField(headerStr,mn,hdrVal));
		TPtrC8 ptrValue1 ( hdrVal.StrF ().DesC() );   
		if (TInt mno = ptrValue1.Compare(iPipeliningTestParams.HeaderVal(xx+1,mn+1)) != 0)
			{
			buf1.Copy(ptrValue1);
			buf2.Copy(iPipeliningTestParams.HeaderVal(xx + 1,mn + 1));
			iTestUtils.LogIt(KExpectedValue, &buf1);
			iTestUtils.LogIt(KActualValue, &buf2);			
			iTestUtils.LogIt(KTxtFailed);
			User::Leave(KErrArgument);
			}
		buf1.Copy(ptrValue1);
		buf2.Copy(iPipeliningTestParams.HeaderVal(xx + 1,mn + 1));
		iTestUtils.LogIt(KExpectedValue, &buf1);
		iTestUtils.LogIt(KActualValue, &buf2);			
		}		
	}







