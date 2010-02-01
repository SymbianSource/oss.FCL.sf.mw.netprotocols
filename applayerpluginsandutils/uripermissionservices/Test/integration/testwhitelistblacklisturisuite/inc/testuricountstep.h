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
// Contains declaration of CTestUriCountStep class
// @internalAll
// 
//

#ifndef __TEST_URICOUNT_STEP_H__
#define __TEST_URICOUNT_STEP_H__
// User Include
#include "testwlistblisturiserver.h"
#include "testwlistblisturibasestep.h"

/**
This test step is to test query functionality to find the count of matching URIs of specific service/list type from database.
This is for both positive and negative tests.
@internalTechnology
@test
*/
class CTestUriCountStep : public CTestWListBListUriBaseStep
	{
public:
	
	CTestUriCountStep();

	~CTestUriCountStep();

	// TEF pure virtual
	virtual TVerdict doTestStepL();
	
protected:

	void CheckUriCount(const TInt aListType, const TInt aServiceType,const TInt count);
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestUriCountStep, "TestUriCountStep");
/*@}*/

#endif		// __TEST_URICOUNT_STEP_H__

