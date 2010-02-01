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

#ifndef __TESTTLDPOLICYSTEP_H_
#define __TESTTLDPOLICYSTEP_H_
// User Include
#include "testwlistblisturiserver.h"
#include "testwlistblisturibasestep.h"

/**
This test step is to test query tld info functionality(fetching policy data or to find out whether the given uri is in whitelist/ Black list).
This is for both positive and negative tests.
@internalTechnology
@test
*/
class CTestTldPolicyStep : public CTestWListBListUriBaseStep
	{
public:
	
	CTestTldPolicyStep();

	~CTestTldPolicyStep();

	// TEF pure virtual
	virtual TVerdict 	doTestStepL();
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestTldPolicyStep, "TestTldPolicyStep");
/*@}*/

#endif		// __TESTTLDPOLICYSTEP_H_
