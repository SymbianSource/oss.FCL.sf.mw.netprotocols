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
// Contains implementation of CTestMatchUriTypeStep class
// @internalAll
// 
//

// User Include
#include "testmatchuritypestep.h"

/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestMatchUriTypeStep::CTestMatchUriTypeStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestMatchUriTypeStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestMatchUriTypeStep::~CTestMatchUriTypeStep()
	{

	}
/**
Tests the matching functionality (matching all uris of specific listtype/servicetype).
It reads values from INI file and does matching uris in DB against ListType/ServiceType.
@internalTechnology 
@test
@param		None
@return		EPass or EFail indicating the success or failure of the test step
*/
TVerdict CTestMatchUriTypeStep::doTestStepL()
	{
	__UHEAP_MARK;
	INFO_PRINTF1(_L("\n"));
	// Get necessary information from INI file

	TInt listType;
	TInt serviceType;
	TInt expRetCode;
	TInt expMatchCount;
	
	if(!GetIntFromConfig(ConfigSection(), 	KIniListType, listType) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniServiceType, serviceType) ||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedRetCode, expRetCode) ||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedMatchCount, 	expMatchCount)
	  )
		{
		ERR_PRINTF5(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n%S\n"
					  ),&KIniListType, &KIniServiceType, &KIniExpectedRetCode, &KIniExpectedMatchCount
				   );
		SetTestStepResult(EFail);
		}
	else
		{// No problem in reading values from INI file. Proceed.
	
		TInt err;
		if(listType == 0)
			{
			if(serviceType == 0)
				{
				TQueryArgs args0;
		    	TRAP(err, iUriDb.QueryUriL(args0, this));	
				}
			else
				{
				TQueryArgs args1 ( (InetUriList::TServiceType)serviceType);
		    	TRAP(err, iUriDb.QueryUriL(args1, this));	
				}
			}
    	else
	    	{
	    	if(serviceType == 0)
				{
				TQueryArgs queryArgs1 ((InetUriList::TListType)listType );
	    		TRAP(err, iUriDb.QueryUriL(queryArgs1, this));
				}
			else
				{
				TQueryArgs args2 ( (InetUriList::TServiceType)serviceType, (InetUriList::TListType)listType );
	    		TRAP(err, iUriDb.QueryUriL(args2, this));
	    		}
	    	}
    
	    // Checking with expected result (positive/negative test case).
	    if(err != expRetCode)
	        {
	        ERR_PRINTF2(_L("Test step failed: RInetUriList::MatchUriL failed: %D\n"), err);
		    SetTestStepResult(EFail);
		    }
	    else
		    {
		    if(err == KErrNone && expMatchCount != iQueryResults.Count())
			    {
			    ERR_PRINTF3(_L("Test step failed: Actual[%D] & Expected[%D] count didn't match.\n"),iQueryResults.Count(),expMatchCount);
			    SetTestStepResult(EFail);
			    }
			 else
			 	{
			 	INFO_PRINTF1(_L("Test step passed: Match succeeded.\n")); 		
			 	}
				
		    }
	    ClearUriArray();
		}
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL



