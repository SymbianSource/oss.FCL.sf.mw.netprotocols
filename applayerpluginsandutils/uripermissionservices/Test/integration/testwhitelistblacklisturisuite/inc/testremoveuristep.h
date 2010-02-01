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
// Contains declaration of CTestRemoveUriStep class
// @internalAll
// 
//

#ifndef __TEST_REMOVEURI_STEP_H__
#define __TEST_REMOVEURI_STEP_H__
// User Include
#include "testwlistblisturiserver.h"
#include "testwlistblisturibasestep.h"

/**
This is to test remove functionality of an URI from database.
This is for both positive and negative tests.
@internalTechnology
@test
*/
class CTestRemoveUriStep : public CTestWListBListUriBaseStep
	{
public:
	
	CTestRemoveUriStep();
	~CTestRemoveUriStep();

	// TEF pure virtual
	virtual TVerdict doTestStepL();

protected:

    void RemoveAndCheckUriL(const TDesC& aUri, TInt aServiceType, TInt aListType, TInt aExpRetCode, const TBool aIsCapabilityTest);
    void VerifyRemove();
    };

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestRemoveUriStep, "TestRemoveUriStep");
/*@}*/

#endif		// __TEST_ADDURI_STEP_H__

