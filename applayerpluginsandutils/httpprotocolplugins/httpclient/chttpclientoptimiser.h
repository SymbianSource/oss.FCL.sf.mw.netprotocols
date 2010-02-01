// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHTTPCLIENTOPTIMISER_H__
#define __CHTTPCLIENTOPTIMISER_H__

#include <e32std.h>
#include <e32base.h>
#include <http/mhttpdataoptimiser.h>

#include "moutputstream.h"
#include "minputstreamobserver.h"
#include "mhttpresponse.h"

class CHttpClientOptimiser: public CBase,
				  public MOutputStream,
				  public MInputStreamObserver
{

public:
	static CHttpClientOptimiser* NewL(MOutputStream& aOutputStream, MInputStreamObserver& aInputStreamObserver);
	
	virtual ~CHttpClientOptimiser();

	void BindOptimiser(MHttpDataOptimiser& aOptimiser);
	
private: 	// methods from MOutputStream
	virtual void Bind(MOutputStreamObserver& aObserver);

	virtual void SendDataReqL(const TDesC8& aBuffer);

	virtual void ShutdownReq();

	virtual void SecureClientReq(const TDesC8& aHostName);

	virtual void Close();

	virtual const CX509Certificate* ServerCert();
	
	virtual TInt CipherSuite(TDes8& aCipherSuite);

	virtual void Reset ();

	virtual void MOutputStream_Reserved();
	
	virtual void SetTCPCorking(TBool aValue);
	
private:	// methods from MInputStreamObserver
	virtual void ReceivedDataIndL(const TDesC8& aBuffer);
	
	virtual void SecureServerCnf();
	
	virtual void InputStreamCloseInd(TInt aError);

	virtual void MInputStreamObserver_Reserved();
	
	virtual MHttpResponse* CurrentResponse();
	
	virtual void OnReceiveTimeOut();
	
private:
	CHttpClientOptimiser(MOutputStream& aOutputStream, MInputStreamObserver& aInputStreamObserver);
	
private:
	MOutputStream*			iOutputStream;

	MHttpDataOptimiser*		iDataOptimiser;
	
	MInputStreamObserver*	iInputStreamObserver;
	
	HBufC8*					iDecodedBuffer;
	
	HBufC8*					iEncodedBuffer;
};

#endif	//__CHTTPCLIENTOPTIMISER_H__

