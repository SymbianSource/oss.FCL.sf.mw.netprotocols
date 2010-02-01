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

#ifndef __CTESTENGINE_H__
#define __CTESTENGINE_H__

#include <e32base.h>

#include "httptestutils.h"
#include "mdriverobserver.h"

/**	@class		CTestEngine
	@todo
	@since		8.0
*/
class CTestEngine : public CBase,
					public MDriverObserver
	{
public:		// methods

/**	@fn			NewL()
	Factory constructor.
	@since		8.0
	@return		A pointer to a fully initialised object.
*/
	static CTestEngine* NewL();

/**	@fn			~CTestEngine
	Destructor.
	@since		8.0
*/
	virtual ~CTestEngine();

/**	@fn			DoTestsL()
	Starts the test run.
	@since		8.0
*/
	void DoTestsL();
	virtual void Log(const TDesC& aComment);
	
private:	// methods from MDriverObserver

	virtual void NotifyError(TInt aError);
	virtual void NotifyStart();
	virtual void NotifyComplete();
	virtual void Dump(const TDesC8& aData);

private:	// typedefs

/**	@typedef	TTestFunctionL
	A typedef defining a pointer to a test function.
	@since		8.0
*/
	typedef void (CTestEngine::*TTestFunctionL) ();

private:	// methods

/**	@fn			CTestEngine()
	Constructor.
	@since		8.0
*/
	CTestEngine();

/**	@fn			ConstructL()
	Second phase constructor.
	@since		8.0
*/
	void ConstructL();

/**	@fn			DoOOMTest(TTestFunctionL aDoTestFunctionL)
	Does OOM testing on the test case specified. Will continue to re-start the
	specified test case until either the test case function does not leave and
	the test passes. If a test case does not pass under standard conditions, 
	then OOM testing of that test case should not be done.
	@since		8.0
	@param		aDoTestFunctionL	The function of the test case to be run.
*/
	void DoOOMTest(TTestFunctionL aDoTestFunctionL);
	
	void DoParseRawBadResponseTest1L();
	void DoParseRawBadResponseTest2L();
	void DoParseRawBadResponseTest3L();
	void DoParseRawBadResponseTest4L();
	void DoParseRawBadResponseTest5L();
	void DoParseRawBadResponseTest6L();
	void DoParseRawBadResponseTest7L();
	void DoParseRawBadResponseTest8L();
	void DoParseRawBadResponseTest9L();
	
	void DoParseSimpleMessageL();
	void DoParseNoEntityBodyL();
	void DoParseNonEncodedBodyL();
	void DoParseChunkEncodedBodyL();
	void DoParseChunkEncodedBodyWithTrailersL();
	void DoParseBadLineL();
	void DoParseBadEmptyLineL();
	void DoParseBadHeaderContinuationL();
	void DoParseBadChunkExtensionL();
	void DoParseBadChunkDataL();
	void DoParseBadChunkSizeL();
	void DoParseMissingCRsL();
	void DoParseSpuriousCRsL();
	void DoParseLongHeaderL();
	void DoParseLeadingEmptyLineL();
	void DoParseNonChunkedResetL();
	void DoParseChunkedResetL();
	void DoParseChunkDataContainingOnlyCRsL();

	void DoComposeSimpleMessageL();
	void DoComposeNoEntityBodyL();
	void DoComposeNonEncodedBodyL();
	void DoComposeChunkEncodedBodyL();
	void DoComposeChunkEncodedBodyWithTrailersL();
	void DoComposeTooMuchEntityBodyDataL();
	void DoComposeTooLittleEntityBodyDataL();
	void DoComposeLongHeaderL();
	void DoComposeNonEncodedBodyResetL();
	void DoComposeChunkEncodedBodyWithTrailersResetL();
	
private:	// attributes

	CHTTPTestUtils*		iTestUtils;
	TInt				iTestCount;
	TBool				iTestFailed;
	TBool				iOOMTesting;
	TBool				iUseUnknownBodyLength;

	};

#endif	// __CTESTENGINE_H__
