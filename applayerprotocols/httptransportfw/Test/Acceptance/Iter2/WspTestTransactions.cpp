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

// System includes
#include <f32file.h>

// User includes
#include "WspTestTransactions.h"
#include "csrvaddrval.h"

// Proxy information properties
_LIT(KItemWspProxyAddress,			"proxy_address");
_LIT(KItemWspBearer,				"wsp_bearer");
_LIT(KItemWspLocalPort,				"local_port");
_LIT(KItemWspRemotePort,			"remote_port");
_LIT(KItemWspServiceNumber,			"service_number");
_LIT(KItemWspSessionType,			"session_type");
_LIT(KItemWspSecurity,				"security");

// Capability properties
_LIT(KItemWspClientMsgSize,			"client_message_size");
_LIT(KItemWspServerMsgSize,			"server_message_size");
_LIT(KItemWspUseAcknowledgements,	"use_acknowledgements");
_LIT(KItemWspSupportSuspendResume,	"support_suspend_resume");
_LIT(KItemWspMaxOutstandingRequests,"max_outstanding_requests");
_LIT(KItemExpectedError,			"test_validation_param_expectederror");

// Security policy properties
_LIT(KSecurityPolicy,				"securitypolicy");
_LIT(KSecurityPolicyFileName,		"securitypolicy.ini");

_LIT(KValidateUntrustedServerCert,	"ValidateUntrustedServerCert");
_LIT(KWTLSCipherSuites,				"WTLSCipherSuites");
_LIT(KWTLSKeyExchangeSuites,		"WTLSKeyExchangeSuites");
_LIT(KTLSCipherSuites,				"TLSCipherSuites");
_LIT(KPolicy,						"Policy");


CWspTestTransactions* CWspTestTransactions::NewL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, const TInt aSectionName)
	{
	CWspTestTransactions* self = new(ELeave) CWspTestTransactions(aIniSettingsFile);
	CleanupStack::PushL(self);
	self->ConstructL(aIniFile, aIniSettingsFile, aSectionName);
	CleanupStack::Pop(self);
	return self;
	}


CWspTestTransactions::CWspTestTransactions(CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile)
	{
	}


void CWspTestTransactions::ConstructL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, const TInt aSectionName)
	{
	CHttpTestTransactions::ConstructL(aIniFile, aIniSettingsFile, aSectionName);
	}


CWspTestTransactions::~CWspTestTransactions()
	{
	}

void CWspTestTransactions::DoRunL()
	{
	OpenTestSessionL(); // set up session and transaction here
	if ((iSessionEvent.iStatus) == THTTPSessionEvent::EDisconnected || (iSessionEvent.iStatus) == THTTPSessionEvent::EConnectionTimedOut)	
		{
		DoCloseTestSession(); // close any strings, transactiona and session

		// Check if the expected status is returned
		if( (iSessionEvent.iStatus == THTTPSessionEvent::EConnectionTimedOut) && (iExpectedStatusCode == 504) )
			{
			// No fail...
			iTestFail = 0;
			}
		else if( (iSessionEvent.iStatus == THTTPSessionEvent::EDisconnected) && (iExpectedError == THTTPSessionEvent::EAuthenticationFailure) && iAuthenticationFailure )
			{
			// No fail...
			iTestFail = 0;
			}
		else
			{
			// Fail!
			iTestFail = 1;
			}

		iEngine->Utils().LogIt(_L("Warning: Connection to proxy closed\n"));
		}
	else
		{
		CActiveScheduler::Start();
		CloseTestSession(); // disconnect from gateway and then close any 
							// strings, transactions and sessions
		}
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	}


void CWspTestTransactions::OpenTestSessionL()
	{
	// Iterate through the TestCases.ini file, beginning at the first element 
	// i.e. index 0, and write the security policy properties to the 
	// securitypolicy.ini file.
	SetWspSecurityPolicyPropertiesL(0);

	iSession.OpenL(_L8("WSP/WSP"));

	iMyStrP = iSession.StringPool();

	// Set the session callback
	iSession.SetSessionEventCallback(this);

	SetWspSessionPropertiesL();

	ConnectToGatewayL();
	}


void CWspTestTransactions::CloseTestSession()
	{
	LogCertificate();
	// Disconnect from the gateway first
	TRAP_IGNORE( DisconnectFromGatewayL() );
	DoCloseTestSession();
	}

void CWspTestTransactions::LogCertificate()
	{
	if(iLogCert)
		{
		TRAPD(error, LogCertL());
		if(error != KErrNone)
			iEngine->Utils().LogIt(_L("Unable to log server certificate.  Error code=%d\n"), error);
		}
	}

void CWspTestTransactions::LogCertL()
	{
	TCertInfo certInfo;
	User::LeaveIfError(iSession.ServerCert(certInfo));

	LogCertificateL(certInfo);
	}

void CWspTestTransactions::DoCloseTestSession()
	{
	CHttpTestTransactions::CloseTestSession();
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction closed\n"));
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session closed"));
	}


void CWspTestTransactions::ConnectToGatewayL()
	{
	iSession.ConnectL();
	CActiveScheduler::Start();
	}


void CWspTestTransactions::DisconnectFromGatewayL()
	{
	iSession.DisconnectL();
	CActiveScheduler::Start();
	}


void CWspTestTransactions::CreateSingleTransactionL()
	{
	// Iterate through the TestCases.ini file, beginning at the first element
	// i.e. index 0, and set up the transaction properties
	iTransaction = CreateTransactionL(0);
	iTransaction.SubmitL();
	}


void CWspTestTransactions::MHFSessionRunL(const THTTPSessionEvent& aEvent)
	{
	iSessionEvent = aEvent;
	switch(aEvent.iStatus)
		{
	case THTTPSessionEvent::EConnectedOK:
		{
		iEngine->Utils().LogIt(_L("WSP Session connected. No reduced capabilities\n"));
		CreateSingleTransactionL();
		CActiveScheduler::Stop();
		} break;
	case THTTPSessionEvent::EConnectedWithReducedCapabilities:
		{
		iEngine->Utils().LogIt(_L("WSP Session connected with reduced capabilities\n"));
		CreateSingleTransactionL();
		CActiveScheduler::Stop();
		} break;
	case THTTPSessionEvent::EConnectionTimedOut:
		{
		// Need to create the transaction even though its timed out to read
		// the ini file for the time out tests in servertestcases.ini
		iTransaction = CreateTransactionL(0);
		iEngine->Utils().LogIt(_L("Proxy connection timed out\n"));
		CActiveScheduler::Stop();
		} break;
	case THTTPSessionEvent::EDisconnected:
		{
		iEngine->Utils().LogIt(_L("WSP Session disconnected\n"));
		CActiveScheduler::Stop();
		} break;
	case THTTPSessionEvent::ERedirected:
		{
		iEngine->Utils().LogIt(_L("Redirected\n"));
		} break;
	case THTTPSessionEvent::EAuthenticatedOK:
		{
		iEngine->Utils().LogIt(_L("Authenticated OK\n"));
		} break;
	case THTTPSessionEvent::EAuthenticationFailure:
		{
		iEngine->Utils().LogIt(_L("Warning: Authentication Failure\n"));
		iAuthenticationFailure = ETrue;
		} break;
	case THTTPSessionEvent::ENotConnected:
		{
		iEngine->Utils().LogIt(_L("Warning: Not Connected\n"));
		} break;
	case THTTPSessionEvent::EExceptionInfo:
		{
		iEngine->Utils().LogIt(_L("Warning: Exception Info\n"));
		} break;
	case THTTPSessionEvent::EAlreadyConnecting:
		{
		iEngine->Utils().LogIt(_L("Warning: Already Connecting\n"));
		} break;
	case THTTPSessionEvent::EAlreadyConnected:
		{
		iEngine->Utils().LogIt(_L("Warning: Already Connected\n"));
		} break;
	case THTTPSessionEvent::EAlreadyDisconnecting:
		{
		iEngine->Utils().LogIt(_L("Warning: Already Disconnecting\n"));
		} break;
	case THTTPSessionEvent::EAlreadyDisconnected:
		{
		iEngine->Utils().LogIt(_L("Warning: Already Disconnected\n"));
		} break;
	default:
		CActiveScheduler::Stop();
		break;
		}
	}


TInt CWspTestTransactions::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}


void CWspTestTransactions::SetWspSessionPropertiesL()
	{
	// Iterate through the TestCases.ini file, beginning at the first element
	// i.e. index 0, and set up the WSP proxy properties and the
	// WSP capability properties
	SetWspProxyPropertiesL(0);
	SetWspCapabilityPropertiesL(0);
	}


void CWspTestTransactions::SetWspProxyPropertiesL(TInt aStartItem)
	{
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
	TInt endItem = FindEndOfTransaction(aStartItem);	
	for (TInt ii = aStartItem; ii < endItem; ++ii)
		{
		TPtrC itemPtr(iIniFile->Section(iSectionNumber).Item(ii).Item());
		HBufC8* itemBuff8 = HBufC8::NewLC(itemPtr.Length());
		TPtr8  itemPtr8 = itemBuff8->Des();
		itemPtr8.Copy(itemPtr);
		RStringF item = iMyStrP.OpenFStringL(itemPtr8);
		CleanupClosePushL(item);
		

		TPtrC valuePtr(iIniFile->Section(iSectionNumber).Item(ii).Value());

		// Retrieve the actual value from the settings.ini file for
		// KItemWspProxyAddress ("proxy_address").  Other values like
		// uri value are retrieved in trans.cpp file.
		
		// Replace the host name in the URI
		HBufC16* valueBuff16 = TSrvAddrVal::ReplaceHostNameL(valuePtr, iIniSettingsFile);
		CleanupStack::PushL(valueBuff16);
		TPtr16 valuePtr16 = valueBuff16->Des();

		// Convert 16 bit to 8 bit.
		HBufC8* valueBuff8 = HBufC8::NewLC(valuePtr16.Length());
		TPtr8 valuePtr8 = valueBuff8->Des();
		valuePtr8.Copy(valuePtr16);

		RStringF value = iMyStrP.OpenFStringL(valuePtr8);
		CleanupClosePushL(value);

		if (itemPtr.CompareF(KItemWspProxyAddress)==0)
			{
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspProxyAddress, RHTTPSession::GetTable()), value);
			}
		if (itemPtr.Compare(KItemWspBearer)==0)
			{
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspBearer, RHTTPSession::GetTable()), value);
			}
		if (itemPtr.Compare(KItemWspLocalPort)==0)
			{
			TLex lex(valuePtr);
			TInt localPort = 0;
			lex.Val(localPort);
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspLocalPort, RHTTPSession::GetTable()), localPort);
			}
		if (itemPtr.Compare(KItemWspRemotePort)==0)
			{
			TLex lex(valuePtr);
			TInt remotePort = 0;
			lex.Val(remotePort);
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspRemotePort, RHTTPSession::GetTable()), remotePort);
			}
		if (itemPtr.Compare(KItemWspServiceNumber)==0)
			{
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspServiceNumber, RHTTPSession::GetTable()), value);
			}
		if (itemPtr.Compare(KItemWspSessionType)==0)
			{
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspSessionType, RHTTPSession::GetTable()), value);
			}
		if (itemPtr.Compare(KItemWspSecurity)==0)
			{
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspSecurity, RHTTPSession::GetTable()), value);
			}

		CleanupStack::PopAndDestroy(5, itemBuff8); // item, valueBuff8, value, valueBuff16
		}
	}

void CWspTestTransactions::SetWspCapabilityPropertiesL(TInt aStartItem)
	{
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
	TInt endItem = FindEndOfTransaction(aStartItem);
	for (TInt ii = aStartItem; ii < endItem; ++ii)
		{
		TPtrC itemPtr(iIniFile->Section(iSectionNumber).Item(ii).Item());
		HBufC8* itemBuff8 = HBufC8::NewLC(itemPtr.Length());
		TPtr8  itemPtr8= itemBuff8->Des();
		itemPtr8.Copy(itemPtr);
		RStringF item = iMyStrP.OpenFStringL(itemPtr8);
		CleanupClosePushL(item);
		

		TPtrC valuePtr(iIniFile->Section(iSectionNumber).Item(ii).Value());
		HBufC8* valueBuff8 = HBufC8::NewLC(valuePtr.Length());
		TPtr8 valuePtr8= valueBuff8->Des();
		valuePtr8.Copy(valuePtr);
		RStringF value = iMyStrP.OpenFStringL(valuePtr8);
		CleanupClosePushL(value);

		if (itemPtr.CompareF(KItemWspClientMsgSize)==0)
			{
			TLex lex(valuePtr);
			TInt cliMsgSize = 0;
			lex.Val(cliMsgSize);
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspCapClientMessageSize, RHTTPSession::GetTable()), cliMsgSize);
			}
		if (itemPtr.CompareF(KItemWspServerMsgSize)==0)
			{
			TLex lex(valuePtr);
			TInt srvMsgSize = 0;
			lex.Val(srvMsgSize);
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspCapServerMessageSize, RHTTPSession::GetTable()), srvMsgSize);
			}
		if (itemPtr.CompareF(KItemWspUseAcknowledgements)==0)
			{
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspCapUseAcknowledgements, RHTTPSession::GetTable()), value);
			}
		if (itemPtr.CompareF(KItemWspSupportSuspendResume)==0)
			{
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspCapSupportSuspendResume, RHTTPSession::GetTable()), value);
			}
		if (itemPtr.CompareF(KItemWspMaxOutstandingRequests)==0)
			{
			TLex lex(valuePtr);
			TInt maxOutReqs = 0;
			lex.Val(maxOutReqs);
			connInfo.SetPropertyL(iMyStrP.StringF(HTTP::EWspCapMaxOutstandingRequests, RHTTPSession::GetTable()), maxOutReqs);
			}
		if (itemPtr.CompareF(KItemExpectedError)==0)
			{
			//get the 'test_validation_param_expectederror' from the ini 
			//file...
			TPtrC expectedParamPtr = iIniFile->Section(iSectionNumber).ItemL(KItemExpectedError).Value();
			TLex paramLex(expectedParamPtr);
			TInt param;
			paramLex.Val(param);
			// ...and put it  into iExpectedError
			iExpectedError = param;
			}

		CleanupStack::PopAndDestroy(4, itemBuff8); // itemBuff8, item, 
												   // valueBuff8, value
		}
	}

void CWspTestTransactions::SetWspSecurityPolicyPropertiesL(TInt aStartItem)
	{
	// Create the security policy plugin ini file
	RFs fs;
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());

	TParse fileName;
	User::LeaveIfError(iEngine->Utils().ResolveFile(KSecurityPolicy, KSecurityPolicyFileName, fileName));

	RFile file;
	CleanupClosePushL(file);
	User::LeaveIfError(file.Replace(fs, fileName.FullName(), 
		EFileWrite|EFileShareExclusive)); 

	// Write the section name
	file.Write(_L8("\r\n[Security Policy]\r\n"));

	TInt endItem = FindEndOfTransaction(aStartItem);
	for (TInt ii = aStartItem; ii < endItem; ++ii)
		{
		TPtrC itemPtr(iIniFile->Section(iSectionNumber).Item(ii).Item());
		HBufC8* itemBuff8 = HBufC8::NewLC(itemPtr.Length());
		TPtr8  itemPtr8 = itemBuff8->Des();
		itemPtr8.Copy(itemPtr);
		
		TPtrC valuePtr(iIniFile->Section(iSectionNumber).Item(ii).Value());
		HBufC8* valueBuff8 = HBufC8::NewLC(valuePtr.Length());
		TPtr8 valuePtr8 = valueBuff8->Des();
		valuePtr8.Copy(valuePtr);

		if( itemPtr.CompareF(KValidateUntrustedServerCert) == 0 ||
			itemPtr.CompareF(KWTLSCipherSuites) == 0||
			itemPtr.CompareF(KWTLSKeyExchangeSuites) == 0 ||
			itemPtr.CompareF(KTLSCipherSuites) == 0||
			itemPtr.Find(KPolicy) == 0) // matches 'Policy1', 'Policy2', etc.
			{
			// Write out entry to the securitypolicy.ini file
			file.Write(itemPtr8);
			file.Write(_L8("= "));
			file.Write(valuePtr8);
			file.Write(_L8("\r\n"));
			}

		CleanupStack::PopAndDestroy(2, itemBuff8); // itemBuff8, valueBuff8
		}
	// Write the end of script marked in security ini
	file.Write(_L8("\r\nendscript\r\n"));

	// Cleanup
	CleanupStack::PopAndDestroy(2, &fs); // fs, file
	}
