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
// Contains implementation of CTestCopyPreConfigXmlStep class
// @internalAll
// 
//

// User Include
#include "testcopypreconfigxmlstep.h"

/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestCopyPreConfigXmlStep::CTestCopyPreConfigXmlStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestCopyPreConfigXmlStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestCopyPreConfigXmlStep::~CTestCopyPreConfigXmlStep()
	{

	}
	
/**
Calls DoOldDbCopyL() which copies ineturilist.xml from z: to c:\private\20009D70\ineturilist.xml
@internalTechnology
@test
*/	
TVerdict CTestCopyPreConfigXmlStep::doTestStepL()
	{
	DoOldDbCopyL();
	return TestStepResult();
	}	// doTestStepL
	
/**
Copies ineturilist.xml from z:\ineturilist\testdata\ineturilist.xml to c:\private\20009D70\ineturilist.xml
@internalTechnology
@test
*/
void CTestCopyPreConfigXmlStep::DoOldDbCopyL()
	{
	RFs fs;
	User::LeaveIfError ( fs.Connect () );		
	CFileMan* fileMan = CFileMan::NewL ( fs );
	CleanupStack::PushL ( fileMan );
	fs.MkDirAll ( KDestinationPath );		
	User::LeaveIfError ( fileMan->Copy ( KSourcePath, KDestinationPath ) );						
	CleanupStack::PopAndDestroy ( fileMan );
	fs.Close ();		
	}
