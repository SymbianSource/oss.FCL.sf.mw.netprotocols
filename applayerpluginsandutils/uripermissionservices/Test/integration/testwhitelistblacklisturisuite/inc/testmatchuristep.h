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
// Contains declaration of CTestMatchUriStep class
// @internalAll
// 
//

#ifndef __TEST_MATCHURI_STEP_H__
#define __TEST_MATCHURI_STEP_H__
// User Include
#include "testwlistblisturiserver.h"
#include "testwlistblisturibasestep.h"

/**
This test step is to test query/match functionality(matching uris in DB against a specific uri and ListType/ServiceType).
This is for both positive and negative tests.
@internalTechnology
@test
*/
class CTestMatchUriStep : public CTestWListBListUriBaseStep
	{
public:
	
	CTestMatchUriStep();

	~CTestMatchUriStep();

	// TEF pure virtual
	virtual TVerdict 	doTestStepL();
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestMatchUriStep, "TestMatchUriStep");
/*@}*/

#endif		// __TEST_MATCHURI_STEP_H__

