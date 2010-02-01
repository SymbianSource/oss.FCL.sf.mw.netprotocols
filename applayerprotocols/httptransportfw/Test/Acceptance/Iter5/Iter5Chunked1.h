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


#ifndef __ITER5CHUNKED1_H__
#define __ITER5CHUNKED1_H__

#include "HttpTestCore.h"
#include "CHttpAccepTtestBase.h"
#include "CHttpTestChunkedBase.h"
#include <http.h>
#include "csrvaddrval.h"

/*	post content which requires authorisation
	and provide valid username/password (scooby/doo) */


class CHttpTestCaseIter5Chunked1: public CHttpTestChunkedBase 
	{
public:
	CHttpTestCaseIter5Chunked1(CScriptFile* aIniSettingsFile);
	TInt OverallDataSize();
	TBool GetNextDataPart(TPtrC8& aDataChunk);
protected:	
	const TDesC& TestName();
private:
	TBool iUseChunkedTransferEncoding;
	TInt iDataChunkCount;
	};


#endif 
