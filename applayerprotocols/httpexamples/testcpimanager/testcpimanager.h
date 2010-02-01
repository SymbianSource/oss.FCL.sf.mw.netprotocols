// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TESTCPIMANAGER_H__
#define __TESTCCPIMANAGER_H__

// System Includes
#include <http/mhttpcpimanager.h>


/** Test CPI Manager to be used to unit test the UAProf Filter
*/
class CTestCpiManager : public CBase, public MHTTPCpiManager
	{
public:

	IMPORT_C static CTestCpiManager* NewL(RStringPool aStringPool);
	
	IMPORT_C ~CTestCpiManager();
	
	IMPORT_C virtual RPointerArray<HBufC8>& GetProfiles();
	
	IMPORT_C virtual RPointerArray<HBufC8>& GetProfileDiffs();

	IMPORT_C void SetProfileDataL(HBufC8* aCpiData);

	IMPORT_C void SetProfileDiffDataL(HBufC8* aCpiData);

private:

	CTestCpiManager(RStringPool aStringPool);
	
	void ConstructL();


private:

	RStringPool iStringPool;
	
	RPointerArray<HBufC8> iProfileData;
	
	RPointerArray<HBufC8> iProfileDiffData;
	};


#endif //__TESTCPIMANAGER_H__
