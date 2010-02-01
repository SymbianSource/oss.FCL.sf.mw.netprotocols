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

#include "post14.h"
#include <http.h>

#include "csrvaddrval.h"

_LIT8(KWapTestUrl,			"http://WapTestIP");

CHttpTestCasePost14::CHttpTestCasePost14(CScriptFile* aIniSettingsFile) : 
	CHttpAcceptTestBase(),
	iIniSettingsFile(aIniSettingsFile)
	{
	}

const TDesC& CHttpTestCasePost14::TestName()
	{
	_LIT(KHeaderTestName,"Iteration 2 Post Test 14 ");
	return KHeaderTestName;
	}

void CHttpTestCasePost14::OpenTestSessionL()
	{
	//open a default Session
	iSession.OpenL();

	//Logs 
	iEngine->Utils().LogIt(_L("Session Created(Iteration 2 Post Test 14)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	
	InstallAuthenticationL(iSession);

	//Get the iSession'string pool handle;
	iMyStrP = iSession.StringPool();
	}


void CHttpTestCasePost14::OpenTestTrans1L()
	{	
	// Literals used in the function
	_LIT8(KUrlPath,				"/perl/protected/dumpform.pl");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);

	iTransaction1 = iSession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));
	++iOpenTransactionCount;

	//Get a handle of the request in iTransaction1
	RHTTPRequest myRequest = iTransaction1.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();

	THTTPHdrVal contentTypeHdr(iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable()));
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType, RHTTPSession::GetTable()),contentTypeHdr);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Content-Type:application/x-www-form-urlencoded "));
	myRequest.SetBody(*this);
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCasePost14::OpenTestTrans2L()
	{	
	// Literals used in the function
	_LIT8(KUrlPath,				"/perl/protected/dumpform.pl");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);

	iTransaction2 = iSession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));
	++iOpenTransactionCount;

	//Get a handle of the request in iTransaction2
	RHTTPRequest myRequest = iTransaction2.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();

	THTTPHdrVal contentTypeHdr(iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable()));
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType, RHTTPSession::GetTable()),contentTypeHdr);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	myRequest.SetBody(*this);
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCasePost14::CloseTestSession()
	{
	iTransaction1.Close();
	iEngine->Utils().LogIt(_L("Transaction1 terminated\n"));
	iTransaction2.Close();
	iEngine->Utils().LogIt(_L("Transaction2 terminated\n"));
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}


TBool CHttpTestCasePost14::GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword)
	{
	 if(iCallsNum++)
		 {
		 iTestFail=1;
		 iEngine->Utils().LogIt(_L("The test will fail: the credential haven't been kept(the GetCredential function has been called again)"));
		 return EFalse;
		 }
	iEngine->Utils().LogIt(_L("Supplying credentials"));
	iEngine->Utils().DumpData(aURI.UriDes());
	iEngine->Utils().DumpData(aRealm.DesC());	
	iEngine->Utils().DumpData(aAuthenticationType.DesC());	
	_LIT8(KScooby, "scooby");
	_LIT8(KDoo, "doo");
	RStringPool p = aRealm.Pool();
	aUsername = p.OpenStringL(KScooby);
	aPassword = p.OpenStringL(KDoo);
	return ETrue;
	}

void CHttpTestCasePost14::DoRunL()
	{
	OpenTestSessionL(); 

	//Trans1
	OpenTestTrans1L();
	iTransaction1.SubmitL();
	CActiveScheduler::Start();

	//Trans2
	OpenTestTrans2L();
	iTransaction2.SubmitL();
	CActiveScheduler::Start();
	
	CloseTestSession();
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	}
