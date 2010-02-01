// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CLOCALTESTSERVER_H__
#define __CLOCALTESTSERVER_H__

#include <e32base.h>
#include <mconnectionprefsprovider.h>
#include <msocketlistenobserver.h>

  
class CHttpTransportLayer;
class MInputStream;
class MOutputStream;
class CHTTPTestUtils;
class MLocalTestServerObserver;
class CLocalTestServerStreamManager;
class CHttpClientTestParamArray;


class CLocalTestServer : public CBase,
							  public MConnectionPrefsProvider,
							  public MSocketListenObserver
	{
public:
	static CLocalTestServer* NewL(CHTTPTestUtils& aTestUtils, MLocalTestServerObserver& aObserver, CHttpClientTestParamArray& aTestParamArray);
	~CLocalTestServer();

	void StartServerL();
	void StopServer();
	void ResetServer();
	
private:
	CLocalTestServer(CHTTPTestUtils& aTestUtils, MLocalTestServerObserver& aObserver, CHttpClientTestParamArray& aTestParamArray);
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
	CHTTPTestUtils& 					iTestUtils;
	MLocalTestServerObserver&	iObserver;
	CHttpTransportLayer*			iTCPLayer;
	RPointerArray<CLocalTestServerStreamManager> iStreamManagers;
	CHttpClientTestParamArray&         iTestParamArray;	
	};
	
#endif // __CLOCALTESTSERVER_H__

