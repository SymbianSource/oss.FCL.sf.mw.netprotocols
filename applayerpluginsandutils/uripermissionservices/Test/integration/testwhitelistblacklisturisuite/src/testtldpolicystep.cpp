// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @internalAll
//

// User Include
#include "testtldpolicystep.h"
// System Include
#include <uri8.h>



/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestTldPolicyStep::CTestTldPolicyStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestTldPolicyStep);
	}

/**
Destructor
@internalTechnology
@test
*/
CTestTldPolicyStep::~CTestTldPolicyStep()
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
TVerdict CTestTldPolicyStep::doTestStepL()
	{
	__UHEAP_MARK;
	INFO_PRINTF1(_L("\n"));
	// Get necessary information from INI file
	TPtrC uri;
	TInt listType;
	TInt queryType;
	TInt expListType;
	TInt expRetCode;
	TInt result = 0;
	
	if(!GetStringFromConfig(ConfigSection(), 	KIniUri, uri) ||
	   !GetIntFromConfig(ConfigSection(), 	KIniListType, listType) ||
	   !GetIntFromConfig(ConfigSection(), KInitldQueryType, queryType) ||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedListType, expListType)||
	   !GetIntFromConfig(ConfigSection(), KIniExpectedRetCode, expRetCode) 
	  )
		{
		ERR_PRINTF6(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n%S\n%S\n"
					  ),&KIniUri, &KIniListType, &KInitldQueryType, &KIniExpectedListType, &KIniExpectedRetCode
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		// No problem in reading values from INI file. Proceed.
		HBufC8* uriBuf = HBufC8::NewLC(uri.Length());
		TPtr8 uri8Bit = uriBuf->Des();
		uri8Bit.Copy(uri);
		TInt err(KErrNone);
		if(queryType == 1)
			{
			//To get list type of Uri
			TPolicyQueryArgs queryArgs2 ( &uri8Bit, (InetUriList::TTLDQueryType)queryType );
			TQueryResults queryResults;
			TRAP(err, iUriDb.QueryTldInfoL(queryArgs2, queryResults));
			if( err == KErrNone )	
				{
				result = queryResults.ListType();	
				}
			if ( result == expListType)
				{
				INFO_PRINTF1(_L("Test step passed: ListType succeeded.\n"));	
				}
			}
		else
			{
			HBufC8* policyData(NULL);
			//To Fetch Policy data for given TLD
			TPolicyQueryArgs queryArgs3 ( &uri8Bit, (InetUriList::TListType)listType, (InetUriList::TTLDQueryType)queryType);
			TQueryResults queryResults;
			TRAP(err, iUriDb.QueryTldInfoL(queryArgs3, queryResults));
			if( err == KErrNone )	
				{
				policyData = queryResults.CharsetL();
				CleanupStack::PushL( policyData );//1
				TPtrC8 policyPtr8( policyData->Des() );
				HBufC* policy16 = HBufC::NewLC( policyPtr8.Length() ); //2
				policy16->Des().Copy(policyPtr8);
				TPtrC policyPointer16 (policy16->Des());	
				INFO_PRINTF1(_L("Test step passed: Fetching Policy data is succeeded.\n"));
				_LIT(KPolicyData, "Given Uri is: %S \n and Policy Data is:\n %S\n");
				INFO_PRINTF3(KPolicyData, &uri, &policyPointer16 );
				CleanupStack::PopAndDestroy(2);
				}
			}
		CleanupStack::PopAndDestroy(); // uriBuf
		if ( err != KErrNone && err !=  expRetCode )
			{
	       	ERR_PRINTF2(_L("Test step failed: Leave occured in CTestTldPolicyStep: %D\n"), err);
		    SetTestStepResult(EFail);
			}
		}
	__UHEAP_MARKEND;
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL
	
	
