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

// This header file defines test classes used for testing of the form encoder
#include <http.h>
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif


/**
 *  Derived test case class : test form encoder
 */
class CFormEncoderTest: public CHttpTestBase
	{
	// to implement in this class' member functs definitions
public:
	virtual ~CFormEncoderTest();
private:
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();

	void TestSinglesL();
	void TestPairsL();
	void TestSinglePairL(const TDesC8& aName, const TDesC8& aValue, TInt aExpectedEncodedLength);
	void TestEmptyNameL();
	void TestL(TBool aTestPass);
	};
