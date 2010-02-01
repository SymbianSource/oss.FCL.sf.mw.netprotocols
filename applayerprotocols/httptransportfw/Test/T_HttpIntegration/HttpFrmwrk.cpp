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
// $Header$
// This module implements the HTTP Framework clas
// by:		irees, December 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Files  
// 
//

#include <commdbconnpref.h>
#include "CHTTPFamily.h"
#include "HttpFrmwrk.h"
#include "CWspEventDispatcher.h"

//-----------------------------------------------------------------------------

_LIT(KITHIAP, "IAP");				//	Integration Test Harness $$ var name

// Proxy information properties
_LIT(KITHWspProxyAddress, "ProxyAddress");
_LIT(KITHWspBearer,"Bearer");
_LIT(KITHWspLocalPort, "LocalPort");
_LIT(KITHWspRemotePort, "RemotePort");
_LIT(KITHWspServiceNumber, "ServiceNumber");
_LIT(KITHWspSessionType, "SessionType");
_LIT(KITHWspSecurity, "Security");

_LIT(KITHWspBearerSMS, "SMS");
_LIT(KITHWspSessionTypeCL, "ConnectionLess");

_LIT(KITHWspSecurityUseWTLS, "UseWTLS");

// Capability properties
_LIT(KITHWspClientMsgSize, "ClientMsgSize");
_LIT(KITHWspServerMsgSize, "ServerMsgSize");
_LIT(KITHWspUseAcknowledgements, "UseAcknowldegements");
_LIT(KITHWspSupportSuspendResume, "SupportSuspendResume");
_LIT(KITHWspMaxOutstandingRequests, "MaxOutstandingRq");

CFrmwrkSession *CFrmwrkSession::NewL(const TDesC& aName,
									 const TDesC& aFramework, 
									 CTEngine *aTestMachine)
	{
	CFrmwrkSession *self = NewLC( aName, aFramework, aTestMachine );
	CleanupStack::Pop();
	return self; 
	}

//-----------------------------------------------------------------------------

CFrmwrkSession *CFrmwrkSession::NewLC(const TDesC& aName,
									  const TDesC& aFramework, 
									  CTEngine *aTestMachine)
	{
	CFrmwrkSession *self = new (ELeave) CFrmwrkSession();
	CleanupStack::PushL(self);
	self->ConstructL(aName, aFramework, aTestMachine);
	return self;
	}

//-----------------------------------------------------------------------------

void CFrmwrkSession::ConstructL(const TDesC& aName,
								const TDesC& aFramework,
								CTEngine *aTestMachine)
	{
	iEngine = aTestMachine ;

	iName = aName.AllocLC();
	CleanupStack::Pop();
	
	iState = ENormal;
	iuseWSPProtocol = (aFramework.CompareF(THA_KTxtFrameworkWSP) == 0);

	}

//-----------------------------------------------------------------------------

CFrmwrkSession::~CFrmwrkSession(void)
	{
	delete iName;
	}

void CFrmwrkSession::Log(TRefByValue<const TDesC> aFmt, ... )
	{
	VA_LIST list;
	VA_START(list, aFmt);

	Machine()->MsgPrintfln(aFmt, list);
	}

TInt CFrmwrkSession::OpenL()
	{
	if (iuseWSPProtocol)
		{
		iSession.OpenL(_L8("WSP/WSP"));
		}
	else
	{
		iSession.OpenL();
	}

	iSession.SetSessionEventCallback(this);

	return 0 ;
	}

TInt CFrmwrkSession::Close()
	{
	iSession.Close(); 
	iConnection.Close() ;

	return 0 ;
	}


TInt CFrmwrkSession::DisconnectL()
	{
	TPtrC myName = Name() ;
	
	Log(_L("Disconnecting %S"), &myName);

	iState = EWSPDisconnecting;
	iSession.DisconnectL();
	
	return 0 ;
	}


//-----------------------------------------------------------------------------

void CFrmwrkSession::SetWspProxyPropertiesL(RHTTPSession & aSession, RStringPool &aStrPool)
	{
	RHTTPConnectionInfo connInfo = aSession.ConnectionInfo();
	TPtrC valuePtr; 

	valuePtr.Set (Machine()->GetDefine(KITHWspProxyAddress));
	if (valuePtr.Length() != 0)
		{
		HBufC8* valueBuff8 = HBufC8::NewLC(valuePtr.Length());
		TPtr8 valuePtr8 = valueBuff8->Des();
		valuePtr8.Copy(valuePtr);
		RStringF value = aStrPool.OpenFStringL(valuePtr8);
		CleanupClosePushL(value);

		connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspProxyAddress, RHTTPSession::GetTable()), value);
		CleanupStack::PopAndDestroy(2); 
		}

	valuePtr.Set(Machine()->GetDefine(KITHWspBearer));
	if (valuePtr.Length() != 0)
		{
		RStringF bearer;
		
		if (valuePtr.CompareF(KITHWspBearerSMS)==0)
			bearer = aStrPool.StringF(HTTP::EWspSMS, RHTTPSession::GetTable());
		else
			bearer = aStrPool.StringF(HTTP::EWspIp, RHTTPSession::GetTable());
		
		connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspBearer, RHTTPSession::GetTable()), bearer);
		}

	valuePtr.Set(Machine()->GetDefine(KITHWspLocalPort));

	if (valuePtr.Length() != 0)
		{
		TLex lex(valuePtr);
		TInt localPort = 0;
		lex.Val(localPort);

		connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspLocalPort, RHTTPSession::GetTable()), localPort);
		}

	valuePtr.Set(Machine()->GetDefine(KITHWspRemotePort));
	if (valuePtr.Length() != 0)
		{
		TLex lex(valuePtr);
		TInt remotePort = 0;
		lex.Val(remotePort);

		connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspRemotePort, RHTTPSession::GetTable()), remotePort);
		}

	valuePtr.Set(Machine()->GetDefine(KITHWspServiceNumber));
	if (valuePtr.Length() != 0)
		{
		HBufC8* valueBuff8 = HBufC8::NewLC(valuePtr.Length());
		TPtr8 valuePtr8 = valueBuff8->Des();
		valuePtr8.Copy(valuePtr);
		RStringF value = aStrPool.OpenFStringL(valuePtr8);
		CleanupClosePushL(value);

		connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspServiceNumber, RHTTPSession::GetTable()), value);
		CleanupStack::PopAndDestroy(2); 
		}
	
	valuePtr.Set(Machine()->GetDefine(KITHWspSessionType));
	if (valuePtr.Length() != 0)
		{
		RStringF sessionType;
		
		if (valuePtr.CompareF(KITHWspSessionTypeCL)==0)
			sessionType = aStrPool.StringF(HTTP::EWspConnectionLess, RHTTPSession::GetTable());
		else
			sessionType = aStrPool.StringF(HTTP::EWspConnectionOriented, RHTTPSession::GetTable());

		connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspSessionType, RHTTPSession::GetTable()), sessionType);
		}

	valuePtr.Set(Machine()->GetDefine(KITHWspSecurity));
	if (valuePtr.Length() != 0)
		{
		RStringF security;
		
		if (valuePtr.CompareF(KITHWspSecurityUseWTLS)==0)
			security = aStrPool.StringF(HTTP::EWspUseWtls, RHTTPSession::GetTable());
		else
			security = aStrPool.StringF(HTTP::EWspDoNotUseWtls, RHTTPSession::GetTable());

		connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspSecurity, RHTTPSession::GetTable()), security);
		}
}

void CFrmwrkSession::SetWspCapabilityPropertiesL(RHTTPSession & aSession, RStringPool &aStrPool)
	{
	RHTTPConnectionInfo connInfo = aSession.ConnectionInfo();
	TPtrC valuePtr; 

	valuePtr.Set (Machine()->GetDefine(KITHWspClientMsgSize));
	if (valuePtr.Length() != 0)
		{
		TLex lex(valuePtr);
		TInt cliMsgSize = 0;
		lex.Val(cliMsgSize);

		connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspCapClientMessageSize, RHTTPSession::GetTable()), cliMsgSize);
		}

	valuePtr.Set (Machine()->GetDefine(KITHWspServerMsgSize));
	if (valuePtr.Length() != 0)
		{
		TLex lex(valuePtr);
		TInt srvMsgSize = 0;
		lex.Val(srvMsgSize);

		connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspCapServerMessageSize, RHTTPSession::GetTable()), srvMsgSize);
		}

	valuePtr.Set(Machine()->GetDefine(KITHWspUseAcknowledgements));
	if (valuePtr.Length() != 0)
		{
		if (valuePtr.CompareF(KTxtTrue)==0)
			connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspCapUseAcknowledgements, RHTTPSession::GetTable()), THTTPHdrVal(1));
		}
	
	valuePtr.Set(Machine()->GetDefine(KITHWspSupportSuspendResume));
	if (valuePtr.Length() != 0)
		{
		if (valuePtr.CompareF(KTxtTrue)==0)
			connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspCapSupportSuspendResume, RHTTPSession::GetTable()), THTTPHdrVal(1));
		}

	valuePtr.Set(Machine()->GetDefine(KITHWspMaxOutstandingRequests));
	if (valuePtr.Length() != 0)
		{
		TLex lex(valuePtr);
		TInt maxOutReqs = 0;
		lex.Val(maxOutReqs);

		connInfo.SetPropertyL(aStrPool.StringF(HTTP::EWspCapMaxOutstandingRequests, RHTTPSession::GetTable()), maxOutReqs);
		}

	}

TPtrC CFrmwrkSession::Name() const
	{
	TPtrC name(iName->Des());
	return name;
	}

TInt CFrmwrkSession::SetPropertiesL()
	{	
	if (iuseWSPProtocol)
		{
		RStringPool strPool = iSession.StringPool();

		SetWspProxyPropertiesL(iSession, strPool);

		SetWspCapabilityPropertiesL(iSession, strPool);
		}
	
	return 0 ;
	}

TInt CFrmwrkSession::ConnectL()
	{

	iState = EWSPConnecting;
	iSession.ConnectL();

	TBuf8<16> bufCommDbId;
	TInt commDbId = 0 ;

	bufCommDbId.Copy(iEngine->GetDefine(KITHIAP));

	if (bufCommDbId.Length())
		{	
		TLex8 lexer(bufCommDbId);
	
		if ((lexer.Val(commDbId) != KErrNone) || (commDbId < 0) || (commDbId > 8))
			{
			Log(THA_KFmtErrIAPVal, &bufCommDbId);
			return (KErrArgument) ;
			}
		}

	if (commDbId)
		{
		RSocketServ sock;

		User::LeaveIfError(sock.Connect());
		
		TCommDbConnPref prefs1;

		prefs1.SetIapId(commDbId);
			
		//Start Connection1

		User::LeaveIfError(iConnection.Open(sock));

		User::LeaveIfError(iConnection.Start(prefs1));

		//Set properties for mySession
		RStringPool strPool = iSession.StringPool();
		RHTTPConnectionInfo connInfo1 = iSession.ConnectionInfo();

		connInfo1.SetPropertyL ( strPool.StringF(HTTP::EHttpSocketServ, 
								 RHTTPSession::GetTable() ), 
								 THTTPHdrVal (sock.Handle()) );

		TInt connPtr = REINTERPRET_CAST(TInt, &iConnection);
		
		connInfo1.SetPropertyL ( strPool.StringF(HTTP::EHttpSocketConnection, 
								 RHTTPSession::GetTable() ), 
								 THTTPHdrVal (connPtr) );
		}

	InstallAuthenticationL(iSession);

	return 0 ;
	}

//-----------------------------------------------------------------------------
//	Called when a authenticated page is requested
//	Asks the user for a username and password that would be appropriate for the url that was
//	supplied.

TBool CFrmwrkSession::GetCredentialsL(	const TUriC8& aURI, 
										RString aRealm, 
										RStringF aAuthenticationType,
										RString& aUsername, 
										RString& aPassword)

	{
	TBuf<80> scratch;
	TBuf8<80> scratch8;

	scratch8.Format(_L8("Enter credentials for URL %S, realm %S"), &aURI.UriDes(), &aRealm.DesC());
	scratch.Copy(scratch8);

	Log(_L("%S"), &scratch);
	scratch.Copy(aAuthenticationType.DesC());
	Log(_L("Using %S authentication"), &scratch);
	Machine()->GetUserInput(_L("Username (or QUIT to give up): "), scratch);
	scratch8.Copy(scratch);
	if (scratch8.CompareF(_L8("quit")))
		{
		TRAPD(err, aUsername = aRealm.Pool().OpenStringL(scratch8));
		if (!err)
			{
			Machine()->GetUserInput(_L("Password: "), scratch);
			scratch8.Copy(scratch);
			TRAP(err, aPassword = aRealm.Pool().OpenStringL(scratch8));
			if (!err)
				return ETrue;
			}
		}

	return EFalse;
	}
	
void CFrmwrkSession::MHFSessionRunL(const THTTPSessionEvent& aEvent)
	{
	TPtrC name = Name() ;

	switch (aEvent.iStatus)
		{
		case THTTPSessionEvent::EConnect:
			{
			Log(_L("CFrmwrkSession::MHFSessionRunL, event EConnect, Session %S"),
				&name);
			} break;
		case THTTPSessionEvent::EConnectedOK:
			{
			Log(_L("CFrmwrkSession::MHFSessionRunL, The session has been connected, , Session %S"),
				&name);
			} break;
		case THTTPSessionEvent::EDisconnect:
			{
			Log(_L("CFrmwrkSession::MHFSessionRunL, EDisconnect - shouldnt get this because it is an outgoing event, Session %S"),
				&name);
			} break;
		case THTTPSessionEvent::EConnectedWithReducedCapabilities:
			{
			Log(_L("CFrmwrkSession::MHFSessionRunL, Session connected with reduced capabilities, , Session %S"),
				&name);
			} break;
		case THTTPSessionEvent::EDisconnected:
			{
			iState = EWSPDisconnected;
			Log(_L("CFrmwrkSession::MHFSessionRunL, Session %S has been disconnected"), 
				&name);
			} 
			break;
		case THTTPSessionEvent::EAuthenticatedOK:
			{
			Log(_L("CFrmwrkSession::MHFSessionRunL, the authentication handshake succeeded, Session %S"),
				&name);
			} break;
		case THTTPSessionEvent::EAuthenticationFailure:
			{
			Log(_L("CFrmwrkSession::MHFSessionRunL, the authentication handshake failed, Session %S"),
				&name);
			} break;
		case THTTPSessionEvent::EConnectionTimedOut:
			{
			iState = EWSPDisconnected;
			Log(_L("CFrmwrkSession::MHFSessionRunL, the connection attempt to the proxy timed out, Session %S"),
				&name);
			} break;

		case THTTPSessionEvent::ENotConnected:
			{
			iState = EWSPDisconnected;
			Log(_L("CFrmwrkSession::MHFSessionRunL, ENotConnected, Session %S"),
				&name);
			}
			break;

		case THTTPSessionEvent::EExceptionInfo: 
			{
			iState = EWSPDisconnected;
			Log(_L("CFrmwrkSession::MHFSessionRunL, EExceptionInfo, Session %S"),
				&name);
			}
			break;

		case THTTPSessionEvent::ERedirected:
			{
			iState = EWSPDisconnected;
			Log(_L("CFrmwrkSession::MHFSessionRunL, ERedirected, Session %S"),
				&name);
			} 
			break;

		case THTTPSessionEvent::EAlreadyConnecting:
			{
			iState = EWSPDisconnected;
			Log(_L("CFrmwrkSession::MHFSessionRunL, EAlreadyConnecting, Session %S"),
				&name);
			} 
			break;

		case THTTPSessionEvent::EAlreadyConnected:
			{
			iState = EWSPDisconnected;
			Log(_L("CFrmwrkSession::MHFSessionRunL, EAlreadyConnected, Session %S"),
				&name);
			} 
			break;

		case THTTPSessionEvent::EAlreadyDisconnecting:
			{
			iState = EWSPDisconnected;
			Log(_L("CFrmwrkSession::MHFSessionRunL, EAlreadyDisconnecting, Session %S"),
				&name);
			} 
			break;

		case THTTPSessionEvent::EAlreadyDisconnected:
			{
			iState = EWSPDisconnected;
			Log(_L("CFrmwrkSession::MHFSessionRunL, EAlreadyDisconnected, Session %S"),
				&name);
			} 
			break;

		default:
			{
			iState = EWSPDisconnected;
		    Log(_L("CFrmwrkSession::MHFSessionRunL, Unrecognised session event [%d], Session %S[%d]"), 
				aEvent.iStatus, &name);
			}
			break;
		}

	TRequestStatus* pStat = &iEventDispatcher->iStatus;

	User::RequestComplete(pStat,aEvent.iStatus);
}

TInt CFrmwrkSession::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	iEngine->LogPrintf(_L("CFrmwrkSession::MHFSessionRunError, error code %d"), aError);

	TRequestStatus* pStat = &iEventDispatcher->iStatus;
	User::RequestComplete(pStat, KErrNone);

	return KErrNone;
	}
