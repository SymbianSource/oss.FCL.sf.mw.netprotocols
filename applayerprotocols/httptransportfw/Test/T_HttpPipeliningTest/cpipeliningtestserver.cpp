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

#include "cpipeliningtestserver.h"
#include "httptestutils.h"
#include "MPipeliningTestCase.h"
#include "MPipeliningTestObserver.h"
#include "CTestServerStreamManager.h"

const TUint16 KListeningPort = 80;

CPipeliningTestServer* CPipeliningTestServer::NewL(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver)
	{
	CPipeliningTestServer* self = new (ELeave) CPipeliningTestServer(aTestUtils, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CPipeliningTestServer::CPipeliningTestServer(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver)
: iTestUtils(aTestUtils), iObserver(aObserver)
	{
	}
	
void CPipeliningTestServer::ConstructL()
	{
	_LIT8(KTxtTCP, "TCP");
	THttpTransportConstructionParams params(*this); 
	params.iPriority = EFalse;
	iTCPLayer = CHttpTransportLayer::NewL(KTxtTCP(), params);
	}

CPipeliningTestServer::~CPipeliningTestServer()
	{
	StopServer();
	iStreamManager.ResetAndDestroy();
	iStreamManager.Close();
	delete iTCPLayer;
	}

void CPipeliningTestServer::StartServerL()
	{
	iTCPLayer->ListenL(*this, KListeningPort);
	}

void CPipeliningTestServer::StopServer()
	{
	if( iTCPLayer!=NULL )
		iTCPLayer->StopListen();
	}

void CPipeliningTestServer::ResetServer()
	{
	iStreamManager.ResetAndDestroy();
	}
	
void CPipeliningTestServer::SetTestCase(MPipeliningTestCase* aTestCase)
	{
	iTestCase = aTestCase;
	
	_LIT(KTxtTransCount, "Total transaction in test case = %d");
	iTestUtils.LogIt(KTxtTransCount(), iTestCase->TotalTransactionCount());

	_LIT(KTxtConnectionCount, "Total connections in test case = %d");
	iTestUtils.LogIt(KTxtConnectionCount(), iTestCase->ConnectionCount());
	}

TInt CPipeliningTestServer::ConnectionCount() const
	{
	return iStreamManager.Count();
	}
	
// From MSocketListenObserver
void CPipeliningTestServer::ConnectionReceivedL(MInputStream& aInputStream, MOutputStream& aOutputStream)
	{
	// New connection received, create a new stream manager to manage the requests and responses
	const TInt connectionCount = iStreamManager.Count();
	
	CTestServerStreamManager* newStreamMgr = CTestServerStreamManager::NewL(iTestUtils, connectionCount, iTestCase, &aInputStream, &aOutputStream);
	CleanupStack::PushL(newStreamMgr);
	User::LeaveIfError(iStreamManager.Append(newStreamMgr));
	CleanupStack::Pop(newStreamMgr);

	_LIT(KTxtConnection, "Server - Client connection received and connected.");
	iTestUtils.LogIt(KTxtConnection());
	}
	
TInt CPipeliningTestServer::HandleListenError(TInt aError)
	{
	_LIT(KTxtListenError, "Server listening error: Error code = %d");
	iTestUtils.LogIt(KTxtListenError(), aError);

	iObserver.EndTest(aError);
	return KErrNone;
	}
	
void CPipeliningTestServer::MSocketListenObserver_Reserved()
	{
	}

// From MConnectionPrefsProvider
TBool CPipeliningTestServer::SupplyCommsConnection(RConnection*& /*aConnectionPtr*/)
	{
	return EFalse;
	}
	
void CPipeliningTestServer::SetCommsConnectionL(RConnection* /*aConnectionPtr*/)
	{
	// Do nothing
	}
	
void CPipeliningTestServer::GetSecurityPrefs(TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy)
	{
	aDialogPrompt = EFalse;
	aSecurityPolicy = NULL;
	}

TBool CPipeliningTestServer::ImmediateSocketShutdown()
	{
	return EFalse;
	}

TInt CPipeliningTestServer::SessionId()
	{
	return KErrNotSupported;
	}

TBool CPipeliningTestServer::SupplySocketServerHandle ( TInt& /*aSocketServerHandle*/ )
 	{
 	return 0;
 	}
 	
void CPipeliningTestServer::SetSocketServerHandleL ( TInt /*aSocketServerHandle*/ )	
 	{	
 	// do nothing
 	}

TInt CPipeliningTestServer::GetRecvBufferSize()
	{
	return KDefaultBufferSize;	
	}

