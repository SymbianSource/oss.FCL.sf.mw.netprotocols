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

// System Includes
#include <http/thttphdrval.h>

// User Includes
#include "testcpimanager.h"



EXPORT_C CTestCpiManager* CTestCpiManager::NewL(RStringPool aStringPool)
	{
	CTestCpiManager* cpiManager = new (ELeave) CTestCpiManager(aStringPool);
	CleanupStack::PushL(cpiManager);
	cpiManager->ConstructL();
	CleanupStack::Pop();
	return cpiManager;
	}
	
CTestCpiManager::CTestCpiManager(RStringPool aStringPool) :
	iStringPool(aStringPool)
	{
	}
	
void CTestCpiManager::ConstructL()
	{
	}	

EXPORT_C CTestCpiManager::~CTestCpiManager()
	{
	iProfileData.ResetAndDestroy();
	iProfileDiffData.ResetAndDestroy();
	}

EXPORT_C void CTestCpiManager::SetProfileDataL(HBufC8* aCpiData)
	{
	User::LeaveIfError(iProfileData.Append(aCpiData));
	}
	
EXPORT_C void CTestCpiManager::SetProfileDiffDataL(HBufC8* aCpiData)
	{
	User::LeaveIfError(iProfileDiffData.Append(aCpiData));
	}

EXPORT_C RPointerArray<HBufC8>& CTestCpiManager::GetProfiles()
	{
	return iProfileData;
	}
	
EXPORT_C RPointerArray<HBufC8>& CTestCpiManager::GetProfileDiffs()
	{
	return iProfileDiffData;
	}
	

