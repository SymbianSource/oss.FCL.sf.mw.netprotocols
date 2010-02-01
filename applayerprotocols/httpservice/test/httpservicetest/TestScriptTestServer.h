// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TEST_SCRIPTTEST_SERVER_H__
#define __TEST_SCRIPTTEST_SERVER_H__

// System Includes
#include <test/testexecuteserverbase.h>

/**
The test server.
@internalTechnology
@test
*/
class CTestScriptTestServer : public CTestServer
	{
public:
	// Construction
	static	CTestScriptTestServer* 	NewL();

	// Destruction
	~CTestScriptTestServer();

	// Base class pure virtual
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	};

#endif		// __TEST_SCRIPTTEST_SERVER_H__
