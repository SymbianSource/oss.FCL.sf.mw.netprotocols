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
// Contains declaration of CTestUpdateUriFavNameStep class
// @internalAll
// 
//

#ifndef __TEST_UPDATEURIFAVNAME_STEP_H__
#define __TEST_UPDATEURIFAVNAME_STEP_H__
// User Include
#include "testwlistblisturiserver.h"
#include "testwlistblisturibasestep.h"

/**
This test step is to test the update favourite name functionality of a specific URI.
This is for both positive and negative tests.
@internalTechnology
@test
*/
class CTestUpdateUriFavNameStep : public CTestWListBListUriBaseStep
	{
public:
	
	CTestUpdateUriFavNameStep();

	~CTestUpdateUriFavNameStep();

	// TEF pure virtual
	virtual TVerdict 	doTestStepL();

protected:
    void UpdateAndCheckFavNameL(const TDesC& aUri, TInt aServiceType, const TDesC& aFavName, TInt aExpRetCode, const TBool aIsCapabilityTest);

	void VerifyFavNameUpdate();	
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestUpdateUriFavNameStep, "TestUpdateUriFavNameStep");
/*@}*/

#endif		// __TEST_UPDATEURIFAVNAME_STEP_H__

