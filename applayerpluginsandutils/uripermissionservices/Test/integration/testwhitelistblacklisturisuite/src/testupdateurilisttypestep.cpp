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
// Contains implementation of CTestUpdateUriListTypeStep class
// @internalAll
// 
//

// User Include
#include "testupdateurilisttypestep.h"
// System Include
#include <uri8.h>
#include <uriutils.h>
	
/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestUpdateUriListTypeStep::CTestUpdateUriListTypeStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestUpdateUriListTypeStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestUpdateUriListTypeStep::~CTestUpdateUriListTypeStep()
	{

	}

/**
Tests the uri listtype updation functionality and check that it is updated successfully if it is positive and non-capability test.
It reads values from INI file and updates the list type of a specific uri.
@internalTechnology 
@test
@param		None
@return		EPass or EFail indicating the success or failure of the test step
*/
TVerdict CTestUpdateUriListTypeStep::doTestStepL()
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
	   !GetIntFromConfig(ConfigSection(), KIniExpectedRetCode, expRetCode) ||
	   !GetBoolFromConfig(ConfigSection(), KIniIsCapabilityTest, isCapabilityTest) 
	  )
		{
		ERR_PRINTF6(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n%S\n%S\n%S\n"
					  ),&KIniUri, &KIniListType, &KIniServiceType, &KIniExpectedRetCode, &isCapabilityTest
				   );
		SetTestStepResult(EFail);
		}
	else
		{// No problem in reading values from INI file. Proceed.
		
	    TRAPD(err, UpdateAndCheckListTypeL(uri, listType, serviceType, expRetCode, isCapabilityTest));
	    if(err != KErrNone)
	        {
        	ERR_PRINTF2(_L("Test step failed: Leave occured in CTestUpdateUriListTypeStep::UpdateListTypeL: %D\n"), err);
        	SetTestStepResult(EFail);
		    }
		}
		
	iUriProps.Close();
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL
	
/*	
Updates list type of an uri and calls isUpdated() method to check whether list type is updated successfully.
@internalTechnology 
@test
@param aUri  				A non-modifiable object of TUriC8
@param aListType  				List type of given URI
@param aServiceType  		Service type of given URI
@param aExpRetCode          Error code i.e. success/failure code
@param aIsCapabilityTest    Capability test if ETrue else EFalse
@return		None
*/
void CTestUpdateUriListTypeStep::UpdateAndCheckListTypeL(const TDesC& aUri, TInt aListType, TInt aServiceType, TInt aExpRetCode, const TBool aIsCapabilityTest)
	{
	//CreateL(aUri, aListType, aServiceType, KNullDesC);
	TInt err;
	if(!aIsCapabilityTest)
		{
		TRAP(err, FetchL(aUri, aServiceType, KNullDesC));
		if((aExpRetCode == KErrNone) || (aExpRetCode == InetUriList::KErrUriNotFound))
			{
				if(err != aExpRetCode)
					{
					ERR_PRINTF3(_L("Test step failed: RInetUriList::OpenUriL failed [Act: %D & ExpRetCode: %D didn't match]\n"), err, aExpRetCode);	
					SetTestStepResult(EFail);
					return;
					}
			}
		}
	if(aIsCapabilityTest || err != KErrNone)
		{
		CreateL(aUri, aListType, aServiceType, KNullDesC);
		}
	else
		{
		iUriProps.SetListType((InetUriList::TListType)aListType);	
		}	
		
	TRAP(err, iUriDb.UpdateL(iUriProps));
	
	// Checking with expected result (positive/negative test case).
    if(err != aExpRetCode)
		{
		ERR_PRINTF2(_L("Test step failed: RInetUriList::UpdateListTypeL failed: %D\n"), err);
		SetTestStepResult(EFail);
		return;
		}	
	INFO_PRINTF1(_L("Uri list type updated successfully \n"));		
	//  Checking whether list type of an uri is updated successfully.
    //  This is executed only for positive(where expRetCode = 0 means KErrNone) and non-capability tests.
			   
	if((err == KErrNone) && (aExpRetCode == KErrNone) && !aIsCapabilityTest)
		{
		VerifyUpdateListType(); 	
		}
	}
	
/*
This is to check whether list type of a specified uri is updated scuccessfully.
@internalTechnology 
@test
@param		None
@return		None
*/
void CTestUpdateUriListTypeStep::VerifyUpdateListType()
	{
	INFO_PRINTF1(_L("Uri listtype updation-Verification: verification started\n"));

    // Calling QueryUriL() with iUriProps object (Uri and properties).
    // It should match with single uri in DB ( means list type of an uri updated successfully).
    TQueryArgs args3 ( &iUriProps.Uri().Uri().UriDes(), iUriProps.ServiceType(), iUriProps.ListType(), InetUriList::EExact);
    TRAPD(err, iUriDb.QueryUriL(args3, this));
    
    
    if(err != KErrNone)
	    {
	    ERR_PRINTF2(_L("Uri listtype updation-Verification failed: Leave occured in RInetUriList::MatchUriL: %D\n"), err);
	    SetTestStepResult(EFail);
	    }
   else
	   {
	   // Match count != 1 means there is some thing wrong in URI list type updation.
	   if(iQueryResults.Count() != 1)
		   {
		   ERR_PRINTF2(_L("Uri listtype updation-Verification failed: Matched %D uris but it should be 1 as exact match has been done\n"), err);
		   SetTestStepResult(EFail);
		   }
	   else
	       {
	       INFO_PRINTF1(_L("Uri listtype updation-Verification :Uri list type verification passed.\n"));	
	       }
	   }
	ClearUriArray();
	}
