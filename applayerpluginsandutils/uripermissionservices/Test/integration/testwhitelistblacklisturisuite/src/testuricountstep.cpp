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
// Contains implementation of CTestUriCountStep class
// @internalAll
// 
//

// User Include
#include "testuricountstep.h"

/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestUriCountStep::CTestUriCountStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestUriCountStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestUriCountStep::~CTestUriCountStep()
	{
	}

	
/**
Tests the count functionality( finds the count of all matched uris of specific servicetype/list-type
and check that returned count is correct if it is positive and non-capability test.
It reads values from INI file and gets the count of matching URIs of specific service/list type from database
@internalTechnology
@test 
@param		None
@return		EPass or EFail indicating the success or failure of the test step
*/
TVerdict CTestUriCountStep::doTestStepL()
	{
	__UHEAP_MARK;
	INFO_PRINTF1(_L("\n"));
	// Get necessary information from INI file

	TInt listType;
	TInt serviceType;
	TInt expCountOrErrCode;
	TInt actCountOrErrCode;
	TBool isCapabilityTest;
	
	if(!GetIntFromConfig(ConfigSection(), 	KIniListType, listType) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniServiceType, serviceType) ||
	   !GetIntFromConfig(ConfigSection(), KIniExpCountOrErrCode, expCountOrErrCode) ||
	   !GetBoolFromConfig(ConfigSection(), KIniIsCapabilityTest, isCapabilityTest) 
	  )
		{
		ERR_PRINTF5(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n%S\n"
					  ),&KIniListType, &KIniServiceType, &KIniExpCountOrErrCode, isCapabilityTest
				   );
		SetTestStepResult(EFail);
		}
	else
		{// No problem in reading values from INI file. Proceed.
		
        //Calling Count() to get the count of matched uris.
	    actCountOrErrCode=iUriDb.Count((InetUriList::TServiceType)serviceType,(InetUriList::TListType)listType);
	    //Checking actual count with expected count.
	    if(actCountOrErrCode != expCountOrErrCode)
	    	{
	    	ERR_PRINTF3(_L("Test step failed: Actual [%D] and Expected return value [%D] didn't match."),actCountOrErrCode,expCountOrErrCode);
			SetTestStepResult(EFail);
			}
		else
			{
			INFO_PRINTF1(_L("Actual and Expected return value matched.\n"));
			}
		
		//  Checking whether that count is correct.
        //  This is executed only for positive(count>0) and non-capability tests.
	    if(expCountOrErrCode>0 && !isCapabilityTest)
			{
			CheckUriCount(listType,serviceType,expCountOrErrCode);	
			}
				
		}
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL

/*
This is to check that match count is correct.	
@internalTechnology
@test 
@param aListType  		List type of given URI
@param aServiceType  	Service type of given URI
@param aExpCount  		expected match count
@return		None
*/
void CTestUriCountStep::CheckUriCount(const TInt aListType, const TInt aServiceType,const TInt aExpCount)
	{
	INFO_PRINTF1(_L("Count verification: verification started\n"));
	TQueryArgs args2 ( (InetUriList::TServiceType)aServiceType,(InetUriList::TListType)aListType); 
    TRAPD(err, iUriDb.QueryUriL(args2, this));
    
    if(err != KErrNone)
    	{
	    ERR_PRINTF1(_L("Count verification failed: Leave occured in QueryUriL \n"));
	    SetTestStepResult(EFail);
	    }
   else if(aExpCount != iQueryResults.Count())
	    { // Checking with expected match count.
	    ERR_PRINTF3(_L("Count verification failed: actCount [%D] and expCount [%D] didn't match\n"),iQueryResults.Count(),aExpCount);
	    SetTestStepResult(EFail);
	    }
	else
		{
		INFO_PRINTF1(_L("Count verification: actCount and expCount matched.\n"));
		}
	ClearUriArray();
	}

