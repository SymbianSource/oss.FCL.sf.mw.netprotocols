// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "Iter5Chunked2.h"
#include <http.h>

CHttpTestCaseIter5Chunked2::CHttpTestCaseIter5Chunked2(CScriptFile* aIniSettingsFile) :
	CHttpTestChunkedBase()
	{
	iIniSettingsFile = aIniSettingsFile;
	}

const TDesC& CHttpTestCaseIter5Chunked2::TestName()
	{
	_LIT(KHeaderTestName,"Iteration 5 Chunked 2 ");
	return KHeaderTestName;
	}

TInt CHttpTestCaseIter5Chunked2::OverallDataSize()
	{
	return 9;
	}

TBool CHttpTestCaseIter5Chunked2::GetNextDataPart(TPtrC8& aDataChunk)
	{
	_LIT8(KHttpPostBodyChunk1,"ABC");
	_LIT8(KHttpPostBodyChunk2,"DEF");
	_LIT8(KHttpPostBodyChunk3,"GHJ");

	TBool retVal = EFalse;
	switch (iReqBodyChunkCount)
		{
	case 0:
		{
		aDataChunk.Set(KHttpPostBodyChunk1());
		} break;
	case 1:
		{
		aDataChunk.Set(KHttpPostBodyChunk2());
		} break;
	case 2:
		{
		aDataChunk.Set(KNullDesC8);
		} break;
	case 3:
		{
		aDataChunk.Set(KHttpPostBodyChunk3());
		} break;
	case 4:
		{
		aDataChunk.Set(KNullDesC8);
		iLastChunk = ETrue;
		retVal = ETrue;
		} break;
		}
	return retVal;
	}
