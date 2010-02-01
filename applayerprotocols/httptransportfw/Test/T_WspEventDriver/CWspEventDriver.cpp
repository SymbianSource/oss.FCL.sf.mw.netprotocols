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


// System includes
#include <http.h>
#include <inetprottextutils.h>
#include <f32file.h>

// Local includes
#include "ceventcallbackfilter.h"
#include "cdriverdatasupplier.h"
#include "t_wspeventdriver/tdriverpanic.h"
#include "t_wspeventdriver/mconsoledumper.h"

// Class signature
#include "t_wspeventdriver/cwspeventdriver.h"


_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");

#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}
#endif // EKA2

EXPORT_C CDriverTrans* CDriverTrans::NewL(RHTTPSession aSession, RStringF aMethod, TUriC8 aUri,
										  MDriverTransObs& aObs, MHTTPTransactionCallback& aCB,
										  const TDesC& aId)
	{
	CDriverTrans* self = new (ELeave) CDriverTrans(aSession, aObs);
	CleanupStack::PushL(self);
	self->ConstructL(aMethod, aUri, aCB, aId);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CDriverTrans::~CDriverTrans()
	{
	iTransaction.Close();
	delete iTransId;
	iObs.Closed(*this);
	}

EXPORT_C RHTTPTransaction CDriverTrans::Trans() const
	{
	return iTransaction;
	}

EXPORT_C void CDriverTrans::AddHeaderL(RStringF aFieldName, THdrValType aFieldType, RString aFieldValue)
	{
	THTTPHdrVal fieldValue;
	ConvertHeaderValueL(aFieldValue, aFieldType, fieldValue);
	RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
	hdr.SetFieldL(aFieldName, fieldValue);
	if (aFieldType == CDriverTrans::ERStringF)
		fieldValue.StrF().Close();
	}

EXPORT_C void CDriverTrans::AddHeaderWithParamL(RStringF aFieldName, THdrValType aFieldType, RString aFieldValue,
											    RStringF aParamName, THdrValType aParamType, RString aParamValue)
	{
	THTTPHdrVal fieldValue;
	ConvertHeaderValueL(aFieldValue, aFieldType, fieldValue);
	THTTPHdrVal paramValue;
	ConvertHeaderValueL(aParamValue, aParamType, paramValue);
	RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
	hdr.SetFieldL(aFieldName, fieldValue, aParamName, paramValue);
	if (aFieldType == CDriverTrans::ERStringF)
		fieldValue.StrF().Close();
	if (aParamType == CDriverTrans::ERStringF)
		paramValue.StrF().Close();
	}

EXPORT_C void CDriverTrans::AddBody(MHTTPDataSupplier& aBody)
	{
	iTransaction.Request().SetBody(aBody);
	}

EXPORT_C TBool CDriverTrans::GetHeaderL(RStringF aFieldName, THdrValType& aFieldType, RString& aFieldValue, TInt aPart)
	{
	RHTTPHeaders headers = iTransaction.Response().GetHeaderCollection();

	RStringPool strP = aFieldName.Pool();

	THTTPHdrVal value;
	TBool hasHeader = headers.GetField(aFieldName, aPart, value) != KErrNotFound;

	// Set the return values, provided the header was present
	if (hasHeader)
		{
		switch( value.Type() )
			{
		case THTTPHdrVal::KTIntVal:
			{
			aFieldType = ETInt;

			HBufC8* buf = NULL;
			InetProtTextUtils::ConvertIntToDescriptorL(value.Int(), buf);
			CleanupStack::PushL(buf);

			aFieldValue = strP.OpenStringL(*buf);
			CleanupStack::PopAndDestroy(buf);
			} break;
		case THTTPHdrVal::KStrVal:
			{
			// Value is a case-sensitive string
			aFieldType = ERString;
			
			aFieldValue = value.Str().Copy();
			} break;
		case THTTPHdrVal::KStrFVal:
			{
			// Value is a case-insensitive string
			aFieldType = ERStringF;

			aFieldValue = strP.OpenStringL(value.StrF().DesC());
			} break;
		case THTTPHdrVal::KDateVal:
		case THTTPHdrVal::KNoType:
		default:
			// No value has yet been set
			TDriverPanic::Panic(TDriverPanic::EUnsupportedHdrValueType);
			}
		}
	return hasHeader;
	}

EXPORT_C MHTTPDataSupplier& CDriverTrans::GetBody()
	{
	return *iTransaction.Response().Body();
	}

EXPORT_C TInt CDriverTrans::GetStatusCode()
	{
	return iTransaction.Response().StatusCode();
	}


EXPORT_C void CDriverTrans::SubmitL()
	{
	iTransaction.SubmitL();
	}

EXPORT_C void CDriverTrans::Cancel()
	{
	iTransaction.Cancel();
	}

EXPORT_C void CDriverTrans::Close()
	{
	delete this;
	}

EXPORT_C const TDesC& CDriverTrans::TransId() const
	{
	return *iTransId;
	}

EXPORT_C void CDriverTrans::MoreRequestBodyDataL()
	{
	iTransaction.NotifyNewRequestBodyPartL();
	}

EXPORT_C void CDriverTrans::DumpResponseHeadersL(MConsoleDumper& aConsole)
	{
	aConsole.StartDump();

	// Dump the status code
	TBuf<KMaxFileName> dumpBuffer;
	dumpBuffer.Format(_L("HTTP Status: %d\n\n"), GetStatusCode());
	aConsole.WriteData(dumpBuffer);

	// Dump the headers
	RHTTPResponse resp = iTransaction.Response();
	RStringPool strP = iTransaction.Session().StringPool();
	RHTTPHeaders hdr = resp.GetHeaderCollection();
	DumpHeaderCollectionL(strP, hdr, dumpBuffer, aConsole);
	
	aConsole.EndDump();
	}

void CDriverTrans::DumpHeaderCollectionL(RStringPool aStrP, RHTTPHeaders aHdr, TDes& aWorkingBuffer, MConsoleDumper& aDumper)
	{
	THTTPHdrFieldIter it = aHdr.Fields();

	TBuf<32> fieldName16;
	TBuf<KMaxFileName> fieldVal16;
	while (it.AtEnd() == EFalse)
		{
		aWorkingBuffer.Zero();
		RStringTokenF fieldName = it();
		RStringF fieldNameStr = aStrP.StringF(fieldName);
		THTTPHdrVal fieldVal;
		if (aHdr.GetField(fieldNameStr,0,fieldVal) == KErrNone)
			{
			fieldName16.Copy(fieldNameStr.DesC());
			switch (fieldVal.Type())
				{
				case THTTPHdrVal::KTIntVal:
					{
					aWorkingBuffer.Format(_L("%S: %d\n"), &fieldName16, fieldVal.Int());
					} break;
				case THTTPHdrVal::KStrFVal:
					{
					RStringF fieldValStr = aStrP.StringF(fieldVal.StrF());
					fieldVal16.Copy(fieldValStr.DesC());
					aWorkingBuffer.Format(_L("%S: %S\n"), &fieldName16, &fieldVal16);
					} break;
				case THTTPHdrVal::KStrVal:
					{
					RString fieldValStr = aStrP.String(fieldVal.Str());
					fieldVal16.Copy(fieldValStr.DesC());
					aWorkingBuffer.Format(_L("%S: %S\n"), &fieldName16, &fieldVal16);
					} break;
				case THTTPHdrVal::KDateVal:
					{
					TDateTime date = fieldVal.DateTime();
					TBuf<40> dateTimeString;
					TTime t(date);
					t.FormatL(dateTimeString,KDateFormat);
					aWorkingBuffer.Format(_L("%S: %S\n"), &fieldName16, &dateTimeString);
					} break;
				default:
					{
					aWorkingBuffer.Format(_L("%S: <unrecognised value type>\n"), &fieldName16);
					}
				}
			aDumper.WriteData(aWorkingBuffer);		
			}
		++it;
		}
	}

EXPORT_C void CDriverTrans::DumpResponseBody(MConsoleDumper& aConsole)
	{
	aConsole.StartDump();
	if (iTransaction.Response().HasBody())
		{
		MHTTPDataSupplier* body = iTransaction.Response().Body();
		TBuf<KMaxFileName> dumpBuffer;
		dumpBuffer.Format(_L("Overall Data Size: %d\n\n"), body->OverallDataSize());
		aConsole.WriteData(dumpBuffer);
		TPtrC8 data;
		TBool moreData = body->GetNextDataPart(data);
		aConsole.DumpData(data);
		dumpBuffer.Format(_L("More data to come: %d\n"), moreData);
		aConsole.WriteData(dumpBuffer);
		body->ReleaseData();
		}
	else
		aConsole.WriteData(_L("No response body"));
	aConsole.EndDump();
	}

CDriverTrans::CDriverTrans(RHTTPSession aSession, MDriverTransObs& aObs)
	: CBase(), iSession(aSession), iObs(aObs)
	{
	}

void CDriverTrans::ConstructL(RStringF aMethod, TUriC8 aUri, MHTTPTransactionCallback& aCB, const TDesC& aId)
	{
	iTransaction = iSession.OpenTransactionL(aUri, aCB, aMethod);
	iTransId = aId.AllocL();
	}

void CDriverTrans::ConvertHeaderValueL(RString aInValue, THdrValType aType, THTTPHdrVal& aOutValue)
	{
	RStringPool strP = aInValue.Pool();
	switch (aType)
		{
	case ETInt:
		{
		TInt val = 0;
		TLex8 lexer(aInValue.DesC());
		TInt err = lexer.Val(val);
		__ASSERT_ALWAYS(err == KErrNone, TDriverPanic::Panic(TDriverPanic::EInvalidInteger));
		aOutValue = val;
		} break;
	case ERString:
		{
		aOutValue = aInValue;
		} break;
	case ERStringF:
		{
		RStringF strf = strP.OpenFStringL(aInValue.DesC());
		aOutValue = strf;
		strf.Copy();
		strf.Close();
		} break;
	case ETDateTime:
		{
		HBufC16* dateBuf = HBufC16::NewL(aInValue.DesC().Length());
		CleanupStack::PushL(dateBuf);
		dateBuf->Des().Copy(aInValue.DesC());
		TTime time;
		TInt err = time.Set(*dateBuf);
		__ASSERT_ALWAYS(err == KErrNone, TDriverPanic::Panic(TDriverPanic::ETimeDescriptor));
		CleanupStack::PopAndDestroy(dateBuf);
		aOutValue = time.DateTime();
		} break;
		};
	}


EXPORT_C CWspEventDriver* CWspEventDriver::NewL(CEventCallbackFilter& aCallbackFilter,
												const TDesC& aTrHndIniFile)
	{
	CWspEventDriver* self = new (ELeave) CWspEventDriver(aCallbackFilter);
	CleanupStack::PushL(self);
	self->ConstructL(aTrHndIniFile);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CWspEventDriver::~CWspEventDriver()
	{
	iTransactionList.ResetAndDestroy();
	iSession.Close();
	}

EXPORT_C RStringPool CWspEventDriver::StringPool()
	{
	return iStrP;
	}

EXPORT_C TInt CWspEventDriver::Reset()
	{
	// Remove all the transactions
	iTransactionList.ResetAndDestroy();

	// Clean-out the session properties
	iSession.ConnectionInfo().RemoveAllProperties();

	// Cleanup the session headers
	RHTTPHeaders requestHeaders;
	TRAPD(error, requestHeaders = iSession.RequestSessionHeadersL() );
	if( error != KErrNone )
		return error;
	
	// Reset the request headers
	requestHeaders.RemoveAllFields();

	RHTTPHeaders responseHeaders;
	TRAP(error, responseHeaders = iSession.ResponseSessionHeadersL() );
	if( error != KErrNone )
		return error;
	
	// Reset the request headers
	responseHeaders.RemoveAllFields();

	// Reset the protocol handler
	TRAP(error, iSession.SendSessionEventL(THTTPSessionEvent(-999), THTTPSessionEvent::EOutgoing) );
	if( error != KErrNone )
		return error;

	CActiveScheduler::Start();
	return KErrNone;
	}


//
// Scriptable methods
//

EXPORT_C void CWspEventDriver::SessionConnectL()
	{
	// Send session event EConnect
	iSession.SendSessionEventL(THTTPSessionEvent::EConnect, THTTPSessionEvent::EOutgoing);
	}

EXPORT_C void CWspEventDriver::SessionDisconnectL()
	{
	// Send session event EDisconnect
	iSession.SendSessionEventL(THTTPSessionEvent::EDisconnect, THTTPSessionEvent::EOutgoing);
	}

EXPORT_C void CWspEventDriver::SetClientMessageSizeL(TUint32 aSize)
	{
	// Set the property
	RHTTPConnectionInfo sessProps = iSession.ConnectionInfo();
	sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspCapClientMessageSize, RHTTPSession::GetTable()), THTTPHdrVal(aSize));
	}

EXPORT_C void CWspEventDriver::SetServerMessageSizeL(TUint32 aSize)
	{
	// Set the property
	RHTTPConnectionInfo sessProps = iSession.ConnectionInfo();
	sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspCapServerMessageSize, RHTTPSession::GetTable()), THTTPHdrVal(aSize));
	}

EXPORT_C void CWspEventDriver::SetUseAcknowledgementsL()
	{
	// Set the property
	RHTTPConnectionInfo sessProps = iSession.ConnectionInfo();
	sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspCapUseAcknowledgements, RHTTPSession::GetTable()), THTTPHdrVal(ETrue));
	}

EXPORT_C void CWspEventDriver::SetUseSuspendResumeL()
	{
	// Set the property
	RHTTPConnectionInfo sessProps = iSession.ConnectionInfo();
	sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspCapSupportSuspendResume, RHTTPSession::GetTable()), THTTPHdrVal(ETrue));
	}

EXPORT_C void CWspEventDriver::SetMethodMORL(TUint32 aMOR)
	{
	// Set the property
	RHTTPConnectionInfo sessProps = iSession.ConnectionInfo();
	sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspCapMaxOutstandingRequests, RHTTPSession::GetTable()), THTTPHdrVal(aMOR));
	}

EXPORT_C void CWspEventDriver::SetConnectionPropertiesL(RStringF aProxyAddress, RStringF aBearer, TInt aLocalPort,
														TInt aRemotePort, TInt aServiceNum, RStringF aSessionType,
														RStringF aIsSecure, TInt aTimeOut)
	{
	RHTTPConnectionInfo sessProps = iSession.ConnectionInfo();
	if (aProxyAddress != RStringF())
		sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspProxyAddress, RHTTPSession::GetTable()), THTTPHdrVal(aProxyAddress));
	//	
	if (aBearer != RStringF())
		sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspBearer, RHTTPSession::GetTable()), THTTPHdrVal(aBearer));
	//
	if (aLocalPort >= 0)
		sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspLocalPort, RHTTPSession::GetTable()), THTTPHdrVal(aLocalPort));
	//
	if (aRemotePort >= 0)
		sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspRemotePort, RHTTPSession::GetTable()), THTTPHdrVal(aRemotePort));
	//
	if (aServiceNum >= 0)
		sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspServiceNumber, RHTTPSession::GetTable()), THTTPHdrVal(aServiceNum));
	//
	if (aSessionType != RStringF())
		sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspSessionType, RHTTPSession::GetTable()), THTTPHdrVal(aSessionType));
	//
	if (aIsSecure != RStringF())
		sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspSecurity, RHTTPSession::GetTable()), THTTPHdrVal(aIsSecure));
	//
	if (aTimeOut >= 0)
		sessProps.SetPropertyL(iStrP.StringF(HTTP::EWspProxyConnectionTimeout, RHTTPSession::GetTable()), THTTPHdrVal(aTimeOut));
	}

EXPORT_C void CWspEventDriver::GetConnectionProperties(RStringF& aProxyAddress, RStringF& aBearer, TInt& aLocalPort,
									   TInt& aRemotePort, TInt& aServiceNum, RStringF& aSessionType,
									   RStringF& aIsSecure, TInt& aTimeOut)
	{
	RHTTPConnectionInfo sessProps = iSession.ConnectionInfo();
	THTTPHdrVal propVal;
	//
	if (sessProps.Property(iStrP.StringF(HTTP::EWspProxyAddress, RHTTPSession::GetTable()), propVal))
		{
		__ASSERT_DEBUG(propVal.Type() == THTTPHdrVal::KStrFVal, TDriverPanic::Panic(TDriverPanic::EWrongPropertyType));
		aProxyAddress = propVal;
		}
	else
		aProxyAddress = RStringF();
	//	
	if (sessProps.Property(iStrP.StringF(HTTP::EWspBearer, RHTTPSession::GetTable()), propVal))
		{
		__ASSERT_DEBUG(propVal.Type() == THTTPHdrVal::KStrFVal, TDriverPanic::Panic(TDriverPanic::EWrongPropertyType));
		aBearer = propVal;
		}
	else
		aBearer = RStringF();
	//
	if (sessProps.Property(iStrP.StringF(HTTP::EWspLocalPort, RHTTPSession::GetTable()), propVal))
		{
		__ASSERT_DEBUG(propVal.Type() == THTTPHdrVal::KTIntVal, TDriverPanic::Panic(TDriverPanic::EWrongPropertyType));
		aLocalPort = propVal;
		}
	else
		aLocalPort = -1;
	//
	if (sessProps.Property(iStrP.StringF(HTTP::EWspRemotePort, RHTTPSession::GetTable()), propVal))
		{
		__ASSERT_DEBUG(propVal.Type() == THTTPHdrVal::KTIntVal, TDriverPanic::Panic(TDriverPanic::EWrongPropertyType));
		aRemotePort = propVal;
		}
	else
		aRemotePort = -1;
	//
	if (sessProps.Property(iStrP.StringF(HTTP::EWspServiceNumber, RHTTPSession::GetTable()), propVal))
		{
		__ASSERT_DEBUG(propVal.Type() == THTTPHdrVal::KTIntVal, TDriverPanic::Panic(TDriverPanic::EWrongPropertyType));
		aServiceNum = propVal;
		}
	else
		aServiceNum = -1;
	//
	if (sessProps.Property(iStrP.StringF(HTTP::EWspSessionType, RHTTPSession::GetTable()), propVal))
		{
		__ASSERT_DEBUG(propVal.Type() == THTTPHdrVal::KStrFVal, TDriverPanic::Panic(TDriverPanic::EWrongPropertyType));
		aSessionType = propVal;
		}
	else
		aSessionType = RStringF();
	//
	if (sessProps.Property(iStrP.StringF(HTTP::EWspSecurity, RHTTPSession::GetTable()), propVal))
		{
		__ASSERT_DEBUG(propVal.Type() == THTTPHdrVal::KStrFVal, TDriverPanic::Panic(TDriverPanic::EWrongPropertyType));
		aIsSecure = propVal;
		}
	else
		aIsSecure = RStringF();
	//
	if (sessProps.Property(iStrP.StringF(HTTP::EWspProxyConnectionTimeout, RHTTPSession::GetTable()), propVal))
		{
		__ASSERT_DEBUG(propVal.Type() == THTTPHdrVal::KTIntVal, TDriverPanic::Panic(TDriverPanic::EWrongPropertyType));
		aTimeOut = propVal;
		}
	else
		aTimeOut = -1;
	}



EXPORT_C CDriverTrans* CWspEventDriver::OpenTransactionL(RStringF aMethod, TUriC8 aUri, const TDesC& aId)
	{
	// Create the driver transaction and place on the list
	CDriverTrans* tr = CDriverTrans::NewL(iSession, aMethod, aUri, *this, iCallbackFilter.TransactionCallback(), aId);
	CleanupStack::PushL(tr);
	User::LeaveIfError(iTransactionList.Append(tr));
	CleanupStack::Pop(tr);
	return tr;
	}

EXPORT_C void CWspEventDriver::AddRequestSessionHeaderL(RStringF aFieldName, CDriverTrans::THdrValType aFieldType, RString aFieldValue)
	{
	THTTPHdrVal fieldValue;
	CDriverTrans::ConvertHeaderValueL(aFieldValue, aFieldType, fieldValue);
	RHTTPHeaders hdr = iSession.RequestSessionHeadersL();
	hdr.SetFieldL(aFieldName, fieldValue);
	if (aFieldType == CDriverTrans::ERStringF)
		fieldValue.StrF().Close();
	}

EXPORT_C void CWspEventDriver::AddRequestSessionHeaderWithParamL(RStringF aFieldName, CDriverTrans::THdrValType aFieldType,
																 RString aFieldValue, RStringF aParamName,
																 CDriverTrans::THdrValType aParamType, RString aParamValue)
	{
	THTTPHdrVal fieldValue;
	CDriverTrans::ConvertHeaderValueL(aFieldValue, aFieldType, fieldValue);
	THTTPHdrVal paramValue;
	CDriverTrans::ConvertHeaderValueL(aParamValue, aParamType, paramValue);
	RHTTPHeaders hdr = iSession.RequestSessionHeadersL();
	hdr.SetFieldL(aFieldName, fieldValue, aParamName, paramValue);
	if (aFieldType == CDriverTrans::ERStringF)
		fieldValue.StrF().Close();
	if (aParamType == CDriverTrans::ERStringF)
		paramValue.StrF().Close();
	}

EXPORT_C void CWspEventDriver::DumpSessionHeadersL(MConsoleDumper& aConsole)
	{
	aConsole.StartDump();

	// Dump the headers
	TBuf<KMaxFileName> dumpBuffer;
	RStringPool strP = iSession.StringPool();

	// Request session headers
	aConsole.WriteData(_L("Session Request Headers:\n"));
	RHTTPHeaders hdr = iSession.RequestSessionHeadersL();
	CDriverTrans::DumpHeaderCollectionL(strP, hdr, dumpBuffer, aConsole);

	// Response session headers
	aConsole.WriteData(_L("Session Response Headers:\n"));
	hdr = iSession.ResponseSessionHeadersL();
	CDriverTrans::DumpHeaderCollectionL(strP, hdr, dumpBuffer, aConsole);

	aConsole.EndDump();
	}

void CWspEventDriver::Closed(CDriverTrans& aDrTr)
	{
	// Locate the trans on the list and remove it (it's already been deleted)
	TBool found = EFalse;
	TInt idx = 0;
	while (!found && (idx < iTransactionList.Count()))
		{
		if (&aDrTr == iTransactionList[idx])
			{
			found = ETrue;
			iTransactionList.Remove(idx);
			}
		else
			++idx;
		}
	}

CDriverTrans& CWspEventDriver::LookUpTransaction(RHTTPTransaction aTr)
	{
	// Locate the trans on the list and remove it (it's already been deleted)
	TBool found = EFalse;
	TInt idx = 0;
	while (!found && (idx < iTransactionList.Count()))
		{
		if (iTransactionList[idx]->Trans() == aTr)
			found = ETrue;
		else
			++idx;
		}
	__ASSERT_DEBUG(found, User::Invariant());
	return *(iTransactionList[idx]);
	}

CWspEventDriver::CWspEventDriver(CEventCallbackFilter& aCallbackFilter)
: CBase(), iCallbackFilter(aCallbackFilter)
	{
	aCallbackFilter.SetTransConverter(*this);
	}

void CWspEventDriver::ConstructL(const TDesC& aTrHndIniFile)
	{
	// Write the location of the ini file for the stub to pick up.  The filename of the actual
	// ini file to be used is written as the contents of the link file.
	_LIT(KTrHndIniLinkFile, "wsptrhndstub_link.ini");
	_LIT(KTrHndIniLinkDir,  "C:\\");
	RFs fileSrvHnd;
	User::LeaveIfError(fileSrvHnd.Connect());
	TParse parsedLinkFile;
	parsedLinkFile.Set(KTrHndIniLinkDir, &(KTrHndIniLinkFile()), NULL);
	RFile linkFileHnd;
	User::LeaveIfError( linkFileHnd.Replace(fileSrvHnd, parsedLinkFile.FullName(),
											EFileWrite|EFileShareExclusive) );
	HBufC8* iniFile8 = HBufC8::NewLC(aTrHndIniFile.Length());
	iniFile8->Des().Copy(aTrHndIniFile);
	User::LeaveIfError( linkFileHnd.Write(iniFile8->Des()) );
	linkFileHnd.Close();
	CleanupStack::PopAndDestroy(iniFile8);

	// Open the session and get the string pool reference from it
	iSession.OpenL(_L8("WSP/WSP"));
	iStrP = iSession.StringPool();

	// Set the session callback
	iSession.SetSessionEventCallback(&iCallbackFilter.SessionCallback());

	// Ensure the request and response session headers are created
	(void)iSession.RequestSessionHeadersL();
	(void)iSession.ResponseSessionHeadersL();
	}

