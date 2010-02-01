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
// Contains implementation of CTestRemoveUriStep class
// @internalAll
// 
//

// User Include
#include "testremoveuristep.h"

	
/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestRemoveUriStep::CTestRemoveUriStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestRemoveUriStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestRemoveUriStep::~CTestRemoveUriStep()
	{
	}
	
/**
Tests the removing fuctionality by removing an uri and verify that it is removed successfully if it is a positive and non-capability test.
It reads values from INI file and removes corresponding uri from DB.
@internalTechnology 
@test
@param		None
@return		EPass or EFail indicating the success or failure of the test step
*/
TVerdict CTestRemoveUriStep::doTestStepL()
	{
	__UHEAP_MARK;
	INFO_PRINTF1(_L("\n"));
	// Get necessary information from INI file
	TPtrC uri;
	TInt listType;
	TInt serviceType;
	TInt expRetCode;
	TBool isCapabilityTest;
	
	if(!GetStringFromConfig(ConfigSection(), 	KIniUri, uri) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniListType, listType) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniServiceType, serviceType) ||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedRetCode, 	expRetCode) ||
	   !GetBoolFromConfig(ConfigSection(), KIniIsCapabilityTest, isCapabilityTest) 
	  )
		{
		ERR_PRINTF6(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n%S\n%S\n"
					  ),&KIniUri, &KIniListType, &KIniServiceType, &KIniExpectedRetCode, &isCapabilityTest
				   );
		SetTestStepResult(EFail);
		}
	else
		{// No problem in reading values from INI file. Proceed.		
		TRAPD(err, RemoveAndCheckUriL(uri, serviceType, listType, expRetCode, isCapabilityTest));
    	if(err != KErrNone)
	        {
      	    ERR_PRINTF2(_L("Test step failed: Leave occured in CTestRemoveUriStep::RemoveAndCheckUriL: %D\n"), err);
       	    SetTestStepResult(EFail);
		    }
		}
	iUriProps.Close(); // Cleans up the URI properties object.
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL
	
	
/**
Removes an uri with given properties and call isRemoved() 
to check that the uri with specified properties has been removed successfully.
@internalTechnology 
@test
@param aUri  				A non-modifiable object of TUriC8
@param aServiceType  		Service type of given URI
@param aExpRetCode          Error code i.e. success/failure code
@param aIsCapabilityTest    Capability test if ETrue else EFalse
@return		None
*/
void CTestRemoveUriStep::RemoveAndCheckUriL(const TDesC& aUri, TInt aServiceType, TInt aListType, TInt aExpRetCode, const TBool aIsCapabilityTest)
	{
	TInt err;
	if(!aIsCapabilityTest)	
		TRAP(err, FetchL(aUri, aServiceType, KNullDesC));
	if(aIsCapabilityTest || err != KErrNone || aExpRetCode == InetUriList::KErrUriNotFound)
		{
		iUriProps.Close();
		CreateL(aUri, aListType, aServiceType, KNullDesC);
		}

	TRAP(err, iUriDb.RemoveL(iUriProps));
	 // Checking with expected result (positive/negative test case).
	if(err != aExpRetCode)
		{
	   	ERR_PRINTF2(_L("Test step failed: RInetUriList::RemoveL failed: %D\n"), err);
		SetTestStepResult(EFail);
		return;
		}
	
	INFO_PRINTF1(_L("Removed test passed successfully\n"));
	//  Checking whether specified uri is removed scuccesfully
    //  This is executed only for positive(where expRetCode = 0 means KErrNone) and non-capability tests.		   
	if((err == KErrNone) && (aExpRetCode == KErrNone) && !aIsCapabilityTest)
		{
		VerifyRemove();
		}			
	}

/*
This is to check whether uri with given properties is removed scuccessfully.
@internalTechnology 
@test
@param		None
@return		None
*/
void CTestRemoveUriStep::VerifyRemove()
	{  
	INFO_PRINTF1(_L("Remove-Verification: Remove verification started\n"));
		 	
    // Calling QueryUriL() with iUriProps object (Uri and properties) which has been added to DB in previous step.
    // It should match with single uri in DB ( means added successfully).
    TQueryArgs args3 ( &iUriProps.Uri().Uri().UriDes(), iUriProps.ServiceType(), iUriProps.ListType(), InetUriList::EExact);
    
    TRAPD(err, iUriDb.QueryUriL(args3, this));
    
    if(err != KErrNone)
	    {
	    ERR_PRINTF2(_L("Remove-Verification: failed: Leave occured in RInetUriList::MatchUriL: %D\n"), err);
	    SetTestStepResult(EFail);
	    }
   else
	   {
	   // Match count != 0 means there is some thing wrong in URI removal.
	   if(iQueryResults.Count() != 0)
		   {
		   ERR_PRINTF2(_L("Remove-Verification failed: Matched %D uris but it should be 0 as exact match has been done\n"), err);
		   SetTestStepResult(EFail);
		   }
	   else
		   {
		   INFO_PRINTF1(_L("Remove-Verification: Remove verification passed.\n"));
		   }
	   }
	ClearUriArray();
	}
