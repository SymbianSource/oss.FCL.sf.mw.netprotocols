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
// Contains implementation of CTestUpdateUriFavNameStep class
// @internalAll
// 
//

// User Include
#include "testupdateurifavnamestep.h"
// System Include
#include <uri8.h>
#include <uriutils.h>

/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestUpdateUriFavNameStep::CTestUpdateUriFavNameStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestUpdateUriFavNameStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestUpdateUriFavNameStep::~CTestUpdateUriFavNameStep()
	{
	}

/**
Tests the uri favourite name updation functionality and check that it is updated successfully if it is positive and non-capability test.
Here, reads values from INI file and calls UpdateAndCheckFavNameL()
@internalTechnology 
@test
@param		None
@return		EPass or EFail indicating the success or failure of the test step
*/
TVerdict CTestUpdateUriFavNameStep::doTestStepL()
	{
	__UHEAP_MARK;
	INFO_PRINTF1(_L("\n"));
	// Get necessary information from INI file

	TPtrC uri;
	TInt serviceType;
	TPtrC favName;
	TInt expRetCode;
	TBool isCapabilityTest;
		
	if(!GetStringFromConfig(ConfigSection(), 	KIniUri, uri) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniServiceType, serviceType) ||
	   !GetStringFromConfig(ConfigSection(), 	KIniFavouriteName, favName) ||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedRetCode, expRetCode) ||
	   !GetBoolFromConfig(ConfigSection(), KIniIsCapabilityTest, isCapabilityTest) 
	  )
		{
		ERR_PRINTF6(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n%S\n%S\n"
					  ),&KIniUri, &KIniServiceType, &KIniFavouriteName, &KIniExpectedRetCode, &KIniIsCapabilityTest
				   );
		SetTestStepResult(EFail);
		}
	else
		{// No problem in reading values from INI file. Proceed.
		   
	   TRAPD(err, UpdateAndCheckFavNameL(uri, serviceType, favName, expRetCode, isCapabilityTest));
	   if(err != KErrNone)
	       {
           ERR_PRINTF2(_L("Test step failed: Leave occured in CTestUpdateUriFavNameStep::UpdateAndCheckFavNameL: %D\n"), err);
           SetTestStepResult(EFail);
	       } 
		}
	iUriProps.Close(); // Cleans up the URI properties object.
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL
	
	
/**
Updates favourite name of an uri in database and call isUpdate() 
to check that favourite name has been updated successfully.
@internalTechnology 
@test
@param		aUri,aServiceType,aFavName,aExpRetCode(success/error code),aUsingTUriC(ETrue/EFalse),aIsCapabilityTest(ETrue/EFalse).
@param aUri  				A non-modifiable object of TUriC8
@param aServiceType  		Service type of given URI
@param aFavName		        New Favourite name
@param aExpRetCode          Error code i.e. success/failure code
@param aIsCapabilityTest    Capability test if ETrue else EFalse
@return		None
*/
void CTestUpdateUriFavNameStep::UpdateAndCheckFavNameL(const TDesC& aUri, TInt aServiceType, const TDesC& aFavName, TInt aExpRetCode, const TBool aIsCapabilityTest)
	{
	TInt err;
	if(!aIsCapabilityTest)
		{
		TRAP(err, FetchL(aUri, aServiceType, aFavName));
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
		CreateL(aUri, InetUriList::EWhiteList, aServiceType, aFavName);
		}
		
	TRAP(err, iUriDb.UpdateL(iUriProps));
		 
	 // Checking with expected result (positive/negative test case).
	 if(err != aExpRetCode)
		 {
		 ERR_PRINTF2(_L("Test step failed: RInetUriList::UpdateFavouriteNameL failed: %D\n"), err);
		 SetTestStepResult(EFail);
		 return;
		 }
	        
	 //  Checking whether fav name is updated scuccesfully
	 //  This is executed only for positive(where expRetCode = 0 means KErrNone) and non-capability tests.
	 INFO_PRINTF1(_L("Update fav name test step passed.\n"));	
	 	   
	 if((err == KErrNone) && (aExpRetCode == KErrNone) && !aIsCapabilityTest)
		 {
		 VerifyFavNameUpdate();
		 }  
	}	        

/*
This is to check whether favourite name is updated scuccessfully. 
@internalTechnology 
@test
@param		None
@return		None
*/
void CTestUpdateUriFavNameStep::VerifyFavNameUpdate()
	{
	INFO_PRINTF1(_L("Fav name update verification: Verification started\n"));

	TQueryArgs args3 ( &iUriProps.Uri().Uri().UriDes(), iUriProps.ServiceType(), iUriProps.ListType(), InetUriList::EExact);
    TRAPD(err, iUriDb.QueryUriL(args3, this));
    
	if(err != KErrNone)
		{
		ERR_PRINTF2(_L("Fav name update verification failed: Leave occured in RInetUriList::MatchUriL: %D\n"), err);
		SetTestStepResult(EFail);
		}
	else
		{
		// Match count != 1 means there is some thing wrong in URI addition.
	  	if(iQueryResults.Count() == 1)
		   { 	
		   const TDesC8& favName = iQueryResults[0].FavouriteName();
		   const TDesC8& expfavName = iUriProps.FavouriteName();
		  	
		   if(favName.Compare(expfavName) !=0)
			   {
			   ERR_PRINTF1(_L("Fav name update verification failed: Expected & actual favourite names are not matching.\n"));
			   SetTestStepResult(EFail);		           
			   }
		   else
		   	   {
		   	   INFO_PRINTF1(_L("Fav name update verification: FavName update operation passed.\n"));
		   	   }
		   }			
		 else 
		   {
		   ERR_PRINTF2(_L("Fav name update verification failed: Matched %D uris but it should be 1 as exact match has been done\n"), iQueryResults.Count());
		   SetTestStepResult(EFail);
		   }
		}
	ClearUriArray();
	}
			   

