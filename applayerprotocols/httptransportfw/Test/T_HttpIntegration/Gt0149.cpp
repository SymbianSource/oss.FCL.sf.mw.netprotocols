// Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// $Header$
// GT0149 Applications Protocol Integration Test Harness
// blank application
// created by: mark j davey
// mjd systems solutions ltd
// mark@mjdss.com
// This is the 'main' code entry point for the Test Harness Application.
// As other projects adopt policy, then separate DLL's will be created for
// each group which can be specified (or colocated) for inclusion.
// This way the harness is sanitized from the project knowledge domain
// 
//

#include <e32base.h>
#include <e32cons.h>
#include <uri8.h>
#include <bacline.h>

//----------------------------------------------------------------------------
//  application specific includes

#include "GT0149.h"
#include "ECommandIDs.h"
#include "ErrorCodes.h"
#include "TEngine.h"
#include "CCmdFamily.h"
#include "CCmdBase.h"
#include "CCmdStandard.h"
#include "CCmdMisc.h"
#include "CCmdLog.h"
#include "CLogFile.h"
#include "HttpSessionCmds.h"
#include "HttpTransactionCmds.h"

//----------------------------------------------------------------------------

#if defined (HTTP_INTEGRATION)

// for StartC32()
#if !defined __C32COMM_H__
	#include <c32comm.h>
#endif

#if !defined __IN_SOCK_H__
	#include <in_sock.h>
#endif

#if defined (__WINS__)
	#define PDD_NAME		_L("ECDRV")
#else
	#define PDD_NAME		_L("EUART1")
	#define PDD2_NAME		_L("EUART2")
	#define PDD3_NAME		_L("EUART3")
	#define PDD4_NAME		_L("EUART4")
#endif

#define LDD_NAME		_L("ECOMM")

//----------------------------------------------------------------------------
//	Integration Domain specific includes

#include "CHTTPFamily.h"

#endif

//----------------------------------------------------------------------------
//  create this (Console) ONLY if we're NOT being driven from the command line

LOCAL_D CConsoleBase *Console;

//----------------------------------------------------------------------------
//	Global Defines for 

GLDEF_D TBool exitConsoleAtExit = ETrue;
GLDEF_D TBool consoleRequired = EFalse;
GLDEF_D TBool debugRequired = EFalse;
GLDEF_D TInt sysError = KErrNone;
TPtrC ScriptFilename;
TBuf<256> Arguments;
TInt nArgs;

//----------------------------------------------------------------------------
//  method defs

LOCAL_C TInt ExecuteManagerL(void);
LOCAL_C TInt CreateFrameworkCommandsL(CCmdFamily *);
LOCAL_C void TestHarnessMainL(void);
LOCAL_C void pressAnyKey(void);
LOCAL_C void reportError(void);
LOCAL_C void processCommandLine(void);

//----------------------------------------------------------------------------

#if defined (HTTP_INTEGRATION)

//	specialist commands here...
//	ideally this is a method from a dynamically loaded DLL...

LOCAL_C void InitCommsL();
LOCAL_C TInt CreateHTTPIntegrationCommands(CCmdFamily *);


#endif

//----------------------------------------------------------------------------
//	Register the fundamental 'framework' commands

LOCAL_C TInt CreateFrameworkCommandsL(CCmdFamily *aList)
{
aList->RegisterL(CCmdRemark::NewL(THA_KCmdRemark, THA_TxtCmdRemark)); 
aList->RegisterL(CCmdRemark::NewL(THA_KCmdComment, THA_TxtCmdComment)); 

aList->RegisterL(CCmdExit::NewL(THA_KCmdExit, THA_TxtCmdExit )); 
aList->RegisterL(CCmdExit::NewL(THA_KCmdExit, THA_TxtCmdQuit )); 
aList->RegisterL(CCmdExit::NewL(THA_KCmdExit, THA_TxtCmdStop )); 

aList->RegisterL(CCmdEchoMode::NewL( THA_KCmdEchoMode, THA_TxtCmdEchoMode));

aList->RegisterL(CCmdPause::NewL(THA_KCmdPause, THA_TxtCmdPause));

aList->RegisterL(CCmdHold::NewL(THA_KCmdHold, THA_TxtCmdHold));
aList->RegisterL(CCmdLogPath::NewL(THA_KCmdLogPath, THA_TxtCmdLogPath));
aList->RegisterL(CCmdLogFile::NewL(THA_KCmdLogFile, THA_TxtCmdLogFile));
aList->RegisterL(CCmdEndLog::NewL(THA_KCmdEndLog, THA_TxtCmdEndLog));

aList->RegisterL(CCmdSetPrompt::NewL(THA_KCmdSetPrompt, THA_TxtCmdSetPrompt));
aList->RegisterL(CCmdSetPath::NewL(THA_KCmdSetPath, THA_TxtCmdSetPath));

aList->RegisterL(CCmdCall::NewL(THA_KCmdCall, THA_TxtCmdCall, THA_TxtCmdCallHelp));

aList->RegisterL(CCmdReturn::NewL(THA_KCmdReturn, THA_TxtCmdReturn));
aList->RegisterL(CCmdCheckMode::NewL(THA_KCmdCheckMode, THA_TxtCmdCheckMode));
aList->RegisterL(CCmdPrint::NewL(THA_KCmdPrint, THA_TxtCmdPrint, THA_TxtCmdPrintHelp));

aList->RegisterL(CCmdDefine::NewL(THA_KCmdDefine, THA_TxtCmdDefine, THA_TxtCmdDefineHelp));
aList->RegisterL(CCmdListAll::NewL(THA_KCmdListAll, THA_TxtCmdListAll, THA_TxtCmdListAllHelp)); 

//	new command: IF, ELSE, ENDIF
aList->RegisterL(CCmdIf::NewL(THA_KCmdIf, THA_TxtCmdIf, THA_TxtCmdIfHelp)); 
aList->RegisterL(CCmdElse::NewL(THA_KCmdElse, THA_TxtCmdElse, THA_TxtCmdElseHelp)); 
aList->RegisterL(CCmdEndIf::NewL(THA_KCmdEndIf, THA_TxtCmdEndIf, THA_TxtCmdEndIfHelp)); 

// Register CASE and SELECT COMMANDS (TVi)
aList->RegisterL(CCmdCase::NewL(ECase, KTxtCase));
aList->RegisterL(CCmdEndCase::NewL(EEndCase, KTxtEndCase));
aList->RegisterL(CCmdSelect::NewL(ECmdSelect, KTxtCmdSelect));

return KErrNone;
}

//----------------------------------------------------------------------------

#if defined (HTTP_INTEGRATION)

//----------------------------------------------------------------------------
//	provide any framework infrastructure configuration

LOCAL_C void InitCommsL()
{
TInt ret = User::LoadPhysicalDevice(PDD_NAME);
User::LeaveIfError(ret == KErrAlreadyExists ? KErrNone : ret);

#ifndef __WINS__
ret = User::LoadPhysicalDevice(PDD2_NAME);
ret = User::LoadPhysicalDevice(PDD3_NAME);
ret = User::LoadPhysicalDevice(PDD4_NAME);
#endif

ret = User::LoadLogicalDevice(LDD_NAME);
User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);
ret = StartC32();
User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);
}

//----------------------------------------------------------------------------
//	Register the fundamental 'framework' commands

LOCAL_C TInt CreateHTTPIntegrationCommands(CCmdFamily *aList)
{
aList->RegisterL(CCmdConnect::NewL(THA_KHTTP_Connect, THA_TxtCmdConnect, THA_TxtCmdConnectHelp)); 
aList->RegisterL(CCmdDisconnect::NewL(THA_KHTTP_Disconnect, THA_TxtCmdDisconnect, THA_TxtCmdDisconnectHelp)); 
aList->RegisterL(CCmdTransaction::NewL(THA_KHTTP_Transaction, THA_TxtCmdTransaction, THA_TxtCmdTransactionHelp)); 
aList->RegisterL(CCmdEndTransaction::NewL(THA_KHTTP_EndTransaction, THA_TxtCmdEndTrans, THA_TxtCmdEndTransHelp)); 
aList->RegisterL(CCmdShowTransaction::NewL(THA_KHTTP_ShowTransaction, THA_TxtCmdShowTrans, THA_TxtCmdShowTransHelp)); 

return KErrNone;
}

#endif

//----------------------------------------------------------------------------
//	This is where it's at
//	
//	Create the scheduler, the Command Tree, Look for PlugIn Language DLL's etc
//	Offer Console *or* script processing

LOCAL_C TInt ExecuteManagerL(void)
{
TInt StackObjects = 0;

//	Create scheduler etc
CActiveScheduler *Scheduler = new (ELeave) CActiveScheduler;
CleanupStack::PushL(Scheduler);
++StackObjects;

//	no problems, so install it
CActiveScheduler::Install(Scheduler);

if (Console)
	Console->Printf(KTHAWelcomeMessage);

//	create the Test Engine and get it cracking...
CTEngine *Engine = CTEngine::NewLC(Console);
++StackObjects;

//	create command list and register

CCmdFamily *Commands = CCmdFamily::NewLC();
++StackObjects;

if (Engine)
	{

	//	we have a basic 'set' of framework commands that are applicable whatever the project
	//	area. as a result these are 'static'
	CreateFrameworkCommandsL(Commands);

	//	now determine if there are any app specific commands to add to 'Commands'
	//	since this is the 'dynamic' part of the system. All the project commands are
	//	to be supplied in a separate '.dll' which we load and append the commands.
	//	e.g.	we have a set of WAP framework commands
	//	CreateWAPFrameworkCommandsL(Commands);

#if defined (HTTP_INTEGRATION)

	InitCommsL();

	//	ideally do search and load of "dll's"
	CreateHTTPIntegrationCommands(Commands);

#endif

	//	now register the command list to the Test Harness Engine
	Engine->SetCmdFamily(Commands);

	//	Set our own prompt for the console
	if (Console)
		(void) Engine->SetPrompt(THA_CommandPrompt);

	if (nArgs > 1)
		sysError = Engine->Run(ScriptFilename, Arguments);
	else
		{
		sysError = KErrNone;
		Engine->Run();
		}
	}
else
	sysError = KErrUnabletoCreateTestEngine;

//	and start it!
if (sysError != KErrNone)
	//	reset the scheduler to run 'as interactive'
	Engine->Run();

//	kick off the scheduler...
CActiveScheduler::Start();

//  finish Console

	CleanupStack::PopAndDestroy(StackObjects); 

//	unnecessary (since global!)
return sysError;
}
 
//----------------------------------------------------------------------------
//  must have some command lines. 
//  <scriptname[.ths]> must be first parameter
//  -c = Console required once script executed
//  -d = debug mode on (log to Console as well)
//  read the command line
//  If we've got a script provided, then do not create a Console object
//  unless we have a -d (for debug) mode command

LOCAL_C void processCommandLine(void)
{
TInt iIgnore;
CCommandLineArguments *cmdArgs = CCommandLineArguments::NewLC();

nArgs = cmdArgs->Count();
iIgnore = nArgs;
if (nArgs == 1)  //  arg(0) is '.exe' name (arg==1)
	// no args (apart from Exename) means assume Interactive (Console) session required
  consoleRequired = ETrue;

else
	{
	//  get the Command File name - why?
	TPtrC ExeFilename;
	ExeFilename.Set(cmdArgs->Arg(0));

	//  construct call arguments
	for (TInt i = 1; i < nArgs; ++i)
		{
		//  look for command/control flags
		TInt Pos = cmdArgs->Arg(i).Find(KControlFlagSeparator);
		if (Pos != KErrNotFound)
			{
			//	valid argument (i.e. has a - in front) but is it supported?
			//	require a Console? (Console created and displayed once script 
			if (cmdArgs->Arg(i).Find(KControlFlagRequiredScript) != KErrNotFound)
				{
				//	remove the -s from the filename and do not add this to the 
				//	argument list				
				//ScriptFilename.Set(cmdArgs->Arg(i).Mid(2));
				ScriptFilename.Set(cmdArgs->Arg(i).Mid(KControlFlagRequiredScript().Length()));
				sysError = KSysCmdParameter;
				}

			//	require a Console? (Console created and displayed once script 
			else if (cmdArgs->Arg(i).Find(KControlFlagRequireConsole) != KErrNotFound)
				{
				consoleRequired = ETrue;
				sysError = KSysCmdParameter;
				--iIgnore;
				}

			else if (cmdArgs->Arg(i).Find(KControlFlagDebugViaConsole) != KErrNotFound)
				{
				debugRequired = ETrue;
				consoleRequired = ETrue;
				sysError = KSysCmdParameter;
				--iIgnore;
				}

			else
				sysError = KErrCommandFlagNotDefined;
			}

		//	get the args - use spaces to separate and 'fill out'
		Arguments.Append(_L(" "));          //  put a space in between them!
		if (sysError == KAppendParameter)
			Arguments.Append(cmdArgs->Arg(i));  //  copy in the argument

		}
	}

//	update the num args to those we're really interested in
nArgs = iIgnore;

//	clear the error
sysError = KErrNone;

//	no longer need them (args) so delete them!
CleanupStack::PopAndDestroy();
}

//----------------------------------------------------------------------------
//  Application Start Point
//  Integration Test Harness
//
//  The application can be passed a parameter <scriptname> which is autoexecuted

LOCAL_C void TestHarnessMainL(void)
{
TInt StackObjects = 0;

//	see what's cooking on the command line...
processCommandLine();

//	create the Console if required and title it
Console = Console::NewL(_L("GT0149 Integration Test Harness"), TSize(KConsFullScreen, KConsFullScreen));
CleanupStack::PushL(Console);
++StackObjects;

//	execute the bit we want

if (sysError == KErrNone)
	TRAP(sysError, ExecuteManagerL());

//	report any errors
if (sysError != KErrNone)
	{
	reportError();
	if (nArgs != 1)  //  arg(0) is '.exe' name (arg==1)
		pressAnyKey();
	}

//  inform user we're closing...
if (Console)
	{
	Console->Printf(_L("\r\n[press any key]"));
		
	//  get the keypress
	Console->Getch();
	}

//  finish Console
if (StackObjects > 0)
	CleanupStack::PopAndDestroy(StackObjects); //  2=Console & cmdArgs
}

//----------------------------------------------------------------------------
//	simple log error message

LOCAL_C void reportError()
{
if (Console)
  {
	Console->Printf(_L("\r\nThe following error code was returned: %d"), sysError);
	Console->Printf(_L("\r\nPlease report this to the appropriate project team"));
	}
}

//----------------------------------------------------------------------------

LOCAL_C void pressAnyKey()
{
if (Console)
	{
	Console->Printf(_L("\r\n[Press any key]"));
	Console->Getch();
	Console->Printf(_L("\n"));
	}
}

//----------------------------------------------------------------------------

GLDEF_C TInt E32Main()
// Main program - run the Test Harness within a TRAP harness, reporting any errors that
// occur via the panic mechanism. Test for memory leaks using heap marking.
{
__UHEAP_MARK;
CTrapCleanup* tc = CTrapCleanup::New();

TRAPD(err, TestHarnessMainL());
if (err!=KErrNone)
	User::Panic(_L("\r\nIntegration Test Harness failed with error code: %i"), err);

  delete tc;

__UHEAP_MARKEND;
return KErrNone;
}

//----------------------------------------------------------------------------


