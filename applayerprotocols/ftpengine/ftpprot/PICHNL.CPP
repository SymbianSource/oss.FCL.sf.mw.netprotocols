// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// FTP protocol engine
// Author:	Philippe Gabriel
// Implements objet controling the PI channel of the FTP protocol
// Model the PI Channel as an FSM
// Transitions between states are triggered by PI methods and events
// coming from ESOCK
// 
//

/**
 @file PICHNL.CPP
 @internalComponent
*/

//#include <in_sock.h>
//#include <c32comm.h>
#include "DEBUG.H"
#include "PICHNL.H"
#include "FTPPROT.H"
#include <e32base.h>
//#include <es_sock.h>
//#include <nifman.h>

//
// Definitions
//

CPISocketRead::CPISocketRead(MPIChannelNotifier* aNotifier,
							 CPIChannel*	aPIChannel)
							 : CActive(EPriorityStandard)
/**
CPISocketRead object
Read Write implementaion.
*/
	{
		iNotifier = aNotifier;
		iPIChannel = aPIChannel;
	};

CPISocketRead* CPISocketRead::NewL(MPIChannelNotifier* aNotifier,
									CPIChannel*	aPIChannel)
	{
	CPISocketRead* self = new(ELeave) CPISocketRead(aNotifier,aPIChannel);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	};

void CPISocketRead::ConstructL()
	{
	CActiveScheduler::Add(this);
	};

void CPISocketRead::DoCancel()
	{
	FTPPROTDEBUG(_DBGPichnl,_L("CPISocketRead::DoCancel() called"));
	iSocket->CancelAll();
	};

void CPISocketRead::RunL()
	{
	FTPPROTDEBUG(_DBGPichnl,_L("CPISocketRead::RunL() called"));
	if (iStatus == KErrNone)
		{
		// Call our notifier
		iNotifier->PIChannelRcvNotification();
		}
	else
		{
		// Uh!!! weird 
		Cancel();
		//Disconnect Pi Channel
		iPIChannel->Disconnect();
		// Notify caller
		iNotifier->PIChannelOperationError(MPIChannelNotifier::EPiConnectionReset);
		}	
	};

void CPISocketRead::IssueRead(TDes8& aBuffer)
/**
Read Data from a Stream Socket 
*/
	{
	FTPPROTDEBUG(_DBGPichnl,_L("CPISocketRead::IssueRead() called\n"));
	if (this->IsActive())
		return;
	iSocket->RecvOneOrMore(aBuffer, 0, iStatus,iRcvLen);
	SetActive();	
	FTPPROTDEBUG(_DBGPichnl,_L("CPISocketRead::IssueRead() recv triggered\n"));
	};

CPIChannel::CPIChannel(MPIChannelNotifier* aNotifier)
						:CActive(CActive::EPriorityStandard)					
/**
PI Channel object
*/
	{
	FTPPROTDEBUG(_DBGPichnl,_L("CPIChannel::CPIChannel called\n"));
	//Initialise State
	iPIChannelState = EPIChannelIdle;
	iNotifier = aNotifier;
	}

CPIChannel* CPIChannel::NewL(MPIChannelNotifier* aNotifier,
							 RSocketServ& aSockServ)
	{
	CPIChannel* self = new (ELeave) CPIChannel(aNotifier);
    CleanupStack::PushL(self);
    self->ConstructL(aSockServ);
    CleanupStack::Pop();
	return self;
	} 

void CPIChannel::ConstructL(RSocketServ& aSockServ)
	{
	FTPPROTDEBUG(_DBGPichnl,_L("CPIChannel::ConstructL called\n"));
	iSockServ = aSockServ;
	iReceiver = CPISocketRead::NewL(iNotifier,this);
	CActiveScheduler::Add(this);
	} 

CPIChannel::~CPIChannel()
	{
	FTPPROTDEBUG(_DBGPichnl,_L("CPIChannel::~CPIChannel called\n"));
	//Abort receive 
	if ( iReceiver )
		{
		iReceiver->Cancel();
		delete iReceiver;
		}
	}

void CPIChannel::RunL()
	{
	FTPPROTDEBUG(_DBGPichnl,_L("Mytest::RunL activated for CPIChannel\n"));
	switch(iPIChannelState)
		{
		case	EPIChannelIdle:
				// Cannot be activated in that state
				__ASSERT_DEBUG(FALSE, User::Panic(_L("CPIChannel"), EPIPanicOutOfState));
				break;
		case	EPIChannelConnecting:
				FTPPROTDEBUG(_DBGPichnl,_L("CPIChannel::RunL state: Connecting\n"));
				// Check result of connecting request
				switch (iStatus.Int())
					{
					case KErrNone:
						// If succeed set state to EPIChannelConnected
						iPIChannelState = EPIChannelConnected;
						// Call notifier
						iNotifier->PIChannelOperationCompletion(MPIChannelNotifier::EPiConnectComplete);
						break;
					default:
						// Reset channel to Idle/disconnect socket
						Disconnect();
						// Notify caller
						iNotifier->PIChannelOperationError(MPIChannelNotifier::EPiConnectFailed);
						break;
					}
				break;
		case	EPIChannelDisconnecting:
				FTPPROTDEBUG(_DBGPichnl,_L("CPIChannel::RunL state: Connecting\n"));
				// Assume disconnect always work
				Disconnect();
				// Notify caller
				iNotifier->PIChannelOperationCompletion(MPIChannelNotifier::EPiDisconnectComplete);
				break;
		case	EPIChannelSending:
				FTPPROTDEBUG(_DBGPichnl,_L("CPIChannel::RunL state: EPIChannelSending\n"));
				// Check result of send request
				switch (iStatus.Int())
					{
					case KErrNone:
						// If succeed set state to EPIChannelConnected
						iPIChannelState = EPIChannelConnected;
						// Call notifier
						iNotifier->PIChannelOperationCompletion(MPIChannelNotifier::EPiSendComplete);
						break;
					default:
						// Reset channel to Idle/disconnect socket
						Disconnect();
						// Notify caller
						iNotifier->PIChannelOperationError(MPIChannelNotifier::EPiSendFailed);
						break;
					}
				break;
		default:
			__ASSERT_DEBUG(FALSE, User::Panic(_L("CPIChannel::RunL"), 0));
				break;
		}
	}

TBool CPIChannel::Connect(TSockAddr& aNetAddr)
/**
@return FALSE if a socket could not be openned
		Caller must check the result and post an error if appropriate
*/
	{
	// Check this is a valid state for this operation
	__ASSERT_DEBUG((iPIChannelState == EPIChannelIdle), User::Panic(_L("CPIChannel"), EPIPanicChannelConnected));
	// Open a socket
	if(KErrNone != iPiSocket.Open(iSockServ, KAfInet, KSockStream, KProtocolInetTcp))
		// Socket could not be openned, bail out
		return FALSE;
	// Give CPISocketRead the ref to my socket
	iReceiver->SetSocket(&iPiSocket);
	// Connect to peer
	iPiSocket.Connect(aNetAddr, iStatus);
	// Update state
	iPIChannelState = EPIChannelConnecting;
	// Activate the object
	SetActive();
	// Everything's OK
	return TRUE;
	}
void CPIChannel::GetLocalAddress(TInetAddr& aLocalAddr)
	{
	// get my local address
	// seems that LocalAddress function does not return
	// the IP address until you've connected to somewhere
	// hence I need this function to be called once the 
	// connect has succeeded to discover my IP address
	iPiSocket.LocalName(aLocalAddr);
	}

TBool CPIChannel::Busy(void) 
/**
@return The state of DTP Channel
*/
	{
	return(iPIChannelState != EPIChannelConnected);
	}

void CPIChannel::Disconnect(void)
	{
	FTPPROTDEBUG(_DBGPichnl,_L("CPIChannel::Disconnect called\n"));
	//Close PI socket
	iReceiver->Cancel();
	Cancel();
	iPiSocket.Close();
	iPIChannelState = EPIChannelIdle;
	}

void CPIChannel::SendCommand(TDes8& aCommand,TUint aSomeFlags,TBool aSendCRLF)
	{
	// If I'm not connected just return don't post error
	if(iPIChannelState == EPIChannelIdle)
		{
		FTPPROTDEBUG(_DBGPichnl,_L("!!!!!Not Connected - SendCommand not triggered\n"));
		return;
		}
	//__ASSERT_DEBUG(iPIChannelState != EPIChannelIdle, User::Panic(_L("CPIChannel"), EPIPanicChannelNotConnected));
	// Update the state
	iPIChannelState = EPIChannelSending;
	// Add the Telnet terminator sequence
	// BIG BIG assumption here !! the buffer is big enough
	// but FtpProtocol is my friend I trust it
	if(aSendCRLF)
		{
		aCommand.Append(CR);
		aCommand.Append(LF);
		}
	// Send the command
	iPiSocket.Send(aCommand,aSomeFlags,iStatus);
	// Activate the object
	SetActive();
	}


void CPIChannel::GetNextAnswer(TDes8&	aBuffer)
	{
	FTPPROTDEBUG(_DBGPichnl,_L("CPIChannel::GetNextAnswer called\n"));
	if(iPIChannelState != EPIChannelIdle)
		// Fetch the reply sent by the server
		iReceiver->IssueRead(aBuffer);
	}

void CPIChannel::DoCancel(void)
	{
	FTPPROTDEBUG(_DBGPichnl,_L("CPIChannel::DoCancel called\n"));
	iReceiver->Cancel();
	switch(iPIChannelState)
		{
		case EPIChannelConnecting:
			iPiSocket.CancelConnect();
			iPiSocket.Close();
			iPIChannelState = EPIChannelIdle;
			break;
		default:
			break;
		}
	}

