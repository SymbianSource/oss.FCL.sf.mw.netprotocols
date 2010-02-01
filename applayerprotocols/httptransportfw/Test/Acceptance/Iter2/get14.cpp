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

#include "get14.h"
#include <http.h>
#include "csrvaddrval.h"

_LIT8(KWapTestUrl,			"http://WapTestIP");

CHttpTestCaseGet14::CHttpTestCaseGet14(CScriptFile* aIniSettingsFile) 
	:	CHttpAcceptTestBase(),
		iIniSettingsFile(aIniSettingsFile)
	{
	}

const TDesC& CHttpTestCaseGet14::TestName()
	{
	_LIT(KHeaderTestName,"Iteration 2 Get Test 14 ");
	return KHeaderTestName;
	}

void CHttpTestCaseGet14::OpenTestTrans1L()
	{	
 	// Literals used in the function
	_LIT8(KUrlPath,				"/http_tests/rc/401/private/index.html");
	
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);
	
	iTransaction1 = iSession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iOpenTransactionCount++;
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCaseGet14::OpenTestTrans2L()
	{	
 	// Literals used in the function
	_LIT8(KUrlPath,				"/http_tests/rc/401/private/index2.html");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);
	
	iTransaction2 = iSession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iOpenTransactionCount++;
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCaseGet14::CloseTestSession()
	{
	iTransaction1.Close();
	iEngine->Utils().LogIt(_L("Transaction1 terminated\n"));
	iTransaction2.Close();
	iEngine->Utils().LogIt(_L("Transaction2 terminated\n"));
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}


TBool CHttpTestCaseGet14::GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword)
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

void CHttpTestCaseGet14::DoRunL()
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

	iTransaction1.Close();
	iTransaction2.Close();
	
	CloseTestSession();
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	}
 
void CHttpTestCaseGet14::OpenTestSessionL()
	{
	//open a default Session
	iSession.OpenL();

	//Logs 
	iEngine->Utils().LogIt(_L("Session Created(Iteration 2 Get Test 14)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	
	InstallAuthenticationL(iSession);

	//Get the iSession'string pool handle;
	iMyStrP = iSession.StringPool();
	}
