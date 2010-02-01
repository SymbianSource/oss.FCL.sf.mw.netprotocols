// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __TEST_SCRIPT_TEST_STEP_H__
#define __TEST_SCRIPT_TEST_STEP_H__

// User Include
#include "TestScriptTestServer.h"

/**
The test step that creates a file based on the path and name specified 
in the ini file.
@internalTechnology 
*/
class CTestScriptTestStep : public CTestStep
	{	
	public:
	CTestScriptTestStep();
	
	// TEF pure virtual
	virtual enum TVerdict doTestStepL();
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestScriptTestStep, "TestScriptTestStep");	
/*@}*/

#endif		// __TEST_SCRIPT_TEST_STEP_H__




