// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CPIPELININGTESTENGINE_H__
#define __CPIPELININGTESTENGINE_H__

#include <e32base.h>

#include "MPipeliningTestObserver.h"
#include "timerlogger.h"

class CPipeliningTestServer;
class CPipeliningTestClient;
class CHTTPTestUtils;
class MPipeliningTestCase;

class CPipeliningTestEngine : public CBase,
							  public MPipeliningTestObserver
	{
public:
	static CPipeliningTestEngine* NewL();
	~CPipeliningTestEngine();
	
	void StartTestsL(TDesC&);
	void DoTestsL(TDesC&);

private:
	CPipeliningTestEngine();
	void ConstructL();
	void RunTestL(const TDesC& aSecName, MPipeliningTestCase& aTestCase);
	void DoPipeliningConfigTestsL(TDesC&);	
	// From MPipeliningTestObserver
	void EndTest(TInt aError);
	
private: // Atttibutes
	CHTTPTestUtils*			iTestUtils;
	CPipeliningTestServer*	iTestServer;
	TBool					iTestFailed;
	MPipeliningTestCase*	iTestCase; // Not owned
	TInt 					iEngineLastError;	
	__DECLARE_PERFORMANCE_LOG
	};
	
#endif // __CPIPELININGTESTENGINE_H__
