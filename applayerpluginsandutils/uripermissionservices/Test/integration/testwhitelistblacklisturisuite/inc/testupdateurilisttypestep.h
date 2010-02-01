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
// Contains declaration of CTestUpdateUriListTypeStep class
// @internalAll
// 
//

#ifndef __TEST_UPDATEURILISTTYPE_STEP_H__
#define __TEST_UPDATEURILISTTYPE_STEP_H__
// User Include
#include "testwlistblisturiserver.h"
#include "testwlistblisturibasestep.h"

/**
This test step is to test the update list-type functionality  of a specific uri.
This is for both positive and negative tests.
@internalTechnology
@test
*/
class CTestUpdateUriListTypeStep : public CTestWListBListUriBaseStep
	{
public:
	// Construction
	CTestUpdateUriListTypeStep();

	//Destruction
	~CTestUpdateUriListTypeStep();
	
	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();
	
protected:

	void UpdateAndCheckListTypeL(const TDesC& aUri, TInt aListType, TInt aServiceType, TInt aExpRetCode, const TBool aIsCapabilityTest);
	void VerifyUpdateListType();
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestUpdateUriListTypeStep, "TestUpdateUriListTypeStep");
/*@}*/

#endif		// __TEST_UPDATEURILISTTYPE_STEP_H__

