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
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// This module implements the CCmdLog (log mode) command class.
// 
//

#include "CCmdLog.h"

//-----------------------------------------------------------------------------

CCmdLog *CCmdLog::NewL(TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdLog *self = NewLC( aCommandId, aKeyphrase, aHelpPhrase);
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdLog *CCmdLog::NewLC( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdLog *self = new (ELeave) CCmdLog();
CleanupStack::PushL( self );
self->ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
return self;
}

//-----------------------------------------------------------------------------

void CCmdLog::ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase)
{
CCmdBase::ConstructL(aCommandId, aKeyphrase, aHelpPhrase);
}

//-----------------------------------------------------------------------------
//	LOG sets controls on logging of input/output
//
//	LOG ALL sets comments, commands and cases to ON
//	LOG NONE resets all comment options to OFF
//	LOG [COMMENTS | COMMANDS | CASES] will enable the appropriate subclass

TInt CCmdLog::ProcessL( const TDesC& aCommand )
{
TPtrC param;
TRAPD( error, param.Set(ParamsL(aCommand)));
if ( error != KErrNone )
	return Error(error, KFmtErrInvalidCmd, &Keyphrase());

TBool logComments = EFalse;
TBool logCommands = EFalse;
TBool logCases    = EFalse;    
param.Set( TfrLex::Trim( param ) );
if ( param.CompareF(KTxtLogALL) == 0 )
	{
	logComments = ETrue;
	logCommands = ETrue;
	logCases    = ETrue;
	}
else if (param.CompareF(KTxtLogNONE) == 0)
	{
	logComments = EFalse;
	logCommands = EFalse;
	logCases    = EFalse;
	}
else if (param.Length() == 0)
	error = KErrArgument;
else
	{
	TLex parse(param);
	parse.SkipSpace();
	while (!parse.Eos())
		{
		if (TfrLex::ValF(parse,KTxtLogCOMMENTS) == KErrNone)
			logComments  = ETrue;
		else if (TfrLex::ValF(parse,KTxtLogCOMMANDS) == KErrNone)
			logCommands  = ETrue;
		else if (TfrLex::ValF(parse,KTxtLogCASES) == KErrNone)
			logCases = ETrue;
		else
			{
			error = KErrArgument;
			break;
			}

		parse.SkipSpace();

		if ( !parse.Eos() && parse.Get() != ',' )
			{
			error = KErrArgument;
			break;
			}

		parse.SkipSpace();
		}
	}

if ( error != KErrNone )
	return Error(error, KFmtErrInvalidCmd, &Keyphrase());

//	now set the flag in each command case
TInt i;
CCmdBase* cmd;
for ( i = 0; i < Family()->Count(); i++ )
	{
	cmd = Family()->At(i);
	if (cmd->CommandId() == THA_KCmdRemark )
		cmd->SetFlag(CCmdBase::EDoLog, logComments);

	else if (cmd->CommandId() == ECase )
		cmd->SetFlag(CCmdBase::EDoLog, logCases);

	else
		cmd->SetFlag(CCmdBase::EDoLog, logCommands);
	}

return error;
}

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------

