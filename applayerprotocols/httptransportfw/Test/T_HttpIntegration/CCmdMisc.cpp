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
// CCmdBase.cpp
// This file contains the implementation for miscellaneous CCmd*
// class' member functions. 
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Files  
// 
//

#include "CCmdMisc.h"

//-----------------------------------------------------------------------------

CCmdCase* CCmdCase::NewL( TInt aCommandId, const TDesC& aKeyphrase )
//	These functions allocate a CASE command object and construct it.
{
CCmdCase* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdCase* CCmdCase::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdCase* self = new (ELeave) CCmdCase();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

CCmdCase::~CCmdCase ( )
//	This function destructs the CASE command object.
{
}

//-----------------------------------------------------------------------------

void CCmdCase::ConstructL(TInt          aCommandId,   // in: command identifier
													const TDesC&  aKeyphrase )  // in: beginning of the command name
//	This function constructs the CASE command object.
{
CCmdBase::ConstructL(aCommandId, aKeyphrase);
}

//-----------------------------------------------------------------------------

TBool CCmdCase::Recognize(const TDesC& aCommand)
//	This function overrides the CCmdBase::Recognize. 
{
// Recognize the command and extract the parameter=description.
TPtrC param;
TRAPD(ret, param.Set(ParamsL(aCommand)));
if (ret != KErrNone) 
	return EFalse;

// Check if this case was selected and set the CASE's StepOver
// flag and the command family's StepOver accordingly.
param.Set(TfrLex::Trim( param));
CCmdSelect *select = (CCmdSelect*)Family()->Command(ECmdSelect);
TBool selected = (select == NULL || select->Selected(param));
SetStepOver(!selected);
Family()->SetStepOver(!selected);
return ETrue;
}

//-----------------------------------------------------------------------------

TInt CCmdCase::ProcessL(const TDesC& aCommand)      // command line
//	This function parses and processes a CASE command line. As a matter of
//	fact, processing the CASE command does nothing (but see the Recognize).
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD( ret, param.Set(ParamsL(aCommand)));
if (ret != KErrNone) 
	return Error(ret, KFmtErrInvalidCmd, &Keyphrase());
return KErrNone;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// CCmdEndCase
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdEndCase* CCmdEndCase::NewL( TInt aCommandId, const TDesC& aKeyphrase )
//	These functions allocate a END CASE command object and construct it.
{
CCmdEndCase* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdEndCase* CCmdEndCase::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdEndCase* self = new (ELeave) CCmdEndCase();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

CCmdEndCase::~CCmdEndCase()
{
}

//-----------------------------------------------------------------------------

void CCmdEndCase::ConstructL(TInt aCommandId,   // in: command identifier
														 const TDesC &aKeyphrase)  // in: beginning of the command name
{
CCmdBase::ConstructL(aCommandId, aKeyphrase);
}

//-----------------------------------------------------------------------------

TBool CCmdEndCase::Recognize( const TDesC& aCommand  )
//	This function overrides the CCmdBase::Recognize
{
// Recognize the command and extract the remainder (is ignored).
TPtrC param;
TRAPD( ret, param.Set( ParamsL( aCommand )) );
if ( ret != KErrNone ) 
	return EFalse;
// Set this END CASE command's StepOver as in command family
SetStepOver( Family()->StepOver() );
// Reset the command family's StepOver flag back to false i.e.
// commands will be processed starting from the next one.
Family()->SetStepOver( EFalse );
return ETrue;
}

//-----------------------------------------------------------------------------

TInt CCmdEndCase::ProcessL ( const TDesC& aCommand)      // command line
//	This function parses and processes a END CASE command line. As a matter
//	of fact, processing the END CASE command in here does nothing (but see
//	the Recognize).
{
// Complete the test machine - will then get the next cmd
Machine()->CompleteRequest();

TPtrC param;
TRAPD( ret, param.Set( ParamsL( aCommand )) );
if ( ret != KErrNone ) return Error(ret, KFmtErrInvalidCmd, &Keyphrase());
return ret; // = KErrNone
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// CCmdSelect
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CCmdSelect* CCmdSelect::NewL( TInt aCommandId, const TDesC& aKeyphrase )
//	These functions allocate a CCmdSelect command object and construct it.
{
CCmdSelect* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdSelect* CCmdSelect::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdSelect* self = new (ELeave) CCmdSelect();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}

//-----------------------------------------------------------------------------

CCmdSelect::~CCmdSelect()
{
delete iSelector;
iSelector = NULL;
}

//-----------------------------------------------------------------------------

void CCmdSelect::ConstructL(TInt          aCommandId,   // in: command identifier
														const TDesC&  aKeyphrase )  // in: beginning of the command name
//	This function constructs the CCmdSelect command object.
{
CCmdBase::ConstructL(aCommandId, aKeyphrase);
iSelector = NULL; // This means SELECT ALL!
}

//-----------------------------------------------------------------------------

TInt CCmdSelect::ProcessL(const TDesC& aCommand)      // command line
//	This function parses and processes a CCmdSelect command line
{
// Complete the test machine
Machine()->CompleteRequest();

TPtrC param;
TRAPD(ret, param.Set(ParamsL(aCommand)));
if (ret != KErrNone)
	return Error(ret, TFR_KFmtErrBadCmd, &Keyphrase());

if (Family()->Switch(CCmdFamily::EParseOnly))
	return ret;

// This is the selector string.
param.Set(TfrLex::Trim(param));

// Delete old selector if any and make a new one.
if (iSelector != NULL)	
	delete iSelector;

iSelector = NULL; // This means ALL.
if (param.Length() > 0)
	{
	iSelector = param.Alloc();
	if (iSelector == NULL)
		{
		ret = KErrNoMemory;
		Printf(TFR_KFmtErrFailed, &Keyphrase(), ret);
		}
	}

return ret;
}

//-----------------------------------------------------------------------------

TPtrC CCmdSelect::Selector() const
//	This function gives the present selector
{
TPtrC selector = iSelector->Des();
return selector;
}

//-----------------------------------------------------------------------------

TBool CCmdSelect::Selected(const TDesC &aCandidate)
//	This function checks if a candidate is selected.
{
// Having no selector means select ALL.
if ( iSelector == NULL ) 
	return ETrue;

// Having empty selector means select NONE.
if ( iSelector->Length() == 0 ) 
	return EFalse;

// Selectors ALL and NONE are accepted as well.
if ( iSelector->Des().CompareF( _L("ALL"))  == 0 ) 
	return ETrue;
if ( iSelector->Des().CompareF( _L("NONE")) == 0 ) 
	return EFalse;

// Otherwise any match will do.
return (aCandidate.MatchF( iSelector->Des() ) != KErrNotFound);
}

//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------
