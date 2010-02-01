/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file ftprot.h
 @internalComponent
*/

#if !defined(__FTPPROT_H__)
#define __FTPPROT_H__

#include <e32base.h>
#include <es_sock.h>

/** FTPPROT.DLL major version number.
@internalComponent */
#define FTPPROTDLL_VERSION_MAJOR 0x01 // The very first release
/** FTPPROT.DLL minor version number. */
#define FTPPROTDLL_VERSION_MINOR 0x03
/** FTPPROT.DLL version number. */
#define FTPPROTDLL_VERSION_NUMBER (FTPPROTDLL_VERSION_MAJOR<<8)|FTPPROTDLL_VERSION_MINOR

class MFtpProtocolNotifier
/** FTP engine callback interface.
*
* An FTP engine client implements this interface to receive status and results 
* from asynchronous FTP commands.
* @internalComponent */
	{
// Operation completion return codes.
	
public:
/** FTP engine/session operation completeness codes. */
	enum TOpComp 
	{
	/** Operation completed normally. */
	EOpComplete=0,
	/** Operation cancelled. */
	EOpCanceled,	// User canceled last operation
	/** Operation failed. */
	EOpFailed, 
	/** Sockets level error. */
	ESocketError, 

	// Connection errors
	/** Connection error: Connection reset. */
	EOpConnectionReset,
	/** Connection error: Connection failed. */
	EOpConnectionFailed,	
	/** Connection error: Server not found. */
	EHostNotFound,

	// Transfer error 
	/** Transfer error: Transfer was reset. */
	EXferReset,
	/** Transfer error: Transfer is not initialised. */	
	EXferNotInitialised,

	//Transfer notification
	/** Transfer notification: Data packet was received. */
	EPacketReceived,
	/** Transfer notification: Data packet was sent. */	
	EPacketSent
	};
public:
	/** Destructor. */
	virtual ~MFtpProtocolNotifier(){};

	/** Positive reply received from server.
	*
	* @param aStatus	Operation completion code */
	virtual void ServerPositiveAnswerNotification(const TOpComp aStatus)=0;


	/** Message sent by the FTP server.
	*
	* This returns the full server reply in plain text format.
	* 
	* @param aMessage	The message sent by the server */
	virtual void ServerMessage(const TDesC8& aMessage)=0;

	/** Data transfer notification received from the server.
	*
	* @param aStatus	Operation completion code */
	virtual void ServerXFerNotification(const TOpComp aStatus)=0;

	/** Negative reply received from server.
	* 
	* @param aStatus	Operation completion code */
	virtual void ServerNegativeAnswerNotification(const TOpComp aStatus)=0;

	/** Error condition notification.
	* 
	* @param aStatus	Operation completion code */
	virtual void ErrorNotification(const TOpComp aStatus)=0;
	};


class CFtpProtocol : public CBase
/** Implements an FTP engine, and allows the client to access the individual FTP 
* commands as defined in RFC959.
* 
* Note that before commands that cause data to transit on the DTP channel (e.g. NLST, 
* LIST, RETR, STOR) a data buffer must be provided using SendBuffer()/RecvBuffer(). 
* Also, when the client is notified of a MoreData() event, it must re-issue RecvBuffer() 
* to get the rest of the data.
* @internalComponent
*/
	{
public:

// Establish a connection:
	/** Connect to an FTP server, specifying an IP address.	
	* 
	* @param aNetAddr	FTP server's IP address */
	virtual void Connect(TSockAddr& aNetAddr)=0;	// IP address

	/** Connect to an FTP server, specifying a DNS name.
	* 
	* @param aServerName	FTP server's DNS name */
	virtual void Connect(const THostName& aServerName)=0;  // URL name

	/** Connect to an FTP server, specifying a DNS name and port number.
	* 
	* @param aServerName	FTP server's DNS name
	* @param aPort			FTP server's port */
	virtual void Connect(const THostName& aServerName, const TUint aPort)=0; // URL name + port

// FTP commands, presented in the same order as RFC959:
	/** Issues the USER command.
	* 
	* @param aParam	Telnet string identifying the user */
	virtual void User(const TDesC8& aParam)=0;

	/** Issues the PASS command.
	* 
	* @param aParam	Telnet string specifying the user's password */
	virtual void Pass(const TDesC8& aParam)=0;

	/** Issues the ACCT command.
	* 
	* @param aParam	Telnet string identifying the user's account */
	virtual void Acct(const TDesC8& aParam)=0;

	/** Issues the CWD command.
	* 
	* @param aParam	Directory or other system dependent file group designator */
	virtual void Cwd(const TDesC8& aParam)=0;

	/** Issues the CDUP command. */
	virtual void Cdup(void)=0;

	/** Issues the SMNT command.
	* 
	* @param aParam	Pathname specifying a directory or other system dependent file 
	* 				group designator */
	virtual void Smnt(const TDesC8& aParam)=0;

	/** Issues the QUIT command. */
	virtual void Quit(void)=0;

	/** Issues the REIN command. */
	virtual void Rein(void)=0;

	/** Issues the PORT command, setting the Data Transfer Process port to a value 
	* allocated by the Sockets Server. */
	virtual void Port(void)=0;			// Sets the DTP port to one allocated by ESOCK

	/** Issues the PORT command, specifying a port number.
	* 
	* @param aPort	Port number */
	virtual void Port(TUint aPort)=0;	// Sets the DTP port to a specific one

	/** Issues the PASV command. */
	virtual void Pasv(void)=0;

	/** Issues the TYPE command (single parameter).
	* 
	* @param aParam	First representation type parameter */
	virtual void Type(const TDesC8& aParam)=0;

	/** Issues the TYPE command (two parameters).
	* 
	* @param aParam1	First representation type parameter
	* @param aParam2	Second representation type parameter */
	virtual void Type(const TDesC8& aParam1, const TDesC8& aParam2)=0;

	/** Issues the STRU command.
	* 
	* @param aParam	Telnet character code specifying the file structure */
	virtual void Stru(const TDesC8& aParam)=0;

	/** Issues the MODE command.
	* 
	* @param aParam	Telnet character code specifying the data transfer mode */
	virtual void Mode(const TDesC8& aParam)=0;

	/** Issues the RETR command.
	* 
	* @param aFileName	File name */
	virtual void Retr(const TDesC8& aFileName)=0;

	/** Issues the STOR command.
	* 
	* @param aFileName	File name */
	virtual void Stor(const TDesC8& aFileName)=0;

	/** Issues the STOU command. */
	virtual void Stou(void)=0;

	/** Issues the APPE command.
	* 
	* @param aFileName	File name */
	virtual void Appe(const TDesC8& aFileName)=0;

	/** Issues the ALLO command (single parameter).
	* 
	* @param aParam	Number of bytes (using the logical byte size) of storage to 
	* 				be reserved for the file */
	virtual void Allo(const TDesC8& aParam)=0;

	/** Issues the ALLO command (two parameters).
	* 
	* @param aParam1	Number of bytes (using the logical byte size) of storage to 
	* 					be reserved for the file
	* @param aParam2	Maximum record or page size (in logical bytes) */
	virtual void Allo(const TDesC8& aParam1, const TDesC8& aParam2)=0;

	/** Issues the REST command.
	* 
	* @param aParam	The server marker at which file transfer is to be restarted */
	virtual void Rest(const TDesC8& aParam)=0;

	/** Issues the RNFR command.
	* 
	* @param aFileName	File name */
	virtual void Rnfr(const TDesC8& aFileName)=0;

	/** Issues the RNTO command.
	* 
	* @param aFileName	File name */
	virtual void Rnto(const TDesC8& aFileName)=0;

	/** Issues the ABOR command. */
	virtual void Abor(void)=0;

	/** Issues the DELE command.
	* 
	* @param aFileName	File name */
	virtual void Dele(const TDesC8& aFileName)=0;

	/** Issues the RMD command.
	* 
	* @param aParam	Directory name */
	virtual void Rmd(const TDesC8& aParam)=0;

	/** Issues the MKD command.
	* 
	* @param aParam	Directory name */
	virtual void Mkd(const TDesC8& aParam)=0;

	/** Issues the PWD command. */
	virtual void Pwd(void)=0;

	/** Issues the LIST command, giving a null argument. */
	virtual void List(void)=0;

	/** Issues the LIST command, specifying a file/directory name.
	* 
	* @param aParam	File/directory name */
	virtual void List(const TDesC8& aParam)=0;

	/** Issues the NLST command, giving a null argument. */
	virtual void Nlst(void)=0;

	/** Issues the NLST command, specifying a directory name.
	* 
	* @param aParam	Directory name */
	virtual void Nlst(const TDesC8& aParam)=0;

	/** Issues the SITE command.
	* 
	* @param aParam	SITE command argument */
	virtual void Site(const TDesC8& aParam)=0;

	/** Issues the SYST command. */
	virtual void Syst(void)=0;

	/** Issues the STAT command, specifying an argument.
	* 
	* @param aParam	STAT command argument */
	virtual void Stat(const TDesC8& aParam)=0;

	/** Issues the STAT command (no argument). */
	virtual void Stat(void)=0;

	/** Issues the HELP command.
	* 
	* @param aParam	HELP command argument */
	virtual void Help(const TDesC8& aParam)=0;

	/** Issues the HELP command (no argument). */
	virtual void Help(void)=0;

	/** Issues the NOOP command. */
	virtual void Noop(void)=0;

// Buffer management for transfer
	// Following functions pass a pointer to a buffer
	// to transfer data to/from the Dtp channel
	// Before an operation which cause data to transit on the 
	// DTP channel to occur (Nlst, List, Retr, Stor)
	// a Buffer must be provided with the following api
	// Also when the client is notified of a MoreData event
	// It must reissue the following operation to get the rest of
	// the data
	/** Specifies a buffer to transfer data to the DTP channel.	
	* 
	* @param aBuffer	Send receive */
	virtual void SendBuffer(TDes8* aBuffer)=0;

	/** Specifies a buffer to receive data from the DTP channel.
	* 
	* @param aBuffer	Receive buffer */
	virtual void RecvBuffer(TDes8* aBuffer)=0;

	/** Finishes the transfer initiated by a STOR command. */
	virtual void SendEOF(void)=0; //Finishes the transfer initiated by a stor command

	/** Cancels current operation. */
	virtual void UserCancel(void)=0;

// Copies the 3 digits answer received from the FTP server
	/** Gets the 3 digits answer received from the FTP server.
	* 
	* @param aServerAnswer	3 digit answer */
	virtual void FTPServerAnswer(TDes& aServerAnswer)=0;

	/**
	Returns 32-bit, with MAJOR_VERSION in the highest byte
	MINOR_VERSION in the next byte
	i.e. MAJOR 2, MINOR 0x34, BUILD 0x278 would be "ver 2.52
	*/
	IMPORT_C static TUint32 GetVersion(void);

	/** Allocates and constructs a new FTP engine object.
	* 
	* @return	New FTP engine object
	*/
	// @param aNotifier Client callback interface. 
	// The FTP engine calls this interface to pass 
	//server responses and status messages to the client.
	IMPORT_C static CFtpProtocol *NewL(MFtpProtocolNotifier*);

	/**Destructor.*/
	virtual ~CFtpProtocol();
	};
#endif //__FTPPROT_H__
