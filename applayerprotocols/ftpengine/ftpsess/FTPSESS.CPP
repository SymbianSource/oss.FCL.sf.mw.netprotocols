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
// EPOC32 FTP Engine header file
// Author:	Philippe Gabriel
// Implements set of APIs simplyfying access to the FTP protocol
// 
//

/**
 @file FTPSESS.CPP
 @internalComponent
*/

#include <e32test.h>
#include "SESSION.H"
#include "DEBUG.H"

//
//					Minterface Implementation
//

void CFTPSessionDerived::SetErrorNotifier(const TInt aError)
/**
Define the CFTPSetError callback notifier
*/
{
	switch(aError)
		{
		case MFtpSessionNotifier::ESocketError:
			iFtpSessionNotifier->ConnectionError((MFtpSessionNotifier::TOpComp)aError);
			ErrorTransition(MFtpProtocolNotifier::EOpConnectionFailed);
			break;
		case MFtpSessionNotifier::EAlreadyConnected:
		case MFtpSessionNotifier::ENotConnected:
			iFtpSessionNotifier->ConnectionError((MFtpSessionNotifier::TOpComp)aError);
			//No state transition in this case
			break;
		case MFtpSessionNotifier::EFileSystemError:
		case MFtpSessionNotifier::EFileOpenFailure:
		case MFtpSessionNotifier::EFileReadError:
		case MFtpSessionNotifier::EFileWriteError:
		case MFtpSessionNotifier::EFileAlreadyExist:
		case MFtpSessionNotifier::EFileNotExist:
		case MFtpSessionNotifier::EDirAlreadyExist:
		case MFtpSessionNotifier::EDirNotExist:
			iFtpSessionNotifier->LocalFileSystemError((MFtpSessionNotifier::TOpComp)aError);
			ErrorTransition(MFtpProtocolNotifier::EOpFailed);
			break;
		case MFtpSessionNotifier::EOpCanceled:
			iFtpSessionNotifier->Cancel();
			ErrorTransition(MFtpProtocolNotifier::EOpCanceled);
			break;
		default:
			// I forgot something
			__ASSERT_DEBUG(FALSE, User::Panic(_L("CFTPSessionDerived::SetError"), 0));
			break;
		}
}

void CFTPSessionDerived::ErrorNotification(const TOpComp aStatus)
{
	switch(aStatus)
		{
		case MFtpProtocolNotifier::EOpConnectionReset:
			// Total mayhem, irrecoverable
			// We got a TCP RST
			// Notify upper layer
			iFtpSessionNotifier->ConnReset();
			break;
		case MFtpProtocolNotifier::EOpConnectionFailed:
			__ASSERT_DEBUG(iState == EConnecting, User::Panic(_L("CFTPSessionDerived::ErrorNotification"), 0));
			iFtpSessionNotifier->ConnectionError(MFtpSessionNotifier::EConnectionFailed);
			break;
		case MFtpProtocolNotifier::EXferReset:
			__ASSERT_DEBUG((iState == EPerformingList)
						||(iState == ERetrieveFile)
						||(iState == EStoreFile)
						||(iState == EStoreFileSendEOF),
						User::Panic(_L("CFTPSessionDerived::ErrorNotification"), 0));
			iFtpSessionNotifier->RemoteFileSystemError(MFtpSessionNotifier::ERemoteFileSystemError);
			break;
		case MFtpProtocolNotifier::EXferNotInitialised:
			__ASSERT_DEBUG((iState == EPerformingList)
						||(iState == ERetrieveFile)
						||(iState == EStoreFile)
						||(iState == EStoreFileSendEOF),
						User::Panic(_L("CFTPSessionDerived::ServerNegativeAnswerNotification"), 0));
			iFtpSessionNotifier->RemoteFileSystemError(MFtpSessionNotifier::ERemoteFileOpenFailure);
			break;
		case MFtpProtocolNotifier::EHostNotFound:
			iFtpSessionNotifier->ConnectionError(MFtpSessionNotifier::EHostNotExist);
			break;
		case MFtpProtocolNotifier::EOpCanceled:
			iFtpSessionNotifier->Cancel();
			break;
		default:
			// I forgot something
			__ASSERT_DEBUG(FALSE, User::Panic(_L("ErrorNotification::SetError"), 0));
			iFtpSessionNotifier->EUnknownError();
			break;
		}
		ErrorTransition(aStatus);
}

void CFTPSessionDerived::ServerMessage(const TDesC8& aMessage)
{
	iFtpSessionNotifier->ServerMessage(aMessage);
}

void CFTPSessionDerived::ServerNegativeAnswerNotification(const TOpComp /*aStatus*/)
{
	// Fetch the answer
	iCFtpProtocol->FTPServerAnswer(iServerAnswer);
	// Have to discriminate on the answer to send back a meaningfull error
	// fetch a Lex object
	TLex input(iServerAnswer);
	TUint uintServerAnswer;
	// Convert 
	input.Val(uintServerAnswer,EDecimal);
	FTPPROTDEBUG1(_DBGFtpsess,_L("CFTPSessionDerived::ServerNegativeAnswerNotification Server converted answer:<%u>\n"),uintServerAnswer);
	// Check the answer is within bounds
	if ((uintServerAnswer<(TUint)100) || (uintServerAnswer>(TUint)599))
		{
		FTPPROTDEBUG2(0xffff,_L("!!!!!!!!!!!!!CFTPSessionDerived::ServerNegativeAnswerNotification Server answer:<%d> is out of bound\n"),iServerAnswer);
		__ASSERT_DEBUG(FALSE, User::Panic(_L("CFTPSessionDerived::ServerNegativeAnswerNotification"), 0));
		// Set it to a generic failure code
		uintServerAnswer = 599;
		}
	// Check this is really an error
	__ASSERT_DEBUG(uintServerAnswer>199, User::Panic(_L("CFTPSessionDerived::ServerNegativeAnswerNotification"), 0));
	// Unfortunately servers do not seem to follow the RFC recommendation very well
	// And the list of messages in RFC959 p40-42 is not reliable.
	// All we can do is best guess, anything more appropriate need the user
	// to read the text meesage sent back by the server
	switch(iState)
		{
		// Intermediate State
		case EConnecting:
			// Assume server unreachable
			iFtpSessionNotifier->ConnectionError(MFtpSessionNotifier::EConnectionFailed);
			break;
		case ELoginUser:
		case ELoginPass:
			// Assume login,pass invalid
			iFtpSessionNotifier->ConnectionError(MFtpSessionNotifier::ELoginFailed);
			break;
		case ERenameFileFrom:
		case ERenameFileTo:
			// file not found or no access
			iFtpSessionNotifier->RemoteFileSystemError(MFtpSessionNotifier::EPermissionDenied);
			break;
		case EPerformingPortForList:
		case EPerformingPasvForList:
		case EPerformingPortForRetrieve:
		case EPerformingPasvForRetrieve:
		case EPerformingPasvForStore:
		case EPerformingPortForStore:
			// What kind of error could we have following a port?
			iFtpSessionNotifier->EUnknownError();
			break;
		case EPerformingRestForRetrieve:
			// file not found or no access
			iFtpSessionNotifier->RemoteFileSystemError(MFtpSessionNotifier::ERestartNotSupported);
			break;
		// Semi-Intermediate states
		case EPerformingList:
		case ERetrieveFile:
		case EStoreFile:
			iFtpSessionNotifier->RemoteFileSystemError(MFtpSessionNotifier::ERemoteFileSystemError);
			break;
		// Final States, call back notifier
		case EConnected:
			// Assume timeout
			iFtpSessionNotifier->ConnectionError(MFtpSessionNotifier::ETimedOut);
			break;
		case EStoreFileSendEOF:
		case EGetCurrentDirectory:
		case EChangeDirectory:
		case ECreateDirectory:
		case EDeleteDirectory:
		case EDeleteFile:
			iFtpSessionNotifier->RemoteFileSystemError(MFtpSessionNotifier::EPermissionDenied);
			break;
		case EClosing:
			// What kind of error could we have here?
			iFtpSessionNotifier->EUnknownError();
			break;
		default:
			// Invalid state
			__ASSERT_DEBUG(FALSE, User::Panic(_L("CFTPSessionDerived::ServerNegativeAnswerNotification"), 0));
			break;
		}
	ErrorTransition(MFtpProtocolNotifier::EOpFailed);
}

void CFTPSessionDerived::ServerXFerNotification(const TOpComp /*aStatus*/)
{
FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::ServerXFerNotification called - State:"));
TDes8& currentBuf = iFileXferBuffer[iCurrentXferBuffer];
iCurrentXferBuffer = !iCurrentXferBuffer; //swap buffers;

	switch(iState)
		{
		// Semi-Intermediate states
		case EPerformingList:
			FTPPROTDEBUG(_DBGFtpsess,_L("EPerformingList \n"));
			iFtpSessionNotifier->MoreData();
			break;
		case ERetrieveFile:
			FTPPROTDEBUG(_DBGFtpsess,_L("ERetrieveFile \n"));
			// Reissue the read request
 			iCFtpProtocol->RecvBuffer(&iFileXferBuffer[iCurrentXferBuffer]);
			// Save packet in the open file
			if(iFileOpErrorOccured || (KErrNone != iFile.Write(currentBuf)))
				{
				// Notify error
				if(!iFileOpErrorOccured)
					//1st time, notify
					iFtpSessionNotifier->LocalFileSystemError(MFtpSessionNotifier::EFileWriteError);
				iFileOpErrorOccured = TRUE;
			//	__ASSERT_DEBUG(fileOpReturn==KErrNone, User::Panic(_L("CFTPSessionDerived::ServerXFerNotification"), 0));
				}
			// Just to be sure
			if(iFileOpErrorOccured || (KErrNone != iFile.Flush()))
				{
				// Notify error
				if(!iFileOpErrorOccured)
					//1st time, notify
					iFtpSessionNotifier->LocalFileSystemError(MFtpSessionNotifier::EFileWriteError);
				iFileOpErrorOccured = TRUE;
			//	__ASSERT_DEBUG(fileOpReturn==KErrNone, User::Panic(_L("CFTPSessionDerived::ServerXFerNotification"), 0));
				}
			// Call the Progress notifier
			iFtpSessionNotifier->TransferProgress(currentBuf.Length());
			break;
		case EStoreFile:
			FTPPROTDEBUG(_DBGFtpsess,_L("EStoreFile \n"));
			// Call the Progress notifier
			if((iFileOpErrorOccured || (currentBuf.Length() == 0)) && !iFirstRun)
 				{
 				// Send an EOF
 				iState = EStoreFileSendEOF;
 				iCFtpProtocol->SendEOF();
 				}
 			else {
 				// Reissue the Send request
 				iCFtpProtocol->SendBuffer(&currentBuf);
 				iFirstRun=EFalse;
			}
 
			iFtpSessionNotifier->TransferProgress(currentBuf.Length());
			
			// Construct a packet from the open file
			if(iFileOpErrorOccured || (KErrNone != iFile.Read(iFileXferBuffer[iCurrentXferBuffer])))
				{
				// Notify error
				if(!iFileOpErrorOccured)
					//1st time, notify
					iFtpSessionNotifier->LocalFileSystemError(MFtpSessionNotifier::EFileWriteError);
				iFileOpErrorOccured = ETrue;
				}	
			break;
		case EAbortingStore:
			FTPPROTDEBUG(_DBGFtpsess,_L("EAbortingStore \n"));
			// Shutdown the DTP channel
			iCFtpProtocol->SendEOF();
			break;
		case EAbortingRetrieve:
		case EAbortingList:
			FTPPROTDEBUG(_DBGFtpsess,_L("EAbortingRetrieve - EAbortingList\n"));
			// Reissue the read request to flush the recv buffer
			iCFtpProtocol->RecvBuffer(&currentBuf);
			break;
		default:
			FTPPROTDEBUG(_DBGFtpsess,_L("INVALID \n"));
			// Invalid state
			__ASSERT_DEBUG(FALSE, User::Panic(_L("CFtpProtocol::ServerXFerNotification"), 0));
			break;
		}
}

void CFTPSessionDerived::ServerPositiveAnswerNotification(const TOpComp /*aStatus*/)
/**
Define the CFtpProtocol callback notifiers
*/
{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::ServerPositiveAnswerNotification called\n"));
	// Fetch the answer
	// PG 200898 Don't need the answer just now, maybe in the future
	// iCFtpProtocol->FTPServerAnswer(iServerAnswer);
	// Just do a normal transition
	NormalTransition();
}
//
//							State Transition functions
//

void CFTPSessionDerived::NormalTransition(void)
/**
NormalTransition
Called when an operation has been positively acknowledged
and we progress to the next operation
No input parameter, only operates on global member data
*/
{
FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::NormalTransition called\n"));
TBool loopAgain;
	do
	{
		// Do Pre State update action
		switch(iState)
			{
			case ERetrieveFile:
			case EStoreFileSendEOF:
				//Finished, replace the old local file with the temp one 
				//and delete the temp file
				if(iTempStore)
				{
                    iTempStore = FALSE;
					if(iFs.Delete(iRenamedLocalFileName)!=KErrNone)
					{
						iCFTPSetError->SetError(MFtpSessionNotifier::EFileSystemError);
						return;
					}	
				}
				iFile.Close();
				break;
			case EPerformingTypeForRetrieve:
			case EPerformingTypeForStore:
				iCurrentRepresentationType = iNewRepresentationType;
				break;
			case EAbortingStore:
			case EAbortingRetrieve:
			case EAbortingList:
				iFtpSessionNotifier->Cancel();
				break;
			case EConnected:
				// We should never get here
				// BUT WE DO, reason is:
				// during xfers, some ftp servers don't ack the abort
				// where we (the client) enter an abort state and wait for the server to
				// send us either
				// 4 (xfer canceled) - 2 (abort ack)
				// or just 2 (abort ack)
				// but some lame servers send us:
				// 2 (xfer finished) - 2 (abort ack)
				// Don't report a completion to the client just bail out
				return;
			default:
				break;
			}
		// Update the state
		iState = UpdateState(iState,MFtpProtocolNotifier::EOpComplete);
		// Do some action according to the new state
		loopAgain = FALSE;
		switch(iState)
			{
			// Intermediate State
			// Don't callback notifier
			case EInitiateListOperation:
			case EInitiateRetrieveOperation:
			case EInitiateStoreOperation:
				// These are just a bunch of dummy states
				// allowing to streamline the code a bit
				loopAgain = TRUE;
				break;
			case ELoginUser:
				iCFtpProtocol->User(iBuffer1);
				break;
			case ELoginPass:
				iCFtpProtocol->Pass(iBuffer2);
				break;
			case ERenameFileTo:
				iCFtpProtocol->Rnto(iBuffer1);
				break;
			case EPerformingTypeForRetrieve:
			case EPerformingTypeForStore:
				if(iNewRepresentationType != iCurrentRepresentationType)
					{
					// New type differs from the old, tell the server to change it
					if(iNewRepresentationType == EASCII)
						iCFtpProtocol->Type(_L8("A"));
					else 
						iCFtpProtocol->Type(_L8("I"));
					break;
					}
				else
					// New type same as the old, do nothing
					loopAgain = TRUE;
				break;
			case EPerformingPasvForList:
			case EPerformingPasvForRetrieve:
			case EPerformingPasvForStore:
				iCFtpProtocol->Pasv();
				break;
			case EPerformingPortForList:
			case EPerformingPortForRetrieve:
			case EPerformingPortForStore:
				iCFtpProtocol->Port();
				break;
			case EPerformingList:
				// Set buffer 
				iCFtpProtocol->RecvBuffer(iFileList);
				iCFtpProtocol->List(iBuffer1);
				break;
			case EPerformingRestForStore:
			case EPerformingRestForRetrieve:
				// Do I really need to send a restart?
				if(iRESTOffset !=0 )
					{
					//Send a REST command
					iBuffer2.Num(iRESTOffset,EDecimal);
					// Reset immediately the RESTOffset
					iRESTOffset = 0;
					iCFtpProtocol->Rest(iBuffer2);
					}
				else
					loopAgain = TRUE;
				break;
			case ERetrieveFile:
				// Set buffer 
				iCFtpProtocol->RecvBuffer(&iFileXferBuffer[iCurrentXferBuffer]);
				iCFtpProtocol->Retr(iBuffer1);
				break;
			case EStoreFile:
				// Set buffer 
				iCFtpProtocol->Stor(iBuffer1);
				// Construct a packet from the open file
				iFileXferBuffer[iCurrentXferBuffer].SetLength(0);
 				iCurrentXferBuffer = !iCurrentXferBuffer;
 				iFile.Read(iFileXferBuffer[iCurrentXferBuffer]);
 				//Don't send yet, just bump the notifier
 				iFirstRun=ETrue;
 				ServerXFerNotification(EOpComplete);
				break;
			// Final States, call back notifier
			case EConnected:
			case EIdle:
				iFtpSessionNotifier->Complete();
				break;
			default:
				// Invalid state
				__ASSERT_DEBUG(FALSE, User::Panic(_L("CFtpProtocol"), 0));
				break;
			}
	}while(loopAgain);
}

void CFTPSessionDerived::ErrorTransition(const TOpComp aStatus)
/**
ErrorTransition
Called when an operation has failed
need to do some clearup
and determine what the next state is gonna be
*/
{
	// Close local file
	iFile.Close();

    if ( iTempStore )
    {//rename orig file back iRenamedLocalFileName contains original file name as well
        iTempStore = FALSE;
    	TBuf<256> fileName;
        fileName.Copy( iRenamedLocalFileName.Ptr(), iRenamedLocalFileName.Length() - 1 );
		iFs.Delete( fileName ); //just in case
        iFs.Rename( iRenamedLocalFileName, fileName );
        //should i report a possible error to anyone?
    }
	// Trust the lower layer to cleanup the connection mess
	// just find out what the next state is
	iState = UpdateState(iState,aStatus);
}

CFTPSessionDerived::TState	CFTPSessionDerived::UpdateState(CFTPSessionDerived::TState aState,MFtpProtocolNotifier::TOpComp aStatus)
	{
	CFTPSessionDerived::TState returnValue;
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::UpdateState called\n"));
	switch(aStatus)
		{
		case MFtpProtocolNotifier::EOpComplete:
			switch(aState)
				{
				case EConnecting:
					returnValue = ELoginUser;
					break;
				case ELoginUser:
					returnValue = ELoginPass;
					break;
				case ERenameFileFrom:
					returnValue = ERenameFileTo;
					break;
				case 	EInitiateListOperation:
					if(iPASVMode)
						returnValue = EPerformingPasvForList;
					else
						returnValue = EPerformingPortForList;
					break;
				case	EInitiateRetrieveOperation:
					returnValue = EPerformingTypeForRetrieve;
					break;
				case	EInitiateStoreOperation:
					returnValue = EPerformingTypeForStore;
					break;
				case EPerformingTypeForRetrieve:
					if(iPASVMode)
						returnValue = EPerformingPasvForRetrieve;
					else
						returnValue = EPerformingPortForRetrieve;
					break;
				case EPerformingTypeForStore:
					if(iPASVMode)
						returnValue = EPerformingPasvForStore;
					else
						returnValue = EPerformingPortForStore;
					break;
				
				case EPerformingPortForList:
				case EPerformingPasvForList:
					returnValue = EPerformingList;
					break;
				case EPerformingPortForRetrieve:
				case EPerformingPasvForRetrieve:
					returnValue = EPerformingRestForRetrieve;
					break;
				case EPerformingPortForStore:
				case EPerformingPasvForStore:
					returnValue = EPerformingRestForStore;
					break;
				case EPerformingRestForRetrieve:
					returnValue = ERetrieveFile;
					break;
				case EPerformingRestForStore:
					returnValue = EStoreFile;
					break;
				case ERetrieveFile:
				case EStoreFileSendEOF:
				case EPerformingList:
				case ELoginPass:
				case EGetCurrentDirectory:
				case EChangeDirectory:
				case ECreateDirectory:
				case EDeleteDirectory:
				case EDeleteFile:
				case ERenameFileTo:
				case EAbortingStore:
				case EAbortingRetrieve:
				case EAbortingList:
					returnValue = EConnected;
					break;
				case EClosing:
					returnValue = EIdle;
					break;
				default:
					// Invalid state
					__ASSERT_DEBUG(FALSE, User::Panic(_L("CFTPSessionDerived::UpdateState Bad State"), 0));
					returnValue = EIdle;
					break;
				} // switch (aState)
				break;
			//Cancel
			case MFtpProtocolNotifier::EOpCanceled:
				switch(aState)
					{
					case EAbortingStore:
					case EAbortingRetrieve:
					case EAbortingList:
						returnValue = EConnected;
						break;
					case EConnecting:
						returnValue = EIdle;
						break;
					default:
						//Otherwise don't change the state
						returnValue = aState;
						break;
					}
				break;
			// Error statuses
			case MFtpProtocolNotifier::EOpFailed:
				// This means I got a Negative answer from the PI Channel
				// hence PI Channel still there ==> still connected
				returnValue = EConnected;
				break;
			case MFtpProtocolNotifier::EOpConnectionReset:
			case MFtpProtocolNotifier::EOpConnectionFailed:
			case MFtpProtocolNotifier::EHostNotFound:
				//Simple massive errors
				// No need to look at the state
					returnValue = EIdle;
					break;
			case MFtpProtocolNotifier::EXferReset:
			case MFtpProtocolNotifier::EXferNotInitialised:
					returnValue = EConnected;
					break;
			case MFtpProtocolNotifier::ESocketError:
				switch(aState)
					{
					// Socket error while connecting PI Channel
					case EConnecting:
						returnValue = EIdle;
						break;
					// else socket error while openning DTP Channel
					default:
						// Check the state is in the acceptable range
						__ASSERT_DEBUG(
							(aState == EPerformingPortForList)||
							(aState == EPerformingList)||
							(aState == EPerformingPortForRetrieve)||
							(aState == ERetrieveFile)||
							(aState == EPerformingPortForStore)||
							(aState == EStoreFile)
						, User::Panic(_L("CFTPSessionDerived::UpdateState Invalid State"), 0));
						returnValue = EConnected;
						break;
					}
				break;
			default:
				// Invalid status
				__ASSERT_DEBUG(FALSE, User::Panic(_L("CFTPSessionDerived::UpdateState Bad Status"), 0));
				returnValue = EIdle;
				break;
		} // switch (aStatus)
		return returnValue;
	}
//
//							Creation, Destruction
//

CFTPSessionDerived::CFTPSessionDerived(
						 ):iState(EIdle),FileServerInitialised(FALSE)
							,iCurrentRepresentationType(EUninitialised),iRESTOffset(0)
/**
C++ constructor
*/
	{
	}

EXPORT_C CFTPSession* CFTPSession::NewL(MFtpSessionNotifier* aNotifier)
/** Allocates and constructs a new FTP session object. 
*
* @param aNotifier	Callback interface to notify the client of the completion 
* 					of operations or to report errors. For each FTP session, the FTP client should 
* 					instantiate an object of this type.
* @return			New FTP session object */
	{
	return CFTPSessionDerived::NewL(aNotifier);
	}

CFTPSessionDerived* CFTPSessionDerived::NewL(MFtpSessionNotifier* aNotifier)
	{
	CFTPSessionDerived* self=new (ELeave) CFTPSessionDerived();
	CleanupStack::PushL(self);
	self->ConstructL(aNotifier);
    CleanupStack::Pop();
	return self;
	}

void CFTPSessionDerived::ConstructL(MFtpSessionNotifier* aNotifier)
/**
2nd phase construction
*/
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::ConstructL called\n"));
	iCFtpProtocol = CFtpProtocol::NewL(this);
	iCFTPSetError = CFTPSetError::NewL(this);
	User::LeaveIfError(iFs.Connect());
	iFtpSessionNotifier = aNotifier;
	}

CFTPSession::~CFTPSession()
/** Destructor. */
	{}

CFTPSessionDerived::~CFTPSessionDerived()
/**
Destruction
*/
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::~CFTPSessionDerived() called\n"));
	iFs.Close();
	delete iCFtpProtocol;
	delete iCFTPSetError;
	}

//
//							Interface
//

void CFTPSessionDerived::Connect(	const TSockAddr& aNetAddr, //IP address
						const TDesC8& aUserName, 
						const TDesC8& aPassword,
						const TConnectionMode aConnectionMode)
/**
Establish a connection with a server:
*/
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::Connect called\n"));
	if(iState != EIdle)
		{
		// Already connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::EAlreadyConnected);
		return;
		}
	iPASVMode = (aConnectionMode == Epassive);
	iBuffer1 = aUserName;
	iBuffer2 = aPassword;
	iTInt1 = aConnectionMode;
	iState = EConnecting;
	iCFtpProtocol->Connect((TSockAddr&)aNetAddr);
	}

void CFTPSessionDerived::Connect(
						const THostName& aServerName, 
						const TDesC8& aUserName, 
						const TDesC8& aPassword,
						const TConnectionMode aConnectionMode, 
						const TUint aPort)
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::Connect called\n"));
	if(iState != EIdle)
		{
		// Already connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::EAlreadyConnected);
		return;
		}
	iPASVMode = (aConnectionMode == Epassive);
	iBuffer1 = aUserName;
	iBuffer2 = aPassword;
	iTInt1 = aConnectionMode;
	iState = EConnecting;
	iCFtpProtocol->Connect(aServerName,aPort);
	}

void CFTPSessionDerived::Close()
/**
Close connection with a server
*/
	{
	if(iState == EIdle)
		{
		// Not connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::ENotConnected);
		return;
		}
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::Close called\n"));
	iState = EClosing;
	iCFtpProtocol->Quit();
	}

void CFTPSessionDerived::Cancel()
/**
Cancel latest FTP operation
*/
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::Cancel called\n"));
	// Switch on current state
	switch(iState)
		{
		// If I'm storing or recving files I need to negociate 
		// an abort with the server
		case EStoreFile:
			iState = EAbortingStore;
			iCFtpProtocol->Abor();
			iFile.Close();
			break;
		case ERetrieveFile:
			iState = EAbortingRetrieve;
			iCFtpProtocol->Abor();
			iFile.Close();
			break;
		case EPerformingList:
			iState = EAbortingList;
			iCFtpProtocol->Abor();
			break;
		// Simpler states, no negociation with server
		case EConnecting:
			// Note: UserCancel is synchronous, hence need to trigger a 
			// notification myself
			iCFtpProtocol->UserCancel();
			iCFTPSetError->SetError(MFtpSessionNotifier::EOpCanceled);
			break;
		default:
			// Cancel currently only implemented for store and receive
			iCFTPSetError->SetError(MFtpSessionNotifier::EOpCanceled);
			return;
		}
	}

// Passes a marker object to store a position for an aborted transfer operation
//void CFTPSessionDerived::SetMarker(TFTPRestartMarker& /*aTFTPRestartMarker*/)
//	{
//	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::SetMarker called\n"));
//	FTPPROTDEBUG(_DBGFtpsess,_L("Not Implemented Yet -- Sorry\n"));
//	}

void CFTPSessionDerived::Restart(const TUint aTFTPRestartOffset)
/**
Restart an aborted transfer operation
*/
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::Restart called\n"));
	iRESTOffset = aTFTPRestartOffset;
	}

void CFTPSessionDerived::Store(const TDesC& aLocalFileName,
					const TDesC8& aNewRemoteFileName,
					const TBool	/*aOverwrite*/,
					const RepresentationType aRepresentationType,
					const TransferMode /*aTransferMode*/)
/**
Transfer APIs
Store a file on the server	
*/
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::Store called\n"));
	if(iState == EIdle)
		{
		// Not connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::ENotConnected);
		return;
		}
	// Try to open the local file
	if (iFile.Open(iFs, aLocalFileName, EFileRead|EFileShareReadersOnly) != KErrNone)
		{
		iCFTPSetError->SetError(MFtpSessionNotifier::EFileNotExist);
		return;
		}
	debugInt =0;
	iFileOpErrorOccured = FALSE;
	iBuffer1 = aNewRemoteFileName;
	iNewRepresentationType = aRepresentationType;
	iState = EInitiateStoreOperation;
	NormalTransition();
	}

void CFTPSessionDerived::Retrieve(const TDesC8& aRemoteFileName,
					const TDesC& aNewLocalFileName,
					const TOpenMode	aOpenMode,
					const RepresentationType aRepresentationType,
					const TransferMode /*aTransferMode*/)
/**
Get a file from the server
*/
	{
	TInt filepos;
	
	//initialise the flag for indicating if the exisiting local file has been renamed
	__ASSERT_DEBUG( iTempStore == FALSE, User::Panic(_L("CFTPSessionDerived::Retrieve"), 0) );
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::Retrieve called\n"));
	TBuf<256> fileName =aNewLocalFileName;
	TInt  counter;
	// Set the path to C:\ as pert of the defetc Fix DEF058464, to avoid getting 
	// files in C:\private\{uid} folder which is the expected behavior as part 
	// of the platform security.
	iFs.SetSessionPath(_L("C:\\"));

	if(iState == EIdle)
		{
		// Not connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::ENotConnected);
		return;
		}
	// Added to test		
	iFs.SetSessionPath(_L("C:\\"));
	// Try to open the local file
	switch(aOpenMode)
		{
		case EOverwrite:
			// In the local folder see if the remote file already exists 
			if(!iFile.Open(iFs, aNewLocalFileName, EFileWrite|EFileShareExclusive))
			{
				for(counter=1;counter<5;counter++)
				{
					//Create a different name for the existing local file  
					fileName.AppendNum(counter);

					// change the existing local file name to the one created above (if possible)
					// If renaming was succesful, then do the settings and start retrieving, else
					// try a new name. 
					if(!iFile.Rename(fileName))
					{
						iRenamedLocalFileName = fileName;
						break;
					}
					else
					{
						if(counter==4)
						{
							// all the 4 possible names are used.Use the last name
							iFs.Delete(fileName);
							if(iFile.Rename(fileName)!=KErrNone)
							{
								iCFTPSetError->SetError(MFtpSessionNotifier::EFileSystemError);
								return;
							}
						
							iRenamedLocalFileName = fileName;
							break;
						}
						fileName=aNewLocalFileName;
					}
				}
				iFile.Close();
				iTempStore= TRUE;
				if(iFile.Replace(iFs,aNewLocalFileName,EFileWrite|EFileShareExclusive)!=KErrNone)
				{
					iCFTPSetError->SetError(MFtpSessionNotifier::EFileOpenFailure);
					return;
				}
				
				
			}
			else
			{
				if(iFile.Replace(iFs,aNewLocalFileName,EFileWrite|EFileShareExclusive)!=KErrNone)
				{
					iCFTPSetError->SetError(MFtpSessionNotifier::EFileOpenFailure);
					return;
				}
			}
			break;
		case ENoOverwrite:
			if (iFile.Create(iFs, aNewLocalFileName, EFileWrite|EFileShareExclusive) != KErrNone)
				{
				iCFTPSetError->SetError(MFtpSessionNotifier::EFileAlreadyExist);
				return;
				}
			break;
		case EExpand:
			// Open file
			if (iFile.Open(iFs, aNewLocalFileName, EFileWrite|EFileShareExclusive) != KErrNone)
				{
				iCFTPSetError->SetError(MFtpSessionNotifier::EFileNotExist);
				return;
				}
			// Goto EOF
			if (iFile.Seek(ESeekEnd,filepos) != KErrNone)
				{
				iFile.Close();
				iCFTPSetError->SetError(MFtpSessionNotifier::EFileOpenFailure);
				return;
				}
			break;
		}
	// Set up parameters
	debugInt =0;
	iFileOpErrorOccured = FALSE;
	iBuffer1 = aRemoteFileName;
	iNewRepresentationType = aRepresentationType;
	iState = EInitiateRetrieveOperation;
	NormalTransition();
	}

void CFTPSessionDerived::ChangeDirectory(const TDesC8& aDirectoryName)
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::ChangeDirectory called\n"));
	if(iState == EIdle)
		{
		// Not connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::ENotConnected);
		return;
		}
	iState = EGetCurrentDirectory;
	iCFtpProtocol->Cwd(aDirectoryName);
	}

void CFTPSessionDerived::CreateDirectory(const TDesC8& aDirectoryName)
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::CreateDirectory called\n"));
	if(iState == EIdle)
		{
		// Not connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::ENotConnected);
		return;
		}
	iState = ECreateDirectory;
	iCFtpProtocol->Mkd(aDirectoryName);
	}

void CFTPSessionDerived::DeleteDirectory(const TDesC8& aDirectoryName)
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::DeleteDirectory called\n"));
	if(iState == EIdle)
		{
		// Not connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::ENotConnected);
		return;
		}
	iState = EDeleteDirectory;
	iCFtpProtocol->Rmd(aDirectoryName);
	}

void CFTPSessionDerived::GetCurrentDirectory()
	{
	// Result on the PI connection between quotes
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::GetCurrentDirectory called\n"));
	if(iState == EIdle)
		{
		// Not connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::ENotConnected);
		return;
		}
	iState = EGetCurrentDirectory;
	iCFtpProtocol->Pwd();
	}

void CFTPSessionDerived::ListDirectory(const	TDesC8& aDirectoryName,TDes8& aFileList)
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::ListDirectory called\n"));
	if(iState == EIdle)
		{
		// Not connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::ENotConnected);
		return;
		}
	if (iState != EPerformingList)
		{
		iBuffer1 = aDirectoryName;
		iFileList = &aFileList;
		iState = EInitiateListOperation;
		NormalTransition();
		}
	else
		{
		// Set buffer 
		iCFtpProtocol->RecvBuffer(&aFileList);
		}
	}

void CFTPSessionDerived::DeleteFile(const TDesC8& aFileName)
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::DeleteFile called\n"));
	if(iState == EIdle)
		{
		// Not connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::ENotConnected);
		return;
		}
	iState = EDeleteFile;
	iCFtpProtocol->Dele(aFileName);
	}

void CFTPSessionDerived::RenameFile(	const TDesC8& aRemoteFileName,
							const TDesC8& aNewRemoteFileName)
	{
	FTPPROTDEBUG(_DBGFtpsess,_L("CFTPSessionDerived::RenameFile called\n"));
	if(iState == EIdle)
		{
		// Not connected, BailOut
		iCFTPSetError->SetError(MFtpSessionNotifier::ENotConnected);
		return;
		}
	iState = ERenameFileFrom;
	iBuffer1 = aNewRemoteFileName;
	iCFtpProtocol->Rnfr(aRemoteFileName);
	}

EXPORT_C TUint32 CFTPSession::GetVersion(void)
/** Gets API version information.
*
* The vesion information format is:
*
* @li byte 3: ftpsess dll major version
* @li byte 2: ftpsess dll minor version
* @li byte 1: ftpprot dll major version
* @li byte 0: ftpprot dll minor version
*
* @return	Version information */
	{
	TUint32 result;
	result = FTPSESS_VERSION_NUMBER;
	result = result<<16;
	result = result | CFtpProtocol::GetVersion();
	return result;
	}

//
//							DLL Entry point
//

