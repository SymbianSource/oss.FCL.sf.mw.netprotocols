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
* Implements objet controling the PI channel of the FTP protocol
* Model the PI Channel as an FSM
* Implements objet controling the PI channel of the FTP protocol
* Model the PI Channel as an FSM
* The FSM has 7 states:
* Idle, Connecting, Disconnecting, Connected, Sending, Parsing
* ParsingSending
* Transitions between states are triggered by PI methods and events
* coming from ESOCK
* 
*
*/



/**
 @file PICHNL.CPP
 @internalComponent
*/

#if !defined(__PICHNL_H__)
#define __PICHNL_H__
#include "DEBUG.H"
#include "FTPDEF.H"
#include <e32base.h>
#include <in_sock.h>

//////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////
class CFtpProtocol;
class MPIChannelNotifier
/**
@internalComponent
*/
	{
public:
/**
Operation completion return codes.
*/ 
	enum TPiOperationCompletion 
		{
		EPiConnectComplete=0, 
		EPiDisconnectComplete,
		EPiSendComplete,	
		EPiOpCancelComplete
		};
/**
Error status
*/
	enum TPiOperationError 
		{
		EPiConnectFailed=0,
		EPiConnectionReset,
		EPiDisconnectFailed,
		EPiSendFailed,
		EPiCancelFailed
		};
public:
		/** Notify of normal completion of an operation */
		virtual void PIChannelOperationCompletion(const TPiOperationCompletion)=0;
		/** Notify of error performing an operation */
		virtual void PIChannelOperationError(const TPiOperationError)=0;
		/** Notify of reception  */
		virtual void PIChannelRcvNotification(void)=0;
	};

class CPIChannel;
 
NONSHARABLE_CLASS(CPISocketRead) : public CActive
/**
Active object waiting for incoming data
@internalComponent
*/
	{
public:
	static CPISocketRead* NewL(MPIChannelNotifier* aNotifier,
								CPIChannel*	aPIChannel);
	void ConstructL();
	/** CActive */
	void DoCancel(); 
	/** CActive */
	void RunL();	
	void IssueRead(TDes8& aBuffer);
	void SetSocket(RSocket* aSocket){iSocket = aSocket;}
	
protected:
	CPISocketRead(MPIChannelNotifier*,CPIChannel*);

private:
	RSocket* iSocket;
	MPIChannelNotifier* iNotifier;
	TSockXfrLength iRcvLen;
	CPIChannel*	iPIChannel;
	};

 
NONSHARABLE_CLASS(CPIChannel) : public CActive
/**
PI Channel
@internalComponent
*/
	{
friend class CPISocketRead;
public:

	CPIChannel(MPIChannelNotifier*);
	~CPIChannel();
	static CPIChannel* NewL(MPIChannelNotifier*,RSocketServ&);
	/** Command interface */
	/** Connect PI socket */
	TBool Connect(TSockAddr& aNetAddr); 
	void GetLocalAddress(TInetAddr& aLocalAddr); 
	/** Disconnect PI socket */
	void Disconnect(void); 
	/** Returns state of DTP Channel */
	TBool Busy(void); 
	/** Send a command on the PI Channel */
	void SendCommand(TDes8& aCommand,TUint aSomeFlags=0,TBool aSendCRLF=TRUE); 
	/** Fetch the reply sent by the server */
	void GetNextAnswer(TDes8&	aBuffer);
protected:
	void RunL();
    void DoCancel(void);
private:
	enum TState
		{
		EPIChannelIdle,EPIChannelConnecting,EPIChannelDisconnecting,
		EPIChannelConnected,EPIChannelSending
		};
	enum TPanic 
		{
		EPIPanicChannelConnected = -1,
		EPIPanicOutOfState = -2,
		EPIPanicChannelNotConnected = -3
		};
	void ConstructL(RSocketServ& aSockServ);
	/** The state of the FSM */
	TState iPIChannelState; 
	/** A socket server to be used for all socket operations */
	RSocketServ iSockServ;	
	RSocket iPiSocket;
	/** Last code sent by the FTP server */
	TUint32	iPIServerCode;	
	/** Active object receiving data */
	CPISocketRead *iReceiver; 
	MPIChannelNotifier* iNotifier;
	};
#endif // __PICHNL_H__
