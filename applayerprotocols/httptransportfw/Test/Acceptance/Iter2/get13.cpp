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

#include "get13.h"
#include <http.h>

#include "csrvaddrval.h"

_LIT8(KWapTestUrl,			"http://WapTestIP");

CHttpTestCaseGet13::CHttpTestCaseGet13(CScriptFile* aIniSettingsFile) 
	:	iIniSettingsFile(aIniSettingsFile)
	{
	iExpectedStatusCode=401;
	iAttempt=0;
	}

const TDesC& CHttpTestCaseGet13::TestName()
	{
	_LIT(KHeaderTestName,"Iteration 2 Get Test 13 ");
	return KHeaderTestName;
	}

void CHttpTestCaseGet13::OpenTestSessionL()
	{
	iAttempt++;
	iSession.OpenL();
	iEngine->Utils().LogIt(_L("Session Created(Iteration 2 Post Test 13)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	InstallAuthenticationL(iSession);
	iMyStrP = iSession.StringPool();

	// Literals used in the function
	_LIT8(KUrlPath,				"/http_tests/rc/401/private/index.html");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();
	newUrlPtr8.Append(KUrlPath());

	TUriParser8 testURI;
	testURI.Parse(newUrlPtr8);
	
	iTransaction = iSession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EGET,RHTTPSession::GetTable()));
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));
	iOpenTransactionCount++;

	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	//provide  some headers 

	THTTPHdrVal acceptHdr(iMyStrP.StringF(HTTP::EAnyAny, RHTTPSession::GetTable()));
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EAccept, RHTTPSession::GetTable()),acceptHdr);
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Get"));
	iEngine->Utils().LogIt(_L("Accept:*/*"));
	CleanupStack::PopAndDestroy(newUrl8);
	}


void CHttpTestCaseGet13::CloseTestSession()
	{
	// Close strings used in this session before closing the session
	//close Transaction and session
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}


TBool CHttpTestCaseGet13::GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword)
	{
	iAttempt++;
	if(iAttempt<=2)
		{
		iEngine->Utils().LogIt(_L("Supplying credentials"));
		iEngine->Utils().DumpData(aURI.UriDes());
		iEngine->Utils().DumpData(aRealm.DesC());	
		iEngine->Utils().DumpData(aAuthenticationType.DesC());	
		_LIT8(KUser, "invalid");
		_LIT8(KPass, "forgot");
		RStringPool p = aRealm.Pool();
		aUsername = p.OpenStringL(KUser);
		aPassword = p.OpenStringL(KPass);
		return ETrue;
		}
	else return EFalse;
	}
