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
// Contains implementation of CTestMatchUriStep class
// @internalAll
// 
//

// User Include
#include "testmatchuristep.h"
// System Include
#include <uri8.h>



/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestMatchUriStep::CTestMatchUriStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestMatchUriStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestMatchUriStep::~CTestMatchUriStep()
	{
	}
	
/**
Tests the match functionality(matching a specific uri with service-type/list-type).
It reads values from INI file and does matching uris in DB against a specific uri and ListType/ServiceType.
@internalTechnology 
@test
@param		None
@return		EPass or EFail indicating the success or failure of the test step
*/
TVerdict CTestMatchUriStep::doTestStepL()
	{
	__UHEAP_MARK;
	INFO_PRINTF1(_L("\n"));
	// Get necessary information from INI file
	TPtrC uri;
	TInt listType;
	TInt serviceType;
	TInt matchType;
	TInt expRetCode;
	TInt expMatchCount;
	
	if(!GetStringFromConfig(ConfigSection(), 	KIniUri, uri) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniListType, listType) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniServiceType, serviceType) ||
	   !GetIntFromConfig(ConfigSection(), KIniMatchType, matchType) ||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedRetCode, expRetCode) ||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedMatchCount, 	expMatchCount)
	  )
		{
		ERR_PRINTF7(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n%S\n%S\n%S\n"
					  ),&KIniUri, &KIniListType, &KIniServiceType, &KIniMatchType, &KIniExpectedRetCode, &KIniExpectedMatchCount
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		// No problem in reading values from INI file. Proceed.
		HBufC8* uriBuf = HBufC8::NewLC(uri.Length());
		TPtr8 uri8Bit = uriBuf->Des();
		uri8Bit.Copy(uri);
		TInt err;
		if(listType == 0)
			{
			if(serviceType == 0)
				{
				TQueryArgs args2 ( &uri8Bit, (InetUriList::TURIMatch)matchType);
	    		TRAP(err, iUriDb.QueryUriL(args2, this, this));	
				}
			else
				{
				TQueryArgs args3 ( &uri8Bit, (InetUriList::TServiceType)serviceType, (InetUriList::TURIMatch)matchType);
	    		TRAP(err, iUriDb.QueryUriL(args3, this, this));	
				}
			
			}
    	else
	    	{
	    	if(serviceType == 0)
				{
				TQueryArgs queryArgs3 ( &uri8Bit, (InetUriList::TListType)listType, (InetUriList::TURIMatch)matchType);
    			TRAP(err, iUriDb.QueryUriL(queryArgs3, this, this));
				}
				else
				{
				TQueryArgs args4 ( &uri8Bit, (InetUriList::TServiceType)serviceType, (InetUriList::TListType)listType, (InetUriList::TURIMatch)matchType);
    			TRAP(err, iUriDb.QueryUriL(args4, this, this));	
				}
	    	
			}
    
		
		CleanupStack::PopAndDestroy(); // uriBuf
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
				ERR_PRINTF3(_L("Test step failed: Actual [%D] and expected [%D] match count didn't match\n"),iQueryResults.Count(),expMatchCount);
	        	SetTestStepResult(EFail);
				}
			else
				{
				INFO_PRINTF1(_L("Test step passed: Match succeeded.\n"));
				}	
			}
        ClearUriArray();
		}
	iUriProps.Close(); // Cleans up the URI properties object.
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL
