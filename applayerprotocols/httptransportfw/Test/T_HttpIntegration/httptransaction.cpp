// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// $Header$
// by:		
// for:	
// Include Files  
// 
//

#include <e32base.h>
#include "CHTTPFamily.h"
#include "httptransaction.h"

//	panic msg
_LIT(KBufferPanic,		"HTTPTRANSACTION-BUFFER");
const TInt KMaxFileBufferSize = 65535;

//_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");
_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S");

_LIT(KITHContentType, "ContentType");		//	Integration Test Harness $$ var name
_LIT(KHTTPContentType, "Content-Type");	//	HTTP Protocol

_LIT(KITHContentLength, "ContentLength");		//	Integration Test Harness $$ var name

_LIT(KITHAccept, "Accept");		//	Integration Test Harness $$ var name

_LIT(KITHAcceptCharSet, "AcceptCharSet");		//	Integration Test Harness $$ var name

_LIT(KITHUserAgent, "UserAgent");		//	Integration Test Harness $$ var name

_LIT(KITHAcceptEncoding, "AcceptEncoding");		//	Integration Test Harness $$ var name


_LIT(KITHDisablePipelining, "DisablePipelining");
_LIT(KITHYes,"Y");

const TInt K_OUTOFBOUNDS_YEAR = 2499;

void CTestTransaction::Log(TRefByValue<const TDesC> aFmt, ... )
	{
	VA_LIST list;
	VA_START(list, aFmt);

	Machine()->MsgPrintfln(aFmt, list);
	}
	
void CTestTransaction::WriteDateStamp()
	{
	TDateTime myTempDate(K_OUTOFBOUNDS_YEAR, EJanuary, 0,0,0,0,0);
	WriteDateStamp(myTempDate);
	}

void CTestTransaction::WriteDateStamp(const TDateTime &aDate)
	{
	TDateTime date;
	TTime now;
	TBool iEOL = (aDate.Year() == K_OUTOFBOUNDS_YEAR);

	if (iEOL)
		{
		now.HomeTime();
		date = now.DateTime();
		}
	else
		date = aDate;

	TTime t(date);
	TBuf<128> dateTimeString;
	TRAPD(err, t.FormatL(dateTimeString, KDateFormat));
	if (err == KErrNone)
		{
		if (iEOL)
			Machine()->MsgPrintf(_L("[%S] "), &dateTimeString);
		else
			Log(_L("[%S]\r\n"), &dateTimeString);
		}
	} 

TInt CTestTransaction::Error( TInt aError, TRefByValue<const TDesC> aFmt, ... )
	{
	VA_LIST list;
	VA_START(list, aFmt);
	Machine()->MsgPrintfln(aFmt, list);
	return aError;
	}
	
CTestTransaction *CTestTransaction::NewL( RHTTPSession& aSession,
										  HTTP::TStrings aTransactiontype,
										  const TDesC& aUri,
										  const TDesC &aFilename,
										  const TDesC &aTransactionName,
									      CTEngine *aTestMachine)
	{
	CTestTransaction *self = NewLC( aSession, 
	                                aTransactiontype, 
	                                aUri, 
	                                aFilename, 
	                                aTransactionName,
									aTestMachine);
	CleanupStack::Pop();
	return self; 
	}
	
//-----------------------------------------------------------------------------

CTestTransaction *CTestTransaction::NewLC( RHTTPSession& aSession,
										   HTTP::TStrings aTransactiontype,
										   const TDesC& aUri,
										   const TDesC &aFilename,
										   const TDesC &aTransactionName,
									       CTEngine *aTestMachine)
	{
	CTestTransaction* self = new (ELeave) CTestTransaction( aTransactiontype,
										                    aUri,
										                    aFilename,
										                    aTransactionName,
										                    aTestMachine);
	

	CleanupStack::PushL(self);
	self->ConstructL(aSession);
	return self;
	}

//-----------------------------------------------------------------------------

void CTestTransaction::ConstructL(RHTTPSession& aSession)
	{
	TBool hasRequestBody = EFalse;

	switch (iTransactionType)
		{
		case HTTP::EPOST : 
		case HTTP::EPUT : 

			if (iFilename.Length() > 0)
				{
				User::LeaveIfError(SetFileNameL(iFilename));
				
				hasRequestBody = ETrue;
				}
		break ;
		
		case HTTP::EGET :
		case HTTP::EDELETE:
		case HTTP::EHEAD:
		case HTTP::ECONNECT :
		default:
		;
		}
	
	TUriParser8 KURI;
	KURI.Parse(iUri);		

	RStringPool strPool = aSession.StringPool();
	
	//	open a transaction within the required session and use the class callback object for 
	//	managing the subsequent events...

	iTransaction = aSession.OpenTransactionL(KURI, *this, strPool.StringF(iTransactionType, RHTTPSession::GetTable()));

	iState = EActive;
	
	RHTTPRequest request = iTransaction.Request();

	RHTTPHeaders headers = request.GetHeaderCollection();
	
	AddAcceptHeaderL(headers, strPool);

	SetUserAgentL(headers, strPool);

	SetContentLengthL(headers, strPool);

	AcceptEncodingL(headers, strPool);
	
	// Check to see if pipelining should be disabled

	if (Machine()->GetDefine(KITHDisablePipelining).CompareF(KITHYes)==0)
		{
		RHTTPTransactionPropertySet transactionPropertySet = iTransaction.PropertySet();
		RStringF name = strPool.StringF(HTTP::EHttpPipelining, RHTTPSession::GetTable());
		RStringF value = strPool.StringF(HTTP::EDisablePipelining,RHTTPSession::GetTable());
		transactionPropertySet.SetPropertyL(name,value);
		}
	
	

	//	Add request body
	if (hasRequestBody)
		{
		if (!AddRequestBodyL(headers, strPool))
			User::LeaveIfError(KErrArgument);
		}
	}

//-----------------------------------------------------------------------------

CTestTransaction::CTestTransaction(	HTTP::TStrings aTransactionType,
									const TDesC& aUri,
									const TDesC &aFilename,
									const TDesC &aTransactionName,
									CTEngine *aTestMachine): iFileBufferPtr(0,0)
	{
	iTransactionType = aTransactionType;
	iUri.Copy(aUri);
	iFilename.Copy(aFilename);
	iTransactionName.Copy(aTransactionName);
	iEngine = aTestMachine ;
	iState = EUnInitialised;
	}

CTestTransaction::~CTestTransaction(void)
	{
	Machine()->DeleteErrorDefine(iTransactionName);
	Machine()->DeleteResultDefine(iTransactionName);
	}


TBool CTestTransaction::GetNextDataPart(TPtrC8 &aDataPart)
{
//	__ASSERT_DEBUG(iFileBuffer, User::Panic(KBufferPanic, EBufferNotAllocated));
    Log(_L("GetNextDataPart"));
	//	Read from the request body file
	TBool retVal = EFalse;
	if ( (iFileBuffer->Des().Length() == 0) && ( (iDataChunkCount == 0) || iReleaseData) ) 
		{
		TInt err = iReqFile.Read(iFileBufferPtr);
		if (err == KErrNone)
			{
			aDataPart.Set(iFileBufferPtr);
			if (iFileBufferPtr.Length() == 0)
				{
				iReqFile.Close();
				retVal = ETrue;
				}
			}
		}
	else 
		{
		aDataPart.Set(iFileBufferPtr);
		if (iFileBufferPtr.Length() == 0)
			retVal = ETrue;
		}

    iReleaseData = EFalse;
	return retVal;
}

TInt CTestTransaction::OverallDataSize()
	{
	Log(_L("OverallDataSize"));
	TInt size = 0;
	if (iReqFile.Size(size))
		User::Panic(KBufferPanic, EInvalidFileSizeError);

	return size;
	}

//-----------------------------------------------------------------------------
// Clear out the submit buffer

void CTestTransaction::ReleaseData()
	{
    Log(_L("ReleaseData"));
	iReleaseData = ETrue;
	++iDataChunkCount;
	if (iFileBuffer->Des().Length() != 0)
		{
		TRAPD(error, iTransaction.NotifyNewRequestBodyPartL());
		if (error != KErrNone) 
		    {
		    Log(_L("RHTTPTransaction::NotifyNewRequestBodyPartL %d"), error);				
		    }
		iFileBuffer->Des().Zero();
		}
	}

//-----------------------------------------------------------------------------

TInt CTestTransaction::Reset()
	{
	return KErrNotSupported;
	}

void CTestTransaction::DoneWaitEvents()
	{
	//	complete the test machine - whll then get the next command
	Machine()->CompleteRequest();
	}
	
//-----------------------------------------------------------------------------
void CTestTransaction::SetTransaction(RHTTPTransaction &aTransaction)
	{
	iTransaction = aTransaction;
	}
 
const RHTTPTransaction &CTestTransaction::Transaction() const
	{
    return (iTransaction);
    }
    
TInt CTestTransaction::SetFileNameL(const TDesC &aName)
	{
	TInt err = Machine()->SetFileName(aName, iReqFile);
	
	if (err == KErrNone)
		{
		//	recreate the buffer to hold the file
		delete iFileBuffer;
		iFileBuffer = NULL;		//	ensure we zap it completely!!!
		iFileBuffer = HBufC8::NewMaxL(KMaxFileBufferSize);
		iFileBuffer->Des().Zero();
		iFileBufferPtr.Set(iFileBuffer->Des());
		}
	
	return err;
}

TBool CTestTransaction::AddRequestBodyL( RHTTPHeaders &aHeaders,
									    RStringPool &aStrPool)
	{
	//	the content type info will be in $ContentType$. If NULL or Empty then an error generated
	TBuf8<KMaxContentTypeSize> contTypeBuf;
	contTypeBuf.Copy(Machine()->GetDefine(KITHContentType));
	if (contTypeBuf.Length() == 0)
		return EFalse; //Error(KErrArgument, THA_KErrNoContentTypeDefined, &transmthd);

	RStringF contTypeStr = aStrPool.OpenFStringL(contTypeBuf);
	THTTPHdrVal contType(contTypeStr);
	aHeaders.SetFieldL(aStrPool.StringF(HTTP::EContentType, RHTTPSession::GetTable()), contType);
	contTypeStr.Close();

	MHTTPDataSupplier *dataSupplier = this;
	
    iTransaction.Request().SetBody(*dataSupplier);

	return ETrue;
	}

//-----------------------------------------------------------------------------

TBool CTestTransaction::AddAcceptHeaderL(RHTTPHeaders &aHeaders, RStringPool &aStrPool)
	{
	//	some tests may require specific 'Accept' parameters
	TBuf8<KMaxAcceptLengthSize> acceptBuf;
	acceptBuf.Copy(Machine()->GetDefine(KITHAccept));
	RStringF accept = (acceptBuf.Length() == 0) ?
		aStrPool.StringF(HTTP::EAnyAny, RHTTPSession::GetTable()) : aStrPool.OpenFStringL(acceptBuf);
	THTTPHdrVal acceptHdr(accept);
	aHeaders.SetFieldL(aStrPool.StringF(HTTP::EAccept, RHTTPSession::GetTable()), acceptHdr);
	accept.Close();
	return ETrue;
	}

//-----------------------------------------------------------------------------
//	some tests may require specific 'Accept' parameters

TBool CTestTransaction::AcceptCharSetL(RHTTPHeaders &aHeaders, RStringPool &aStrPool)
	{
	TBuf8<KMaxAcceptCharSetSize> acceptCharSetBuf;
	acceptCharSetBuf.Copy(Machine()->GetDefine(KITHAcceptCharSet));
	if (acceptCharSetBuf.Length() > 0)
		{
		RStringF accept = aStrPool.OpenFStringL(acceptCharSetBuf);
		THTTPHdrVal acceptCharSetHdr(accept);
		aHeaders.SetFieldL(aStrPool.StringF(HTTP::EAcceptCharset, RHTTPSession::GetTable()), acceptCharSetHdr);
		accept.Close();
		}

	return ETrue;
	}

void CTestTransaction::SubmitL()
	{
	iTransaction.SubmitL();
	}
	
TInt CTestTransaction::MHFRunError(TInt aError, 
								  RHTTPTransaction aTransaction,
								  const THTTPEvent& aEvent)
	{
	//	date stamp the response
	WriteDateStamp();
	Log(_L("MHFRunError  : Test failed with error code %d"), aError);

	Machine()->SetErrorDefine(iTransactionName, aEvent.iStatus);

	//	set the response status code for processing l8r...
	TInt statusCode = aTransaction.Response().StatusCode();

	//	date stamp the response
	WriteDateStamp();

	Log(_L("Event Status Code: %d"), statusCode);

	Machine()->SetResultDefine(iTransactionName, statusCode);

	// SetCompletionCode(statusCode);

	return KErrNone;
	}

//-----------------------------------------------------------------------------
//	some tests may require specific 'AcceptEncoding' parameters

TBool CTestTransaction::AcceptEncodingL(RHTTPHeaders &aHeaders, RStringPool &aStrPool)
	{
	TBuf8<KMaxAcceptEncodingSize> acceptEncodingBuf;
	acceptEncodingBuf.Copy(Machine()->GetDefine(KITHAcceptEncoding));
	if (acceptEncodingBuf.Length() > 0)
		{
		RStringF acceptEncoding = aStrPool.OpenFStringL(acceptEncodingBuf);
		THTTPHdrVal acceptEncodingHdr(acceptEncoding);
		aHeaders.SetFieldL(aStrPool.StringF(HTTP::EAcceptCharset, RHTTPSession::GetTable()), acceptEncodingHdr);
		acceptEncoding.Close();
		}

	return ETrue;
	}

//-----------------------------------------------------------------------------
//	Add User Agent to header if supplied else default

TBool CTestTransaction::SetUserAgentL(RHTTPHeaders &aHeaders, RStringPool &aStrPool)
	{
	//	get the DEFINE for UserAgent
	TBuf8<KMaxUserAgentSize> userAgentBuf;
	userAgentBuf.Copy(Machine()->GetDefine(KITHUserAgent));

	TInt iLen = userAgentBuf.Length();
	//	assign the selection
	RStringF agent = (iLen == 0) ? aStrPool.OpenFStringL(KUserAgent) : aStrPool.OpenFStringL(userAgentBuf);
	THTTPHdrVal userAgent(agent);
	aHeaders.SetFieldL(aStrPool.StringF(HTTP::EUserAgent, RHTTPSession::GetTable()), userAgent);
	agent.Close();

	return ETrue;
	}

//-----------------------------------------------------------------------------
//	Add Content Length Header

TBool CTestTransaction::SetContentLengthL(RHTTPHeaders &aHeaders, RStringPool &aStrPool)
	{
	TBuf8<KMaxContentLengthSize> contLengthBuf;
	contLengthBuf.Copy(Machine()->GetDefine(KITHContentLength));
	if (contLengthBuf.Length() != 0)
		{
		RStringF contLengthStr = aStrPool.OpenFStringL(contLengthBuf);
		THTTPHdrVal contLength(contLengthStr);
		aHeaders.SetFieldL(aStrPool.StringF(HTTP::EContentLength, RHTTPSession::GetTable()), contLength);
		contLengthStr.Close();
		}

	return ETrue;
	}
	
//-----------------------------------------------------------------------------
//	dump all the body's chunks 

void CTestTransaction::DumpResponseBody(RHTTPTransaction &aTrans)
	{
	
	MHTTPDataSupplier *body = aTrans.Response().Body();
	TPtrC8 dataChunk;
	TBool isLast = body->GetNextDataPart(dataChunk);
	Machine()->DumpData(dataChunk);
	if (isLast)// && !iEngine->IsSilent())
		{
		//	date stamp the response
		WriteDateStamp();
		Log(_L("  - Got last data chunk."));
		}
	body->ReleaseData();
	}


void CTestTransaction::DumpRespHeaders(RHTTPTransaction &aTrans)
	{
		
	RHTTPResponse resp = aTrans.Response();
	RStringPool strP = aTrans.Session().StringPool();
	RHTTPHeaders hdr = resp.GetHeaderCollection();
	THTTPHdrFieldIter it = hdr.Fields();

	TBuf<32>  fieldName16;
	TBuf<128> fieldVal16;

	while (it.AtEnd() == EFalse)
		{
		RStringTokenF fieldNameTk = it();
		RStringF fieldName = strP.StringF(fieldNameTk);
		THTTPHdrVal hVal;
		if (hdr.GetField(fieldName, 0, hVal) == KErrNone)
			{
			TPtrC8 fieldNameStr(strP.StringF(fieldName).DesC());
			if (fieldNameStr.Length() > 32)
				fieldNameStr.Set(fieldNameStr.Left(32));

			fieldName16.Copy(fieldNameStr);

			THTTPHdrVal fieldVal;
			hdr.GetField(fieldName, 0, fieldVal);
			switch (fieldVal.Type())
				{
				case THTTPHdrVal::KTIntVal: Log(_L("%S: %d"), &fieldName16, fieldVal.Int()); break;
				case THTTPHdrVal::KStrVal:
				case THTTPHdrVal::KStrFVal:
					{
					TPtrC8 fieldValStr(strP.StringF(fieldVal.StrF()).DesC());
					if (fieldValStr.Length() > 128)
						fieldValStr.Set(fieldValStr.Left(128));

					fieldVal16.Copy(fieldValStr);
					Log(_L("%S: %S"), &fieldName16, &fieldVal16);

					//	see if we've got the Content-Type header
					if (fieldName16.Find(KHTTPContentType) != KErrNotFound)
						{
						//	check that the contenttype script sets matches (in some way) received header
						TBuf8<KMaxContentTypeSize> contTypeBuf;
						contTypeBuf.Copy(Machine()->GetDefine(KITHContentType));

						TInt iPos = fieldValStr.Find(contTypeBuf);
						if (iPos == KErrNotFound)
							Log(_L("  - Content Type string [%S:%S] is different to ContentType setting"), &fieldName16, &fieldVal16);
						else	
							Log(_L("  - Content Type [%S:%S] acknowledged"), &fieldName16, &fieldVal16);
						}
					} 
					break;
				case THTTPHdrVal::KDateVal: 
					{
					TDateTime myDate = fieldVal.DateTime();
					WriteDateStamp(myDate); 
					Log(_L(" : %S"), &fieldName16);
					}
					break;

				default: Log(_L("%S: <unrecognised value type>"), &fieldName16); break;
				}
			}
		++it;
		}
	}
	
void CTestTransaction::CloseTransaction()
	{
	if (iState != EClosed)
		{
		iTransaction.Close();
		iState = EClosed;
		}
	}
	
void CTestTransaction::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
{
	//	log the event code 
	THTTPEvent	receivedError = aEvent;

	Machine()->SetErrorDefine(iTransactionName, receivedError.iStatus);

	if (aEvent.iStatus < 0)
		{
		WriteDateStamp();

		switch (aEvent.iStatus)
			{
			case KErrEtelNoAnswer : 
				Log(_L("CCmdTransaction::MHFRunL(%S) - No Answer! Has the RAS or Internet Connection been enabled???"), &iTransactionName); 
				break;
			case -3606 : 
				Log(_L("CCmdTransaction::MHFRunL(%S) - Not able to make connection due to invalid or undefined COMMSDB configuration"), &iTransactionName); 
				break;
			case KErrHttpGeneralHeaderMissingHost : 
				Log(_L("CCmdTransaction::MHFRunL(%S) - General Header missing host information"), &iTransactionName); 
				break;
			case KErrHttpCantResetRequestBody : 
				Log(_L("CCmdTransaction::MHFRunL(%S) - Cannot reset request body. Redirect received"), &iTransactionName); 
				break;
			case KErrHttpInvalidHeaderInRequest : 
				Log(_L("CCmdTransaction::MHFRunL(%S) - Invalid Header in Request"), &iTransactionName); 
				break;
			case KErrHttpEncodeUserAgent : 
				Log(_L("CCmdTransaction::MHFRunL(%S) - Invalid Encode User Agent Header in Request"), &iTransactionName); 
				break;
			default : 
				Log(_L("CCmdTransaction::MHFRunL(%S) - The error [%d] was unexpected"), &iTransactionName, aEvent.iStatus); 
				break;
			}
			
		return;
		}

	
	TInt statusCode = aTransaction.Response().StatusCode();

	WriteDateStamp();

	Log(_L("CCmdTransaction::MHFRunL( %S: Event Status Code: %d"), &iTransactionName, statusCode);

	Machine()->SetResultDefine(iTransactionName, statusCode);

	switch (aEvent.iStatus)
		{
		// The transaction's response is complete
		case THTTPEvent::EResponseComplete:
			Log(_L("(%S)  : EResponseComplete received (Event Status: %d)"),&iTransactionName, aEvent.iStatus);
			break;

		//	transaction completed (and successful)
		case THTTPEvent::ESucceeded:
			Log(_L("(%S)  : ESucceeded received from the VF (Event Status: %d)"), &iTransactionName, aEvent.iStatus);
			CloseTransaction();
			break;

		//	transaction completed (and successful)
		case THTTPEvent::EFailed:
			Log(_L("(%S)  : Transaction EFailed received (Event Status: %d)"), &iTransactionName, aEvent.iStatus);
			CloseTransaction();
			//Validate(aTransaction);
			break;

		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received
			Log(_L("(%S)  : EGotResponseHeaders (%d)"),&iTransactionName, aEvent.iStatus);
			
			//TInt statusCode = aTransaction.Response().StatusCode();
			//Log(_L("Status Code: %d "), statusCode);
			//SetCompletionCode(statusCode);
			DumpRespHeaders(aTransaction);
			}
			break;

		case THTTPEvent::EGotResponseBodyData:
			{
			// Some (more) body data has been received (in the HTTP response)
			Log(_L("(%S)  : EGotResponseBodyData received (Event Status: %d"), &iTransactionName, aEvent.iStatus);

			// for each chunk of data received we have to empty the buffer before to be able to receive 
			MHTTPDataSupplier *body = aTransaction.Response().Body();
			body->ReleaseData();
			} 
			break;

		case KErrHttpRequestHasBody:
			Log(_L("(%S)  : KErrHttpRequestHasBody received from the VF (Event Status: %d)"), &iTransactionName, aEvent.iStatus);
			break;

		case KErrHttpEntityHeaderMissingContentType:
			Log(_L("(%S)  : KErrHttpEntityHeaderMissingContentType received from the VF (Event Status: %d)"), &iTransactionName, aEvent.iStatus);
			break;

		case KErrHttpRequestBodyMissing:
			Log(_L("(%S)  : KErrHttpRequestBodyMissing  received from the VF (Event Status: %d)"), &iTransactionName, aEvent.iStatus);
			break;

		case KErrTimedOut:							//-33
			Log(_L("(%S)  : KErrTimedOut  received from the VF (Event Status: %d)"), &iTransactionName, aEvent.iStatus);
			break;

		case KErrHttpRedirectExceededLimit:			
			Log(_L("(%S)  : KErrHttpRedirectExceededLimit received (Event Status: %d)"), &iTransactionName, aEvent.iStatus);
			break;

		case KErrHttpRedirectNoLocationField:	
			Log(_L("(%S)  : KErrHttpRedirectNoLocationField received (Event Status: %d)"), &iTransactionName, aEvent.iStatus);
			break;

		default:
			Log(_L("(%S)  : unrecognised event> (Event Status: %d)"), &iTransactionName, aEvent.iStatus);
			if (aEvent.iStatus < THTTPEvent::ERedirectedPermanently)
				{
				Log(_L("Test Failed: The event received was not expected and it's not just a warning"));
				}
			break;
	}

	if (receivedError == THTTPEvent::EResponseComplete)
		{
		Log(_L("CCmdTransaction::MHFRunL (%S) receivedError == EResponseComplete"), &iTransactionName);
		}
	else if (receivedError == THTTPEvent::ESucceeded)
		{
		Log(_L("CCmdTransaction::MHFRunL (%S) : receivedError == ESucceeded"), &iTransactionName);
//		Machine()->Domains()->Delete(transNameValueBuf);
		DoneWaitEvents();
		}
	else if (receivedError == THTTPEvent::EFailed)
		{
//		Machine()->Domains()->Delete(transNameValueBuf);
		DoneWaitEvents();
		}
	else 
		{
//			Log(_L("CCmdTransaction::MHFRunL : (%S) iTestFail is TRUE"), &transNameValueBuf);
		DoneWaitEvents();
		}
}

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------

