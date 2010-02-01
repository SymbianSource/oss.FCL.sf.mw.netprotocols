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
//

// Tiny Telnet - A simple telnet client to test CTelnetSession API
#include "TTELNET.H"
//#include <debug.h>


// Device driver names
#if defined (__WINS__)
#define PDD_NAME _L("ECDRV")
#define LDD_NAME _L("ECOMM")
#else
#define PDD_NAME _L("EUART1")
#define LDD_NAME _L("ECOMM")
#endif

static CConsoleBase* console;



LOCAL_C void CommInitL()
//
// Initialisation code - loads the serial LDD and PDD
// starts the comm subsystem (for EPOC32 builds)
// On a full EPOC implementation, this code would not
// be required because higher level components (EIKON) 
// automatically start the services.
//
	{
	// Load the physical device driver
	// The OS will automatically append .PDD and 
	// search /System/Libs on all drives.
	TInt r=User::LoadPhysicalDevice(PDD_NAME);
	if (r != KErrNone && r!= KErrAlreadyExists)
		User::Leave(r);
	//test(r==KErrNone || r==KErrAlreadyExists);

	// Similarly for the Logical device driver
	r=User::LoadLogicalDevice(LDD_NAME);
	if (r != KErrNone && r != KErrAlreadyExists)
		User::Leave(r);
	//test(r==KErrNone|| r==KErrAlreadyExists);

	}



void DriveEngineL();
void SetupConsoleL();

GLDEF_C TInt E32Main() // main function called by E32
    {
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	TRAPD(error,SetupConsoleL()); // more initialization, then do example
	__ASSERT_ALWAYS(!error,User::Panic(_L("TTiny Telnet"),error));
	delete cleanup; // destroy clean-up stack
	
	__UHEAP_MARKEND;
	return 0; // and return
    }

void SetupConsoleL() // initialize and call example code under cleanup stack
    {
	console=Console::NewL(_L("Tiny Telnet"),
		TSize(KDefaultConsWidth,KDefaultConsHeight));
	CleanupStack::PushL(console);
	TRAPD(error,DriveEngineL()); // perform example function
	if (error) console->Printf(_L("failed: leave code=%d"), error);
	else console->Printf(_L("ok"));
	console->Printf(_L(" [press any key]"));
	console->Getch(); // get and ignore character
	CleanupStack::PopAndDestroy(); // close console
    }

/*
	TTelnet Test code starts here
*/

void DriveEngineL()
	{

	//initialization for COMM port 
	CommInitL();
	
	// Setup Active Scheduler
	CActiveScheduler* myActiveScheduler = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(myActiveScheduler);
	CActiveScheduler::Install(myActiveScheduler);

	// Create Tiny Telnet
	CTinyTelnet* aTinyTelnet = CTinyTelnet::NewLC(console);
	//.. and post request to object
	aTinyTelnet->RequestCharacter();

	myActiveScheduler->Start();

	CleanupStack::PopAndDestroy(2); //aTinyTelnet, myActiveScheduler	
	}

CTinyTelnet::CTinyTelnet() : CActiveConsole (EPriorityStandard)
	{
	}

CTinyTelnet::~CTinyTelnet() 
	{
	delete iTelnetSession;
	delete iCommandLine;
	}

CTinyTelnet* CTinyTelnet::NewLC(CConsoleBase* aConsole)
	{
	CTinyTelnet* self = new (ELeave) CTinyTelnet();
    CleanupStack::PushL(self);
	self->ConstructL(aConsole);
	return self;
	}

void CTinyTelnet::ConstructL(CConsoleBase* aConsole)
	{
	CActiveConsole::ConstructL(aConsole);

	iState = EComplete;
	iLocalEcho = FALSE;
	iBinaryOnOff = FALSE;

	iCommandLine=CLineEdit::NewL(iConsole,10);
	// TODO 
	// Modify iTelnetConfig members here for different speed window size etc
//	iConfig.iTermType = _L8("vt100");
	iConfig.iServerEcho = TRUE;
	iTelnetSession = CTelnetSession::NewL(iConfig,this);
	iConsole->Printf(_L("'<ESC>' for command prompt\n"));
	};

void CTinyTelnet::Error(TInt aError)
	{
	iConsole->Printf(_L("Error(%d)\n"),aError);
	}

void CTinyTelnet::ReadComplete(const TDesC8& aBuffer)
	{
	// I know the driver won't chuck more than 128 bytes at us
	// The real code should use variable descriptors on the heap
	TBuf<128> tmpBuffer;
	// For unicode, use the overloaded copy
	tmpBuffer.Copy(aBuffer);
	iConsole->Printf(_L("%S"),&tmpBuffer);
	Read();
	}

void CTinyTelnet::WriteComplete()
	{
	// Request another character
	RequestCharacter();
	return;
	}

void CTinyTelnet::ConnectionClosed()
	{
	iState = EDisconnected;
	iConsole->Printf(_L("ConnectionClosed()\n"));
	}

void CTinyTelnet::OptionsChanged()
	{
	TOptionStatus status;
	if(iState != EConnected)
		{
		iConsole->Printf(_L("No Connection , status not valid\n"));
		}
	else
		iTelnetSession->OptionStatus(status);
	}

void CTinyTelnet::Connected()
	{
	iConsole->Printf(_L("Connected()"));
	Read();
	iState = EConnected;
	}

void CTinyTelnet::DisplayOptionStates()
// It's debatable whether a user would ever want to see this stuff
// It's just here as demo of the call
	{
	if(iState != EConnected)
		{
		iConsole->Printf(_L("No Connection , status not valid\n"));
		}
	else
		{
		TOptionStatus aStatus;
		iTelnetSession->OptionStatus(aStatus);

		iConsole->Printf(_L("Server Send   Binary         Option = %d\n"),aStatus.iServerBinary);
		iConsole->Printf(_L("Client Send   Binary         Option = %d\n"),aStatus.iClientBinary);
		iConsole->Printf(_L("Server Echo   Data           Option = %d\n"),aStatus.iEcho);
		iConsole->Printf(_L("Client Supply Window Size    Option = %d\n"),aStatus.iNAWS);
		iConsole->Printf(_L("Client Supply Terminal Speed Option = %d\n"),aStatus.iTerminalSpeed);
		iConsole->Printf(_L("Client Supply Terminal Type  Option = %d\n"),aStatus.iTerminalType);
		iConsole->Printf(_L("Server Supply Status         Option = %d\n"),aStatus.iServerStatus);
		iConsole->Printf(_L("Client Supply Status         Option = %d\n"),aStatus.iClientStatus);
		}
	}

void CTinyTelnet::ProcessKeyPress(TChar aChar)
	{
	if (aChar != EKeyEscape)
		{
		// Send input Character to echo host
		if(Write(aChar) != KErrNone)
			{
			RequestCharacter();
			}
		else
			{
			if(iLocalEcho)
				iConsole->Printf(_L("%C"), TUint(aChar));
			}
		}
	else
		{
		if(DoCommand())
			RequestCharacter();
		}
	}

TBool CTinyTelnet::DoCommand()
// Handle command strings from the user
// Demo only
	{
	iCommandLine->Edit(_L("\nTinyTelnet>"), &iCommand);

	// Seperate Command from Command Arguments.
	TInt commandEnd;
	TBuf<80> commandArgs;
	TBool ret = TRUE;

	iCommand.TrimAll();  // Remove leading/trailing/double white spaces              
	
	commandEnd = iCommand.Locate(' ');

	if ( (commandEnd != iCommand.Length()) && (commandEnd != KErrNotFound) )
		{
		commandArgs = iCommand.Mid(commandEnd+1);
		iCommand.SetLength(commandEnd);	
		}
	
	if(iCommand.Length()==0) 
		Cancel(); // do nothing
	else if ( iCommand.MatchF(_L("open")) == 0 )
		// Attempt to open a telnet session
		Open(commandArgs);
	else if ( iCommand.MatchF(_L("status")) == 0 )
		{
		// Display the state of the Telnet RFC options
		DisplayOptionStates();
		}
	else if ( iCommand.MatchF(_L("openip")) == 0 )
		// Other open
		OpenIP(commandArgs);
	else if ( iCommand.MatchF(_L("close")) == 0 )
		// Close a connection
		Close(commandArgs);
	else if ( iCommand.MatchF(_L("help")) == 0 )
		Help(commandArgs);
	else if ( iCommand.MatchF(_L("echo")) == 0 )
		{
		// Toggle local echo
		LocalEcho(commandArgs);
		(iConfig.iServerEcho == TRUE) ? (iConfig.iServerEcho = FALSE) : (iConfig.iServerEcho = TRUE);
		iTelnetSession->DoModifyConfig(iConfig);
		}
	else if ( iCommand.MatchF(_L("Logout")) == 0 )
		// Force Telnet level logout to the host ,host could refuse 
		iTelnetSession->DoForceLogout();
	else if ( iCommand.MatchF(_L("AllowLogout")) == 0 )
		// Demo only Toggle allow server to logout option
		{
		(iConfig.iAllowLogout == TRUE) ? (iConfig.iAllowLogout = FALSE) : (iConfig.iAllowLogout = TRUE);
		iTelnetSession->DoModifyConfig(iConfig);
		}
	else if ( iCommand.MatchF(_L("Window")) == 0)
		// Demo only, changes the window size
		{
		iConfig.iWindowSize.x = 60;
		iConfig.iWindowSize.y = 20;
		iTelnetSession->DoModifyConfig(iConfig);
		}
	else if ( iCommand.MatchF(_L("Speed")) == 0)
		// Demo only, sets the speed
		{
		iConfig.iTermSpeed = _L8("9600");
		iTelnetSession->DoModifyConfig(iConfig);
		}
	else if ( iCommand.MatchF(_L("Type")) == 0)
		// Demo only, changes the terminal type
		// See API document implementation of terminal type configuration
		{
		iConfig.iTermType = _L8("vt100");
		iTelnetSession->DoModifyConfig(iConfig);
		}

	else if(iCommand.MatchF(_L("exit")) == 0)
		{
		// Clean exit, all destructors called
		ret = FALSE;
		CActiveScheduler::Stop();
		}
	else
		{
		iConsole->Printf(_L("command = %S.\n"),&iCommand);
		iConsole->Printf(_L("unknown command\n"));
		}
	return ret;
	}

void CTinyTelnet::DoCancel()
	{

	}

void CTinyTelnet::Open(TDesC& aCommandArg)
	{
	TInt err;
	iCommandArg = aCommandArg;
	err = iTelnetSession->Connect(iCommandArg);
	if(err != KErrNone)
		iState = EConnecting;
	iConsole->Printf(_L("Connect returned - %d\n"), err);
	}

void CTinyTelnet::Help(TDesC& /*aCommandArg*/)
	{
	iConsole->Printf(_L("\nHelp\n"));
	iConsole->Printf(_L("----\n\n"));
	iConsole->Printf(_L(" <esc>				   - command prompt\n"));
	iConsole->Printf(_L(" open <server>		   - connect to server on telnet port 23\n"));
	iConsole->Printf(_L(" openip <aa.bb.cc.dd> - connect to server on telnet port 23\n"));
	iConsole->Printf(_L(" close                - disconnect connection\n"));
	iConsole->Printf(_L(" echo                 - toggle local echo on/off\n"));
	iConsole->Printf(_L(" status               - print option status\n\n"));
	}

void CTinyTelnet::LocalEcho(TDesC& /*aCommandArg*/)
	{
	iLocalEcho = (!iLocalEcho);
	}

void CTinyTelnet::Logout(TDesC& /*aCommandArg*/)
	{
	iTelnetSession->DoForceLogout();
	}

void CTinyTelnet::OpenIP(TDesC& aCommandArg)
	{
	TInt err;
	iAddress.Input(aCommandArg);	// Accepts a, a.b, a.b.c, a.b.c.d or 0xaabbccdd
	err = iTelnetSession->Connect(iAddress);
	iConsole->Printf(_L("Connect returned - %d\n"), err);
	}

void CTinyTelnet::Read()
	{
	iTelnetSession->Read();
	}


TInt CTinyTelnet::Write(TChar aChar)
// Write characters to the Server
// For Demo, we are trapping :-
// CTRL+A AO  (Abort Output)
// CTRL+B AYT (Are You There)
// CTRL+C IP  (Interrupt Process)
// CTRL+D EC  (Erase Character, NVT only)
// CTRL+E EL  (Erase Line, NVT only)
// CTRL+F BRK (Break)
	{

	TInt ret = KErrNone;
	TTelnetUserControl code;
	if (iState==EConnected)
		{
		TBuf8<10> buffer;
		buffer.SetLength(0);

		if(aChar == 1)
			{
			// ABORT OUTPUT
			code = EAo;
			ret = iTelnetSession->Write(code);
			}
		else if(aChar == 2)
			{
			// ARE YOU THERE
			code = EAyt;
			ret = iTelnetSession->Write(code);
			}
		else if(aChar == 3)
			{
			// INTERRUPT PROCESS
			code = EIp;
			ret = iTelnetSession->Write(code);
			}
		else if(aChar == 4)
			{
			// ERASE CHARACTER
			code = EEc;
			ret = iTelnetSession->Write(code);
			}
		else if(aChar == 5)
			{
			// ERASE LINE
			code = EEl;
			ret = iTelnetSession->Write(code);
			}
		else if(aChar == 6)
			{
			// BREAK
			code = EBrk;
			ret = iTelnetSession->Write(code);
			}
		else
			{
			buffer.Append(aChar);
			ret = iTelnetSession->Write(buffer);
			}
		
		
		//iConsole->Printf(_L("Write returned - (%d)\n"), err);
		}
	else
		{
		ret = KErrDisconnected;
		iConsole->Printf(_L("Not connected [write failed]\n"));
		}
	return ret;
	}

void CTinyTelnet::Close(TDesC& /*aCommandArg*/)
	{
	TInt err;
	err = iTelnetSession->Disconnect();
	iConsole->Printf(_L("Disconnect returned - %d\n"), err);
	}


//
// CActiveConsole Implementation
//
void CActiveConsole::ConstructL(CConsoleBase* aConsole)
	{
	iConsole = aConsole;
	CActiveScheduler::Add(this);
	}

CActiveConsole::~CActiveConsole()
	{
	Cancel();
	}

void CActiveConsole::RequestCharacter()
	{
	__ASSERT_DEBUG(!IsActive(), User::Panic(_L("CActiveConsole::RequestCharacter()"), KRequestPending));
	iConsole->Read(iStatus);
	SetActive();
	}

void CActiveConsole::RunL()
	{
	ProcessKeyPress(TChar(iConsole->KeyCode()));
	}

void CActiveConsole::DoCancel()
	{
	iConsole->ReadCancel();
	}
