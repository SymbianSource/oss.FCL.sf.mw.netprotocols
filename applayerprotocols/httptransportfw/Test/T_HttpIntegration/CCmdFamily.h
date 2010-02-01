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
// This module implements the CCmdFamily class: a generic collection of
// registered CCmdBase class derived commands.
// Include Files  
// 
//

#ifndef __CMDFAMILY_H__
#define __CMDFAMILY_H__

//----------------------------------------------------------------------------
//	Forward declarations

class CTEngine;
class CCmdBase;

//----------------------------------------------------------------------------
//  Definitions

class CCmdFamily : public CBase
{

public:     // Data types

	// Switches. Please, see the iSwitch.
	enum TSwitch
		{
		EParseOnly,		// Parse only
		ESwitch2,			// For future use
		ESwitch3,
		ESwitch4,
		ESwitch5,
		ESwitch6,
		ESwitch7,
		ESwitch8,
		EMaxSwitches
		};

public:     // Constructors and destructors

	// Constructors.
	static CCmdFamily* NewL ( );
	static CCmdFamily* NewLC( );

	//  Destructor.
	~CCmdFamily();

private:

	// Local Constructor.
	void ConstructL( );

protected:

	// No default constructor.
	CCmdFamily( ) {};

public:     // New functions

	// Set and Get Test Machine context.
	void SetMachine(CTEngine *aTestMachine );
	CTEngine * Machine( );

	// Register a Command.
	void RegisterL ( CCmdBase* aCommand );

	// Get Count of Commands.
	TInt Count( ) const;

	// Get i'th Command. Warning: legal index is a must.
	CCmdBase*  At( TInt aIndex ) const;
	CCmdBase*  operator[]( TInt aIndex ) const;

	// Get Command by command id (NULL if was not found).
	CCmdBase* Command(TInt aCommandId);

	// Get Command by command string (NULL if not found).
	CCmdBase* Recognize(const TDesC& aCommand);

	// Set a Get a switch.
	TBool Switch(const TSwitch &);
	void  SetSwitch(const TSwitch &aSwitch, TBool aBoolean = ETrue);

	// Set and Get the StepOver flag.
	void  SetStepOver(const TBool &aBoolean = ETrue);
	TBool StepOver( );

	//	List all the commands out
	void ListAll(CConsoleBase *);

private:    // Data

	// Tester Context.
	CTEngine *iMachine;

	// Registered Commands.
	CArrayPtrSeg<CCmdBase>* iCommands;

	// Switches. Must be long enough for all TSwitch values.
	TBool iSwitch[EMaxSwitches];

	// Step over us.
	TBool iStepOver;

};

//----------------------------------------------------------------------------

#endif  // __CMDFAMILY_H__  

//----------------------------------------------------------------------------
// End of File
//----------------------------------------------------------------------------
