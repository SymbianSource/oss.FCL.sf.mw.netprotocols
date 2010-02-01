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
// SingleTrans..cpp
// 
//

#include <x509cert.h>

#include "SingleTrans.h"
#include <http.h>

void CHttpTestSingleTrans::CreateSingleTransactionL()
	{
	iTransaction = CreateTransactionL(0);
	}

void CHttpTestSingleTrans::OpenTestSessionL()
	{
	iSession.OpenL();
	CleanupClosePushL ( iSession );
	SetProxyL(0); // 0 is the start item to look for it in the param list for the test in the ini file
	SetMaxRedirectsL(0);
	MHTTPAuthenticationCallback::InstallAuthenticationL(iSession);
	iMyStrP = iSession.StringPool();
	TPtrC testName = CHttpTestTransactions::TestName();
	if (testName.CompareF(_L("SSL9"))==0)
		{
		RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
		connInfo.SetPropertyL( 
							 iMyStrP.StringF(HTTP::ESecureDialog, RHTTPSession::GetTable() ),
							 iMyStrP.StringF(HTTP::EDialogNoPrompt, RHTTPSession::GetTable() )
							 );
		iEngine->Utils().LogIt(_L("Secure Dialog session property set\n"));
		}
	CreateSingleTransactionL();
	CleanupStack::Pop (&iSession);
	}

void CHttpTestSingleTrans::CloseTestSession()
	{
	LogCertificate();
	// Allow the base class to do any closing
	CHttpTestTransactions::CloseTestSession();
	// Close strings used in this session before closing the session
	//close Transaction and session
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}

void CHttpTestSingleTrans::LogCertificate()
	{
	if(iLogCert)
		{
		TRAPD(error, LogCertL());
		if(error != KErrNone)
			iEngine->Utils().LogIt(_L("Unable to log server certificate.  Error code=%d\n"), error);
		}
	}

void CHttpTestSingleTrans::LogCertL()
	{
	TCertInfo certInfo;
	User::LeaveIfError(iTransaction.ServerCert(certInfo));

	LogCertificateL(certInfo);

	const CX509Certificate* cert = static_cast<const CX509Certificate*>(iTransaction.ServerCert());
	LogCertificateL(cert);

	}

CHttpTestSingleTrans* CHttpTestSingleTrans::NewL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, const TInt aSectionNumber)
	{
	CHttpTestSingleTrans* self= new(ELeave) CHttpTestSingleTrans;
	CleanupStack::PushL(self);
	self->ConstructL(aIniFile, aIniSettingsFile, aSectionNumber);
	CleanupStack::Pop();
	return self;
	}

void CHttpTestSingleTrans::ConstructL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, const TInt aSectionNumber)
	{
	CHttpTestTransactions::ConstructL(aIniFile, aIniSettingsFile, aSectionNumber);

	_LIT(KLogCipherSuite, "LogCipherSuite");
	CScriptSectionItem* cipherSuiteItem = aIniFile.Section(aSectionNumber).Item(KLogCipherSuite);
	_LIT(KItemSet, "1");
	if(cipherSuiteItem && cipherSuiteItem->Value().Compare(KItemSet) == 0)
		{
		iLogCipherSuite = ETrue;
		}
	}
	
void CHttpTestSingleTrans::MHFRunL(RHTTPTransaction aTransaction,
								   const THTTPEvent& aEvent)
	{
	// Handle the event
	switch (aEvent.iStatus)
		{
	case THTTPEvent::EGotResponseHeaders:
		{
		if (iLogCipherSuite)
			{
			RString cipherSuite = aTransaction.CipherSuite();
			
			iEngine->Utils().LogIt(_L("Cipher Suite = %s\n"), &cipherSuite.DesC());
			}
		}
		break;
	default:
		;
		}
	CHttpAcceptTestBase::MHFRunL(aTransaction,aEvent);
	}

