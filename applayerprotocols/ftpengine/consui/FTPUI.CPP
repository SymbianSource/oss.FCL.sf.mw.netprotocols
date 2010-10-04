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
// Author: Philippe Gabriel
// 
//

/**
 @file FTPUI.CPP
 @internalComponent
*/

#include "FTPUI.H"
#include "FTPSESS.H"
#include <e32base.h>

void CFtpUI::ServerMessage(const TDesC8& aMessage)
/**
Implementation of the FTPSession Call back Minterface
*/
{
// Display Message from server
iUBuffer.Copy(aMessage);
iConsole->Write(iUBuffer);
}

void CFtpUI::Complete(void)
{
switch (iCurCmd)
	{
	// Connection commands
	case EConnectDNS:
	case EConnectIP:
		iConsole->Write(_L("Connection Completed OK\n"));
		break;
	case EClose:
		iConsole->Write(_L("Connection Closed OK\n"));
		break;
	case EActive:
		break;
	case Epasv:
		break;
	// Xfer cmds
	case EPut:
	case EGet:
	case EBin:
	case EAsc:
	case ERest:
	case EOver:
	// File cmds
	case EDel:
	case ERen:
	// Dir cmds
	case EMkdir:
	case ERmdir:
	case ECd:
	case EPwd:
		break;
	case EList:
		iUBuffer.Copy(iFileBuffer);
		iConsole->Write(iUBuffer);
		break;
	// Exit
	case EQuit:
	case ECancel:
	default:
		break;
	}
// Reset the console to fetch next command
iCmdConsole->Reset();
}

TText CFtpUI::getProgressChar() 
{
 if(iProgress) 
 {
 iProgressIdx=(iProgressIdx+1) & 0x3;
 }
 switch(iProgressIdx) 
 {
 case 0: return TText('|');
 case 1: return TText('/');
 case 2: return TText('-');
 case 3: return TText('\\');
 }
 return TText('O');
 }

void CFtpUI::TransferProgress(TUint aProgress)
{
iTransferProgress += aProgress;
if(aProgress) 
 iProgress = ETrue;
TTime now; 
now.HomeTime();
TTimeIntervalSeconds elapsedSec;
TInt res=now.SecondsFrom(iLastTime,elapsedSec);
if(res!=KErrNone || elapsedSec.Int()>=10) 
 {
 iConsole->SetPos(0,iConsole->WhereY()-1);
 iConsole->Printf(_L("\n%c TransferProgress : %u kB        "),getProgressChar(),(TUint)( iTransferProgress>>10));
 iLastTime.HomeTime();
 iProgress=EFalse;
 }
}

void CFtpUI::Cancel(void)
{
iConsole->Printf(_L("\n---Cancel Completed---\n"));
// Reset the console to fetch next command
iCmdConsole->Reset();
}

void CFtpUI::ConnReset(void)
{
iConsole->Printf(_L("\n---Conn RESET---\n"));
// Reset the console to fetch next command
iCmdConsole->Reset();
}

void CFtpUI::ConnectionError(TOpComp aTConnectionError)
{
iConsole->Printf(_L("\nConnection ERROR : "));
switch(aTConnectionError)
	{
	case EHostNotExist:	// Connect address invalid
		iConsole->Printf(_L("Host does not exist\n"));
		break;
	case ESocketError:	// Problem with socket operation
		iConsole->Printf(_L("Socket error\n"));
		break;
	case EConnectionFailed:	// Can't connect to FTP port
		iConsole->Printf(_L("Connection failed\n"));
		break;
	case EPasswordNeeded:
		iConsole->Printf(_L("Password needed\n"));
		break;
	case EAccountNeeded:	// i.e. anonymous login disallowed
		iConsole->Printf(_L("Account needed\n"));
		break;
	case ELoginFailed:	// UserName,Password combination invalid
		iConsole->Printf(_L("Login failed\n"));
		break;
	case ENotConnected:	// Not connected to a server
		iConsole->Printf(_L("Not connected anywhere\n"));
		break;
	case EAlreadyConnected:	// Already connected to a server
		iConsole->Printf(_L("Already connected\n"));
		break;
	case ETimedOut:	// Inactive for too long
		iConsole->Printf(_L("Timed Out\n"));
		break;
	default:
		iConsole->Printf(_L("Internal Error: 0x56af087bb:0234\nThis is very very bad news\n"));
		break;
	}
// Reset the console to fetch next command
iCmdConsole->Reset();
}

void CFtpUI::OperationNotSupported(void)
{}

void CFtpUI::LocalFileSystemError(TOpComp /*aTLocalFileSystemError*/)
{
iConsole->Printf(_L("\nLocalFileSystemError\n"));
// Reset the console to fetch next command
iCmdConsole->Reset();
}

void CFtpUI::RemoteFileSystemError(TOpComp /*aTRemoteFileSystemError*/)
{
//CActiveScheduler::Stop();
iConsole->Printf(_L("\nRemoteFileSystemError\n"));
// Reset the console to fetch next command
iCmdConsole->Reset();
}

void CFtpUI::EUnknownError(void)
{
iConsole->Printf(_L("\nUnknownError\n"));
// Reset the console to fetch next command
iCmdConsole->Reset();
}

void CFtpUI::MoreData(void)	
{
	iUBuffer.Copy(iFileBuffer);
	iConsole->Write(iUBuffer);
	
	iFileBuffer.FillZ(iFileBuffer.MaxLength());
	iFileBuffer.Zero();
    iFTPSession->ListDirectory(_L8(""),iFileBuffer);
	return;
}

void CFtpUI::Escape(void)
/**
Implement a console callback Minterface notifier
*/
{
	iCurCmd = ECancel;
	iConsole->Write(_L("\nCaNceling lAsT coMmaNd\n"));
	iFTPSession->Cancel();
}
void CFtpUI::CmdReady(void)
{

/*__FTPDebugConsole->Write(_L("\nNotif recved\n"));
__FTPDebugConsole->Write(_L("Buffer :"));
__FTPDebugConsole->Write(myCmdConsole->FetchCmd());
__FTPDebugConsole->Write(_L("\n"));*/
switch(Parse(iCmdConsole->FetchCmd()))
	{
	case CFtpUI::EError:
		// Just reset the thing
		iCmdConsole->Reset();
		break;
	case CFtpUI::EContinue:
		// Loop again to fetch more params
		iCmdConsole->Reset();
		break;
	case CFtpUI::ESuccess:
		// Execute the command
		Execute();	
		break;
	}
}


CFtpUI* CFtpUI::NewL()
    {
    CFtpUI* self = CFtpUI::NewLC();
    CleanupStack::Pop();
    return self;
    }

CFtpUI* CFtpUI::NewLC()
    {
    CFtpUI* self = new(ELeave) CFtpUI();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

void CFtpUI::ConstructL()
    {
    User::LeaveIfError(iFs.Connect(KFileServerDefaultMessageSlots));
    }

CFtpUI::CFtpUI(void)
{
iCurCmd = EInvalid ;
iConnMode = CFTPSession::EActive;
iOpenMode = CFTPSession::EOverwrite;
iType = CFTPSession::EBinary;
iState = EInputCmd;
}

CFtpUI::~CFtpUI(void)
{
iFs.Close();
}

void CFtpUI::SetConsole(CConsoleBase* aConsole)
	{iConsole = aConsole;}

void CFtpUI::SetCmdConsole(CmdConsole* aCmdConsole)
	{iCmdConsole = aCmdConsole;}

void CFtpUI::SetFTPSession(CFTPSession* aFTPSession)
	{iFTPSession = aFTPSession;}

CFtpUI::TParseResult CFtpUI::Parse(TDesC& iCommand)
/**
Parse a command - Fill in iCmdBuffer - iParamBuffer1 - iParamBuffer2
*/
	{
	TLex input(iCommand);
	// Extract Tokens
	switch(iState)
		{
		case EInputLogin:
			iParamBuffer2.FillZ(iParamBuffer2.MaxLength());
			iParamBuffer2.Copy(input.NextToken());
			iState = EInputPass;
			iConsole->Write(_L("Password: "));
			return EContinue;
		case EInputPass:
			iParamBuffer3.FillZ(iParamBuffer2.MaxLength());
			iParamBuffer3.Copy(input.NextToken());
			iState = EInputCmd;
			return ESuccess;
		default:
			break;
		}
	// Default case, Fetch command + params
	// Reset buffers
	iCmdBuffer.FillZ(iCmdBuffer.MaxLength());
	iParamBuffer1.FillZ(iParamBuffer1.MaxLength());
	iParamBuffer2.FillZ(iParamBuffer2.MaxLength());
	iCmdBuffer.Copy(input.NextToken());
	iCmdBuffer.LowerCase();
	iParamBuffer1.Copy(input.NextToken());
	iParamBuffer2.Copy(input.NextToken());
	// Drop a trace
/*	FTPPROTDEBUG(0xffff,_L("\nCmd:"));
	FTPPROTDEBUG(0xffff,iCmdBuffer);
	FTPPROTDEBUG(0xffff,_L("\n"));
	FTPPROTDEBUG(0xffff,_L("Param1:"));
	FTPPROTDEBUG(0xffff,iParamBuffer1);
	FTPPROTDEBUG(0xffff,_L("\n"));
	FTPPROTDEBUG(0xffff,_L("Param2:"));
	FTPPROTDEBUG(0xffff,iParamBuffer2);
	FTPPROTDEBUG(0xffff,_L("\n"));*/
	// Fetch a command
	// Very very crude parser - programmers in a hurry Limited
	
		if (KErrNotFound != iCmdBuffer.Match(_L("connect")))
			{
			iCurCmd = EConnectDNS;
			iState = EInputLogin;
			iConsole->Write(_L("Login: "));
			return EContinue;
			}
		else if (KErrNotFound != iCmdBuffer.Match(_L("close")))
			iCurCmd = EClose;
		else if (KErrNotFound != iCmdBuffer.Match(_L("active")))
			iCurCmd = EActive;
		else if (KErrNotFound != iCmdBuffer.Match(_L("pasv")))
			iCurCmd = Epasv;
		else if (KErrNotFound != iCmdBuffer.Match(_L("put")))
			iCurCmd = EPut;
		else if (KErrNotFound != iCmdBuffer.Match(_L("get")))
			iCurCmd = EGet;
		else if (KErrNotFound != iCmdBuffer.Match(_L("bin")))
			iCurCmd = EBin;
		else if (KErrNotFound != iCmdBuffer.Match(_L("asc")))
			iCurCmd = EAsc;
		else if (KErrNotFound != iCmdBuffer.Match(_L("rest")))
			iCurCmd = ERest;
		else if (KErrNotFound != iCmdBuffer.Match(_L("over")))
			iCurCmd = EOver;
		else if (KErrNotFound != iCmdBuffer.Match(_L("expand")))
			iCurCmd = EExpand;
		else if (KErrNotFound != iCmdBuffer.Match(_L("del")))
			iCurCmd = EDel;
		else if (KErrNotFound != iCmdBuffer.Match(_L("ren")))
			iCurCmd = ERen;
		else if (KErrNotFound != iCmdBuffer.Match(_L("mkdir")))
			iCurCmd = EMkdir;
		else if (KErrNotFound != iCmdBuffer.Match(_L("rmdir")))
			iCurCmd = ERmdir;
		else if (KErrNotFound != iCmdBuffer.Match(_L("cd")))
			iCurCmd = ECd;
		else if (KErrNotFound != iCmdBuffer.Match(_L("pwd")))
			iCurCmd = EPwd;
		else if (KErrNotFound != iCmdBuffer.Match(_L("lcd")))
			iCurCmd = ELcd;
		else if (KErrNotFound != iCmdBuffer.Match(_L("list")))
			iCurCmd = EList;
		else if (KErrNotFound != iCmdBuffer.Match(_L("ls")))
			iCurCmd = EList;
		else if (KErrNotFound != iCmdBuffer.Match(_L("quit")))
			iCurCmd = EQuit;
		else if (KErrNotFound != iCmdBuffer.Match(_L("ver")))
			iCurCmd = EVer;
		else if (
			(KErrNotFound != iCmdBuffer.Match(_L("help")))
			||(KErrNotFound != iCmdBuffer.Match(_L("?"))))
			iCurCmd = EHelp;
		else if (iCmdBuffer.Length() == 0)
			{
			iCurCmd = EInvalid;
			return EContinue;
			}
		else
			{
			iConsole->Write(_L("Enter a valid command please\n"));
			iCurCmd = EInvalid;
			return EError;
			}
	return ESuccess;
	}

TBool CFtpUI::Execute(void)
/**
Execute a previously parsed command
*/
	{
	TUint	tempValue;
	iUParam.Copy(iParamBuffer1);
	TLex input(iUParam); // Needed to convert rest parameter
	// Reset server message
	switch (iCurCmd)
		{
		
		case EInvalid:
			iConsole->Write(_L("Enter a valid command first please\n"));
			return EFalse;
		// Connection commands
		case EConnectDNS:
			iFTPSession->Connect(iUParam,iParamBuffer2,iParamBuffer3,iConnMode);
			break;
		case EConnectIP:
		case EClose:
			iFTPSession->Close();
			break;
		case EActive:
			iConnMode = CFTPSession::EActive;
			iConsole->Write(_L("Now connecting in active mode -Dude!\n"));
			// Synchronous cmd Get next cmd
			iCmdConsole->Reset();
			break;
		case Epasv:
			iConnMode = CFTPSession::Epassive;
			iConsole->Write(_L("Now connecting in passive mode -Dude!\n"));
			// Synchronous cmd Get next cmd
			iCmdConsole->Reset();
			break;
		// Xfer cmds
		case EPut:
			iTransferProgress = 0;
			iProgress=EFalse;
            iProgressIdx=0;
			iFTPSession->Store(iUParam,
							iParamBuffer1,
							FALSE,
							iType,
							CFTPSession::EStream);
							iLastTime.HomeTime();          
			break;
		case EGet:
			iTransferProgress = 0;
			iLastTime.HomeTime();
           iProgress=EFalse;
           iProgressIdx=0;
			iFTPSession->Retrieve(iParamBuffer1,
							iUParam,
							iOpenMode,
							iType,
							CFTPSession::EStream);

			break;
		case EBin:
			iType = CFTPSession::EBinary;
			iConsole->Write(_L("Representation type is now binary\n"));
			// Synchronous cmd Get next cmd
			iCmdConsole->Reset();
			break;
		case EAsc:
			iType = CFTPSession::EASCII;
			iConsole->Write(_L("Representation type is now ASCII\n"));
			// Synchronous cmd Get next cmd
			iCmdConsole->Reset();
			break;
		case EVer:
			iConsole->Printf(_L("Version is:%x\n"),iFTPSession->GetVersion());
			// Synchronous cmd Get next cmd
			iCmdConsole->Reset();
			break;
		case ERest:
			input.Val(tempValue,EDecimal);
			iConsole->Write(_L("Restarting next file transfer at: "));
			iConsole->Printf(_L("%u\n"),tempValue);
			iFTPSession->Restart(tempValue);
			// Synchronous cmd Get next cmd
			iCmdConsole->Reset();
			break;
		case EExpand:
			iConsole->Write(_L("Now openning local files in Expand mode\n"));
			iOpenMode = CFTPSession::EExpand;
			// Synchronous cmd Get next cmd
			iCmdConsole->Reset();
			break;
		case EOver:
			if(iOpenMode == CFTPSession::EOverwrite)
				{
				iConsole->Write(_L("Now openning local files in Non Overwriting mode\n"));
				iOpenMode = CFTPSession::ENoOverwrite;
				}
			else
				{
				iConsole->Write(_L("Now openning local files in Overwriting mode\n"));
				iOpenMode = CFTPSession::EOverwrite;
				}
			// Synchronous cmd Get next cmd
			iCmdConsole->Reset();
			break;
		// Local Filesystem cmds
		case ELcd:
			{/* PG 13/08/1999 This code needs beefed up to implement a clever parser
			//Fetch the current session path
			iFs.SessionPath(iDirPath);
			//Unicodify
			iUParam.Copy(iParamBuffer1);
			p.Set(iUParam,NULL,&iDirPath);
			
			iConsole->Printf(_L("current session path:>%S<\n"),&iDirPath);
			iConsole->Printf(_L("Parse.FullName:>%S<\n"),&p.FullName());
			iConsole->Printf(_L("Parse.DriveAndPath:>%S<\n"),&p.DriveAndPath());
			switch(iUParam.Length())
			{
				case 0:
				//if no argument is given to lcd
				// just print the current default path
					iConsole->Printf(_L("local directory is: %S\n"),&p.FullName());
					break;
				default:
					//Check if root 
					if (p.IsRoot())
						{}
					else
						{
						// Check this path
						if (KErrNone != iFs.Entry(p.FullName(),anEntry))
							{
							iConsole->Printf(_L("%S directory not found\n"),&p.FullName());
							break;
							}
						if (!anEntry.IsDir())
							{
							iConsole->Printf(_L("%S is not a directory\n"),&p.FullName());
							break;
							}
						if (KErrNone != iFs.SetSessionPath(p.FullName()))
							{
							iConsole->Printf(_L("could not set session directory to %S to \n"),&p.FullName());
							break;
							}
						}
				iConsole->Printf(_L("Local directory now: %S"),&p.FullName());
				}
			*/
			// Synchronous cmd Get next cmd
			iCmdConsole->Reset();
			break;
			}
		// Remote Filesystem cmds
		case EDel:
			iFTPSession->DeleteFile(iParamBuffer1);
			break;
		case ERen:
			iFTPSession->RenameFile(iParamBuffer1,iParamBuffer2);
			break;
		// Remote Dir cmds
		case EMkdir:
			iFTPSession->CreateDirectory(iParamBuffer1);
			break;
		case ERmdir:
			iFTPSession->DeleteDirectory(iParamBuffer1);
			break;
		case ECd:
			iFTPSession->ChangeDirectory(iParamBuffer1);
			break;
		case EPwd:
			iFTPSession->GetCurrentDirectory();
			break;
		case EList:
			iFileBuffer.FillZ(iFileBuffer.MaxLength());
			iFileBuffer.Zero();
			iFTPSession->ListDirectory(_L8("."),
									iFileBuffer);
			break;
		// Help
		case EHelp:
			Help();
			// Synchronous cmd Get next cmd
			iCmdConsole->Reset();
			break;
		// Exit
		case EQuit:
			CActiveScheduler::Stop();
		default:
			break;
		}
	return ETrue;
	}

void CFtpUI::Help(void)
	{
	iConsole->Write(_L("Enter an FTP Command - commands are:\n"));
	iConsole->Write(_L("active	: Connect in active mode   - pasv	: Connection in passive mode\n"));
	iConsole->Write(_L("connect <hostname|IP address>      - close  : Close connection\n"));
	iConsole->Write(_L("cd <dirName>    : Change directory - pwd 	: Print current directory\n"));
	iConsole->Write(_L("mkdir <dirName> : Create directory - rmdir <dirName> : Remove directory\n"));
	iConsole->Write(_L("list  [dirName] : List directory\n"));
	iConsole->Write(_L("del   <filename>: Delete file\n"));
	iConsole->Write(_L("ren   <Oldname> <NewName>	: Rename file\n"));
	iConsole->Write(_L("bin   : Xfer file in binary mode - asc : Xfer file in Ascii mode\n"));
	iConsole->Write(_L("over  : Toggle overwriting of existing Xfered file\n"));
	iConsole->Write(_L("expand: Expand existing Xfered file\n"));
	iConsole->Write(_L("rest <offset>  : Restart next file Xfer at offset\n"));
	iConsole->Write(_L("put <Filename> : Store a file onto a server\n"));
	iConsole->Write(_L("get <filename> : Get a file from server\n"));
	iConsole->Write(_L("ver : returns the dlls version numbers\n"));
	iConsole->Write(_L("Hit the Escape key to cancel the current operation\n"));
	}


