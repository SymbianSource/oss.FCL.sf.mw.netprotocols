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
// CActiveReader & CActiveWriter
// Contains basic Read and Write Active object definitions for Socket I/O
// 
//

/**
 @file
*/

#include "ACTIVEIO.H"
#include "IOBUFFER.H"
#include "TELFSM.H"
#include "TELDEBUG.H"

CActiveWriter::CActiveWriter() : CActive(EPriorityStandard)
/**
Constructor
*/
	{
	CActiveScheduler::Add(this);
	}

CActiveWriter::~CActiveWriter()
/**
Destructor
*/
	{
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CActiveWriter::D'Tor"));
	Cancel();
	}

CActiveWriter* CActiveWriter::NewL(MIONotifier* aNotifier)
	{
	CActiveWriter* self = new(ELeave) CActiveWriter;
	CleanupStack::PushL(self);
	self->ConstructL(aNotifier);
	CleanupStack::Pop();
	return self;
	}

void CActiveWriter::ConstructL(MIONotifier* aNotifier)
	{	
	iNotifier = aNotifier;
	iSocket = NULL;
	}

void CActiveWriter::SetSocket(RSocket* aSocket)
	{
	iSocket = aSocket;
	}

TInt CActiveWriter::IssueWrite(const TDesC8& aBuffer)
/**
Standard write
*/
	{
	if (IsActive())
		return KRequestPending;

	iSocket->Send(aBuffer,NULL,iStatus,iXfrLength);
	SetActive();
	return KErrNone;
	}

TInt CActiveWriter::IssueUrgentWrite(const TDesC8& aBuffer)
/**
Caller wants to send data as TCP urgent
*/
	{
	iSocket->SetOpt(KSoTcpNextSendUrgentData, KSolInetTcp, ETrue);
	iSocket->Send(aBuffer,NULL,iStatus,iXfrLength);
	SetActive();

	return(KErrNone);
	}


void CActiveWriter::RunL()
/**
Write completed on the socket
*/
	{
	if(iStatus != KErrNone)
		iNotifier->WriteError(iStatus.Int());
	else
		iNotifier->WriteComplete();
	}


CActiveReader::CActiveReader() : CActive(EPriorityStandard)
/**
Constructor
*/
	{
	CActiveScheduler::Add(this);
	}

CActiveReader::~CActiveReader()
/**
Destructor
*/
	{
	__FLOG_STATIC(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CActiveReader::D'Tor"));
	Cancel();
	}


CActiveReader* CActiveReader::NewL(MIONotifier* aNotifier)
	{
	CActiveReader* self = new(ELeave) CActiveReader;
	CleanupStack::PushL(self);
	self->ConstructL(aNotifier);
	CleanupStack::Pop();
	return self;
	}

void CActiveReader::ConstructL(MIONotifier* aNotifier)
/**
@internalComponent
*/
	{	
	iNotifier = aNotifier;
	iSocket = NULL;
	iClientBuffer = NULL;
	iRequest = ERequestNone;
	}

void CActiveReader::SetSocket(RSocket* aSocket)
	{
	iSocket = aSocket;
	}

TInt CActiveReader::IssueRead(TDes8& aBuffer)
/**
Issues read using Ioctl so urgents can be picked up
TCP test code Test_05() demonstrates this
*/
	{
	if (IsActive())
		return KRequestPending;
	
	iClientBuffer = &aBuffer;
	// Tell the RunL() to expect an Ioctl completion
	iRequest = EIOctlRequest;
	iflags() = KSockSelectRead | KSockSelectExcept;
	iSocket->Ioctl(KIOctlSelect,iStatus, &iflags, KSOLSocket);
	SetActive();

	return KErrNone;
	}

TInt CActiveReader::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

void CActiveReader::RunL()
/**
Read completion
Performed in 2 stages
We get a completion on the Ioctl first where we test for urgent data, we set active again and
Next we get a normal completion for the read following a call to RecvOneOrMore()
*/
	{
	TInt bytesPending;
	TInt urgentOffset;
	TInt ret;
	TInt urgentData;

	if(iRequest == EIOctlRequest)
		// First completion for the Ioctl
		{
		iSocket->GetOpt(KSoTcpRcvAtMark, KSolInetTcp,ret);
		iSocket->GetOpt(KSOUrgentDataOffset, KSOLSocket,urgentOffset);
		iSocket->GetOpt(KSOReadBytesPending, KSOLSocket,bytesPending);
		urgentData = 0;
		// Peek check for urgent data
		if(iSocket->GetOpt(KSoTcpPeekUrgentData, KSolInetTcp,urgentData)==KErrNone)
			{
			// We have urgent data
			urgentData = 0;
			// Read the urgent data
			iSocket->GetOpt(KSoTcpReadUrgentData, KSolInetTcp,urgentData);
			__FLOG_STATIC1(KTelnetLoggingCompnt(),KTelnetLoggingTag(),_L("CActiveReader::RunL() Urgent Data = %X"),urgentData);
			// Notify that we have received urgent data
			iNotifier->Event(CProto::EEventUrgentData,urgentData);
			}
		// Now set to receive data
		iSocket->RecvOneOrMore(*iClientBuffer,NULL,iStatus,iXfrLength);
		SetActive();
		// Completion will be normal next time
		iRequest = EReadRequest;
		}
	else if(iRequest == EReadRequest)
		{
		// Normal read complete
		// iClientBuffer will contain the data
		if(iStatus.Int() == KErrNone)
			{
			iNotifier->ReadCompleteL();
			}
		else
			{
			iNotifier->ReadComplete(iStatus.Int());
			}
		}
	}

