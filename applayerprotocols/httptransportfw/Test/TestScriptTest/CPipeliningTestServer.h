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

#ifndef __CPIPELININGTESTSERVER_H__
#define __CPIPELININGTESTSERVER_H__

#include <e32base.h>
#include <mconnectionprefsprovider.h>
#include <msocketlistenobserver.h>

class CHttpTransportLayer;
class MInputStream;
class MOutputStream;
class CHTTPTestUtils;
class MPipeliningTestCase;
class MPipeliningTestObserver;
class CTestServerStreamManager;

const TInt KDefaultBufferSize		= 6*1024;

class CPipeliningTestServer : public CBase,
							  public MConnectionPrefsProvider,
							  public MSocketListenObserver
	{
public:
	static CPipeliningTestServer* NewL(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver);
	~CPipeliningTestServer();

	void StartServerL();
	void StopServer();
	void ResetServer();
	void SetTestCase(MPipeliningTestCase* aTestCase);
	TInt ConnectionCount() const;
	
private:
	CPipeliningTestServer(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver);
	void ConstructL();
	
	// From MSocketListenObserver
	void ConnectionReceivedL(MInputStream& aInputStream, MOutputStream& aOutputStream);
	TInt HandleListenError(TInt aError);
	void MSocketListenObserver_Reserved();
	
	// From MConnectionPrefsProvider
	TBool SupplyCommsConnection(RConnection*& aConnectionPtr);
 	void SetCommsConnectionL(RConnection* aConnectionPtr);
	void GetSecurityPrefs(TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy);
	TBool ImmediateSocketShutdown();
	virtual TBool SupplySocketServerHandle ( TInt& aSocketServerHandle );
  	virtual void SetSocketServerHandleL ( TInt aSocketServerHandle );
	TInt SessionId();
	TInt GetRecvBufferSize();

private:
	CHTTPTestUtils& 			iTestUtils;
	MPipeliningTestObserver&	iObserver;
	CHttpTransportLayer*		iTCPLayer;
	MPipeliningTestCase*		iTestCase;		// Not owned
	RPointerArray<CTestServerStreamManager>	iStreamManager;
	};
	
#endif // __CPIPELININGTESTSERVER_H__

