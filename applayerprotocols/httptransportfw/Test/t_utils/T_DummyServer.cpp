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
// defines test classes: CDummyServer
// 
//

#include "T_DummyServer.h"
#include <uri8.h>


// Constants used in CDummyServer test
#ifdef _DEBUG
	_LIT(KDummyServerPanic, "DUMMYHTTPSVR");
#endif

const TInt KRecvBufSize = 128;
const TInt KChunkSize = 10240;
const TInt KListenQSize = 1;
_LIT(KTcpProtName, "tcp");
_LIT(KFileSystemRoot,"Z:\\");
_LIT8(KCrLf, "\r\n");


// Canned responses for each error situation we report
_LIT8(KErrPageMissingQuery,		"HTTP/1.1 404 Query Not Found\r\nDate: %S\r\nServer: T_DummyServer(EPOC32/6.2)\r\nContent-Type: text/html\r\nContent-Length: 299\r\nConnection: close\r\n\r\n<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n<HTML><HEAD><TITLE>Query Not Found</TITLE></HEAD><BODY><H1>Query Not Found</H1>The requested URL did not include a query portion for validation of request and response.<P><HR><ADDRESS>T_DummyServer at 127.0.0.1 on port %d</ADDRESS></BODY></HTML>");
_LIT8(KErrRequestFileNotFound,	"HTTP/1.1 404 Request Content File Not Found\r\nDate: %S\r\nServer: T_DummyServer(EPOC32/6.2)\r\nContent-Type: text/html\r\nContent-Length: 330\r\nConnection: close\r\n\r\n<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n<HTML><HEAD><TITLE>Request Content File Not Found</TITLE></HEAD><BODY><H1>Request Content File Not Found</H1>The request filename specified in the URL query portion could not be found.<P><HR><ADDRESS>T_DummyServer at 127.0.0.1 on port %d</ADDRESS></BODY></HTML>");
_LIT8(KErrResponseFileNotFound, "HTTP/1.1 404 Response Content File Not Found\r\nDate: %S\r\nServer: T_DummyServer(EPOC32/6.2)\r\nContent-Type: text/html\r\nContent-Length: 334\r\nConnection: close\r\n\r\n<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n<HTML><HEAD><TITLE>Response Content File Not Found</TITLE></HEAD><BODY><H1>Response Content File Not Found</H1>The response filename specified in the URL query portion could not be found.<P><HR><ADDRESS>T_DummyServer at 127.0.0.1 on port %d</ADDRESS></BODY></HTML>");
_LIT8(KErrRequestMismatch,		"HTTP/1.1 400 Invalid Request\r\nDate: %S\r\nServer: T_DummyServer(EPOC32/6.2)\r\nContent-Type: text/html\r\nContent-Length: 337\r\nConnection: close\r\n\r\n<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n<HTML><HEAD><TITLE>Invalid Request</TITLE></HEAD><BODY><H1>Invalid Request</H1>The request differed from that in the validation file.  First difference at byte %3d.  Expected [%S], Received [%S]<P><HR><ADDRESS>T_DummyServer at 127.0.0.1 on port %d</ADDRESS></BODY></HTML>");


// format for output of data/time values
// e.g. Wed, 21 Mar 2001 15:29:19 GMT
_LIT(KDateFormat,"%F%*E, %D %*N %Y %H%:1%T%:2%S GMT");


// CDummyServer-----------------------------------

EXPORT_C
CDummyServer* CDummyServer::NewL(TInt aPort, TInt aRespChunkSize)
	{
	CDummyServer* me = new(ELeave)CDummyServer(aPort, aRespChunkSize, 1);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

EXPORT_C
CDummyServer::~CDummyServer()
	{
	// Cancel any outstanding request and remove from active scheduler
	Cancel();
	if (IsAdded())
		Deque();

	// Close handles - individual files and sockets first, then server connections
	if (iListSockOpen)
		iListenSockHnd.Close();
	if (iAccSockOpen)
		iAcceptSockHnd.Close();
	if (iReqFileOpen)
		iRequestFile.Close();
	if (iRespFileOpen)
		iResponseFile.Close();

	if (iSockSrvOpen)
		iSockSrvHnd.Close();
	if (iFileSrvOpen)
		iFileSrvHnd.Close();

	// Free memory
	delete iRecvBuffer;
	delete iErrorPageData;
	delete iResponseData;
	}

EXPORT_C
void CDummyServer::ListenL()
	{
	// Start listening for incoming connections
	TProtocolDesc desc;
	User::LeaveIfError(iSockSrvHnd.FindProtocol(KTcpProtName(), desc));
	iListenSockHnd.Open(iSockSrvHnd, desc.iAddrFamily, desc.iSockType, desc.iProtocol);
	iListSockOpen = ETrue;
	TSockAddr addr;
	addr.SetPort(iPort);
	iListenSockHnd.Bind(addr);
	iListenSockHnd.Listen(KListenQSize);

	// Add to the scheduler and set to 'listening' state
	CActiveScheduler::Add(this);
	iState = EInitialising;

	// Now go active, ready to accept connections
	CompleteSelf();
	}

CDummyServer::CDummyServer(TInt aPort, TInt aRespChunkSize, TInt aSleepPeriodSec):
	CActive(EPriorityStandard),
	iPort(aPort),iSleepPeriodSec(aSleepPeriodSec), iRecvBuff_ptr(NULL, 0), iErrorPage_ptr(NULL, 0), iResponse_ptr(NULL, 0),
	iRespChunkSize(aRespChunkSize) 
	{
	// Set the package containing a TCP frame size, based on the desired response chunk size
	iFrameSize = iRespChunkSize;
	}

void CDummyServer::ConstructL()
	{
	// Make a receive buffer, to hold the data read from the accepting socket
	iRecvBuffer = HBufC8::NewL(KRecvBufSize);
	iRecvBuff_ptr.Set(iRecvBuffer->Des());
	
	// Make a buffer in which error response pages are assembled before returning to the client
	iErrorPageData = HBufC8::NewL(KChunkSize);
	iErrorPage_ptr.Set(iErrorPageData->Des());

	// Make a buffer in which the response chunks are read from file before returning to the client
	iResponseData = HBufC8::NewL(iRespChunkSize);
	iResponse_ptr.Set(iResponseData->Des());

	// Make socket server and file server connections
	User::LeaveIfError(iSockSrvHnd.Connect());
	iSockSrvOpen = ETrue;
	User::LeaveIfError(iFileSrvHnd.Connect());
	iFileSrvOpen = ETrue;
	}

EXPORT_C
void CDummyServer::RunL()
	{
	switch (iState)
		{
	case EInitialising:
		{
		// Open the blank accepting socket and await an incoming connection
		iAcceptSockHnd.Open(iSockSrvHnd);
		iAccSockOpen = ETrue;
		iState = EListening;
		CompleteSelf();
		} break;
	case EListening:
		{
		iListenSockHnd.Accept(iAcceptSockHnd, iStatus);
		iState = EAccepted;
		SetActive();
		} break;

	case EAccepted:
		{
		// Incoming connection is bound - now read from it to get the first part of the client request
		iAcceptSockHnd.Read(iRecvBuff_ptr, iStatus);
		iState = EProcessingRequest;
		SetActive();
		} break;

	case EProcessingRequest:
		{
		// Determine whether there is more data to come, or if we're at the end of the request
		if (iStatus.Int() == KErrNone)
			{
			// Locate the request line
			TPtrC8 reqLine;
			TInt crLfPos = iRecvBuffer->Find(KCrLf());
			__ASSERT_DEBUG(crLfPos > 0, User::Panic(KDummyServerPanic(), KErrGeneral));
			reqLine.Set(iRecvBuffer->Left(crLfPos - 1));

			// Within the request line, locate the URI. It must follow a space and have a space afterwards
			TInt spPos1 = reqLine.Locate(TChar(' '));
			TPtrC8 uri;
			uri.Set(reqLine.Mid(spPos1 + 1));
			TInt spPos2 = uri.Locate(TChar(' '));
			uri.Set(uri.Left(spPos2));

			// Form a URI based on this descriptor
			TUriParser8 parse;
			parse.Parse(uri);
			if (parse.IsPresent(EUriQuery))
				{
				TPtrC8 query = parse.Extract(EUriQuery);

				// Extract the request file
				TInt eqPos1 = query.Locate(TChar('='));
				TInt ampPos = query.Locate(TChar('&'));
				if ((eqPos1 == KErrNotFound) || (ampPos == KErrNotFound))
					User::Leave(EInvalidQuery);
				TPtrC8 reqFile8 = query.Mid(eqPos1 + 1, ampPos - eqPos1 - 1);
				TPtr reqFile16_ptr = iReqFileName.Des();
				reqFile16_ptr.Copy(reqFile8);

				// Extract the response file
				query.Set(query.Mid(ampPos + 1));
				TInt eqPos2 = query.Locate(TChar('='));
				if (eqPos2 == KErrNotFound)
					User::Leave(EInvalidQuery);
				TPtrC8 respFile8  = query.Mid(eqPos2 + 1);
				TPtr respFile16_ptr = iRespFileName.Des();
				respFile16_ptr.Copy(respFile8);

				// Got the file names - attempt to open both files
				iFileNameParser.Set(KFileSystemRoot,&iReqFileName,NULL);
				if (iRequestFile.Open(iFileSrvHnd, iFileNameParser.FullName(), EFileRead) != KErrNone)
					User::Leave(ERequestFileNotFound);
				iFileNameParser.Set(KFileSystemRoot,&iRespFileName,NULL);
				if (iResponseFile.Open(iFileSrvHnd, iFileNameParser.FullName(), EFileRead) != KErrNone)
					User::Leave(EResponseFileNotFound);

				// Copy this chunk, minus the request line (which will be wrong due to the query portion) into the request buffer,
				// then copy back (easiest way of chopping off the request line)
				iErrorPage_ptr.Append(iRecvBuffer->Mid(crLfPos + 2));
				iRecvBuff_ptr.Copy(iErrorPage_ptr);
				iFirstDiffPos = 0;

				// Validate what's left from the first read
				TBool hitEndOfSocketData, hitEndOfFileData;
				iCharBuffer.Zero();
				if (!ValidateRequest(hitEndOfSocketData, hitEndOfFileData))
					User::Leave(ERequestMismatch);

				// Ready to receive more request data and to validate it
				iAcceptSockHnd.Read(iRecvBuff_ptr, iStatus);
				iState = EReadingRequest;
				SetActive();
				}
			else
				User::Leave(EInvalidQuery);
			}
		else if (iStatus.Int() == KErrEof)
			{
			__DEBUGGER();
			}
		} break;

	case EReadingRequest:
		{
		// Provided the socket read completed OK, validate what we got
		TBool hitEndOfSocketData, hitEndOfFileData;
		TBool valid = ValidateRequest(hitEndOfSocketData, hitEndOfFileData);

		// If the data isn't valid, or either of the socket stream or the validation file has reached EOF but not the other, then
		// there's a mismatch
		if ((!valid) || (hitEndOfSocketData != hitEndOfFileData))
			User::Leave(ERequestMismatch);
		if (valid)
			{
			if (hitEndOfFileData) // or hitEndOfSocketData, they should be the same by here
				{
				// Reached end of both the request data and the validation file - there is a match. Send the required response.
				iState = ESendingBody;
				CompleteSelf();
				}
			else
				// Provided there's more data available, read from the socket and don't change state
				{
				iAcceptSockHnd.RecvOneOrMore(iRecvBuff_ptr, 0, iStatus, iXferLength);
				SetActive();
				}
			}
		} break;

	case ESendingBody:
		{
		// Sleep for a while to allow the client's read operation to complete as soon as possible - in an
		// attempt to defeat the TCP frame reassembly/segmentation that goes on in the TCP/IP stack
		//User::After(iSleepPeriodSec*1000000);

		// Notify TCP/IP of the data last sent, immediately, in an attempt to get over the frame reassembly
		// that appears to go on in the server.
		TRequestStatus istat;
		iAcceptSockHnd.Ioctl(KIoctlTcpNotifyDataSent, istat, 0, KSolInetTcp);
		User::WaitForRequest(istat);

		// Re-use the request buffer to send the validation body back to the client
		iResponse_ptr.Zero();
		TInt err = iResponseFile.Read(iResponse_ptr);
		if (!err && (iResponse_ptr.Length() > 0))
			{
			// copy data from file to the socket and stay in the same state
			iXferLength = iRespChunkSize;;
			iAcceptSockHnd.Send(iResponse_ptr, 0, iStatus, iXferLength);
			SetActive();
			}
		else
			{
			iState = EIdleShuttingDown;
			CompleteSelf();
			}
		} break;

	case ECompleteError:
		{
		// Get the date now
		TBuf<40> dateTimeString;
		TTime t;
		t.UniversalTime();
		t.FormatL(dateTimeString,KDateFormat);

		// Write an error message back to the client, according to the error state. Re-use the request buffer to
		// formulate the message.
		switch (iError)
			{
		case EInvalidQuery:
			{
			iErrorPage_ptr.Format(KErrPageMissingQuery(), &dateTimeString, iPort);
			} break;
		case ERequestFileNotFound:
			{
			iErrorPage_ptr.Format(KErrRequestFileNotFound(), &dateTimeString, iPort);
			} break;
		case EResponseFileNotFound:
			{
			iErrorPage_ptr.Format(KErrResponseFileNotFound(), &dateTimeString, iPort);
			} break;
		case ERequestMismatch:
			{
			//  to send an error page containing useful information
			// about where the request differed
			iErrorPage_ptr.Format(KErrRequestMismatch(), &dateTimeString, iFirstDiffPos, &iActualReqSlice, &iExpectedReqSlice, iPort);
			} break;
		default:
			;
			}
		iAcceptSockHnd.Write(iErrorPage_ptr, iStatus);
		iState = EIdleShuttingDown;
		SetActive();
		} break;

	case EIdleShuttingDown:
		{
		iAcceptSockHnd.Shutdown(RSocket::ENormal, iStatus);
		iState = EIdleDisconnected;
		SetActive();
		} break;

	case EIdleDisconnected:
		{
		} break;
		}
	}
	
EXPORT_C
TInt CDummyServer::RunError(TInt aErr)
	{
	switch (iState)
		{
	case EListening:
		{
		} break;

	case EAccepted:
		{
		} break;

	case EProcessingRequest:
		{
		if ((aErr == EInvalidQuery) ||
			(aErr == ERequestFileNotFound) ||
			(aErr == EResponseFileNotFound) ||
			(aErr == ERequestMismatch))
			{
			iError = (TError)aErr;
			iState = ECompleteError;
			CompleteSelf();
			}
		} break;

	case EReadingRequest:
		{
		if (aErr == ERequestMismatch)
			{
			iError = (TError)aErr;
			iState = ECompleteError;
			CompleteSelf();
			}
		} break;

	case ESendingBody:
		{
		} break;

	case ECompleteError:
		{
		} break;

	case EIdleShuttingDown:
		{
		} break;

	case EIdleDisconnected:
		{
		} break;
	default:
		break;
		}
	return KErrNone;
	}

EXPORT_C
void CDummyServer::DoCancel()
	{
	}

void CDummyServer::CompleteSelf()
	{
	SetActive();
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	}

TBool CDummyServer::ValidateRequest(TBool& aHitEndOfFileData, TBool& aHitEndOfSocketData)
	{
	aHitEndOfFileData = EFalse;
	aHitEndOfSocketData = EFalse;

	// Validate the receive buffer content against data read byte by byte from the request file
	// (slow, but we need an actual byte index for where the two start to differ)
	TInt bufferSize = iRecvBuff_ptr.Length();

	// Read first char from file in advance - if we've already at the end we don't want to enter the loop
	// Don't do this if the single char buffer is already populated since that is a carryover from the
	// previous call to this method
	if (iCharBuffer.Length() == 0)
		{
		(void) iRequestFile.Read(iCharBuffer);
		aHitEndOfFileData = (iCharBuffer.Length() == 0);
		}

	// Provided we're not already at EOF, then check the receieve buffer against the file
	for (TInt ii = 0; ((ii < bufferSize) && !aHitEndOfFileData); ++ii)
		{
		++iFirstDiffPos;
		TChar socketCh = iRecvBuff_ptr[ii];
		TChar fileCh = iCharBuffer[0];
		if (socketCh != fileCh)
			{
			iActualReqSlice.Copy(iRecvBuff_ptr.Mid(ii, 8));
			iExpectedReqSlice.Append(fileCh);
			TBuf8<7> expBuffCh1To7;
			iRequestFile.Read(expBuffCh1To7);
			iExpectedReqSlice.Append(expBuffCh1To7);
			return EFalse;
			}

		// Read next char, to detect whether we're at EOF yet
		iCharBuffer.Zero();
		(void) iRequestFile.Read(iCharBuffer);
		aHitEndOfFileData = (iCharBuffer.Length() == 0);
		}

	// OK so far -  Check for EOF/no more data on socket (client closed it) by peeking for one char
	// NB This code doesn't work - it hangs when no more data is available.  Frig for now so that
	// both flags get the same value
	aHitEndOfSocketData = aHitEndOfFileData;
	/*
	TRequestStatus peekStat;
	TBuf8<1> sockChBuffer;
	iAcceptSockHnd.RecvOneOrMore(sockChBuffer, KSockReadPeek, peekStat, iXferLength);
	User::WaitForRequest(peekStat);
	aHitEndOfSocketData = (peekStat.Int() != KErrNone); */	
	return ETrue;
	}
