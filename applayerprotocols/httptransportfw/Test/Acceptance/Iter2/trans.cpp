// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <f32file.h>
#include <x509cert.h>

#include "trans.h"
#include <http.h>

//For Proxy Info.
#include <cdblen.h>
#include <cdbcols.h>
#include <commsdat.h>
#include <comms-infras/dbaccess.h>
#include <metadatabase.h>
#include <commsdattypesv1_1.h>
#include <commsdattypeinfov1_1.h>

_LIT(KProxyServerName, 			"10.29.23.12");
_LIT(KProtocolName, 			"http");
const TInt KPortNumber = 9003;


_LIT(KItemName,						"name");

// Proxy server set as either a session property or transaction property
_LIT(KItemProxy,					"proxy");
_LIT(KItemTransactionProxy,			"transactionproxy");  
_LIT(KItemProxyPort,					"proxy_port");
_LIT(KItemTransactionProxyPort,			"proxy_trans_port");  

// Proxy usage at either session or transaction level. Turning off proxy at 
// the session level has no effect or the transaction proxy settings and vice 
// versa
_LIT(KItemProxyUsage,				"proxyUsage");
_LIT(KItemTransactionProxyUsage,	"transactionProxyUsage");
_LIT8(KItemProxyOff,				"ProxyOff");

_LIT(KItemMethod,					"method");
_LIT(KItemUri,						"uri");
_LIT(KItemUrisch,						"uri_scheme"); //new
_LIT(KItemUriport,						"port"); // new
_LIT(KItemUripath,						"path");  //new
_LIT(KItemBody,						"body");
_LIT(KItemBodyFile,					"bodyfile");
_LIT(KItemAccept,					"Accept");
_LIT(KItemAcceptCharset,			"Accept-Charset");
_LIT(KItemAcceptEncoding,			"Accept-Encoding");
_LIT(KItemContentType,				"Content-Type");
_LIT(KItemContentLength,			"Content-Length");
_LIT(KItemExpect,					"Expect");
_LIT(KItemExpectedStatusCode,		"test_validation_param_statuscode");
_LIT(KItemExpectedError,			"test_validation_param_expectederror");
_LIT(KItemTransactionNumber,		"Transaction");
_LIT(KItemHeader,					"header");
_LIT(KUserName,						"username");
_LIT(KPassword,						"password");
_LIT(KFrom,							"from");
_LIT(KItemRedirectCheck,			"test_validation_param_numredirects");

_LIT8(KMethodGet,					"GET");
_LIT8(KUriSymbian,					"www.symbian.com");
_LIT8(KUrisch,						"http://"); //new

_LIT(KItemDefaultProxyUsage,		"defaultProxyUsage");
_LIT8(KItemDefaultProxyOn, 			"On");



const TDesC& CHttpTestTransactions::TestName()
	{
	return iTestName;
	}

CHttpTestTransactions::~CHttpTestTransactions()
	{
	delete iBody;
	}	

void CHttpTestTransactions::CloseTestSession()
	{
	iUsername.Close();
	iPassword.Close();
	delete iBody;
	iBody = NULL;
	}

// Returns back index of the item in the section that is the end of 
// transaction, used to find where transactions are for multiple transactions
TInt CHttpTestTransactions::FindEndOfTransaction(TInt aStartItem)
	{
	TInt count= iIniFile->Section(iSectionNumber).Items().Count();
	TInt ii = aStartItem;
	for (; ii < count; ++ii)
		{
		//Start of a new transaction
		if (iIniFile->Section(iSectionNumber).Item(ii).Item().CompareF(
			KItemTransactionNumber) == 0)
			{
			return ii -1;
			}
		}
	// Got to end of section;
	return ii;
	}

HBufC8* CHttpTestTransactions::FindItemLC(const TDesC& aItem, TInt aStart, TInt aEnd, const TDesC8 aDefault)
	{
	for (TInt ii = aStart; ii< aEnd; ++ii)
		{
		if (iIniFile->Section(iSectionNumber).Item(ii).Item().CompareF(aItem) == 0)
			{
			TPtrC valuePtr(iIniFile->Section(iSectionNumber).Item(ii).Value());
			HBufC8* value8 = HBufC8::NewLC(valuePtr.Length());
			TPtr8  valuePtr8= value8->Des();
			valuePtr8.Copy(valuePtr);
			return value8;
			}
		}
	return aDefault.AllocLC();
	}

RHTTPTransaction CHttpTestTransactions::CreateTransactionL(TInt aStartItem)
	{
	TInt endItem= FindEndOfTransaction(aStartItem);
	
	// Get the method
	HBufC8* method = FindItemLC(KItemMethod,aStartItem,endItem, KMethodGet);
	TPtr8  methodPtr8= method->Des();

	// Get the uri value
	HBufC8* urisch = FindItemLC(KItemUrisch, aStartItem,endItem, KUrisch);
	HBufC8* uriport = FindItemLC(KItemUriport, aStartItem,endItem, KNullDesC8);
	HBufC8* uripath = FindItemLC(KItemUripath, aStartItem,endItem, KNullDesC8);

	HBufC8* uri = FindItemLC(KItemUri, aStartItem,endItem, KUriSymbian);
	TPtr8  uritempPtr8= uri->Des();

	HBufC8* newtempUri8 = TSrvAddrVal::ReplaceHostNameL(uritempPtr8, iIniSettingsFile);
	CleanupStack::PushL(newtempUri8);
	TPtr8 Bufsch(urisch->Des());
	TPtr8 Bufuri=newtempUri8->Des();
	TPtr8 Bufport(uriport->Des());
	TPtr8 Bufpath=uripath->Des();
	TInt len;

	len=Bufsch.Size()+Bufuri.Size()+Bufport.Size()+Bufpath.Size()+1;
	//HBufC8* newUri8;
	HBufC8* newUri8=HBufC8::NewL(len);
	CleanupStack::PushL(newUri8);
	TPtr8 newUriPtr8(newUri8->Des());

	if (uritempPtr8.Find(_L8("http://"))==KErrNotFound) 
	{
		newUriPtr8.Copy(Bufsch);
		newUriPtr8.Append(Bufuri);
		if(Bufport.Size()>0) 
		{
			newUriPtr8.Append(_L8(":"));  //add ":" before the port number
		}
		newUriPtr8.Append(Bufport);
		newUriPtr8.Append(Bufpath);
	}
	else
	{
		newUriPtr8.Copy(uritempPtr8); // Compatible with old uri format

	}
// 
	TUriParser8 testUri;
		testUri.Parse(newUriPtr8);

	//open a  transaction and log it
	RHTTPTransaction transaction = iSession.OpenTransactionL(testUri, *this, iMyStrP.OpenFStringL(*method));
	iOpenTransactionCount++;
	TSrvAddrVal::LogUsing8BitDesL(iEngine, _L8("Transaction Created:\t method "));
	TSrvAddrVal::LogUsing8BitDesL(iEngine, methodPtr8);
	TSrvAddrVal::LogUsing8BitDesL(iEngine, _L8("Transaction Created:\t URI "));
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUriPtr8);
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	//Get a handle of the request and its headers in iTransaction
	RHTTPRequest myRequest = transaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	
	TBool allowTransactionProxySetting = ETrue;
	// check for transaction proxy support first
	HBufC8* proxyUsage = FindItemLC(KItemTransactionProxyUsage,aStartItem, endItem,KNullDesC8);
	if (proxyUsage->Length() > 0 && proxyUsage->CompareF(KItemProxyOff) == 0)
		{	
		RHTTPTransactionPropertySet transactionProperties = transaction.PropertySet();
		RStringPool strP = iSession.StringPool();
		
		THTTPHdrVal proxyUsage(strP.StringF(HTTP::EDoNotUseProxy, RHTTPSession::GetTable()));
		transactionProperties.SetPropertyL(strP.StringF(HTTP::EProxyUsage, RHTTPSession::GetTable()), proxyUsage);
				
		allowTransactionProxySetting = EFalse;
		}
		
	CleanupStack::PopAndDestroy(proxyUsage);


	for (TInt ii = aStartItem; ii<endItem; ++ii)
		{
		TPtrC itemPtr(iIniFile->Section(iSectionNumber).Item(ii).Item());
		HBufC8* itemBuff8 = HBufC8::NewLC(itemPtr.Length());
		TPtr8  itemPtr8= itemBuff8->Des();
		itemPtr8.Copy(itemPtr);
		RStringF item=iMyStrP.OpenFStringL(itemPtr8);
		CleanupClosePushL ( item );

		TPtrC valuePtr(iIniFile->Section(iSectionNumber).Item(ii).Value());
		HBufC8* valueBuff8 = HBufC8::NewLC(valuePtr.Length());
		TPtr8 valuePtr8= valueBuff8->Des();
		valuePtr8.Copy(valuePtr);
		RStringF value =iMyStrP.OpenFStringL(valuePtr8);
		CleanupClosePushL ( value );
	
		if (itemPtr.CompareF(KItemHeader)==0)
			DoHeaderL(myHeaders,*valueBuff8);
		else if (itemPtr.CompareF(KUserName)==0)
			iUsername= iMyStrP.OpenStringL(valuePtr8);
		else if (itemPtr.CompareF(KPassword)==0)
			iPassword= iMyStrP.OpenStringL(valuePtr8);
		else if (itemPtr.CompareF(KItemExpect) == 0 &&
					valuePtr.CompareF(_L("100-continue"))==0)
						myHeaders.SetFieldL(item, value);
		else if (itemPtr.CompareF(KFrom) == 0)
			myHeaders.SetFieldL(item, value);


		else if (itemPtr.CompareF(KItemContentType)==0)
			myHeaders.SetFieldL(item, value);
		else if (itemPtr.CompareF(KItemContentLength)==0)	
			{
			//convert content-length to TInt
			TLex lex(valuePtr);
			TInt length = 0;
			lex.Val(length);
			myHeaders.SetFieldL(item, length);
			}
		else if (itemPtr.CompareF(KItemAccept)==0)
			CreateAcceptStringL(myHeaders);
		else if (itemPtr.CompareF(KItemAcceptCharset)==0)
			CreateAcceptCharsetStringL(myHeaders);
		else if (itemPtr.CompareF(KItemAcceptEncoding)==0)
			CreateAcceptEncodingStringL(myHeaders);
		else if (itemPtr.CompareF(KItemBody)==0)
			{
			iBody= (*valueBuff8).AllocL();
			myRequest.SetBody(*this );
			}
		else if (itemPtr.CompareF(KItemBodyFile)==0)
			{
			LoadBodyFromFileL(valuePtr);
			myRequest.SetBody(*this );
			}
		else if (itemPtr.CompareF(KItemExpectedStatusCode)==0)
			{
			//get the 'test_validation_param_statuscode' from the ini file...
			TPtrC expectedStatusCodePtr = iIniFile->Section(iSectionNumber).ItemL(KItemExpectedStatusCode).Value();
			TLex statusCodeLex(expectedStatusCodePtr);
			TInt statuscode;
			statusCodeLex.Val(statuscode);
			// ...and put it  into iExpectedStatusCode
			iExpectedStatusCode = statuscode;
			}	
		else if (itemPtr.CompareF(KItemExpectedError)==0)
			{
			//get the 'test_validation_param_expectederror' from the 
			//ini file...
			TPtrC expectedParamPtr = iIniFile->Section(iSectionNumber).ItemL(KItemExpectedError).Value();
			TLex paramLex(expectedParamPtr);
			TInt param;
			paramLex.Val(param);
			// ...and put it  into iExpectedError
			iExpectedError = param;
			}
		else if (itemPtr.CompareF(KItemTransactionProxy)==0 && 
			allowTransactionProxySetting)
			// set proxy settings on transaction
			{
			// Get the proxy value
			TPtrC proxyPtr16 = iIniFile->Section(iSectionNumber).ItemL(KItemTransactionProxy).Value();
			// Get the proxy port
			HBufC8* Transactionport = FindItemLC(KItemTransactionProxyPort,aStartItem,endItem, KNullDesC8); // new
			TPtr8  TransactionportPtr8= Transactionport->Des();

			HBufC16* proxyUrl16 = TSrvAddrVal::ReplaceHostNameL(proxyPtr16, iIniSettingsFile);
			CleanupStack::PushL(proxyUrl16);
			TPtr16 proxyUrlPtr16 = proxyUrl16->Des();
			if (TransactionportPtr8.Length())
			{			
				len=proxyUrlPtr16.Length() + TransactionportPtr8.Length() +1;
			}
			else
				len=proxyUrlPtr16.Length();

			// Converting proxyUrl16 to 8 bit
			HBufC8* proxyUrl8 = HBufC8::NewLC(len);
			TPtr8 proxyUrlPtr8 = proxyUrl8->Des();
			proxyUrlPtr8.Copy(proxyUrlPtr16);
			if (TransactionportPtr8.Length())
			{
				proxyUrlPtr8.Append(_L(":"));
				proxyUrlPtr8.Append(TransactionportPtr8);
			}

			if (proxyPtr16.Length() > 0)
				{
				RHTTPTransactionPropertySet transactionProperties = transaction.PropertySet();
				RStringPool strP = iSession.StringPool();
				RStringF proxyAddr = strP.OpenFStringL(proxyUrlPtr8);
				CleanupClosePushL(proxyAddr);
				THTTPHdrVal proxyUsage(strP.StringF(HTTP::EUseProxy, RHTTPSession::GetTable()));
				transactionProperties.SetPropertyL(strP.StringF(HTTP::EProxyUsage,RHTTPSession::GetTable()), proxyUsage);
				transactionProperties.SetPropertyL(strP.StringF(HTTP::EProxyAddress,RHTTPSession::GetTable()), proxyAddr);
				CleanupStack::PopAndDestroy(1,&proxyAddr);
				}
			CleanupStack::PopAndDestroy(3, Transactionport);// and proxyUrl16, proxyUrl8
			}
	
		CleanupStack::PopAndDestroy ( 2 ); // item, value
		CleanupStack::PopAndDestroy(2);// valueBuff8, itemBuff8
		}
	
	CleanupStack::PopAndDestroy(7, method); // Urisch,Uriport, Utipath, uri, newtempUri8, newUri




	return transaction;
	}

void CHttpTestTransactions::ConstructL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, const TInt aSectionNumber)
	{
	iUsernameUsed = EFalse;
	iIniFile =&aIniFile;
	iIniSettingsFile = aIniSettingsFile;
	iSectionNumber=aSectionNumber;
	//get the 'Name' section item 
	iTestName.Append(iIniFile->Section(iSectionNumber).ItemL(KItemName).Value());

	_LIT(KLogCert, "LogCert");
	if(aIniFile.Section(aSectionNumber).Item(KLogCert) != NULL)
		iLogCert = ETrue;
	}

TBool CHttpTestTransactions::GetNextDataPart(TPtrC8& aDataChunk)
	{
	//add a body 
	aDataChunk.Set(*iBody);
	return ETrue;
	}

void CHttpTestTransactions::ReleaseData() 
	{
	}
	
TInt CHttpTestTransactions::OverallDataSize()
	{
	return iBody->Length();
	}

// aHeader will be in the format field:value
void CHttpTestTransactions::DoHeaderL(RHTTPHeaders aRequestHeaders, const TDesC8& aHeader)
	{
	// get the field
	TInt colonPos = aHeader.Locate(':');
	if (colonPos == KErrNotFound)
		return;

	RStringF field = iMyStrP.OpenFStringL(aHeader.Left(colonPos));
	CleanupClosePushL(field);
	TPtrC8 value8 = aHeader.Mid(colonPos+1, aHeader.Length() - (colonPos+1));

	RStringF value = iMyStrP.OpenFStringL(value8);
	CleanupClosePushL(value);

	aRequestHeaders.SetFieldL(field,value);
	CleanupStack::PopAndDestroy(2, &field); // and value
	}


void CHttpTestTransactions::CreateAcceptStringL(RHTTPHeaders aRequestHeaders)
	{
	// parse the accept header
	TLex input(iIniFile->Section(iSectionNumber).ItemL(KItemAccept).Value());
	TPtrC accept;

	// go through all the accept values and add to the header
	// 
	while (CScriptFile::GetNextWord(input, ',', accept) == KErrNone)
		{
		// There may be parameters that will have to be store along with the 
		// accept value Accept: text/plain, text/html; q=0.8, 
		TPtrC paramPtr(accept);
		if (paramPtr.Locate(';') != KErrNotFound)
			{
			// extra parameter exists and this needs to be stored
			TLex param(paramPtr);
			
			// skip any leading whitespace
			param.SkipSpaceAndMark();

			// anything upto the semicolon (;) is the normal value
			while (param.Peek() != ';')
				param.Inc();
			TPtrC acceptVal = param.MarkedToken();

			// skip over the semicolon (;)
			param.Inc();

			// skip any leading whitespace
			param.SkipSpaceAndMark();

			// anything upto the equal sign (=) is the parameter name
			while (param.Peek() != '=')
				param.Inc();
			param.MarkedToken();  // qParam should be  'q'
			// skip over the equals sign (=)
			param.Inc();

			// skip any leading whitespace
			param.SkipSpaceAndMark();
			TPtrC qParamVal=param.Remainder(); // qParamVal is the value of q

			// store each value in the header class
			HBufC8*  acceptValBuff8= HBufC8::NewLC(acceptVal.Length());
			TPtr8 acceptVal8= acceptValBuff8->Des();
			acceptVal8.Copy(acceptVal);
			RStringF acceptStr= iMyStrP.OpenFStringL(acceptVal8);
			CleanupClosePushL(acceptStr);
			THTTPHdrVal acceptHdr(acceptStr);
			// store each Q-value in the header class
			HBufC8*  qParamValBuff8= HBufC8::NewLC(qParamVal.Length());
			TPtr8 qParamVal8= qParamValBuff8->Des();
			qParamVal8.Copy(qParamVal);
			TLex8 str(qParamVal8);
			TReal qReal=0;
			str.Val(qReal);
			THTTPHdrVal::TQConv qVal(qReal);
			THTTPHdrVal qHdr(qVal);
			aRequestHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()), acceptHdr, iMyStrP.OpenFStringL(_L8("q")), qHdr);

			CleanupStack::PopAndDestroy(3, acceptValBuff8); // and acceptStr, qParamValBuff8
			}
		else
			{
			TPtrC acceptVal = accept;

			// store each value in the header class
			TBuf8<1000> acceptVal8;
			acceptVal8.Copy(acceptVal);

			RStringF accept= iMyStrP.OpenFStringL(acceptVal8);
			CleanupClosePushL ( accept );
			THTTPHdrVal acceptHdr(accept);
			aRequestHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()),acceptHdr);
			CleanupStack::PopAndDestroy ( &accept );
			}
		}
	}

void CHttpTestTransactions::CreateAcceptCharsetStringL(RHTTPHeaders aRequestHeaders)
	{
	// parse the accept header
	TLex input(iIniFile->Section(iSectionNumber).ItemL(KItemAcceptCharset).Value());
	TPtrC accept;
	
	// go through all the accept values and add to the header
	// 
	while (CScriptFile::GetNextWord(input, ',', accept) == KErrNone)
		{
		// There may be parameters that will have to be store along with the 
		// accept value Accept: text/plain, text/html; q=0.8, 
		TPtrC paramPtr(accept);
		if (paramPtr.Locate(';') != KErrNotFound)
			{
			// extra parameter exists and this needs to be stored
			TLex param(paramPtr);
			
			// skip any leading whitespace
			param.SkipSpaceAndMark();

			// anything upto the semicolon (;) is the normal value
			while (param.Peek() != ';')
				param.Inc();
			TPtrC acceptVal = param.MarkedToken();

			// skip over the semicolon (;)
			param.Inc();

			// skip any leading whitespace
			param.SkipSpaceAndMark();

			// anything upto the equal sign (=) is the parameter name
			while (param.Peek() != '=')
				param.Inc();
			param.MarkedToken();

			// skip over the equals sign (=)
			param.Inc();

			// skip any leading whitespace
			param.SkipSpaceAndMark();
			param.Remainder();

			// store each value in the header class
			HBufC8*  acceptValBuff8= HBufC8::NewLC(acceptVal.Length());
			TPtr8 acceptVal8= acceptValBuff8->Des();
			acceptVal8.Copy(acceptVal);

			RStringF acceptStr= iMyStrP.OpenFStringL(acceptVal8);
			CleanupClosePushL(acceptStr);
			THTTPHdrVal acceptHdr(acceptStr);
			
			aRequestHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAcceptCharset, RHTTPSession::GetTable()),acceptHdr);
			CleanupStack::PopAndDestroy(2, acceptValBuff8); // and acceptStr
			}
		else
			{
			TPtrC acceptVal = accept;

			// store each value in the header class
			TBuf8<1000> acceptVal8;
			acceptVal8.Copy(acceptVal);
			RStringF accept= iMyStrP.OpenFStringL(acceptVal8);
			CleanupClosePushL ( accept );
			THTTPHdrVal acceptHdr(accept);
			aRequestHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAcceptCharset, RHTTPSession::GetTable()),acceptHdr);
			CleanupStack::PopAndDestroy ( &accept );
			}
		}
	}


void CHttpTestTransactions::CreateAcceptEncodingStringL(RHTTPHeaders aRequestHeaders)
	{
	// parse the accept header
	TLex input(iIniFile->Section(iSectionNumber).ItemL(KItemAcceptEncoding).Value());
	TPtrC accept;
	
	// go through all the accept values and add to the header
	// 
	while (CScriptFile::GetNextWord(input, ',', accept) == KErrNone)
		{
		// There may be parameters that will have to be store along with the 
		// accept value Accept: text/plain, text/html; q=0.8, 
		TPtrC paramPtr(accept);
		if (paramPtr.Locate(';') != KErrNotFound)
			{
			// extra parameter exists and this needs to be stored
			TLex param(paramPtr);
			
			// skip any leading whitespace
			param.SkipSpaceAndMark();

			// anything upto the semicolon (;) is the normal value
			while (param.Peek() != ';')
				param.Inc();
			TPtrC acceptVal = param.MarkedToken();

			// skip over the semicolon (;)
			param.Inc();

			// skip any leading whitespace
			param.SkipSpaceAndMark();

			// anything upto the equal sign (=) is the parameter name
			while (param.Peek() != '=')
				param.Inc();
			param.MarkedToken();

			// skip over the equals sign (=)
			param.Inc();

			// skip any leading whitespace
			param.SkipSpaceAndMark();
			param.Remainder();

			// store each value in the header class
			HBufC8*  acceptValBuff8= HBufC8::NewLC(acceptVal.Length());
			TPtr8 acceptVal8= acceptValBuff8->Des();
			acceptVal8.Copy(acceptVal);

			RStringF acceptStr= iMyStrP.OpenFStringL(acceptVal8);
			CleanupClosePushL(acceptStr);
			THTTPHdrVal acceptHdr(acceptStr);
			
			aRequestHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAcceptEncoding, RHTTPSession::GetTable()),acceptHdr);
			CleanupStack::PopAndDestroy(2, acceptValBuff8); // and acceptStr
			}
		else
			{
			TPtrC acceptVal = accept;

			// store each value in the header class
			TBuf8<1000> acceptVal8;
			acceptVal8.Copy(acceptVal);
			RStringF accept= iMyStrP.OpenFStringL(acceptVal8);
			CleanupClosePushL ( accept );
			THTTPHdrVal acceptHdr(accept);
			aRequestHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAcceptEncoding, RHTTPSession::GetTable()),acceptHdr);
			CleanupStack::PopAndDestroy ( &accept );
			}
		}
	}


TBool CHttpTestTransactions::GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword)
	{
	if (!iUsernameUsed && !(((RStringTokenBase) iUsername).IsNull()) && !(((RStringTokenBase) iPassword).IsNull()) )
		{
		iEngine->Utils().LogIt(_L("Supplying credentials"));
		iEngine->Utils().DumpData(aURI.UriDes());
		iEngine->Utils().DumpData(aRealm.DesC());	
		iEngine->Utils().DumpData(aAuthenticationType.DesC());
		aUsername= iUsername.Copy();
		aPassword= iPassword.Copy();
		iUsernameUsed = ETrue;
		return ETrue;
		}
	else
		return EFalse;
	}


TReal CHttpTestTransactions::StrToReal(TPtr8 aStrToConvToReal)
	{
	// this function parses a real number of the type r = a.b
	// and  creates the correspondent real number 'result'
	TLex8 str(aStrToConvToReal);
	TReal result=0;

	// A is the part of the string before the point
	str.Mark();
	while (str.Peek() != '.')
		{
		if(str.Peek() > 48 ||  str.Peek() < 57 )
			{
			str.Val(result) ;
			str.Inc();
			}
		}
	return result;
	}

void CHttpTestTransactions::SetProxyL(TInt aStartItem)
	{
	TInt endItem= FindEndOfTransaction(aStartItem);
	
	// Check to see if proxies have been disabled at the session level
	HBufC8* proxyUsage = FindItemLC(KItemProxyUsage,aStartItem,endItem, KNullDesC8);
	if (proxyUsage->Length() > 0 && proxyUsage->CompareF(KItemProxyOff) == 0)
		{
		CleanupStack::PopAndDestroy(1, proxyUsage);
		return; // don't use a proxy so kip setting it
		}
	
		// Check to see if proxy type was default/defined for the session.
	HBufC8* proxyType = FindItemLC(KItemDefaultProxyUsage, aStartItem, endItem, KNullDesC8);
 if(proxyType->Length() > 0)
		{
		if (proxyType->CompareF(KItemDefaultProxyOn) == 0)
			{//Use default Proxy.
			CleanupStack::PopAndDestroy(2, proxyUsage);//and proxyType
				
			iEngine->Utils().LogIt(_L("Create Default Proxy in DB and Set it"));
			//Delete Proxy if found in Ced file. 
			TRAPD(cedErr, ConfigureDefaultProxyL(EDelete));
			//Create Default Proxy in CommsDat.
			TRAP(cedErr, ConfigureDefaultProxyL(ECreate));			
			#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
			iSession.SetupDefaultProxyFromCommsDatL();
			#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
			//Delete the Default Proxy stored in CommsDat.
			TRAP(cedErr, ConfigureDefaultProxyL(EDelete));
			}
		else
			{
			CleanupStack::PopAndDestroy(2, proxyUsage);//and proxyType
			//Donot make Use of any type of Proxy.
			}
		}
	else
		{	

		// Get the proxy if there is one and set it on the session
		HBufC8* proxy = FindItemLC(KItemProxy,aStartItem,endItem, KNullDesC8);
		TPtr8 proxyAddrPtr8 = proxy->Des();
		HBufC8* proxyport = FindItemLC(KItemProxyPort,aStartItem,endItem, KNullDesC8); //new
		TPtr8 proxyportPtr8 = proxyport->Des();

	HBufC8* proxyAddrUrl8 = TSrvAddrVal::ReplaceHostNameL(proxyAddrPtr8, iIniSettingsFile);
		CleanupStack::PushL(proxyAddrUrl8);
		TPtr8 proxyUrlAddrPtr8 = proxyAddrUrl8->Des();
		TInt len;
		if(proxyportPtr8.Size()) 
			{
			len= proxyUrlAddrPtr8.Size() + proxyportPtr8.Size()+1;
			}
		else
			len= proxyUrlAddrPtr8.Size();

      HBufC8* proxyUrl8 = HBufC8::NewL(len);  //new
	  CleanupStack::PushL(proxyUrl8);
		TPtr8 proxyUrlPtr8(proxyUrl8->Des());
		proxyUrlPtr8.Copy(proxyUrlAddrPtr8);
		if(proxyportPtr8.Size()>0)
			{
			proxyUrlPtr8.Append(_L8(":"));
			proxyUrlPtr8.Append(proxyportPtr8);
			}
		
		if (proxy->Length() > 0)
			{
			RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
			RStringPool strP = iSession.StringPool();
			RStringF proxyAddr = strP.OpenFStringL(proxyUrlPtr8);
			CleanupClosePushL(proxyAddr);
			THTTPHdrVal proxyUsage(strP.StringF(HTTP::EUseProxy, RHTTPSession::GetTable()));
			connInfo.SetPropertyL(strP.StringF(HTTP::EProxyUsage, RHTTPSession::GetTable()), proxyUsage);
			connInfo.SetPropertyL(strP.StringF(HTTP::EProxyAddress, RHTTPSession::GetTable()), proxyAddr);
			CleanupStack::PopAndDestroy(1,&proxyAddr);
			}
		CleanupStack::PopAndDestroy(6, proxyUsage); // and proxyType, proxy, proxyport, proxyAddrUrl8, proxyUrl8

		}
	
	}

void CHttpTestTransactions::SetMaxRedirectsL(TInt aStartItem)
	{
	TInt endItem= FindEndOfTransaction(aStartItem);
	HBufC8* maxRedirectsText = FindItemLC(KItemRedirectCheck,aStartItem, endItem, KNullDesC8);
	if (maxRedirectsText && maxRedirectsText->Length() > 0)
		{
		TLex8 paramLex(*maxRedirectsText);
		TInt param;
		paramLex.Val(param);
		iExpectedNumberRedirects = param;
		RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
		RStringPool p=iSession.StringPool();
		connInfo.SetPropertyL(p.StringF(HTTP::EMaxRedirect, RHTTPSession::GetTable()),THTTPHdrVal(iExpectedNumberRedirects));
		}
	CleanupStack::PopAndDestroy(maxRedirectsText);
	}

void CHttpTestTransactions::LogCertificateL(TCertInfo aCertInfo)
	{
	TBuf<20> fingerprint;
	fingerprint.Copy(aCertInfo.iFingerprint);
	TBuf<64> subjectName;
	subjectName.Copy(aCertInfo.iSubjectDNInfo.iCommonName);
	TBuf<64> issuerName;
	issuerName.Copy(aCertInfo.iIssuerDNInfo.iCommonName);
	TBuf<32> serialNo;
	const TInt serLength = aCertInfo.iSerialNo.Length();
	for(TInt index = 0; index < serLength; ++index)
		{
		serialNo.AppendFormat(_L("%02x"), aCertInfo.iSerialNo[index]);
		}
	serialNo.UpperCase();

	// Log certificate data
	iEngine->Utils().LogIt(_L("--Server Certificate:"));
	iEngine->Utils().LogIt(_L("-----------------------------------"));
	iEngine->Utils().LogIt(_L("--Subject:     %S"), &subjectName);
	iEngine->Utils().LogIt(_L("--Issuer:      %S"), &issuerName);
	iEngine->Utils().LogIt(_L("--Version:     %d"), aCertInfo.iVersionNo);
	iEngine->Utils().LogIt(_L("--Serial No:   %S"), &serialNo);
	iEngine->Utils().LogIt(_L("--Fingerprint: %S"), &fingerprint);
	iEngine->Utils().LogIt(_L("--Digest:	  %d"), aCertInfo.iDigAlg );
	
	_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");
	TBuf<40> dateTimeString;
	aCertInfo.iStartValDate.FormatL(dateTimeString, KDateFormat);
	iEngine->Utils().LogIt(_L("--Start Date:  %S"), &dateTimeString);
	aCertInfo.iEndValDate.FormatL(dateTimeString, KDateFormat);
	iEngine->Utils().LogIt(_L("--End Date:    %S"), &dateTimeString);
	iEngine->Utils().LogIt(_L("-----------------------------------"));
	}

void CHttpTestTransactions::LogCertificateL(const CX509Certificate* aCertInfo)
	{
	HBufC* issuerName = aCertInfo->IssuerL();
	HBufC* subjectName = aCertInfo->SubjectL();

	// Log certificate data
	iEngine->Utils().LogIt(_L("--X905 Server Certificate:"));
	iEngine->Utils().LogIt(_L("-----------------------------------"));
	iEngine->Utils().LogIt(_L("-----------------------------------"));


	CleanupStack::PopAndDestroy(2, issuerName);
	}


void CHttpTestTransactions::LoadBodyFromFileL(const TDesC& aFilePath)
	{
	RFs fileSession;
	User::LeaveIfError(fileSession.Connect());
	CleanupClosePushL(fileSession);
	
	RFile bodyFile;
	TInt err = bodyFile.Open(fileSession, aFilePath, EFileRead & EFileShareReadersOnly);
	if( err==KErrNotFound || err==KErrPathNotFound)
		{
		// Try and resolve the file location as it may reside on another drive
		TParse fileName;
		fileName.Set(aFilePath, NULL, NULL);
		TPtrC resolvedFile;
		if(fileName.DrivePresent())
			{
			// Drive is present, so strip the drive letter off
			resolvedFile.Set(aFilePath.Mid(2));
			}
		else
			{
			resolvedFile.Set(aFilePath);
			}
			
		// file finder will look in the session drive first, then Y->A,Z
		// so set session drive to Y (save old and restore it afterwards)
		TFileName* savedPath = new TFileName;
		fileSession.SessionPath(*savedPath);
		
		_LIT(KTopDrive,"Y:\\");
		fileSession.SetSessionPath(KTopDrive);
		TFindFile fileFinder(fileSession);
    	err = fileFinder.FindByDir(resolvedFile,KNullDesC);
		if(err==KErrNone)
			{
			fileName.Set(fileFinder.File(),NULL,NULL);
			err = bodyFile.Open(fileSession, fileName.FullName(), EFileRead & EFileShareReadersOnly);
			}
			
		// Revert back to the original session path
		fileSession.SetSessionPath(*savedPath);

		delete savedPath;
		}

	User::LeaveIfError(err);
	CleanupClosePushL(bodyFile);
	
	delete iBody;
	iBody = NULL;
	
	TInt fileSize = 0;
	User::LeaveIfError(bodyFile.Size(fileSize));
	iBody = HBufC8::NewL(fileSize);
	TPtr8 buffer = iBody->Des();
	User::LeaveIfError(bodyFile.Read(buffer));
	
	CleanupStack::PopAndDestroy(2, &fileSession);
	}

//Creates a Default Proxy in CommsDat database for use by the build machine.
//Once all test cases were executed, deletes the Proxy record that was successfully created.
void CHttpTestTransactions::ConfigureDefaultProxyL(TConfigureProxyAction aAction)
	{		
	TInt32 defaultIapId = 0;
	TUint32 serviceId = 0;
	TBuf<KCommsDbSvrMaxFieldLength> serviceType;
	TBuf<KCommsDbSvrMaxFieldLength> protName;
	TBuf<KCommsDbSvrMaxFieldLength> serverName;
	
	// use commsDat API
 	// Create CommmsDat session using latest version of commsdat
	CMDBSession* dbSession = CMDBSession::NewLC(CMDBSession::LatestVersion());
	
	//Read Default IAP inorder to get its service Id and type.	
	//Default IAP is the one in "connection preferences" table and with a "rank" of 1.
	CCDConnectionPrefsRecord *connPrefRecord = static_cast<CCDConnectionPrefsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdConnectionPrefsRecord));
    CleanupStack::PushL(connPrefRecord);   

    // Set the direction of connection
    connPrefRecord->iDirection = ECommDbConnectionDirectionOutgoing;
    connPrefRecord->iRanking = 1;
    
    if(!connPrefRecord->FindL(*dbSession))
        {
        User::Leave(KErrNotFound);      
        }           
    defaultIapId = connPrefRecord->iDefaultIAP;
    iEngine->Utils().LogIt(_L("Default IAP value %d"), defaultIapId);
	//Done with Connection Preference Table, Destroy it.
	CleanupStack::PopAndDestroy	(connPrefRecord);
	
	//Got the default IAP id which was its record id in IAP table.
	//Get the Service ID and Serivce type of this IAP.
	CCDIAPRecord* defaultIapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	CleanupStack::PushL(defaultIapRecord);
	
	//Set the ID.
	defaultIapRecord->SetRecordId (defaultIapId);
	//Load the record.
	defaultIapRecord->LoadL(*dbSession);
	//read its serice id and service type.	
	serviceId = defaultIapRecord->iService;		
	serviceType.Copy(defaultIapRecord->iServiceType);
	//Done with IAP Record read, Destroy it.
	CleanupStack::PopAndDestroy(defaultIapRecord);	
	iEngine->Utils().LogIt(_L("Default ServiceID:%d;ServiceType:%S"), serviceId, &serviceType);
	// Now we've got serviceId and serviceType,

 	if (aAction == ECreate)
		{// Create proxy record for this Service ID and service Type	
		CCDProxiesRecord* pProxyRecord = static_cast<CCDProxiesRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdProxiesRecord));
		CleanupStack::PushL(pProxyRecord);

		pProxyRecord->iServiceType.SetMaxLengthL(serviceType.Length());
		pProxyRecord->iServiceType = serviceType;
	
		pProxyRecord->iService = serviceId;
		pProxyRecord->iUseProxyServer = ETrue;
	
		serverName.Copy(KProxyServerName);
		pProxyRecord->iServerName.SetMaxLengthL(serverName.Length());
		pProxyRecord->iServerName = serverName;
	
		protName.Copy(KProtocolName);
		pProxyRecord->iProtocolName.SetMaxLengthL(protName.Length());
		pProxyRecord->iProtocolName = protName;
	
		pProxyRecord->iPortNumber = KPortNumber;	
		pProxyRecord->SetRecordId(KCDNewRecordRequest);
	
		TRAPD(storeErr,	dbSession->OpenTransactionL());
		TRAP(storeErr, pProxyRecord->StoreL(*dbSession));
		iEngine->Utils().LogIt(_L("Created New Default Proxy:%S, at Port:%d"), &serverName, KPortNumber);
		TRAP(storeErr,	dbSession->CommitTransactionL());			
		
		CleanupStack::PopAndDestroy(pProxyRecord);
		}
	else if (aAction == EDelete)
		{// Delete proxy record for this Service ID and service Type that was created earlier by us		
		CMDBRecordSet<CCDProxiesRecord>* pProxiesRecordSet = new (ELeave)CMDBRecordSet<CCDProxiesRecord>(KCDTIdProxiesRecord);
		CleanupStack::PushL(pProxiesRecordSet);

		CCDProxiesRecord* primingProxyRecord = static_cast <CCDProxiesRecord*> (CCDRecordBase::RecordFactoryL(KCDTIdProxiesRecord));
		CleanupStack::PushL(primingProxyRecord);
		// build priming record for the search
		primingProxyRecord->iServiceType.SetMaxLengthL(serviceType.Length());
		primingProxyRecord->iServiceType = serviceType;
	
		primingProxyRecord->iService = serviceId;
		primingProxyRecord->iUseProxyServer = ETrue;
	
		pProxiesRecordSet->iRecords.AppendL(primingProxyRecord);
		CleanupStack::Pop(primingProxyRecord);
	
		iEngine->Utils().LogIt(_L("Searching for this Default Proxy"));	
		
		// Search for the priming record		
		if (pProxiesRecordSet->FindL(*dbSession))
			{//If exists Delete it
			serverName.Copy(primingProxyRecord->iServerName);
			TUint32 proxyPort = primingProxyRecord->iPortNumber;
			iEngine->Utils().LogIt(_L("Found Default Proxy Record:%S at port%d"), &serverName, proxyPort);
			TRAPD(deleteErr, dbSession->OpenTransactionL());

			TRAP(deleteErr, primingProxyRecord->DeleteL(*dbSession));
			iEngine->Utils().LogIt(_L("Deleted Default Proxy"));	
			
			TRAP(deleteErr, dbSession->CommitTransactionL());
			}
			
		CleanupStack::PopAndDestroy(pProxiesRecordSet);
		}
     CleanupStack::PopAndDestroy(dbSession);
	}
