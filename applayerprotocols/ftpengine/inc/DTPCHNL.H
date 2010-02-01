/**
* Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* FTP protocol engine
* Author:	Philippe Gabriel
* Implements objet controling the DTP channel of the FTP protocol
* Model the DTP Channel as an FSM
* 
*
*/



/**
 @file DTPCHNL.H
 @internalComponent
*/

#if !defined(__DTPCHNL_H__)
#define __DTPCHNL_H__
#include "DEBUG.H"
#include "FTPDEF.H"
#include "SETERROR.H"
#include <e32base.h>
#include <in_sock.h>

//////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////

class CFtpProtocol;
class MDTPChannelNotifier
/**
@internalComponent
*/
	{
public:
/**
Operation completion return codes.
*/ 
	enum TDtpOperationCompletion 
		{
		EDtpConnectComplete=0, 
		EDtpAcceptComplete, 
		EDtpDisconnectComplete,
		EDtpSendComplete,	
		EDtpSendEOFComplete,	
		EDtpRcvMoreData,	
		EDtpRcvComplete,	
		EDtpOpCancelComplete
		};
/**
Error status
*/
	enum TDtpOperationError 
		{
		EDtpConnectFailed=0,
		EDtpSendAborted,
		EDtpRecvAborted
		};
public:
		/** Notify of normal completion of an operation */
		virtual void DTPChannelOperationCompletion(const TDtpOperationCompletion)=0;
		/** Notify of error performing an operation */
		virtual void DTPChannelOperationError(const TDtpOperationError)=0;
		/** Notify of reception */
		virtual void DTPChannelXferNotification(const TDtpOperationCompletion)=0;
	};

NONSHARABLE_CLASS(CDTPChannel) : public CActive
/**
DTP Channel
@internalComponent
*/
	{
public:

	CDTPChannel(MDTPChannelNotifier*,CFTPSetError*);
	~CDTPChannel();
	static CDTPChannel* NewL(MDTPChannelNotifier*,CFTPSetError*,RSocketServ&);
	/** Command interface .Connect DTP socket*/
	TBool Connect(TSockAddr&); 
	/** Open the DTP channel and wait for connection */
	void Accept(void); 
	/** returns the port the socket listens on */
	TUint ListeningPort(void); 
	void Disconnect(void); 
	/** Send the content of aBuffer down the DTP Channel */
	void Send(TDesC8&); 
	/** Shutdown the DTP connection to notify an end of send */
	void SendEOF(void); 
	/** Rcv from DTP Channel and store in aBuffer */
	void Recv(TDes8&); 
	/** returns state of DTP Channel */
	TBool Closed(void); 
	/** returns state of DTP Channel */
	TBool Connected(void); 
protected:
	void RunL();
    void DoCancel(void);
private:
	enum TState
		{
		EDTPChannelIdle,EDTPChannelConnecting,EDTPChannelListening,EDTPChannelConnected,
		EDTPChannelShutingDown,EDTPChannelSending,EDTPChannelReceiving
		};
	enum TPanic 
		{
		EDTPPanicChannelNotConnected = -1,
		EDTPPanicChannelOutOfState = -2
		};
	void ConstructL(RSocketServ&);
	/** The state of the FSM */
	TState iDTPChannelState; 
	/** A socket server to be used for all socket operations */
	RSocketServ iSockServ;	
	
	RSocket iDTPTransferSocket;
	RSocket iDTPListenSocket;
	MDTPChannelNotifier* iNotifier;
	//	CFtpProtocol* iFtpProtocol;
	/** A pointer to a CFTPSetError object */
	CFTPSetError* iCFTPSetError;
	TInetAddr iLocalAddress;
	TSockXfrLength iRcvLen;
	};
#endif // __DTPCHNL_H__
