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
// Contains declaration of CTestAddUriStep class
// @internalAll
// 
//

#ifndef __TEST_ADDURI_STEP_H__
#define __TEST_ADDURI_STEP_H__

// User Include
#include "testwlistblisturiserver.h"
#include "testwlistblisturibasestep.h"

/**
This test step is to test add functionality of an URI to database
This is for both positive and negative tests.
@internalTechnology
@test
*/
class CTestAddUriStep: public CTestWListBListUriBaseStep
	{
public:
	
	CTestAddUriStep();

	~CTestAddUriStep();

	// TEF pure virtual
	virtual TVerdict 	doTestStepL();
	
protected:

	void AddAndCheckUriL(const TDesC& aUri, const TInt aListType, const TInt aServiceType, const TDesC& aFavName, const TInt aExpRetCode, const TBool aIsCapabilityTest);
	
	void VerifyAdd();

	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestAddUriStep, "TestAddUriStep");
/*@}*/

#endif		// __TEST_ADDURI_STEP_H__

