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
* Author: Philippe Gabriel
* 
*
*/



/**
 @file FTPUI.H
 @internalComponent
*/

#include <e32cons.h>
#include <e32base.h>
#include <f32file.h>
#include "FTPSESS.H"
#include "CONSOLE.H"

class CFtpUI : public CBase, public MCmdConsoleReport,public MFtpSessionNotifier
/**
a simple FTPui 
@internalComponent
*/
{
public:
    static CFtpUI* NewL();
    static CFtpUI* NewLC();    
    
    ~CFtpUI(void);
    
private:
    CFtpUI(void);
    void ConstructL();
    
public:
	/** Console callback Minterface */
	void CmdReady(void);
	void Escape(void);
	/** FTP Session callback Minterface notifier */
	void Complete(void);
	/** Operation completed, more data to follow */
	void MoreData(void);
	/** Transfer Progress report */
	void TransferProgress(TUint aProgress);
	/** User canceled ongoing operation */
	void Cancel(void);
	/** Peer reset the connection */
	void ConnReset(void);
	/** Connection error */
	void ConnectionError(TOpComp aTConnectionError);
	/** FTP server does not implement the operation requested */
	void OperationNotSupported(void);
	/** Local File system error */
	void LocalFileSystemError(TOpComp aTLocalFileSystemError);
	/** Remote File system error */
	void RemoteFileSystemError(TOpComp aTRemoteFileSystemError);
	/** Not specified yet */
	void EUnknownError();
	/** Message reported by server */
	void ServerMessage(const TDesC8&);
	/** FTP User interface specific methods */
	
	/** Execute a previously parsed command */
	TBool Execute(void);	
	void SetConsole(CConsoleBase*);
	void SetCmdConsole(CmdConsole*);
	void SetFTPSession(CFTPSession*);
	void Help(void);

	enum TParseResult{
		EError,EContinue,ESuccess
		};
	/** Parse a command */
	TParseResult Parse(TDesC&); 
private:
	enum ftpCmds{
		/** Syntax error
		*/
		EInvalid,
		/** Connection cmds
		*/
		EConnectDNS,EConnectIP,EClose,EActive,Epasv,
		/** Xfer cmds
		*/
		EPut,EGet,EBin,EAsc,ERest,EOver,EExpand,
		/** File cmds
		*/
		EDel,ERen,
		/** Dir cmds
		*/
		EMkdir,ERmdir,ECd,EPwd,EList,ELcd,
		/** Help
		*/
		EHelp,
		/** Exit
		*/
		EQuit,
		/** Version
		*/
		EVer,
		/** Cancel
		*/
		ECancel
		};
	enum TState{
		EInputCmd,EInputLogin,EInputPass
		};
	/** A Buffer to fill in with command */
	TBuf<256> iCmdBuffer; 
	/** A Buffer to fill in with param */
	TBuf<256> iUParam; 
	/** A dir path in the local file system */
	TBuf<256> iDirPath; 
	/** A Buffer to fill in with param */
	TBuf8<256> iParamBuffer1; 
	/** A Buffer to fill in with param */
	TBuf8<256> iParamBuffer2; 
	/** A Buffer to fill in with param */
	TBuf8<256> iParamBuffer3;
	/** A Buffer to fill with data comming from the DTP Channel */
	TBuf8<2000> iFileBuffer; 
	/** A Buffer to fill with data comming from the DTP Channel */
	TBuf<2000> iUBuffer; 
	/** The amount transfered so far in bytes */
	TUint	iTransferProgress; 
    /** The last moment when the amount transferred was displayed */
    TTime iLastTime;
    TInt iProgressIdx;
    TBool iProgress;
	ftpCmds	iCurCmd;
	TState	iState;
	CConsoleBase* iConsole;
	CmdConsole* iCmdConsole;
	/** A pointer to an FtpSess object */
	CFTPSession* iFTPSession;
	/** Default params */
	CFTPSession::TConnectionMode iConnMode;
	CFTPSession::TOpenMode	iOpenMode;
	CFTPSession::RepresentationType iType;
	RFs	iFs;
	virtual TText getProgressChar();
};
