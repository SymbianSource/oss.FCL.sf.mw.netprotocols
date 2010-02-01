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
// 
//

#ifndef __CCMDBASE_H__
#define __CCMDBASE_H__

//-----------------------------------------------------------------------------
//  Include Files  

#include <e32base.h>
#include <uri8.h>

//-----------------------------------------------------------------------------

#include "CCmdFamily.h"
#include "TEngine.h"
#include "TfrLex.h"

//-----------------------------------------------------------------------------

class CCmdBase : public CBase
{
public:     // Data Types

	// Boolean flags 1-8 from the 1-32 are reserved for the framework's
	// use, present or future.
	enum
		{
		EDoLog  = 1,    // print to log
		EFlag2  = 2,
		EFlag3  = 3,
		EFlag4  = 4,
		EFlag5  = 5,
		EFlag6  = 6,
		EFlag7  = 7,
		EFlag8  = 8,
		EMaxFlags
		};

public:     // Constructors and destructors

	// Constructors. Have your own ones in the derived classes.
	static CCmdBase* NewL (TInt, const TDesC &);
	static CCmdBase* NewLC(TInt, const TDesC &);
	static CCmdBase* NewL (TInt, const TDesC &, const TDesC &);
	static CCmdBase* NewLC(TInt, const TDesC &, const TDesC &);

	// Destructor.
	~CCmdBase();

protected:

	// Local constructor (is protected for the derived classes).
	virtual void ConstructL(TInt, const TDesC &);
	virtual void ConstructL(TInt, const TDesC &, const TDesC &);
	
	//	have a wait event method (which may or may not be inherited)
	virtual void WaitEvents() {};

	//	have a wait event method (which may or may not be inherited)
	virtual void DoneWaitEvents() {};

	//	this provides a generic means of updating the completion code
	virtual void SetCompletionCode(TInt);
	virtual TInt GetCompletionCode() { return iCompletionCode; }

protected:

	// No default constructor.
	CCmdBase();

public:     // New functions

	// Get the Command Id.
	TInt CommandId();

	// Get the Keyword Phrase.
	const TDesC &Keyphrase();

	// Set and Get the Command Family context.
	void SetFamily(CCmdFamily *);
	CCmdFamily *Family();

	// Set and Get the Help Text.
	void SetHelpText(const TDesC &);
	const TDesC& HelpText();

	// Set and Get a boolean flag, flags 1-32 supported.
	void  SetFlag(TInt, TBool = ETrue);
	TBool Flag(TInt);

	// Set and Get the StepOver flag.
	void  SetStepOver(TBool = ETrue);
	TBool StepOver();

	// Recognize command i.e. check that the command string begins with
	// the Keyword Phrase AND that the next after the keyword phrase is
	// not alphabetic nor digit (end-of-string is accepted). NOTE: this
	// sets the command's StepOver flag from the command family.
	virtual TBool Recognize(const TDesC & );
	virtual TBool Recognize(TLex);

	// Get command parameters = the remainder after the Keyword Phrase
	// in the command string. Leaves (KErrArgument) if cannot recogize
	// the command.
	virtual TPtrC ParamsL(const TDesC &);
	virtual TPtrC ParamsL(TLex aCommand);

	// Process command string.
	// Override...
	virtual TInt ProcessL(const TDesC &);

	// Process command string.
	// Override...
	TInt ProcessStepOverL();

	//	simplify logging facility
	void Log(TRefByValue<const TDesC> aFmt, ... );

	//	WriteDateStamp
	void WriteDateStamp();
	void WriteDateStamp(const TDateTime &);

protected:  // New functions

	// Print out formatted/unformatted message.
	void Printf(TRefByValue<const TDesC> aFmt, ...);
	void Print (const TDesC& aText, const TBool & = ETrue);

	// Raise error: returns back the given error code.
	TInt Error( TInt aError, TRefByValue<const TDesC> aFmt, ... );

	// Get Test Machine.
	CTEngine *Machine();

	// Get Test Console.
	CConsoleBase* Console();

protected:  // Data

	// Command Flags, a bit for each flag numbered from 1 to 32.
	TUint32 iFlags;

private:    // Data

	// Command Family.
	CCmdFamily* iFamily;

	// Command id.
	TInt iCommandId;

	// Keyword phrase.
	TPtrC iKeyphrase;

	// Help text.
	TPtrC iHelpText;

	// Step over me.
	TBool iStepOver;

	//	operation completion code
	TInt iCompletionCode;

};

/*
//
// The constuction of CCmdBase derived classes goes as outlined below:
//

class CCmdSample : public CCmdBase
{
public:
static CCmdSample* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
static CCmdSample* NewLC( TInt aCommandId, const TDesC& aKeyphrase );
~CCmdSample( );
protected:
void ConstructL( TInt aCommandId, const TDesC& aKeyphrase );
public:
TInt ProcessL( const TDesC& aCommand, TBool doExecute = ETrue );
...
};

CCmdSample* CCmdSample::NewL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdSample* self = NewLC( aCommandId, aKeyphrase );
CleanupStack::Pop();
return self; 
}
CCmdSample* CCmdSample::NewLC( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdsample* self = new (ELeave) CCmdSample();
CleanupStack::PushL( self );
self->ConstructL( aCommandId, aKeyphrase );
return self;
}
CCmdSample::~CCmdSample( )
{
...destroy additional member data in here.
};
void CCmdSample::ConstructL( TInt aCommandId, const TDesC& aKeyphrase )
{
CCmdBase::ConstructL ( aCommandId, aKeyphrase );
...initialize additional member data in here.
}

*/


#endif  // __CCMDBASE_H__  

//-----------------------------------------------------------------------------
//	End of File
//-----------------------------------------------------------------------------
