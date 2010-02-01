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
// Contains declaration of CTestRemoveDBStep class
// @internalAll
// 
//

#ifndef __TEST_RemoveDB_STEP_H__
#define __TEST_RemoveDB_STEP_H__
// User Include
#include "testwlistblisturiserver.h"

/**
This is to remove the existing database.
@internalTechnology
@test
*/
_LIT ( KOldDB, "c:\\private\\10281e17\\[20009D70]URIList.dat");
class CTestRemoveDBStep : public CTestStep
	{
public:
	
	CTestRemoveDBStep();
	~CTestRemoveDBStep();
	// TEF pure virtual
	virtual TVerdict doTestStepL();	
	
protected:

    void RemoveDBL();
	
	};

	
/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestRemoveDBStep, "TestRemoveDBStep");
/*@}*/

#endif		// __TEST_RemoveDB_STEP_H__
