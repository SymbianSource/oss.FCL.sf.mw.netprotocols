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

#include "t_INC035621.h"

void CTestFixINC035621::DoRunL()
	{
	complete=EFalse;
	// Create a session and a transaction
	iSession.OpenL();
	InstallAuthenticationL(iSession);
//	_LIT8(KTestURIString,"http://WapTestName/http_tests/rc/401/private/index.html");
	_LIT8(KTestURIString,"WapTestName");
	
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KTestURIString(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUriPtr8=newUrl8->Des();
	newUriPtr8.Insert(0,_L8("http://"));
	newUriPtr8.Append(_L8("/http_tests/rc/401/private/index.html"));

	TUriParser8 uri8; 
//	uri8.Parse(*newUrl8); // -- old
	uri8.Parse(newUriPtr8);


	iMyStrP = iSession.StringPool();
	iTransaction = iSession.OpenTransactionL(uri8, *this);

	iError = KErrGeneral;

	// Submit a transaction
	iTransaction.SubmitL();

	// Run the test
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(newUrl8);

	iSession.Close();

	// Leave if it hasn't passed.
	User::LeaveIfError(iError);
	}

void CTestFixINC035621::DoCancel()
	{
	}


TInt CTestFixINC035621::MHFRunError(TInt /*aError*/,
										RHTTPTransaction aTransaction,
										const THTTPEvent& /*aEvent*/)
	{
	aTransaction.Fail();
	return 0;
	}

CTestFixINC035621* CTestFixINC035621::NewLC(CScriptFile* aIniSettingsFile)
	{
	CTestFixINC035621* that = new (ELeave) CTestFixINC035621(aIniSettingsFile);
	CleanupStack::PushL(that);
	return that;
	}

CTestFixINC035621::CTestFixINC035621(CScriptFile* aIniSettingsFile)	:
	CHttpTestBase(),
	iIniSettingsFile(aIniSettingsFile)
	{
	}

CTestFixINC035621::~CTestFixINC035621()
	{
	}


void CTestFixINC035621::MHFRunL(RHTTPTransaction aTransaction,
								  const THTTPEvent& aEvent)
	{
	switch(aEvent.iStatus)
		{
	case THTTPEvent::ESucceeded:
		{
		if (!complete)
			{
			complete=ETrue;
		//	_LIT8(KTestURIString,"http://WapTestName/http_tests/rc/401/private/dumpform.pl");
			_LIT8(KTestURIString,"WapTestName");
	
			HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KTestURIString(), iIniSettingsFile);
			CleanupStack::PushL(newUrl8);
			TPtr8 newUriPtr8=newUrl8->Des();
			newUriPtr8.Insert(0,_L8("http://"));
			newUriPtr8.Append(_L8("/http_tests/rc/401/private/dumpform.pl"));

			TUriParser8 uri8; 
			//	uri8.Parse(*newUrl8); // -- old
			uri8.Parse(newUriPtr8);

			iTransaction = iSession.OpenTransactionL(uri8, *this);

			// Submit a transaction
			iTransaction.SubmitL();

			CleanupStack::PopAndDestroy(newUrl8);
			}
		else
			{
			// If nothing has gone wrong so far, we've passed
			if (iError == KErrGeneral)
				iError = KErrNone;

			// Send second request
			CActiveScheduler::Stop();
			}
		}
		break;

	case THTTPEvent::EGotResponseBodyData:
		{
		TInt  statusCode = aTransaction.Response().StatusCode();
		if (statusCode==HTTPStatus::EUnauthorized)
			{

			}
		aTransaction.Response().Body()->ReleaseData();
		}
		break;

	case THTTPEvent::EFailed:
		CActiveScheduler::Stop();
		break;

	default:
		break;
		}
	}

const TDesC& CTestFixINC035621::TestName()
	{
	_LIT(KTestName,"Defect INC035621");
	return KTestName;
	}


TBool CTestFixINC035621::GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType,
									RString& aUsername, RString& aPassword)
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
