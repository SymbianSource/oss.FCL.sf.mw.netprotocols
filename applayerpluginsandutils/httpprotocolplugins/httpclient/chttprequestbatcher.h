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

#ifndef __CHTTPREQUESTBATCHER_H__
#define __CHTTPREQUESTBATCHER_H__

#include <e32base.h>
#include <http/framework/logging.h>

#include "moutputstream.h"
#include "moutputstreamobserver.h"

class CX509Certificate;


class CHttpRequestBatcher : public CActive,
							public MOutputStream,
							public MOutputStreamObserver
	{
public:		// methods
	static CHttpRequestBatcher* NewL(MOutputStream& aOutputStream, const TInt aBufferSize);

	virtual ~CHttpRequestBatcher();

	virtual void Bind(MOutputStreamObserver& aObserver);


private:	// methods from MOutputStream
	virtual void SendDataReqL(const TDesC8& aBuffer);

	virtual void ShutdownReq();

	virtual void SecureClientReq(const TDesC8& aHostName);

	virtual void Close();

	virtual const CX509Certificate* ServerCert();
	
	virtual TInt CipherSuite(TDes8& aCipherSuite);

	virtual void MOutputStream_Reserved();

	virtual void Reset ();
	
	TBool SecureRetry();
	
	virtual void SetTCPCorking(TBool aValue);
private:	// methods from MOutputStreamObserver
	virtual void SendDataCnfL();

	virtual void SecureClientCnf();

	virtual void OutputStreamCloseInd(TInt aError);

	virtual void MOutputStreamObserver_Reserved();
	
	virtual void OnSendTimeOut();
	
	virtual TInt SendTimeOutVal();
	
private:	// methods from CActive
	virtual void RunL();
	
	virtual void DoCancel();
	
	virtual TInt RunError(TInt aError);


private:	// methods
	CHttpRequestBatcher(MOutputStream& aOutputStream, const TInt aBufferSize);

	void SendRequestImmediatelyL(const TDesC8& aBuffer);

	void SendRequestsBatchedL(const TDesC8& aBuffer);

	void BatchRequestsL(const TDesC8& aBuffer);

	void HandleExcessDataL();

	void StartTimer();

	void ConstructL();


private:	// typedefs
	typedef void (CHttpRequestBatcher::*TSendFunction)(const TDesC8&);


private:	// attributes
	MOutputStreamObserver*		iObserver;

	MOutputStream*				iOutputStream;

	HBufC8*						iBuffer;

	TPtr8						iDataToSend;

	TPtrC8						iExcessData;

	TInt						iMaxBufferSize;

	TSendFunction				iFuncPtr;

	RTimer						iTimer;

	TBool						iTimerCompleted;

public:	// attributes
	__FLOG_DECLARATION_MEMBER2
	};

#endif	// __CHTTPREQUESTBATCHER_H__
