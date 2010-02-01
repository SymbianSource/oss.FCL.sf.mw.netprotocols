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

#include "get12.h"
#include <http.h>
#include <es_sock.h>
#include <commdbconnpref.h>
#include <es_enum.h>
#include "csrvaddrval.h"

// Literals used in the file
_LIT8(KWapTestIPSection, "http://WapTestIP");

CHttpTestCaseGet12::CHttpTestCaseGet12(CScriptFile* aIniSettingsFile)
	:	iIniSettingsFile(aIniSettingsFile)
	{
	iExpectedStatusCode=200;
	}

const TDesC& CHttpTestCaseGet12::TestName()
	{
	_LIT(KHeaderTestName,"Iteration 2 Get Test 12 ");
	return KHeaderTestName;
	}

void CHttpTestCaseGet12::OpenTestSessionL()
	{
	iSession.OpenL();
	iEngine->Utils().LogIt(_L("Session Created(Iteration 2 Get Test 12)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));
	InstallAuthenticationL(iSession);
	iMyStrP = iSession.StringPool();

	// Literals used in the function
	_LIT8(KUrlPath,				"/http_tests/rc/401/private/index.html");

	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestIPSection(), iIniSettingsFile);
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
	iEngine->Utils().LogIt(_L("Method:GET"));
	CleanupStack::PopAndDestroy(newUrl8);
	}

void CHttpTestCaseGet12::LogConnectionProperties()
	{
	RHTTPConnectionInfo	connectionInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();
	THTTPHdrVal value;
	TBool hasValue = connectionInfo.Property (stringPool.StringF(HTTP::EHttpSocketConnection, RHTTPSession::GetTable()), value);
	if (hasValue)
		{
		RConnection* conn = REINTERPRET_CAST(RConnection*, value.Int());
		TUint count;
		//get the no of active connections
		TInt err = conn->EnumerateConnections(count);
		if(err==KErrNone)
			{
			for (TUint i=1; i<=count; ++i)
				{
				TPckgBuf<TConnectionInfo> connInfo;
				conn->GetConnectionInfo(i, connInfo);
				iEngine->Utils().LogIt(_L("Connection %d: IapId=%d, NetId=%d\n"),i, connInfo().iIapId, connInfo().iNetId);
				}
			}
		else
			{
			iEngine->Utils().LogIt(_L("Unable to enumerate number of connections, Error: %d"), err);
			}
		}
	}

void CHttpTestCaseGet12::CloseTestSession()
	{
	//log connection properties before closing the session
	LogConnectionProperties();
	// Close strings used in this session before closing the session
	//close Transaction and session
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}


TBool CHttpTestCaseGet12::GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword)
	{
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
