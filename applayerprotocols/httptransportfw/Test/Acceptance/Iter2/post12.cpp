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

#include "post12.h"
#include <http.h>

#include "csrvaddrval.h"

_LIT8(KWapTestUrl,			"http://WapTestIP");

CHttpTestCasePost12::CHttpTestCasePost12(CScriptFile* aIniSettingsFile) 
	:	CHttpAcceptTestBase(),
		iIniSettingsFile(aIniSettingsFile)
	{
	}

const TDesC& CHttpTestCasePost12::TestName()
	{
	_LIT(KHeaderTestName,"Iteration 2 Post Test 12 ");
	return KHeaderTestName;
	}

void CHttpTestCasePost12::OpenTestSessionL()
	{
	//open  a Session
	
	iSession.OpenL();
	iEngine->Utils().LogIt(_L("Session Created(Iteration 2 Post Test 12)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));

	InstallAuthenticationL(iSession);

	//Get the iSession'string pool handle;
	iMyStrP = iSession.StringPool();
	//open string this testcase needs

	// Literals used in the function
	_LIT8(KUrlPath,				"/perl/protected/dumpform.pl");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);

	iTransaction = iSession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));
	iOpenTransactionCount++;

	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	//provide  some headers 

	THTTPHdrVal contentTypeHdr(iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable()));
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType, RHTTPSession::GetTable()),contentTypeHdr);

	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Content-Type:application/x-www-form-urlencoded "));
	CleanupStack::PopAndDestroy(newUrl8);
	myRequest.SetBody(*this);
	} 

void CHttpTestCasePost12::CloseTestSession()
	{
	// Close strings used in this session before closing the session
	//close Transaction and session
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}


TBool CHttpTestCasePost12::GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword)
	{
	iEngine->Utils().LogIt(_L("Supplying credentials"));
	// Can't log 8 bit data, so dump it.
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
