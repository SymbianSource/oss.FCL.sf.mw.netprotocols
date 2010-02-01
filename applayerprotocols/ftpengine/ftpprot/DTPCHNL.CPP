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
// Implements objet controling the DTP channel of the FTP protocol
// Model the DTP Channel as an FSM
// 
//

/**
 @file DTPCHNL.CPP
 @internalComponent
*/

//#include <in_sock.h>
//#include <c32comm.h>
#include "DEBUG.H"
#include "DTPCHNL.H"
#include "FTPPROT.H"
//#include <es_sock.h>

//
// Definitions
//

CDTPChannel::CDTPChannel(MDTPChannelNotifier* aNotifier,
					 	 CFTPSetError* aCFTPSetError)
						 :CActive(CActive::EPriorityStandard)
/**
DTP Channel object
*/			
	{
	FTPPROTDEBUG(_DBGDtpchnl,_L("CDTPChannel::CDTPChannel called\n"));
	//Initialise State
	iDTPChannelState = EDTPChannelIdle;
	iNotifier = aNotifier;
	iCFTPSetError = aCFTPSetError;
	}

CDTPChannel* CDTPChannel::NewL(MDTPChannelNotifier* aNotifier,
							   CFTPSetError* aCFTPSetError,
							   RSocketServ& aSockServ)
	{
	CDTPChannel* self = new (ELeave) CDTPChannel(aNotifier,
												 aCFTPSetError);
    CleanupStack::PushL(self);
    self->ConstructL(aSockServ);
    CleanupStack::Pop();
	return self;
	} 

void CDTPChannel::ConstructL(RSocketServ& aSockServ)
	{
	FTPPROTDEBUG(_DBGDtpchnl,_L("CDTPChannel::ConstructL called\n"));
	iSockServ = aSockServ;
	CActiveScheduler::Add(this);
	} 

CDTPChannel::~CDTPChannel()
	{
	FTPPROTDEBUG(_DBGDtpchnl,_L("CDTPChannel::~CDTPChannel called\n"));
	Disconnect();
	}

void CDTPChannel::DoCancel(void)
	{
	FTPPROTDEBUG(_DBGDtpchnl,_L("CDTPChannel::DoCancel -state: "));
		switch(iDTPChannelState)
		{
		case	EDTPChannelConnected:
		case	EDTPChannelIdle:
		case	EDTPChannelShutingDown:
				FTPPROTDEBUG(_DBGDtpchnl,_L(" No req pending\n"));
				//Nothing to do
				break;
		case	EDTPChannelConnecting:
		case	EDTPChannelListening:
		case	EDTPChannelReceiving:
		case	EDTPChannelSending:
				FTPPROTDEBUG(_DBGDtpchnl,_L("Req pending - Canceling all "));
				// Cancel all request
				iDTPTransferSocket.CancelAll();
				// Close all sockets
				iDTPListenSocket.Close();
				iDTPTransferSocket.Close();
				// Reset state
				iDTPChannelState = EDTPChannelIdle;
				break;
		default:
				FTPPROTDEBUG(_DBGDtpchnl,_L("INVALID STATE\n "));
				// Cannot be activated in that state
				__ASSERT_DEBUG(FALSE, User::Panic(_L("DTPChannel"), EDTPPanicChannelOutOfState));
				break;
		}
	}

TBool CDTPChannel::Connect(TSockAddr& aNetAddr)
	{
	FTPPROTDEBUG(_DBGDtpchnl,_L("CDTPChannel::Connect called\n"));
	if(KErrNone != iDTPTransferSocket.Open(iSockServ, KAfInet, KSockStream, KProtocolInetTcp))
		return FALSE;
	// Connect to peer
	iDTPTransferSocket.Connect(aNetAddr, iStatus);
	// Update state
	iDTPChannelState = EDTPChannelConnecting;
	// Activate the object
	SetActive();
	return TRUE;
	}

void CDTPChannel::Accept(void)	
	{
	FTPPROTDEBUG(_DBGDtpchnl,_L("CDTPChannel::Accept called\n"));
	iDTPListenSocket.Accept(iDTPTransferSocket, iStatus);
	// Activate the object
	SetActive();
	}

TUint CDTPChannel::ListeningPort(void)
	{
	FTPPROTDEBUG(_DBGDtpchnl,_L("CDTPChannel::ListeningPort called\n"));
	if(KErrNone != iDTPListenSocket.Open(iSockServ, KAfInet, KSockStream, KProtocolInetTcp))
		return 0;
	if(KErrNone != iDTPTransferSocket.Open(iSockServ))
		return 0;
	iLocalAddress.SetPort(0);
	if(KErrNone != iDTPListenSocket.Bind(iLocalAddress))
		{
		Disconnect();
		return 0;
		}
	if(KErrNone != iDTPListenSocket.Listen(1))
		{
		Disconnect();
		return 0;
		}
	iDTPChannelState = EDTPChannelListening;
	return iDTPListenSocket.LocalPort();
	}

void CDTPChannel::Disconnect(void)
	{
	Cancel();
	iDTPListenSocket.Close();
	iDTPTransferSocket.Close();
	iDTPChannelState = EDTPChannelIdle;
	}	

void CDTPChannel::Send(TDesC8& aBuffer)
	{
	iDTPTransferSocket.Send(aBuffer,0,iStatus);
	iDTPChannelState = EDTPChannelSending;
	SetActive();	
	}

void CDTPChannel::SendEOF(void)
	{
	// Following line caused the connection to be closed with a 
	// RST, SunOS ftpd complained about that 
	// iDTPTransferSocket.Shutdown(RSocket::ENormal,iStatus);
	// Following line to stop on a sending socket
	// iDTPTransferSocket.Shutdown(RSocket::EStopOutput,iStatus);
	// Following line to stop on a sending socket
	iDTPTransferSocket.Shutdown(RSocket::EStopInput,iStatus);
	iDTPChannelState = EDTPChannelShutingDown;
	SetActive();	
	}

void CDTPChannel::Recv(TDes8& aBuffer)
	{
	FTPPROTDEBUG(_DBGDtpchnl,_L("CDTPChannel::Recv called\n"));
	iDTPTransferSocket.RecvOneOrMore(aBuffer, 0, iStatus,iRcvLen);
	iDTPChannelState = EDTPChannelReceiving;
	SetActive();	
	}

TBool CDTPChannel::Closed(void) 
/**
@return The state of DTP Channel
*/
	{
	return(iDTPChannelState==EDTPChannelIdle);
	}

TBool CDTPChannel::Connected(void) 
/**
@return The state of DTP Channel
*/
	{
	return((iDTPChannelState==EDTPChannelConnected)
		|| (iDTPChannelState==EDTPChannelSending)
		|| (iDTPChannelState==EDTPChannelReceiving));
	}

void CDTPChannel::RunL()
	{
	FTPPROTDEBUG(_DBGDtpchnl,_L("CDTPChannel::RunL -state: "));
		switch(iDTPChannelState)
		{
		case	EDTPChannelIdle:
				FTPPROTDEBUG(_DBGDtpchnl,_L(" Idle -PANIC-\n"));
				// Cannot be activated in that state
				__ASSERT_DEBUG(FALSE, User::Panic(_L("DTPChannel"), EDTPPanicChannelOutOfState));
				// Do error recovery
				break;
		case	EDTPChannelConnecting:
				FTPPROTDEBUG(_DBGDtpchnl,_L("EDTPChannelConnecting "));
				// Check result of connecting request
				switch (iStatus.Int())
					{
					case KErrNone:
						FTPPROTDEBUG(_DBGDtpchnl,_L("iStatus:OK\n"));
						// If succeed change state
						iDTPChannelState = EDTPChannelConnected;
						// Call notifier
						iNotifier->DTPChannelOperationCompletion(MDTPChannelNotifier::EDtpConnectComplete);
						break;
					default:
						FTPPROTDEBUG1(_DBGDtpchnl,_L("FAILED iStatus:<%d>\n "),iStatus.Int());
						// Disconnect DTP Channel
						Disconnect();
						iNotifier->DTPChannelOperationError(MDTPChannelNotifier::EDtpConnectFailed);
						break;
					}
				break;
		case	EDTPChannelListening:
				FTPPROTDEBUG(_DBGDtpchnl,_L("EDTPChannelListening "));
				// Close listening socket immediately
				// Don't want open ports hangin round
				iDTPListenSocket.Close();
				// Check result of accepting request
				switch (iStatus.Int())
					{
					case KErrNone:
						FTPPROTDEBUG(_DBGDtpchnl,_L("iStatus:OK\n"));
						// If succeed change state
						iDTPChannelState = EDTPChannelConnected;
						// Call notifier
						iNotifier->DTPChannelOperationCompletion(MDTPChannelNotifier::EDtpAcceptComplete);
						break;
					default:
						FTPPROTDEBUG1(_DBGDtpchnl,_L("FAILED iStatus:<%d>\n "),iStatus.Int());
						// Disconnect DTP Channel
						Disconnect();
						iNotifier->DTPChannelOperationError(MDTPChannelNotifier::EDtpConnectFailed);
						break;
					}
				break;
		case	EDTPChannelReceiving:
				FTPPROTDEBUG(_DBGDtpchnl,_L("EDTPChannelReceiving "));
				switch (iStatus.Int())
					{
					case KErrNone:
						FTPPROTDEBUG(_DBGDtpchnl,_L("iStatus:OK\n"));
						// Packet received - Call notifier
						iNotifier->DTPChannelXferNotification(MDTPChannelNotifier::EDtpRcvMoreData);
						break;
					case KErrEof:
						FTPPROTDEBUG(_DBGDtpchnl,_L("iStatus:KErrEof\n"));
						// Disconnect DTP Channel
						Disconnect();
						// Server closed the DTP Channel - Call notifier
						iNotifier->DTPChannelXferNotification(MDTPChannelNotifier::EDtpRcvComplete);
						break;
					default:
						FTPPROTDEBUG1(_DBGDtpchnl,_L("FAILED iStatus: <%d>\n"),iStatus.Int());
						// Disconnect DTP Channel
						Disconnect();
						// Notify caller
						iNotifier->DTPChannelOperationError(MDTPChannelNotifier::EDtpRecvAborted);
						break;
					}
				break;
		case	EDTPChannelSending:
				FTPPROTDEBUG(_DBGDtpchnl,_L("EDTPChannelSending "));
				switch (iStatus.Int())
					{
					case KErrNone:
						FTPPROTDEBUG(_DBGDtpchnl,_L("iStatus:OK\n"));
						// Packet sent - Call notifier
						iNotifier->DTPChannelXferNotification(MDTPChannelNotifier::EDtpSendComplete);
						break;
					default:
						FTPPROTDEBUG1(_DBGDtpchnl,_L("FAILED iStatus: <%d>"),iStatus.Int());
						// Disconnect DTP Channel
						Disconnect();
						// Notify caller
						iNotifier->DTPChannelOperationError(MDTPChannelNotifier::EDtpSendAborted);
						break;
					}
				break;
		case	EDTPChannelShutingDown:
				FTPPROTDEBUG(_DBGDtpchnl,_L("EDTPChannelShutingDown "));
				// Whatever shutdown returns Disconnect DTP Channel
				Disconnect();
				switch (iStatus.Int())
					{
					// Only do a shutdown on a SendEOF operation
					case KErrNone:
						FTPPROTDEBUG(_DBGDtpchnl,_L("iStatus:OK\n"));
						// DTP transfer socket closed Ok - Call notifier
						iNotifier->DTPChannelXferNotification(MDTPChannelNotifier::EDtpSendEOFComplete);
						break;
					default:
						// Something wrong with the shutdown, can't assume the peer received the file ok
						FTPPROTDEBUG1(_DBGDtpchnl,_L("FAILED iStatus: <%d>\n"),iStatus.Int());
						// Notify caller
						iNotifier->DTPChannelOperationError(MDTPChannelNotifier::EDtpSendAborted);
						break;
					}
				break;
			default:
				FTPPROTDEBUG(_DBGDtpchnl,_L("INVALID STATE\n "));
				// Cannot be activated in that state
				__ASSERT_DEBUG(FALSE, User::Panic(_L("DTPChannel"), EDTPPanicChannelOutOfState));
				break;
		}

	}
