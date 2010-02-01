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
// Functions for testing HttpTextCodec.
// Tested directly, not via protocol handler.
// 
//

#include "ctesthttpheadercodec.h"

#include <http/rhttpheaders.h>
#include "chttpserverheadercodec.h"


#include "CHeaders.h"

_LIT8(KHttpServerCodecName, "HTTP/server");


void CTestHttpHeaderCodec::DoRunL()
	{
	iExpectedStatusCode = 0;
	// insert test code here, allow it to leave if the test fails
	RunTestsL();
	}
	
TInt CTestHttpHeaderCodec::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CTestHttpHeaderCodec::DoCancel()
	{
	}
	
const TDesC& CTestHttpHeaderCodec::TestName()
	{
	_LIT(KHeaderTestName,"T_TextHeaderCodec");
	return KHeaderTestName;
	}

CTestHttpHeaderCodec::CTestHttpHeaderCodec()
// Constructor
	{
	// Initial timestamp is time now
	ResetTimeElapsed();
	}

CTestHttpHeaderCodec::~CTestHttpHeaderCodec()
// Destructor (virtual)
	{
	}

CTestHttpHeaderCodec* CTestHttpHeaderCodec::NewLC()
// Create a new CTestHttpHeaderCodec and place it on the clean-up stack then return it
	{
	CTestHttpHeaderCodec* me = new(ELeave) CTestHttpHeaderCodec;
	CleanupStack::PushL(me);
	return me;
	}

CTestHttpHeaderCodec* CTestHttpHeaderCodec::NewL()
// Create a new CTestHttpHeaderCodec and return it
	{
	CTestHttpHeaderCodec* me = new(ELeave) CTestHttpHeaderCodec;
	return me;
	}

void CTestHttpHeaderCodec::RunTestsL()
	{
	
	CleanupClosePushL(iStrP);
	iStrP.OpenL(RHTTPSession::GetTable());
	
	iCodec = CHeaderCodecPlugin::NewL( KHttpServerCodecName, iStrP);
	CleanupStack::PushL(iCodec);

	iEngine->Console().Printf(_L("\nStart of header codec tests"));
	ResetTimeElapsed();

	// Encode tests.
	TestEncodeWWWAuthenticateL();

	// Decode tests.
	TestDecodeAcceptL();
	TestDecodeAcceptCharsetL();
	TestDecodeAuthorizationL();
	TestDecodeHostL();
	TestDecodeExpectL();
	TestDecodeUserAgentL();

	// Close the string pool
	CleanupStack::PopAndDestroy(iCodec);
	iStrP.Close();
	CleanupStack::PopAndDestroy(&iStrP);
	REComSession::FinalClose();


	// End of tests
	DisplayTimeElapsed();
	iEngine->Console().Printf(_L("\nEnd of header codec tests"));
	
	}


void CTestHttpHeaderCodec::TestDecodeAcceptL()
	{
	_LIT8(KAcceptOTAVal, "text/html;q=0.8, text/vnd.wap.wml;q=0.2;extended=value, text/*");

	RStringF acceptStr = iStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable());

	// Open strings used in this test
	RStringF textHtml = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF textWml  = iStrP.StringF(HTTP::ETextVndWapWml,RHTTPSession::GetTable());
	RStringF textAny  = iStrP.StringF(HTTP::ETextAny,RHTTPSession::GetTable());

	RStringF extended  = iStrP.OpenFStringL(_L8("extended"));
	CleanupClosePushL(extended);
	RStringF extendVal = iStrP.OpenFStringL(_L8("value"));
	CleanupClosePushL(extendVal);

	//
	iEngine->Console().Printf(_L("\nTest decoding 'Accept' header"));
	CHeaders* hdr= CHeaders::NewL(*iCodec);

	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	hdr->SetRawFieldL(acceptStr, KAcceptOTAVal(), _L8("\n"));

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr->GetField(acceptStr,0,hVal));

	iEngine->Console().Printf(_L("\ncheck for correct number of parts"));
	if(hdr->FieldPartsL(acceptStr) != 3)
		User::Leave(KErrArgument);

	// Check 1st value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt1;  
	User::LeaveIfError(hdr->GetField(acceptStr, 0, vPt1));
	if (vPt1.StrF() != textHtml)
		User::Leave(KErrArgument);

	RStringF qStr = iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable());
 	THTTPHdrVal paramVal;
	User::LeaveIfError(hdr->GetParam(acceptStr, qStr, paramVal, 0));
	if (!CompareStringL(paramVal.StrF(), _L8("0.8")))
		User::Leave(KErrArgument);
	

	// Check 2st value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt2;  
	User::LeaveIfError(hdr->GetField(acceptStr, 1, vPt2));
	if (vPt2.StrF() != textWml)
		User::Leave(KErrArgument);

	User::LeaveIfError(hdr->GetParam(acceptStr, qStr, paramVal, 1));
	if (!CompareStringL(paramVal.StrF(), _L8("0.2")))
		User::Leave(KErrArgument);
	
	User::LeaveIfError(hdr->GetParam(acceptStr, extended, paramVal, 1));
	if (!CompareStringL(paramVal.StrF(), _L8("value")))
		User::Leave(KErrArgument);

	// Check 3rd value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt3;  
	User::LeaveIfError(hdr->GetField(acceptStr, 2, vPt3));
	if (vPt3.StrF() != textAny)
		User::Leave(KErrArgument);
	
	delete hdr;
	CleanupStack::PopAndDestroy(2,&extended);
	}


// NEW test in ServerTextHeader codec ...
void CTestHttpHeaderCodec::TestDecodeAcceptCharsetL()
	{
	_LIT8(KAccChSetOTAVal, "iso-8859-5, unicode-1-1;q=0.8");

	RStringF accChSet= iStrP.StringF(HTTP::EAcceptCharset,RHTTPSession::GetTable());

	// Open strings used in this test
	RStringF iso8859_5 = iStrP.OpenFStringL(_L8("iso-8859-5"));
	CleanupClosePushL(iso8859_5);
	RStringF unicode1_1  = iStrP.OpenFStringL(_L8("unicode-1-1"));
	CleanupClosePushL(unicode1_1);

	//
	CHeaders* hdr= CHeaders::NewL(*iCodec);
	iEngine->Console().Printf(_L("\nTest decoding 'Accept-Charset' header"));

	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	hdr->SetRawFieldL(accChSet, KAccChSetOTAVal(), _L8("\n"));

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr->GetField(accChSet,0,hVal));

	iEngine->Console().Printf(_L("\ncheck for correct number of parts"));
	if(hdr->FieldPartsL(accChSet) != 2)
		User::Leave(KErrArgument);

	// Check 1st value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt1;  
	User::LeaveIfError(hdr->GetField(accChSet, 0, vPt1));
	if (vPt1.StrF() != iso8859_5)
		User::Leave(KErrArgument);
	

	// Check 2st value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt2;  
	User::LeaveIfError(hdr->GetField(accChSet, 1, vPt2));
	if (vPt2.StrF() != unicode1_1)
		User::Leave(KErrArgument);

 	THTTPHdrVal paramVal;
	RStringF qStr = iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable());
	User::LeaveIfError(hdr->GetParam(accChSet, qStr, paramVal, 1));
	if (!CompareStringL(paramVal.StrF(), _L8("0.8")))
		User::Leave(KErrArgument);
	
	delete hdr;
	CleanupStack::PopAndDestroy(2,&iso8859_5);
	}


void CTestHttpHeaderCodec::TestDecodeUserAgentL()
	{
 	// the result we expect and that we will validate
	_LIT8(KUserAgentOTAVal, "CERN-LineMode/2.15 libwww/2.17b3");

	RStringF useAgent= iStrP.StringF(HTTP::EUserAgent,RHTTPSession::GetTable());

	// Open strings used in this test
	RStringF ua1Str = iStrP.OpenFStringL(_L8("CERN-LineMode/2.15"));
	CleanupClosePushL(ua1Str);
	RStringF ua2Str = iStrP.OpenFStringL(_L8("libwww/2.17b3"));
	CleanupClosePushL(ua2Str);
	
	//
	CHeaders* hdr= CHeaders::NewL(*iCodec);
	iEngine->Console().Printf(_L("\nTest decoding 'User-Agent' header"));

	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	hdr->SetRawFieldL(useAgent, KUserAgentOTAVal(), _L8("\n"));

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr->GetField(useAgent,0,hVal));

	iEngine->Console().Printf(_L("\ncheck for correct number of parts"));
	if(hdr->FieldPartsL(useAgent) != 2)
		User::Leave(KErrArgument);

	// Check 1st value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt1;  
	User::LeaveIfError(hdr->GetField(useAgent, 0, vPt1));
	if (vPt1.StrF() != ua1Str)
		User::Leave(KErrArgument);
	

	// Check 2st value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt2;  
	User::LeaveIfError(hdr->GetField(useAgent, 1, vPt2));
	if (vPt2.StrF() != ua2Str)
		User::Leave(KErrArgument);
	
	delete hdr;
	CleanupStack::PopAndDestroy(2,&ua1Str);
	}

void CTestHttpHeaderCodec::TestDecodeAuthorizationL()
	{
	// the result we expect and that we will validate
	_LIT8(KAuthorizationOTAVal, "Basic c3ltYmlhbjpmMXN5bmNtbA==");

	// Open strings used in this test
	RStringF basicCred = iStrP.OpenFStringL(_L8("c3ltYmlhbjpmMXN5bmNtbA=="));
	CleanupClosePushL(basicCred);

	RStringF authStr = iStrP.StringF(HTTP::EAuthorization,RHTTPSession::GetTable());
	THTTPHdrVal authVal(iStrP.StringF(HTTP::EBasic,RHTTPSession::GetTable()));
	
	//
	CHeaders* hdr= CHeaders::NewL(*iCodec);
	iEngine->Console().Printf(_L("\nTest decoding 'Authorization' header"));

	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	hdr->SetRawFieldL(authStr, KAuthorizationOTAVal(), _L8("\n"));

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr->GetField(authStr,0,hVal));

	iEngine->Console().Printf(_L("\ncheck for correct number of parts"));
	if(hdr->FieldPartsL(authStr) != 2)
		User::Leave(KErrArgument);

	// Check 1st value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt1;  
	User::LeaveIfError(hdr->GetField(authStr, 0, vPt1));
	if (vPt1.StrF() != authVal)
		User::Leave(KErrArgument);
	
	// Check 1st value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt2;  
	User::LeaveIfError(hdr->GetField(authStr, 1, vPt2));
	if (vPt2.StrF() != basicCred)
		User::Leave(KErrArgument);

	delete hdr;
	CleanupStack::PopAndDestroy(&basicCred);
	}

void CTestHttpHeaderCodec::TestDecodeHostL()
	{
	_LIT8(KHostOTAVal1, "www.symbian.com"); // ota host header
	_LIT8(KHostOTAVal2, "www.psion.com:8080"); // ota host header

	// Open strings used in this test
	RStringF host1Str = iStrP.OpenFStringL(_L8("www.symbian.com"));
	CleanupClosePushL(host1Str);
	RStringF host2Str = iStrP.OpenFStringL(_L8("www.psion.com"));
	CleanupClosePushL(host2Str);

	RStringF port = iStrP.StringF(HTTP::EPort,RHTTPSession::GetTable());
	RStringF host = iStrP.StringF(HTTP::EHost,RHTTPSession::GetTable());
	
	//
	CHeaders* hdr= CHeaders::NewL(*iCodec);
	iEngine->Console().Printf(_L("\nTest decoding 'Host' header"));

	// Test 1
	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	hdr->SetRawFieldL(host, KHostOTAVal1(), _L8("\n"));

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr->GetField(host,0,hVal));

	iEngine->Console().Printf(_L("\ncheck for correct number of parts"));
	if(hdr->FieldPartsL(host) != 1)
		User::Leave(KErrArgument);

	// check value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt1;  
	User::LeaveIfError(hdr->GetField(host, 0, vPt1));
	if (vPt1.StrF() != host1Str)
		User::Leave(KErrArgument);

	delete hdr;
	hdr=NULL;
	hdr= CHeaders::NewL(*iCodec);
	
	// Test 2
	iEngine->Console().Printf(_L("\nadd 2nd OTA data to header collection"));
	hdr->SetRawFieldL(host, KHostOTAVal2(), _L8("\n"));

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	User::LeaveIfError(hdr->GetField(host,0,hVal));

	iEngine->Console().Printf(_L("\ncheck for correct number of parts"));
	if(hdr->FieldPartsL(host) != 1)
		User::Leave(KErrArgument);

	// check value
	iEngine->Console().Printf(_L("\ndecode and check header part"));
	User::LeaveIfError(hdr->GetField(host, 0, vPt1));
	if (vPt1.StrF() != host2Str)
		User::Leave(KErrArgument);

	// check param
	iEngine->Console().Printf(_L("\ndecode and check for parameter"));
 	THTTPHdrVal paramVal;
	User::LeaveIfError(hdr->GetParam(host, port, paramVal, 0));
	if (!CompareStringL(paramVal.StrF(), _L8("8080")))
		User::Leave(KErrArgument);

	delete hdr;
	CleanupStack::PopAndDestroy(2, &host1Str);
	}

void CTestHttpHeaderCodec::TestDecodeExpectL()
	{
	// the result we expect and that we will validate
	_LIT8(KExpectValOne, " 100-continue");

	// Open strings used in this test
	RStringF expectStr = iStrP.StringF(HTTP::EExpect,RHTTPSession::GetTable());
	THTTPHdrVal expectVal(iStrP.StringF(HTTP::E100Continue,RHTTPSession::GetTable()));
	
	//
	CHeaders* hdr= CHeaders::NewL(*iCodec);
	iEngine->Console().Printf(_L("\nTest decoding 'Expect' header"));

	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	hdr->SetRawFieldL(expectStr, KExpectValOne(), _L8("\n"));

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr->GetField(expectStr,0,hVal));

	iEngine->Console().Printf(_L("\ncheck for correct number of parts"));
	if(hdr->FieldPartsL(expectStr) != 1)
		User::Leave(KErrArgument);

	// Check value
	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt1;  
	User::LeaveIfError(hdr->GetField(expectStr, 0, vPt1));
	TBuf8<50> buf(vPt1.StrF().DesC());
	if (vPt1.StrF() != expectVal)
		User::Leave(KErrArgument);	

	delete hdr;
	}


void CTestHttpHeaderCodec::TestEncodeWWWAuthenticateL()
	{
	// the result we expect to generate
	_LIT8(KWWWAuthenticateVal1, "Basic realm=\"basicrealm\", Digest realm=\"digestrealm\""); 
	iEngine->Console().Printf(_L("\nTest encoding 'WWW-Authenticate' header"));

	// Open strings used in this test
	RStringF basic = iStrP.StringF(HTTP::EBasic,RHTTPSession::GetTable());
	RStringF digest = iStrP.StringF(HTTP::EDigest,RHTTPSession::GetTable());
	RStringF realm = iStrP.StringF(HTTP::ERealm,RHTTPSession::GetTable());

	RStringF basicrealm  = iStrP.OpenFStringL(_L8("basicrealm"));
	CleanupClosePushL(basicrealm);
	RStringF digestrealm = iStrP.OpenFStringL(_L8("digestrealm"));
	CleanupClosePushL(digestrealm);

	//	
	CHeaders* hdr = CHeaders::NewL(*iCodec);
	CleanupStack::PushL(hdr);

	// Set up the parsed header
	RStringF wwwAuth = iStrP.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable());
	THTTPHdrVal val(basic);
	THTTPHdrVal realmVal(basicrealm);
	hdr->SetFieldL(wwwAuth, val, iStrP.StringF(HTTP::ERealm,RHTTPSession::GetTable()), realmVal);

	val.SetStrF(digest);
	realmVal.SetStrF(digestrealm);
	hdr->SetFieldL(wwwAuth, val, iStrP.StringF(HTTP::ERealm,RHTTPSession::GetTable()), realmVal);

	// now get the OTA data
	TPtrC8 ota;
	TInt err =  hdr->GetRawField(wwwAuth, ota);
	if (err != KErrNone)
		User::Leave(err);

	if (ota.Compare(KWWWAuthenticateVal1()) != 0)
		User::Leave(KErrArgument);

	// Close strings used in this test
	CleanupStack::PopAndDestroy(hdr);
	CleanupStack::PopAndDestroy(2, &basicrealm);
	}


void CTestHttpHeaderCodec::ResetTimeElapsed()
// Resets timestamp to time now
	{
	iLastTimeStamp.UniversalTime(); 
	}


void CTestHttpHeaderCodec::DisplayTimeElapsed()
// Calculate elapsed time since last measurement, and display
	{
	TTime timeNow;
	timeNow.UniversalTime();
	TTimeIntervalMicroSeconds elapsedMicroSec =
									timeNow.MicroSecondsFrom(iLastTimeStamp);
	iLastTimeStamp = timeNow;
	iEngine->Console().Printf(
		_L("Time elapsed since last measurement is: %d ms\n"),
		elapsedMicroSec.Int64()/1000
		);
	}

TBool CTestHttpHeaderCodec::CompareStringL(RStringF aStrTk, const TDesC8& aDes)
	{
	RStringF desStr = iStrP.OpenFStringL(aDes);
	TBool retVal = (desStr == aStrTk);
	desStr.Close();
	return retVal;
	}
TBool CTestHttpHeaderCodec::CompareStringL(RString aStrTk, const TDesC8& aDes)
	{
	RString desStr = iStrP.OpenStringL(aDes);
	TBool retVal = (desStr == aStrTk);
	desStr.Close();
	return retVal;
	}

TBool CTestHttpHeaderCodec::CompareDate(TDateTime aDate1, TDateTime aDate2)
	{
	return ((aDate1.Year() == aDate2.Year()) &&
			(aDate1.Month() == aDate2.Month()) &&
			(aDate1.Day() == aDate2.Day()) &&
			(aDate1.Hour() == aDate2.Hour()) &&
			(aDate1.Minute() == aDate2.Minute()) &&
			(aDate1.Second() == aDate2.Second()) &&
			(aDate1.MicroSecond() == aDate2.MicroSecond()));
	}
