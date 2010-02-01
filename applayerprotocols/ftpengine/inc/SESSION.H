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
* EPOC32 FTP Engine header file
* Author:	Philippe Gabriel
* Exports set of APIs simplyfying access to the FTP protocol
* 
*
*/



/**
 @file SESSION.H
 @internalComponent
*/

#if !defined(__SESSION_H__)
#define __SESSION_H__
#include <e32base.h>
#include <es_sock.h>
#include <f32file.h>
#include <ftpprot.h>
#include <ftpsess.h>
#include "SETERROR.H"

//class CFtpProtocol;
//class CFtpSessSetError;

/**
MInterface definition to provide callback functions to the client
*/

class MSetErrorNotifier;
NONSHARABLE_CLASS(CFTPSessionDerived) : public CFTPSession,public MFtpProtocolNotifier,MSetErrorNotifier
/**
The CFTPSession class
@internalComponent
*/
	{
public:
	/** Define the CFTPSetError callback notifier */
	void SetErrorNotifier(const TInt);
	/** Define the CFtpProtocol callback notifiers */
	void ServerMessage(const TDesC8& aMessage);
	void ServerPositiveAnswerNotification(const TOpComp aStatus);
	void ServerXFerNotification(const TOpComp aStatus);
	void ServerNegativeAnswerNotification(const TOpComp aStatus);
	void ErrorNotification(const TOpComp aStatus);
	/** Construction */
	static CFTPSessionDerived* NewL(MFtpSessionNotifier* aNotifier);

	/** Destruction */
	~CFTPSessionDerived();

	/** Connection APIs */

	/** Establish a connection with a server: */
	virtual void Connect(	const TSockAddr& aNetAddr, //IP address
							const TDesC8& aUserName, 
							const TDesC8& aPassword,
							const TConnectionMode aConnectionMode=EActive);

	virtual void Connect(	const THostName& aServerName, //DNS name
							const TDesC8& aUserName, 
							const TDesC8& aPassword,
							const TConnectionMode aConnectionMode=EActive, 
							const TUint aPort=KDefaultServerPiPort);

	/** Close connection with a server */
	virtual void Close();

	/** Cancel last FTP operation */
	virtual void Cancel();

	/** Restart an aborted transfer operation */
	virtual void Restart(const TUint aTFTPRestartOffset);

	/** Transfer APIs */

	/** Store a file on the server	*/
	virtual void Store(const TDesC& aLocalFileName,
						const TDesC8& aNewRemoteFileName,
						const TBool	aOverwrite = FALSE,
						const RepresentationType aRepresentationType = EBinary,
						const TransferMode aTransferMode = EStream);
	
	/** Get a file from the server */
	virtual void Retrieve(	const TDesC8& aRemoteFileName,
							const TDesC& aNewLocalFileName,
							const TOpenMode	aOpenMode = EOverwrite,
							const RepresentationType aRepresentationType = EBinary,
							const TransferMode aTransferMode = EStream);
	
	/** File system management functions */
	virtual void ChangeDirectory(const TDesC8& aDirectoryName);
	virtual void CreateDirectory(const TDesC8& aDirectoryName);
	virtual void DeleteDirectory(const TDesC8& aDirectoryName);
	virtual void GetCurrentDirectory(void);
	virtual void ListDirectory(const	TDesC8& aDirectoryName,
										TDes8& aFileList);
	
	virtual void DeleteFile(const TDesC8& aFileName);
	virtual void RenameFile(	const TDesC8& aRemoteFileName,
								const TDesC8& aNewRemoteFileName);
private:
	/** C++ constructor - not exported; implicitly called from NewL() */
	CFTPSessionDerived(void);
	/** 2nd phase construction, called by NewL() */
	void ConstructL(MFtpSessionNotifier* aNotifier); 
	void NormalTransition(void);
	void ErrorTransition(const MFtpProtocolNotifier::TOpComp);
enum TState 
	{
	EIdle,
	EConnecting,EConnected,ELoginUser,ELoginPass,EClosing,
	EGetCurrentDirectory,EChangeDirectory,ECreateDirectory,
	EDeleteDirectory,EDeleteFile,ERenameFileFrom,
	ERenameFileTo,
	EInitiateListOperation,EInitiateRetrieveOperation,EInitiateStoreOperation,
	EPerformingPortForList,EPerformingPasvForList,EPerformingList,
	EPerformingTypeForRetrieve,EPerformingTypeForStore,
	EPerformingRestForRetrieve,EPerformingRestForStore,
	EPerformingPortForRetrieve,EPerformingPasvForRetrieve,ERetrieveFile,
	EPerformingPortForStore,EPerformingPasvForStore,EStoreFile,EStoreFileSendEOF,
	EAbortingStore,EAbortingRetrieve,EAbortingList
	};
TState	UpdateState(TState,MFtpProtocolNotifier::TOpComp);
TInt debugInt;
TState	iState;
/** A pointer to the set of callback notifiers */
MFtpSessionNotifier*	iFtpSessionNotifier;
/** A pointer to the CFTPProtocol object */
CFtpProtocol*	iCFtpProtocol;
/** A pointer to the CFTPSetError object */
CFTPSetError*	iCFTPSetError;
TBool	FileServerInitialised;
RFs	iFs;
RFile	iFile;
TBool	iFileOpErrorOccured;
/** The current representation type */
RepresentationType	iCurrentRepresentationType;
/** A placeholder for the parameter passed to retrieve and store functions*/
RepresentationType	iNewRepresentationType;
/** An offset for the FTP REST command to perform a restart */
TUint	iRESTOffset;
/** FTP Connection can be passive (server connects on one of my ports) */
TBool	iPASVMode;
/**
A place holder buffer for my API parameters
client can pass me pointer to params stored on its stack
need to copy them to a safe place so I can use them latter
when the scheduler calls my notifier
*/
TBuf8<512> iBuffer1; 
TBuf8<512> iBuffer2; 
TBuf8<5000> iFileXferBuffer[2]; 
TInt iCurrentXferBuffer;
TDes8*	iFileList;
TInt	iTInt1,iTInt2;
TBool	iTBool1;
/** Placeholder for the answer I got from the FTP server */
TBuf<3> iServerAnswer;	
TBuf<256> iRenamedLocalFileName;
TBool iTempStore;
TBool iFirstRun; //first run of ServerXFerNotification
};
#endif
