// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The CTEngine is the class that manages the test infrastructure
// It knows how to process scripts and where to find command knowledge
// The plan is:
// Process console command line or command line supplied script
// read in each line (or command) and process that (if appropriate)
// Include Files  
// 
//

#include <e32std.h>
#include <s32file.h>
#include <f32file.h>
#include <e32hal.h>
#include <fbs.h>

//-----------------------------------------------------------------------------

const TInt KMaxLogEntrySize = 512;
const TInt KMaxUserEntrySize = 128;

//-----------------------------------------------------------------------------

//	result define names
_LIT(KLastResultCode, "$LastResult$");
_LIT(KLastErrorCode, "$LastError$");

//-----------------------------------------------------------------------------

#include "TEngine.h"

//-----------------------------------------------------------------------------
// This is used by the CallCmdFile, CloseCmdFile and CloseCmdFiles member
// functions to alter the command file context. This function defines the
// call argument tags %0%, %1%, ... %k% to those of the present cmd file.
// If there is no command file, then the argument tags (all) are removed.

#if !defined(TEMPLATED_CATALOGUE)

LOCAL_C void SetContextL(CCatalogue *aCatalog, CCmdFile* aCmdFile )

#else

template <class T>
LOCAL_C void SetContextL(CCatalogue<T> *aCatalog, CCmdFile* aCmdFile )

#endif

{
if ( aCmdFile == NULL )
	{
	// No command file = console context: parameter markers are not
	// recognised at all i.e. remove them all.
	TBuf<6> buf;
	TInt i;
	for ( i = 0; i <= TFR_MAX_CALL_ARGUMENTS; ++i )
		{
		buf.Format( _L("%%%d%%"), i );
		aCatalog->Delete(buf);
		}
	}
else
	{
	// Command file context: parameters markers will be mapped to the
	// file name, to the call arguments and to "".
	TBuf<6> buf;
	TPtrC   arg;
	TInt    i;
	TInt		iArgs = aCmdFile->Argc();

	for ( i = 0; ( (i < TFR_MAX_CALL_ARGUMENTS) && (i < iArgs) ); ++i )
		{
		buf.Format(_L("%%%d%%"), i);
		arg.Set(aCmdFile->Argv(i));
		aCatalog->Delete(buf);
		aCatalog->AddL(buf,arg);
		}

	for ( ; i < 10; ++i )
		{
		buf.Format(_L("%%%d%%"), i);
		aCatalog->Delete(buf);
		aCatalog->AddL(buf, _L(""));
		}
	}

}

//-----------------------------------------------------------------------------
//    Overflow handler.

void TTestMachineOverflow::Overflow( TDes16& aDes)
{
_LIT(KErrOverflowMsg, "...");
if( aDes.MaxLength() >= KErrOverflowMsg().Length() + aDes.Length() )
	aDes.Append(KErrOverflowMsg);
}

//-----------------------------------------------------------------------------
//	NewL for Console interface

CTEngine* CTEngine::NewL( CConsoleBase* aConsole )
{
CTEngine* self = NewLC(aConsole);
CleanupStack::Pop(self);
return self; 
}

//-----------------------------------------------------------------------------
//	NewLC for Console Interface

CTEngine* CTEngine::NewLC( CConsoleBase* aConsole )
{
CTEngine* self = new (ELeave) CTEngine(aConsole);
CleanupStack::PushL(self);
self->ConstructL();
return self;
}

//-----------------------------------------------------------------------------
//	Destructor

CTEngine::~CTEngine()
{
Cancel();

	iTimer.Close();
	iFileServer.Close();
	delete iCmdFile;
	delete iLogFile;

	delete iConsoleReader;
	delete iDomain;
	delete iDefines;
	delete iIFPile;
	delete iLogPile;
 	delete iCmdPile;
}

//-----------------------------------------------------------------------------

void CTEngine::ConstructL()
{

iConsoleEnabled = ( iConsole != NULL );    

iCmdPile = CPile<CCmdFile>::NewL();
iLogPile = CPile<CLogFile>::NewL();
iIFPile = CPile<CIFControl>::NewL();


#if !defined(TEMPLATED_CATALOGUE)
iDefines = CCatalogue::NewL();
#else
iDefines = CCatalogue<CLabeledText>::NewL();
#endif


//	domain specific catalogue of items. This provides
//	a mechanism by which we can disassociate the framework
//	from the domain specific commands/data requirements
iDomain = CObjCatalogue::NewL();

iConsoleReader = CConsoleReader::NewL(*iConsole);
TInt r = iTimer.CreateLocal();

if (r!=KErrNone)
	User::Leave(r);

//	Provide access to the fileserver
User::LeaveIfError(iFileServer.Connect());

//	This is normally true (to enable replication of printed
//	output to log (if enabled)
iLogActivity = ETrue;

//	show the unprocessed command lines on screen and log
SetShowCmdLine(ETrue);
}

//-----------------------------------------------------------------------------

CTEngine::CTEngine(CConsoleBase* aConsole)
	: 
	CActive(EPriorityStandard), 
	iStopped(ETrue), 
	iEchoMode(ETrue),
	iConsole(aConsole)
{
iConsoleEnabled = ( iConsole != NULL );    

iCmdPath.Zero();
iLogPath.Zero();
iCmdline.Zero();
iCommand.Zero();
iPrompt.Copy(THA_CommandPrompt);

CActiveScheduler::Add(this);
}

//-----------------------------------------------------------------------------

void CTEngine::SetCmdFamily(CCmdFamily* aCmdFamily)
{
iCmdFamily = aCmdFamily;
if (aCmdFamily != NULL) 
	aCmdFamily->SetMachine( this );
}

//-----------------------------------------------------------------------------

TBool CTEngine::ConsoleEnabled( )
{
return ( iConsoleEnabled );
}

//-----------------------------------------------------------------------------

void CTEngine::DisableConsole()
{
iConsoleEnabled = EFalse;
}

//-----------------------------------------------------------------------------

void CTEngine::EnableConsole()
{
iConsoleEnabled = (iConsole != NULL);
}

//-----------------------------------------------------------------------------

CConsoleBase* CTEngine::Console() const
{
return (iConsole);
}

//-----------------------------------------------------------------------------

void CTEngine::Write(const TDesC& aText)
{    
if (ConsoleEnabled()) 
	iConsole->Write(aText);
}

//-----------------------------------------------------------------------------

void CTEngine::Writeln( const TDesC& aText )
{
if (ConsoleEnabled())
	{
	iConsole->Write(aText);
	iConsole->Write(TFR_KTxtNewLine);
	}
}

//-----------------------------------------------------------------------------

void CTEngine::Writeln()
{    
if (ConsoleEnabled()) 
	iConsole->Write(TFR_KTxtNewLine);
}

//-----------------------------------------------------------------------------

void CTEngine::Printf( TRefByValue<const TDesC> aFmt, ... )
{    
VA_LIST list;
VA_START( list, aFmt );
Printf( aFmt, list ); 
}

//-----------------------------------------------------------------------------

void CTEngine::Printf( TRefByValue<const TDesC> aFmt, VA_LIST& aList )
{
if ( ConsoleEnabled() )
	{
	iPrintBuf.Zero();
	TRAPD(error,iPrintBuf.AppendFormatList( aFmt, aList, &iOverflow));
	if ( error == KErrNone )
		{
		iConsole->Write( iPrintBuf );
		}
	else
		{
		iConsole->Write( TFR_KTxtErrPrintOflo );
		iConsole->Write( TFR_KTxtNewLine );
		}
	}
}

//-----------------------------------------------------------------------------

void CTEngine::Printfln( TRefByValue<const TDesC> aFmt, ... )
{    
VA_LIST list;
VA_START( list, aFmt );
Printfln( aFmt, list );
}

//-----------------------------------------------------------------------------

void CTEngine::Printfln( TRefByValue<const TDesC> aFmt, VA_LIST& aList )
{
if ( ConsoleEnabled( ) )
	{
	iPrintBuf.Zero();
	TInt error = KErrNone;
	TRAP(error,iPrintBuf.AppendFormatList( aFmt, aList, &iOverflow));
	if ( error != KErrNone ) 
		return;
	TRAP(error,iPrintBuf.AppendFormat( _L("%S"), &iOverflow, &TFR_KTxtNewLine ));
	if (error == KErrNone )
		{
		iConsole->Write( iPrintBuf );
		}
	else
		{
		iConsole->Write( TFR_KTxtErrPrintOflo );
		iConsole->Write( TFR_KTxtNewLine );
		}
	}
}

//-----------------------------------------------------------------------------

void CTEngine::Pause(const TDesC& aPrompt)
{
if (ConsoleEnabled())
	{
	iCmdline.Zero();
	iConsole->Write(aPrompt);
	if (!IsActive())
		SetActive();
	iConsoleReader->ReadChar(iCmdline, iStatus);
	}
else
	CompleteRequest();
}

//-----------------------------------------------------------------------------

void CTEngine::Hold(TTimeIntervalMicroSeconds32 aInterval)
{
iTimer.After(iStatus, aInterval);
SetActive();
}

//-----------------------------------------------------------------------------

TInt CTEngine::SetCmdPath( const TDesC& aPath )
{
if (aPath.Length() == 0)
	{
	Printf(_L("Path is set to: %S\r\n"), &iCmdPath);
	return KErrNone;
	}

if (iParse.SetNoWild( aPath, NULL, NULL ) != KErrNone)
	return KErrArgument;

if (!iParse.PathPresent() || iParse.NameOrExtPresent())
	return KErrArgument;

//	set it
iCmdPath.Copy(aPath);

//	echo it
Printf(_L("Path is set to: %S\r\n"), &iCmdPath);

return KErrNone;
}

//-----------------------------------------------------------------------------

TPtrC CTEngine::CmdPath() const
{
TPtrC path(iCmdPath);
return path;
}

//-----------------------------------------------------------------------------

TPtrC CTEngine::FileName() const
{
TPtrC fileName(iFileName);
return fileName;
}

//-----------------------------------------------------------------------------
//	parameterless version

TInt CTEngine::CallCmdFile(const TDesC& aFile)
{
return CallCmdFile(aFile, _L(""));
}

//-----------------------------------------------------------------------------

TInt CTEngine::CallCmdFile(const TDesC &aFile, const TDesC &aArgs)
{
// Make CCmdFile object.
CCmdFile *file = NULL;
TRAPD(error, file = CCmdFile::NewL());
if ( error != KErrNone ) 
	return error;

error = file->Open(iCmdPath, aFile);
if ( error != KErrNone )
	{
	delete file;

	//	print file x not found echo it
	Printf(_L("Error: file [%S] not found\r\n"), &aFile);
	Printf(_L("       Check file path <%S>\r\n"), &iCmdPath);
	return error;
	}

// Save file name and arguments.
TRAP(error, file->AddArgsL(aFile));
if (error == KErrNone) 
	TRAP(error, file->AddArgsL(aArgs));

if ( error != KErrNone )
	{
	delete file;
	return error;
	}

// Check that not too many call arguments.
if ( file->Argc() > (TFR_MAX_CALL_ARGUMENTS + 1))
	{
	delete file;
	return error = KErrArgument;
	}

// Push present command file to cmdfile pile.
TRAP(error, iCmdPile->PushL( iCmdFile ));
if ( error != KErrNone )
	{
	delete file;
	return error;
	}

// Switch context to the called command file.
iCmdFile = file;
file = NULL;
TRAP(error, SetContextL(iDefines, iCmdFile));
return error;
}

//-----------------------------------------------------------------------------

CCmdFile* CTEngine::CmdFile()
{
return iCmdFile;
}

//-----------------------------------------------------------------------------

TInt CTEngine::CloseCmdFile()
{
// Get rid of the present command file (NULL iCmdFile is Ok).
delete iCmdFile;
iCmdFile = NULL;

// Pop the previous one from the pile (returns NULL if none).
iCmdFile = iCmdPile->Pop();

// Switch context to that file (NULL iCmdFile does no harm).
TRAPD(error,SetContextL(iDefines,iCmdFile));
return error;
}

//-----------------------------------------------------------------------------

void CTEngine::CloseCmdFiles()
{
// Pop and delete all previous command files from the file pile.
iCmdPile->Empty();

// Get rid of the present file as well (NULL iCmdFile is Ok).
delete iCmdFile;
iCmdFile = NULL;

// Switch context to "no command file" (since iCmdFile = NULL).
TRAP_IGNORE(SetContextL(iDefines,iCmdFile)); // doesn't LEAVE, see yourself
}

//-----------------------------------------------------------------------------

TInt CTEngine::SetLogPath( const TDesC& aPath )
{
if (aPath.Length() == 0)
	{
	iLogPath.Zero();
	return KErrNone;
	}

if (iParse.SetNoWild( aPath, NULL, NULL ) != KErrNone)
	return KErrArgument;

if (!iParse.PathPresent() || iParse.NameOrExtPresent())
	return KErrArgument;

TInt error;
RFs fs;
if (error = fs.Connect(), error == KErrNone)
	{
	error = fs.MkDirAll( iParse.DriveAndPath());
	if (error == KErrAlreadyExists) 
		error = KErrNone;

	fs.Close();
	if (error == KErrNone) 
		iLogPath.Copy(aPath);
	}
return error;
}

//-----------------------------------------------------------------------------

TPtrC CTEngine::LogPath() const
{
TPtrC logpath(iLogPath);
return logpath;
}

//-----------------------------------------------------------------------------

TInt CTEngine::OpenLogFile(const TDesC& aFile)
{
// Construct a full log file.
TInt error = iParse.Set(aFile, NULL, &iLogPath);
if (error != KErrNone) 
	return error;

// Construct and open the new log file.
CLogFile* file = NULL;
TRAP(error, file = CLogFile::NewL());
if ( error != KErrNone ) 
	return error;

error = file->Open( iParse.FullName() );
if ( error == KErrNone && iLogFile != NULL )
	{
	// Close and save the present log file.
	iLogFile->Close();
	TRAP(error, iLogPile->PushL(iLogFile));
	if ( error == KErrNone ) 
		iLogFile = NULL;
	}

if (error == KErrNone)
	{
	// Switch the log file.
	file->PrintHeading();
	iLogFile = file;
	file = NULL;
	}

delete file; // is NULL if no errors!
return error;
}

//-----------------------------------------------------------------------------

CLogFile *CTEngine::LogFile()
{
return iLogFile;
}

//-----------------------------------------------------------------------------

TInt CTEngine::CloseLogFile( )
{
delete iLogFile;
iLogFile = NULL;
iLogFile = iLogPile->Pop();
TInt error = KErrNone;
if ( iLogFile != NULL )
	error = iLogFile->Open();
return error;
}

//-----------------------------------------------------------------------------

void CTEngine::CloseLogFiles()
{
TInt count = iLogPile->Count();
while (count > 0 )
	{
	CLogFile* file = iLogPile->Pop();
	delete file; // NULL does no harm!
	}

delete iLogFile;
iLogFile = NULL;
}

//-----------------------------------------------------------------------------

void CTEngine::LogWrite( const TDesC& aText )
{
if ( iLogFile != NULL ) 
	iLogFile->Write( aText );
}

//-----------------------------------------------------------------------------

void CTEngine::LogWriteln( const TDesC& aText )
{
if ( iLogFile != NULL ) 
	iLogFile->Writeln( aText );
}

//-----------------------------------------------------------------------------

void CTEngine::LogWriteln()
{
if ( iLogFile != NULL ) 
	iLogFile->Writeln();
}

//-----------------------------------------------------------------------------

void CTEngine::LogPrintf( TRefByValue<const TDesC> aFmt, ... )
{
VA_LIST list;
VA_START(list, aFmt);
LogPrintf(aFmt, list);
}

//-----------------------------------------------------------------------------

void CTEngine::LogPrintf( TRefByValue<const TDesC> aFmt, VA_LIST& aList )
{
if (iLogFile != NULL) 
	iLogFile->Printf(aFmt, aList);
}

//-----------------------------------------------------------------------------

void CTEngine::LogPrintfln( TRefByValue<const TDesC> aFmt, ... )
{
VA_LIST list;
VA_START(list, aFmt);
LogPrintfln(aFmt, list);
}

//-----------------------------------------------------------------------------

void CTEngine::LogPrintfln( TRefByValue<const TDesC> aFmt, VA_LIST& aList )
{
if (iLogFile != NULL)
	{
	iLogFile->Printf(aFmt, aList);
	iLogFile->Writeln();
	}
}

//-----------------------------------------------------------------------------
//    Standard message output:
//    - CTEngine::MsgWrite
//    - CTEngine::MsgWriteln
//    - CTEngine::MsgWriteln
//    - CTEngine::MsgPrintf
//    - CTEngine::MsgPrintf
//    - CTEngine::MsgPrintfln
//    - CTEngine::MsgPrintfln
//
//	A message goes to the log if there is one open. The message is printed
//	to the display as well, except when having echo mode off and running a
//	command file.
//
//-----------------------------------------------------------------------------

void CTEngine::MsgWrite(const TDesC& aText)
{
LogWrite(aText);
if (EchoMode() || (iCmdFile == NULL)) 
	Write(aText);
}

//-----------------------------------------------------------------------------

void CTEngine::MsgWriteln(const TDesC& aText)
{
LogWriteln(aText);
if (EchoMode() || (iCmdFile == NULL)) 
	Writeln(aText);
}

//-----------------------------------------------------------------------------

void CTEngine::MsgWriteln()
{
LogWriteln();
if (EchoMode() || (iCmdFile == NULL)) 
	Writeln();
}

//-----------------------------------------------------------------------------

void CTEngine::MsgPrintf( TRefByValue<const TDesC> aFmt, ... )
{
VA_LIST list;
VA_START( list, aFmt  );
MsgPrintf( aFmt, list );
}

//-----------------------------------------------------------------------------

void CTEngine::MsgPrintf( TRefByValue<const TDesC> aFmt, VA_LIST& aList )
{
if ( EchoMode() || (iCmdFile == NULL) ) 
	{
	VA_LIST clone;
	Mem::Move( &clone, &aList, sizeof(VA_LIST) );
	LogPrintf( aFmt, aList );
	Printf( aFmt, clone );
	}
else
	{
	LogPrintf( aFmt, aList );
	}
}

//-----------------------------------------------------------------------------

void CTEngine::MsgPrintfln( TRefByValue<const TDesC> aFmt, ... )
{
VA_LIST list;
VA_START( list, aFmt );
MsgPrintfln( aFmt, list );
}

//-----------------------------------------------------------------------------

void CTEngine::MsgPrintfln( TRefByValue<const TDesC> aFmt, VA_LIST& aList )
{
if ( EchoMode() || (iCmdFile == NULL) ) 
	{
	VA_LIST clone;
	Mem::Move( &clone, &aList, sizeof(VA_LIST) );
	LogPrintfln( aFmt, aList );
	Printfln( aFmt, clone );
	}
else
	LogPrintfln(aFmt, aList);
}

//-----------------------------------------------------------------------------

TInt CTEngine::SetPrompt(const TDesC& aPrompt)
{
// Return error in case a too long prompt was proposed.
if (aPrompt.Length() > iPrompt.MaxLength()) 
	return KErrTooBig;

// Otherwise alter trhe prompt and return Ok.
iPrompt.Copy(aPrompt);
return KErrNone;
}

//-----------------------------------------------------------------------------

TPtrC CTEngine::Prompt() const
{
TPtrC prompt(iPrompt);
return prompt;
}

//-----------------------------------------------------------------------------
//	Run
//	Start things off! We have a variety of options: 
//	1.	interactive. (no params) => Call Run()
//	2.	semi-interactive. (some params, no script) => Call Run(Args)
//	3.  automatic. (some params & script) => Run(Script, Args)

void CTEngine::Run()
{
if (!IsActive())
	{
	// Start the engine and set the state to get the next cmd
	iStopped   = EFalse;
	iState = EGetNextCmd;

	// Complete request
	CompleteRequest();
	}
}

//-----------------------------------------------------------------------------

TInt CTEngine::Run(const TDesC& aFile)
{
return Run(aFile, _L(""));
}

//-----------------------------------------------------------------------------

TInt CTEngine::Run(const TDesC& aFile, const TDesC& aArgs)
{
iStopped = EFalse;

// Open (call) the command file. Stop test engine if fails.
TInt error = CallCmdFile(aFile, aArgs);
if (error != KErrNone) 
	StopEngine();

// Keep going until stopped or reading the commands fails.
while ((!iStopped) && (error == KErrNone))
	{
	error = ReadCmdline(EFalse);
	if (error == KErrNone)
		{
		if (MakeCommand() == KErrNone)
			{
			TRAP(error,(void) ObeyCommandL()); 
			}
		}
	}

// Make sure that command files and log files get closed and return.
CloseCmdFiles();
CloseLogFiles();
return error;
}

//-----------------------------------------------------------------------------
//	CTEngine::Terminate
//
//	Terminate processing of command files. If not inside a command file,
//	then does not do anything - doesn't even close the opened log files.

void  CTEngine::Terminate()
{
// Inside command file?
if (iCmdFile != NULL)
	{

	// Close all command files.
	CloseCmdFiles();

	// Close all log files. Write the "run aborted" message to the end
	// of every log file.
	while (iLogPile->Count() > 0)
		{
		// There is yet at least one log file in pile: write the abort
		// message and close => previous file in pile will be opened.
		LogWriteln( TFR_KTxtMsgRunAborted );
		(void)CloseLogFile();
		}

	// Close the very last log file too. Having iLogFile = NULL is not
	// checkd in here because that doesn't do any harm.
	LogWriteln( TFR_KTxtMsgRunAborted );
	delete iLogFile;
	iLogFile = NULL;

	// Print abort message to console too.
	Writeln( TFR_KTxtMsgRunAborted );
	}
}

//-----------------------------------------------------------------------------

void CTEngine::StopEngine( )
{
iStopped = ETrue;
CloseCmdFiles();

// Print message to console
Writeln(TFR_KTxtMsgRunStopped);
}

//-----------------------------------------------------------------------------

TBool CTEngine::Stopped()
{
return iStopped;
}

//-----------------------------------------------------------------------------

void CTEngine::SetEchoMode(const TBool &aBoolean)
{
iEchoMode = aBoolean;
}

//-----------------------------------------------------------------------------

TBool CTEngine::EchoMode()
{
return iEchoMode;
}

//-----------------------------------------------------------------------------

#if !defined(TEMPLATED_CATALOGUE)
CCatalogue *CTEngine::Defines()
#else
CCatalogue<CLabeledText> *CTEngine::Defines()
#endif
{
return iDefines;
}

//-----------------------------------------------------------------------------

TInt CTEngine::ReadCmdline(TBool aConsole)
{

// Reset cmd.
iCmdline.Zero();

TInt error = KErrNone;
if (iCmdFile != NULL)
	error = ReadCmdFile();

else
	// Set error to imply that the end of the cmd file has been reached
	error = KErrEof;

if ((error == KErrEof) && aConsole )
	// Get the next cmd line from the console
	error = ReadConsole();

return error;
}

//-----------------------------------------------------------------------------
//	Read command line (iCmdline) from command files.

TInt CTEngine::ReadCmdFile()
{

// Reset cmd and read from the command files. Close files if at end.
iCmdline.Zero();
TInt error = KErrEof;
while ((iCmdFile != NULL) && (error == KErrEof))
	{
	error = iCmdFile->Read(iCmdline);
	if (error == KErrEof)
		{
		error = CloseCmdFile();
		if (error == KErrNone) 
			error = KErrEof;
		}
	else
		CompleteRequest();

	}

return error;
}

//-----------------------------------------------------------------------------
//	Read command line (iCmdline) from console.

TInt CTEngine::ReadConsole()
{
// Get cmd from console - first write prompt.
iConsole->Write(iPrompt);
iConsoleReader->ReadLine(iCmdline, iStatus);
SetActive();

return KErrNone;
}

//-----------------------------------------------------------------------------

TPtrC CTEngine::GetDefine(const TDesC &aTag) const
{
TPtrC iVal;
TInt len = aTag.Length();

for (TInt i = 0; i < iDefines->Count(); ++i)
	{
	CLabeledText *tag = iDefines->At(i);
	TInt pos = tag->Label().Find(aTag);
	//make sure that 'xyz' does not match with '$xyz_abcd$' 
	if ( (pos == 1) && (tag->Label().Length() == len+2) )
		{
		iVal.Set(tag->Value());
		break;
		}
	}

return iVal;
}

//-----------------------------------------------------------------------------
//	Make the command (iCommand) to obey. Copies the command line (iCmdline)
//	to the iCommand buffer and expands the tags to their defined values. If
//	fails, prints out the original command line and an error message.

TInt CTEngine::MakeCommand()
{
TInt error = KErrNone;

// Copy command line from iCmdline buffer to iCommand buffer.
iCommand.Copy(iCmdline);

// Expand tags = replace tag markers with their defined values.
TInt  counter = 0;
TBool proceed = ETrue;
while (proceed && (error == KErrNone))
	{
	// One round = replace all tag labels with their values and
	// alter the proceed to ETrue in replaced one or more tags.
	proceed = EFalse;
	for (TInt i = 0; i < iDefines->Count(); ++i)
		{
		CLabeledText *tag = iDefines->At(i);
		TInt pos = iCommand.Find(tag->Label());
		if (pos >= 0)
			{

			// More than a hundred replacements as the whole is
			// propably due to an infinite loop ( recursive tag
			// definitions ): 
			if (counter > 99)
				{
				// Propably recursive tag definitions.
				MsgWriteln(TfrLex::Trim(iCmdline));
				MsgWriteln(TFR_KTxtErrRecursiveTags);
				error = KErrOverflow;
				break;
				}

			// Check that this very tag replacement won't cause
			// overflow in command buffer.
			TInt newlength = iCommand.Length() - tag->Label().Length();
			newlength += tag->Value().Length();
			if (newlength > iCommand.MaxLength())
				{
				// Command has grown too big in size.
				MsgWriteln(TfrLex::Trim( iCmdline));
				MsgWriteln(TFR_KTxtErrTooLongCmd);
				error = KErrOverflow;
				break;
				}

			// Fine, replace the tag with its defined value and
			// mark that there shall be at least one more round.
			proceed = ETrue;
			iCommand.Replace(pos, tag->Label().Length(), tag->Value());
			++counter; // keep up total count of replacements.
			}
		} // end-for
	} // end-while

// Return if everything went fine.
if ( error == KErrNone ) 
	return error;

// Otherwise terminate command files, unless when in check mode.
if ( (iCmdFamily == NULL) || !iCmdFamily->Switch(CCmdFamily::EParseOnly) )
	Terminate();

return error;
}

//-----------------------------------------------------------------------------

TInt CTEngine::ObeyCommandL()
{
TInt error = KErrNone;

// The command is handled without the leading/trailing spaces.
TPtrC cmd = TfrLex::Trim(iCommand);

// The command shall be echoed if echo mode is On and command
// was read from a command file. 
TBool doEcho = (iEchoMode && (iCmdFile != NULL));

// Check if command begins with @ character and remove if yes.
// That kind of commands aren't echoed afterall.
if (cmd.Match(_L("@*")) == 0)
	{
	// Command is not echoed afterall.
	doEcho = EFalse;

	// Remove @ and them trim once more.
	cmd.Set(cmd.Ptr()+1, cmd.Length()-1);
	cmd.Set(TfrLex::TrimLeft(cmd));
	}

//	Look up from Command Family if there's a registered command to
//	handle this very command string. If there is no Command Family
//	then there is no handler either.
//	bear in mind that we have to possibly skip a valid command if
//	our if mode directs us... 
CCmdBase *handler = NULL; 
if (iCmdFamily != NULL) 
	handler = iCmdFamily->Recognize(cmd);

if (handler != NULL)
	{
	TBool stepover = DetermineStepover(handler);

	if (!stepover)
		{
		// This one shall be processed.
		// ----------------------------
		// Echo the original input command to the display.

		//	log command and display the command on the screen if appropriate
		if (iShowCmdLine)
			MsgWriteln(iCmdline);	
		else if (doEcho) 
			Writeln(TfrLex::TrimRight(iCommand));

		//	Write into the log unless that has been prevented.
		//	and then process the command.
		error = handler->ProcessL(cmd);
		}
	else
		// This one shall not be processed.
		// --------------------------------
		// Command it is not written to log nor echoed either.
		{
		error = handler->ProcessStepOverL();
		}

	}
else
	{
	// There was no handler for this command i.e. it's an unknown
	// command and we don't known what to do with it. However, if
	// the processing is in step over mode odds commands are here
	// silently ignored (not echoed either).

	if (iCmdFamily->StepOver())
		// Step over mode: ignore odd command silently.
		error = KErrNone;

	else
		{
		// Otherwise, echo the original input command to display
		// and log an error message.
		if (doEcho) 
			Writeln(TfrLex::TrimRight(iCommand));
		LogWriteln(cmd);
		MsgWriteln(TFR_KTxtErrUnknownCmd);
		error = KErrNotFound;
		}
	}

//	if the last command was a 'domain' command store the result (deano requirement)
//	unless the last command was IF!
if (handler == NULL || (handler->CommandId() < THA_KCmdMaxStandard))
	;//	do nothing
else
	SetErrorDefine(error);

//	As we are recording the value of the return, it is no longer necessary 
//	to abort here - let the 'script' worry about subsequent action if required

// Return if successful.
//if (error == KErrNone) 
//	return error;
// Error/failure: terminate command files, unless when check mode.
//if ((iCmdFamily == NULL) || !iCmdFamily->Switch(CCmdFamily::EParseOnly))
  //Terminate();

return error;
}


//-----------------------------------------------------------------------------
//	Assign the passed value to the last result define

TBool CTEngine::DetermineStepover(CCmdBase *aHandler)
{
//	Handler was found: check if the command is to be processed
//	and if the command is among those to write into the log.
//	dont forget to check the if, else, endif situation
//	first - incase the if mode needs changing
TBool iSOver = aHandler->StepOver();

//	we must be aware of the problem of nested loops
//	we must 'count' and 'match' the intervening if/else/endif's
//	until we get back to our current level
//
//	to do this, we need to know what command is being executed
//	and use that and the current state to work out where we are.
//	At this point we have not set the ifstate so we can predetermine
//	the next move.
//
//	NOTE: If we encounter if's we must process them (if they are part
//				of the correct branch!

CIFControl::TIFMode eIFMode = GetIfMode();
CIFControl::TIFProcessing eIFState = GetIfState();

//	iBranch merely tells us if the current clause return true/false
//	not which branch is being executed (thats from eIFState)
TBool iBranch = (iIFPile->Peek()) ? (iIFPile->Peek())->GetIFResult() : EFalse;
TInt iCmd = aHandler->CommandId();
TBool iInSubClause = EFalse;
switch (iCmd)
	{
	case THA_KCmdIf : 
		
		//	we've encountered an IF statement, which should be processed ONLY
		//	if we're in the correct branch of a current IF
		//	if the if is FALSE we still need to count the internal IFs/ELSE/ENDIFs as normal
		switch (eIFState)
			{
			//	we are in the middle of an if,else,endif block note: the logic here is tortuous!
			//	iIfState is true if the IF was true
			case CIFControl::EInIF : 
				
				//	process the IF if we're in the correct (true) clause this will start 
				//	another IF object (unless it's false)
				iSOver = (eIFMode == CIFControl::EELSETrue);
				if (iSOver)
					//	we now have to count the else's and endifs as we're not processing the IF!
					{
					if (iIFPile->Peek())
						(iIFPile->Peek())->If();
					}
				break;

			case CIFControl::EInELSE : 

				iSOver = (eIFMode == CIFControl::EIFTrue); 
				if (iSOver)
					{
					if (iIFPile->Peek())
						(iIFPile->Peek())->Else();
					}
				break;
			
			//	must be a 'virgin' IF so ensure we call it!
			default : iSOver = EFalse; break;
			}			
		break;	

	case THA_KCmdElse :

		//	we're already in an IF or ELSE so merely continue as 'normal'
		switch (eIFState)
			{
			//	We should execute the ELSE as this sets up the state var but not the subsequent
			//	commands as necessary. We are in the middle of an if,else,endif block 
			//	note: the logic here is tortuous! iIfState is true if the IF was true (i.e.
			case CIFControl::EInIF : 
				if (iIFPile->Peek())
					//	we need to ensure we're accounting for outer IF's etc
					//	we need to ensure we process it correctly
					iInSubClause = ((iIFPile->Peek())->GetIfCount() > 0) ? ETrue : EFalse;

				if (iInSubClause)
					iSOver = iInSubClause;
				else
					//	if the subclause is subject to fail then we should ignore this completely
					//iSOver = (eIFMode == CIFControl::EELSETrue) ? ETrue : EFalse;
					//	if the subclause is subject to fail then we should ignore this completely
					//	unless we are supposed to execute it...
					if (iBranch)
						iSOver = (eIFMode == CIFControl::EELSETrue) ? ETrue : EFalse;
					else	
						iSOver = (eIFMode == CIFControl::EELSETrue) ? EFalse : ETrue;

				if (iSOver)
					{
					if (iIFPile->Peek())
						(iIFPile->Peek())->Else();
					}
				break;

			case CIFControl::EInELSE : 
				if (iIFPile->Peek())
					//	we need to ensure we're accounting for outer IF's etc
					//	we need to ensure we process it correctly
					iInSubClause = ((iIFPile->Peek())->GetIfCount() > 0) ? ETrue : EFalse;

				if (iInSubClause)
					iSOver = iInSubClause;
				else
					//	if the subclause is subject to fail then we should ignore this completely
					//	unless we are supposed to execute it...
					if (iBranch)
						iSOver = (eIFMode == CIFControl::EELSETrue) ? ETrue : EFalse;
					else	
						iSOver = (eIFMode == CIFControl::EELSETrue) ? EFalse : ETrue;

				if (iSOver)
					{
					if (iIFPile->Peek())
						(iIFPile->Peek())->Else();
					}
				break;

			default : 
				//	accept current iSOver state but if we're in the non-executing
				//	branch of current clause, increment the else count and ensure we 
				//	skip the command
				if (!iBranch)
					{
					if (iIFPile->Peek())
						(iIFPile->Peek())->Else();
					iSOver = true;
					}
				break;
			}
		break;

	//	ok, here's where the real 'meat' of the situation
	case THA_KCmdEndIf : 
		{
		
		//	we MUST be in an IF sequence, but we also need to be aware
		//	of the state of any outer IF clause 
		if (iIFPile->Peek())
			{
			//	before we decrement our counters, we need to see the state of play
			//	if we're in an IF which is part of an untrue IF clause *see if test9/10*
			//	we need to ensure we process it correctly
			TInt iIfs = (iIFPile->Peek())->GetIfCount();
			//TInt iElses = (iIFPile->Peek())->GetElseCount();
			iInSubClause = (iIfs == 0) ? EFalse : ETrue;
			//iInSubClause = ( (iIfs == 0) && (iElses == 0) ) ? EFalse : ETrue;

			//	decrement our counters. If we are NOT in a SubClause then these will
			//	be zero. If so we SHOULD execute the ENDIF
			(iIFPile->Peek())->EndIf();
			}

		switch (eIFState)
			{
			//	we are in the middle of an if,else,endif block
			//	note: the logic here is tortuous!
			//	iIfState is true if the IF was true (i.e.
			//iSOver = iIfState; break;
			case CIFControl::EInIF : 
				iSOver = (iSOver || (eIFMode == CIFControl::EELSETrue)) && iInSubClause; 
				break;
			case CIFControl::EInELSE : 
				iSOver = (iSOver || (eIFMode == CIFControl::EIFTrue)) && iInSubClause; 
				break;
			default : 
				if (iInSubClause)
					iSOver = ETrue;
				break;
			}
		break;
		}
				
	//	other commands must be checked against our if state!
	default : 
		switch (eIFState)
			{
			case CIFControl::EInIF :
				switch (eIFMode)
					{
					case CIFControl::EIFTrue : iSOver = EFalse; break;
					case CIFControl::EELSETrue : iSOver = ETrue; break;
					default : iSOver = ETrue; break;	//	?????
					}
				break;

			case CIFControl::EInELSE :
				switch (eIFMode)
					{
					case CIFControl::EIFTrue : iSOver = ETrue; break;
					case CIFControl::EELSETrue : iSOver = EFalse; break;
					default : iSOver = ETrue; break;	//	?????
					//case CIFControl::EENDIF : iSOver = !iBranch; break;
					}
				break;

			default : 
				switch (eIFMode)
					{
					case CIFControl::EIFTrue : iSOver = ETrue; break;
					case CIFControl::EELSETrue : iSOver = EFalse; break;
					default : iSOver = EFalse; break;	//	?????
					//case CIFControl::EENDIF : iSOver = ETrue; break;
					}
			break;
			}
		break;
	}

return iSOver;
}

//-----------------------------------------------------------------------------
//	Assign the passed value to the last result define

void CTEngine::SetResultDefine(TInt aValue)
	{
	TInt error;

	//	so that we can process it if required - code stolen from CmdDefine...
	TBuf<16+2> taglabel;
	taglabel.Format(KLastResultCode);

	// Set tag value to be the last error code received
	TBuf<12> tagvalue;
	tagvalue.Format(_L("%d"), aValue);
	CLabeledText *tag = Defines()->Text(taglabel);
	if (tag != NULL)
		{
		//	alter tag
		TRAP(error, tag->SetL(tagvalue));
		}
	else
		{
		//	Add a tag.
		TRAP(error, Defines()->AddL(taglabel, tagvalue));
		}
	}

void CTEngine::SetResultDefine (const TDesC& aDefineTxt, TInt aValue)
	{
	TInt error;

	TBuf<32+2> taglabel;

	taglabel.Format(_L("$%SHttpStatus$"), &aDefineTxt);

	// Set tag value to be the last error code received
	TBuf<16> tagvalue;
	tagvalue.Format(_L("%d"), aValue);
	CLabeledText *tag = Defines()->Text(taglabel);

	if (tag != NULL)
		{
		TRAP(error, tag->SetL(tagvalue));
		}
	else
		{
		//	Add a tag.
		TRAP(error, Defines()->AddL(taglabel, tagvalue));
		}
	}

void CTEngine::DeleteResultDefine (const TDesC& aDefineTxt)
	{
	TBuf<32+2> taglabel;

	taglabel.Format(_L("$%SHttpStatus$"), &aDefineTxt);

	Defines()->Delete(taglabel);
	}

//-----------------------------------------------------------------------------
//	Assign the passed value to the last result define

void CTEngine::SetErrorDefine(TInt aValue)
	{
	TInt error;

	//	so that we can process it if required - code stolen from CmdDefine...
	TBuf<16+2> taglabel;
	taglabel.Format(KLastErrorCode);

	// Set tag value to be the last error code received
	TBuf<12> tagvalue;
	tagvalue.Format(_L("%d"), aValue);
	CLabeledText *tag = Defines()->Text(taglabel);
	if (tag != NULL)
		{
		//	alter tag
		TRAP(error, tag->SetL(tagvalue));
		}
	else
		{
		//	Add a tag.
		TRAP(error, Defines()->AddL(taglabel, tagvalue));
		}
	}

void CTEngine::SetErrorDefine (const TDesC& aDefineTxt, TInt aValue)
	{
	TInt error;

	TBuf<32+2> taglabel;

	taglabel.Format(_L("$%SLastEvent$"), &aDefineTxt);

	// Set tag value to be the last error code received
	TBuf<16> tagvalue;
	tagvalue.Format(_L("%d"), aValue);
	CLabeledText *tag = Defines()->Text(taglabel);

	if (tag != NULL)
		{
		TRAP(error, tag->SetL(tagvalue));
		}
	else
		{
		//	Add a tag.
		TRAP(error, Defines()->AddL(taglabel, tagvalue));
		}
	}

void CTEngine::DeleteErrorDefine (const TDesC& aDefineTxt)
	{
	TBuf<32+2> taglabel;

	taglabel.Format(_L("$%SLastEvent$"), &aDefineTxt);

	Defines()->Delete(taglabel);
	}

//-----------------------------------------------------------------------------
//	Called by the scheduler evert time request completed.

void CTEngine::RunL()
{
switch (iState)
	{
	case EIdle:
		if (iStopped)
			// We've finished - close resources...
			{
			CloseCmdFiles();
			CloseLogFiles();

			// Cancel...
			Cancel();

			//	Stop the active scheduler only if we're driven via commandline script.
			//	if via interactive then has no meaning!
			//if (Console != NULL)
			CActiveScheduler::Stop();
			}
		break;

	case EStopping:
		{
		// Close any open sessions and their transactions
		if ( iCmdFamily != NULL )
			{
			//	Perform any operational close/shutdown operations here
			//CCmdBase *handler = NULL;
			//handler = iCmdFamily->Command(EDoClose);
			//STATIC_CAST(CCmdDoClose*, handler)->CloseAll();
			}
		iState = EIdle;
		} 
		CompleteRequest();
		break;

	case EGetNextCmd:
		// Get the next cmd
		if (ReadCmdline(iConsoleEnabled) != KErrNone)
			{
			// Something went wrong, so stop
			iStopped = ETrue;
			iState = EIdle;

			// Complete self
			CompleteRequest();
			}
		else
			// Parse the next cmd once we get it
			iState = EParseCurrentCmd;
		break;

	case EParseCurrentCmd:
		// Run the parsed cmd or Could not parse the cmd line so get the next cmd
		iState = (MakeCommand() == KErrNone) ? ERunCurrentCmd : EGetNextCmd;

		// Complete self and set active
		CompleteRequest();
		break;

	case ERunCurrentCmd:
		{
		TInt iErr = ObeyCommandL();

		// Run the cmd - the cmd will complete the request 
		if (iErr != KErrNone) 
			{
			//	if major problem then complete the request
			if (!IsActive())
				CompleteRequest();
			else	//	command requested produced subsequent operational error (i.e. what we're looking for!)
				{
				//	assign the response to an accessible 'variable'
				}
			}

		// Set state...
		iState = (!iStopped) ? EGetNextCmd : EStopping;
		}
		
		break;
	
	default : break;
	}
}

//-----------------------------------------------------------------------------

void CTEngine::DoCancel()
{
iTimer.Cancel();
iConsoleReader->ReadCancel();
}

//-----------------------------------------------------------------------------
//	Standard Issue...

void CTEngine::CompleteRequest()
{
if (IsActive())
	return ;

TRequestStatus* pStat = &iStatus;

User::RequestComplete(pStat, KErrNone);
SetActive();
}

//-----------------------------------------------------------------------------
//	was 'GetAnEntry' but that is slightly inaccurate!

void CTEngine::GetUserInput(const TDesC& ourPrompt, TDes& currentstring)
{
TBuf16<KMaxUserEntrySize> ourLine;
TBuf<KMaxUserEntrySize> tempstring;	//tempstring is a unicode descriptor

//	create a temporary buffer where the unicode strings are stored in order to 
//	be displayed
ourLine.Zero();
tempstring.Copy(currentstring);		//Copy current string to Unicode buffer
TKeyCode key = EKeyNull;						//current string buffer is 8 bits wide.

//	Unicode string bufffer (tempstring) is 16 bits wide.
FOREVER
	{
	if (ourLine.Length () == 0)
		{

		iConsole->SetPos (0, iConsole->WhereY ());
		iConsole->Printf (_L ("%S"), &ourPrompt);
		if (tempstring.Length () != 0)						//get tempstring's number of items
			iConsole->Printf (_L (" = %S"), &tempstring);	//if not zero print them to iTest.Console()
		iConsole->Printf (_L (" : "));
		iConsole->ClearToEndOfLine ();
		}

	key = iConsole->Getch();

	if (key == EKeyBackspace)
		{
		if (ourLine.Length() !=0)
			{
			ourLine.SetLength(ourLine.Length()-1);
			iConsole->Printf(_L ("%c"), key);
			iConsole->SetPos(iConsole->WhereX(), iConsole->WhereY());
			iConsole->ClearToEndOfLine();
			}	// end if (ourLine.Length() !=0)
		}	// end if (key == KeyBackSpace)


	if (key == EKeyDelete) 			
		{
		ourLine.Zero();
		iConsole->SetPos(0, iConsole->WhereY());
		iConsole->ClearToEndOfLine();
		tempstring.Copy(ourLine);
		break;
		}

	if (key == EKeyEnter)
		break;

	if (key < ' ') // ascii code thats not a printable character
		continue;

	ourLine.Append(key);
	iConsole->Printf(_L ("%c"), key);
	iConsole->SetPos(iConsole->WhereX(), iConsole->WhereY());
	iConsole->ClearToEndOfLine();
	if (ourLine.Length () == ourLine.MaxLength ())
		break;
	}	// end of for statement

if ((key == EKeyEnter) && (ourLine.Length () == 0))
	tempstring.Copy (currentstring);				//copy contents of 8 bit "ourLine" descriptor

iConsole->SetPos(0, iConsole->WhereY());		
iConsole->ClearToEndOfLine();

if ((key == EKeyEnter) && (ourLine.Length() !=0))
	tempstring.Copy(ourLine);

if (tempstring.Length () != 0)						//if temstring length is not zero
	{
	iConsole->Printf(_L(" Entered = %S\n"), &tempstring);	//print the contents to iTest.Console()
	LogPrintf(_L("%S = %S\n"), &ourPrompt, &tempstring);
	}

iConsole->Printf (_L("\n"));
currentstring.Copy(tempstring);						//copy 16 bit tempstring descriptor back 
} 

//-----------------------------------------------------------------------------
//	File Management and Support
//	note: this takes the current 'path' as defined by PATH as the root file path 
//	      if defined.

TInt CTEngine::SetFileName(const TDesC &aName, 	RFile &aReqFile)
	{
	TBuf<256> ourFile(iCmdPath);
	ourFile.Append(aName);

	TParse ParsedFileName;
	TInt iError = ParsedFileName.Set(ourFile, NULL, NULL); 

	if (iError == KErrNone)
		{//	check is a valid file and open a handle

		//	assign the filename to our reference
		TFileName fileName;
		fileName.Copy(ParsedFileName.FullName());

		if (iFileServer.IsValidName(fileName))
			{
			iError = aReqFile.Open(iFileServer, ParsedFileName.FullName(), EFileRead);
			}
		}
		
	return iError;
	}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Do a formatted dump of binary data, optionally logging it
//	Iterate the supplied block of data in blocks of 16 bytes

void CTEngine::DumpData(const TDesC8& aData, const TBool &logIt)
{
TInt pos = 0;
TBuf<KMaxLogEntrySize> logLine;
TBuf<KMaxLogEntrySize> anEntry;

while (pos < aData.Length())
	{
	anEntry.Format(TRefByValue<const TDesC>_L("%04x : "), pos);
	logLine.Append(anEntry);

	// Hex output
	TInt offset;
	for (offset = 0; offset < 16; ++offset)
		{
		if (pos + offset < 10)//aData.Length())
			{
			TInt nextByte = aData[pos + offset];
			anEntry.Format(TRefByValue<const TDesC>_L("%02x "), nextByte);
			logLine.Append(anEntry);
			}
		else
			{
			anEntry.Format(TRefByValue<const TDesC>_L("   "));
			logLine.Append(anEntry);
			}
		}
	
	anEntry.Format(TRefByValue<const TDesC>_L(": "));
	logLine.Append(anEntry);

	// Char output
	for (offset = 0; offset < 16; ++offset)
		{
		if (pos + offset < 10)//aData.Length())
			{
			TInt nextByte = aData[pos + offset];
			if ((nextByte >= 32) && (nextByte <= 127))
				{
				anEntry.Format(TRefByValue<const TDesC>_L("%c"), nextByte);
				logLine.Append(anEntry);
				}
			else
				{
				anEntry.Format(TRefByValue<const TDesC>_L("."));
				logLine.Append(anEntry);
				}
			}
		else
			{
			anEntry.Format(TRefByValue<const TDesC>_L(" "));
			logLine.Append(anEntry);
			}
		}

	if (logIt)
		LogPrintf(TRefByValue<const TDesC>_L("%S"), &logLine);
	
	// Advance to next 16 byte segment
	pos += 16;
	}
}

//-----------------------------------------------------------------------------

CIFControl::TIFMode CTEngine::GetIfMode() 
{
return (iIFPile->Peek()) ? (iIFPile->Peek())->GetIFMode() : CIFControl::ENotIf;
}

//-----------------------------------------------------------------------------

void CTEngine::SetIfMode(const CIFControl::TIFMode &eMode) 
{ 
if (iIFPile->Peek())
  (iIFPile->Peek())->SetIFMode(eMode); 
}

//-----------------------------------------------------------------------------

CIFControl::TIFProcessing CTEngine::GetIfState() 
{
return (iIFPile->Peek()) ? (iIFPile->Peek())->GetIFState() : CIFControl::EIgnoreIF;
}

//-----------------------------------------------------------------------------

void CTEngine::SetIfState(const CIFControl::TIFProcessing &eProc) 
{
if (iIFPile->Peek())
  (iIFPile->Peek())->SetIFState(eProc); 
}

//-----------------------------------------------------------------------------
//	Create new IF object to manage the current processing
//	Use the EndIf to Pop the current object when ENDIF is processed

void CTEngine::IfL(const CIFControl::TIFMode &eProc, 
									const TBool &aResult, 
									const CIFControl::TIFProcessing &aProcess) 
{
CIFControl *cif = CIFControl::NewLC(eProc, aResult, aProcess);

iIFPile->PushL(cif);
// CleanupStack::PopAndDestroy();
CleanupStack::Pop();
}

//-----------------------------------------------------------------------------

void CTEngine::EndIf() 
{
if (iIFPile->Peek())
	iIFPile->Skim();
}

//-----------------------------------------------------------------------------
//	End of File
//-----------------------------------------------------------------------------
