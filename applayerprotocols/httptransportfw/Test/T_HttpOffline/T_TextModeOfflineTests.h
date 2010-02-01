// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// This header file defines test classes used for online testing of the Text-Mode protocol handler
#include <http.h>
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

/**
 *  Derived test case class : test parallel transactions (dummy)
 */
class CSomeOfflineTest: public CHttpTestBase
	{
	// to implement in this class' member functs definitions
public:
	virtual ~CSomeOfflineTest();
private:
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	};


/**
 *  Derived test case class : test cancellation (dummy)
 */
class CSomeOtherOfflineTest: public CHttpTestBase
	{
	// to implement in this class' member functs definitions
public:
	virtual ~CSomeOtherOfflineTest();
private:
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	};

