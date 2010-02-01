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
// This module implements the collection of stadard command classes for the
// tester framework.
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Definition Files  
// 
//

#include "CCmdStandard.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Comment command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdRemark *CCmdRemark::NewL(TInt aCommandId, const TDesC& aKeyphrase)
{
CCmdRemark* self = NewLC(aCommandId, aKeyphrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdRemark *CCmdRemark::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdRemark* self = new (ELeave) CCmdRemark();
CleanupStack::PushL(self);
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

void CCmdRemark::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL( aCommandId, aKeyphrase );
}

//-----------------------------------------------------------------------------

TBool CCmdRemark::Recognize( const TDesC& aCommand  )
{
// Obey the command family's step over mode.
SetStepOver(Family()->StepOver());

// Check if is empty or begins with # character.
TPtrC cmd = TfrLex::Trim(aCommand);

//	look for empty, # or //
return ((cmd.Length() == 0) || (cmd.Match( _L("#*")) == 0) || (cmd.Match(_L("//*")) == 0 ));
}

//-----------------------------------------------------------------------------

TInt CCmdRemark::ProcessL( const TDesC& aCommand )
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

if (!Recognize(aCommand))
	return Error(KErrArgument, TFR_KFmtErrBadCmd, &Keyphrase());

// Do nothing.
return KErrNone;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	EXIT command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdExit *CCmdExit::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdExit* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdExit *CCmdExit::NewLC(TInt aCommandId, const TDesC &aKeyphrase)
{
CCmdExit* self = new (ELeave) CCmdExit();
CleanupStack::PushL(self);
self->ConstructL(aCommandId, aKeyphrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdExit::ConstructL(TInt aCommandId, const TDesC &aKeyphrase)
{
CCmdBase::ConstructL(aCommandId, aKeyphrase);
}

//-----------------------------------------------------------------------------

TInt CCmdExit::ProcessL(const TDesC &aCommand)
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD(error, param.Set(ParamsL(aCommand)));
if (error != KErrNone )
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

TLex parse( param );
if (!parse.Eos() && !parse.Peek().IsSpace())
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase());

// Print out param if any and stop.
param.Set( TfrLex::Peel(param));
if (param.Length() > 0) 
	Print(param);

Machine()->StopEngine();
return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	ECHOMODE On/Off command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdEchoMode *CCmdEchoMode::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdEchoMode* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdEchoMode *CCmdEchoMode::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdEchoMode* self = new (ELeave) CCmdEchoMode();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

void CCmdEchoMode::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL( aCommandId, aKeyphrase );
}

//-----------------------------------------------------------------------------

TInt CCmdEchoMode::ProcessL(const TDesC& aCommand)
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD(error, param.Set(ParamsL(aCommand)));
if (error != KErrNone)
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

TLex parse( param );
if (!parse.Peek().IsSpace())
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

// Param shall be On or Off?
param.Set(TfrLex::TrimAndPeel(param));
TBool value = (param.CompareF(TFR_KTxtTermOn) == 0);
if ( !value && param.CompareF(TFR_KTxtTermOff) != 0)
	return Error(KErrArgument, TFR_KFmtErrBadCmd, &Keyphrase());

// Set Echo Mode On/Off.
Machine()->SetEchoMode(value);
return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	CHECKMODE on/off command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdCheckMode* CCmdCheckMode::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdCheckMode* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdCheckMode* CCmdCheckMode::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdCheckMode* self = new (ELeave) CCmdCheckMode();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

void CCmdCheckMode::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL( aCommandId, aKeyphrase );
}

//-----------------------------------------------------------------------------

TInt CCmdCheckMode::ProcessL( const TDesC& aCommand )
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD( error, param.Set( ParamsL( aCommand )) );
if ( error != KErrNone )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

TLex parse( param );
if ( !parse.Peek().IsSpace() )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

// Param shall On or Off?
param.Set( TfrLex::TrimAndPeel( param ) );
TBool value = (param.CompareF(TFR_KTxtTermOn) == 0);
if ( !value && param.CompareF(TFR_KTxtTermOff) != 0 )
	return Error( KErrArgument, TFR_KFmtErrBadCmd, &Keyphrase() );

// Set Check Mode (ParseOnly) On/Off.
Family()->SetSwitch(CCmdFamily::EParseOnly, value);
return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	PRINT command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdPrint *CCmdPrint::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdPrint *self = NewLC(aCommandId, aKeyphrase, aHelpPhrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdPrint *CCmdPrint::NewLC(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdPrint *self = new (ELeave) CCmdPrint();
CleanupStack::PushL(self);
self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdPrint::ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);

//	set the dummy echo mode 
iCurEchoMode = EFalse;
}

//-----------------------------------------------------------------------------
//	look for PRINT* but note that Match is casesensitive and hence use temp TBuf value to check
//	and note that the command string might be quite long (and we're only interested in the 
//	first left bit!!

TBool CCmdPrint::Recognize(const TDesC& aCommand)
{
TBuf<16> iCmd = aCommand.Left(5);
iCmd.UpperCase();
TInt iResult = iCmd.Match(THA_TxtCmdPrint);

if (iResult == KErrNotFound)	//	NF(-1) otherwise >=0 here means position of string in host
	return EFalse;

return ETrue;			//	accept it for now!
}

//-----------------------------------------------------------------------------

TInt CCmdPrint::ProcessL(const TDesC& aCommand)
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD(error, param.Set(ParamsL(aCommand)));
if (error != KErrNone)
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

TLex parse(param);
if ( !parse.Eos() && !parse.Peek().IsSpace() )
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

// ParseOnly => not executed.
if (Family()->Switch(CCmdFamily::EParseOnly))
	return error;

iCurEchoMode = Machine()->EchoMode();
Machine()->SetEchoMode(EFalse);

// Print out the param text.
param.Set(TfrLex::TrimAndPeel(param));
//Print(param, EFalse);
Print(param, ETrue);
//Print(param.Mid(6), EFalse);

//	reset the echo mode to that of before this command!
Machine()->SetEchoMode(iCurEchoMode);

return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	PAUSE command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdPause* CCmdPause::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdPause* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdPause* CCmdPause::NewLC(TInt aCommandId, const TDesC& aKeyphrase)
{
CCmdPause* self = new (ELeave) CCmdPause();
CleanupStack::PushL(self);
self->ConstructL(aCommandId, aKeyphrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdPause::ConstructL(TInt aCommandId, const TDesC& aKeyphrase)
{
CCmdBase::ConstructL(aCommandId, aKeyphrase);
}

//-----------------------------------------------------------------------------

TInt CCmdPause::ProcessL(const TDesC& aCommand)
{
TPtrC param;
TRAPD(error, param.Set(ParamsL(aCommand)));
if (error != KErrNone)
	{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());
	}

TLex parse( param );
if (!parse.Eos() && !parse.Peek().IsSpace())
	{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());
	}

// ParseOnly => not executed.
if (Family()->Switch(CCmdFamily::EParseOnly))
	{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();
	return error;
	}

// Ask any key, prompt is either param or a default.
param.Set( TfrLex::TrimAndPeel(param));
if (param.Length() == 0) 
	param.Set(TFR_KTxtMsgPressAnyKey);

Machine()->Pause(param);
return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	HOLD command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdHold* CCmdHold::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdHold* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdHold* CCmdHold::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdHold* self = new (ELeave) CCmdHold();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

void CCmdHold::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL ( aCommandId, aKeyphrase );
}

//-----------------------------------------------------------------------------

TInt CCmdHold::ProcessL( const TDesC& aCommand )
{
TPtrC param;
TRAPD( error, param.Set( ParamsL( aCommand )) );
if ( error != KErrNone )
	{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase() );
	}

TLex parse( param );
if ( !parse.Eos() && !parse.Peek().IsSpace() )
	{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );
	}

// Param must be integer. If no param then default is 1 second.
param.Set( TfrLex::TrimAndPeel(param) );
parse = param;

TInt n;
if ( parse.Eos() )
	n = 1;

else if (parse.Val(n) != KErrNone || !parse.Eos())
	{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());
	}

// ParseOnly => not executed!
if (Family()->Switch(CCmdFamily::EParseOnly) )
	{
	// Complete the test machine - will then get the next cmd
	Machine()->CompleteRequest();
	return error;
	}

// Hold processing for n seconds.
TTimeIntervalMicroSeconds32 microseconds;
microseconds = 1000000 * n;
Machine()->Hold(microseconds);
return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	SETPROMPT command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdSetPrompt* CCmdSetPrompt::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdSetPrompt* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdSetPrompt* CCmdSetPrompt::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdSetPrompt* self = new (ELeave) CCmdSetPrompt();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

void CCmdSetPrompt::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL ( aCommandId, aKeyphrase );
}

//-----------------------------------------------------------------------------

TInt CCmdSetPrompt::ProcessL( const TDesC& aCommand )
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD( error, param.Set( ParamsL( aCommand )) );
if ( error != KErrNone )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

TLex parse( param );
if ( !parse.Eos() && !parse.Peek().IsSpace() )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

// ParseOnly => not executed.
if ( Family()->Switch(CCmdFamily::EParseOnly) )
	return error;

// Alter prompt, back to the default one if no new one.
param.Set( TfrLex::TrimAndPeel( param ) );
if ( param.Length() == 0 ) 
	param.Set(THA_CommandPrompt);

if ( error = Machine()->SetPrompt( param ), error != KErrNone )
	// ### Failure
	Log( TFR_KFmtErrFailed, &Keyphrase(), error );

return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	SETPATH command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdSetPath *CCmdSetPath::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdSetPath* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdSetPath *CCmdSetPath::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdSetPath* self = new (ELeave) CCmdSetPath();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

void CCmdSetPath::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL ( aCommandId, aKeyphrase );
}

//-----------------------------------------------------------------------------

TInt CCmdSetPath::ProcessL( const TDesC& aCommand )
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD(error, param.Set(ParamsL(aCommand)));
if (error != KErrNone)
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

TLex parse( param );
if ( !parse.Eos() && !parse.Peek().IsSpace() )
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

// Set the command file path.
param.Set( TfrLex::TrimAndPeel(param));
error = Machine()->SetCmdPath(param);
switch (error)
	{
	// Bad argument
	case KErrArgument : Log(TFR_KFmtErrParams, &Keyphrase()); break;
	case KErrNone : break;
	// Other errors
	default : Log(TFR_KFmtErrFailed, &Keyphrase(), error); break;
	}

return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	CALL command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//	This executes a script file (!) and stacks the commands in it ready for the
//	scheduler... 

CCmdCall* CCmdCall::NewL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdCall* self = NewLC( aCommandId, aKeyphrase, aHelpPhrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdCall* CCmdCall::NewLC( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdCall* self = new (ELeave) CCmdCall();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase, aHelpPhrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdCall::ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdBase::ConstructL ( aCommandId, aKeyphrase, aHelpPhrase);
}

//-----------------------------------------------------------------------------

TInt CCmdCall::ProcessL( const TDesC& aCommand )
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD( error, param.Set( ParamsL( aCommand )) );
if ( error != KErrNone )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

TLex parse( param );
if ( !parse.Peek().IsSpace() )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

// Extract command file name.
parse = TfrLex::Trim( param );
TPtrC file;
TRAP(error,file.Set(TfrLex::GetL(parse)));
if ( error != KErrNone )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

file.Set(TfrLex::PeelAndTrim(file));
if ( file.Length() == 0 )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

// The remainder contains optional arguments
TPtrC args = TfrLex::Trim(parse.Remainder());

// Before calling the command file, do check that the argument list
// is lexically correct and that there are not too many of them....
TPtrC argv;
TInt  argc = -1;
do
	{
	argc++;
	TRAP(error, argv.Set(TfrLex::GetL(parse)));
	} while ( error == KErrNone && argv.Length() > 0 );

// Well, how was it?
if ( error != KErrNone || argc > TFR_MAX_CALL_ARGUMENTS )
	{
	if ( error == KErrNone ) 
		error = KErrArgument;
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );
	}

// Call the command file with the argument list.
error = Machine()->CallCmdFile(file, args);
if (error != KErrNone )
	// ### Failure
	Log( TFR_KFmtErrFailed, &Keyphrase(), error );

return error;    
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	RETURN command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdReturn* CCmdReturn::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdReturn* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdReturn* CCmdReturn::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdReturn* self = new (ELeave) CCmdReturn();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

void CCmdReturn::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL( aCommandId, aKeyphrase );
}

//-----------------------------------------------------------------------------

TInt CCmdReturn::ProcessL(const TDesC& aCommand)
{
// Complete the test machine
Machine()->CompleteRequest();

TPtrC param;
TRAPD(error, param.Set(ParamsL(aCommand)));
if (error != KErrNone)
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

TLex parse(param);
parse.SkipSpace();
if (!parse.Eos())
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

// Return from the current command file
error = Machine()->CloseCmdFile();
if (error != KErrNone)
	Log(TFR_KFmtErrFailed, &Keyphrase(), error);

return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	LOGPATH command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdLogPath* CCmdLogPath::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdLogPath* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdLogPath* CCmdLogPath::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdLogPath* self = new (ELeave) CCmdLogPath();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

void CCmdLogPath::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL ( aCommandId, aKeyphrase );
}

//-----------------------------------------------------------------------------

TInt CCmdLogPath::ProcessL( const TDesC& aCommand )
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD( error, param.Set( ParamsL( aCommand )) );
if ( error != KErrNone )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

TLex parse( param );
if ( !parse.Eos() && !parse.Peek().IsSpace() )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

// Set log file path.
param.Set( TfrLex::TrimAndPeel( param ) );

//	
if ( error = Machine()->SetLogPath( param ), error != KErrNone )
	{
	// Failed
	if ( error == KErrArgument )
		// Bad arguments
		Log( TFR_KFmtErrParams, &Keyphrase() );
	else
		// Other errors
		Log( TFR_KFmtErrFailed, &Keyphrase(), error );
	}

return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	LOGFILE command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdLogFile* CCmdLogFile::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdLogFile *self = NewLC(aCommandId, aKeyphrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdLogFile* CCmdLogFile::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdLogFile *self = new (ELeave) CCmdLogFile();
CleanupStack::PushL(self);
self->ConstructL(aCommandId, aKeyphrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdLogFile::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL(aCommandId, aKeyphrase);
}

//-----------------------------------------------------------------------------

TInt CCmdLogFile::ProcessL( const TDesC& aCommand )
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD(error, param.Set(ParamsL(aCommand)));
if (error != KErrNone)
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

TLex parse(param);
if (!parse.Peek().IsSpace())
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

// Open log file.
param.Set(TfrLex::TrimAndPeel(param));
error = Machine()->OpenLogFile(param);
if (error != KErrNone)
	// ### Failure
	Log(TFR_KFmtErrFailed, &Keyphrase(), error);

return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	ENDLOG command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdEndLog* CCmdEndLog::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdEndLog* self = NewLC(aCommandId, aKeyphrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdEndLog* CCmdEndLog::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdEndLog* self = new (ELeave) CCmdEndLog();
CleanupStack::PushL(self);
self->ConstructL(aCommandId, aKeyphrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdEndLog::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL( aCommandId, aKeyphrase );
}

//-----------------------------------------------------------------------------

TInt CCmdEndLog::ProcessL( const TDesC& aCommand )
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD( error, param.Set( ParamsL( aCommand )) );
if ( error != KErrNone )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

TLex parse( param );
parse.SkipSpace();
if ( !parse.Eos() )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase() );

// Close present log file
if ( error = Machine()->CloseLogFile(), error != KErrNone )
	// ### Failure
	Log( TFR_KFmtErrFailed, &Keyphrase(), error );

return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	DEFINE command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdDefine *CCmdDefine::NewL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdDefine *self = NewLC( aCommandId, aKeyphrase, aHelpPhrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdDefine *CCmdDefine::NewLC( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdDefine *self = new (ELeave) CCmdDefine();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase, aHelpPhrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdDefine::ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
}

//-----------------------------------------------------------------------------
//	We can define variables ($var16charname$) which can then be used by the 
//	commands we supply

TInt CCmdDefine::ProcessL(const TDesC& aCommand)
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD(error, param.Set(ParamsL(aCommand)));
if ( error != KErrNone )
	return Error( error, TFR_KFmtErrBadCmd, &Keyphrase());

TLex parse(param);
if ( !parse.Eos() && !parse.Peek().IsSpace())
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

parse.SkipSpace();
if (parse.Eos())
	{
	// If no params, print out present tag definitions.
	CCatalogue* catalog = Machine()->Defines();
	for (TInt i = 0; i < catalog->Count(); ++i)
		{
		CLabeledText *tag = catalog->At(i);
		TPtrC label = tag->Label();
		TPtrC value = tag->Value();
		Printf( _L("\t%S \"%S\""), &label, &value );
		}
	return error;
	}

// Get tag name, must not exceed 16 characters.
TPtrC tagname;
TRAP(error,tagname.Set(TfrLex::GetL(parse)));
if ( error != KErrNone) 
	return Error(KErrArgument, TFR_KFmtErrBadCmd, &Keyphrase());

tagname.Set(TfrLex::Peel(tagname));
if ( tagname.Length() > 32 )
	return Error(KErrArgument, TFR_KFmtErrBadCmd, &Keyphrase());

// Get tag value (is optional).
TPtrC tagvalue;
TRAP(error,tagvalue.Set(TfrLex::GetL(parse)));
if (error != KErrNone) 
	return Error(KErrArgument, TFR_KFmtErrBadCmd, &Keyphrase());

// There shall not be anything more.
TPtrC remainder = TfrLex::Trim(parse.Remainder());
if (remainder.Length() > 0) 
	return Error(KErrArgument, TFR_KFmtErrBadCmd, &Keyphrase());

// Execute...
// Construct tag label = $tagname$ (never more than 16+2 chars).
TBuf<32+2> taglabel;
taglabel.Format(_L("$%S$"), &tagname);

// Tag value specified?
if (tagvalue.Length() > 0)
	{
	// Check if the tag already is defined. Depending on that,
	// either alter the tag or add a new one.
	tagvalue.Set(TfrLex::Peel(tagvalue));

	CLabeledText *tag = Machine()->Defines()->Text(taglabel);
	
	if (tag != NULL)
		{
		//	alter tag
		TRAP(error, tag->SetL(tagvalue) );
		}
	else
		{
		//	Add a tag.
		TRAP(error, Machine()->Defines()->AddL(taglabel, tagvalue));
		}
	
	}
else
	{
	// Define with no tag value: delete tag if it's defined.
	Machine()->Defines()->Delete(taglabel);
	error = KErrNone;
	}

if (error != KErrNone)
	Log(TFR_KFmtErrFailed, &Keyphrase(), error);

return error;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Comment ? - the ListAll Command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdListAll *CCmdListAll::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdListAll* self = NewLC(aCommandId, aKeyphrase, aHelpPhrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdListAll *CCmdListAll::NewLC( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdListAll* self = new (ELeave) CCmdListAll();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase, aHelpPhrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdListAll::ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdBase::ConstructL( aCommandId, aKeyphrase, aHelpPhrase);
}

//-----------------------------------------------------------------------------

TBool CCmdListAll::Recognize( const TDesC& aCommand  )
{
// Check if is empty or begins with # character.
TPtrC cmd = TfrLex::Trim(aCommand);

return (cmd.Match( _L("?")) == 0);
}

//-----------------------------------------------------------------------------

TInt CCmdListAll::ProcessL(const TDesC& aCommand)
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

if (!Recognize(aCommand))
	return Error(KErrArgument, TFR_KFmtErrBadCmd, &Keyphrase());

// List commands out
Family()->ListAll(Console());
return KErrNone;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	IF command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdIf *CCmdIf::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdIf *self = NewLC(aCommandId, aKeyphrase, aHelpPhrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdIf *CCmdIf::NewLC(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdIf *self = new (ELeave) CCmdIf();
CleanupStack::PushL(self);
self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdIf::ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
}

//-----------------------------------------------------------------------------
//	recognize offers us a mechanism of 'peeking' at the command such that
//	we can then decide if we need to process it. This is vital for IF, ELSE and ENDIF
//	since we need to look at the command, see if its one that can change the course
//	of processing or not. Clearly doing this for every other command is wasteful and
//	unnecessary
//	
//	NOTE: we must return true only if the command does indeed match
//				we can do all other processing if necessary!

TBool CCmdIf::Recognize(const TDesC& aCommand)
{
//	look for IF* but note that Match is casesensitive and hence use temp TBuf value to check
//	and note that the command string might be quite long (and we're only interested in the 
//	first left bit!!
TBuf<16> iCmd = aCommand.Left(15);
iCmd.UpperCase();
TInt iResult = iCmd.Match(THA_TxtCmdIfS);
if (iResult == KErrNotFound)	//	NF(-1) otherwise >=0here means position of string in host
	return EFalse;

//	determine what the state is and if already in an IF, need to increment the IF counter!
//Machine()->UpdateIFState(CIFControl::EInIF);

return ETrue;			//	accept it for now!
}

//-----------------------------------------------------------------------------
//	the command is [ IF  $definedname$ == givenvalue ]
//	the result is either TRUE or FALSE
//	we should expect 3 parameters (including ==) and evaluate the resultant
//	comparison

TInt CCmdIf::ProcessL(const TDesC &aCommand)
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

//	this converts the first parameter (if found) into something useful!
TPtrC param;
TRAPD(error, param.Set(ParamsL(aCommand)));
if (error != KErrNone)
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

TLex parse(param);
if (!parse.Eos() && !parse.Peek().IsSpace())
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

parse.SkipSpace();
if (parse.Eos())
	//	no parameters so invalid command
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

//	convert into our LValue
TPtrC lvalue;
TRAP(error, lvalue.Set(TfrLex::GetL(parse)));
if (error != KErrNone)
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

lvalue.Set(TfrLex::Peel(lvalue));
if (lvalue.Length() > 32)
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

//	now check that the second 'param' is one of our comparison tokens...
TPtrC opr;
TRAP(error, opr.Set(TfrLex::GetL(parse)));
if (error != KErrNone) 
	return Error(KErrArgument, TFR_KFmtErrBadCmd, &opr);

//	check is valid length
opr.Set(TfrLex::Peel(opr));
if (opr.Length() > 16) 
	return Error(KErrArgument, THA_KErrParameterLong, &opr);

//	check operator is one of our known list
//	Param shall be HTTP or WSP
opr.Set(TfrLex::TrimAndPeel(opr));
TBool eValue;
if (opr.CompareF(KTxtEquals) == 0)
	eValue = TCO_Equals;

else if (opr.CompareF(KTxtNotEqual) == 0)
	eValue = TCO_NotEqual;

else if (opr.CompareF(KTxtGreaterThan) == 0)
	eValue = TCO_GreaterThan;

else if (opr.CompareF(KTxtLessThan) == 0)
	eValue = TCO_LessThan;

else if (opr.CompareF(KTxtGreaterThanEq) == 0)
	eValue = TCO_GreaterThanEq;

else if (opr.CompareF(KTxtLessThanEq) == 0)
	eValue = TCO_LessThanEq;

else	//	not recognized the comp.operator so error
	return Error(KErrArgument, THA_KErrInvalidOptr, &opr);

//	now look at the final parameter and convert into our LValue
//	assuming that there is anything more.
TPtrC rvalue = TfrLex::Trim(parse.Remainder());
if (rvalue.Length() == 0) 
	return Error(KErrArgument, TFR_KFmtErrBadCmd, &aCommand);

rvalue.Set(TfrLex::TrimAndPeel(rvalue));
if (rvalue.Length() > 16)
	return Error(error, TFR_KFmtErrBadCmd, &Keyphrase());

// ParseOnly => not executed.
if (Family()->Switch(CCmdFamily::EParseOnly))
	return error;

//	determine if we're comparing numeric or data (or both!)
TBool iNumeric = ETrue;
TInt ilval;
TInt irval;
if (InetProtTextUtils::ConvertDescriptorToInt(lvalue,ilval) < 0)
	iNumeric = EFalse;

//	check we're not comparing apples with oranges...
iNumeric = ((InetProtTextUtils::ConvertDescriptorToInt(rvalue,irval) < 0) && iNumeric) ? EFalse : ETrue;

//	now determine the comparison result (also 
//	-1 => error or failure (i.e. < 0) 0 = false, 1 = true
TInt iResult = -2;
switch (eValue)
	{
	case TCO_Equals : 
		iResult = (iNumeric) ? ((TInt) (ilval == irval)) : ((lvalue.Match(rvalue) == 0) ? 1 : 0); break;
	case TCO_NotEqual : 
		iResult = (iNumeric) ? ((TInt) (ilval != irval)) : ((lvalue.Match(rvalue) != 0) ? 1 : 0); break;
	case TCO_GreaterThan : 
		iResult = (iNumeric) ? ((TInt) (ilval > irval)) : ((lvalue.Match(rvalue) == 1) ? 1 : 0); break;
	case TCO_LessThan : 
		iResult = (iNumeric) ? ((TInt) (ilval < irval)) : ((lvalue.Match(rvalue) == -1) ? 1 : 0); break;
	case TCO_GreaterThanEq : 
		iResult = (iNumeric) ? ((TInt) (ilval >= irval)) : ((lvalue.Match(rvalue) >= 0) ? 1 : 0); break;
	case TCO_LessThanEq : 
		iResult = (iNumeric) ? ((TInt) (ilval <= irval)) : ((lvalue.Match(rvalue) < 1) ? 1 : 0); break;
	}

//	the result now determines if we continue processing (and ignore any following ELSE)
//	before we meet the ENDIF or we skip and perform the ELSE clause...
//	NOTE: now we can nest IF's etc, we MUST count those that are met when we
//				'ignore' lines as these MAY be incorrectly nested...
TBool iSuccess = (TBool) iResult;
if (iSuccess)
	Machine()->IfL(CIFControl::EIFTrue, iSuccess, CIFControl::EInIF);
else
	//	failed test so skip the true block
	Machine()->IfL(CIFControl::EELSETrue, iSuccess, CIFControl::EInIF);

//	whilst we would be better off returning the actual result, it bears little consequence
//	- in fact if we get here we're ok anyway...
return KErrNone;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	ELSE command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdElse *CCmdElse::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdElse *self = NewLC(aCommandId, aKeyphrase, aHelpPhrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdElse *CCmdElse::NewLC(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdElse *self = new (ELeave) CCmdElse();
CleanupStack::PushL(self);
self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdElse::ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
}

//-----------------------------------------------------------------------------
//	if we read ELSE then we need to check:
//	a) we're in an IF clause
//	b) the current branch to execute
//	c) if we need to change the stepover state

TBool CCmdElse::Recognize(const TDesC& aCommand)
{
//	look for ELSE
TBuf<16> iCmd = aCommand.Left(15);
iCmd.UpperCase();
TInt iResult = iCmd.Match(THA_TxtCmdElse);
if (iResult == KErrNotFound)
	return EFalse;

//	determine the next course of action in the ProcessL section
//	otherwise we won't complete the request!
return ETrue;
}

//-----------------------------------------------------------------------------
//	the command is [ IF  $definedname$ == givenvalue ]
//	the result is either TRUE or FALSE
//	we should expect 3 parameters (including ==) and evaluate the resultant
//	comparison

TInt CCmdElse::ProcessL(const TDesC &aCommand)
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD(ret, param.Set(ParamsL(aCommand)));
if (ret != KErrNone) 
	return Error(ret, KFmtErrInvalidCmd, &Keyphrase());

//	determine what the state is and if already in an IF, need to increment the IF counter!
//Machine()->UpdateIFState(CIFControl::EInELSE);

Machine()->SetIfState(CIFControl::EInELSE);

//	expect endif next
//TBool iResult = (iIFPile->Peek()) ? (iIFPile->Peek())->GetIFResult() : EFalse;
//Machine()->SetIfMode(CIFControl::EENDIF, iResult);
	
return ret;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	Command:	ENDIF command.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdEndIf *CCmdEndIf::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdEndIf *self = NewLC(aCommandId, aKeyphrase, aHelpPhrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdEndIf *CCmdEndIf::NewLC(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdEndIf *self = new (ELeave) CCmdEndIf();
CleanupStack::PushL(self);
self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdEndIf::ConstructL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
}

//-----------------------------------------------------------------------------
//	if we read ENDIF then we need to:
//	a) ensure we're in an IF clause
//	b) to reset the stepover state

TBool CCmdEndIf::Recognize(const TDesC& aCommand)
{
//	look for ENDIF
TBuf<16> iCmd = aCommand.Left(15);
iCmd.UpperCase();
TInt iResult = iCmd.Match(THA_TxtCmdEndIf);
if (iResult == KErrNotFound)
	return EFalse;

//	check we processing an IF sequence
TBool iIFInProgress = (TBool) (Machine()->GetIfMode() == CIFControl::ENotIf);
if (iIFInProgress)
	return EFalse;			//	abort if not in IF seq

return ETrue;
}

//-----------------------------------------------------------------------------
//	the command is [ IF  $definedname$ == givenvalue ]
//	the result is either TRUE or FALSE
//	we should expect 3 parameters (including ==) and evaluate the resultant
//	comparison

TInt CCmdEndIf::ProcessL(const TDesC& aCommand)
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD(ret, param.Set(ParamsL(aCommand)));
if (ret != KErrNone) 
	return Error(ret, KFmtErrInvalidCmd, &Keyphrase());

//	clearing the end if block is actually not as simple as at first
//	might be thought. we should get the 'machine' to verify that
//	the endif is appropriate (i.e. we are not reading an endif within
//	a step'd over clause

//	probably academic now!
//Machine()->SetIfState(CIFControl::EIgnoreIF);

Machine()->EndIf();

return ret;
}

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------
