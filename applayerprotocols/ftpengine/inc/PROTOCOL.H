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
* FTP Protocol header file
* Author:	Philippe Gabriel
* 
*
*/



/**
 @file protocol.h
 @internalComponent
*/

#if !defined(__PROTOCOL_H__)
#define __PROTOCOL_H__

#include <e32base.h>
#include <es_sock.h>
#include <ftpprot.h>
#include "FTPDEF.H"
#include "DEBUG.H"
#include "PICHNL.H"
#include "DTPCHNL.H"
#include "RESOLVER.H"
#include "SETERROR.H"



class TFTPServerAnswerParser;
class TFtpPASVAnswerParser;

NONSHARABLE_CLASS(CFtpProtocolDerived) : public CFtpProtocol,public MPIChannelNotifier,MDTPChannelNotifier,MFTPResolverNotifier,MSetErrorNotifier
/**
FtpEngine
@internalComponent
*/
	{
friend class CDTPChannel;
friend class CPIChannel;
public:
	/** Establish a connection: IP address */
	void Connect(TSockAddr& aNetAddr);	
	/** URL name */
	void Connect(const THostName& aServerName);  
	/** URL name + port */
	void Connect(const THostName& aServerName, const TUint aPort); 

	/** FTP commands, presented in the same order as RFC959: */
	void User(const TDesC8& aParam);
	void Pass(const TDesC8& aParam);
	void Acct(const TDesC8& aParam);
	void Cwd(const TDesC8& aParam);
	void Cdup(void);
	void Smnt(const TDesC8& aParam);
	void Quit(void);
	void Rein(void);
	/** Sets the DTP port to one allocated by ESOCK */
	void Port(void);	
	/** Sets the DTP port to a specific one */
	void Port(TUint aPort);	
	void Pasv(void);
	void Type(const TDesC8& aParam);
	void Type(const TDesC8& aParam1, const TDesC8& aParam2);
	void Stru(const TDesC8& aParam);
	void Mode(const TDesC8& aParam);
	void Retr(const TDesC8& aFileName);
	void Stor(const TDesC8& aFileName);
	void Stou(void);
	void Appe(const TDesC8& aFileName);
	void Allo(const TDesC8& aParam);
	void Allo(const TDesC8& aParam1, const TDesC8& aParam2);
	void Rest(const TDesC8& aParam);
	void Rnfr(const TDesC8& aFileName);
	void Rnto(const TDesC8& aFileName);
	void Abor(void);
	void Dele(const TDesC8& aFileName);
	void Rmd(const TDesC8& aParam);
	void Mkd(const TDesC8& aParam);
	void Pwd(void);
	void List(void);
	void List(const TDesC8& aParam);
	void Nlst(void);
	void Nlst(const TDesC8& aParam);
	void Site(const TDesC8& aParam);
	void Syst(void);
	void Stat(const TDesC8& aParam);
	void Stat(void);
	void Help(const TDesC8& aParam);
	void Help(void);
	void Noop(void);
	/** 
	Buffer management for transfer
	Following functions pass a pointer to a buffer
	to transfer data to/from the Dtp channel
	Before an operation which cause data to transit on the 
	DTP channel to occur (Nlst, List, Retr, Stor)
	a Buffer must be provided with the following api
	Also when the client is notified of a MoreData event
	It must reissue the following operation to get the rest of
	the data
	*/
	void SendBuffer(TDes8* aBuffer);
	void RecvBuffer(TDes8* aBuffer);
	/** Finishes the transfer initiated by a stor command */
	void SendEOF(void); 
	/** PI Channel Minterface */
	void PIChannelOperationCompletion(const TPiOperationCompletion);
	/** Notify of error performing an operation */
	void PIChannelOperationError(const TPiOperationError);
	/** Notify of reception */
	void PIChannelRcvNotification(void);
	/** Notify server has unexpectidly dropped the connection */
	void PIChannelReset(void);
	/** DTP Channel Minterface */
	/** Notify of normal completion of an operation */
	void DTPChannelOperationCompletion(const TDtpOperationCompletion);
	/** Notify of error performing an operation */
	void DTPChannelOperationError(const TDtpOperationError);
	/** Notify of reception */
	void DTPChannelXferNotification(const TDtpOperationCompletion);
	/** FTP DNS resolver Minterface */
	void FTPResolverNotifier(const TFTPResolverNotificationCode);
	/** Copies the 3 digits answer received from the FTP server */
	void FTPServerAnswer(TDes& aServerAnswer){aServerAnswer.Copy(iAnswer);}
	/** Define the CFTPSetError callback notifier */
	void SetErrorNotifier(const TInt);

	void UserCancel(void);
	~CFtpProtocolDerived();
	static CFtpProtocolDerived *NewL(MFtpProtocolNotifier*);

protected:
	CFtpProtocolDerived(MFtpProtocolNotifier* aNotifier);

    void ConstructL(void);
	/** Address of the remote FTP server */
	TInetAddr iRemoteAddress; 
	/** A Buffer to receive server answers */
	TBuf8<512> iServerAnswerBuffer; 
private:
	enum TState 
		{
	EIdle,
	ELookingUp,
	EConnecting,
	EPerformingUser,EPerformingPass,EPerformingAcct,EPerformingCwd,EPerformingCdup,
	EPerformingSmnt,EPerformingQuit,EPerformingRein,EPerformingPort,
	EPerformingPasv,EPerformingType,EPerformingStru,EPerformingMode,EPerformingRetr,
	EPerformingStor,EPerformingStou,EPerformingAppe,EPerformingAllo,EPerformingRest,
	EPerformingRnfr,EPerformingRnto,EPerformingDele,EPerformingRmd,
	EPerformingMkd,EPerformingPwd,EPerformingList,EPerformingNlst,EPerformingSite,
	EPerformingSyst,EPerformingStat,EPerformingHelp,EPerformingNoop,
	EPerformingAbor
		};
	enum TInternalEvents
		{
		EFtpCodeReply,
		EDtpChannelConnected,
		EDtpChannelClosed,
		EPiChannelConnectComplete,
		EPiChannelSendComplete,
		EPacketReceived,
		/** Error events
		*/
		EPiReset,
		EDtpReset,
		};
	enum TPanic 
		{
		EPIPanicParserFailure = -1,
		EPIPanicMegaOI = -2,
		EAddressFamily = -3
		};
	void InitCommL(void);
	//void FTPReply(void);
	void UpdateState(const TInternalEvents aEvent);
	TInt	iState;
	MFtpProtocolNotifier*	iNotifier;
	TFTPServerAnswerParser*	iFTPServerAnswerParser;
	TFtpPASVAnswerParser*	iFtpPASVAnswerParser;
	CPIChannel* iPiChannel;
	CDTPChannel* iDtpChannel;
	CFTPResolver* iResolver;
	/** A pointer to the CFTPSetError object */
	CFTPSetError*	iCFTPSetError;
	/** A socket server to be used for */
	RSocketServ iSockServ;	
	/** all socket operations */
	/** my local address (to advertise to the server) */
	TInetAddr iLocalAddress; 
	/** FTP Connection can be passive (server connects on one of my ports) */
	TBool iPASVMode;
	/** Port number for the DTP Channel (to advertise to the server)
		or active, FTP server gives me an address to connect to */
	TUint	iPort; 
	/** Address of the FTP channel (note: RFC says this 
	    IP address can be different than the PI Channel IP address */
	TInetAddr iFTPDTPAddress; 
	/** Placeholder for the answer I got from the FTP server */
	TBuf8<3> iAnswer;	
	/** A Buffer to construct FTP commands */
	TBuf8<256> iFTPCmdBuffer; 
	/** A Buffer to construct FTP commands */
	TDes8* iIOBuffer; 
	/** helper to Port() */
	void BuildPORTCommand(); 
	/** helper to Port() */
	void BuildEPRTCommand(); 
	};
#endif //__PROTOCOL_H__
