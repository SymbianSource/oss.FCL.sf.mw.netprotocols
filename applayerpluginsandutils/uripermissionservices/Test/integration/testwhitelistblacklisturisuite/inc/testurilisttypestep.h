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
// Contains declaration of CTestUriListTypeStep class
// @internalAll
// 
//

#ifndef __TEST_TESTURILISTTYPE_STEP_H__
#define __TEST_TESTURILISTTYPE_STEP_H__
// User Include
#include "testwlistblisturiserver.h"
#include "testwlistblisturibasestep.h"

/**
This test step is to test the query functionality to find the list-type of a specific URI from database
This is for both positive and negative tests.
@internalTechnology
@test
*/
class CTestUriListTypeStep : public CTestWListBListUriBaseStep
	{
public:
	
	CTestUriListTypeStep();

	~CTestUriListTypeStep();

	// TEF pure virtual
	virtual TVerdict doTestStepL();
	
protected:

	void FindAndCheckListTypeL(const TDesC& aUri, const TInt aExpListType, const TInt aServiceType, const TInt aExpRetCode,const TBool aIsCapabilityTest);
	void IsListTypeCorrect(const TDesC& aUri, const TInt aExpListType, const TInt aServiceType);
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestUriListTypeStep, "TestUriListTypeStep");
/*@}*/

#endif		// __TEST_TESTURILISTTYPE_STEP_H__

