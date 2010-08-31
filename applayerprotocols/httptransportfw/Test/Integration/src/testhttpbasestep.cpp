// Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Contains implementation of CTestHttpBaseStep class
// @internalAll
// 
//

// System Include
// for StartC32()
#include <c32comm.h>
#include <e32base.h>
#include <http.h>
// User Include
#include "testhttpbasestep.h"


// PDD names for the physical device drivers that are loaded in wins or arm
#if defined (__WINS__)
#define PDD_NAME		_L("ECDRV")
#else
#define PDD_NAME		_L("EUART1")
#define PDD2_NAME		_L("EUART2")
#define PDD3_NAME		_L("EUART3")
#define PDD4_NAME		_L("EUART4")
#endif

#define LDD_NAME		_L("ECOMM")


// Loads the physical device drivers
void CTestHttpBaseStep::InitCommsL()
	{
#ifndef SIROCCO_CODE_MIGRATION
	TInt ret = User::LoadPhysicalDevice(PDD_NAME);
	User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);

#ifndef __WINS__
	ret = User::LoadPhysicalDevice(PDD2_NAME);
	ret = User::LoadPhysicalDevice(PDD3_NAME);
	ret = User::LoadPhysicalDevice(PDD4_NAME);
#endif

	ret = User::LoadLogicalDevice(LDD_NAME);
	User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);
	ret = StartC32();
	User::LeaveIfError(ret == KErrAlreadyExists?KErrNone:ret);
#endif //SIROCCO_CODE_MIGRATION
	}
	

/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestHttpBaseStep::CTestHttpBaseStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestHttpBaseStep);
	}


/**
Destructor: Closes the iFileServ.
@internalTechnology
@test
*/
CTestHttpBaseStep::~CTestHttpBaseStep()
	{
	iFileServ.Close();
	delete iSched;
	//CleanupStack::PopAndDestroy(); // iSched
	iSched = NULL;
	}
	

/**
Base class virtual doTestStepPreambleL(): 
Create and install the active scheduler and connect to iFileServ (RFs).
@internalTechnology
@test
@param		None
@return		EPass or EFail.
*/
TVerdict CTestHttpBaseStep::doTestStepPreambleL()
	{
	iSched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(iSched);
	User::LeaveIfError(iFileServ.Connect());
    return TestStepResult();
	}	// doTestPreambleL


/**
Base class pure virtual doTestStepL(): 
Tests the ebo support in http.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestHttpBaseStep::doTestStepL()
	{
	__UHEAP_MARK;
	INFO_PRINTF1(_L("\n"));
	// Start C32 and initalize some device drivers. This is necessary when running a test console as these won't 
	// have been started
	TRAPD(err,InitCommsL());
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Teststep Failed: Leave occured in CTestHttpBaseStep::InitCommsL: %D\n"
					  ),err
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		// Opening session
		iSess.OpenL();
		// start the client
		TRAP(err,StartClientL());
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Teststep Failed: Leave occured in CTestHttpBaseStep::StartClientL: %D\n"
						  ),err
					   );
			SetTestStepResult(EFail);
			}
		// Closing session
		iSess.Close();
		}
	__UHEAP_MARKEND; 
	
	INFO_PRINTF1(_L("\n"));
	return TestStepResult();
	}	// doTestStepL
	

/**
Sets the headers.
@internalTechnology
@test
@param		aHeaders 	Headers collection
@param		aHdrField 	Header field to be added
@return		aHdrValue	Header value to be added	
*/
void CTestHttpBaseStep::SetHeaderL(RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue)
	{
	RStringF valStr = iSess.StringPool().OpenFStringL(aHdrValue);
	THTTPHdrVal val(valStr);
	aHeaders.SetFieldL(iSess.StringPool().StringF(aHdrField,RHTTPSession::GetTable()), val);
	valStr.Close();
	}
	
	



