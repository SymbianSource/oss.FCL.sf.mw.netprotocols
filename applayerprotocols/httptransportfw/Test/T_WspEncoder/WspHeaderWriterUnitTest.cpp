/*
* Copyright (c) 2001 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


/**
	@file WspHeaderWriterUnitTest.cpp
	Comments : The unit test class implementations for the CWspHeaderWriter class.
 */

#include "WspHeaderWriterUnitTest.h"

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterCreateAndDestroyUnitTest,"CWspHeaderWriter_CreateAndDestroy_UnitTest");

CWspHeaderWriter_CreateAndDestroy_UnitTest* CWspHeaderWriter_CreateAndDestroy_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_CreateAndDestroy_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_CreateAndDestroy_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_CreateAndDestroy_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_CreateAndDestroy_UnitTest::~CWspHeaderWriter_CreateAndDestroy_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_CreateAndDestroy_UnitTest::CWspHeaderWriter_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterCreateAndDestroyUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_CreateAndDestroy_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAccept);
	
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterUserAgentUnitTest,"CWspHeaderWriter_UserAgent_UnitTest");

CWspHeaderWriter_UserAgent_UnitTest* CWspHeaderWriter_UserAgent_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_UserAgent_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_UserAgent_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_UserAgent_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUserAgentValidator;
	iUserAgentValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_UserAgent_UnitTest::~CWspHeaderWriter_UserAgent_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUserAgentValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_UserAgent_UnitTest::CWspHeaderWriter_UserAgent_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterUserAgentUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_UserAgent_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EUserAgent);
	
	_LIT8(KTxtData, "user-agent-text");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtData);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtData);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iUserAgentValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iUserAgentValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterUserAgentNullTextUnitTest,"CWspHeaderWriter_UserAgentNullText_UnitTest");

CWspHeaderWriter_UserAgentNullText_UnitTest* CWspHeaderWriter_UserAgentNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_UserAgentNullText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_UserAgentNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_UserAgentNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUserAgentNullTextValidator;
	iUserAgentNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_UserAgentNullText_UnitTest::~CWspHeaderWriter_UserAgentNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUserAgentNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_UserAgentNullText_UnitTest::CWspHeaderWriter_UserAgentNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterUserAgentNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_UserAgentNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EUserAgent);
	
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8());
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iUserAgentNullTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iUserAgentNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAuthorizationBasicUnitTest,"CWspHeaderWriter_AuthorizationBasic_UnitTest");

CWspHeaderWriter_AuthorizationBasic_UnitTest* CWspHeaderWriter_AuthorizationBasic_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AuthorizationBasic_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AuthorizationBasic_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AuthorizationBasic_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAuthorizationBasicValidator;
	iAuthorizationBasicValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AuthorizationBasic_UnitTest::~CWspHeaderWriter_AuthorizationBasic_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAuthorizationBasicValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AuthorizationBasic_UnitTest::CWspHeaderWriter_AuthorizationBasic_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAuthorizationBasicUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AuthorizationBasic_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAuthorization);
	
	// Part1
	_LIT8(KTxtBasic, "Basic");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtBasic);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtUser, "myuserid");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtUser);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	// Part2
	_LIT8(KTxtPassword, "mypass");
	RStringF stringVal3 = iUTContext->iStrPool.OpenFStringL(KTxtPassword);
	CleanupClosePushL(stringVal3);
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(stringVal3);
	CleanupStack::PushL(part3);
	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::Pop(part3);
	CleanupStack::PopAndDestroy(&stringVal3);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x11); // value-length for 17
	expectedBuf.Append(0x80);
	expectedBuf.Append(KTxtUser);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtPassword);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAuthorizationBasicValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAuthorizationBasicValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAuthorizationBasicNullUserUnitTest,"CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest");

CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest* CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAuthorizationBasicNullUserValidator;
	iAuthorizationBasicNullUserValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest::~CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAuthorizationBasicNullUserValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest::CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAuthorizationBasicNullUserUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAuthorization);
	
	// Part1
	_LIT8(KTxtBasic, "Basic");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtBasic);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KNullDesC8());
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	// Part2
	_LIT8(KTxtPassword, "mypass");
	RStringF stringVal3 = iUTContext->iStrPool.OpenFStringL(KTxtPassword);
	CleanupClosePushL(stringVal3);
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(stringVal3);
	CleanupStack::PushL(part3);
	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::Pop(part3);
	CleanupStack::PopAndDestroy(&stringVal3);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x09); // value-length for 9
	expectedBuf.Append(0x80);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtPassword);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAuthorizationBasicNullUserValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAuthorizationBasicNullUserValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterBasicNullPasswordUnitTest,"CWspHeaderWriter_BasicNullPassword_UnitTest");

CWspHeaderWriter_BasicNullPassword_UnitTest* CWspHeaderWriter_BasicNullPassword_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_BasicNullPassword_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_BasicNullPassword_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_BasicNullPassword_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iBasicNullPasswordValidator;
	iBasicNullPasswordValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_BasicNullPassword_UnitTest::~CWspHeaderWriter_BasicNullPassword_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iBasicNullPasswordValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_BasicNullPassword_UnitTest::CWspHeaderWriter_BasicNullPassword_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterBasicNullPasswordUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_BasicNullPassword_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAuthorization);
	
	// Part1
	_LIT8(KTxtBasic, "Basic");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtBasic);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtUser, "myuserid");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtUser);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	// Part2
	RStringF stringVal3 = iUTContext->iStrPool.OpenFStringL(KNullDesC8());
	CleanupClosePushL(stringVal3);
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(stringVal3);
	CleanupStack::PushL(part3);
	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::Pop(part3);
	CleanupStack::PopAndDestroy(&stringVal3);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x0B); // value-length for 11
	expectedBuf.Append(0x80);
	expectedBuf.Append(KTxtUser);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iBasicNullPasswordValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iBasicNullPasswordValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterBasicNullUserNullPassUnitTest,"CWspHeaderWriter_BasicNullUserNullPass_UnitTest");

CWspHeaderWriter_BasicNullUserNullPass_UnitTest* CWspHeaderWriter_BasicNullUserNullPass_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_BasicNullUserNullPass_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_BasicNullUserNullPass_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_BasicNullUserNullPass_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iBasicNullUserNullPassValidator;
	iBasicNullUserNullPassValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_BasicNullUserNullPass_UnitTest::~CWspHeaderWriter_BasicNullUserNullPass_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iBasicNullUserNullPassValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_BasicNullUserNullPass_UnitTest::CWspHeaderWriter_BasicNullUserNullPass_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterBasicNullUserNullPassUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_BasicNullUserNullPass_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAuthorization);
	
	// Part1
	_LIT8(KTxtBasic, "Basic");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtBasic);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KNullDesC8());
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	// Part2
	RStringF stringVal3 = iUTContext->iStrPool.OpenFStringL(KNullDesC8());
	CleanupClosePushL(stringVal3);
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(stringVal3);
	CleanupStack::PushL(part3);
	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::Pop(part3);
	CleanupStack::PopAndDestroy(&stringVal3);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value-length for 3
	expectedBuf.Append(0x80);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iBasicNullUserNullPassValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iBasicNullUserNullPassValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAuthorizationSchemeNoParamUnitTest,"CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest");

CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest* CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAuthorizationSchemeNoParamValidator;
	iAuthorizationSchemeNoParamValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest::~CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAuthorizationSchemeNoParamValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest::CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAuthorizationSchemeNoParamUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAuthorization);
	
	// Part1
	_LIT8(KTxtScheme, "authscheme");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtScheme);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x0B); // value-length for 11
	expectedBuf.Append(KTxtScheme);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAuthorizationSchemeNoParamValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAuthorizationSchemeNoParamValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAuthorizationInvalidSchemeTokenUnitTest,"CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest");

CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest* CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAuthorizationInvalidSchemeTokenValidator;
	iAuthorizationInvalidSchemeTokenValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest::~CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAuthorizationInvalidSchemeTokenValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest::CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAuthorizationInvalidSchemeTokenUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAuthorization);
	
	// Part1
	_LIT8(KTxtScheme, "auth@scheme");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtScheme);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	_LIT8(KTxtExpected, "-20");
	expectedBuf.Append(KTxtExpected);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAuthorizationInvalidSchemeTokenValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderLTrap_Transition(*iUTContext,*iAuthorizationInvalidSchemeTokenValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAuthorizationScheme2ParamsUnitTest,"CWspHeaderWriter_AuthorizationScheme2Params_UnitTest");

CWspHeaderWriter_AuthorizationScheme2Params_UnitTest* CWspHeaderWriter_AuthorizationScheme2Params_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AuthorizationScheme2Params_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AuthorizationScheme2Params_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AuthorizationScheme2Params_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAuthorizationScheme2ParamsValidator;
	iAuthorizationScheme2ParamsValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AuthorizationScheme2Params_UnitTest::~CWspHeaderWriter_AuthorizationScheme2Params_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAuthorizationScheme2ParamsValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AuthorizationScheme2Params_UnitTest::CWspHeaderWriter_AuthorizationScheme2Params_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAuthorizationScheme2ParamsUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AuthorizationScheme2Params_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAuthorization);
	
	// Part1
	_LIT8(KTxtScheme, "authscheme");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtScheme);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtParam1Name, "Name");
	_LIT8(KTxtParam1Value, "myname");
	RStringF paramName1 = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(paramName1);
	RStringF paramValueStr1 = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(paramValueStr1);
	THTTPHdrVal paramValue1(paramValueStr1);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(paramName1, paramValue1);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	// Param2
	_LIT8(KTxtParam2Name, "Q");
	_LIT8(KTxtParam2Value, "0.333");
	RStringF paramName2 = iUTContext->iStrPool.OpenFStringL(KTxtParam2Name);
	CleanupClosePushL(paramName2);
	RStringF paramValueStr2 = iUTContext->iStrPool.OpenFStringL(KTxtParam2Value);
	CleanupClosePushL(paramValueStr2);
	THTTPHdrVal paramValue2(paramValueStr2);
	CHeaderFieldParam* param2 = CHeaderFieldParam::NewL(paramName2, paramValue2);
	CleanupStack::PushL(param2);
	part1->AddParamL(param2);
	CleanupStack::Pop(param2);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(4, &paramName1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x16); // value length for 22
	expectedBuf.Append(KTxtScheme);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x97); // token for param1 "Name" using encoding version 1.4
	expectedBuf.Append(KTxtParam1Value);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x80); // token for param2 "Q"
	expectedBuf.Append(0x83); // encoding for 0.333...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAuthorizationScheme2ParamsValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAuthorizationScheme2ParamsValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAuthorizationScheme2ParamV1_2UnitTest,"CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest");

CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest* CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAuthorizationScheme2ParamV1_2Validator;
	iAuthorizationScheme2ParamV1_2Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest::~CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAuthorizationScheme2ParamV1_2Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest::CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAuthorizationScheme2ParamV1_2UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAuthorization);
	(REINTERPRET_CAST(CWspHeaderCodec*, iUTContext->iCodec))->SetWspVersion(CWspHeaderCodec::EVersion1_2);
	
	// Part1
	_LIT8(KTxtScheme, "authscheme");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtScheme);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtParam1Name, "Name");
	_LIT8(KTxtParam1Value, "myname");
	RStringF paramName1 = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(paramName1);
	RStringF paramValueStr1 = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(paramValueStr1);
	THTTPHdrVal paramValue1(paramValueStr1);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(paramName1, paramValue1);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	// Param2
	_LIT8(KTxtParam2Name, "Q");
	_LIT8(KTxtParam2Value, "0.333");
	RStringF paramName2 = iUTContext->iStrPool.OpenFStringL(KTxtParam2Name);
	CleanupClosePushL(paramName2);
	RStringF paramValueStr2 = iUTContext->iStrPool.OpenFStringL(KTxtParam2Value);
	CleanupClosePushL(paramValueStr2);
	THTTPHdrVal paramValue2(paramValueStr2);
	CHeaderFieldParam* param2 = CHeaderFieldParam::NewL(paramName2, paramValue2);
	CleanupStack::PushL(param2);
	part1->AddParamL(param2);
	CleanupStack::Pop(param2);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(4, &paramName1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x16); // value length for 22
	expectedBuf.Append(KTxtScheme);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x85); // token for param1 "Name" using encoding version 1.2
	expectedBuf.Append(KTxtParam1Value);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x80); // token for param2 "Q"
	expectedBuf.Append(0x83); // encoding for 0.333...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAuthorizationScheme2ParamV1_2Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAuthorizationScheme2ParamV1_2Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentLocationUnitTest,"CWspHeaderWriter_ContentLocation_UnitTest");

CWspHeaderWriter_ContentLocation_UnitTest* CWspHeaderWriter_ContentLocation_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentLocation_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentLocation_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentLocation_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLocationValidator;
	iContentLocationValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentLocation_UnitTest::~CWspHeaderWriter_ContentLocation_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLocationValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentLocation_UnitTest::CWspHeaderWriter_ContentLocation_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentLocationUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentLocation_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLocation);
	
	// Part1
	_LIT8(KTxtUri, "http://www.symbian.com");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtUri);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtUri);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLocationValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentLocationValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentLocationNullTextUnitTest,"CWspHeaderWriter_ContentLocationNullText_UnitTest");

CWspHeaderWriter_ContentLocationNullText_UnitTest* CWspHeaderWriter_ContentLocationNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentLocationNullText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentLocationNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentLocationNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLocationNullTextValidator;
	iContentLocationNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentLocationNullText_UnitTest::~CWspHeaderWriter_ContentLocationNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLocationNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentLocationNullText_UnitTest::CWspHeaderWriter_ContentLocationNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentLocationNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentLocationNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLocation);
	
	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8());
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLocationNullTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentLocationNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentTypeShortIntUnitTest,"CWspHeaderWriter_ContentTypeShortInt_UnitTest");

CWspHeaderWriter_ContentTypeShortInt_UnitTest* CWspHeaderWriter_ContentTypeShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentTypeShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentTypeShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentTypeShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeShortIntValidator;
	iContentTypeShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentTypeShortInt_UnitTest::~CWspHeaderWriter_ContentTypeShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentTypeShortInt_UnitTest::CWspHeaderWriter_ContentTypeShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentTypeShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentTypeShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/vnd.wap.wbxml");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0xA9); // encoded token for type above with top bit set
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeShortIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentTypeShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentTypeConstrainedTextUnitTest,"CWspHeaderWriter_ContentTypeConstrainedText_UnitTest");

CWspHeaderWriter_ContentTypeConstrainedText_UnitTest* CWspHeaderWriter_ContentTypeConstrainedText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentTypeConstrainedText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentTypeConstrainedText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentTypeConstrainedText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeConstrainedTextValidator;
	iContentTypeConstrainedTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentTypeConstrainedText_UnitTest::~CWspHeaderWriter_ContentTypeConstrainedText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeConstrainedTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentTypeConstrainedText_UnitTest::CWspHeaderWriter_ContentTypeConstrainedText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentTypeConstrainedTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentTypeConstrainedText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/my-app.exe");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtTypeToken);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeConstrainedTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentTypeConstrainedTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentTypeConstrainedNullTextUnitTest,"CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest");

CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest* CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeConstrainedNullTextValidator;
	iContentTypeConstrainedNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest::~CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeConstrainedNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest::CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentTypeConstrainedNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeConstrainedNullTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentTypeConstrainedNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentTypeGeneralShortInt1ParamUnitTest,"CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest");

CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest* CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeGeneralShortInt1ParamValidator;
	iContentTypeGeneralShortInt1ParamValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest::~CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeGeneralShortInt1ParamValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest::CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentTypeGeneralShortInt1ParamUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtParam1Name, "Level");
	_LIT8(KTxtParam1Value, "1.1");
	RStringF param1Name = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(param1Name);
	RStringF param1Value = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(param1Value);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(param1Name, param1Value);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &param1Name);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // length value
	expectedBuf.Append(0x90); // token for application
	expectedBuf.Append(0x82); // token for value param
	expectedBuf.Append(0x91); // token for version 1.1

	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeGeneralShortInt1ParamValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentTypeGeneralShortInt1ParamValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentTypeGeneralLongIntUnitTest,"CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest");

CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest* CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeGeneralLongIntValidator;
	iContentTypeGeneralLongIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest::~CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeGeneralLongIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest::CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentTypeGeneralLongIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/vnd.nokia.syncset+wbxml");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // length value
	expectedBuf.Append(0x02); // long int length
	expectedBuf.Append(0x02); // long int for application/vnd.nokia...
	expectedBuf.Append(0x0B); // ...

	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeGeneralLongIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentTypeGeneralLongIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentTypeGeneralLongInt2ParamsUnitTest,"CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest");

CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest* CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeGeneralLongInt2ParamsValidator;
	iContentTypeGeneralLongInt2ParamsValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest::~CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeGeneralLongInt2ParamsValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest::CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentTypeGeneralLongInt2ParamsUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/vnd.uplanet.cacheop-wbxml");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtParam1Name, "Read-date");
	TDateTime dateTime(1970, EJanuary, 0, 0, 0, 0, 0);
	TTime time(dateTime);
	time+=TTimeIntervalSeconds(654321);
	TDateTime param1Value = time.DateTime();
	RStringF paramName1 = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(paramName1);
	THTTPHdrVal paramValue1(param1Value);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(paramName1, paramValue1);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	// Param2
	_LIT8(KTxtParam2Name, "Charset");
	_LIT8(KTxtParam2Value, "utf-8");
	RStringF paramName2 = iUTContext->iStrPool.OpenFStringL(KTxtParam2Name);
	CleanupClosePushL(paramName2);
	RStringF paramValueStr2 = iUTContext->iStrPool.OpenFStringL(KTxtParam2Value);
	CleanupClosePushL(paramValueStr2);
	THTTPHdrVal paramValue2(paramValueStr2);
	CHeaderFieldParam* param2 = CHeaderFieldParam::NewL(paramName2, paramValue2);
	CleanupStack::PushL(param2);
	part1->AddParamL(param2);
	CleanupStack::Pop(param2);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(3, &paramName1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x0A); // length value
	expectedBuf.Append(0x02); // long int length
	expectedBuf.Append(0x02); // long int for application/vnd.nokia...
	expectedBuf.Append(0x01); // ...
	expectedBuf.Append(0x95); // encoded token for read-date
	expectedBuf.Append(0x03); // length of date value
	expectedBuf.Append(0x09);
	expectedBuf.Append(0xFB);
	expectedBuf.Append(0xF1);
	expectedBuf.Append(0x81); // encoded tiken for charset
	expectedBuf.Append(0xEA); // encoded token for 'utf-8'

	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeGeneralLongInt2ParamsValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentTypeGeneralLongInt2ParamsValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentTypeGeneralTextUnitTest,"CWspHeaderWriter_ContentTypeGeneralText_UnitTest");

CWspHeaderWriter_ContentTypeGeneralText_UnitTest* CWspHeaderWriter_ContentTypeGeneralText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentTypeGeneralText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentTypeGeneralText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentTypeGeneralText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeGeneralTextValidator;
	iContentTypeGeneralTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentTypeGeneralText_UnitTest::~CWspHeaderWriter_ContentTypeGeneralText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeGeneralTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentTypeGeneralText_UnitTest::CWspHeaderWriter_ContentTypeGeneralText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentTypeGeneralTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentTypeGeneralText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	_LIT8(KTxtTypeToken, "my-content-type");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	// Param1
	_LIT8(KTxtParam1Name, "Differences");
	_LIT8(KTxtParam1Value, "Pragma");
	RStringF param1Name = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(param1Name);
	RStringF param1Value = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(param1Value);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(param1Name, param1Value);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &param1Name);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x12); // length value
	expectedBuf.Append(KTxtTypeToken);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x87); // encoded token for 'Differences'
	expectedBuf.Append(0x9F); // encoded token for 'Pragma'

	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeGeneralTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentTypeGeneralTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentTypeGeneralNullTextUnitTest,"CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest");

CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest* CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentTypeGeneralNullTextValidator;
	iContentTypeGeneralNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest::~CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentTypeGeneralNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest::CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentTypeGeneralNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	// Param1
	_LIT8(KTxtParam1Name, "Level");
	_LIT8(KTxtParam1Value, "3");
	RStringF param1Name = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(param1Name);
	RStringF param1Value = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(param1Value);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(param1Name, param1Value);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &param1Name);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // length value
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x82); // encoded token for 'Level'
	expectedBuf.Append(0xBF); // encoded token for version 3

	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentTypeGeneralNullTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentTypeGeneralNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptCharsetShortIntUnitTest,"CWspHeaderWriter_AcceptCharsetShortInt_UnitTest");

CWspHeaderWriter_AcceptCharsetShortInt_UnitTest* CWspHeaderWriter_AcceptCharsetShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptCharsetShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptCharsetShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptCharsetShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptCharsetShortIntValidator;
	iAcceptCharsetShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptCharsetShortInt_UnitTest::~CWspHeaderWriter_AcceptCharsetShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptCharsetShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptCharsetShortInt_UnitTest::CWspHeaderWriter_AcceptCharsetShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptCharsetShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptCharsetShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptCharset);
	
	// Part1
	_LIT8(KTxtCharset, "iso-8859-9");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCharset);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x8C);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptCharsetShortIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptCharsetShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptCharsetTextUnitTest,"CWspHeaderWriter_AcceptCharsetText_UnitTest");

CWspHeaderWriter_AcceptCharsetText_UnitTest* CWspHeaderWriter_AcceptCharsetText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptCharsetText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptCharsetText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptCharsetText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptCharsetTextValidator;
	iAcceptCharsetTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptCharsetText_UnitTest::~CWspHeaderWriter_AcceptCharsetText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptCharsetTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptCharsetText_UnitTest::CWspHeaderWriter_AcceptCharsetText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptCharsetTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptCharsetText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptCharset);
	
	// Part1
	_LIT8(KTxtCharset, "my-new-charset");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCharset);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtCharset);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptCharsetTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptCharsetTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptCharsetAnyCharsetUnitTest,"CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest");

CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest* CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptCharsetAnyCharsetValidator;
	iAcceptCharsetAnyCharsetValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest::~CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptCharsetAnyCharsetValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest::CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptCharsetAnyCharsetUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptCharset);
	
	// Part1
	_LIT8(KTxtCharset, "*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCharset);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x80);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptCharsetAnyCharsetValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptCharsetAnyCharsetValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptCharsetGeneralShortIntWithQValUnitTest,"CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest");

CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest* CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptCharsetGeneralShortIntWithQValValidator;
	iAcceptCharsetGeneralShortIntWithQValValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest::~CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptCharsetGeneralShortIntWithQValValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest::CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptCharsetGeneralShortIntWithQValUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptCharset);
	
	// Part1
	_LIT8(KTxtCharset, "utf-8");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCharset);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // length-val
	expectedBuf.Append(0xEA); // Encoded token for utf-8
	expectedBuf.Append(0x83); // encoding for Q 0.333...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptCharsetGeneralShortIntWithQValValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptCharsetGeneralShortIntWithQValValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptCharsetGeneralLongIntQValUnitTest,"CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest");

CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest* CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptCharsetGeneralLongIntQValValidator;
	iAcceptCharsetGeneralLongIntQValValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest::~CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptCharsetGeneralLongIntQValValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest::CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptCharsetGeneralLongIntQValUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptCharset);
	
	// Part1
	_LIT8(KTxtCharset, "big5");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCharset);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x05); // length-val
	expectedBuf.Append(0x02); // length of long int
	expectedBuf.Append(0x07); // long int encoding of big5...
	expectedBuf.Append(0xEA); // ..
	expectedBuf.Append(0x83); // encoding for Q 0.333...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptCharsetGeneralLongIntQValValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptCharsetGeneralLongIntQValValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptCharsetGeneralTextQValUnitTest,"CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest");

CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest* CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptCharsetGeneralTextQValValidator;
	iAcceptCharsetGeneralTextQValValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest::~CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptCharsetGeneralTextQValValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest::CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptCharsetGeneralTextQValUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptCharset);
	
	// Part1
	_LIT8(KTxtCharset, "my-new-charset");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCharset);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x11); // length-val
	expectedBuf.Append(KTxtCharset); // charset text
	expectedBuf.Append(0x00); // null terminator
	expectedBuf.Append(0x83); // encoding for Q 0.333...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptCharsetGeneralTextQValValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptCharsetGeneralTextQValValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptCharsetAnyCharsetV1_2UnitTest,"CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest");

CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest* CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptCharsetAnyCharsetV1_2Validator;
	iAcceptCharsetAnyCharsetV1_2Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest::~CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptCharsetAnyCharsetV1_2Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest::CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptCharsetAnyCharsetV1_2UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptCharset);
	(REINTERPRET_CAST(CWspHeaderCodec*, iUTContext->iCodec))->SetWspVersion(CWspHeaderCodec::EVersion1_2);
	
	// Part1
	_LIT8(KTxtCharset, "*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCharset);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtCharset);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptCharsetAnyCharsetV1_2Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptCharsetAnyCharsetV1_2Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptLanguageShortIntUnitTest,"CWspHeaderWriter_AcceptLanguageShortInt_UnitTest");

CWspHeaderWriter_AcceptLanguageShortInt_UnitTest* CWspHeaderWriter_AcceptLanguageShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptLanguageShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptLanguageShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptLanguageShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptLanguageShortIntValidator;
	iAcceptLanguageShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptLanguageShortInt_UnitTest::~CWspHeaderWriter_AcceptLanguageShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptLanguageShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptLanguageShortInt_UnitTest::CWspHeaderWriter_AcceptLanguageShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptLanguageShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptLanguageShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptLanguage);
	
	// Part1
	_LIT8(KTxtLang, "en");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtLang);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x99); // encoded token for 'en'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptLanguageShortIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptLanguageShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptLanguageAnyLangUnitTest,"CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest");

CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest* CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptLanguageAnyLangValidator;
	iAcceptLanguageAnyLangValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest::~CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptLanguageAnyLangValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest::CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptLanguageAnyLangUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptLanguage);
	
	// Part1
	_LIT8(KTxtLang, "*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtLang);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x80); // encoded token for 'en'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptLanguageAnyLangValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptLanguageAnyLangValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptLanguageGeneralShortQValUnitTest,"CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest");

CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest* CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptLanguageGeneralShortQValValidator;
	iAcceptLanguageGeneralShortQValValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest::~CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptLanguageGeneralShortQValValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest::CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptLanguageGeneralShortQValUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptLanguage);
	
	// Part1
	_LIT8(KTxtLang, "en");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtLang);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	
	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);
	
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value-length
	expectedBuf.Append(0x99); // encoded token for 'en'
	expectedBuf.Append(0x83); // encoding for Q 0.333...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptLanguageGeneralShortQValValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptLanguageGeneralShortQValValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentLanguageGeneralLongIntUnitTest,"CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest");

CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest* CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLanguageGeneralLongIntValidator;
	iContentLanguageGeneralLongIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest::~CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLanguageGeneralLongIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest::CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentLanguageGeneralLongIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptLanguage);
	
	// Part1
	_LIT8(KTxtLang, "fy");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtLang);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x02); // value-length
	expectedBuf.Append(0x01); // long int encoded token for 'fy' ...
	expectedBuf.Append(0x83); // ..
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLanguageGeneralLongIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentLanguageGeneralLongIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptLanguageGeneralTextQValUnitTest,"CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest");

CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest* CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptLanguageGeneralTextQValValidator;
	iAcceptLanguageGeneralTextQValValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest::~CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptLanguageGeneralTextQValValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest::CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptLanguageGeneralTextQValUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptLanguage);
	
	// Part1
	_LIT8(KTxtLang, "my-new-language");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtLang);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	
	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);
	
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x12); // value-length
	expectedBuf.Append(KTxtLang);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x83); // encoding for Q 0.333...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptLanguageGeneralTextQValValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptLanguageGeneralTextQValValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptLanguageTextUnitTest,"CWspHeaderWriter_AcceptLanguageText_UnitTest");

CWspHeaderWriter_AcceptLanguageText_UnitTest* CWspHeaderWriter_AcceptLanguageText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptLanguageText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptLanguageText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptLanguageText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptLanguageTextValidator;
	iAcceptLanguageTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptLanguageText_UnitTest::~CWspHeaderWriter_AcceptLanguageText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptLanguageTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptLanguageText_UnitTest::CWspHeaderWriter_AcceptLanguageText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptLanguageTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptLanguageText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptLanguage);
	
	// Part1
	_LIT8(KTxtLang, "my-new-language");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtLang);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtLang);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptLanguageTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptLanguageTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentEncodingGZipUnitTest,"CWspHeaderWriter_ContentEncodingGZip_UnitTest");

CWspHeaderWriter_ContentEncodingGZip_UnitTest* CWspHeaderWriter_ContentEncodingGZip_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentEncodingGZip_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentEncodingGZip_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentEncodingGZip_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentEncodingGZipValidator;
	iContentEncodingGZipValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentEncodingGZip_UnitTest::~CWspHeaderWriter_ContentEncodingGZip_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentEncodingGZipValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentEncodingGZip_UnitTest::CWspHeaderWriter_ContentEncodingGZip_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentEncodingGZipUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentEncodingGZip_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentEncoding);
	
	// Part1
	_LIT8(KTxtEncoding, "Gzip");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x80);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentEncodingGZipValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentEncodingGZipValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentEncodingCompressUnitTest,"CWspHeaderWriter_ContentEncodingCompress_UnitTest");

CWspHeaderWriter_ContentEncodingCompress_UnitTest* CWspHeaderWriter_ContentEncodingCompress_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentEncodingCompress_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentEncodingCompress_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentEncodingCompress_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentEncodingCompressValidator;
	iContentEncodingCompressValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentEncodingCompress_UnitTest::~CWspHeaderWriter_ContentEncodingCompress_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentEncodingCompressValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentEncodingCompress_UnitTest::CWspHeaderWriter_ContentEncodingCompress_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentEncodingCompressUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentEncodingCompress_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentEncoding);
	
	// Part1
	_LIT8(KTxtEncoding, "Compress");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x81);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentEncodingCompressValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentEncodingCompressValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentEncodingDeflateUnitTest,"CWspHeaderWriter_ContentEncodingDeflate_UnitTest");

CWspHeaderWriter_ContentEncodingDeflate_UnitTest* CWspHeaderWriter_ContentEncodingDeflate_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentEncodingDeflate_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentEncodingDeflate_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentEncodingDeflate_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentEncodingDeflateValidator;
	iContentEncodingDeflateValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentEncodingDeflate_UnitTest::~CWspHeaderWriter_ContentEncodingDeflate_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentEncodingDeflateValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentEncodingDeflate_UnitTest::CWspHeaderWriter_ContentEncodingDeflate_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentEncodingDeflateUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentEncodingDeflate_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentEncoding);
	
	// Part1
	_LIT8(KTxtEncoding, "Deflate");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x82);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentEncodingDeflateValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentEncodingDeflateValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentEncodingTextUnitTest,"CWspHeaderWriter_ContentEncodingText_UnitTest");

CWspHeaderWriter_ContentEncodingText_UnitTest* CWspHeaderWriter_ContentEncodingText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentEncodingText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentEncodingText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentEncodingText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentEncodingTextValidator;
	iContentEncodingTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentEncodingText_UnitTest::~CWspHeaderWriter_ContentEncodingText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentEncodingTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentEncodingText_UnitTest::CWspHeaderWriter_ContentEncodingText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentEncodingTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentEncodingText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentEncoding);
	
	// Part1
	_LIT8(KTxtEncoding, "my-encoding");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtEncoding);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentEncodingTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentEncodingTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentLanguageShortIntUnitTest,"CWspHeaderWriter_ContentLanguageShortInt_UnitTest");

CWspHeaderWriter_ContentLanguageShortInt_UnitTest* CWspHeaderWriter_ContentLanguageShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentLanguageShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentLanguageShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentLanguageShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLanguageShortIntValidator;
	iContentLanguageShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentLanguageShortInt_UnitTest::~CWspHeaderWriter_ContentLanguageShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLanguageShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentLanguageShortInt_UnitTest::CWspHeaderWriter_ContentLanguageShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentLanguageShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentLanguageShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLanguage);
	
	// Part1
	_LIT8(KTxtEncoding, "su");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0xEF);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLanguageShortIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentLanguageShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentLanguageLongIntUnitTest,"CWspHeaderWriter_ContentLanguageLongInt_UnitTest");

CWspHeaderWriter_ContentLanguageLongInt_UnitTest* CWspHeaderWriter_ContentLanguageLongInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentLanguageLongInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentLanguageLongInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentLanguageLongInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLanguageLongIntValidator;
	iContentLanguageLongIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentLanguageLongInt_UnitTest::~CWspHeaderWriter_ContentLanguageLongInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLanguageLongIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentLanguageLongInt_UnitTest::CWspHeaderWriter_ContentLanguageLongInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentLanguageLongIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentLanguageLongInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLanguage);
	
	// Part1
	_LIT8(KTxtEncoding, "ie");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x01);
	expectedBuf.Append(0x86);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLanguageLongIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentLanguageLongIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentLanguageTextUnitTest,"CWspHeaderWriter_ContentLanguageText_UnitTest");

CWspHeaderWriter_ContentLanguageText_UnitTest* CWspHeaderWriter_ContentLanguageText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentLanguageText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentLanguageText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentLanguageText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentLanguageTextValidator;
	iContentLanguageTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentLanguageText_UnitTest::~CWspHeaderWriter_ContentLanguageText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentLanguageTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentLanguageText_UnitTest::CWspHeaderWriter_ContentLanguageText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentLanguageTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentLanguageText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentLanguage);
	
	// Part1
	_LIT8(KTxtEncoding, "my-new-language");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtEncoding);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentLanguageTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentLanguageTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentMD516ByteDataUnitTest,"CWspHeaderWriter_ContentMD516ByteData_UnitTest");

CWspHeaderWriter_ContentMD516ByteData_UnitTest* CWspHeaderWriter_ContentMD516ByteData_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentMD516ByteData_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentMD516ByteData_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentMD516ByteData_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentMD516ByteDataValidator;
	iContentMD516ByteDataValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentMD516ByteData_UnitTest::~CWspHeaderWriter_ContentMD516ByteData_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentMD516ByteDataValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentMD516ByteData_UnitTest::CWspHeaderWriter_ContentMD516ByteData_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentMD516ByteDataUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentMD516ByteData_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentMD5);
	
	// Part1
	_LIT8(KTxtEncoding, "encrypteddata_16");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x10); // value length
	expectedBuf.Append(KTxtEncoding);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentMD516ByteDataValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentMD516ByteDataValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentMD531ByteDataUnitTest,"CWspHeaderWriter_ContentMD531ByteData_UnitTest");

CWspHeaderWriter_ContentMD531ByteData_UnitTest* CWspHeaderWriter_ContentMD531ByteData_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentMD531ByteData_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentMD531ByteData_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentMD531ByteData_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentMD531ByteDataValidator;
	iContentMD531ByteDataValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentMD531ByteData_UnitTest::~CWspHeaderWriter_ContentMD531ByteData_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentMD531ByteDataValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentMD531ByteData_UnitTest::CWspHeaderWriter_ContentMD531ByteData_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentMD531ByteDataUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentMD531ByteData_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentMD5);
	
	// Part1
	_LIT8(KTxtEncoding, "my_encrypteddataencrypteddata31");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x1F); // value length <octet 31>
	expectedBuf.Append(0x1F); // value length 31 bytes
	expectedBuf.Append(KTxtEncoding);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentMD531ByteDataValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentMD531ByteDataValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterDateUnitTest,"CWspHeaderWriter_Date_UnitTest");

CWspHeaderWriter_Date_UnitTest* CWspHeaderWriter_Date_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_Date_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_Date_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_Date_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDateValidator;
	iDateValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_Date_UnitTest::~CWspHeaderWriter_Date_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDateValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_Date_UnitTest::CWspHeaderWriter_Date_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterDateUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_Date_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EDate);
	
	// Part1
	TDateTime dateTime(1970, EJanuary, 0, 0, 0, 0, 0);
	TTime time(dateTime);
	time+=TTimeIntervalSeconds(654321);
	TDateTime part1Value = time.DateTime();
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(part1Value);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value length
	expectedBuf.Append(0x09);
	expectedBuf.Append(0xFB);
	expectedBuf.Append(0xF1);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iDateValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iDateValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterFromTextUnitTest,"CWspHeaderWriter_FromText_UnitTest");

CWspHeaderWriter_FromText_UnitTest* CWspHeaderWriter_FromText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_FromText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_FromText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_FromText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iFromTextValidator;
	iFromTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_FromText_UnitTest::~CWspHeaderWriter_FromText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iFromTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_FromText_UnitTest::CWspHeaderWriter_FromText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterFromTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_FromText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EFrom);
	
	// Part1
	_LIT8(KTxtFrom, "from-example-value");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtFrom);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtFrom);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iFromTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iFromTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterFromNullTextUnitTest,"CWspHeaderWriter_FromNullText_UnitTest");

CWspHeaderWriter_FromNullText_UnitTest* CWspHeaderWriter_FromNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_FromNullText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_FromNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_FromNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iFromNullTextValidator;
	iFromNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_FromNullText_UnitTest::~CWspHeaderWriter_FromNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iFromNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_FromNullText_UnitTest::CWspHeaderWriter_FromNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterFromNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_FromNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EFrom);
	
	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iFromNullTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iFromNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterLastModifiedUnitTest,"CWspHeaderWriter_LastModified_UnitTest");

CWspHeaderWriter_LastModified_UnitTest* CWspHeaderWriter_LastModified_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_LastModified_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_LastModified_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_LastModified_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iLastModifiedValidator;
	iLastModifiedValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_LastModified_UnitTest::~CWspHeaderWriter_LastModified_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iLastModifiedValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_LastModified_UnitTest::CWspHeaderWriter_LastModified_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterLastModifiedUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_LastModified_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ELastModified);
	
	// Part1
	TDateTime dateTime(1970, EJanuary, 0, 0, 0, 0, 0);
	TTime time(dateTime);
	time+=TTimeIntervalSeconds(5654321);
	TDateTime part1Value = time.DateTime();
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(part1Value);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value length
	expectedBuf.Append(0x56);
	expectedBuf.Append(0x47);
	expectedBuf.Append(0x31);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iLastModifiedValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iLastModifiedValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterRangeByteRangeNoLastPosUnitTest,"CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest");

CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest* CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iRangeByteRangeNoLastPosValidator;
	iRangeByteRangeNoLastPosValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest::~CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iRangeByteRangeNoLastPosValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest::CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterRangeByteRangeNoLastPosUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ERange);
	
	// Part1
	_LIT8(KTxtRange, "Byte-range");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtRange);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	TInt firstBytePos = 145;
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(firstBytePos);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value length
	expectedBuf.Append(0x80); // encoded token for 'byte-range'
	expectedBuf.Append(0x81); // value-length for 145 ...
	expectedBuf.Append(0x11); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iRangeByteRangeNoLastPosValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iRangeByteRangeNoLastPosValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterRangeByteRangeWithLastPosUnitTest,"CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest");

CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest* CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iRangeByteRangeWithLastPosValidator;
	iRangeByteRangeWithLastPosValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest::~CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iRangeByteRangeWithLastPosValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest::CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterRangeByteRangeWithLastPosUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ERange);
	
	// Part1
	_LIT8(KTxtRange, "Byte-range");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtRange);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	TInt firstBytePos = 145;
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(firstBytePos);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);

	// Part3
	TInt lastBytePos = 300;
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(lastBytePos);
	CleanupStack::PushL(part3);
	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::Pop(part3);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x05); // value length
	expectedBuf.Append(0x80); // encoded token for 'byte-range'
	expectedBuf.Append(0x81); // value-length for 145 ...
	expectedBuf.Append(0x11); // ...
	expectedBuf.Append(0x82); // value-length for 300 ...
	expectedBuf.Append(0x2C); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iRangeByteRangeWithLastPosValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iRangeByteRangeWithLastPosValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterRangeSuffixRangeUnitTest,"CWspHeaderWriter_RangeSuffixRange_UnitTest");

CWspHeaderWriter_RangeSuffixRange_UnitTest* CWspHeaderWriter_RangeSuffixRange_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_RangeSuffixRange_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_RangeSuffixRange_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_RangeSuffixRange_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iRangeSuffixRangeValidator;
	iRangeSuffixRangeValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_RangeSuffixRange_UnitTest::~CWspHeaderWriter_RangeSuffixRange_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iRangeSuffixRangeValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_RangeSuffixRange_UnitTest::CWspHeaderWriter_RangeSuffixRange_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterRangeSuffixRangeUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_RangeSuffixRange_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ERange);
	
	// Part1
	_LIT8(KTxtRange, "Suffix-byte-range");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtRange);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	TInt firstBytePos = 145;
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(firstBytePos);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value length
	expectedBuf.Append(0x81); // encoded token for 'suffix-byte-range'
	expectedBuf.Append(0x81); // value-length for 145 ...
	expectedBuf.Append(0x11); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iRangeSuffixRangeValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iRangeSuffixRangeValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterRefererUriUnitTest,"CWspHeaderWriter_RefererUri_UnitTest");

CWspHeaderWriter_RefererUri_UnitTest* CWspHeaderWriter_RefererUri_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_RefererUri_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_RefererUri_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_RefererUri_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iRefererUriValidator;
	iRefererUriValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_RefererUri_UnitTest::~CWspHeaderWriter_RefererUri_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iRefererUriValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_RefererUri_UnitTest::CWspHeaderWriter_RefererUri_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterRefererUriUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_RefererUri_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EReferer);
	
	// Part1
	_LIT8(KTxtReferer, "http://www.symbian.com");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtReferer);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtReferer);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iRefererUriValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iRefererUriValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterRefererNullTextUnitTest,"CWspHeaderWriter_RefererNullText_UnitTest");

CWspHeaderWriter_RefererNullText_UnitTest* CWspHeaderWriter_RefererNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_RefererNullText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_RefererNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_RefererNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iRefererNullTextValidator;
	iRefererNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_RefererNullText_UnitTest::~CWspHeaderWriter_RefererNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iRefererNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_RefererNullText_UnitTest::CWspHeaderWriter_RefererNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterRefererNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_RefererNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EReferer);
	
	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iRefererNullTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iRefererNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterViaUriUnitTest,"CWspHeaderWriter_ViaUri_UnitTest");

CWspHeaderWriter_ViaUri_UnitTest* CWspHeaderWriter_ViaUri_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ViaUri_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ViaUri_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ViaUri_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iViaUriValidator;
	iViaUriValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ViaUri_UnitTest::~CWspHeaderWriter_ViaUri_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iViaUriValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ViaUri_UnitTest::CWspHeaderWriter_ViaUri_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterViaUriUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ViaUri_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EVia);
	
	// Part1
	_LIT8(KTxtReferer, "http://www.symbian.com");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtReferer);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtReferer);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iViaUriValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iViaUriValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterViaNullTextUnitTest,"CWspHeaderWriter_ViaNullText_UnitTest");

CWspHeaderWriter_ViaNullText_UnitTest* CWspHeaderWriter_ViaNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ViaNullText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ViaNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ViaNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iViaNullTextValidator;
	iViaNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ViaNullText_UnitTest::~CWspHeaderWriter_ViaNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iViaNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ViaNullText_UnitTest::CWspHeaderWriter_ViaNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterViaNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ViaNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EVia);
	
	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iViaNullTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iViaNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterXWapAppIdUriUnitTest,"CWspHeaderWriter_XWapAppIdUri_UnitTest");

CWspHeaderWriter_XWapAppIdUri_UnitTest* CWspHeaderWriter_XWapAppIdUri_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_XWapAppIdUri_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_XWapAppIdUri_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_XWapAppIdUri_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iXWapAppIdUriValidator;
	iXWapAppIdUriValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_XWapAppIdUri_UnitTest::~CWspHeaderWriter_XWapAppIdUri_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iXWapAppIdUriValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_XWapAppIdUri_UnitTest::CWspHeaderWriter_XWapAppIdUri_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterXWapAppIdUriUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_XWapAppIdUri_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EXWapApplicationId);
	
	// Part1
	_LIT8(KTxtUri, "http://www.symbian.com");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtUri);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtUri);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iXWapAppIdUriValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iXWapAppIdUriValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterXWapAppIdNullTextUnitTest,"CWspHeaderWriter_XWapAppIdNullText_UnitTest");

CWspHeaderWriter_XWapAppIdNullText_UnitTest* CWspHeaderWriter_XWapAppIdNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_XWapAppIdNullText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_XWapAppIdNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_XWapAppIdNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iXWapAppIdNullTextValidator;
	iXWapAppIdNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_XWapAppIdNullText_UnitTest::~CWspHeaderWriter_XWapAppIdNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iXWapAppIdNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_XWapAppIdNullText_UnitTest::CWspHeaderWriter_XWapAppIdNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterXWapAppIdNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_XWapAppIdNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EXWapApplicationId);
	
	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iXWapAppIdNullTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iXWapAppIdNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterXWapAppIdShortIntUnitTest,"CWspHeaderWriter_XWapAppIdShortInt_UnitTest");

CWspHeaderWriter_XWapAppIdShortInt_UnitTest* CWspHeaderWriter_XWapAppIdShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_XWapAppIdShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_XWapAppIdShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_XWapAppIdShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iXWapAppIdShortIntValidator;
	iXWapAppIdShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_XWapAppIdShortInt_UnitTest::~CWspHeaderWriter_XWapAppIdShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iXWapAppIdShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_XWapAppIdShortInt_UnitTest::CWspHeaderWriter_XWapAppIdShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterXWapAppIdShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_XWapAppIdShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EXWapApplicationId);
	
	// Part1
	TInt shortInt = 100;
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(shortInt);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0xE4); // encoded short int for 100
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iXWapAppIdShortIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iXWapAppIdShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterXWapAppIdLongIntUnitTest,"CWspHeaderWriter_XWapAppIdLongInt_UnitTest");

CWspHeaderWriter_XWapAppIdLongInt_UnitTest* CWspHeaderWriter_XWapAppIdLongInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_XWapAppIdLongInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_XWapAppIdLongInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_XWapAppIdLongInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iXWapAppIdLongIntValidator;
	iXWapAppIdLongIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_XWapAppIdLongInt_UnitTest::~CWspHeaderWriter_XWapAppIdLongInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iXWapAppIdLongIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_XWapAppIdLongInt_UnitTest::CWspHeaderWriter_XWapAppIdLongInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterXWapAppIdLongIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_XWapAppIdLongInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EXWapApplicationId);
	
	// Part1
	TInt longInt = 654321;
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(longInt);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // encoded long int for 654321 ...
	expectedBuf.Append(0x09); // ...
	expectedBuf.Append(0xFB); // ...
	expectedBuf.Append(0xF1); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iXWapAppIdLongIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iXWapAppIdLongIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterEncodingVersionUnitTest,"CWspHeaderWriter_EncodingVersion_UnitTest");

CWspHeaderWriter_EncodingVersion_UnitTest* CWspHeaderWriter_EncodingVersion_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_EncodingVersion_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_EncodingVersion_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_EncodingVersion_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iEncodingVersionValidator;
	iEncodingVersionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_EncodingVersion_UnitTest::~CWspHeaderWriter_EncodingVersion_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iEncodingVersionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_EncodingVersion_UnitTest::CWspHeaderWriter_EncodingVersion_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterEncodingVersionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_EncodingVersion_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EEncodingVersion);
	
	// Part1
	_LIT8(KTxtVersion, "1.4");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtVersion);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x94); // encoding for v1.4
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iEncodingVersionValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iEncodingVersionValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterEncodingVersionCodePageIntUnitTest,"CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest");

CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest* CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iEncodingVersionCodePageIntValidator;
	iEncodingVersionCodePageIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest::~CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iEncodingVersionCodePageIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest::CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterEncodingVersionCodePageIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EEncodingVersion);
	
	// Part1
	TInt codePage = 1;
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(codePage);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x01); // value length
	expectedBuf.Append(0x81); // encoding for short int 1
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iEncodingVersionCodePageIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iEncodingVersionCodePageIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterEncodingVersionCodePageIntWithVersionUnitTest,"CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest");

CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest* CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iEncodingVersionCodePageIntWithVersionValidator;
	iEncodingVersionCodePageIntWithVersionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest::~CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iEncodingVersionCodePageIntWithVersionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest::CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterEncodingVersionCodePageIntWithVersionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EEncodingVersion);
	
	// Part1
	TInt codePage = 1;
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(codePage);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	// Part2
	_LIT8(KTxtVersion, "1.4");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtVersion);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x02); // value length
	expectedBuf.Append(0x81); // encoding for code page short int 1
	expectedBuf.Append(0x94); // encoding for version 1.4
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iEncodingVersionCodePageIntWithVersionValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iEncodingVersionCodePageIntWithVersionValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterExpect100ContinueUnitTest,"CWspHeaderWriter_Expect100Continue_UnitTest");

CWspHeaderWriter_Expect100Continue_UnitTest* CWspHeaderWriter_Expect100Continue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_Expect100Continue_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_Expect100Continue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_Expect100Continue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iExpect100ContinueValidator;
	iExpect100ContinueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_Expect100Continue_UnitTest::~CWspHeaderWriter_Expect100Continue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iExpect100ContinueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_Expect100Continue_UnitTest::CWspHeaderWriter_Expect100Continue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterExpect100ContinueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_Expect100Continue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EExpect);
	
	// Part1
	_LIT8(KTxtExpect, "100-Continue");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtExpect);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x80); // encoding for '100-Continue'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iExpect100ContinueValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iExpect100ContinueValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterExpectExpressionTokenTextUnitTest,"CWspHeaderWriter_ExpectExpressionTokenText_UnitTest");

CWspHeaderWriter_ExpectExpressionTokenText_UnitTest* CWspHeaderWriter_ExpectExpressionTokenText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ExpectExpressionTokenText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ExpectExpressionTokenText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ExpectExpressionTokenText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iExpectExpressionTokenTextValidator;
	iExpectExpressionTokenTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ExpectExpressionTokenText_UnitTest::~CWspHeaderWriter_ExpectExpressionTokenText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iExpectExpressionTokenTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ExpectExpressionTokenText_UnitTest::CWspHeaderWriter_ExpectExpressionTokenText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterExpectExpressionTokenTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ExpectExpressionTokenText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EExpect);
	
	// Part1
	_LIT8(KTxtExpect, "expect-var");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtExpect);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtExpect2, "expect-value");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtExpect2);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x18); // value-length for 24 bytes
	expectedBuf.Append(KTxtExpect);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtExpect2);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iExpectExpressionTokenTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iExpectExpressionTokenTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterExpectExpressionQuotedTextUnitTest,"CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest");

CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest* CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iExpectExpressionQuotedTextValidator;
	iExpectExpressionQuotedTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest::~CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iExpectExpressionQuotedTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest::CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterExpectExpressionQuotedTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EExpect);
	
	// Part1
	_LIT8(KTxtExpect, "expect-var");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtExpect);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtExpect2, "\"expect-value\"");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtExpect2);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TPtrC8 quotedData(KTxtExpect2().Left(KTxtExpect2().Length()-1));
	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x19); // value-length for 25 bytes
	expectedBuf.Append(KTxtExpect);
	expectedBuf.Append(0x00);
	expectedBuf.Append(quotedData);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iExpectExpressionQuotedTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iExpectExpressionQuotedTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterExpectExpressionTokenTextParamUnitTest,"CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest");

CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest* CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iExpectExpressionTokenTextParamValidator;
	iExpectExpressionTokenTextParamValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest::~CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iExpectExpressionTokenTextParamValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest::CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterExpectExpressionTokenTextParamUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EExpect);
	
	// Part1
	_LIT8(KTxtExpect, "expect-var");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtExpect);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtExpect2, "expect-value");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtExpect2);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	_LIT8(KTxtParam1Name, "param-name");
	_LIT8(KTxtParam1Value, "param-value");
	RStringF param1Name = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(param1Name);
	RStringF param1Val = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(param1Val);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(param1Name, param1Val);
	CleanupStack::PushL(param1);
	part2->AddParamL(param1);
	CleanupStack::Pop(param1);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::PopAndDestroy(2, &param1Name);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x1F); // value-length for 47 bytes...
	expectedBuf.Append(0x2F); // ...
	expectedBuf.Append(KTxtExpect);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtExpect2);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParam1Name);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParam1Value);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iExpectExpressionTokenTextParamValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iExpectExpressionTokenTextParamValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterExpectExpressionQuotedTextParamUnitTest,"CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest");

CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest* CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iExpectExpressionQuotedTextParamValidator;
	iExpectExpressionQuotedTextParamValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest::~CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iExpectExpressionQuotedTextParamValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest::CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterExpectExpressionQuotedTextParamUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EExpect);
	
	// Part1
	_LIT8(KTxtExpect, "expect-var");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtExpect);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtExpect2, "expect-value");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtExpect2);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	_LIT8(KTxtParam1Name, "param-name");
	_LIT8(KTxtParam1Value, "\"param-value\"");
	RStringF param1Name = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(param1Name);
	RStringF param1Val = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(param1Val);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(param1Name, param1Val);
	CleanupStack::PushL(param1);
	part2->AddParamL(param1);
	CleanupStack::Pop(param1);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::PopAndDestroy(2, &param1Name);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x1F); // value-length for 48 bytes...
	expectedBuf.Append(0x30); // ...
	expectedBuf.Append(KTxtExpect);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtExpect2);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParam1Name);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParam1Value().Left(KTxtParam1Value().Length()-1));
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iExpectExpressionQuotedTextParamValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iExpectExpressionQuotedTextParamValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterExpectExpression2ParamsUnitTest,"CWspHeaderWriter_ExpectExpression2Params_UnitTest");

CWspHeaderWriter_ExpectExpression2Params_UnitTest* CWspHeaderWriter_ExpectExpression2Params_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ExpectExpression2Params_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ExpectExpression2Params_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ExpectExpression2Params_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iExpectExpression2ParamsValidator;
	iExpectExpression2ParamsValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ExpectExpression2Params_UnitTest::~CWspHeaderWriter_ExpectExpression2Params_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iExpectExpression2ParamsValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ExpectExpression2Params_UnitTest::CWspHeaderWriter_ExpectExpression2Params_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterExpectExpression2ParamsUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ExpectExpression2Params_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EExpect);
	
	// Part1
	_LIT8(KTxtExpect, "expect-var");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtExpect);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtExpect2, "expect-value");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtExpect2);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	// Param1
	_LIT8(KTxtParam1Name, "param1-name");
	_LIT8(KTxtParam1Value, "\"param1-value\"");
	RStringF param1Name = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(param1Name);
	RStringF param1Val = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(param1Val);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(param1Name, param1Val);
	CleanupStack::PushL(param1);
	part2->AddParamL(param1);
	CleanupStack::Pop(param1);
	// Param2
	_LIT8(KTxtParam2Name, "param2-name");
	_LIT8(KTxtParam2Value, "param2-value");
	RStringF param2Name = iUTContext->iStrPool.OpenFStringL(KTxtParam2Name);
	CleanupClosePushL(param2Name);
	RStringF param2Val = iUTContext->iStrPool.OpenFStringL(KTxtParam2Value);
	CleanupClosePushL(param2Val);
	CHeaderFieldParam* param2 = CHeaderFieldParam::NewL(param2Name, param2Val);
	CleanupStack::PushL(param2);
	part2->AddParamL(param2);
	CleanupStack::Pop(param2);

	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::PopAndDestroy(4, &param1Name);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x1F); // value-length for 75 bytes...
	expectedBuf.Append(0x4B); // ...
	expectedBuf.Append(KTxtExpect);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtExpect2);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParam1Name);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParam1Value().Left(KTxtParam1Value().Length()-1));
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParam2Name);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParam2Value);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iExpectExpression2ParamsValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iExpectExpression2ParamsValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTETrailersUnitTest,"CWspHeaderWriter_TETrailers_UnitTest");

CWspHeaderWriter_TETrailers_UnitTest* CWspHeaderWriter_TETrailers_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TETrailers_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TETrailers_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TETrailers_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTETrailersValidator;
	iTETrailersValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TETrailers_UnitTest::~CWspHeaderWriter_TETrailers_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTETrailersValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TETrailers_UnitTest::CWspHeaderWriter_TETrailers_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTETrailersUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TETrailers_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETE);
	
	// Part1
	_LIT8(KTxtTE, "Trailers");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTE);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x81); // encoded token for trailers
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTETrailersValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTETrailersValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTEChunkedUnitTest,"CWspHeaderWriter_TEChunked_UnitTest");

CWspHeaderWriter_TEChunked_UnitTest* CWspHeaderWriter_TEChunked_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TEChunked_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TEChunked_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TEChunked_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTEChunkedValidator;
	iTEChunkedValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TEChunked_UnitTest::~CWspHeaderWriter_TEChunked_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTEChunkedValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TEChunked_UnitTest::CWspHeaderWriter_TEChunked_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTEChunkedUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TEChunked_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETE);
	
	// Part1
	_LIT8(KTxtTE, "Chunked");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTE);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x01); // value-length
	expectedBuf.Append(0x82); // encoded token for chunked
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTEChunkedValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTEChunkedValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTEIdentitiyWithQValueUnitTest,"CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest");

CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest* CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTEIdentitiyWithQValueValidator;
	iTEIdentitiyWithQValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest::~CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTEIdentitiyWithQValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest::CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTEIdentitiyWithQValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETE);
	
	// Part1
	_LIT8(KTxtTE, "Identity");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTE);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	
	// Param1
	_LIT8(KTxtQParamName, "Q");
	_LIT8(KTxtQParamVal, "0.99");
	RStringF paramName = iUTContext->iStrPool.OpenFStringL(KTxtQParamName);
	CleanupClosePushL(paramName);
	RStringF paramVal = iUTContext->iStrPool.OpenFStringL(KTxtQParamVal);
	CleanupClosePushL(paramVal);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(paramName, paramVal);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &paramName);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value-length
	expectedBuf.Append(0x83); // encoded token for identity
	expectedBuf.Append(0x80); // encoded token for q-value
	expectedBuf.Append(0x64); // encoded token for 0.99 Q value
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTEIdentitiyWithQValueValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTEIdentitiyWithQValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTEGzipUnitTest,"CWspHeaderWriter_TEGzip_UnitTest");

CWspHeaderWriter_TEGzip_UnitTest* CWspHeaderWriter_TEGzip_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TEGzip_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TEGzip_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TEGzip_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTEGzipValidator;
	iTEGzipValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TEGzip_UnitTest::~CWspHeaderWriter_TEGzip_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTEGzipValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TEGzip_UnitTest::CWspHeaderWriter_TEGzip_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTEGzipUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TEGzip_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETE);
	
	// Part1
	_LIT8(KTxtTE, "Gzip");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTE);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x01); // value-length
	expectedBuf.Append(0x84); // encoded token for chunked
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTEGzipValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTEGzipValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTECompressWithQValueUnitTest,"CWspHeaderWriter_TECompressWithQValue_UnitTest");

CWspHeaderWriter_TECompressWithQValue_UnitTest* CWspHeaderWriter_TECompressWithQValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TECompressWithQValue_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TECompressWithQValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TECompressWithQValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTECompressWithQValueValidator;
	iTECompressWithQValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TECompressWithQValue_UnitTest::~CWspHeaderWriter_TECompressWithQValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTECompressWithQValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TECompressWithQValue_UnitTest::CWspHeaderWriter_TECompressWithQValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTECompressWithQValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TECompressWithQValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETE);
	
	// Part1
	_LIT8(KTxtTE, "Compress");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTE);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	
	// Param1
	_LIT8(KTxtQParamName, "Q");
	_LIT8(KTxtQParamVal, "0.1");
	RStringF paramName = iUTContext->iStrPool.OpenFStringL(KTxtQParamName);
	CleanupClosePushL(paramName);
	RStringF paramVal = iUTContext->iStrPool.OpenFStringL(KTxtQParamVal);
	CleanupClosePushL(paramVal);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(paramName, paramVal);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &paramName);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value-length
	expectedBuf.Append(0x85); // encoded token for compress
	expectedBuf.Append(0x80); // encoded token for q-value
	expectedBuf.Append(0x0B); // encoded token for 0.1 Q value
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTECompressWithQValueValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTECompressWithQValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTETokenTextUnitTest,"CWspHeaderWriter_TETokenText_UnitTest");

CWspHeaderWriter_TETokenText_UnitTest* CWspHeaderWriter_TETokenText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TETokenText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TETokenText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TETokenText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTETokenTextValidator;
	iTETokenTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TETokenText_UnitTest::~CWspHeaderWriter_TETokenText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTETokenTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TETokenText_UnitTest::CWspHeaderWriter_TETokenText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTETokenTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TETokenText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETE);
	
	// Part1
	_LIT8(KTxtTE, "my-new-TE-value");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTE);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x10); // value-length
	expectedBuf.Append(KTxtTE); // encoded token for chunked
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTETokenTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTETokenTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTETokenTextWithQValueUnitTest,"CWspHeaderWriter_TETokenTextWithQValue_UnitTest");

CWspHeaderWriter_TETokenTextWithQValue_UnitTest* CWspHeaderWriter_TETokenTextWithQValue_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TETokenTextWithQValue_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TETokenTextWithQValue_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TETokenTextWithQValue_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTETokenTextWithQValueValidator;
	iTETokenTextWithQValueValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TETokenTextWithQValue_UnitTest::~CWspHeaderWriter_TETokenTextWithQValue_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTETokenTextWithQValueValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TETokenTextWithQValue_UnitTest::CWspHeaderWriter_TETokenTextWithQValue_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTETokenTextWithQValueUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TETokenTextWithQValue_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETE);
	
	// Part1
	_LIT8(KTxtTE, "my-new-TE-value");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTE);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	
	// Param1
	_LIT8(KTxtQParamName, "Q");
	_LIT8(KTxtQParamVal, "0.333");
	RStringF paramName = iUTContext->iStrPool.OpenFStringL(KTxtQParamName);
	CleanupClosePushL(paramName);
	RStringF paramVal = iUTContext->iStrPool.OpenFStringL(KTxtQParamVal);
	CleanupClosePushL(paramVal);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(paramName, paramVal);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &paramName);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x13); // value-length
	expectedBuf.Append(KTxtTE); // encoded token for compress
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x80); // encoded token for q-value
	expectedBuf.Append(0x83); // encoded token for 0.333 Q value ...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTETokenTextWithQValueValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTETokenTextWithQValueValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTrailerShortIntUnitTest,"CWspHeaderWriter_TrailerShortInt_UnitTest");

CWspHeaderWriter_TrailerShortInt_UnitTest* CWspHeaderWriter_TrailerShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TrailerShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TrailerShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TrailerShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTrailerShortIntValidator;
	iTrailerShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TrailerShortInt_UnitTest::~CWspHeaderWriter_TrailerShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTrailerShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TrailerShortInt_UnitTest::CWspHeaderWriter_TrailerShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTrailerShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TrailerShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETrailer);
	
	// Part1
	_LIT8(KTxtTrailer, "Accept");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTrailer);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x80); // encoding for 'accept'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTrailerShortIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTrailerShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTrailerv1_4NewShortIntUnitTest,"CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest");

CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest* CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTrailerv1_4NewShortIntValidator;
	iTrailerv1_4NewShortIntValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest::~CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTrailerv1_4NewShortIntValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest::CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTrailerv1_4NewShortIntUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETrailer);
	
	// Part1
	_LIT8(KTxtTrailer, "Profile-warning");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTrailer);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0xC4); // encoding for 'profile-warning'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTrailerv1_4NewShortIntValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTrailerv1_4NewShortIntValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTrailerTokenTextUnitTest,"CWspHeaderWriter_TrailerTokenText_UnitTest");

CWspHeaderWriter_TrailerTokenText_UnitTest* CWspHeaderWriter_TrailerTokenText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TrailerTokenText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TrailerTokenText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TrailerTokenText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTrailerTokenTextValidator;
	iTrailerTokenTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TrailerTokenText_UnitTest::~CWspHeaderWriter_TrailerTokenText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTrailerTokenTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TrailerTokenText_UnitTest::CWspHeaderWriter_TrailerTokenText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTrailerTokenTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TrailerTokenText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETrailer);
	
	// Part1
	_LIT8(KTxtTrailer, "my-new-trailer-token");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTrailer);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtTrailer);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTrailerTokenTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTrailerTokenTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptEncodingGzipUnitTest,"CWspHeaderWriter_AcceptEncodingGzip_UnitTest");

CWspHeaderWriter_AcceptEncodingGzip_UnitTest* CWspHeaderWriter_AcceptEncodingGzip_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptEncodingGzip_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptEncodingGzip_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptEncodingGzip_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptEncodingGzipValidator;
	iAcceptEncodingGzipValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptEncodingGzip_UnitTest::~CWspHeaderWriter_AcceptEncodingGzip_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptEncodingGzipValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptEncodingGzip_UnitTest::CWspHeaderWriter_AcceptEncodingGzip_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptEncodingGzipUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptEncodingGzip_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptEncoding);
	
	// Part1
	_LIT8(KTxtAcceptEncoding, "Gzip");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAcceptEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x80);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptEncodingGzipValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptEncodingGzipValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptEncodingCompressUnitTest,"CWspHeaderWriter_AcceptEncodingCompress_UnitTest");

CWspHeaderWriter_AcceptEncodingCompress_UnitTest* CWspHeaderWriter_AcceptEncodingCompress_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptEncodingCompress_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptEncodingCompress_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptEncodingCompress_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptEncodingCompressValidator;
	iAcceptEncodingCompressValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptEncodingCompress_UnitTest::~CWspHeaderWriter_AcceptEncodingCompress_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptEncodingCompressValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptEncodingCompress_UnitTest::CWspHeaderWriter_AcceptEncodingCompress_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptEncodingCompressUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptEncodingCompress_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptEncoding);
	
	// Part1
	_LIT8(KTxtAcceptEncoding, "Compress");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAcceptEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x81);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptEncodingCompressValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptEncodingCompressValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptEncodingDeflateUnitTest,"CWspHeaderWriter_AcceptEncodingDeflate_UnitTest");

CWspHeaderWriter_AcceptEncodingDeflate_UnitTest* CWspHeaderWriter_AcceptEncodingDeflate_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptEncodingDeflate_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptEncodingDeflate_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptEncodingDeflate_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptEncodingDeflateValidator;
	iAcceptEncodingDeflateValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptEncodingDeflate_UnitTest::~CWspHeaderWriter_AcceptEncodingDeflate_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptEncodingDeflateValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptEncodingDeflate_UnitTest::CWspHeaderWriter_AcceptEncodingDeflate_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptEncodingDeflateUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptEncodingDeflate_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptEncoding);
	
	// Part1
	_LIT8(KTxtAcceptEncoding, "Deflate");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAcceptEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x82); // encoded token for deflate
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptEncodingDeflateValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptEncodingDeflateValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptEncodingTokenTextUnitTest,"CWspHeaderWriter_AcceptEncodingTokenText_UnitTest");

CWspHeaderWriter_AcceptEncodingTokenText_UnitTest* CWspHeaderWriter_AcceptEncodingTokenText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptEncodingTokenText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptEncodingTokenText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptEncodingTokenText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptEncodingTokenTextValidator;
	iAcceptEncodingTokenTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptEncodingTokenText_UnitTest::~CWspHeaderWriter_AcceptEncodingTokenText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptEncodingTokenTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptEncodingTokenText_UnitTest::CWspHeaderWriter_AcceptEncodingTokenText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptEncodingTokenTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptEncodingTokenText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptEncoding);
	
	// Part1
	_LIT8(KTxtAcceptEncoding, "my-accept-encoding-type");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAcceptEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtAcceptEncoding);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptEncodingTokenTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptEncodingTokenTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptEncodingGeneralAnyUnitTest,"CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest");

CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest* CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptEncodingGeneralAnyValidator;
	iAcceptEncodingGeneralAnyValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest::~CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptEncodingGeneralAnyValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest::CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptEncodingGeneralAnyUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptEncoding);
	
	// Part1
	_LIT8(KTxtAcceptEncoding, "*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAcceptEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x01);
	expectedBuf.Append(0x83);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptEncodingGeneralAnyValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptEncodingGeneralAnyValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptEncodingGeneralAnyQValUnitTest,"CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest");

CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest* CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptEncodingGeneralAnyQValValidator;
	iAcceptEncodingGeneralAnyQValValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest::~CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptEncodingGeneralAnyQValValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest::CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptEncodingGeneralAnyQValUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptEncoding);

	// Part1
	_LIT8(KTxtAcceptEncoding, "*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAcceptEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	
	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value-length
	expectedBuf.Append(0x83); // token for any '*'
	expectedBuf.Append(0x83); // token for for q-value 0.333...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptEncodingGeneralAnyQValValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptEncodingGeneralAnyQValValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptEncodingGeneralGzipQValUnitTest,"CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest");

CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest* CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptEncodingGeneralGzipQValValidator;
	iAcceptEncodingGeneralGzipQValValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest::~CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptEncodingGeneralGzipQValValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest::CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptEncodingGeneralGzipQValUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptEncoding);
	
	// Part1
	_LIT8(KTxtAcceptEncoding, "Gzip");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAcceptEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	
	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value-length
	expectedBuf.Append(0x80); // token for 'Gzip'
	expectedBuf.Append(0x83); // token for for q-value 0.333...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptEncodingGeneralGzipQValValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptEncodingGeneralGzipQValValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptEncodingGeneralAnyQValVersion1_2UnitTest,"CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest");

CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest* CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptEncodingGeneralAnyQValVersion1_2Validator;
	iAcceptEncodingGeneralAnyQValVersion1_2Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest::~CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptEncodingGeneralAnyQValVersion1_2Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest::CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptEncodingGeneralAnyQValVersion1_2UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptEncoding);
	REINTERPRET_CAST(CWspHeaderCodec*, iUTContext->iCodec)->SetWspVersion(CWspHeaderCodec::EVersion1_2);
	
	// Part1
	_LIT8(KTxtAcceptEncoding, "*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAcceptEncoding);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	
	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x04); // value-length
	expectedBuf.Append(KTxtAcceptEncoding);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x83); // token for for q-value 0.333...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptEncodingGeneralAnyQValVersion1_2Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptEncodingGeneralAnyQValVersion1_2Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentRangeUnitTest,"CWspHeaderWriter_ContentRange_UnitTest");

CWspHeaderWriter_ContentRange_UnitTest* CWspHeaderWriter_ContentRange_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentRange_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentRange_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentRange_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentRangeValidator;
	iContentRangeValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentRange_UnitTest::~CWspHeaderWriter_ContentRange_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentRangeValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentRange_UnitTest::CWspHeaderWriter_ContentRange_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentRangeUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentRange_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentRange);
	
	// Part1
	TInt firstBytePos = 65;
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(firstBytePos);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	// Part2
	TInt entityLength = 512;
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(entityLength);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x03); // value-length
	expectedBuf.Append(0x41);
	expectedBuf.Append(0x84);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentRangeValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentRangeValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentRangeUnknownLengthUnitTest,"CWspHeaderWriter_ContentRangeUnknownLength_UnitTest");

CWspHeaderWriter_ContentRangeUnknownLength_UnitTest* CWspHeaderWriter_ContentRangeUnknownLength_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentRangeUnknownLength_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentRangeUnknownLength_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentRangeUnknownLength_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentRangeUnknownLengthValidator;
	iContentRangeUnknownLengthValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentRangeUnknownLength_UnitTest::~CWspHeaderWriter_ContentRangeUnknownLength_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentRangeUnknownLengthValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentRangeUnknownLength_UnitTest::CWspHeaderWriter_ContentRangeUnknownLength_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentRangeUnknownLengthUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentRangeUnknownLength_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentRange);
	
	// Part1
	TInt firstBytePos = 65;
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(firstBytePos);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	// Part2
	_LIT8(KTxtUnknownLen, "*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtUnknownLen);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x02); // value-length
	expectedBuf.Append(0x41);
	expectedBuf.Append(0x80); // encoded token for '*'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentRangeUnknownLengthValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentRangeUnknownLengthValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAllowGetUnitTest,"CWspHeaderWriter_AllowGet_UnitTest");

CWspHeaderWriter_AllowGet_UnitTest* CWspHeaderWriter_AllowGet_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AllowGet_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AllowGet_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AllowGet_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAllowGetValidator;
	iAllowGetValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AllowGet_UnitTest::~CWspHeaderWriter_AllowGet_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAllowGetValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AllowGet_UnitTest::CWspHeaderWriter_AllowGet_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAllowGetUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AllowGet_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAllow);

	// Part1
	_LIT8(KTxtUnknownLen, "Get");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtUnknownLen);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0xC0);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAllowGetValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAllowGetValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAllowPostUnitTest,"CWspHeaderWriter_AllowPost_UnitTest");

CWspHeaderWriter_AllowPost_UnitTest* CWspHeaderWriter_AllowPost_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AllowPost_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AllowPost_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AllowPost_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAllowPostValidator;
	iAllowPostValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AllowPost_UnitTest::~CWspHeaderWriter_AllowPost_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAllowPostValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AllowPost_UnitTest::CWspHeaderWriter_AllowPost_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAllowPostUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AllowPost_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAllow);

	// Part1
	_LIT8(KTxtUnknownLen, "Post");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtUnknownLen);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0xE0);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAllowPostValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAllowPostValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAllowInvalidUnitTest,"CWspHeaderWriter_AllowInvalid_UnitTest");

CWspHeaderWriter_AllowInvalid_UnitTest* CWspHeaderWriter_AllowInvalid_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AllowInvalid_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AllowInvalid_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AllowInvalid_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAllowInvalidValidator;
	iAllowInvalidValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AllowInvalid_UnitTest::~CWspHeaderWriter_AllowInvalid_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAllowInvalidValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AllowInvalid_UnitTest::CWspHeaderWriter_AllowInvalid_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAllowInvalidUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AllowInvalid_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAllow);

	// Part1
	_LIT8(KTxtUnknownLen, "Invalid");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtUnknownLen);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	_LIT8(KTxtExpected, "-1");
	iUTContext->SetExpectedL(KTxtExpected);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAllowInvalidValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderLTrap_Transition(*iUTContext,*iAllowInvalidValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterPragmaNoCacheUnitTest,"CWspHeaderWriter_PragmaNoCache_UnitTest");

CWspHeaderWriter_PragmaNoCache_UnitTest* CWspHeaderWriter_PragmaNoCache_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_PragmaNoCache_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_PragmaNoCache_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_PragmaNoCache_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iPragmaNoCacheValidator;
	iPragmaNoCacheValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_PragmaNoCache_UnitTest::~CWspHeaderWriter_PragmaNoCache_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iPragmaNoCacheValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_PragmaNoCache_UnitTest::CWspHeaderWriter_PragmaNoCache_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterPragmaNoCacheUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_PragmaNoCache_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EPragma);

	// Part1
	_LIT8(KTxtUnknownLen, "No-cache");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtUnknownLen);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x80); // encoded token for 'no-cache;
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iPragmaNoCacheValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iPragmaNoCacheValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterPragmaParameterUnitTest,"CWspHeaderWriter_PragmaParameter_UnitTest");

CWspHeaderWriter_PragmaParameter_UnitTest* CWspHeaderWriter_PragmaParameter_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_PragmaParameter_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_PragmaParameter_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_PragmaParameter_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iPragmaParameterValidator;
	iPragmaParameterValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_PragmaParameter_UnitTest::~CWspHeaderWriter_PragmaParameter_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iPragmaParameterValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_PragmaParameter_UnitTest::CWspHeaderWriter_PragmaParameter_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterPragmaParameterUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_PragmaParameter_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EPragma);

	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtQParamName, "Q");
	_LIT8(KTxtQParamVal, "0.1");
	RStringF paramName = iUTContext->iStrPool.OpenFStringL(KTxtQParamName);
	CleanupClosePushL(paramName);
	RStringF paramVal = iUTContext->iStrPool.OpenFStringL(KTxtQParamVal);
	CleanupClosePushL(paramVal);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(paramName, paramVal);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &paramName);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x02); // value-length
	expectedBuf.Append(0x80); // Encoded token for 'Q'
	expectedBuf.Append(0x0B); // encoded token for '0.1'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iPragmaParameterValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iPragmaParameterValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterUpgradeTextUnitTest,"CWspHeaderWriter_UpgradeText_UnitTest");

CWspHeaderWriter_UpgradeText_UnitTest* CWspHeaderWriter_UpgradeText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_UpgradeText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_UpgradeText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_UpgradeText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUpgradeTextValidator;
	iUpgradeTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_UpgradeText_UnitTest::~CWspHeaderWriter_UpgradeText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUpgradeTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_UpgradeText_UnitTest::CWspHeaderWriter_UpgradeText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterUpgradeTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_UpgradeText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EUpgrade);

	// Part1
	_LIT8(KTxtUnknownLen, "upgrade-text");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtUnknownLen);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtUnknownLen);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iUpgradeTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iUpgradeTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterUpgradeNullTextUnitTest,"CWspHeaderWriter_UpgradeNullText_UnitTest");

CWspHeaderWriter_UpgradeNullText_UnitTest* CWspHeaderWriter_UpgradeNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_UpgradeNullText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_UpgradeNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_UpgradeNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUpgradeNullTextValidator;
	iUpgradeNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_UpgradeNullText_UnitTest::~CWspHeaderWriter_UpgradeNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUpgradeNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_UpgradeNullText_UnitTest::CWspHeaderWriter_UpgradeNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterUpgradeNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_UpgradeNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EUpgrade);

	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iUpgradeNullTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iUpgradeNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterWarning110UnitTest,"CWspHeaderWriter_Warning110_UnitTest");

CWspHeaderWriter_Warning110_UnitTest* CWspHeaderWriter_Warning110_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_Warning110_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_Warning110_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_Warning110_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWarning110Validator;
	iWarning110Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_Warning110_UnitTest::~CWspHeaderWriter_Warning110_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWarning110Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_Warning110_UnitTest::CWspHeaderWriter_Warning110_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterWarning110UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_Warning110_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWarning);

	// Part1
	TInt warningVal = 110;
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(warningVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x8A); // encoded token for '214'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iWarning110Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iWarning110Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterWarning214UnitTest,"CWspHeaderWriter_Warning214_UnitTest");

CWspHeaderWriter_Warning214_UnitTest* CWspHeaderWriter_Warning214_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_Warning214_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_Warning214_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_Warning214_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWarning214Validator;
	iWarning214Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_Warning214_UnitTest::~CWspHeaderWriter_Warning214_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWarning214Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_Warning214_UnitTest::CWspHeaderWriter_Warning214_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterWarning214UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_Warning214_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWarning);

	// Part1
	TInt warningVal = 214;
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(warningVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x8E); // encoded token for '214'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iWarning214Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iWarning214Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterWarningWarnValue100UnitTest,"CWspHeaderWriter_WarningWarnValue100_UnitTest");

CWspHeaderWriter_WarningWarnValue100_UnitTest* CWspHeaderWriter_WarningWarnValue100_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_WarningWarnValue100_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_WarningWarnValue100_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_WarningWarnValue100_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWarningWarnValue100Validator;
	iWarningWarnValue100Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_WarningWarnValue100_UnitTest::~CWspHeaderWriter_WarningWarnValue100_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWarningWarnValue100Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_WarningWarnValue100_UnitTest::CWspHeaderWriter_WarningWarnValue100_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterWarningWarnValue100UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_WarningWarnValue100_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWarning);

	// Part1
	TInt warningVal = 110;
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(warningVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	// Part2
	_LIT8(KTxtWarnAgent, "warn-agent");
	RStringF warnAgent = iUTContext->iStrPool.OpenFStringL(KTxtWarnAgent);
	CleanupClosePushL(warnAgent);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(warnAgent);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);

	// Part3
	_LIT8(KTxtWarnText, "warn-text");
	RStringF warnText = iUTContext->iStrPool.OpenFStringL(KTxtWarnText);
	CleanupClosePushL(warnText);
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(warnText);
	CleanupStack::PushL(part3);
	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::Pop(part3);

	CleanupStack::PopAndDestroy(2, &warnAgent);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x16); // value-length
	expectedBuf.Append(0x8A); // encoded token for '110'
	expectedBuf.Append(KTxtWarnAgent);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtWarnText);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);
	
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iWarningWarnValue100Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iWarningWarnValue100Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterWarningWarnValue110NullAgentUnitTest,"CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest");

CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest* CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iWarningWarnValue110NullAgentValidator;
	iWarningWarnValue110NullAgentValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest::~CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iWarningWarnValue110NullAgentValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest::CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterWarningWarnValue110NullAgentUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EWarning);

	// Part1
	TInt warningVal = 110;
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(warningVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);

	// Part2
	RStringF warnAgent = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(warnAgent);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(warnAgent);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);

	// Part3
	_LIT8(KTxtWarnText, "warn-text");
	RStringF warnText = iUTContext->iStrPool.OpenFStringL(KTxtWarnText);
	CleanupClosePushL(warnText);
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(warnText);
	CleanupStack::PushL(part3);
	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::Pop(part3);

	CleanupStack::PopAndDestroy(2, &warnAgent);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x0C); // value-length
	expectedBuf.Append(0x8A); // encoded token for '110'
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtWarnText);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);
	
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iWarningWarnValue110NullAgentValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iWarningWarnValue110NullAgentValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptConstrainedTokenUnitTest,"CWspHeaderWriter_AcceptConstrainedToken_UnitTest");

CWspHeaderWriter_AcceptConstrainedToken_UnitTest* CWspHeaderWriter_AcceptConstrainedToken_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptConstrainedToken_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptConstrainedToken_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptConstrainedToken_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptConstrainedTokenValidator;
	iAcceptConstrainedTokenValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptConstrainedToken_UnitTest::~CWspHeaderWriter_AcceptConstrainedToken_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptConstrainedTokenValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptConstrainedToken_UnitTest::CWspHeaderWriter_AcceptConstrainedToken_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptConstrainedTokenUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptConstrainedToken_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAccept);

	// Part1
	_LIT8(KTxtAccept, "*/*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAccept);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x80);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptConstrainedTokenValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptConstrainedTokenValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptConstrainedTextUnitTest,"CWspHeaderWriter_AcceptConstrainedText_UnitTest");

CWspHeaderWriter_AcceptConstrainedText_UnitTest* CWspHeaderWriter_AcceptConstrainedText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptConstrainedText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptConstrainedText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptConstrainedText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptConstrainedTextValidator;
	iAcceptConstrainedTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptConstrainedText_UnitTest::~CWspHeaderWriter_AcceptConstrainedText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptConstrainedTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptConstrainedText_UnitTest::CWspHeaderWriter_AcceptConstrainedText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptConstrainedTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptConstrainedText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAccept);

	// Part1
	_LIT8(KTxtAccept, "my-content-type");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAccept);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtAccept);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptConstrainedTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptConstrainedTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptGeneralTokenQValUnitTest,"CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest");

CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest* CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptGeneralTokenQValValidator;
	iAcceptGeneralTokenQValValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest::~CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptGeneralTokenQValValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest::CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptGeneralTokenQValUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAccept);

	// Part1
	_LIT8(KTxtAccept, "multipart/*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAccept);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x04); // value-length
	expectedBuf.Append(0x8B);
	expectedBuf.Append(0x80);
	expectedBuf.Append(0x83);
	expectedBuf.Append(0x31);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptGeneralTokenQValValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptGeneralTokenQValValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptGeneralLongInt2ParamsUnitTest,"CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest");

CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest* CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptGeneralLongInt2ParamsValidator;
	iAcceptGeneralLongInt2ParamsValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest::~CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptGeneralLongInt2ParamsValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest::CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptGeneralLongInt2ParamsUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAccept);

	// Part1
	_LIT8(KTxtAccept, "application/vnd.uplanet.signal");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAccept);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtParamName, "my-param");
	_LIT8(KTxtParamVal, "param-value");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtParamName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtParamVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	// Param2
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal3 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal3);
	RStringF stringVal4 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal4);
	CHeaderFieldParam* param2 = CHeaderFieldParam::NewL(stringVal3, stringVal4);
	CleanupStack::PushL(param2);
	part1->AddParamL(param2);
	CleanupStack::Pop(param2);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(4, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x1B); // value-length
	expectedBuf.Append(0x02); // long int encoding for type ...
	expectedBuf.Append(0x02); // ...
	expectedBuf.Append(0x02); // ...
	expectedBuf.Append(KTxtParamName);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParamVal);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x80); // encoding for q value ...
	expectedBuf.Append(0x83); // ...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptGeneralLongInt2ParamsValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptGeneralLongInt2ParamsValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptGeneralNullToken3ParamsUnitTest,"CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest");

CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest* CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptGeneralNullToken3ParamsValidator;
	iAcceptGeneralNullToken3ParamsValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest::~CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptGeneralNullToken3ParamsValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest::CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptGeneralNullToken3ParamsUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAccept);

	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtParamName, "my-param");
	_LIT8(KTxtParamVal, "param-value");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtParamName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtParamVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	// Param2
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal3 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal3);
	RStringF stringVal4 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal4);
	CHeaderFieldParam* param2 = CHeaderFieldParam::NewL(stringVal3, stringVal4);
	CleanupStack::PushL(param2);
	part1->AddParamL(param2);
	CleanupStack::Pop(param2);

	// Param3
	_LIT8(KTxtParam2Name, "Charset");
	_LIT8(KTxtParam2Val, "utf-8");
	RStringF stringVal5 = iUTContext->iStrPool.OpenFStringL(KTxtParam2Name);
	CleanupClosePushL(stringVal5);
	RStringF stringVal6 = iUTContext->iStrPool.OpenFStringL(KTxtParam2Val);
	CleanupClosePushL(stringVal6);
	CHeaderFieldParam* param3 = CHeaderFieldParam::NewL(stringVal5, stringVal6);
	CleanupStack::PushL(param3);
	part1->AddParamL(param3);
	CleanupStack::Pop(param3);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(6, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x1B); // value-length
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParamName);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParamVal);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x80); // encoding for q value ...
	expectedBuf.Append(0x83); // ...
	expectedBuf.Append(0x31); // ...
	expectedBuf.Append(0x81); // encoding for charset param ...
	expectedBuf.Append(0xEA); // ... utf-8
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptGeneralNullToken3ParamsValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptGeneralNullToken3ParamsValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterCookieVersionUnitTest,"CWspHeaderWriter_CookieVersion_UnitTest");

CWspHeaderWriter_CookieVersion_UnitTest* CWspHeaderWriter_CookieVersion_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_CookieVersion_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_CookieVersion_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_CookieVersion_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iCookieVersionValidator;
	iCookieVersionValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_CookieVersion_UnitTest::~CWspHeaderWriter_CookieVersion_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iCookieVersionValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_CookieVersion_UnitTest::CWspHeaderWriter_CookieVersion_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterCookieVersionUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_CookieVersion_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ECookie);

	// Part1
	_LIT8(KTxtCookie, "1.1");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCookie);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x01);
	expectedBuf.Append(0x91); // encoded version 1.1
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iCookieVersionValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iCookieVersionValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterCookieVersionNameValUnitTest,"CWspHeaderWriter_CookieVersionNameVal_UnitTest");

CWspHeaderWriter_CookieVersionNameVal_UnitTest* CWspHeaderWriter_CookieVersionNameVal_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_CookieVersionNameVal_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_CookieVersionNameVal_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_CookieVersionNameVal_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iCookieVersionNameValValidator;
	iCookieVersionNameValValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_CookieVersionNameVal_UnitTest::~CWspHeaderWriter_CookieVersionNameVal_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iCookieVersionNameValValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_CookieVersionNameVal_UnitTest::CWspHeaderWriter_CookieVersionNameVal_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterCookieVersionNameValUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_CookieVersionNameVal_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ECookie);

	// Part1
	_LIT8(KTxtCookie, "1.1");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCookie);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtCookieName, "cookie-name"); // 11 bytes
	RStringF nameStr = iUTContext->iStrPool.OpenFStringL(KTxtCookieName);
	CleanupClosePushL(nameStr);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(nameStr);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&nameStr);

	// Part3
	_LIT8(KTxtCookieVal, "cookie-value"); // 12 bytes
	RStringF valueStr = iUTContext->iStrPool.OpenFStringL(KTxtCookieVal);
	CleanupClosePushL(valueStr);
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(valueStr);
	CleanupStack::PushL(part3);
	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::Pop(part3);
	CleanupStack::PopAndDestroy(&valueStr);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x1B); // value-length
	expectedBuf.Append(0x91); // encoded version 1.1
	expectedBuf.Append(0x19); // cookie-length
	expectedBuf.Append(KTxtCookieName);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtCookieVal);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iCookieVersionNameValValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iCookieVersionNameValValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterCookieVersionNameNullValPathUnitTest,"CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest");

CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest* CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iCookieVersionNameNullValPathValidator;
	iCookieVersionNameNullValPathValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest::~CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iCookieVersionNameNullValPathValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest::CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterCookieVersionNameNullValPathUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ECookie);

	// Part1
	_LIT8(KTxtCookie, "1.1");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCookie);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtCookieName, "cookie-name"); // 11 bytes
	RStringF nameStr = iUTContext->iStrPool.OpenFStringL(KTxtCookieName);
	CleanupClosePushL(nameStr);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(nameStr);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&nameStr);

	// Part3
	RStringF valueStr = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(valueStr);
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(valueStr);
	CleanupStack::PushL(part3);

	// Param1
	_LIT8(KTxtPath, "Path");
	_LIT8(KTxtPathVal, "my-path"); // 7 bytes
	RStringF pathStr = iUTContext->iStrPool.OpenFStringL(KTxtPath);
	CleanupClosePushL(pathStr);
	RStringF pathValStr = iUTContext->iStrPool.OpenFStringL(KTxtPathVal);
	CleanupClosePushL(pathValStr);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(pathStr, pathValStr);
	CleanupStack::PushL(param1);
	part3->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::PopAndDestroy(2, &pathStr);
	CleanupStack::Pop(part3);
	CleanupStack::PopAndDestroy(&valueStr);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x18); // value-length
	expectedBuf.Append(0x91); // encoded version 1.1
	expectedBuf.Append(0x16); // cookie-length
	expectedBuf.Append(KTxtCookieName);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x9D); // encoded 'path'
	expectedBuf.Append(KTxtPathVal);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iCookieVersionNameNullValPathValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iCookieVersionNameNullValPathValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterCookieVaerionNameValNullPathDomainUnitTest,"CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest");

CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest* CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iCookieVaerionNameValNullPathDomainValidator;
	iCookieVaerionNameValNullPathDomainValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest::~CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iCookieVaerionNameValNullPathDomainValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest::CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterCookieVaerionNameValNullPathDomainUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ECookie);

	// Part1
	_LIT8(KTxtCookie, "1.1");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtCookie);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtCookieName, "cookie-name"); // 11 bytes
	RStringF nameStr = iUTContext->iStrPool.OpenFStringL(KTxtCookieName);
	CleanupClosePushL(nameStr);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(nameStr);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&nameStr);

	// Part3
	_LIT8(KTxtCookieValue, "cookie-value"); // 12 bytes
	RStringF valueStr = iUTContext->iStrPool.OpenFStringL(KTxtCookieValue);
	CleanupClosePushL(valueStr);
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(valueStr);
	CleanupStack::PushL(part3);

	// Param1
	_LIT8(KTxtPath, "Path");
	RStringF pathStr = iUTContext->iStrPool.OpenFStringL(KTxtPath);
	CleanupClosePushL(pathStr);
	RStringF pathValStr = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(pathValStr);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(pathStr, pathValStr);
	CleanupStack::PushL(param1);
	part3->AddParamL(param1);
	CleanupStack::Pop(param1);

	// Param2
	_LIT8(KTxtDomain, "Domain");
	_LIT8(KTxtDomainVal, "my-domain"); // 9 bytes
	RStringF domainStr = iUTContext->iStrPool.OpenFStringL(KTxtDomain);
	CleanupClosePushL(domainStr);
	RStringF domainValStr = iUTContext->iStrPool.OpenFStringL(KTxtDomainVal);
	CleanupClosePushL(domainValStr);
	CHeaderFieldParam* param2 = CHeaderFieldParam::NewL(domainStr, domainValStr);
	CleanupStack::PushL(param2);
	part3->AddParamL(param2);
	CleanupStack::Pop(param2);

	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::PopAndDestroy(4, &pathStr);
	CleanupStack::Pop(part3);
	CleanupStack::PopAndDestroy(&valueStr);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x1F); // value-length ...
	expectedBuf.Append(0x28); // ...
	expectedBuf.Append(0x91); // encoded version 1.1
	expectedBuf.Append(0x26); // cookie-length
	expectedBuf.Append(KTxtCookieName);
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtCookieValue);
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x9D); // encoded 'path'
	expectedBuf.Append(0x00);
	expectedBuf.Append(0x9C); // encoded 'domain'
	expectedBuf.Append(KTxtDomainVal);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iCookieVaerionNameValNullPathDomainValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iCookieVaerionNameValNullPathDomainValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptMultiHeaderUnitTest,"CWspHeaderWriter_AcceptMultiHeader_UnitTest");

CWspHeaderWriter_AcceptMultiHeader_UnitTest* CWspHeaderWriter_AcceptMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptMultiHeaderValidator;
	iAcceptMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptMultiHeader_UnitTest::~CWspHeaderWriter_AcceptMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptMultiHeader_UnitTest::CWspHeaderWriter_AcceptMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAccept);

	// Part1
	_LIT8(KTxtAccept, "multipart/*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAccept);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtAccept2, "*/*");
	RStringF stringValPt2 = iUTContext->iStrPool.OpenFStringL(KTxtAccept2);
	CleanupClosePushL(stringValPt2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringValPt2);
	CleanupStack::PushL(part2);

	// Param2
	RStringF stringVal3 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal3);
	RStringF stringVal4 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal4);
	CHeaderFieldParam* param2 = CHeaderFieldParam::NewL(stringVal3, stringVal4);
	CleanupStack::PushL(param2);
	part2->AddParamL(param2);
	CleanupStack::Pop(param2);

	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::PopAndDestroy(2, &stringVal3);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringValPt2);

	TBuf8<256> expectedBuf;
	//Part1
	expectedBuf.Append(0x04); // value-length
	expectedBuf.Append(0x8B); // token for 'multipart/*'
	expectedBuf.Append(0x80); // token for QVal token
	expectedBuf.Append(0x83); // token for '0.333' ...
	expectedBuf.Append(0x31); // ...
	//Part2
	expectedBuf.Append(0x80); // token for 'Accept'
	expectedBuf.Append(0x04); // value-length
	expectedBuf.Append(0x80); // token for '*/*'
	expectedBuf.Append(0x80); // token for QVal token
	expectedBuf.Append(0x83); // token for '0.333' ...
	expectedBuf.Append(0x31); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptMultiHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAcceptCharsetMultiHeaderUnitTest,"CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest");

CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest* CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAcceptCharsetMultiHeaderValidator;
	iAcceptCharsetMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest::~CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAcceptCharsetMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest::CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAcceptCharsetMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAcceptCharset);

	// Part1
	_LIT8(KTxtAcceptCharset, "utf-8");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAcceptCharset);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtQName, "Q");
	_LIT8(KTxtQVal, "0.333");
	RStringF stringVal1 = iUTContext->iStrPool.OpenFStringL(KTxtQName);
	CleanupClosePushL(stringVal1);
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtQVal);
	CleanupClosePushL(stringVal2);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(stringVal1, stringVal2);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &stringVal1);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtAcceptCharset2, "us-ascii");
	RStringF stringValPt2 = iUTContext->iStrPool.OpenFStringL(KTxtAcceptCharset2);
	CleanupClosePushL(stringValPt2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringValPt2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringValPt2);

	TBuf8<256> expectedBuf;
	//Part1
	expectedBuf.Append(0x03); // value-length
	expectedBuf.Append(0xEA); // token for 'utf-8'
	expectedBuf.Append(0x83); // token for '0.333' ...
	expectedBuf.Append(0x31); // ...
	//Part2
	expectedBuf.Append(0xBB); // token for 'Accept-charset'
	expectedBuf.Append(0x83); // token for 'us-ascii' ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAcceptCharsetMultiHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAcceptCharsetMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterRangeMultiHeaderUnitTest,"CWspHeaderWriter_RangeMultiHeader_UnitTest");

CWspHeaderWriter_RangeMultiHeader_UnitTest* CWspHeaderWriter_RangeMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_RangeMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_RangeMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_RangeMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iRangeMultiHeaderValidator;
	iRangeMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_RangeMultiHeader_UnitTest::~CWspHeaderWriter_RangeMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iRangeMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_RangeMultiHeader_UnitTest::CWspHeaderWriter_RangeMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterRangeMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_RangeMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ERange);
	
	// Part1
	_LIT8(KTxtRange, "Byte-range");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtRange);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	TInt firstBytePos = 145;
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(firstBytePos);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);

	// Part3
	TInt lastBytePos = 300;
	CHeaderFieldPart* part3 = CHeaderFieldPart::NewL(lastBytePos);
	CleanupStack::PushL(part3);
	iUTContext->iTestHeader->SetPartL(part3, 2);
	CleanupStack::Pop(part3);

	// Part4 - new header
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtRange);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part4 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part4);
	iUTContext->iTestHeader->SetPartL(part4, 3);
	CleanupStack::Pop(part4);
	CleanupStack::PopAndDestroy(&stringVal2);

	// Part5
	TInt firstBytePos2 = 145;
	CHeaderFieldPart* part5 = CHeaderFieldPart::NewL(firstBytePos2);
	CleanupStack::PushL(part5);
	iUTContext->iTestHeader->SetPartL(part5, 4);
	CleanupStack::Pop(part5);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x05); // value length
	expectedBuf.Append(0x80); // encoded token for 'byte-range'
	expectedBuf.Append(0x81); // value-length for 145 ...
	expectedBuf.Append(0x11); // ...
	expectedBuf.Append(0x82); // value-length for 300 ...
	expectedBuf.Append(0x2C); // ...

	expectedBuf.Append(0xA3); // token for 'range'
	expectedBuf.Append(0x03); // value length
	expectedBuf.Append(0x80); // encoded token for 'byte-range'
	expectedBuf.Append(0x81); // value-length for 145 ...
	expectedBuf.Append(0x11); // ...
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iRangeMultiHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iRangeMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTEMultiHeaderUnitTest,"CWspHeaderWriter_TEMultiHeader_UnitTest");

CWspHeaderWriter_TEMultiHeader_UnitTest* CWspHeaderWriter_TEMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TEMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TEMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TEMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTEMultiHeaderValidator;
	iTEMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TEMultiHeader_UnitTest::~CWspHeaderWriter_TEMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTEMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TEMultiHeader_UnitTest::CWspHeaderWriter_TEMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTEMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TEMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETE);
	
	// Part1
	_LIT8(KTxtTE, "Trailers");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTE);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtTE2, "Chunked");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtTE2);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x81); // encoded token for trailers
	expectedBuf.Append(0xB9); // encoded token for TE
	expectedBuf.Append(0x01); // value length
	expectedBuf.Append(0x82); // encoded token for chunked
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTEMultiHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTEMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterTrailerMultiHeaderUnitTest,"CWspHeaderWriter_TrailerMultiHeader_UnitTest");

CWspHeaderWriter_TrailerMultiHeader_UnitTest* CWspHeaderWriter_TrailerMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_TrailerMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_TrailerMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_TrailerMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iTrailerMultiHeaderValidator;
	iTrailerMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_TrailerMultiHeader_UnitTest::~CWspHeaderWriter_TrailerMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iTrailerMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_TrailerMultiHeader_UnitTest::CWspHeaderWriter_TrailerMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterTrailerMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_TrailerMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETrailer);
	
	// Part1
	_LIT8(KTxtTrailer, "Accept");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTrailer);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtTrailer2, "my-new-field");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtTrailer2);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x80); // encoding for 'accept'
	expectedBuf.Append(0xBA); // encoding for 'Trailer'
	expectedBuf.Append(KTxtTrailer2);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iTrailerMultiHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iTrailerMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterViaMultiHeaderUnitTest,"CWspHeaderWriter_ViaMultiHeader_UnitTest");

CWspHeaderWriter_ViaMultiHeader_UnitTest* CWspHeaderWriter_ViaMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ViaMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ViaMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ViaMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iViaMultiHeaderValidator;
	iViaMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ViaMultiHeader_UnitTest::~CWspHeaderWriter_ViaMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iViaMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ViaMultiHeader_UnitTest::CWspHeaderWriter_ViaMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterViaMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ViaMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EVia);
	
	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtVia, "go-via-me");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtVia);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00); // null terminator
	expectedBuf.Append(0xAB); // encoding for 'Via'
	expectedBuf.Append(KTxtVia);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iViaMultiHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iViaMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterAllowMultiHeaderUnitTest,"CWspHeaderWriter_AllowMultiHeader_UnitTest");

CWspHeaderWriter_AllowMultiHeader_UnitTest* CWspHeaderWriter_AllowMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_AllowMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_AllowMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_AllowMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iAllowMultiHeaderValidator;
	iAllowMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_AllowMultiHeader_UnitTest::~CWspHeaderWriter_AllowMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iAllowMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_AllowMultiHeader_UnitTest::CWspHeaderWriter_AllowMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterAllowMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_AllowMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EAllow);
	
	// Part1
	_LIT8(KTxtAllow, "Get");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAllow);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtAllow2, "Post");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtAllow2);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0xC0); // token for 'Get'
	expectedBuf.Append(0x86); // encoding for 'Allow'
	expectedBuf.Append(0xE0); // token for 'Post'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iAllowMultiHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iAllowMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterContentEncodingMultiHeaderUnitTest,"CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest");

CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest* CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iContentEncodingMultiHeaderValidator;
	iContentEncodingMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest::~CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iContentEncodingMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest::CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterContentEncodingMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentEncoding);
	
	// Part1
	_LIT8(KTxtContentEnc, "Gzip");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtContentEnc);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtContentEnc2, "Deflate");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtContentEnc2);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x80); // token for 'gzip'
	expectedBuf.Append(0x8B); // encoding for 'content-encoding'
	expectedBuf.Append(0x82); // token for 'deflate'
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iContentEncodingMultiHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iContentEncodingMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterUnknownHeaderUnitTest,"CWspHeaderWriter_UnknownHeader_UnitTest");

CWspHeaderWriter_UnknownHeader_UnitTest* CWspHeaderWriter_UnknownHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_UnknownHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_UnknownHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_UnknownHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUnknownHeaderValidator;
	iUnknownHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_UnknownHeader_UnitTest::~CWspHeaderWriter_UnknownHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUnknownHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_UnknownHeader_UnitTest::CWspHeaderWriter_UnknownHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterUnknownHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_UnknownHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(KErrNotFound);
	
	// Part1
	_LIT8(KTxtTestData, "This is test data");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTestData);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtTestData);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iUnknownHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iUnknownHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterUnknownNullHeaderUnitTest,"CWspHeaderWriter_UnknownNullHeader_UnitTest");

CWspHeaderWriter_UnknownNullHeader_UnitTest* CWspHeaderWriter_UnknownNullHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_UnknownNullHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_UnknownNullHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_UnknownNullHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iUnknownNullHeaderValidator;
	iUnknownNullHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_UnknownNullHeader_UnitTest::~CWspHeaderWriter_UnknownNullHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iUnknownNullHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_UnknownNullHeader_UnitTest::CWspHeaderWriter_UnknownNullHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterUnknownNullHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_UnknownNullHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(KErrNotFound);
	
	// Part1
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8());
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iUnknownNullHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iUnknownNullHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterMultiUnknownHeaderUnitTest,"CWspHeaderWriter_MultiUnknownHeader_UnitTest");

CWspHeaderWriter_MultiUnknownHeader_UnitTest* CWspHeaderWriter_MultiUnknownHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_MultiUnknownHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_MultiUnknownHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_MultiUnknownHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iMultiUnknownHeaderValidator;
	iMultiUnknownHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_MultiUnknownHeader_UnitTest::~CWspHeaderWriter_MultiUnknownHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iMultiUnknownHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_MultiUnknownHeader_UnitTest::CWspHeaderWriter_MultiUnknownHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterMultiUnknownHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_MultiUnknownHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(KErrNotFound);
	
	// Part1
	_LIT8(KTxtTestData1, "First test data");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTestData1());
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	// Part2
	_LIT8(KTxtTestData2, "Second test data");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtTestData2());
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	_LIT8(KTxtTestHdrSeperator, "TestHeader\0");
	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtTestData1());
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtTestHdrSeperator());
	expectedBuf.Append(KTxtTestData2());
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iMultiUnknownHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iMultiUnknownHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterProfileUnitTest,"CWspHeaderWriter_Profile_UnitTest");

CWspHeaderWriter_Profile_UnitTest* CWspHeaderWriter_Profile_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_Profile_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_Profile_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_Profile_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileValidator;
	iProfileValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_Profile_UnitTest::~CWspHeaderWriter_Profile_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_Profile_UnitTest::CWspHeaderWriter_Profile_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterProfileUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_Profile_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfile);

	_LIT8(KTxtProfileUri, "http://www.symbian.com");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtProfileUri);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part);
	iUTContext->iTestHeader->SetPartL(part, 0);
	CleanupStack::Pop(part);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtProfileUri);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iProfileValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterProfileNullTextUnitTest,"CWspHeaderWriter_ProfileNullText_UnitTest");

CWspHeaderWriter_ProfileNullText_UnitTest* CWspHeaderWriter_ProfileNullText_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ProfileNullText_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ProfileNullText_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ProfileNullText_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileNullTextValidator;
	iProfileNullTextValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ProfileNullText_UnitTest::~CWspHeaderWriter_ProfileNullText_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileNullTextValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ProfileNullText_UnitTest::CWspHeaderWriter_ProfileNullText_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterProfileNullTextUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ProfileNullText_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfile);

	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KNullDesC8);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part);
	iUTContext->iTestHeader->SetPartL(part, 0);
	CleanupStack::Pop(part);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileNullTextValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iProfileNullTextValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterProfileMalformedUnitTest,"CWspHeaderWriter_ProfileMalformed_UnitTest");

CWspHeaderWriter_ProfileMalformed_UnitTest* CWspHeaderWriter_ProfileMalformed_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ProfileMalformed_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ProfileMalformed_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ProfileMalformed_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileMalformedValidator;
	iProfileMalformedValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ProfileMalformed_UnitTest::~CWspHeaderWriter_ProfileMalformed_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileMalformedValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ProfileMalformed_UnitTest::CWspHeaderWriter_ProfileMalformed_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterProfileMalformedUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ProfileMalformed_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfile);

	_LIT8(KTxtProfileUri, "http://www.@symbian!.com-profiles");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtProfileUri);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part);
	iUTContext->iTestHeader->SetPartL(part, 0);
	CleanupStack::Pop(part);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtProfileUri);
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileMalformedValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iProfileMalformedValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterProfileDiffShortUnitTest,"CWspHeaderWriter_ProfileDiffShort_UnitTest");

CWspHeaderWriter_ProfileDiffShort_UnitTest* CWspHeaderWriter_ProfileDiffShort_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ProfileDiffShort_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ProfileDiffShort_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ProfileDiffShort_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileDiffShortValidator;
	iProfileDiffShortValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ProfileDiffShort_UnitTest::~CWspHeaderWriter_ProfileDiffShort_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileDiffShortValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ProfileDiffShort_UnitTest::CWspHeaderWriter_ProfileDiffShort_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterProfileDiffShortUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ProfileDiffShort_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfileDiff);
	
	_LIT8(KTxtShortCCPP, "short_profile_diff_info25");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtShortCCPP);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part);
	iUTContext->iTestHeader->SetPartL(part, 0);
	CleanupStack::Pop(part);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x19); // value-length
	expectedBuf.Append(KTxtShortCCPP);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileDiffShortValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iProfileDiffShortValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterProfileDiffLongUnitTest,"CWspHeaderWriter_ProfileDiffLong_UnitTest");

CWspHeaderWriter_ProfileDiffLong_UnitTest* CWspHeaderWriter_ProfileDiffLong_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ProfileDiffLong_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ProfileDiffLong_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ProfileDiffLong_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileDiffLongValidator;
	iProfileDiffLongValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ProfileDiffLong_UnitTest::~CWspHeaderWriter_ProfileDiffLong_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileDiffLongValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ProfileDiffLong_UnitTest::CWspHeaderWriter_ProfileDiffLong_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterProfileDiffLongUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ProfileDiffLong_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfileDiff);

	_LIT8(KTxtLongCCPP, "long_length_profile_diff_information38");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtLongCCPP);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part);
	iUTContext->iTestHeader->SetPartL(part, 0);
	CleanupStack::Pop(part);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x1F); // value-length <octet 31>
	expectedBuf.Append(0x26); // value-length
	expectedBuf.Append(KTxtLongCCPP);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileDiffLongValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iProfileDiffLongValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterProfileDiffMultiHeaderUnitTest,"CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest");

CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest* CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iProfileDiffMultiHeaderValidator;
	iProfileDiffMultiHeaderValidator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest::~CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iProfileDiffMultiHeaderValidator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest::CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterProfileDiffMultiHeaderUnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EProfileDiff);

	_LIT8(KTxtLongCCPP, "long_length_profile_diff_information38");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtLongCCPP);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	_LIT8(KTxtShortCCPP, "short_profile_diff_info25");
	RStringF stringVal2 = iUTContext->iStrPool.OpenFStringL(KTxtShortCCPP);
	CleanupClosePushL(stringVal2);
	CHeaderFieldPart* part2 = CHeaderFieldPart::NewL(stringVal2);
	CleanupStack::PushL(part2);
	iUTContext->iTestHeader->SetPartL(part2, 1);
	CleanupStack::Pop(part2);
	CleanupStack::PopAndDestroy(&stringVal2);

	TBuf8<256> expectedBuf;
	// Part 1
	expectedBuf.Append(0x1F); // value-length <octet 31>
	expectedBuf.Append(0x26); // value-length
	expectedBuf.Append(KTxtLongCCPP);
	//Part 2
	expectedBuf.Append(0xB6); // encoded token for 'Profile-Diff'
	expectedBuf.Append(0x19); // value-length
	expectedBuf.Append(KTxtShortCCPP);

	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iProfileDiffMultiHeaderValidator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iProfileDiffMultiHeaderValidator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}


// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterDefectFix_HOL595DR6_1UnitTest,"CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest");

CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest* CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDefectFix_HOL595DR6_1Validator;
	iDefectFix_HOL595DR6_1Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest::~CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDefectFix_HOL595DR6_1Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest::CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterDefectFix_HOL595DR6_1UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::ETrailer);
	(REINTERPRET_CAST(CWspHeaderCodec*, iUTContext->iCodec))->SetWspVersion(CWspHeaderCodec::EVersion1_2);
	
	// Part1
	_LIT8(KTxtAccept, "Accept");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtAccept);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtAccept());
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iDefectFix_HOL595DR6_1Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iDefectFix_HOL595DR6_1Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterDefectFix_HOL595DR6_2UnitTest,"CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest");

CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest* CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDefectFix_HOL595DR6_2Validator;
	iDefectFix_HOL595DR6_2Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest::~CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDefectFix_HOL595DR6_2Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest::CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterDefectFix_HOL595DR6_2UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/vnd.wap.mms-message");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0xBE); // encoded token for type above with top bit set
	iUTContext->SetExpectedL(expectedBuf);
	
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iDefectFix_HOL595DR6_2Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iDefectFix_HOL595DR6_2Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterDefectFix_HOL595DR6_3UnitTest,"CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest");

CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest* CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDefectFix_HOL595DR6_3Validator;
	iDefectFix_HOL595DR6_3Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest::~CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDefectFix_HOL595DR6_3Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest::CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterDefectFix_HOL595DR6_3UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	(REINTERPRET_CAST(CWspHeaderCodec*, iUTContext->iCodec))->SetWspVersion(CWspHeaderCodec::EVersion1_2);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/vnd.wap.mms-message");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);
	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(KTxtTypeToken()); // not encoded, should be text as encoding not available in 1.2
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);
	
	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iDefectFix_HOL595DR6_3Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iDefectFix_HOL595DR6_3Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterDefectFix_HOL595DR6_4UnitTest,"CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest");

CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest* CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDefectFix_HOL595DR6_4Validator;
	iDefectFix_HOL595DR6_4Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest::~CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDefectFix_HOL595DR6_4Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest::CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterDefectFix_HOL595DR6_4UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtParam1Name, "Name");
	_LIT8(KTxtParam1Value, "TestName");
	RStringF param1Name = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(param1Name);
	RStringF param1Value = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(param1Value);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(param1Name, param1Value);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &param1Name);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x0B); // length value
	expectedBuf.Append(0x90); // token for application
	expectedBuf.Append(0x97); // token for name param
	expectedBuf.Append(KTxtParam1Value); // param value
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iDefectFix_HOL595DR6_4Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iDefectFix_HOL595DR6_4Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterDefectFix_HOL595DR6_5UnitTest,"CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest");

CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest* CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDefectFix_HOL595DR6_5Validator;
	iDefectFix_HOL595DR6_5Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest::~CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDefectFix_HOL595DR6_5Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest::CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterDefectFix_HOL595DR6_5UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	(REINTERPRET_CAST(CWspHeaderCodec*, iUTContext->iCodec))->SetWspVersion(CWspHeaderCodec::EVersion1_2);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtParam1Name, "Name");
	_LIT8(KTxtParam1Value, "TestName");
	RStringF param1Name = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(param1Name);
	RStringF param1Value = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(param1Value);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(param1Name, param1Value);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &param1Name);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x0B); // length value
	expectedBuf.Append(0x90); // token for application
	expectedBuf.Append(0x85); // token for name param in v1.2
	expectedBuf.Append(KTxtParam1Value); // param value
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iDefectFix_HOL595DR6_5Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iDefectFix_HOL595DR6_5Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterDefectFix_HOL595DR6_6UnitTest,"CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest");

CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest* CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDefectFix_HOL595DR6_6Validator;
	iDefectFix_HOL595DR6_6Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest::~CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDefectFix_HOL595DR6_6Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest::CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterDefectFix_HOL595DR6_6UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtParam1Name, "Comment");
	_LIT8(KTxtParam1Value, "TestName");
	RStringF param1Name = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(param1Name);
	RStringF param1Value = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(param1Value);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(param1Name, param1Value);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &param1Name);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x0B); // length value
	expectedBuf.Append(0x90); // token for application
	expectedBuf.Append(0x9B); // token for comment param in v1.4
	expectedBuf.Append(KTxtParam1Value); // param value
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iDefectFix_HOL595DR6_6Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iDefectFix_HOL595DR6_6Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterDefectFix_HOL595DR6_7UnitTest,"CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest");

CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest* CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest::NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver)
	{
	CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest* self = 
					new(ELeave) CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest(aDataLogger,
																aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self; 
	}

inline TInt CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest::RunError(TInt aError)
	{
	// The RunL left so chain to the base first and then cleanup
	TInt error = CUnitTest::RunError(aError);	// Chain to base
	delete iUTContext;
	iUTContext = NULL;
	delete iStateAccessor;
	iStateAccessor = NULL;
	/* delete any validators used */
	delete iCtorValidator;
	iCtorValidator = NULL;
	delete iDefectFix_HOL595DR6_7Validator;
	iDefectFix_HOL595DR6_7Validator = NULL;
	delete iDtorValidator;
	iDtorValidator = NULL;
	return error;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest::~CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest()
	{
	// Simply delete our test class instance
	delete iUTContext;
	delete iStateAccessor;
	/* delete any validators used */
	delete iCtorValidator;
	delete iDefectFix_HOL595DR6_7Validator;
	delete iDtorValidator;
	}

inline CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest::CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest(CDataLogger& aDataLogger,
																	MUnitTestObserver& aObserver)
: CUnitTest(KWspHeaderWriterDefectFix_HOL595DR6_7UnitTest, aDataLogger, aObserver)
	{
	//Do nothing
	}

// Now the Individual transitions need to be added.
inline void CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest::ConstructL()
	{
	// Perform the base class initialization
	UnitTestConstructL();

	// Create the Unit test state accessor
	iStateAccessor = new(ELeave) TWspHeaderWriter_StateAccessor;
	// Construct the Unit test context.
	iUTContext = new(ELeave) CWspHeaderWriter_UnitTestContext(iDataLogger, *iStateAccessor, *this, WSP::Table);
	iUTContext->ConstructL(WSP::EContentType);
	(REINTERPRET_CAST(CWspHeaderCodec*, iUTContext->iCodec))->SetWspVersion(CWspHeaderCodec::EVersion1_2);
	
	// Part1
	_LIT8(KTxtTypeToken, "application/*");
	RStringF stringVal = iUTContext->iStrPool.OpenFStringL(KTxtTypeToken);
	CleanupClosePushL(stringVal);
	CHeaderFieldPart* part1 = CHeaderFieldPart::NewL(stringVal);
	CleanupStack::PushL(part1);

	// Param1
	_LIT8(KTxtParam1Name, "Comment");
	_LIT8(KTxtParam1Value, "TestName");
	RStringF param1Name = iUTContext->iStrPool.OpenFStringL(KTxtParam1Name);
	CleanupClosePushL(param1Name);
	RStringF param1Value = iUTContext->iStrPool.OpenFStringL(KTxtParam1Value);
	CleanupClosePushL(param1Value);
	CHeaderFieldParam* param1 = CHeaderFieldParam::NewL(param1Name, param1Value);
	CleanupStack::PushL(param1);
	part1->AddParamL(param1);
	CleanupStack::Pop(param1);

	iUTContext->iTestHeader->SetPartL(part1, 0);
	CleanupStack::PopAndDestroy(2, &param1Name);
	CleanupStack::Pop(part1);
	CleanupStack::PopAndDestroy(&stringVal);

	TBuf8<256> expectedBuf;
	expectedBuf.Append(0x12); // length value
	expectedBuf.Append(0x90); // token for application
	expectedBuf.Append(KTxtParam1Name); // param name
	expectedBuf.Append(0x00);
	expectedBuf.Append(KTxtParam1Value); // param value
	expectedBuf.Append(0x00);
	iUTContext->SetExpectedL(expectedBuf);

	// Add the Transitions in the order they are to run
	// C'tor first, D'tor last...
	/*	Examples of C'tor and D'tor transitions on CWspHeaderWriter class.
		using constructor and destuctor validators */
	iCtorValidator = new(ELeave) TWspHeaderWriter_Ctor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_NewL_Transition(*iUTContext,*iCtorValidator));
	iDefectFix_HOL595DR6_7Validator = new(ELeave) TWspHeaderWriter_EncodeHeaderL_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_EncodeHeaderL_Transition(*iUTContext,*iDefectFix_HOL595DR6_7Validator));
	iDtorValidator = new(ELeave) TWspHeaderWriter_Dtor_TransitionValidator(*iUTContext);
	AddTransitionL(new(ELeave)CWspHeaderWriter_Dtor_Transition(*iUTContext,*iDtorValidator));
	}
