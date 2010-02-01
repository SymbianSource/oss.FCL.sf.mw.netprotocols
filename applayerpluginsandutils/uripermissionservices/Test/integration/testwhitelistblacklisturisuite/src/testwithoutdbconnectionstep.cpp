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
// Contains implementation of CTestWithOutDBConnectionStep class
// @internalAll
// 
//

// User Include
#include "testwithoutdbconnectionstep.h"

	
/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestWithOutDBConnectionStep::CTestWithOutDBConnectionStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestWithOutDBConnectionStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestWithOutDBConnectionStep::~CTestWithOutDBConnectionStep()
	{

	}

/**
This is to test an operation without opening (creating session) database.
This is the implementation of a negative test case. 
It tests that an operation without opening/establishing connection/creating session with database should fail.
Here it calls MatchUriL() with out opening DB connection.
@internalTechnology 
@test
@param		None
@return		EPass or EFail indicating the success or failure of the test step
*/
TVerdict CTestWithOutDBConnectionStep::doTestStepL()
	{
	__UHEAP_MARK;
	INFO_PRINTF1(_L("\n"));
	// Get necessary information from INI file

	TInt listType;
	TInt serviceType;
	TInt expRetCode;
	
	if(!GetIntFromConfig(ConfigSection(), 	KIniListType, listType) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniServiceType, serviceType)||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedRetCode, expRetCode)
	  )
		{
		ERR_PRINTF4(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n"
					  ),&KIniListType, &KIniServiceType, &KIniExpectedRetCode
				   );
		SetTestStepResult(EFail);
		}
	else
		{// No problem in reading values from INI file. Proceed.
		
	    // Here closing the DB connection which is opened by base class method CTestWListBListUriBaseStep::doTestStepPreambleL()
	    iUriDb.Close();
	    // Calling QueryL() method with out opening DB connection.
	    TQueryArgs args2 ((InetUriList::TServiceType)serviceType,(InetUriList::TListType)listType);  
    	TRAPD(err, iUriDb.QueryUriL(args2, this));
    
	    // Ideally it should fail, here KErrNone means there is some thing wrong.
	    if(err == expRetCode)
	    	{
	    	INFO_PRINTF1(_L("Test step passed: Database dont perform any operation without opening it.\n"));
	    	}
	    else
	        {
	    	ERR_PRINTF1(_L("Test step failed: RInetUriList::MatchUriL passed but It should fail: %D\n"));
			SetTestStepResult(EFail);
			}  
    	ClearUriArray();
		}
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL


