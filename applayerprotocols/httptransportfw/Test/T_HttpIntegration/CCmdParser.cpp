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
// GT0149 Applications Protocol Integration Test Harness
// blank application
// This file contains the implementation for CCmdPar class' member 
// functions. CCmdPar is an abstract base class for deriving different
// SET command classes.  CCmdPar contains parsing of SET commands.
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Files  
// 
//

#include "CCmdParser.h"

//-----------------------------------------------------------------------------

CCmdPar::~CCmdPar ( )
{
}

//-----------------------------------------------------------------------------

void CCmdPar::ConstructL(TInt aCommandId,   // in: command identifier
												const TDesC&  aKeyphrase )  // in: beginning of the command name
{
CCmdBase::ConstructL(aCommandId, aKeyphrase);
}

//-----------------------------------------------------------------------------
//	This function parses and processes a SET command line

TInt CCmdPar::ProcessL( const TDesC& aCommand)      // command line
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD( ret, param.Set( ParamsL( aCommand )) );
if ( ret != KErrNone )
	return Error(ret, KFmtErrInvalidCmd, &Keyphrase());

// Mark that the value is set
TInt old = iHasBeenSet;
iHasBeenSet = ETrue;

// Check that "=" or "+=" or ":=" follows
TBool assignment = ETrue;
TBool is_setfrom = EFalse;

TLex parse(param);
parse.SkipSpace();
if (TfrLex::ValF(parse, KTxtAssign) == KErrNone)
	assignment = ETrue;
else if (TfrLex::ValF(parse, KTxtAddition) == KErrNone)
	assignment = EFalse;
else if (TfrLex::ValF(parse, KTxtSetFrom) == KErrNone)
	assignment = is_setfrom = ETrue;
else if (parse.TokenLength() == 0)
	ret = Error(KErrArgument, KFmtErrInvalidCmd, &Keyphrase());
else
	ret = Error(KErrArgument, KFmtErrMissing, &KTxtAssign);

// Check that the value exists and set or add the value
if (ret == KErrNone)
	{
	parse.SkipSpace();
	TPtrC token = parse.Remainder(); 
	if (token.Length() == 0)
		ret = Error(KErrArgument, KTxtErrValueMissing);
	else if (assignment)
		ret = (is_setfrom ? SetFrom(token) : SetValue(token));
	else
		ret = AddValue( token );
	}

// If not succeeded, reset information about setting
if (ret != KErrNone)
	iHasBeenSet = old;

return ret;
}

//-----------------------------------------------------------------------------
//	This function provides the default implementation which gives an 
//	error message about addition

TInt CCmdPar::AddValue(const TDesC& aValue)
{
aValue.Length();  // to avoid warning about unused parameter
return Error(KErrArgument, KTxtErrNoAddition); 
}

//-----------------------------------------------------------------------------
//	This function provides the default implementation which gives an 
//	error message about setting the value from file. +TVi

TInt CCmdPar::SetFrom(const TDesC& aValue)
{
aValue.Length();  // to avoid warning about unused parameter
return Error(KErrArgument, KTxtErrNoSetFrom); 
}

//-----------------------------------------------------------------------------

TBool CCmdPar::HasBeenSet ( )
{
return iHasBeenSet;
}

//-----------------------------------------------------------------------------

void CCmdPar::MarkNotSet ( )
{
iHasBeenSet = EFalse;
}

//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------

