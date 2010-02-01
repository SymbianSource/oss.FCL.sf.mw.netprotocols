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
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Module Definitions
// 
//

#include "CCmdBase.h"

//-----------------------------------------------------------------------------
// format for output of data/time values

//_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");
_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S");

const TInt K_OUTOFBOUNDS_YEAR = 2499;

//-----------------------------------------------------------------------------

CCmdBase *CCmdBase::NewL(TInt aCommandId, const TDesC& aKeyphrase)
{
CCmdBase *self = NewLC(aCommandId, aKeyphrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdBase *CCmdBase::NewLC(TInt aCommandId, const TDesC& aKeyphrase)
{
CCmdBase *self = new (ELeave) CCmdBase();
CleanupStack::PushL( self );
self->ConstructL(aCommandId, aKeyphrase);
return self;
}

//-----------------------------------------------------------------------------

CCmdBase *CCmdBase::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdBase *self = NewLC(aCommandId, aKeyphrase, aHelpPhrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdBase* CCmdBase::NewLC(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdBase* self = new (ELeave) CCmdBase();
CleanupStack::PushL(self);
self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdBase::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
iFamily = NULL;
iCommandId = aCommandId;
iKeyphrase.Set(aKeyphrase);
iHelpText.Set(NULL, 0);
iFlags = 0x00000000;
SetFlag( EDoLog , ETrue  );
iStepOver = EFalse;
}

//-----------------------------------------------------------------------------

void CCmdBase::ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase )
{
iFamily = NULL;
iCommandId = aCommandId;
iKeyphrase.Set(aKeyphrase);
iHelpText.Set(aHelpPhrase);
iFlags = 0x00000000;
SetFlag(EDoLog, ETrue);
iStepOver = EFalse;
}

//-----------------------------------------------------------------------------

CCmdBase::~CCmdBase()
{
}

//-----------------------------------------------------------------------------

CCmdBase::CCmdBase()
	: 
	CBase()
{
}

//-----------------------------------------------------------------------------

TInt CCmdBase::CommandId()
{
return iCommandId;
}

//-----------------------------------------------------------------------------

const TDesC &CCmdBase::Keyphrase()
{
return iKeyphrase;
}

//-----------------------------------------------------------------------------

void CCmdBase::SetFamily(CCmdFamily* aCmdFamily)
{
iFamily = aCmdFamily;
}

//-----------------------------------------------------------------------------

CCmdFamily* CCmdBase::Family( )
{
return iFamily;
}

//-----------------------------------------------------------------------------

void CCmdBase::SetHelpText( const TDesC& aHelpText )
{
iHelpText.Set(aHelpText);
}

//-----------------------------------------------------------------------------

const TDesC& CCmdBase::HelpText()
{
return iHelpText;
}

//-----------------------------------------------------------------------------

void CCmdBase::SetFlag( TInt aFlag, TBool aBoolean )
{
if ( aFlag < 1 || aFlag > 32 ) 
	return;

TUint32 mask = ( 0x00000001<<(aFlag-1) );
iFlags = (aBoolean) ? ( iFlags | mask ) : ( iFlags & (~mask) );
}

//-----------------------------------------------------------------------------

TBool CCmdBase::Flag( TInt aFlag )
{
if ( aFlag < 1 || aFlag > 32 ) 
	return EFalse;
TUint32 mask = ( 0x00000001<<(aFlag-1) );
return ( (iFlags & mask) != 0x00000000 );
}

//-----------------------------------------------------------------------------

void CCmdBase::SetStepOver( TBool aBoolean )
{
iStepOver = aBoolean;
}

//-----------------------------------------------------------------------------

TBool CCmdBase::StepOver( )
{
return iStepOver;
}

//-----------------------------------------------------------------------------
//
//  CCmdBase::ParamsL
//
//  Get command parameters = the remainder after the Keyword Phrase part in
//    command string. Leaves ((KErrArgument) if can not recogize the command.
//    This function accepts the Keywork Phrase if the non-space characters in
//    it match to those which are found from the beginning of command string.
//    Spaces (white space characters) are fully ignored in here i.e. you can
//    have spaces where you like and how may of then as you like; they simply
//    do not matter. The case does not matter either (folded comparison).
//
//    The next character after the Keyword Phrase must not be alphabetic nor
//    a digit. The end-of-string will do fine.
//
//    Return Values:  TPtrC = params (or Leaves)
//
//-----------------------------------------------------------------------------

TPtrC CCmdBase::ParamsL( const TDesC& aCommand )
{
// The actual beef is in the TLex version, see below.
TLex command( aCommand );
return ParamsL( command );
}

//-----------------------------------------------------------------------------

TPtrC CCmdBase::ParamsL( TLex aCommand )
{
// TLex for Keyphrase
TLex phrase( iKeyphrase );
// Mark start point
TLexMark mark;
aCommand.Mark( mark );

// First char in phrase
phrase.SkipSpace();

// Check whole phrase...
while ( !phrase.Eos() )
	{
	// Check the next true non-space character in command. The
	// end-of-command is not checked here. We rely on that the
	// Get() returns 0 if that is the case (and the 0 does not
	// match to the true character got from the phrase).
	aCommand.SkipSpace();
	if ( User::Fold(phrase.Get()) != User::Fold(aCommand.Get()) )
		{
		// No match: unget to the start point and "L E A V E".
		aCommand.UnGetToMark( mark );
		User::Leave( KErrArgument  );
		}

	// Next true non-space character in phrase.
	phrase.SkipSpace();
	}

// Apparently the keyword phrase was there: check that the next
// character is not alphabetic nor a decimal digit.
if ( !aCommand.Eos() && aCommand.Peek().IsAlphaDigit() )
	{
	// What a pitty. We were almost there but not quite. Unget
	// to the start point and "L E A V E".
	aCommand.UnGetToMark( mark );
	User::Leave( KErrArgument  );
	}

// Did it: return the remainder, which hopefully is just what
// we aimed at.
return aCommand.Remainder();
}

//-----------------------------------------------------------------------------
//
//    CCmdBase::Recognize
//
//    Recognize command string. Resets the StepOver flag from the command
//		family. If the family is not defined, then StepOver flag remains as
//    happens to be.
//
//    Return Values:  ETrue/EFalse
//
//-----------------------------------------------------------------------------

TBool CCmdBase::Recognize(const TDesC&  aCommand)
{
// Obey the command family's step over mode..
if (iFamily != NULL) 
	SetStepOver(iFamily->StepOver());

// The beef is in the ParamsL() function.
TPtrC params;
TRAPD(error, params.Set(ParamsL(aCommand)));
return (error == KErrNone);
}

//-----------------------------------------------------------------------------

TBool CCmdBase::Recognize(TLex aCommand)
{
// Reset the StepOver flag to what's in command family.
if (iFamily != NULL) 
	SetStepOver(iFamily->StepOver());

// The beef is in the ParamsL() function.
TPtrC params;
TRAPD(error, params.Set(ParamsL(aCommand)));
return (error == KErrNone);
}

//-----------------------------------------------------------------------------

TInt CCmdBase::ProcessL( const TDesC& aCommand )
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC params;
TRAPD( error, params.Set( ParamsL( aCommand )) );
return error;
}

//-----------------------------------------------------------------------------
//	Complete process without doing anything

TInt CCmdBase::ProcessStepOverL()
{
Machine()->CompleteRequest();
return KErrNone;
}

//-----------------------------------------------------------------------------

void CCmdBase::Printf( TRefByValue<const TDesC> aFmt, ... )
{
VA_LIST list;
VA_START(list, aFmt);
Machine()->MsgPrintfln( aFmt, list );
}

//-----------------------------------------------------------------------------

void CCmdBase::Print(const TDesC& aText, const TBool &aNewLine)
{
if (aNewLine)
	Machine()->MsgWriteln(aText);
else
	Machine()->MsgWrite(aText);
}

//-----------------------------------------------------------------------------

TInt CCmdBase::Error( TInt aError, TRefByValue<const TDesC> aFmt, ... )
{
VA_LIST list;
VA_START(list, aFmt);
Machine()->MsgPrintfln(aFmt, list);
return aError;
}   

//-----------------------------------------------------------------------------

CTEngine* CCmdBase::Machine()
{
return iFamily->Machine();
}

//-----------------------------------------------------------------------------

CConsoleBase* CCmdBase::Console()
{
return iFamily->Machine()->Console();
}

//-----------------------------------------------------------------------------
//	provide general log facility (access standard Machine features)

void CCmdBase::Log(TRefByValue<const TDesC> aFmt, ... )
{
VA_LIST list;
VA_START(list, aFmt);

Machine()->MsgPrintfln(aFmt, list);
//Printf(aFmt, list);
//if (Machine()->Log())
//	Machine()->LogPrintf(aFmt, list);
}

//-----------------------------------------------------------------------------
//	quick default to known 'invalid' date!

void CCmdBase::WriteDateStamp()
{
TDateTime myTempDate(K_OUTOFBOUNDS_YEAR, EJanuary, 0,0,0,0,0);
WriteDateStamp(myTempDate);
}
//TDateTime myTempDate;
//myTempDate.SetMonth(0);
//myTempDate.SetYear(K_OUTOFBOUNDS_YEAR);

//-----------------------------------------------------------------------------
//	displays the given date or current date depending on parameter
//buf.Format(_L8 ("%04d/%02d/%02d %02d.%02d:%02d:%06d "),dateTime.Year(),dateTime.Month()+1, dateTime.Day()+1,dateTime.Hour(),dateTime.Minute(),dateTime.Second(),dateTime.MicroSecond());
//buf.AppendFormat(_L8("%S\015\012"),&aText);

void CCmdBase::WriteDateStamp(const TDateTime &aDate)
{
TDateTime date;
TTime now;
TBool iEOL = (aDate.Year() == K_OUTOFBOUNDS_YEAR);

if (iEOL)
	{
	now.HomeTime();
	date = now.DateTime();
	}
else
	date = aDate;

TTime t(date);
TBuf<128> dateTimeString;
TRAPD(err, t.FormatL(dateTimeString, KDateFormat));
if (err == KErrNone)
	{
	if (iEOL)
		Machine()->MsgPrintf(_L("[%S] "), &dateTimeString);
	else
		Log(_L("[%S]\r\n"), &dateTimeString);
	}
} 

//-----------------------------------------------------------------------------
//	update the completion code and set the $ResultCode$ define in Machine()

void CCmdBase::SetCompletionCode(TInt aCode)
{
iCompletionCode = aCode;
Machine()->SetResultDefine(aCode);
}


//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------

