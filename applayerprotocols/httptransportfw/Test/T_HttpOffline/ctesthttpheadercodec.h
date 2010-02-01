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

#ifndef __CTESTHTTPHEADERCODEC_H__
#define __CTESTHTTPHEADERCODEC_H__

#include <http/framework/cheadercodec.h>

#include "HttpTestCore.h"

class CTestHttpHeaderCodec : public CHttpTestBase
	{
public:
	virtual ~CTestHttpHeaderCodec();

	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();

	static CTestHttpHeaderCodec* NewLC();
	static CTestHttpHeaderCodec* NewL();
	void RunTestsL();

protected:
	CTestHttpHeaderCodec();
	void ConstructL();

private:
	void ResetTimeElapsed();
	void DisplayTimeElapsed();

	void TestEncodeWWWAuthenticateL();
	void TestDecodeAcceptL();
	void TestDecodeAcceptCharsetL();
	void TestDecodeUserAgentL();
	void TestDecodeAuthorizationL();
	void TestDecodeHostL();
	void TestDecodeExpectL();

	//
	TBool CompareStringL(RStringF aStr, const TDesC8& aDes);
	TBool CompareStringL(RString aStr, const TDesC8& aDes);
	TBool CompareDate(TDateTime aDate1, TDateTime aDate2);
	void ReleaseHeaders();

private:
	TTime			iLastTimeStamp;
	RStringPool		iStrP;
	CHeaderCodec*	iCodec;
	};

#endif	// __CTESTHTTPHEADERCODEC_H__
