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

#ifndef __CLOCALTESTSERVERSTREAMMANAGER_H__
#define __CLOCALTESTSERVERSTREAMMANAGER_H__

#include <e32base.h>
#include <f32file.h>

#include <minputstreamobserver.h>
#include <moutputstreamobserver.h>
class MInputStream;
class MOutputStream;
class CHTTPTestUtils;
class CHttpClientTestParamArray;
class CHttpClientTestParams;



class CLocalTestServerStreamManager : public CBase, public MInputStreamObserver,
								 											public MOutputStreamObserver
	{  
	public:
	static CLocalTestServerStreamManager* NewL(CHTTPTestUtils& aTestUtils, MInputStream* aInStream, MOutputStream* aOutStream, CHttpClientTestParamArray& aTestParamArray);
	~CLocalTestServerStreamManager();
	
private:
	CLocalTestServerStreamManager(CHTTPTestUtils& aTestUtils, MInputStream* aInStream, MOutputStream* aOutStream, CHttpClientTestParamArray& aTestParamArray);
	// From MInputStreamObserver
	void ReceivedDataIndL(const TDesC8& aBuffer);
	void SecureServerCnf();
	void InputStreamCloseInd(TInt aError);
	void MInputStreamObserver_Reserved();
	MHttpResponse* CurrentResponse();
	virtual void OnReceiveTimeOut();
	
	// From MOutputStreamObserver
	void SendDataCnfL();
	void SecureClientCnf();
	inline TBool SecureRetry(){ return EFalse;}
	void OutputStreamCloseInd(TInt aError);
	void MOutputStreamObserver_Reserved();
	void OnSendTimeOut();
	TInt SendTimeOutVal();
	TPtrC8 GetLine(const TDesC8& aData);
	void ProcessRequestStreamL();
	void ParseRequestStream();
	
	void SendDataL();
private:
	CHTTPTestUtils&				iTestUtils;
	MInputStream*			    iInputStream;	// Not owned
	MOutputStream*				iOutputStream;	// Not owned
	HBufC8*                     iRequestStream;
	HBufC8*                     iResponseStream;
    CHttpClientTestParamArray&  iTestParamArray; 	
    TBool                       iSendingBodyData;
    CHttpClientTestParams*       iCurrentTestParam;
    TBool                       iWritingUploadFile;
	};								 												
#endif // __CLOCALTESTSERVERSTREAMMANAGER_H__
