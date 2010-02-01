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
// FTP protocol engine
// Author:	Philippe Gabriel
// Entry point in the DLL
// Implements the interface to the FTP protocol
// through FTP commands defined as per RFC 959
// Drives the DTP and PI channels
// 
//

/**
 @file FTPPROT.CPP
 @internalComponent
*/

#include <es_sock.h>
#include <c32comm.h>
#include "DEBUG.H"
#include "PROTOCOL.H"
#include "PICHNL.H"
#include "ANSPARSE.H"
#include "PASVANS.H"
#include "DTPCHNL.H"
#include "SETERROR.H"

//
//					Definitions
//

//
//					Minterface Implementation
//

void CFtpProtocolDerived::PIChannelOperationCompletion(const TPiOperationCompletion aCompletionStatus)
/**
Define the PIChannel callback notifiers
These notifiers are called from PIChannel, but the event source
can be quite different:
PIChannelOperationCompletion, PIChannelOperationError and PIChannelReset
come from an error signaled at the TCP/IP level
where PIChannelRcvNotification is called whenever the server sent us a message
*/
	{
	switch (aCompletionStatus)
		{
		case MPIChannelNotifier::EPiConnectComplete:
			FTPPROTDEBUG(_DBGFtpprot,_L("PIChannelNotifierClass::PIChannelOperationCompletion(EPiConnectComplete) called\n"));
			// Get my IP address for future use
			iPiChannel->GetLocalAddress(iLocalAddress);
			// Get welcome message from server 
			iPiChannel->GetNextAnswer(iServerAnswerBuffer);
			UpdateState(EPiChannelConnectComplete);
			break;
		case MPIChannelNotifier::EPiSendComplete:
			FTPPROTDEBUG(_DBGFtpprot,_L("PIChannelNotifierClass::PIChannelOperationCompletion(EPiSendComplete) called\n"));
			UpdateState(EPiChannelSendComplete);
			break;
		default:
			FTPPROTDEBUG(_DBGFtpprot,_L("PIChannelNotifierClass::PIChannelOperationCompletion() called\n"));
			break;
		}
	return;
	}

void CFtpProtocolDerived::PIChannelOperationError(const TPiOperationError aErrorStatus)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("PIChannelNotifier::PIChannelOperationError called\n"));
		//Close DTP Channel
		iDtpChannel->Disconnect();
		//Reset state
		iState = EIdle;
		switch (aErrorStatus)
			{
			case MPIChannelNotifier::EPiConnectionReset:
				//ARGHH this is serious and not recoverable
				// Notify upper layer
				iNotifier->ErrorNotification(MFtpProtocolNotifier::EOpConnectionReset);
				break;
			case MPIChannelNotifier::EPiConnectFailed:
				// Notify upper layer
				iNotifier->ErrorNotification(MFtpProtocolNotifier::EOpConnectionFailed);
				break;
			default:
				break;
			}
	return;
	}

void CFtpProtocolDerived::PIChannelRcvNotification(void)
	{
	// Must call upper level client before issuing another read, otherwise
	// I lose my buffer
	FTPPROTDEBUG(_DBGFtpprot,_L("PIChannelNotifier::PIChannelRcvNotification called\n"));
	// At this point need to parse the incoming buffer to decide if 
	// more data is to be fetched
	// 2 cases
	// - 1 line message
	// - multiline message
	// Parse the buffer 
	// it may contain several answers
	for(;;)
		{
		TBool answer;
		// Parse an answer
		answer = iFTPServerAnswerParser->Parse(iServerAnswerBuffer);
		// If I'm performing a PASV
		// Parse an address
		if(	iState == EPerformingPasv)		
			iFtpPASVAnswerParser->Parse(iServerAnswerBuffer, iRemoteAddress);
		//Passes the server text to upper layer
		iNotifier->ServerMessage(iServerAnswerBuffer.Left(iFTPServerAnswerParser->NChars()));
		// Remove what's already parsed from buffer
		iServerAnswerBuffer.Delete(0,iFTPServerAnswerParser->NChars());
		if(answer)
			{
			// if I parsed an answer succesfully, notify upper layer
			// of answer code
			UpdateState(EFtpCodeReply);
			}
		//If buffer empty, stop parsing
		if(iServerAnswerBuffer.Length()==0)
			break;
		}
	// Fetch next answer, if there's more to come
	// PG 2808 Always fetch next, potential hanging on close bug here
	//if (iState != EPerformingQuit)
		// Get ready to fetch next answer from server 
		iPiChannel->GetNextAnswer(iServerAnswerBuffer);
	return;
	}

void CFtpProtocolDerived::DTPChannelOperationCompletion(const TDtpOperationCompletion aCompletionStatus)
/**
Define the DTPChannel callback notifiers
Notify of normal completion of an operation
*/
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("CFtpProtocolDerived::DTPChannelOperationCompletion called "));
	switch(aCompletionStatus)
		{
		case MDTPChannelNotifier::EDtpAcceptComplete:
		case MDTPChannelNotifier::EDtpConnectComplete:
			FTPPROTDEBUG(_DBGFtpprot,_L("Connected \n"));
			switch(iState)
				{
				case EPerformingList:
				case EPerformingNlst:
				case EPerformingRetr:
					iDtpChannel->Recv(*iIOBuffer);
					break;
				case EPerformingStor:
				case EPerformingStou:
					iDtpChannel->Send(*iIOBuffer);
					break;
				}
			break;
		default:
			break;
		}
	}

void CFtpProtocolDerived::DTPChannelOperationError(const TDtpOperationError aErrorStatus)
/**
Notify of error performing an operation
*/
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("CFtpProtocolDerived::DTPChannelOperationError called\n"));
		switch (aErrorStatus)
			{
			case MDTPChannelNotifier::EDtpConnectFailed:
				// 
				iNotifier->ErrorNotification(MFtpProtocolNotifier::EXferNotInitialised);
				break;
			case MDTPChannelNotifier::EDtpRecvAborted:
			case MDTPChannelNotifier::EDtpSendAborted:
				switch(iState)
					{
					// Some bad behaved ftp server (microsoft)
					// reset the DTP connection on abort
					case EPerformingAbor:
						UpdateState(EDtpChannelClosed);
						break;
					default:
						//Close PI Channel
						iPiChannel->Disconnect();
						//Reset state
						iState = EIdle;
						iNotifier->ErrorNotification(MFtpProtocolNotifier::EXferReset);
						break;
					}
				break;
			default:
				// Cannot come here
				__ASSERT_DEBUG(FALSE, User::Panic(_L("CFtpProtocolDerived"), 0));
				break;
			}
	return;
	}
	
void CFtpProtocolDerived::DTPChannelXferNotification(const TDtpOperationCompletion aCompletionStatus)
/**
Notify of reception 
*/
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("CFtpProtocolDerived::DTPChannelRcvNotification called\n"));
	// notify upper layer a packet is ready
	switch(aCompletionStatus)
		{
		case MDTPChannelNotifier::EDtpRcvMoreData:
			FTPPROTDEBUG(_DBGFtpprot,_L("EDtpRcvMoreData\n"));
			iNotifier->ServerXFerNotification(MFtpProtocolNotifier::EPacketReceived);
			break;
		case MDTPChannelNotifier::EDtpSendEOFComplete:
			FTPPROTDEBUG(_DBGFtpprot,_L("EDtpSendEOFComplete\n"));
			UpdateState(EDtpChannelClosed);
			break;
		case MDTPChannelNotifier::EDtpRcvComplete:
			FTPPROTDEBUG(_DBGFtpprot,_L("EDtpRecvEOFComplete\n"));
			UpdateState(EDtpChannelClosed);
			break;
		case MDTPChannelNotifier::EDtpSendComplete:
			FTPPROTDEBUG(_DBGFtpprot,_L("EDtpSendComplete\n"));
			iNotifier->ServerXFerNotification(MFtpProtocolNotifier::EPacketSent);
			break;
		default:
			FTPPROTDEBUG(_DBGFtpprot,_L("WARNING !!!!! Unrecognised event\n"));
			break;
		}
	}

void CFtpProtocolDerived::FTPResolverNotifier(const TFTPResolverNotificationCode aCompletionStatus)
/**
Resolver callback
*/
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("CFtpProtocolDerived::DFTPResolverNotifier called"));
	__ASSERT_DEBUG(iState == ELookingUp, User::Panic(_L("CFtpProtocolDerived"), EPIPanicMegaOI));
	switch(aCompletionStatus)
		{
		case MFTPResolverNotifier::EDtpLookupComplete:
			FTPPROTDEBUG(_DBGFtpprot,_L("EDtpLookupComplete\n"));
			iState = EConnecting;
			iResolver->SetAddress(iRemoteAddress);
			Connect(iRemoteAddress);
			break;
		case MFTPResolverNotifier::EDtpLookupFailed:
			FTPPROTDEBUG(_DBGFtpprot,_L("EDtpLookupFailed\n"));
			//
			iNotifier->ErrorNotification(MFtpProtocolNotifier::EHostNotFound);
			iState = EIdle;

			break;
		}
	}

void CFtpProtocolDerived::SetErrorNotifier(const TInt aError)
/**
Define the CFTPSetError callback notifier
*/
{
	switch(aError)
		{
		case MFtpProtocolNotifier::ESocketError:
			iNotifier->ErrorNotification(MFtpProtocolNotifier::ESocketError);
			break;
		case MFtpProtocolNotifier::EOpCanceled:
			iNotifier->ErrorNotification(MFtpProtocolNotifier::EOpCanceled);
			break;
		default:
			__ASSERT_DEBUG(FALSE, User::Panic(_L("CFtpProtocolDerived"), EPIPanicMegaOI));
			break;
		}
}

//
//					FTP Private methods implementation 
//

void CFtpProtocolDerived::UpdateState(const TInternalEvents aEvent)
	{
#if defined(__FTPPROTDEBUG__)
	TBuf<4> debugBuffer;	//Placeholder for the answer I got from the FTP server
#endif
	FTPPROTDEBUG(_DBGFtpprot,_L("CFtpProtocolDerived::UpdateState\n"));
	// Do some preprocessing according to the event
	switch(aEvent)
		{
		case EFtpCodeReply:
			// Called when an answer has been parsed 
			FTPPROTDEBUG(_DBGFtpprot,_L("CFtpProtocolDerived::UpdateState called aEvent==EFtpCodeReply\n"));
			// Fetch the answer
			iFTPServerAnswerParser->ServerAnswer(iAnswer);
			//Display the answer for debug purpose
#if defined(__FTPPROTDEBUG__)
			debugBuffer.Copy(iAnswer);
			debugBuffer.Append(0);
			FTPPROTDEBUG(_DBGFtpprot,_L("Parsed server answer: <"));
			FTPPROTDEBUG(_DBGFtpprot,debugBuffer);
			FTPPROTDEBUG(_DBGFtpprot,_L(">\n"));
#endif
			// Check the answer to keep it in the allowed bounds
			if((iAnswer[0]<'1') || (iAnswer[0]>'5'))
				{
				// Answer out of bounds
				FTPPROTDEBUG(0xffff,_L("!!!!!!SERVER ANSWER OUT OF BOUND\n"));
				}
			break;
		default:
			break;
		}
	// Completion callback can only be sent when 
	// - PI Channel has been notified of success for the send operation
	// - An answer has been received and parsed
	// Accordingly, we bail out if any of these conditions is not
	// completed yet
	if (iAnswer.Length()<3)
		{
		FTPPROTDEBUG(_DBGFtpprot,_L("iAnswer.Length()<3\n"));
		return;
		}
	if (iPiChannel->Busy()) 
		{
		FTPPROTDEBUG(_DBGFtpprot,_L("iPiChannel->Busy()) \n"));
		return;
		}
	// Execute the next sequence of operations depending on our current state
	switch(iState)
		{
		// Simple commands as per FSM page 54 of RFC 959
		case EConnecting:
		case EPerformingAllo:
		case EPerformingDele:
		case EPerformingCwd:
		case EPerformingCdup:
		case EPerformingSmnt:
		case EPerformingHelp:
		case EPerformingMode:
		case EPerformingNoop:
		case EPerformingSite:
		case EPerformingPort:
		case EPerformingSyst:
		case EPerformingStat:
		case EPerformingRmd:
		case EPerformingMkd:
		case EPerformingPwd:
		case EPerformingStru:
		case EPerformingType:
			// Only allow a "2" answer
				if (iAnswer[0]=='2')
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
				else
					iNotifier->ServerNegativeAnswerNotification(MFtpProtocolNotifier::EOpFailed);
			break;
		case EPerformingQuit:
			// In any case close the PI Channel
			iPiChannel->Disconnect();
			// Only allow a "2" answer
				if (iAnswer[0]=='2')
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
				else
					iNotifier->ServerNegativeAnswerNotification(MFtpProtocolNotifier::EOpFailed);
			break;
		case EPerformingAbor:
			// rfc says, I might get a 426
			// then I'll get a 2xx to ack the ABORT
			// Just wait for a "2"
			// Note: Not checking for error here
			// If the server nack my abort, I'll hang and the user has to reset me
				if (iAnswer[0]=='2')
					{
					// Check the DtpChannel has been closed
					if(!iDtpChannel->Closed())
						{
						FTPPROTDEBUG(_DBGFtpprot,_L("!iDtpChannel->Closed()\n"));
						return;
						}
					// Channel closed and 2 answer - notify completion
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
					}
				else if (iAnswer[0]=='4')
					// Assume that if the server acknowledged my abort
					// request with a 4, it has already issued a FIN
					// I just reset the connection, otherwise I might have to 
					// wait for a long time to flush the queud packets
					iDtpChannel->Disconnect();
			break;
		case EPerformingPasv:
			// Only allow a "2" answer, verify we got a valid address to connect to
				if ((iAnswer[0]=='2') && (iFtpPASVAnswerParser->Fetch(iFTPDTPAddress)))
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
				else
					iNotifier->ServerNegativeAnswerNotification(MFtpProtocolNotifier::EOpFailed);
			break;
		// Commands defined as per 1st FSM page 55 of RFC 959
		case EPerformingList:
		case EPerformingNlst:
		case EPerformingRetr:
		case EPerformingStor:
		case EPerformingStou:
		// If We get an error close DTP channel, notify error and bail out
			switch(iAnswer[0])
				{
				case '1':
					// Ignore a 1 answer
					break;
				case '2':
					// Check the DtpChannel has been closed
					if(!iDtpChannel->Closed())
						{
						FTPPROTDEBUG(_DBGFtpprot,_L("!iDtpChannel->Closed()\n"));
						return;
						}
					// Channel closed and 2 answer - notify completion
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
					break;
				case '3':
				case '4':
				case '5':
				default:
					// If something else than 1-5 it really screwed up!!
					iDtpChannel->Disconnect();
					iNotifier->ServerNegativeAnswerNotification(MFtpProtocolNotifier::EOpFailed);
					break;
				}
			break;
		case EPerformingAppe:
		case EPerformingRein:
		// Not much to do, just ignore any "1" answer
			if (iAnswer[0]=='1')
				break;
			if (iAnswer[0]=='2')
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
				else
					iNotifier->ServerNegativeAnswerNotification(MFtpProtocolNotifier::EOpFailed);
			break;
		// rename sequence defined as per 2nd FSM page 55 of RFC 959
		case EPerformingRnfr:
				if (iAnswer[0]=='3')
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
				else
					iNotifier->ServerNegativeAnswerNotification(MFtpProtocolNotifier::EOpFailed);
			break;
		case EPerformingRnto:
				if (iAnswer[0]=='2')
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
				else
					iNotifier->ServerNegativeAnswerNotification(MFtpProtocolNotifier::EOpFailed);
			break;
		// Rest sequence as defined per FSM page 56 of RFC 959 
		case EPerformingRest:
				if (iAnswer[0]=='3')
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
				else
					iNotifier->ServerNegativeAnswerNotification(MFtpProtocolNotifier::EOpFailed);
			break;
		// Login sequence as defined by FSM page 57 of RFC 959 
		case EPerformingUser:
				if ((iAnswer[0]=='3') || 
					(iAnswer[0]=='2')) // Slight divergence from the RFC here, but some servers
					// allow disabling the passwd check and send back a 2 answer at this stage
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
				else
					iNotifier->ServerNegativeAnswerNotification(MFtpProtocolNotifier::EOpFailed);
			break;
		case EPerformingPass:
				if (iAnswer[0]=='2')
					iNotifier->ServerPositiveAnswerNotification(MFtpProtocolNotifier::EOpComplete);
				else
					iNotifier->ServerNegativeAnswerNotification(MFtpProtocolNotifier::EOpFailed);
			break;
		case EIdle:
			// Can't be here in Idle state
			__ASSERT_DEBUG(FALSE, User::Panic(_L("CFtpProtocolDerived"), EPIPanicMegaOI));
			break;
		default:
			// OOps, forgot something
			__ASSERT_DEBUG(FALSE, User::Panic(_L("CFtpProtocolDerived"), 0));
		}


	}
//
//					FTP Command interface implementation 
//

void CFtpProtocolDerived::SendBuffer(TDes8* aBuffer)
/**
Set a buffer for reception and initiate reception 
if the dtp channel is connected
*/
	{
	iIOBuffer = aBuffer;
	if(iDtpChannel->Connected())
		{
		iDtpChannel->Send(*iIOBuffer);
		}
	}

void CFtpProtocolDerived::SendEOF(void)
/**
Terminates the connection with the peer to mark the end of transfer.
*/
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::SendEOF called\n"));
	iDtpChannel->SendEOF();
	}

void CFtpProtocolDerived::RecvBuffer(TDes8* aBuffer)
/**
Set a buffer for reception and initiate reception if the dtp channel is connected.
*/
	{
	iIOBuffer = aBuffer;
	if(iDtpChannel->Connected())
		{
		iDtpChannel->Recv(*iIOBuffer);
		}
	}

void CFtpProtocolDerived::Connect(TSockAddr& aNetAddr)
/**
Establish a connection:
*/
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Connect called\n"));
	iState = EConnecting;
	iRemoteAddress = aNetAddr;
	// Reset the last answer
	iAnswer.Zero();
	if(!(iPiChannel->Connect(aNetAddr)))
		// PiChannel could not open a socket 
		// Post an error
		iCFTPSetError->SetError(MFtpProtocolNotifier::ESocketError);
	}

void CFtpProtocolDerived::Connect(const THostName& aServerName)
	{
	// DNS name
	Connect(aServerName,DEFAULT_SERVER_PI_PORT);
	}

void CFtpProtocolDerived::Connect(const THostName& aServerName, const TUint aPort)
	{
	// DNS name + port
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Connect called\n"));
	iState = ELookingUp;
	// Reset the last answer
	iAnswer.Zero();
	iRemoteAddress.SetPort(aPort);
	iResolver->Lookup(aServerName);
	} 

void CFtpProtocolDerived::User(const TDesC8& aParam)
/**
FTP commands, presented in the same order as RFC959:
*/
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::User called\n"));
	iFTPCmdBuffer = _L8("USER ");
	iFTPCmdBuffer.Append(aParam);
	iState = EPerformingUser;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Pass(const TDesC8& aParam)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Pass called\n"));
	iFTPCmdBuffer = _L8("PASS ");
	iFTPCmdBuffer.Append(aParam);
	iState = EPerformingPass;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Acct(const TDesC8& /*aParam*/)
	{
	iState = EPerformingAcct;
	}

void CFtpProtocolDerived::Cwd(const TDesC8& aParam)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Cwd called\n"));
	iFTPCmdBuffer = _L8("CWD ");
	iFTPCmdBuffer.Append(aParam);
	iState = EPerformingCwd;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Cdup(void)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Cdup called\n"));
	iFTPCmdBuffer = _L8("CDUP");
	iState = EPerformingCdup;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Smnt(const TDesC8& /*aParam*/)
	{
	iState = EPerformingSmnt;
	}

void CFtpProtocolDerived::Quit(void)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Quit called\n"));
	iFTPCmdBuffer = _L8("QUIT");
	iState = EPerformingQuit;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Rein(void)
	{
	iState = EPerformingRein;
	}

void CFtpProtocolDerived::Port(void)
	{// Sets the DTP port to one allocated by ESOCK
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Port called\n"));
	iState = EPerformingPort;
	iPort = iDtpChannel->ListeningPort();
	if ( iPort ==0)
		{
		// Some socket operations failed, post an async error
		iCFTPSetError->SetError(MFtpProtocolNotifier::ESocketError);
		// and bail out
		return;
		}
	if (iLocalAddress.Family() == KAfInet)
		{
		FTPPROTDEBUG(_DBGFtpprot,_L("***Using PORT\n"));
		BuildPORTCommand();
		}
	else
		{
		__ASSERT_DEBUG(iLocalAddress.Family() == KAfInet6, User::Panic(_L("CFtpProtocolDerived"), EAddressFamily));
		FTPPROTDEBUG(_DBGFtpprot,_L("***Using EPRT\n"));
		BuildEPRTCommand();
		}
	//PG 12/08/1999 following won't build, flogger doesn't take TDes8
	//FTPPROTDEBUG(_DBGFtpprot,_L("Sending command -->"));
	//FTPPROTDEBUG(_DBGFtpprot,iFTPCmdBuffer);
	//FTPPROTDEBUG(_DBGFtpprot,_L("\n"));
	// Set the PASV switch
	iPASVMode = FALSE;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::BuildPORTCommand()
	{
	iFTPCmdBuffer = _L8("PORT ");
	iFTPCmdBuffer.AppendNum(((iLocalAddress.Address())>>24), EDecimal);
	iFTPCmdBuffer.Append(_L(","));
	iFTPCmdBuffer.AppendNum((((iLocalAddress.Address()) & 0xFF0000) >>16), EDecimal);
	iFTPCmdBuffer.Append(_L(","));
	iFTPCmdBuffer.AppendNum((((iLocalAddress.Address()) & 0xFF00) >>8), EDecimal);
	iFTPCmdBuffer.Append(_L(","));
	iFTPCmdBuffer.AppendNum(((iLocalAddress.Address()) & 0xFF), EDecimal);
	iFTPCmdBuffer.Append(_L(","));
	iFTPCmdBuffer.AppendNum((((iPort) & 0xFF00) >>8), EDecimal);
	iFTPCmdBuffer.Append(_L(","));
	iFTPCmdBuffer.AppendNum(((iPort) & 0xFF), EDecimal);
	}

void CFtpProtocolDerived::BuildEPRTCommand()
	{
	TBuf<39> addrBuf;
	iLocalAddress.Output(addrBuf);
	
	iFTPCmdBuffer = _L8("EPRT ");
	iFTPCmdBuffer.Append(_L("|2|"));
	iFTPCmdBuffer.Append(addrBuf);
	iFTPCmdBuffer.Append(_L("|"));
	iFTPCmdBuffer.AppendNum(iPort, EDecimal);
	iFTPCmdBuffer.Append(_L("|"));
	}

void CFtpProtocolDerived::Port(TUint /*aPort*/)
	{
	// Sets the DTP port to a specific one
	}

void CFtpProtocolDerived::Pasv(void)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Pasv called\n"));
	if (iLocalAddress.Family() == KAfInet)
		{
		FTPPROTDEBUG(_DBGFtpprot,_L("***Using PASV\n"));
		iFTPCmdBuffer = _L8("PASV");
		}
	else
		{
		FTPPROTDEBUG(_DBGFtpprot,_L("***Using EPSV\n"));
		iFTPCmdBuffer = _L8("EPSV");
		}

	iState = EPerformingPasv;
	// Reset the last answer
	iAnswer.Zero();
	// Reset the Pasv Address Parser
	iFtpPASVAnswerParser->Reset();
	// Set the bool switch
	iPASVMode = TRUE;
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Type(const TDesC8& aParam)
	{
	iState = EPerformingType;
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Type called\n"));
	iFTPCmdBuffer = _L8("TYPE ");
	iFTPCmdBuffer.Append(aParam);
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Type(const TDesC8& /*aParam1*/, const TDesC8& /*aParam2*/)
	{
	iState = EPerformingType;
	}

void CFtpProtocolDerived::Stru(const TDesC8& /*aParam*/)
	{
	iState = EPerformingStru;
	}

void CFtpProtocolDerived::Mode(const TDesC8& /*aParam*/)
	{
	iState = EPerformingMode;
	}

void CFtpProtocolDerived::Retr(const TDesC8& aFileName)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Retr called\n"));
	iFTPCmdBuffer = _L8("RETR ");
	iFTPCmdBuffer.Append(aFileName);
	iState = EPerformingRetr;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	if(iPASVMode)
		{
		if (!(iDtpChannel->Connect(iFTPDTPAddress)))
			{
			// Some socket operations failed, post an async error
			iCFTPSetError->SetError(MFtpProtocolNotifier::ESocketError);
			return;
			}

		}
	else
		{
		// For non passive mode accept server connection
		iDtpChannel->Accept();
		}
	}

void CFtpProtocolDerived::Stor(const TDesC8& aFileName)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Stor called\n"));
	iFTPCmdBuffer = _L8("STOR ");
	iFTPCmdBuffer.Append(aFileName);
	iState = EPerformingStor;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	if(iPASVMode)
		{
		if (!(iDtpChannel->Connect(iFTPDTPAddress)))
			{
			// Some socket operations failed, post an async error
			iCFTPSetError->SetError(MFtpProtocolNotifier::ESocketError);
			return;
			}
		}
	else
		{
		// For non passive mode accept server connection
		iDtpChannel->Accept();
		}
	}

void CFtpProtocolDerived::List(void)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::List called\n"));
	iFTPCmdBuffer = _L8("LIST");
	iState = EPerformingList;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	if(iPASVMode)
		{
		if (!(iDtpChannel->Connect(iFTPDTPAddress)))
			{
			// Some socket operations failed, post an async error
			iCFTPSetError->SetError(MFtpProtocolNotifier::ESocketError);
			return;
			}
		}
	else
		{
		// For non passive mode accept server connection
		iDtpChannel->Accept();
		}
	}

void CFtpProtocolDerived::List(const TDesC8& aParam)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::List called\n"));
	iFTPCmdBuffer = _L8("LIST ");
	iFTPCmdBuffer.Append(aParam);
	iState = EPerformingList;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	if(iPASVMode)
		{
		if (!(iDtpChannel->Connect(iFTPDTPAddress)))
			{
			// Some socket operations failed, post an async error
			iCFTPSetError->SetError(MFtpProtocolNotifier::ESocketError);
			return;
			}
		}
	else
		{
		// For non passive mode accept server connection
		iDtpChannel->Accept();
		}
	}

void CFtpProtocolDerived::Stou(void)
	{
	iState = EPerformingStou;
	}

void CFtpProtocolDerived::Appe(const TDesC8& /*aFileName*/)
	{
	iState = EPerformingAppe;
	}

void CFtpProtocolDerived::Allo(const TDesC8& /*aParam*/)
	{
	iState = EPerformingAllo;
	}

void CFtpProtocolDerived::Allo(const TDesC8& /*aParam1*/, const TDesC8& /*aParam2*/)
	{
	iState = EPerformingAllo;
	}

void CFtpProtocolDerived::Rest(const TDesC8& aParam)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Rest called\n"));
	iFTPCmdBuffer = _L8("REST ");
	iFTPCmdBuffer.Append(aParam);
	iState = EPerformingRest;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Rnfr(const TDesC8& aFileName)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Rnfr called\n"));
	iFTPCmdBuffer = _L8("RNFR ");
	iFTPCmdBuffer.Append(aFileName);
	iState = EPerformingRnfr;
	// Reset the last answer
	iAnswer.Zero();
	// Get ready to receive an answer
//	iPiChannel->GetNextAnswer(iServerAnswerBuffer);
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Rnto(const TDesC8& aFileName)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Rnto called\n"));
	iFTPCmdBuffer = _L8("RNTO ");
	iFTPCmdBuffer.Append(aFileName);
	iState = EPerformingRnto;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Abor(void)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Abor called\n"));
	iFTPCmdBuffer.Zero();
	iFTPCmdBuffer.Append(IAC);
	iFTPCmdBuffer.Append(IP); 
	iFTPCmdBuffer.Append(IAC);
	iFTPCmdBuffer.Append(SYNCH);
	iFTPCmdBuffer.Append(_L8("ABOR"));
	iState = EPerformingAbor;
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer,KSockWriteUrgent);
	}

void CFtpProtocolDerived::Dele(const TDesC8& aFileName)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Dele called\n"));
	iFTPCmdBuffer = _L8("DELE ");
	iFTPCmdBuffer.Append(aFileName);
	iState = EPerformingDele;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Rmd(const TDesC8& aParam)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Rmd called\n"));
	iFTPCmdBuffer = _L8("RMD ");
	iFTPCmdBuffer.Append(aParam);
	iState = EPerformingRmd;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Mkd(const TDesC8& aParam)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Mkd called\n"));
	iFTPCmdBuffer = _L8("MKD ");
	iFTPCmdBuffer.Append(aParam);
	iState = EPerformingMkd;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Pwd(void)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Pwd called\n"));
	iFTPCmdBuffer = _L8("PWD");
	iState = EPerformingPwd;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Nlst(void)
	{
	iState = EPerformingNlst;
	}

void CFtpProtocolDerived::Nlst(const TDesC8& /*aParam*/)
	{
	iState = EPerformingNlst;
	}

void CFtpProtocolDerived::Site(const TDesC8& /*aParam*/)
	{
	iState = EPerformingSite;
	}

void CFtpProtocolDerived::Syst(void)
	{
	iState = EPerformingSyst;
	}

void CFtpProtocolDerived::Stat(const TDesC8& /*aParam*/)
	{
	iState = EPerformingStat;
	}

void CFtpProtocolDerived::Stat(void)
	{
	iState = EPerformingStat;
	}

void CFtpProtocolDerived::Help(const TDesC8& /*aParam*/)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Help called\n"));
	iFTPCmdBuffer = _L8("HELP");
	iState = EPerformingHelp;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Help(void)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Help called\n"));
	iFTPCmdBuffer = _L8("HELP");
	iState = EPerformingHelp;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::Noop(void)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::Help called\n"));
	iFTPCmdBuffer = _L8("NOOP");
	iState = EPerformingNoop;
	// Reset the last answer
	iAnswer.Zero();
	// Fire the command
	iPiChannel->SendCommand(iFTPCmdBuffer);
	}

void CFtpProtocolDerived::UserCancel(void)
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("***CFtpProtocolDerived::UserCancel called "));
	switch(iState)
		{
		case ELookingUp:
			FTPPROTDEBUG(_DBGFtpprot,_L("State: ELookingUp\n"));
			iResolver->Cancel();
			break;
		case EConnecting:
			FTPPROTDEBUG(_DBGFtpprot,_L("State: EConnecting\n"));
			iPiChannel->Disconnect();
			break;
		default:
			FTPPROTDEBUG(_DBGFtpprot,_L("Do Nothing\n"));
			break;
		}
	}


//Extensions: 

EXPORT_C TUint32 CFtpProtocol::GetVersion(void)
/** Gets the API version number.
*
* @return	32-bit number: with MAJOR_VERSION in the highest byte, 
* 			MINOR_VERSION in the next byte, and BUILD_NUMBER in the lowest two bytes
* 			i.e. MAJOR 2, MINOR 0x34, BUILD 0x278 would be "ver 2.52, build 632".
*/
	{
		return FTPPROTDLL_VERSION_NUMBER;
	}

EXPORT_C CFtpProtocol *CFtpProtocol::NewL( MFtpProtocolNotifier* aNotifier)
/** Allocates and constructs a new FTP engine object.
*
* @param aNotifier	Client callback interface. 
* 					The FTP engine calls this interface to pass server responses and 
* 					status messages to the client.
* @return			New FTP engine object.
*/
	{
	return CFtpProtocolDerived::NewL(aNotifier);
	}
	
CFtpProtocolDerived *CFtpProtocolDerived::NewL( MFtpProtocolNotifier* aNotifier)
	{
	CFtpProtocolDerived* self = new (ELeave) CFtpProtocolDerived(aNotifier);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CFtpProtocolDerived::CFtpProtocolDerived(MFtpProtocolNotifier* aNotifier
									):iNotifier(aNotifier)
	{
	}

void CFtpProtocolDerived::ConstructL(void)
	{
	InitCommL();
	iCFTPSetError = CFTPSetError::NewL(this);
	iPiChannel = CPIChannel::NewL(this,iSockServ);
	iDtpChannel = CDTPChannel::NewL(this,iCFTPSetError,iSockServ);
	iResolver = CFTPResolver::NewL(this,iSockServ);
	iFTPServerAnswerParser = new (ELeave) TFTPServerAnswerParser();
	iFtpPASVAnswerParser = new (ELeave) TFtpPASVAnswerParser();
	// Reset the last answer
	iAnswer.Zero();
	}

CFtpProtocol::~CFtpProtocol()
/** Destructor. */
	{}
	
CFtpProtocolDerived::~CFtpProtocolDerived()
	{
	FTPPROTDEBUG(_DBGFtpprot,_L("CFtpProtocolDerived::~CFtpProtocolDerived called\n"));
	delete  iPiChannel;
	delete  iDtpChannel;
	delete  iResolver;
	delete	iCFTPSetError;
	delete  iFTPServerAnswerParser;
	delete  iFtpPASVAnswerParser;
	iSockServ.Close();		
	}

void CFtpProtocolDerived::InitCommL(void)
	{
	// Initialise Comm modules
 	// When bootstrapping C32 we have to avoid the PhBkSyncServer being started, since
 	// it needs a different CommDB
 	_LIT(KPhbkSyncCMI, "phbsync.cmi");
    StartC32WithCMISuppressions(KPhbkSyncCMI);
	// Open socket server
	User::LeaveIfError(iSockServ.Connect());
	}

