// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __T_HTTPHDR_H__
#define __T_HTTPHDR_H__

#include "httptestcore.h"

class CHttpHdrTest : public CHttpTestBase
	{
public:
	virtual ~CHttpHdrTest();
	static CHttpHdrTest* NewLC();
	static CHttpHdrTest* NewL();
	const TDesC& TestName();
	void DoRunL();
	void DoCancel();
protected:
	CHttpHdrTest();
	void ConstructL();
private:
	void RunTestsL();	
	void ResetTimeElapsed();
	void DisplayTimeElapsed();
	//
	void TestHdrValL();
	void TestBasicsL();
	void TestAddSinglePartLongL();
	void TestAddSinglePartQuickL();
	void TestAddMultiPartL();
	void TestAddParamL();
	void TestRemovePartL();
	void TestRemoveParamL();
	void TestLookupPartL();
	void TestLookupParamL();
	void TestHeaderCollL();
	void TestHeaderCollIterL();
	//
	TBool CompareStringL(RStringF aStr, const TDesC8& aDes);
	TBool CompareStringTokenL(RStringTokenF aStr, const TDesC8& aDes);
	TBool CompareDate(TDateTime aDate1, TDateTime aDate2);

	void TestL(TBool aTestCondition);

private:
	TTime iLastTimeStamp;
	RStringPool iStrP;
	CHeaders* iHdrColl;
	};

#endif
