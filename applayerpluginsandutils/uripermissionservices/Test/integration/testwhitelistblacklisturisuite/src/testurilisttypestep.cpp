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
// Contains implementation of CTestUriListTypeStep class
// @internalAll
// 
//

// User Include
#include "testurilisttypestep.h"
// System Include
#include <uri8.h>
#include <uriutils.h>

	
/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestUriListTypeStep::CTestUriListTypeStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestUriListTypeStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestUriListTypeStep::~CTestUriListTypeStep()
	{

	}
	
/**
This is to test the find fuctionality to get the list type of a specific given uri
and check that returned list type is corrrect if it is positive and non-capability test.
It reads values from INI file and gets the list type of a specific uri from DB.
@internalTechnology 
@test
@param		None
@return		EPass or EFail indicating the success or failure of the test step
*/
TVerdict CTestUriListTypeStep::doTestStepL()
	{
	__UHEAP_MARK;
	INFO_PRINTF1(_L("\n"));
	// Get necessary information from INI file

	TPtrC uri;
	TInt serviceType;
	TInt expListType;
	TInt expRetCode;
	TBool isCapabilityTest;
	
	if(!GetStringFromConfig(ConfigSection(), 	KIniUri, uri) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniServiceType, serviceType) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniExpectedListType, expListType) ||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedRetCode, expRetCode) ||
	   !GetBoolFromConfig(ConfigSection(), KIniIsCapabilityTest, isCapabilityTest) 
	  )
		{
		ERR_PRINTF6(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n%S\n%S\n"
					  ),&KIniUri, &KIniServiceType, &KIniExpectedRetCode, &KIniExpectedListType, &isCapabilityTest
				   );
		SetTestStepResult(EFail);
		}
	else
		{// No problem in reading values from INI file. Proceed.
		
		TRAPD(err,FindAndCheckListTypeL(uri,expListType,serviceType,expRetCode,isCapabilityTest));
		if(err != KErrNone)
	        {
        	ERR_PRINTF2(_L("Test step failed: Leave occured in CTestUriListTypeStep::FindAndCheckListTypeL: %D\n"), err);
        	SetTestStepResult(EFail);
		    }
		
		}
	iUriProps.Close();
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL
	
/*
Finds the list type of given uri and calls IsListTypeCorrect() to verify the returned list type is correct.
@internalTechnology 
@test
@param aUri  				A non-modifiable object of TUriC8
@param aExpListType  		Expected listype of given URI
@param aServiceType  		Service type of given URI
@param aExpRetCode          Error code i.e. success/failure code
@param aIsCapabilityTest    Capability test if ETrue else EFalse
@return		None
*/
void CTestUriListTypeStep::FindAndCheckListTypeL(const TDesC& aUri, const TInt aExpListType, const TInt aServiceType, const TInt aExpRetCode,const TBool aIsCapabilityTest)
	{
	InetUriList::TListType actListType;
	HBufC8* uriBuf = HBufC8::NewLC(aUri.Length());
	TPtr8 uri8Bit = uriBuf->Des();
	uri8Bit.Copy(aUri);
	
	TInt err = 0;

    // Calling GetListType() to get the list type of an Uri
    err=iUriDb.GetListType(uri8Bit, (InetUriList::TServiceType)aServiceType, actListType);
    // Checking with expected result (positive/negative test case).
    
    if(err != aExpRetCode)
    	{
    	ERR_PRINTF3(_L("Test step failed: Actual[%D] and Expected[%D] ret code didn't match.\n"), err, aExpRetCode);
		SetTestStepResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("Actual and Expected ret code matched\n"));	
		}
	
	//  Comparing actual & expected list type
    //  This is executed only for positive test cases where expRetCode = 0 means KErrNone
	if((err == KErrNone) && (aExpRetCode == KErrNone))
		{
		if(actListType != (InetUriList::TListType)aExpListType)
			{
			ERR_PRINTF3(_L("Test step failed: actListType: %D is not matched with expListType: %D.\n"),(TInt)actListType,aExpListType);
	    	SetTestStepResult(EFail);	
			}
		else
			{
			// This is executed for non-capability test
			// to check that returned list type is correct.
			if(!aIsCapabilityTest) 
				{
				IsListTypeCorrect(aUri,aExpListType,aServiceType);		
				}
			}
		}
	CleanupStack::PopAndDestroy(); //uriBuf
	}
	
/*	
This is to check whether that list type is correct.
@test
@param		aUri,aListType,aServiceType.
@param aUri  				A non-modifiable object of TUriC8
@param aExpListType  				List type of given URI
@param aServiceType  		Service type of given URI
@return		None
*/
void CTestUriListTypeStep::IsListTypeCorrect(const TDesC& aUri, const TInt aExpListType, const TInt aServiceType)
	{
	INFO_PRINTF1(_L("Listtype-Verification: verification started\n"));
	TRAPD(err, FetchL(aUri, aServiceType, KNullDesC));
	iUriProps.SetListType((InetUriList::TListType)aExpListType);
	if(err != KErrNone)
	    {
	    ERR_PRINTF2(_L("Listtype-Verification: Leave occured in CreateL: %D\n"), err);
	    SetTestStepResult(EFail);
	    return;
	    }
	   	
    // Calling QueryUriL() with iUriProps object (Uri and properties).
    // It should match with single uri in DB ( means list type of an uri is correct).
    TQueryArgs args3 ( &iUriProps.Uri().Uri().UriDes(), iUriProps.ServiceType(), iUriProps.ListType(), InetUriList::EExact);
    TRAP(err, iUriDb.QueryUriL(args3, this));
    
    if(err != KErrNone)
	    {
	    ERR_PRINTF2(_L("Listtype-Verification: Leave occured in RInetUriList::MatchUriL: %D\n"), err);
	    SetTestStepResult(EFail);
	    }
   else
	   {
	   // Match count != 1 means there is some thing wrong in finding of list type of an uri.
	   if(iQueryResults.Count() != 1)
		   {
		   ERR_PRINTF2(_L("Listtype-Verification: Matched uris %D but it should be 1 as exact match has been done\n"),iQueryResults.Count());
		   SetTestStepResult(EFail);
		   }
	   }
	ClearUriArray();
	}


