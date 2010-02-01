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
//

#if !defined(__T_DUMMYSERVER_H__)
#define __T_DUMMYSERVER_H__

// This header file defines a dummy HTTP server that listens on a port for a connection, which
// when connected is serviced by validating the request data and sending a canned response.  The request and
// response data is obtained from text files specified in the URL:
//
// e.g. http://127.0.0.1:8000/?req=request.txt&resp=response.txt

#include <e32base.h>
#include <e32std.h>
#include <in_sock.h>
#include <f32file.h>


class CDummyServer : public CActive
	{
	enum TState
		{
		EInitialising,
		EListening,
		EAccepted,
		EProcessingRequest,
		EReadingRequest,
		ESendingBody,
		ECompleteError,
		EIdleShuttingDown,
		EIdleDisconnected
		};
	enum TError
		{
		ENone = 0,
		EInvalidQuery = -7299,
		ERequestFileNotFound = -7298,
		EResponseFileNotFound = -7297,
		ERequestMismatch = -7296
		};
public:
	IMPORT_C static CDummyServer* NewL(TInt aPort, TInt aRespChunkSize);
	IMPORT_C virtual ~CDummyServer();
	IMPORT_C void ListenL();
protected:
	IMPORT_C virtual void RunL();
	IMPORT_C virtual TInt RunError(TInt aErr);	
	IMPORT_C virtual void DoCancel();
private:
	CDummyServer(TInt aPort, TInt aRespChunkSize, TInt aSleepPeriodSec);
	void ConstructL();
	void CompleteSelf();
	TBool ValidateRequest(TBool& aHitEndOfFileData, TBool& aHitEndOfSocketData);
private:
	// dummy server state
	TState iState;
	TError iError;

	// socket stuff
	TInt iPort;
	RSocketServ iSockSrvHnd;
	RSocket iListenSockHnd;
	RSocket iAcceptSockHnd;
	TSockXfrLength iXferLength;
	TBool iSockSrvOpen;
	TBool iListSockOpen;
	TBool iAccSockOpen;
	TInt iSleepPeriodSec;
	TPckgBuf<TInt> iFrameSize;

	// buffers
	HBufC8* iRecvBuffer;
	TPtr8 iRecvBuff_ptr;
	HBufC8* iErrorPageData;
	TPtr8 iErrorPage_ptr;
	HBufC8* iResponseData;
	TPtr8 iResponse_ptr;

	// file stuff
	RFs iFileSrvHnd;
	RFile iRequestFile;
	RFile iResponseFile;
	TBool iFileSrvOpen;
	TBool iReqFileOpen;
	TBool iRespFileOpen;
	TBufC<KMaxFileName> iReqFileName;
	TBufC<KMaxFileName> iRespFileName;
	TParse iFileNameParser;
	TInt iRespChunkSize;

	// comparison
	TInt iFirstDiffPos;
	TBuf8<8> iActualReqSlice;
	TBuf8<8> iExpectedReqSlice;
	TBuf8<1> iCharBuffer;
	};

#endif // !defined __T_DUMMYSERVER_H__
