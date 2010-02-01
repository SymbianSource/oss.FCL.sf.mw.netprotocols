// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contains implementation of CTestRemoveDBStep class
// @internalAll
// 
//

// User Include
#include "testremovedbstep.h"

/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestRemoveDBStep::CTestRemoveDBStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestRemoveDBStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestRemoveDBStep::~CTestRemoveDBStep()
	{

	}
	
/**
Calls DoOldDbCopyL() which copies ineturilist.xml from z: to c:\private\20009D70\ineturilist.xml
@internalTechnology
@test
*/	
TVerdict CTestRemoveDBStep::doTestStepL()
	{
	RemoveDBL();
	return TestStepResult();
	}	// doTestStepL
	
/**
Copies ineturilist.xml from z:\ineturilist\testdata\ineturilist.xml to c:\private\20009D70\ineturilist.xml
@internalTechnology
@test
*/
void CTestRemoveDBStep::RemoveDBL()
	{
	RFs fs;
	User::LeaveIfError ( fs.Connect () );		
	fs.Delete(KOldDB);
	fs.Close ();		
	}
