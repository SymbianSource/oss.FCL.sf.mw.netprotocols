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
// Contains declaration of CTestMatchUriTypeStep class
// @internalAll
// 
//

#ifndef __TEST_MATCHURITYPE_STEP_H__
#define __TEST_MATCHURITYPE_STEP_H__
// User Include
#include "testwlistblisturiserver.h"
#include "testwlistblisturibasestep.h"

/**
This test step is to test query/match functionality(matching uris in DB against ListType/ServiceType).
This is for both positive and negative tests.
@internalTechnology
@test
*/
class CTestMatchUriTypeStep : public CTestWListBListUriBaseStep
	{
public:
	
	CTestMatchUriTypeStep();

	~CTestMatchUriTypeStep();

	// TEF pure virtual
	virtual TVerdict 	doTestStepL();
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestMatchUriTypeStep, "TestMatchUriTypeStep");
/*@}*/

#endif		// __TEST_MATCHURITYPE_STEP_H__

