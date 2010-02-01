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
// T_RespParse.cpp
// Definition of CResponseParserTestBase
// 
//

#ifndef __T_RESPPARSE_H__
#define __T_RESPPARSE_H__


#if !defined __E32TEST_H__
#include <e32test.h>
#endif

#ifndef __CTEXTMODERXDATA_H__
#include "CTextModeRxData.h"
#endif

#include "httptestcore.h"

class CResponseParserTestBase : public CHttpTestBase
	{
public:
	virtual ~CResponseParserTestBase();
protected:
	CResponseParserTestBase();
	void ConstructL(TInt aLineBufferSize);
	void DoSingleParseLoopL(const TDesC8& aRespData, RArray<TInt> aExpectedStatusCodes,
							TBool aValidateErrorCodes, TInt& aCodeIndex);
	void DumpStatusLineL();
	void DumpHeaderL();
	void DumpBody();
	void DumpString8L(const TDesC& aFmt, const TDesC8& aString8);
	virtual void HandleUnexpectedStatusCodeL(TInt aStat);
protected:
	TResponseParser iParse;
	HBufC8* iLineBuffer;
	};

class CRespParsTestSingleBuff : public CResponseParserTestBase
	{
public:
	virtual ~CRespParsTestSingleBuff();
	static CRespParsTestSingleBuff* NewLC();
	static CRespParsTestSingleBuff* NewL();
	void DoRunL();
	void DoCancel();
	const TDesC& TestName();
private:
	CRespParsTestSingleBuff();
	void ConstructL();
	};

class CRespParsTestSplitStatusLine : public CResponseParserTestBase
	{
public:
	virtual ~CRespParsTestSplitStatusLine();
	static CRespParsTestSplitStatusLine* NewLC();
	static CRespParsTestSplitStatusLine* NewL();
	void DoRunL();
	void DoCancel();
	const TDesC& TestName();
private:
	CRespParsTestSplitStatusLine();
	void ConstructL();
	};

class CRespParsTestMultipleBuffers : public CResponseParserTestBase
	{
public:
	virtual ~CRespParsTestMultipleBuffers();
	static CRespParsTestMultipleBuffers* NewLC();
	static CRespParsTestMultipleBuffers* NewL();
	void DoRunL();
	void DoCancel();
	const TDesC& TestName();
private:
	CRespParsTestMultipleBuffers();
	void ConstructL();
	};

class CRespParsTestMultiBodyNormal : public CResponseParserTestBase
	{
public:
	virtual ~CRespParsTestMultiBodyNormal();
	static CRespParsTestMultiBodyNormal* NewLC();
	static CRespParsTestMultiBodyNormal* NewL();
	void DoRunL();
	void DoCancel();
	const TDesC& TestName();
private:
	CRespParsTestMultiBodyNormal();
	void ConstructL();
	};

class CRespParsTestChunkedResponse : public CResponseParserTestBase
	{
public:
	virtual ~CRespParsTestChunkedResponse();
	static CRespParsTestChunkedResponse* NewLC();
	static CRespParsTestChunkedResponse* NewL();
	void DoRunL();
	void DoCancel();
	const TDesC& TestName();
private:
	CRespParsTestChunkedResponse();
	void ConstructL();
	};

class CRespParsTestApacheHeadResponse : public CResponseParserTestBase
	{
public:
	virtual ~CRespParsTestApacheHeadResponse();
	static CRespParsTestApacheHeadResponse* NewLC();
	static CRespParsTestApacheHeadResponse* NewL();
	void DoRunL();
	void DoCancel();
	const TDesC& TestName();
private:
	CRespParsTestApacheHeadResponse();
	void ConstructL();
	};

class CRespParsTestIISHeadResponse : public CResponseParserTestBase
	{
public:
	virtual ~CRespParsTestIISHeadResponse();
	static CRespParsTestIISHeadResponse* NewLC();
	static CRespParsTestIISHeadResponse* NewL();
	void DoRunL();
	void DoCancel();
	const TDesC& TestName();
private:
	CRespParsTestIISHeadResponse();
	void ConstructL();
	};

class CRespParsTestLineBuffOverrun : public CResponseParserTestBase
	{
public:
	virtual ~CRespParsTestLineBuffOverrun();
	static CRespParsTestLineBuffOverrun* NewLC();
	static CRespParsTestLineBuffOverrun* NewL();
	void DoRunL();
	void DoCancel();
	const TDesC& TestName();
private:
	CRespParsTestLineBuffOverrun();
	void ConstructL();
	virtual void HandleUnexpectedStatusCodeL(TInt aStat);
	};

class CRespParsTestCyclingRespBuffer : public CResponseParserTestBase
	{
public:
	virtual ~CRespParsTestCyclingRespBuffer();
	static CRespParsTestCyclingRespBuffer* NewLC(TInt aMinRespBuffSize, TInt aRespBuffInc);
	static CRespParsTestCyclingRespBuffer* NewL(TInt aMinRespBuffSize, TInt aRespBuffInc);
	void DoRunL();
	void DoCancel();
	const TDesC& TestName();
private:
	CRespParsTestCyclingRespBuffer(TInt aMinRespBuffSize, TInt aRespBuffInc);
	void ConstructL();
	virtual void HandleUnexpectedStatusCodeL(TInt aStat);
	void DoTestL(TInt aRespBuffSize);
private:
	TInt iMinRespBuffSize;
	TInt iRespBuffInc;
	};

#endif
