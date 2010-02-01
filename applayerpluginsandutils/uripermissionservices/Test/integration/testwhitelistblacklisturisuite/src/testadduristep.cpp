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
// Contains implementation of CTestAddUriStep class
// @internalAll
// 
//

// User Include
#include "testadduristep.h"



/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestAddUriStep::CTestAddUriStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestAddUriStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestAddUriStep::~CTestAddUriStep()
	{
	}
	
/**
Tests the add functionality by adding an uri and verify that it is added succesfully
if it is a positive and non-capability test.
For capability test no need of add verification.
Here, reads values from INI file and calls AddAndCheckPermissionL
@test
@param		None
@return		EPass or EFail indicating the success or failure of the test step
@internalTechnology 
*/
TVerdict CTestAddUriStep::doTestStepL()
	{
	__UHEAP_MARK;
	INFO_PRINTF1(_L("\n"));
	// Get necessary information from INI file
	TPtrC uri;
	TInt listType;
	TInt serviceType;
	TPtrC favName;
	TInt expRetCode;
	TBool isCapabilityTest;
	
	if(!GetStringFromConfig(ConfigSection(), 	KIniUri, uri) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniListType, listType) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniServiceType, serviceType) ||
	   !GetStringFromConfig(ConfigSection(), KIniFavouriteName, favName) ||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedRetCode, 	expRetCode) ||
	   !GetBoolFromConfig(ConfigSection(), KIniIsCapabilityTest, isCapabilityTest) 
	  )
		{
		ERR_PRINTF7(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n%S\n%S\n%S\n"
					  ),&KIniUri, &KIniListType, &KIniServiceType, &KIniFavouriteName, &KIniExpectedRetCode, &KIniIsCapabilityTest
				   );
		SetTestStepResult(EFail);
		}
	else
		{// No problem in reading values from INI file. Proceed.
		
		TRAPD(err, AddAndCheckUriL(uri,listType, serviceType,favName,expRetCode,isCapabilityTest));
    	if(err != KErrNone)
	        {
      	    ERR_PRINTF2(_L("Test step failed: Leave occured in CTestAddUriStep::AddAndCheckUriL: %D\n"), err);
       	    SetTestStepResult(EFail);
		    }
		}
	iUriProps.Close(); // Cleans up the URI properties object.
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL
	
/**
Adds an uri with ReadWrite permission to database and call isAdded() 
to check that it has been added successfully with ReadWrite permission.
@internalTechnology 
@test
@param aUri  				A non-modifiable object of TUriC8
@param aListType  				List type of given URI
@param aServiceType  		Service type of given URI
@param aFavName		        New Favourite name
@param aExpRetCode          Error code i.e. success/failure code
@param aIsCapabilityTest    Capability test if ETrue else EFalse
@return		None
*/
void CTestAddUriStep::AddAndCheckUriL(const TDesC& aUri, const TInt aListType, const TInt aServiceType, const TDesC& aFavName, const TInt aExpRetCode, const TBool aIsCapabilityTest)
	{
	//Creating RInetUri object	
	CreateL(aUri, aListType, aServiceType, aFavName);
	
	TRAPD(err, iUriDb.AddL(iUriProps));
	 // Checking with expected result (positive/negative test case).
	if(err != aExpRetCode)
		{
	   	ERR_PRINTF2(_L("Test step failed: RInetUriList::AddL failed: %D\n"), err);
		SetTestStepResult(EFail);
		return;
		}
	INFO_PRINTF1(_L("URI ADD test step passed.\n"));
	//  Checking whether uri is added scuccesfully
    //  This is executed only for positive(where expRetCode = 0 means KErrNone) and non-capability tests.
	if((err == KErrNone) && (aExpRetCode == KErrNone) && !aIsCapabilityTest)
		{
		VerifyAdd();		
		}
	}

/*
This is to check whether uri is added scuccessfully with given uri properties and ReadWrite permission. 
@internalTechnology 
@param		None
@test
@return		None
*/
void CTestAddUriStep::VerifyAdd()
	{
	INFO_PRINTF1(_L("Add-Verification: Add verification started\n"));
	
    // Calling QueryUriL() with iUriProps object (Uri and properties) which has been added to DB in previous step.
    // It should match with single uri in DB ( means added successfully).
    TQueryArgs args3 ( &iUriProps.Uri().Uri().UriDes(), iUriProps.ServiceType(), iUriProps.ListType(), InetUriList::EExact);
    TRAPD(err, iUriDb.QueryUriL(args3, this));
    
    if(err != KErrNone)
	    {
	    ERR_PRINTF2(_L("Add verification failed: Leave occured in RInetUriList::MatchUriL: %D\n"), err);
	    SetTestStepResult(EFail);
	    }
   else
	   {
	       // Match count != 1 means there is some thing wrong in URI addition.
	   if (iQueryResults.Count() == 1)
		   {
		   // Permission should be ReadWrite for all uris other than preconfigured ones.
		   // Pre-configured uri can be ReadOnly or ReadWrite.	
		   TBool isMatched = ETrue;	  	
		   if(iQueryResults[0].Permission() == InetUriList::EReadOnly)
			   {
		  	   ERR_PRINTF1(_L("Add verification failed: Added uri permission is readonly, but it should be ReadWrite.\n"));
		  	   SetTestStepResult(EFail);
		  	   isMatched = EFalse;		           
			   }
			   
		   // Checking the favourite name.
		   const TDesC8& favName = iQueryResults[0].FavouriteName();
		   const TDesC8& expfavName = iUriProps.FavouriteName();
		   
		   if(err != KErrNone)
		       {
		       ERR_PRINTF2(_L("Add verification failed: HBufC8 object creation failed: %D\n"), err);
			   SetTestStepResult(EFail);		           
			   }

		   if(favName.Compare(expfavName) !=0)
		       {
		       ERR_PRINTF1(_L("Add verification failed: Expected & actual favourite names are not matched.\n"));
			   SetTestStepResult(EFail);
			   isMatched = EFalse;		           
			   }
		   if(isMatched)
			   {
			   INFO_PRINTF1(_L("Add-Verification : Uri add verification passed\n"));	
			   }
		   else
			   {
			   INFO_PRINTF1(_L("Add-Verification: Uri add verification failed\n"));	
			   }
		   }
		else
			{
			ERR_PRINTF2(_L("Add verification failed: Matched %D uris but it should be 1 as exact match has been done\n"), iQueryResults.Count());
		   	SetTestStepResult(EFail);
			}
	   }
	ClearUriArray();
	}

