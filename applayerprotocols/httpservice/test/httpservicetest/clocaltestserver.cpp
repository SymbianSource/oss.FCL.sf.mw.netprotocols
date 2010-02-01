// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <chttptransportlayer.h>

#include "clocaltestserver.h"
#include "httptestutils.h"
#include "mlocaltestserverobserver.h"
#include "ctestserverstreammanager.h"
#include "chttpclienttestparams.h"

const TUint16 KListeningPort = 80;  
const TInt KDefaultBufferSize = 16 * 1024;

CLocalTestServer* CLocalTestServer::NewL(CHTTPTestUtils& aTestUtils, MLocalTestServerObserver& aObserver, CHttpClientTestParamArray& aTestParamArray)
	{
	CLocalTestServer* self = new (ELeave) CLocalTestServer(aTestUtils, aObserver, aTestParamArray);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CLocalTestServer::CLocalTestServer(CHTTPTestUtils& aTestUtils, MLocalTestServerObserver& aObserver, CHttpClientTestParamArray& aTestParamArray)
: iTestUtils(aTestUtils), iObserver(aObserver), iTestParamArray(aTestParamArray)
	{
	}
	
void CLocalTestServer::ConstructL()
	{
	_LIT8(KTxtTCP, "TCP");
	THttpTransportConstructionParams params(*this); 
	iTCPLayer = CHttpTransportLayer::NewL(KTxtTCP(), params);	
	}

CLocalTestServer::~CLocalTestServer()
	{
	iStreamManagers.ResetAndDestroy();
	iStreamManagers.Close();
	StopServer();
	delete iTCPLayer;
	}

void CLocalTestServer::StartServerL()
	{
	iTCPLayer->ListenL(*this, KListeningPort);
	}

void CLocalTestServer::StopServer()
	{
	if( iTCPLayer != NULL )
		{
		iTCPLayer->StopListen();
		}
	}

void CLocalTestServer::ResetServer()
	{
	iStreamManagers.ResetAndDestroy();
	}
	
// From MSocketListenObserver
void CLocalTestServer::ConnectionReceivedL(MInputStream& aInputStream, MOutputStream& aOutputStream)
	{
	// New connection received, create a new stream manager to manage the requests and responses
/*	const TInt connectionCount = iStreamManager.Count();
*/	
	CLocalTestServerStreamManager* newStreamMgr = CLocalTestServerStreamManager::NewL(iTestUtils, &aInputStream, &aOutputStream, iTestParamArray);
	CleanupStack::PushL(newStreamMgr);
	User::LeaveIfError(iStreamManagers.Append(newStreamMgr));
	CleanupStack::Pop(newStreamMgr);

	_LIT(KTxtConnection, "Server - Client connection received and connected.");
	iTestUtils.LogIt(KTxtConnection());
	}
	
TInt CLocalTestServer::HandleListenError(TInt aError)
	{
	_LIT(KTxtListenError, "Server listening error: Error code = %d");
	iTestUtils.LogIt(KTxtListenError(), aError);

	iObserver.EndTest(aError);
	return KErrNone;
	}
	
void CLocalTestServer::MSocketListenObserver_Reserved()
	{
	}

// From MConnectionPrefsProvider
TBool CLocalTestServer::SupplyCommsConnection(RConnection*& /* aConnectionPtr */)
	{	
	return EFalse;
	}
	

void CLocalTestServer::SetCommsConnectionL(RConnection* /* aConnectionPtr */)
	{
	//do nothing
	}
	
void CLocalTestServer::GetSecurityPrefs(TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy)
	{
	aDialogPrompt = EFalse;
	aSecurityPolicy = NULL;
	}

TBool CLocalTestServer::ImmediateSocketShutdown()
	{
	return EFalse;
	}

TInt CLocalTestServer::SessionId()
	{
	return KErrNotSupported;
	}

TBool CLocalTestServer::SupplySocketServerHandle ( TInt& aSocketServerHandle )
  	{
  	aSocketServerHandle = 0;
  	return EFalse;
  	}
  	
void CLocalTestServer::SetSocketServerHandleL ( TInt /*aSocketServerHandle*/ )	
  	{	
  	// do nothing
  	}

TInt CLocalTestServer::GetRecvBufferSize()
	{
	return KDefaultBufferSize;
	}

