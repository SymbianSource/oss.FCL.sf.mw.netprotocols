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
* EPOC32 FTP Engine header file
* Author:	Philippe Gabriel
* Exports set of APIs simplyfying access to the FTP protocol
* 
*
*/



/**
 @file FTPSESS.H
 @internalComponent
*/

#if !defined(__FTPSESS_H__)
#define __FTPSESS_H__
#include <e32base.h>
#include <es_sock.h>
#include <f32file.h>

/**
The very first release
*/

/** FTPSESS.DLL major version number. */
#define FTPSESS_VERSION_MAJOR 0x01 // The very first release
/** FTPSESS.DLL minor version number. */
#define FTPSESS_VERSION_MINOR 0x03
/** FTPSESS.DLL version number. */
#define FTPSESS_VERSION_NUMBER (FTPSESS_VERSION_MAJOR<<8)|FTPSESS_VERSION_MINOR

// Following Def as per RFC 959
/** Default server port.
@internalComponent */
const TUint KDefaultServerPiPort = 21;
//
// MInterface definition to provide callback functions to the client
//

class MFtpSessionNotifier
/** FTP session callback interface.
*
* An FTP session client implements this interface to receive status and results 
* from asynchronous FTP operations.
* 
* Note that, as defined in RFC959, FTP does not allow concurrent execution of 
* several requests. Hence, even though calling an FTP function and getting the 
* result through this interface are asynchronous operations, these events happen 
* in a sequential manner. Each notification that the client receives corresponds 
* to only one currently outstanding operation.
* @internalComponent
*/
	{
public:
//
// Operation completion return codes.
// 
/** FTP engine/session operation completeness codes. */
	enum TOpComp 
		{
	/** Operation completed normally. */
		EOpComplete=0,	// No error
	/** Operation cancelled. */
		EOpCanceled,	// User canceled last operation

		//Connection errors
	/** Connection error: Connect address invalid. */
		EHostNotExist,	// Connect address invalid
	/** Connection error: Sockets level error. */
		ESocketError,	// Problem with socket operation
	/** Connection error: Connection failed. */
		EConnectionFailed,	// Can't connect to FTP port
	/** Connection error: Password needed. */
		EPasswordNeeded,
	/** Connection error: Anonymous login not permitted. */
		EAccountNeeded,	// i.e. anonymous login disallowed
	/** Connection error: UserName, Password combination invalid. */
		ELoginFailed,	// UserName,Password combination invalid
	/** Connection error: Not connected to a server. */
		ENotConnected,	// Not connected to a server
	/** Connection error: Already connected to a server. */
		EAlreadyConnected,	// Already connected to a server
	/** Connection error: Operation timed out. */
		ETimedOut,	// Inactive for too long

		//Local filesystem errors
	/** Local filesystem error: General file system error. */
		EFileSystemError, 
	/** Local filesystem error: File opening failure. */
		EFileOpenFailure, 
	/** Local filesystem error: File reading failure. */
		EFileReadError, 
	/** Local filesystem error: File writing failure. */
		EFileWriteError,
	/** Local filesystem error: File already exists. */
		EFileAlreadyExist,
	/** Local filesystem error: File does not exist. */
		EFileNotExist,
	/** Local filesystem error: Directory already exists. */
		EDirAlreadyExist,
	/** Local filesystem error: Directory does not exist. */
		EDirNotExist,

		// Permission error
	/** Permission error: Permission denied. */
		EPermissionDenied,

		//Remote filesystem errors
	/** Remote filesystem error: General remote file system error. */
		ERemoteFileSystemError, 
	/** Remote filesystem error: Remote file opening failure. */
		ERemoteFileOpenFailure, 
	/** Remote filesystem error: Remote file reading failure. */
		ERemoteFileReadError, 
	/** Remote filesystem error: Remote file writing failure. */
		ERemoteFileWriteError,
	/** Remote filesystem error: Remote file already exists. */
		ERemoteFileAlreadyExist,
	/** Remote filesystem error: Remote file does not exist. */
		ERemoteFileNotExist,
	/** Remote filesystem error: Remote directory already exists. */
		ERemoteDirAlreadyExist,
	/** Remote filesystem error: Remote directory does not exist. */
		ERemoteDirNotExist,
	/** Remote filesystem error: Restart is not supported. */
		ERestartNotSupported
		};

	/** Normal operation completion. */
	virtual void Complete(void)=0;

	// Operation completed, more data to follow
	/** Operation partially completed, with more data to follow. */
	virtual void MoreData(void)=0;

	/** Reports the amount of data already transferred in bytes.
	* 
	* @param aProgress	Amount of data already transferred */
	virtual void TransferProgress(TUint aProgress)=0;

	/** User cancelled an on-going operation. */
	virtual void Cancel(void)=0;

	/** Peer reset the connection. */
	virtual void ConnReset(void)=0;

	/** Error in establishing the connection with the FTP server.
	* 
	* @param aTConnectionError	Operation completion code */
	virtual void ConnectionError(TOpComp aTConnectionError)=0;

	// FTP server does not implement the operation requested
	/** Restart operation not supported. */
	virtual void OperationNotSupported(void)=0;

	// Local File system error
	/** Error with the local file system.
	* 
	* @param aTLocalFileSystemError	Operation completion code */
	virtual void LocalFileSystemError(TOpComp aTLocalFileSystemError)=0;

	// Remote File system error
	/** Error with the remote file system.
	* 
	* @param aTRemoteFileSystemError	Operation completion code */
	virtual void RemoteFileSystemError(TOpComp aTRemoteFileSystemError)=0;

	// Not specified yet
	/** Unspecified error. */
	virtual void EUnknownError()=0;

	// Message reported by server
/** Message sent by the FTP server.
* 
* As specified by the RFC, the server answers all requests from the 
* client with a plain text message beginning with a 3 digit code.
* The error/completion notifications sent back by the FTP session API 
* are derived from these codes. Additionally, this function can be 
* used to get the full string reporting the result of the request. 
* It is recommended that the user interface displays this string to 
* the user, as this gives a more precise idea of the result of the 
* requested operation, especially in case of error.
* 
* @param TDesC8	The message sent by the server */
	virtual void ServerMessage(const TDesC8&)=0;
    	};		
//
// The CFTPSession class
//
class CFTPSession : public CBase

/** Abstracts the complexity of the full FTP protocol and exports only 
* a few simplified APIs.
* @internalComponent */
	{
public:
/** FTP connection mode (passive or active see RFC959). */
	enum TConnectionMode 
		{
	/** Active mode. Server initiates DTP connection to client. */
		EActive=0, //(see RFC959)
	/** Passive mode. Client initiates DTP connection to server.*/
		Epassive   //(see RFC959)
		};
/** Representation type of a transferred file. */
	enum RepresentationType
		{
	/** Uninitialised. */
		EUninitialised=0,						   
	/** File transfered in Binary mode, no translation. */
		EBinary,
	/** File transfered in ASCII mode, translation. */
		EASCII
		};
/** FTP file transfer mode. */
	enum TransferMode
		{
	/** Stream mode; file transfered as a stream of bytes. */
		EStream=0,
	/** Block mode; file transfered as blocks, with header needed to restart aborted transfer. */
		Eblock 
		};
/** FTP file open mode. */
	enum TOpenMode
		{
	/** Overwrite existing file. */
		EOverwrite,
	/** Do not overwrite existing file. */
		ENoOverwrite,
	/** Expand existing file. */
		EExpand
		};

/** Construction */
public:

//
// Connection APIs
//
// Establish a connection with a server:
	/** Connects to a remote FTP server, specifying the FTP server by a numeric IP 
	* address.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::ConnectionError(), 
	* or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aNetAddr			FTP server's IP address
	* @param aUserName			User name to log on the FTP server
	* @param aPassword			Password to identify to the FTP server
	* @param aConnectionMode	Connection mode (passive or active, see RFC959). 
	* 							You must use passive mode if the client is behind a firewall. */
	virtual void Connect(	const TSockAddr& aNetAddr, //IP address
							const TDesC8& aUserName, 
							const TDesC8& aPassword,
							const TConnectionMode aConnectionMode=EActive)=0;


	/** Connects to a remote FTP server, specifying the FTP server by a DNS name.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::ConnectionError(), 
	* or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aServerName		FTP server's DNS name
	* @param aUserName			User name to log on the FTP server
	* @param aPassword			Password to identify to the FTP server
	* @param aConnectionMode	Connection mode (passive or active, see RFC959). You 
	* 							must use passive mode if the client is behind a firewall.
	* @param aPort				Port to connect to initiate the PI connection (see RFC959) */
	virtual void Connect(	const THostName& aServerName, //DNS name
							const TDesC8& aUserName, 
							const TDesC8& aPassword,
							const TConnectionMode aConnectionMode=EActive, 
							const TUint aPort=KDefaultServerPiPort)=0;


// Close connection with a server
	/** Closes the current connection with the FTP server.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), or MFtpSessionNotifier::EUnknownError().
	* 
	* This cannot be called when an operation is in progress. */
	virtual void Close()=0;


// Cancel last FTP operation
	/** Cancels the last FTP operation.
	* 
	* Cancel is only implemented for lengthy operations, that is: Connect(), Store(), 
	* Retrieve(), and ListDirectory(). For these operations, once cancel has been 
	* called, the MFtpSessionNotifier::Cancel() callback is called.
	* 
	* For other operations, calling Cancel() has no effect (it would take longer to 
	* wait for an acknowledgement to the Cancel(), than waiting for the result of 
	* the current operation). However, a completion callback will be called, as 
	* well as MFtpSessionNotifier::Cancel(). */
	virtual void Cancel()=0;


// Restart an aborted transfer operation
	/** After a connection is re-established, restarts the last aborted transfer operation 
	* (i.e. Store/Retrieve).
	* 
	* It is the responsibility of the client to remember and reset the state of 
	* the connection before attempting to resume the transfer: i.e. the client should 
	* re-establish the connection to the server and return to the relevant directory, 
	* then it should issue the Restart() command with the offset it has saved, and 
	* then issue the Store() or Retrieve() command.
	* 
	* The Restart() command should be avoided if the transfer was done in ASCII mode, 
	* as, because the server peforms a conversion on the bytestream format that 
	* it gets from the file before sending, the file size on the receiving end will 
	* be different than the size on the sending end. This means it is not possible 
	* to compute an offset for the sending end. 
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::OperationNotSupported(), 
	* or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aTFTPRestartOffset	An offset in bytes in the file from where transfer 
	* 							is to be resumed */
	virtual void Restart(const TUint aTFTPRestartOffset)=0;

//
// Transfer APIs
//
// Store a file on the server	
	/** Transfers a file to the FTP server.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::ConnectionError(), 
	* MFtpSessionNotifier::LocalFileSystemError(), MFtpSessionNotifier::RemoteFileSystemError() 
	* or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aLocalFileName			Name of the local file to be transferred
	* @param aNewRemoteFileName		Name of the remote file to be created
	* @param aOverwrite				If ETrue, overwrite a remote file with the same name if it 
	* 								exists; if EFalse, fail if a remote file with the same name exists
	* @param aRepresentationType	The representation type of the transferred file, ASCII or Binary
	* @param aTransferMode			The transfer mode, stream mode or block mode. This is 
	* 								ignored and assumed to be stream, as block mode seems to be obsolete. */
	virtual void Store(	const TDesC& aLocalFileName,
						const TDesC8& aNewRemoteFileName,
						const TBool	aOverwrite = EFalse,
						const RepresentationType aRepresentationType = EBinary,
						const TransferMode aTransferMode = EStream)=0;
	

// Get a file from the server
	/** Transfers a file from the FTP server.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::LocalFileSystemError(), 
	* MFtpSessionNotifier::RemoteFileSystemError() or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aRemoteFileName		The remote file Name
	* @param aNewLocalFileName		Name of the local file to be created
	* @param aOpenMode				Specifies whether to overwrite a local file with the same 
	* 								name if it already exists
	* @param aRepresentationType	The representation type of the transferred file, 
	* 								ASCII or Binary
	* @param aTransferMode			The transfer mode, stream mode or block mode. This is 
	* 								ignored and assumed to be stream, as block mode seems to be obsolete. */
	virtual void Retrieve(	const TDesC8& aRemoteFileName,
							const TDesC& aNewLocalFileName,
							const TOpenMode	aOpenMode = EOverwrite,
							const RepresentationType aRepresentationType = EBinary,
							const TransferMode aTransferMode = EStream)=0;
	

//
// File system management functions
//
	/** Sets the current directory on the remote file system.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::RemoteFileSystemError() 
	* or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aDirectoryName	Directory name */
	virtual void ChangeDirectory(const TDesC8& aDirectoryName)=0;


	/** Creates a directory on the remote file system.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::RemoteFileSystemError() 
	* or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aDirectoryName	A directory name. This can be absolute or relative. */
	virtual void CreateDirectory(const TDesC8& aDirectoryName)=0;


	/** Deletes a directory on the remote file system.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::RemoteFileSystemError() 
	* or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aDirectoryName	A directory name. This can be absolute or relative. */
	virtual void DeleteDirectory(const TDesC8& aDirectoryName)=0;


	/** Gets the client's current directory on the remote file system.
	* 
	* The result is returned to the MFtpSessionNotifier::ServerMessage() callback. 
	* The directory name is defined by the RFC as being enclosed between double 
	* quotes: for example, an answer will look like:
	* 
	* @code
	* 257 "/developr/rfc" is current directory.
	* @endcode
	* The client must implement a parser to find the text between quotes.
	* 
	* The result can be passed in two or more consecutive calls of MFtpSessionNotifier::ServerMessage(). 
	* For example:
	* 
	* First call of MFtpSessionNotifier::ServerMessage(): @code 257 "/developr @endcode
	* 
	* Second call of MFtpSessionNotifier::ServerMessage(): @code /rfc" is current directory. @endcode
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::RemoteFileSystemError() 
	* or MFtpSessionNotifier::EUnknownError(). */
	virtual void GetCurrentDirectory(void)=0;


	/** Lists the files in a directory on the remote file system.
	* 
	* On successful completion, the aFileList buffer contains the list of files 
	* as transmitted by the server. It is the responsibility of the client to parse 
	* this buffer to extract relevant information. aFileList is always appended 
	* to, so the client should set its current length to a meaningful value (i.e. 
	* 0, to fill the buffer from scratch).
	* 
	* If the list of files is larger than the aFileList buffer, MFtpSessionNotifier::MoreData() 
	* is called. At this point, the client must reissue the ListDirectory() request 
	* until the MFtpSessionNotifier::Complete() is called.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::RemoteFileSystemError() 
	* or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aDirectoryName	A directory name. This can be absolute or relative.
	* @param aFileList		On completion, the file list. The buffer is allocated by the client. */
	virtual void ListDirectory(const	TDesC8& aDirectoryName,
										TDes8& aFileList)=0;
	

	/** Deletes a file on the remote file system.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::RemoteFileSystemError() 
	* or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aFileName	A file name */
	virtual void DeleteFile(const TDesC8& aFileName)=0;
	
	/** Renames a file on the remote file system.
	* 
	* Completion is indicated by a callback to one of MFtpSessionNotifier::Complete(), 
	* MFtpSessionNotifier::ConnReset(), MFtpSessionNotifier::RemoteFileSystemError() 
	* or MFtpSessionNotifier::EUnknownError().
	* 
	* @param aRemoteFileName	An existing file name
	* @param aNewRemoteFileName	A new file name */
	virtual void RenameFile(const TDesC8& aRemoteFileName,
							const TDesC8& aNewRemoteFileName)=0;


	/**
	Returns 32-bit, with 
	ftpsess dll MAJOR_VERSION in msb of the msw
	ftpsess dll MINOR_VERSION in lsb of the msw
	ftpprot dll MAJOR_VERSION in msb of the lsw
	ftpprot dll MINOR_VERSION in lsb of the lsw
	*/
	IMPORT_C static TUint32 GetVersion(void);

	/** Allocates and constructs a new FTP session object. 
	* 
	* @param aNotifier	Callback interface to notify the client of the completion of 
	* 					operations or to report errors. For each FTP session, the FTP 
	* 					client should instantiate an object of this type.
	* @return			New FTP session object
	*/
	IMPORT_C static CFTPSession* NewL(MFtpSessionNotifier* aNotifier);

	/**Destructor.*/
	virtual ~CFTPSession();
};
#endif
