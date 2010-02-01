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
// The unit test class definitions for the CWspHeaderReader class.
// 
//

/**
 @file WspHeaderReaderUnitTest.h
*/
#ifndef __WSPHEADERREADERUNITTEST_H__
#define __WSPHEADERREADERUNITTEST_H__

#include "TestBedDefinitions.h"
#include "WspHeaderReaderStateAccessor.h"
#include "WspHeaderReaderTransitionValidation.h"
#include "WspHeaderReaderTransitions.h"

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_CreateAndDestroy_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for CreateAndDestroy on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_CreateAndDestroy_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_CreateAndDestroy_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_CreateAndDestroy_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_CreateAndDestroy_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_CreateAndDestroy_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_CreateAndDestroy_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_CreateAndDestroy_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_CreateAndDestroy_UnitTest is fully constructed.
		@post			CWspHeaderReader_CreateAndDestroy_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_CreateAndDestroy_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_CreateAndDestroy_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_CreateAndDestroy_UnitTest is fully constructed.
		@post			CWspHeaderReader_CreateAndDestroy_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_CreateAndDestroy_UnitTest


// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentTypeShortToken_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentTypeShortToken on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentTypeShortToken_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentTypeShortToken_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeShortToken_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentTypeShortToken_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentTypeShortToken_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentTypeShortToken_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentTypeShortToken_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentTypeShortToken_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeShortToken_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentTypeShortToken_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentTypeShortToken_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeShortToken_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentTypeShortToken_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentTypeShortToken_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeShortToken_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentTypeShortTokenValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentTypeShortToken_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentTypeExtensionMedia_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentTypeExtensionMedia on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentTypeExtensionMedia_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentTypeExtensionMedia_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeExtensionMedia_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentTypeExtensionMedia_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentTypeExtensionMedia_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentTypeExtensionMedia_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentTypeExtensionMedia_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentTypeExtensionMedia_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeExtensionMedia_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentTypeExtensionMedia_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentTypeExtensionMedia_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeExtensionMedia_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentTypeExtensionMedia_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentTypeExtensionMedia_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeExtensionMedia_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentTypeExtensionMediaValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentTypeExtensionMedia_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentTypeNull_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentTypeNull on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentTypeNull_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentTypeNull_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeNull_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentTypeNull_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentTypeNull_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentTypeNull_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentTypeNull_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentTypeNull_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeNull_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentTypeNull_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentTypeNull_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeNull_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentTypeNull_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentTypeNull_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeNull_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentTypeNullValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentTypeNull_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentTypeWellKnownMediaInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentTypeWellKnownMediaIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentTypeWellKnownMediaInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentTypeMediaTypeExtension on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentTypeMediaTypeExtensionValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentTypeMediaTypeExtension_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentTypeWith1Param_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentTypeWith1Param on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentTypeWith1Param_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentTypeWith1Param_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeWith1Param_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentTypeWith1Param_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentTypeWith1Param_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentTypeWith1Param_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentTypeWith1Param_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentTypeWith1Param_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeWith1Param_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentTypeWith1Param_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentTypeWith1Param_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeWith1Param_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentTypeWith1Param_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentTypeWith1Param_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeWith1Param_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentTypeWith1ParamValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentTypeWith1Param_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentTypeWith2Params_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentTypeWith2Params on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentTypeWith2Params_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentTypeWith2Params_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeWith2Params_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentTypeWith2Params_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentTypeWith2Params_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentTypeWith2Params_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentTypeWith2Params_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentTypeWith2Params_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeWith2Params_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentTypeWith2Params_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentTypeWith2Params_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentTypeWith2Params_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentTypeWith2Params_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentTypeWith2Params_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentTypeWith2Params_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentTypeWith2ParamsValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentTypeWith2Params_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_Date_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for Date on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_Date_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_Date_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_Date_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_Date_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_Date_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_Date_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_Date_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_Date_UnitTest is fully constructed.
		@post			CWspHeaderReader_Date_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_Date_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_Date_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_Date_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_Date_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_Date_UnitTest is fully constructed.
		@post			CWspHeaderReader_Date_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iDateValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_Date_UnitTest


// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_LastModified_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for LastModified on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_LastModified_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_LastModified_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_LastModified_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_LastModified_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_LastModified_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_LastModified_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_LastModified_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_LastModified_UnitTest is fully constructed.
		@post			CWspHeaderReader_LastModified_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_LastModified_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_LastModified_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_LastModified_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_LastModified_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_LastModified_UnitTest is fully constructed.
		@post			CWspHeaderReader_LastModified_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iLastModifiedValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_LastModified_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_PragmaNoCache_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for PragmaNoCache on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_PragmaNoCache_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_PragmaNoCache_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_PragmaNoCache_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_PragmaNoCache_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_PragmaNoCache_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_PragmaNoCache_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_PragmaNoCache_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_PragmaNoCache_UnitTest is fully constructed.
		@post			CWspHeaderReader_PragmaNoCache_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_PragmaNoCache_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_PragmaNoCache_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_PragmaNoCache_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_PragmaNoCache_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_PragmaNoCache_UnitTest is fully constructed.
		@post			CWspHeaderReader_PragmaNoCache_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iPragmaNoCacheValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_PragmaNoCache_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_PragmaTokenParam_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for PragmaTokenParam on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_PragmaTokenParam_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_PragmaTokenParam_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_PragmaTokenParam_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_PragmaTokenParam_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_PragmaTokenParam_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_PragmaTokenParam_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_PragmaTokenParam_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_PragmaTokenParam_UnitTest is fully constructed.
		@post			CWspHeaderReader_PragmaTokenParam_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_PragmaTokenParam_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_PragmaTokenParam_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_PragmaTokenParam_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_PragmaTokenParam_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_PragmaTokenParam_UnitTest is fully constructed.
		@post			CWspHeaderReader_PragmaTokenParam_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iPragmaTokenParamValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_PragmaTokenParam_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_PragmaTokenParamNoValue_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for PragmaTokenParamNoValue on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_PragmaTokenParamNoValue_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_PragmaTokenParamNoValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_PragmaTokenParamNoValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_PragmaTokenParamNoValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_PragmaTokenParamNoValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_PragmaTokenParamNoValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_PragmaTokenParamNoValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_PragmaTokenParamNoValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_PragmaTokenParamNoValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_PragmaTokenParamNoValue_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_PragmaTokenParamNoValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_PragmaTokenParamNoValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_PragmaTokenParamNoValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_PragmaTokenParamNoValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_PragmaTokenParamNoValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iPragmaTokenParamNoValueValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_PragmaTokenParamNoValue_UnitTest


// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_VaryShortIntToken_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for VaryShortIntToken on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_VaryShortIntToken_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_VaryShortIntToken_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_VaryShortIntToken_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_VaryShortIntToken_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_VaryShortIntToken_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_VaryShortIntToken_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_VaryShortIntToken_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_VaryShortIntToken_UnitTest is fully constructed.
		@post			CWspHeaderReader_VaryShortIntToken_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_VaryShortIntToken_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_VaryShortIntToken_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_VaryShortIntToken_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_VaryShortIntToken_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_VaryShortIntToken_UnitTest is fully constructed.
		@post			CWspHeaderReader_VaryShortIntToken_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iVaryShortIntTokenValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_VaryShortIntToken_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_VaryTokenText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for VaryTokenText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_VaryTokenText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_VaryTokenText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_VaryTokenText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_VaryTokenText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_VaryTokenText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_VaryTokenText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_VaryTokenText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_VaryTokenText_UnitTest is fully constructed.
		@post			CWspHeaderReader_VaryTokenText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_VaryTokenText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_VaryTokenText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_VaryTokenText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_VaryTokenText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_VaryTokenText_UnitTest is fully constructed.
		@post			CWspHeaderReader_VaryTokenText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iVaryTokenTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_VaryTokenText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WWWAuthenticateBasic_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WWWAuthenticateBasic on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WWWAuthenticateBasic_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WWWAuthenticateBasic_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateBasic_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WWWAuthenticateBasic_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WWWAuthenticateBasic_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WWWAuthenticateBasic_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WWWAuthenticateBasic_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WWWAuthenticateBasic_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateBasic_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WWWAuthenticateBasic_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WWWAuthenticateBasic_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateBasic_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WWWAuthenticateBasic_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WWWAuthenticateBasic_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateBasic_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWWWAuthenticateBasicValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WWWAuthenticateBasic_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WWWAuthenticateBasicWithoutRealm on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWWWAuthenticateBasicWithoutRealmValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WWWAuthenticateBasicWithoutRealm_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WWWAuthenticateAuthScheme on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWWWAuthenticateAuthSchemeValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WWWAuthenticateAuthScheme_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WWWAuthenticateAuthSchemeNoRealm on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWWWAuthenticateAuthSchemeNoRealmValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WWWAuthenticateAuthScheme2Params on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWWWAuthenticateAuthScheme2ParamsValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WWWAuthenticateAuthScheme2Params_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WWWAuthenticateAuthSchemeNoRealm2Params on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest is fully constructed.
		@post			CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWWWAuthenticateAuthSchemeNoRealm2ParamsValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WWWAuthenticateAuthSchemeNoRealm2Params_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_SetCookieShortIntVersion_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for SetCookieShortIntVersion on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_SetCookieShortIntVersion_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_SetCookieShortIntVersion_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_SetCookieShortIntVersion_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_SetCookieShortIntVersion_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_SetCookieShortIntVersion_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_SetCookieShortIntVersion_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_SetCookieShortIntVersion_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_SetCookieShortIntVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_SetCookieShortIntVersion_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_SetCookieShortIntVersion_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_SetCookieShortIntVersion_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_SetCookieShortIntVersion_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_SetCookieShortIntVersion_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_SetCookieShortIntVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_SetCookieShortIntVersion_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iSetCookieShortIntVersionValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_SetCookieShortIntVersion_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for SetCookieMajorVersionNullName1Param on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest is fully constructed.
		@post			CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest is fully constructed.
		@post			CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iSetCookieMajorVersionNullName1ParamValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_SetCookieMajorVersionNullName1Param_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for SetCookieTextVersionNullValue on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iSetCookieTextVersionNullValueValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_SetCookieTextVersionNullValue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_SetCookieNullVersion_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for SetCookieNullVersion on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_SetCookieNullVersion_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_SetCookieNullVersion_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_SetCookieNullVersion_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_SetCookieNullVersion_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_SetCookieNullVersion_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_SetCookieNullVersion_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_SetCookieNullVersion_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_SetCookieNullVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_SetCookieNullVersion_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_SetCookieNullVersion_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_SetCookieNullVersion_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_SetCookieNullVersion_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_SetCookieNullVersion_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_SetCookieNullVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_SetCookieNullVersion_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iSetCookieNullVersionValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_SetCookieNullVersion_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentLocation_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentLocation on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentLocation_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentLocation_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentLocation_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentLocation_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentLocation_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentLocation_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentLocation_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentLocation_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLocation_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentLocation_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentLocation_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentLocation_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentLocation_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentLocation_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLocation_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentLocationValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentLocation_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_LocationWithoutQuote_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for LocationWithoutQuote on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_LocationWithoutQuote_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_LocationWithoutQuote_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_LocationWithoutQuote_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_LocationWithoutQuote_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_LocationWithoutQuote_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_LocationWithoutQuote_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_LocationWithoutQuote_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_LocationWithoutQuote_UnitTest is fully constructed.
		@post			CWspHeaderReader_LocationWithoutQuote_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_LocationWithoutQuote_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_LocationWithoutQuote_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_LocationWithoutQuote_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_LocationWithoutQuote_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_LocationWithoutQuote_UnitTest is fully constructed.
		@post			CWspHeaderReader_LocationWithoutQuote_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iLocationWithoutQuoteValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_LocationWithoutQuote_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_EncodingVersionShortInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for EncodingVersionShortInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_EncodingVersionShortInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_EncodingVersionShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_EncodingVersionShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_EncodingVersionShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_EncodingVersionShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_EncodingVersionShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_EncodingVersionShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_EncodingVersionShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_EncodingVersionShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_EncodingVersionShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_EncodingVersionShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iEncodingVersionShortIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_EncodingVersionShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_EncodingVersionNullVersion_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for EncodingVersionNullVersion on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_EncodingVersionNullVersion_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_EncodingVersionNullVersion_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionNullVersion_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_EncodingVersionNullVersion_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_EncodingVersionNullVersion_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_EncodingVersionNullVersion_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_EncodingVersionNullVersion_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_EncodingVersionNullVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionNullVersion_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_EncodingVersionNullVersion_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_EncodingVersionNullVersion_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionNullVersion_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_EncodingVersionNullVersion_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_EncodingVersionNullVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionNullVersion_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iEncodingVersionNullVersionValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_EncodingVersionNullVersion_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_EncodingVersionTextVersion_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for EncodingVersionTextVersion on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_EncodingVersionTextVersion_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_EncodingVersionTextVersion_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionTextVersion_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_EncodingVersionTextVersion_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_EncodingVersionTextVersion_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_EncodingVersionTextVersion_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_EncodingVersionTextVersion_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_EncodingVersionTextVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionTextVersion_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_EncodingVersionTextVersion_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_EncodingVersionTextVersion_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionTextVersion_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_EncodingVersionTextVersion_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_EncodingVersionTextVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionTextVersion_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iEncodingVersionTextVersionValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_EncodingVersionTextVersion_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_EncodingVersionCodePage_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for EncodingVersionCodePage on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_EncodingVersionCodePage_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_EncodingVersionCodePage_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionCodePage_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_EncodingVersionCodePage_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_EncodingVersionCodePage_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_EncodingVersionCodePage_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_EncodingVersionCodePage_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_EncodingVersionCodePage_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionCodePage_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_EncodingVersionCodePage_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_EncodingVersionCodePage_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionCodePage_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_EncodingVersionCodePage_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_EncodingVersionCodePage_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionCodePage_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iEncodingVersionCodePageValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_EncodingVersionCodePage_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for EncodingVersionCodePageIntVersion on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iEncodingVersionCodePageIntVersionValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_EncodingVersionCodePageIntVersion_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for EncodingVersionCodePageTextVersion on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iEncodingVersionCodePageTextVersionValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_EncodingVersionCodePageTextVersion_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for EncodingVersionCodePageNullVersion on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest is fully constructed.
		@post			CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iEncodingVersionCodePageNullVersionValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_EncodingVersionCodePageNullVersion_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_AcceptRangesNone_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for AcceptRangesNone on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_AcceptRangesNone_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_AcceptRangesNone_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_AcceptRangesNone_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_AcceptRangesNone_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_AcceptRangesNone_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_AcceptRangesNone_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_AcceptRangesNone_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_AcceptRangesNone_UnitTest is fully constructed.
		@post			CWspHeaderReader_AcceptRangesNone_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_AcceptRangesNone_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_AcceptRangesNone_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_AcceptRangesNone_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_AcceptRangesNone_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_AcceptRangesNone_UnitTest is fully constructed.
		@post			CWspHeaderReader_AcceptRangesNone_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iAcceptRangesNoneValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_AcceptRangesNone_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_AcceptRangesBytes_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for AcceptRangesBytes on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_AcceptRangesBytes_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_AcceptRangesBytes_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_AcceptRangesBytes_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_AcceptRangesBytes_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_AcceptRangesBytes_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_AcceptRangesBytes_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_AcceptRangesBytes_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_AcceptRangesBytes_UnitTest is fully constructed.
		@post			CWspHeaderReader_AcceptRangesBytes_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_AcceptRangesBytes_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_AcceptRangesBytes_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_AcceptRangesBytes_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_AcceptRangesBytes_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_AcceptRangesBytes_UnitTest is fully constructed.
		@post			CWspHeaderReader_AcceptRangesBytes_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iAcceptRangesBytesValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_AcceptRangesBytes_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_AcceptRangesTokenText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for AcceptRangesTokenText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_AcceptRangesTokenText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_AcceptRangesTokenText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_AcceptRangesTokenText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_AcceptRangesTokenText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_AcceptRangesTokenText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_AcceptRangesTokenText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_AcceptRangesTokenText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_AcceptRangesTokenText_UnitTest is fully constructed.
		@post			CWspHeaderReader_AcceptRangesTokenText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_AcceptRangesTokenText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_AcceptRangesTokenText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_AcceptRangesTokenText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_AcceptRangesTokenText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_AcceptRangesTokenText_UnitTest is fully constructed.
		@post			CWspHeaderReader_AcceptRangesTokenText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iAcceptRangesTokenTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_AcceptRangesTokenText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentEncodingGZip_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentEncodingGZip on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentEncodingGZip_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentEncodingGZip_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentEncodingGZip_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentEncodingGZip_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentEncodingGZip_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentEncodingGZip_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentEncodingGZip_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentEncodingGZip_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentEncodingGZip_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentEncodingGZip_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentEncodingGZip_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentEncodingGZip_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentEncodingGZip_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentEncodingGZip_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentEncodingGZip_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentEncodingGZipValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentEncodingGZip_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentEncodingCompress_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentEncodingCompress on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentEncodingCompress_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentEncodingCompress_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentEncodingCompress_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentEncodingCompress_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentEncodingCompress_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentEncodingCompress_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentEncodingCompress_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentEncodingCompress_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentEncodingCompress_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentEncodingCompress_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentEncodingCompress_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentEncodingCompress_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentEncodingCompress_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentEncodingCompress_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentEncodingCompress_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentEncodingCompressValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentEncodingCompress_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentEncodingDeflate_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentEncodingDeflate on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentEncodingDeflate_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentEncodingDeflate_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentEncodingDeflate_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentEncodingDeflate_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentEncodingDeflate_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentEncodingDeflate_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentEncodingDeflate_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentEncodingDeflate_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentEncodingDeflate_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentEncodingDeflate_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentEncodingDeflate_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentEncodingDeflate_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentEncodingDeflate_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentEncodingDeflate_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentEncodingDeflate_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentEncodingDeflateValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentEncodingDeflate_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentEncodingTokenText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentEncodingTokenText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentEncodingTokenText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentEncodingTokenText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentEncodingTokenText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentEncodingTokenText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentEncodingTokenText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentEncodingTokenText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentEncodingTokenText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentEncodingTokenText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentEncodingTokenText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentEncodingTokenText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentEncodingTokenText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentEncodingTokenText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentEncodingTokenText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentEncodingTokenText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentEncodingTokenText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentEncodingTokenTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentEncodingTokenText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentLanguageAnyLang_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentLanguageAnyLang on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentLanguageAnyLang_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentLanguageAnyLang_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageAnyLang_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentLanguageAnyLang_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentLanguageAnyLang_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentLanguageAnyLang_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentLanguageAnyLang_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentLanguageAnyLang_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageAnyLang_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentLanguageAnyLang_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentLanguageAnyLang_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageAnyLang_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentLanguageAnyLang_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentLanguageAnyLang_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageAnyLang_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentLanguageAnyLangValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentLanguageAnyLang_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentLanguageShortInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentLanguageShortInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentLanguageShortInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentLanguageShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentLanguageShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentLanguageShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentLanguageShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentLanguageShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentLanguageShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentLanguageShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentLanguageShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentLanguageShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentLanguageShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentLanguageShortIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentLanguageShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentLanguageLongInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentLanguageLongInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentLanguageLongInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentLanguageLongInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageLongInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentLanguageLongInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentLanguageLongInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentLanguageLongInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentLanguageLongInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentLanguageLongInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageLongInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentLanguageLongInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentLanguageLongInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageLongInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentLanguageLongInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentLanguageLongInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageLongInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentLanguageLongIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentLanguageLongInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentLanguageTokenText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentLanguageTokenText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentLanguageTokenText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentLanguageTokenText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageTokenText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentLanguageTokenText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentLanguageTokenText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentLanguageTokenText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentLanguageTokenText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentLanguageTokenText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageTokenText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentLanguageTokenText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentLanguageTokenText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageTokenText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentLanguageTokenText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentLanguageTokenText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageTokenText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentLanguageTokenTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentLanguageTokenText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentLanguageNotExist_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentLanguageNotExist on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentLanguageNotExist_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentLanguageNotExist_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageNotExist_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentLanguageNotExist_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentLanguageNotExist_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentLanguageNotExist_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentLanguageNotExist_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentLanguageNotExist_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageNotExist_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentLanguageNotExist_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentLanguageNotExist_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageNotExist_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentLanguageNotExist_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentLanguageNotExist_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageNotExist_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentLanguageNotExistValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentLanguageNotExist_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentMD5_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentMD5 on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentMD5_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentMD5_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentMD5_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentMD5_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentMD5_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentMD5_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentMD5_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentMD5_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentMD5_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentMD5_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentMD5_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentMD5_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentMD5_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentMD5_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentMD5_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentMD5Validator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentMD5_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentMD5TooShort_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentMD5TooShort on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentMD5TooShort_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentMD5TooShort_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentMD5TooShort_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentMD5TooShort_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentMD5TooShort_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentMD5TooShort_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentMD5TooShort_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentMD5TooShort_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentMD5TooShort_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentMD5TooShort_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentMD5TooShort_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentMD5TooShort_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentMD5TooShort_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentMD5TooShort_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentMD5TooShort_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentMD5TooShortValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentMD5TooShort_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ProxyAuthenticateBasic_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ProxyAuthenticateBasic on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ProxyAuthenticateBasic_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ProxyAuthenticateBasic_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ProxyAuthenticateBasic_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ProxyAuthenticateBasic_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ProxyAuthenticateBasic_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ProxyAuthenticateBasic_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ProxyAuthenticateBasic_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ProxyAuthenticateBasic_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProxyAuthenticateBasic_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ProxyAuthenticateBasic_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ProxyAuthenticateBasic_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ProxyAuthenticateBasic_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ProxyAuthenticateBasic_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ProxyAuthenticateBasic_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProxyAuthenticateBasic_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iProxyAuthenticateBasicValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ProxyAuthenticateBasic_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ServerText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ServerText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ServerText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ServerText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ServerText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ServerText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ServerText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ServerText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ServerText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ServerText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ServerText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ServerText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ServerText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ServerText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ServerText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ServerText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ServerText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iServerTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ServerText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ServerNullText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ServerNullText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ServerNullText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ServerNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ServerNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ServerNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ServerNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ServerNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ServerNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ServerNullText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ServerNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ServerNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ServerNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ServerNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ServerNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ServerNullText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ServerNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iServerNullTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ServerNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for RetryAfterAbsoluteDate on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest is fully constructed.
		@post			CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest is fully constructed.
		@post			CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iRetryAfterAbsoluteDateValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_RetryAfterAbsoluteDate_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_RetryAfterRelativeTime_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for RetryAfterRelativeTime on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_RetryAfterRelativeTime_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_RetryAfterRelativeTime_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_RetryAfterRelativeTime_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_RetryAfterRelativeTime_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_RetryAfterRelativeTime_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_RetryAfterRelativeTime_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_RetryAfterRelativeTime_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_RetryAfterRelativeTime_UnitTest is fully constructed.
		@post			CWspHeaderReader_RetryAfterRelativeTime_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_RetryAfterRelativeTime_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_RetryAfterRelativeTime_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_RetryAfterRelativeTime_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_RetryAfterRelativeTime_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_RetryAfterRelativeTime_UnitTest is fully constructed.
		@post			CWspHeaderReader_RetryAfterRelativeTime_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iRetryAfterRelativeTimeValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_RetryAfterRelativeTime_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ViaText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ViaText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ViaText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ViaText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ViaText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ViaText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ViaText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ViaText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ViaText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ViaText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ViaText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ViaText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ViaText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ViaText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ViaText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ViaText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ViaText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iViaTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ViaText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ViaNullText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ViaNullText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ViaNullText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ViaNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ViaNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ViaNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ViaNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ViaNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ViaNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ViaNullText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ViaNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ViaNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ViaNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ViaNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ViaNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ViaNullText_UnitTest is fully constructed.
		@post			CWspHeaderReader_ViaNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iViaNullTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ViaNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ViaTextWithoutTerminator_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ViaTextWithoutTerminator on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ViaTextWithoutTerminator_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ViaTextWithoutTerminator_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ViaTextWithoutTerminator_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ViaTextWithoutTerminator_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ViaTextWithoutTerminator_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ViaTextWithoutTerminator_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ViaTextWithoutTerminator_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ViaTextWithoutTerminator_UnitTest is fully constructed.
		@post			CWspHeaderReader_ViaTextWithoutTerminator_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ViaTextWithoutTerminator_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ViaTextWithoutTerminator_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ViaTextWithoutTerminator_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ViaTextWithoutTerminator_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ViaTextWithoutTerminator_UnitTest is fully constructed.
		@post			CWspHeaderReader_ViaTextWithoutTerminator_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iViaTextWithoutTerminatorValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ViaTextWithoutTerminator_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_TrailerShortInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for TrailerShortInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_TrailerShortInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_TrailerShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_TrailerShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_TrailerShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_TrailerShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_TrailerShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_TrailerShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_TrailerShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_TrailerShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_TrailerShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_TrailerShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_TrailerShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_TrailerShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_TrailerShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_TrailerShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iTrailerShortIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_TrailerShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_TrailerNotExistToken_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for TrailerNotExistToken on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_TrailerNotExistToken_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_TrailerNotExistToken_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_TrailerNotExistToken_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_TrailerNotExistToken_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_TrailerNotExistToken_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_TrailerNotExistToken_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_TrailerNotExistToken_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_TrailerNotExistToken_UnitTest is fully constructed.
		@post			CWspHeaderReader_TrailerNotExistToken_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_TrailerNotExistToken_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_TrailerNotExistToken_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_TrailerNotExistToken_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_TrailerNotExistToken_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_TrailerNotExistToken_UnitTest is fully constructed.
		@post			CWspHeaderReader_TrailerNotExistToken_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iTrailerNotExistTokenValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_TrailerNotExistToken_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_TrailerTokenText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for TrailerTokenText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_TrailerTokenText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_TrailerTokenText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_TrailerTokenText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_TrailerTokenText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_TrailerTokenText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_TrailerTokenText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_TrailerTokenText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_TrailerTokenText_UnitTest is fully constructed.
		@post			CWspHeaderReader_TrailerTokenText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_TrailerTokenText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_TrailerTokenText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_TrailerTokenText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_TrailerTokenText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_TrailerTokenText_UnitTest is fully constructed.
		@post			CWspHeaderReader_TrailerTokenText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iTrailerTokenTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_TrailerTokenText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentRangeUnknownLength_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentRangeUnknownLength on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentRangeUnknownLength_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentRangeUnknownLength_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentRangeUnknownLength_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentRangeUnknownLength_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentRangeUnknownLength_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentRangeUnknownLength_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentRangeUnknownLength_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentRangeUnknownLength_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentRangeUnknownLength_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentRangeUnknownLength_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentRangeUnknownLength_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentRangeUnknownLength_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentRangeUnknownLength_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentRangeUnknownLength_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentRangeUnknownLength_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentRangeUnknownLengthValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentRangeUnknownLength_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentRangeKnownLength_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentRangeKnownLength on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentRangeKnownLength_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentRangeKnownLength_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentRangeKnownLength_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentRangeKnownLength_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentRangeKnownLength_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentRangeKnownLength_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentRangeKnownLength_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentRangeKnownLength_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentRangeKnownLength_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentRangeKnownLength_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentRangeKnownLength_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentRangeKnownLength_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentRangeKnownLength_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentRangeKnownLength_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentRangeKnownLength_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentRangeKnownLengthValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentRangeKnownLength_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentRangeV1_2_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentRangeV1_2 on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentRangeV1_2_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentRangeV1_2_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentRangeV1_2_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentRangeV1_2_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentRangeV1_2_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentRangeV1_2_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentRangeV1_2_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentRangeV1_2_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentRangeV1_2_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentRangeV1_2_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentRangeV1_2_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentRangeV1_2_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentRangeV1_2_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentRangeV1_2_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentRangeV1_2_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentRangeV1_2Validator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentRangeV1_2_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_XWapAppIdUriValue_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for XWapAppIdUriValue on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_XWapAppIdUriValue_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_XWapAppIdUriValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_XWapAppIdUriValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_XWapAppIdUriValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_XWapAppIdUriValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_XWapAppIdUriValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_XWapAppIdUriValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_XWapAppIdUriValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapAppIdUriValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_XWapAppIdUriValue_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_XWapAppIdUriValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_XWapAppIdUriValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_XWapAppIdUriValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_XWapAppIdUriValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapAppIdUriValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iXWapAppIdUriValueValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_XWapAppIdUriValue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_XWapAppIdNullUri_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for XWapAppIdNullUri on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_XWapAppIdNullUri_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_XWapAppIdNullUri_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_XWapAppIdNullUri_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_XWapAppIdNullUri_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_XWapAppIdNullUri_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_XWapAppIdNullUri_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_XWapAppIdNullUri_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_XWapAppIdNullUri_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapAppIdNullUri_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_XWapAppIdNullUri_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_XWapAppIdNullUri_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_XWapAppIdNullUri_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_XWapAppIdNullUri_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_XWapAppIdNullUri_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapAppIdNullUri_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iXWapAppIdNullUriValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_XWapAppIdNullUri_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_XWapAppIdShortInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for XWapAppIdShortInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_XWapAppIdShortInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_XWapAppIdShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_XWapAppIdShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_XWapAppIdShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_XWapAppIdShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_XWapAppIdShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_XWapAppIdShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_XWapAppIdShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapAppIdShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_XWapAppIdShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_XWapAppIdShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_XWapAppIdShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_XWapAppIdShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_XWapAppIdShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapAppIdShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iXWapAppIdShortIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_XWapAppIdShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_XWapAppIdLongInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for XWapAppIdLongInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_XWapAppIdLongInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_XWapAppIdLongInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_XWapAppIdLongInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_XWapAppIdLongInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_XWapAppIdLongInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_XWapAppIdLongInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_XWapAppIdLongInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_XWapAppIdLongInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapAppIdLongInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_XWapAppIdLongInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_XWapAppIdLongInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_XWapAppIdLongInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_XWapAppIdLongInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_XWapAppIdLongInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapAppIdLongInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iXWapAppIdLongIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_XWapAppIdLongInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_XWapInitUriTextValue_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for XWapInitUriTextValue on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_XWapInitUriTextValue_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_XWapInitUriTextValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_XWapInitUriTextValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_XWapInitUriTextValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_XWapInitUriTextValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_XWapInitUriTextValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_XWapInitUriTextValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_XWapInitUriTextValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapInitUriTextValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_XWapInitUriTextValue_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_XWapInitUriTextValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_XWapInitUriTextValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_XWapInitUriTextValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_XWapInitUriTextValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapInitUriTextValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iXWapInitUriTextValueValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_XWapInitUriTextValue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_XWapInitUriNullValue_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for XWapInitUriNullValue on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_XWapInitUriNullValue_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_XWapInitUriNullValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_XWapInitUriNullValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_XWapInitUriNullValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_XWapInitUriNullValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_XWapInitUriNullValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_XWapInitUriNullValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_XWapInitUriNullValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapInitUriNullValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_XWapInitUriNullValue_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_XWapInitUriNullValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_XWapInitUriNullValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_XWapInitUriNullValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_XWapInitUriNullValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_XWapInitUriNullValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iXWapInitUriNullValueValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_XWapInitUriNullValue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_PushFlagShortInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for PushFlagShortInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_PushFlagShortInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_PushFlagShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_PushFlagShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_PushFlagShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_PushFlagShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_PushFlagShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_PushFlagShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_PushFlagShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_PushFlagShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_PushFlagShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_PushFlagShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_PushFlagShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_PushFlagShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_PushFlagShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_PushFlagShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iPushFlagShortIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_PushFlagShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_PushFlagLongInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for PushFlagLongInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_PushFlagLongInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_PushFlagLongInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_PushFlagLongInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_PushFlagLongInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_PushFlagLongInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_PushFlagLongInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_PushFlagLongInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_PushFlagLongInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_PushFlagLongInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_PushFlagLongInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_PushFlagLongInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_PushFlagLongInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_PushFlagLongInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_PushFlagLongInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_PushFlagLongInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iPushFlagLongIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_PushFlagLongInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_PushFlagTextValue_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for PushFlagTextValue on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_PushFlagTextValue_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_PushFlagTextValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_PushFlagTextValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_PushFlagTextValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_PushFlagTextValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_PushFlagTextValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_PushFlagTextValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_PushFlagTextValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_PushFlagTextValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_PushFlagTextValue_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_PushFlagTextValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_PushFlagTextValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_PushFlagTextValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_PushFlagTextValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_PushFlagTextValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iPushFlagTextValueValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_PushFlagTextValue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_AllowGet_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for AllowGet on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_AllowGet_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_AllowGet_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_AllowGet_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_AllowGet_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_AllowGet_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_AllowGet_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_AllowGet_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_AllowGet_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowGet_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_AllowGet_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_AllowGet_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_AllowGet_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_AllowGet_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_AllowGet_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowGet_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iAllowGetValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_AllowGet_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_AllowTrace_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for AllowTrace on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_AllowTrace_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_AllowTrace_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_AllowTrace_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_AllowTrace_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_AllowTrace_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_AllowTrace_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_AllowTrace_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_AllowTrace_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowTrace_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_AllowTrace_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_AllowTrace_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_AllowTrace_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_AllowTrace_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_AllowTrace_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowTrace_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iAllowTraceValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_AllowTrace_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_AllowLongInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for AllowLongInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_AllowLongInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_AllowLongInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_AllowLongInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_AllowLongInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_AllowLongInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_AllowLongInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_AllowLongInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_AllowLongInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowLongInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_AllowLongInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_AllowLongInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_AllowLongInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_AllowLongInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_AllowLongInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowLongInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iAllowLongIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_AllowLongInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_AllowInvalidShortInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for AllowInvalidShortInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_AllowInvalidShortInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_AllowInvalidShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_AllowInvalidShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_AllowInvalidShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_AllowInvalidShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_AllowInvalidShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_AllowInvalidShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_AllowInvalidShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowInvalidShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_AllowInvalidShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_AllowInvalidShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_AllowInvalidShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_AllowInvalidShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_AllowInvalidShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowInvalidShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iAllowInvalidShortIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_AllowInvalidShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_UpgradeText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for UpgradeText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_UpgradeText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_UpgradeText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_UpgradeText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_UpgradeText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_UpgradeText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_UpgradeText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_UpgradeText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_UpgradeText_UnitTest is fully constructed.
		@post			CWspHeaderReader_UpgradeText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_UpgradeText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_UpgradeText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_UpgradeText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_UpgradeText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_UpgradeText_UnitTest is fully constructed.
		@post			CWspHeaderReader_UpgradeText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iUpgradeTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_UpgradeText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_UpgradeNullText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for UpgradeNullText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_UpgradeNullText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_UpgradeNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_UpgradeNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_UpgradeNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_UpgradeNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_UpgradeNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_UpgradeNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_UpgradeNullText_UnitTest is fully constructed.
		@post			CWspHeaderReader_UpgradeNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_UpgradeNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_UpgradeNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_UpgradeNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_UpgradeNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_UpgradeNullText_UnitTest is fully constructed.
		@post			CWspHeaderReader_UpgradeNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iUpgradeNullTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_UpgradeNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WarningShortInt_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WarningShortInt on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WarningShortInt_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WarningShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WarningShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WarningShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WarningShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WarningShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WarningShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WarningShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_WarningShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WarningShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WarningShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WarningShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WarningShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WarningShortInt_UnitTest is fully constructed.
		@post			CWspHeaderReader_WarningShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWarningShortIntValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WarningShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WarningFullValue_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WarningFullValue on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WarningFullValue_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WarningFullValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WarningFullValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WarningFullValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WarningFullValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WarningFullValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WarningFullValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WarningFullValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_WarningFullValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WarningFullValue_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WarningFullValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WarningFullValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WarningFullValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WarningFullValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_WarningFullValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWarningFullValueValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WarningFullValue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WarningShortWarnNullAgent_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WarningShortWarnNullAgent on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WarningShortWarnNullAgent_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WarningShortWarnNullAgent_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WarningShortWarnNullAgent_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WarningShortWarnNullAgent_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WarningShortWarnNullAgent_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WarningShortWarnNullAgent_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WarningShortWarnNullAgent_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WarningShortWarnNullAgent_UnitTest is fully constructed.
		@post			CWspHeaderReader_WarningShortWarnNullAgent_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WarningShortWarnNullAgent_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WarningShortWarnNullAgent_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WarningShortWarnNullAgent_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WarningShortWarnNullAgent_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WarningShortWarnNullAgent_UnitTest is fully constructed.
		@post			CWspHeaderReader_WarningShortWarnNullAgent_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWarningShortWarnNullAgentValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WarningShortWarnNullAgent_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WarningShortIntNullText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WarningShortIntNullText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WarningShortIntNullText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WarningShortIntNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WarningShortIntNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WarningShortIntNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WarningShortIntNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WarningShortIntNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WarningShortIntNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WarningShortIntNullText_UnitTest is fully constructed.
		@post			CWspHeaderReader_WarningShortIntNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WarningShortIntNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WarningShortIntNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WarningShortIntNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WarningShortIntNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WarningShortIntNullText_UnitTest is fully constructed.
		@post			CWspHeaderReader_WarningShortIntNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWarningShortIntNullTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WarningShortIntNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for WarningShortIntNullAgentNullText on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest is fully constructed.
		@post			CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest is fully constructed.
		@post			CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iWarningShortIntNullAgentNullTextValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_WarningShortIntNullAgentNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentEncodingMultiHeader_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentEncodingMultiHeader on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentEncodingMultiHeader_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentEncodingMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentEncodingMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentEncodingMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentEncodingMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentEncodingMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentEncodingMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentEncodingMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentEncodingMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentEncodingMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentEncodingMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentEncodingMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentEncodingMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentEncodingMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentEncodingMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentEncodingMultiHeaderValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentEncodingMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ContentLanguageMultiHeader_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ContentLanguageMultiHeader on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ContentLanguageMultiHeader_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ContentLanguageMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ContentLanguageMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentLanguageMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ContentLanguageMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ContentLanguageMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ContentLanguageMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ContentLanguageMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ContentLanguageMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ContentLanguageMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ContentLanguageMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ContentLanguageMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_ContentLanguageMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iContentLanguageMultiHeaderValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ContentLanguageMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ViaMultiHeader_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ViaMultiHeader on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ViaMultiHeader_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ViaMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ViaMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ViaMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ViaMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ViaMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ViaMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ViaMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_ViaMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ViaMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ViaMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ViaMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ViaMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ViaMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_ViaMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iViaMultiHeaderValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ViaMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_TrailerMultiHeader_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for TrailerMultiHeader on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_TrailerMultiHeader_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_TrailerMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_TrailerMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_TrailerMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_TrailerMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_TrailerMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_TrailerMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_TrailerMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_TrailerMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_TrailerMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_TrailerMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_TrailerMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_TrailerMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_TrailerMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_TrailerMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iTrailerMultiHeaderValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_TrailerMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_UpgradeMultiHeader_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for UpgradeMultiHeader on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_UpgradeMultiHeader_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_UpgradeMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_UpgradeMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_UpgradeMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_UpgradeMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_UpgradeMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_UpgradeMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_UpgradeMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_UpgradeMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_UpgradeMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_UpgradeMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_UpgradeMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_UpgradeMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_UpgradeMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_UpgradeMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iUpgradeMultiHeaderValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_UpgradeMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_AllowMultiHeader_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for AllowMultiHeader on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_AllowMultiHeader_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_AllowMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_AllowMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_AllowMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_AllowMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_AllowMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_AllowMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_AllowMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_AllowMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_AllowMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_AllowMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_AllowMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_AllowMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iAllowMultiHeaderValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_AllowMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_AllowMultiHeader_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for AllowMultiHeader on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_UnknownHeader_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_AllowMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_AllowMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_UnknownHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_AllowMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_AllowMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_UnknownHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_AllowMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_AllowMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_UnknownHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_UnknownHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_AllowMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_UnknownHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_UnknownHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_UnknownHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iUnknownHeaderValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_UnknownHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_UnknownHeaderNull_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for UnknownHeaderNull on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_UnknownHeaderNull_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_UnknownHeaderNull_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_UnknownHeaderNull_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_UnknownHeaderNull_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ContentTypeNull_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_UnknownHeaderNull_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_UnknownHeaderNull_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_UnknownHeaderNull_UnitTest is fully constructed.
		@post			CWspHeaderReader_UnknownHeaderNull_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_UnknownHeaderNull_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_UnknownHeaderNull_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_UnknownHeaderNull_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_UnknownHeaderNull_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_UnknownHeaderNull_UnitTest is fully constructed.
		@post			CWspHeaderReader_UnknownHeaderNull_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iUnknownHeaderNullValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_UnknownHeaderNull_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_MultiUnknownHeader_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for MultiUnknownHeader on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_MultiUnknownHeader_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_MultiUnknownHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_MultiUnknownHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_MultiUnknownHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_AllowMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_MultiUnknownHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_MultiUnknownHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_MultiUnknownHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_MultiUnknownHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_MultiUnknownHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_MultiUnknownHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_MultiUnknownHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_MultiUnknownHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_MultiUnknownHeader_UnitTest is fully constructed.
		@post			CWspHeaderReader_MultiUnknownHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iMultiUnknownHeaderValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_AllowMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ProfileWarningWarnCode_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ProfileWarningWarnCode on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ProfileWarningWarnCode_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ProfileWarningWarnCode_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningWarnCode_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ProfileWarningWarnCode_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ProfileWarningWarnCode_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ProfileWarningWarnCode_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ProfileWarningWarnCode_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ProfileWarningWarnCode_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningWarnCode_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ProfileWarningWarnCode_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ProfileWarningWarnCode_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningWarnCode_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ProfileWarningWarnCode_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ProfileWarningWarnCode_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningWarnCode_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iProfileWarningWarnCodeValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ProfileWarningWarnCode_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ProfileWarningWarnCodeTextWarnTarget on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iProfileWarningWarnCodeTextWarnTargetValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ProfileWarningWarnCodeTextWarnTarget_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ProfileWarningWarnCodeHostWarnTarget on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iProfileWarningWarnCodeHostWarnTargetValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ProfileWarningWarnCodeHostWarnTarget_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ProfileWarningLongFullValue_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ProfileWarningLongFullValue on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ProfileWarningLongFullValue_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ProfileWarningLongFullValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningLongFullValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ProfileWarningLongFullValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ProfileWarningLongFullValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ProfileWarningLongFullValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ProfileWarningLongFullValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ProfileWarningLongFullValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningLongFullValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ProfileWarningLongFullValue_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ProfileWarningLongFullValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningLongFullValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ProfileWarningLongFullValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ProfileWarningLongFullValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningLongFullValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iProfileWarningLongFullValueValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ProfileWarningLongFullValue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ProfileWarningLongWarnCodeHostWarnTarget on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iProfileWarningLongWarnCodeHostWarnTargetValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTarget_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest
	@Depends CWspHeaderReader & CUnitTest 

	Comments : Unit Test for ProfileWarningLongWarnCodeHostWarnTargetDateValue on CWspHeaderReader, the class under test.
 */
class CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest : public CUnitTest
	{
public:
	/**
		@fn				NewL(CDataLogger& aDataLogger,
							MUnitTestObserver& aObserver)
		Intended Usage	: Standard two-phase construction which leaves nothing on the
						cleanup stack.
		Error Condition	: Leaves with the error code.
		@exceptions		KErrNoMemory
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@return			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest();

private:
	/**
		@fn				CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class.
		@pre 			None.
		@post			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest is fully constructed.
		@post			CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderReader class tested by this UnitTest's transitions.
	 */
	CWspHeaderReader_UnitTestContext* iUTContext;
	TWspHeaderReader_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderReader_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderReader_DecodeHeaderL_TransitionValidator*	iProfileWarningLongWarnCodeHostWarnTargetDateValueValidator;
	TWspHeaderReader_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderReader_ProfileWarningLongWarnCodeHostWarnTargetDateValue_UnitTest


#endif	// __WSPHEADERREADERUNITTEST_H__
