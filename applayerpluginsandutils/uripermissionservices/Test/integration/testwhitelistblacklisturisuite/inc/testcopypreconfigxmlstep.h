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
// Contains declaration of CTestCopyPreConfigXmlStep class
// @internalAll
// 
//

#ifndef __TEST_COPY_PRECONFIGXML_STEP_H__
#define __TEST_COPY_PRECONFIGXML_STEP_H__
// User Include
#include "testwlistblisturiserver.h"

/**
This is the test step to copy ineturilist.xml(preconfiguration file)
from z:\\ineturilist\\testdata\\ineturilist.xml to c:\\private\\20009D70\\ineturilist.xml
@internalTechnology
@test
*/
_LIT ( KSourcePath, "z:\\ineturilist\\testdata\\ineturilist.xml" );
_LIT ( KDestinationPath, "c:\\private\\20009d70\\ineturilist.xml" );
class CTestCopyPreConfigXmlStep : public CTestStep
	{
public:
	
	CTestCopyPreConfigXmlStep();
	~CTestCopyPreConfigXmlStep();
	// TEF pure virtual
	virtual TVerdict doTestStepL();	
	
protected:

    void DoOldDbCopyL ();
	
	};

	
/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestCopyPreConfigXmlStep, "TestCopyPreConfigXmlStep");
/*@}*/

#endif		// __TEST_COPY_PRECONFIGXML_STEP_H__
