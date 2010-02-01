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
// The unit test class definitions for the CWspHeaderWriter class.
// 
//

/**
 @file WspHeaderWriterUnitTest.h
*/
#ifndef __WSPHEADERWRITERUNITTEST_H__
#define __WSPHEADERWRITERUNITTEST_H__

#include "TestBedDefinitions.h"
#include "WspHeaderWriterStateAccessor.h"
#include "WspHeaderWriterTransitionValidation.h"
#include "WspHeaderWriterTransitions.h"

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_CreateAndDestroy_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for CreateAndDestroy on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_CreateAndDestroy_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_CreateAndDestroy_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_CreateAndDestroy_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_CreateAndDestroy_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_CreateAndDestroy_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_CreateAndDestroy_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_CreateAndDestroy_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_CreateAndDestroy_UnitTest is fully constructed.
		@post			CWspHeaderWriter_CreateAndDestroy_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_CreateAndDestroy_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_CreateAndDestroy_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_CreateAndDestroy_UnitTest is fully constructed.
		@post			CWspHeaderWriter_CreateAndDestroy_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_CreateAndDestroy_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_UserAgent_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for UserAgent on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_UserAgent_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_UserAgent_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_UserAgent_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_UserAgent_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_UserAgent_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_UserAgent_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_UserAgent_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_UserAgent_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UserAgent_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_UserAgent_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_UserAgent_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_UserAgent_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_UserAgent_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_UserAgent_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UserAgent_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iUserAgentValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_UserAgent_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_UserAgentNullText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for UserAgentNullText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_UserAgentNullText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_UserAgentNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_UserAgentNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_UserAgentNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_UserAgentNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_UserAgentNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_UserAgentNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_UserAgentNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UserAgentNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_UserAgentNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_UserAgentNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_UserAgentNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_UserAgentNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_UserAgentNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UserAgentNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iUserAgentNullTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_UserAgentNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AuthorizationBasic_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AuthorizationBasic on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AuthorizationBasic_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AuthorizationBasic_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationBasic_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AuthorizationBasic_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AuthorizationBasic_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AuthorizationBasic_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AuthorizationBasic_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AuthorizationBasic_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationBasic_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AuthorizationBasic_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AuthorizationBasic_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationBasic_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AuthorizationBasic_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AuthorizationBasic_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationBasic_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAuthorizationBasicValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AuthorizationBasic_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AuthorizationBasicNullUser on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAuthorizationBasicNullUserValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AuthorizationBasicNullUser_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_BasicNullPassword_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for BasicNullPassword on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_BasicNullPassword_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_BasicNullPassword_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_BasicNullPassword_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_BasicNullPassword_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_BasicNullPassword_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_BasicNullPassword_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_BasicNullPassword_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_BasicNullPassword_UnitTest is fully constructed.
		@post			CWspHeaderWriter_BasicNullPassword_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_BasicNullPassword_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_BasicNullPassword_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_BasicNullPassword_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_BasicNullPassword_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_BasicNullPassword_UnitTest is fully constructed.
		@post			CWspHeaderWriter_BasicNullPassword_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iBasicNullPasswordValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_BasicNullPassword_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_BasicNullUserNullPass_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for BasicNullUserNullPass on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_BasicNullUserNullPass_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_BasicNullUserNullPass_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_BasicNullUserNullPass_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_BasicNullUserNullPass_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_BasicNullUserNullPass_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_BasicNullUserNullPass_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_BasicNullUserNullPass_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_BasicNullUserNullPass_UnitTest is fully constructed.
		@post			CWspHeaderWriter_BasicNullUserNullPass_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_BasicNullUserNullPass_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_BasicNullUserNullPass_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_BasicNullUserNullPass_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_BasicNullUserNullPass_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_BasicNullUserNullPass_UnitTest is fully constructed.
		@post			CWspHeaderWriter_BasicNullUserNullPass_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iBasicNullUserNullPassValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_BasicNullUserNullPass_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AuthorizationSchemeNoParam on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAuthorizationSchemeNoParamValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AuthorizationSchemeNoParam_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AuthorizationInvalidSchemeToken on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAuthorizationInvalidSchemeTokenValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AuthorizationInvalidSchemeToken_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AuthorizationScheme2Params_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AuthorizationScheme2Params on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AuthorizationScheme2Params_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AuthorizationScheme2Params_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationScheme2Params_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AuthorizationScheme2Params_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AuthorizationScheme2Params_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AuthorizationScheme2Params_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AuthorizationScheme2Params_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AuthorizationScheme2Params_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationScheme2Params_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AuthorizationScheme2Params_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AuthorizationScheme2Params_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationScheme2Params_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AuthorizationScheme2Params_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AuthorizationScheme2Params_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationScheme2Params_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAuthorizationScheme2ParamsValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AuthorizationScheme2Params_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AuthorizationScheme2ParamV1_2 on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAuthorizationScheme2ParamV1_2Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AuthorizationScheme2ParamV1_2_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentLocation_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentLocation on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentLocation_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentLocation_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLocation_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentLocation_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentLocation_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentLocation_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentLocation_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentLocation_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLocation_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentLocation_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentLocation_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLocation_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentLocation_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentLocation_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLocation_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentLocationValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentLocation_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentLocationNullText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentLocationNullText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentLocationNullText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentLocationNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLocationNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentLocationNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentLocationNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentLocationNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentLocationNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentLocationNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLocationNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentLocationNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentLocationNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLocationNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentLocationNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentLocationNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLocationNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentLocationNullTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentLocationNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentTypeShortInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentTypeShortInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentTypeShortInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentTypeShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentTypeShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentTypeShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentTypeShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentTypeShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentTypeShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentTypeShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentTypeShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentTypeShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentTypeShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentTypeShortIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentTypeShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentTypeConstrainedText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentTypeConstrainedText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentTypeConstrainedText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentTypeConstrainedText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeConstrainedText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentTypeConstrainedText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentTypeConstrainedText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentTypeConstrainedText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentTypeConstrainedText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentTypeConstrainedText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeConstrainedText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentTypeConstrainedText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentTypeConstrainedText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeConstrainedText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentTypeConstrainedText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentTypeConstrainedText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeConstrainedText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentTypeConstrainedTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentTypeConstrainedText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentTypeConstrainedNullText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentTypeConstrainedNullTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentTypeConstrainedNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentTypeGeneralShortInt1Param on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentTypeGeneralShortInt1ParamValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentTypeGeneralShortInt1Param_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentTypeGeneralLongInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentTypeGeneralLongIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentTypeGeneralLongInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentTypeGeneralLongInt2Params on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentTypeGeneralLongInt2ParamsValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentTypeGeneralLongInt2Params_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentTypeGeneralText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentTypeGeneralText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentTypeGeneralText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentTypeGeneralText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeGeneralText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentTypeGeneralText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentTypeGeneralText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentTypeGeneralText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentTypeGeneralText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentTypeGeneralText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeGeneralText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentTypeGeneralText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentTypeGeneralText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeGeneralText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentTypeGeneralText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentTypeGeneralText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeGeneralText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentTypeGeneralTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentTypeGeneralText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentTypeGeneralNullText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentTypeGeneralNullTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentTypeGeneralNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptCharsetShortInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptCharsetShortInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptCharsetShortInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptCharsetShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptCharsetShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptCharsetShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptCharsetShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptCharsetShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptCharsetShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptCharsetShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptCharsetShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptCharsetShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptCharsetShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptCharsetShortIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptCharsetShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptCharsetText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptCharsetText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptCharsetText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptCharsetText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptCharsetText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptCharsetText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptCharsetText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptCharsetText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptCharsetText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptCharsetText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptCharsetText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptCharsetText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptCharsetText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptCharsetTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptCharsetText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptCharsetAnyCharset on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptCharsetAnyCharsetValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptCharsetAnyCharset_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptCharsetGeneralShortIntWithQVal on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptCharsetGeneralShortIntWithQValValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptCharsetGeneralShortIntWithQVal_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptCharsetGeneralLongIntQVal on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptCharsetGeneralLongIntQValValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptCharsetGeneralLongIntQVal_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptCharsetGeneralTextQVal on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptCharsetGeneralTextQValValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptCharsetGeneralTextQVal_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptCharsetAnyCharsetV1_2 on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptCharsetAnyCharsetV1_2Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptCharsetAnyCharsetV1_2_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptLanguageShortInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptLanguageShortInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptLanguageShortInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptLanguageShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptLanguageShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptLanguageShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptLanguageShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptLanguageShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptLanguageShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptLanguageShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptLanguageShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptLanguageShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptLanguageShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptLanguageShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptLanguageShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptLanguageShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptLanguageShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptLanguageShortIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptLanguageShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptLanguageAnyLang on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptLanguageAnyLangValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptLanguageAnyLang_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptLanguageGeneralShortQVal on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptLanguageGeneralShortQValValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptLanguageGeneralShortQVal_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentLanguageGeneralLongInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentLanguageGeneralLongIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentLanguageGeneralLongInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptLanguageGeneralTextQVal on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptLanguageGeneralTextQValValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptLanguageGeneralTextQVal_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptLanguageText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptLanguageText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptLanguageText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptLanguageText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptLanguageText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptLanguageText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptLanguageText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptLanguageText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptLanguageText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptLanguageText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptLanguageText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptLanguageText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptLanguageText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptLanguageText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptLanguageText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptLanguageText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptLanguageText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptLanguageTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptLanguageText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentEncodingGZip_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentEncodingGZip on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentEncodingGZip_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentEncodingGZip_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentEncodingGZip_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentEncodingGZip_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentEncodingGZip_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentEncodingGZip_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentEncodingGZip_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentEncodingGZip_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentEncodingGZip_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentEncodingGZip_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentEncodingGZip_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentEncodingGZip_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentEncodingGZip_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentEncodingGZip_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentEncodingGZip_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentEncodingGZipValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentEncodingGZip_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentEncodingCompress_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentEncodingCompress on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentEncodingCompress_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentEncodingCompress_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentEncodingCompress_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentEncodingCompress_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentEncodingCompress_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentEncodingCompress_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentEncodingCompress_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentEncodingCompress_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentEncodingCompress_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentEncodingCompress_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentEncodingCompress_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentEncodingCompress_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentEncodingCompress_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentEncodingCompress_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentEncodingCompress_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentEncodingCompressValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentEncodingCompress_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentEncodingDeflate_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentEncodingDeflate on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentEncodingDeflate_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentEncodingDeflate_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentEncodingDeflate_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentEncodingDeflate_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentEncodingDeflate_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentEncodingDeflate_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentEncodingDeflate_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentEncodingDeflate_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentEncodingDeflate_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentEncodingDeflate_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentEncodingDeflate_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentEncodingDeflate_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentEncodingDeflate_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentEncodingDeflate_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentEncodingDeflate_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentEncodingDeflateValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentEncodingDeflate_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentEncodingText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentEncodingText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentEncodingText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentEncodingText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentEncodingText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentEncodingText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentEncodingText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentEncodingText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentEncodingText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentEncodingText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentEncodingText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentEncodingText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentEncodingText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentEncodingText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentEncodingText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentEncodingText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentEncodingText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentEncodingTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentEncodingText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentLanguageShortInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentLanguageShortInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentLanguageShortInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentLanguageShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLanguageShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentLanguageShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentLanguageShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentLanguageShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentLanguageShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentLanguageShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLanguageShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentLanguageShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentLanguageShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLanguageShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentLanguageShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentLanguageShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLanguageShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentLanguageShortIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentLanguageShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentLanguageLongInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentLanguageLongInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentLanguageLongInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentLanguageLongInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLanguageLongInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentLanguageLongInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentLanguageLongInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentLanguageLongInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentLanguageLongInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentLanguageLongInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLanguageLongInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentLanguageLongInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentLanguageLongInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLanguageLongInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentLanguageLongInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentLanguageLongInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLanguageLongInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentLanguageLongIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentLanguageLongInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentLanguageText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentLanguageText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentLanguageText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentLanguageText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLanguageText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentLanguageText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentLanguageText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentLanguageText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentLanguageText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentLanguageText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLanguageText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentLanguageText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentLanguageText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentLanguageText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentLanguageText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentLanguageText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentLanguageText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentLanguageTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentLanguageText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentMD516ByteData_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentMD516ByteData on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentMD516ByteData_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentMD516ByteData_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentMD516ByteData_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentMD516ByteData_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentMD516ByteData_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentMD516ByteData_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentMD516ByteData_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentMD516ByteData_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentMD516ByteData_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentMD516ByteData_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentMD516ByteData_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentMD516ByteData_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentMD516ByteData_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentMD516ByteData_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentMD516ByteData_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentMD516ByteDataValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentMD516ByteData_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentMD531ByteData_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentMD531ByteData on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentMD531ByteData_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentMD531ByteData_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentMD531ByteData_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentMD531ByteData_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentMD531ByteData_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentMD531ByteData_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentMD531ByteData_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentMD531ByteData_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentMD531ByteData_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentMD531ByteData_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentMD531ByteData_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentMD531ByteData_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentMD531ByteData_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentMD531ByteData_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentMD531ByteData_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentMD531ByteDataValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentMD531ByteData_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_Date_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for Date on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_Date_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_Date_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_Date_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_Date_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_Date_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_Date_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_Date_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_Date_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Date_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_Date_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_Date_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_Date_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_Date_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_Date_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Date_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iDateValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_Date_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_FromText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for FromText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_FromText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_FromText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_FromText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_FromText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_FromText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_FromText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_FromText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_FromText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_FromText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_FromText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_FromText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_FromText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_FromText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_FromText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_FromText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iFromTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_FromText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_FromNullText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for FromNullText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_FromNullText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_FromNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_FromNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_FromNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_FromNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_FromNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_FromNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_FromNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_FromNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_FromNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_FromNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_FromNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_FromNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_FromNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_FromNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iFromNullTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_FromNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_LastModified_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for LastModified on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_LastModified_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_LastModified_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_LastModified_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_LastModified_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_LastModified_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_LastModified_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_LastModified_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_LastModified_UnitTest is fully constructed.
		@post			CWspHeaderWriter_LastModified_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_LastModified_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_LastModified_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_LastModified_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_LastModified_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_LastModified_UnitTest is fully constructed.
		@post			CWspHeaderWriter_LastModified_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iLastModifiedValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_LastModified_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for RangeByteRangeNoLastPos on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iRangeByteRangeNoLastPosValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_RangeByteRangeNoLastPos_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for RangeByteRangeWithLastPos on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iRangeByteRangeWithLastPosValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_RangeByteRangeWithLastPos_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_RangeSuffixRange_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for RangeSuffixRange on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_RangeSuffixRange_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_RangeSuffixRange_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_RangeSuffixRange_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_RangeSuffixRange_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_RangeSuffixRange_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_RangeSuffixRange_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_RangeSuffixRange_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_RangeSuffixRange_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RangeSuffixRange_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_RangeSuffixRange_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_RangeSuffixRange_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_RangeSuffixRange_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_RangeSuffixRange_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_RangeSuffixRange_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RangeSuffixRange_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iRangeSuffixRangeValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_RangeSuffixRange_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_RefererUri_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for RefererUri on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_RefererUri_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_RefererUri_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_RefererUri_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_RefererUri_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_RefererUri_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_RefererUri_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_RefererUri_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_RefererUri_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RefererUri_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_RefererUri_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_RefererUri_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_RefererUri_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_RefererUri_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_RefererUri_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RefererUri_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iRefererUriValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_RefererUri_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_RefererNullText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for RefererNullText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_RefererNullText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_RefererNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_RefererNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_RefererNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_RefererNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_RefererNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_RefererNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_RefererNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RefererNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_RefererNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_RefererNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_RefererNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_RefererNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_RefererNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RefererNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iRefererNullTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_RefererNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ViaUri_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ViaUri on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ViaUri_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ViaUri_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ViaUri_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ViaUri_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ViaUri_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ViaUri_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ViaUri_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ViaUri_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ViaUri_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ViaUri_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ViaUri_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ViaUri_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ViaUri_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ViaUri_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ViaUri_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iViaUriValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ViaUri_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ViaNullText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ViaNullText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ViaNullText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ViaNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ViaNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ViaNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ViaNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ViaNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ViaNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ViaNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ViaNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ViaNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ViaNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ViaNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ViaNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ViaNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ViaNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iViaNullTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ViaNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_XWapAppIdUri_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for XWapAppIdUri on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_XWapAppIdUri_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_XWapAppIdUri_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_XWapAppIdUri_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_XWapAppIdUri_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_XWapAppIdUri_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_XWapAppIdUri_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_XWapAppIdUri_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_XWapAppIdUri_UnitTest is fully constructed.
		@post			CWspHeaderWriter_XWapAppIdUri_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_XWapAppIdUri_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_XWapAppIdUri_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_XWapAppIdUri_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_XWapAppIdUri_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_XWapAppIdUri_UnitTest is fully constructed.
		@post			CWspHeaderWriter_XWapAppIdUri_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iXWapAppIdUriValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_XWapAppIdUri_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_XWapAppIdNullText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for XWapAppIdNullText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_XWapAppIdNullText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_XWapAppIdNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_XWapAppIdNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_XWapAppIdNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_XWapAppIdNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_XWapAppIdNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_XWapAppIdNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_XWapAppIdNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_XWapAppIdNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_XWapAppIdNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_XWapAppIdNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_XWapAppIdNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_XWapAppIdNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_XWapAppIdNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_XWapAppIdNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iXWapAppIdNullTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_XWapAppIdNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_XWapAppIdShortInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for XWapAppIdShortInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_XWapAppIdShortInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_XWapAppIdShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_XWapAppIdShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_XWapAppIdShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_XWapAppIdShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_XWapAppIdShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_XWapAppIdShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_XWapAppIdShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_XWapAppIdShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_XWapAppIdShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_XWapAppIdShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_XWapAppIdShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_XWapAppIdShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_XWapAppIdShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_XWapAppIdShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iXWapAppIdShortIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_XWapAppIdShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_XWapAppIdLongInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for XWapAppIdLongInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_XWapAppIdLongInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_XWapAppIdLongInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_XWapAppIdLongInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_XWapAppIdLongInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_XWapAppIdLongInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_XWapAppIdLongInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_XWapAppIdLongInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_XWapAppIdLongInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_XWapAppIdLongInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_XWapAppIdLongInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_XWapAppIdLongInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_XWapAppIdLongInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_XWapAppIdLongInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_XWapAppIdLongInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_XWapAppIdLongInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iXWapAppIdLongIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_XWapAppIdLongInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_EncodingVersion_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for EncodingVersion on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_EncodingVersion_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_EncodingVersion_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_EncodingVersion_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_EncodingVersion_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_EncodingVersion_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_EncodingVersion_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_EncodingVersion_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_EncodingVersion_UnitTest is fully constructed.
		@post			CWspHeaderWriter_EncodingVersion_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_EncodingVersion_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_EncodingVersion_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_EncodingVersion_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_EncodingVersion_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_EncodingVersion_UnitTest is fully constructed.
		@post			CWspHeaderWriter_EncodingVersion_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iEncodingVersionValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_EncodingVersion_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for EncodingVersionCodePageInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iEncodingVersionCodePageIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_EncodingVersionCodePageInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for EncodingVersionCodePageIntWithVersion on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest is fully constructed.
		@post			CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest is fully constructed.
		@post			CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iEncodingVersionCodePageIntWithVersionValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_EncodingVersionCodePageIntWithVersion_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_Expect100Continue_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for Expect100Continue on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_Expect100Continue_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_Expect100Continue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_Expect100Continue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_Expect100Continue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_Expect100Continue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_Expect100Continue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_Expect100Continue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_Expect100Continue_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Expect100Continue_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_Expect100Continue_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_Expect100Continue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_Expect100Continue_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_Expect100Continue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_Expect100Continue_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Expect100Continue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iExpect100ContinueValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_Expect100Continue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ExpectExpressionTokenText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ExpectExpressionTokenText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ExpectExpressionTokenText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ExpectExpressionTokenText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ExpectExpressionTokenText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ExpectExpressionTokenText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ExpectExpressionTokenText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ExpectExpressionTokenText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ExpectExpressionTokenText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ExpectExpressionTokenText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ExpectExpressionTokenText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ExpectExpressionTokenText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ExpectExpressionTokenText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ExpectExpressionTokenText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ExpectExpressionTokenText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ExpectExpressionTokenText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ExpectExpressionTokenText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iExpectExpressionTokenTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ExpectExpressionTokenText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ExpectExpressionQuotedText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iExpectExpressionQuotedTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ExpectExpressionQuotedText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ExpectExpressionTokenTextParam on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iExpectExpressionTokenTextParamValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ExpectExpressionTokenTextParam_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ExpectExpressionQuotedTextParam on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iExpectExpressionQuotedTextParamValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ExpectExpressionQuotedTextParam_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ExpectExpression2Params_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ExpectExpression2Params on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ExpectExpression2Params_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ExpectExpression2Params_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ExpectExpression2Params_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ExpectExpression2Params_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ExpectExpression2Params_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ExpectExpression2Params_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ExpectExpression2Params_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ExpectExpression2Params_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ExpectExpression2Params_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ExpectExpression2Params_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ExpectExpression2Params_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ExpectExpression2Params_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ExpectExpression2Params_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ExpectExpression2Params_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ExpectExpression2Params_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iExpectExpression2ParamsValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ExpectExpression2Params_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TETrailers_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TETrailers on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TETrailers_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TETrailers_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TETrailers_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TETrailers_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TETrailers_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TETrailers_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TETrailers_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TETrailers_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TETrailers_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TETrailers_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TETrailers_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TETrailers_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TETrailers_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TETrailers_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TETrailers_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTETrailersValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TETrailers_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TEChunked_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TEChunked on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TEChunked_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TEChunked_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TEChunked_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TEChunked_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TEChunked_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TEChunked_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TEChunked_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TEChunked_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TEChunked_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TEChunked_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TEChunked_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TEChunked_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TEChunked_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TEChunked_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TEChunked_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTEChunkedValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TEChunked_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TEIdentitiyWithQValue on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTEIdentitiyWithQValueValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TEIdentitiyWithQValue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TEGzip_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TEGzip on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TEGzip_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TEGzip_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TEGzip_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TEGzip_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TEGzip_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TEGzip_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TEGzip_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TEGzip_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TEGzip_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TEGzip_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TEGzip_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TEGzip_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TEGzip_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TEGzip_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TEGzip_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTEGzipValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TEGzip_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TECompressWithQValue_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TECompressWithQValue on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TECompressWithQValue_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TECompressWithQValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TECompressWithQValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TECompressWithQValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TECompressWithQValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TECompressWithQValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TECompressWithQValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TECompressWithQValue_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TECompressWithQValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TECompressWithQValue_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TECompressWithQValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TECompressWithQValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TECompressWithQValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TECompressWithQValue_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TECompressWithQValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTECompressWithQValueValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TECompressWithQValue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TETokenText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TETokenText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TETokenText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TETokenText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TETokenText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TETokenText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TETokenText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TETokenText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TETokenText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TETokenText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TETokenText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TETokenText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TETokenText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TETokenText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TETokenText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TETokenText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TETokenText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTETokenTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TETokenText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TETokenTextWithQValue_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TETokenTextWithQValue on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TETokenTextWithQValue_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TETokenTextWithQValue_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TETokenTextWithQValue_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TETokenTextWithQValue_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TETokenTextWithQValue_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TETokenTextWithQValue_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TETokenTextWithQValue_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TETokenTextWithQValue_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TETokenTextWithQValue_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TETokenTextWithQValue_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TETokenTextWithQValue_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TETokenTextWithQValue_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TETokenTextWithQValue_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TETokenTextWithQValue_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TETokenTextWithQValue_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTETokenTextWithQValueValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TETokenTextWithQValue_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TrailerShortInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TrailerShortInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TrailerShortInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TrailerShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TrailerShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TrailerShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TrailerShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TrailerShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TrailerShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TrailerShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TrailerShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TrailerShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TrailerShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TrailerShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TrailerShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TrailerShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TrailerShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTrailerShortIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TrailerShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for Trailerv1_4NewShortInt on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTrailerv1_4NewShortIntValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_Trailerv1_4NewShortInt_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TrailerTokenText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TrailerTokenText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TrailerTokenText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TrailerTokenText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TrailerTokenText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TrailerTokenText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TrailerTokenText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TrailerTokenText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TrailerTokenText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TrailerTokenText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TrailerTokenText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TrailerTokenText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TrailerTokenText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TrailerTokenText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TrailerTokenText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TrailerTokenText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TrailerTokenText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTrailerTokenTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TrailerTokenText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptEncodingGzip_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptEncodingGzip on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptEncodingGzip_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptEncodingGzip_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingGzip_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptEncodingGzip_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptEncodingGzip_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptEncodingGzip_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptEncodingGzip_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptEncodingGzip_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingGzip_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptEncodingGzip_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptEncodingGzip_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingGzip_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptEncodingGzip_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptEncodingGzip_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingGzip_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptEncodingGzipValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptEncodingGzip_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptEncodingCompress_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptEncodingCompress on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptEncodingCompress_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptEncodingCompress_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingCompress_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptEncodingCompress_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptEncodingCompress_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptEncodingCompress_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptEncodingCompress_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptEncodingCompress_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingCompress_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptEncodingCompress_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptEncodingCompress_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingCompress_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptEncodingCompress_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptEncodingCompress_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingCompress_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptEncodingCompressValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptEncodingCompress_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptEncodingDeflate_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptEncodingDeflate on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptEncodingDeflate_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptEncodingDeflate_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingDeflate_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptEncodingDeflate_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptEncodingDeflate_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptEncodingDeflate_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptEncodingDeflate_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptEncodingDeflate_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingDeflate_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptEncodingDeflate_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptEncodingDeflate_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingDeflate_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptEncodingDeflate_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptEncodingDeflate_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingDeflate_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptEncodingDeflateValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptEncodingDeflate_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptEncodingTokenText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptEncodingTokenText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptEncodingTokenText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptEncodingTokenText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingTokenText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptEncodingTokenText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptEncodingTokenText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptEncodingTokenText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptEncodingTokenText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptEncodingTokenText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingTokenText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptEncodingTokenText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptEncodingTokenText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingTokenText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptEncodingTokenText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptEncodingTokenText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingTokenText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptEncodingTokenTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptEncodingTokenText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptEncodingGeneralAny on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptEncodingGeneralAnyValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptEncodingGeneralAny_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptEncodingGeneralAnyQVal on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptEncodingGeneralAnyQValValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptEncodingGeneralAnyQVal_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptEncodingGeneralGzipQVal on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptEncodingGeneralGzipQValValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptEncodingGeneralGzipQVal_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptEncodingGeneralAnyQValVersion1_2 on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptEncodingGeneralAnyQValVersion1_2Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptEncodingGeneralAnyQValVersion1_2_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentRange_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentRange on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentRange_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentRange_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentRange_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentRange_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentRange_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentRange_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentRange_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentRange_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentRange_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentRange_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentRange_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentRange_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentRange_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentRange_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentRange_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentRangeValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentRange_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentRangeUnknownLength_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentRangeUnknownLength on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentRangeUnknownLength_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentRangeUnknownLength_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentRangeUnknownLength_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentRangeUnknownLength_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentRangeUnknownLength_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentRangeUnknownLength_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentRangeUnknownLength_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentRangeUnknownLength_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentRangeUnknownLength_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentRangeUnknownLength_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentRangeUnknownLength_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentRangeUnknownLength_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentRangeUnknownLength_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentRangeUnknownLength_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentRangeUnknownLength_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentRangeUnknownLengthValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentRangeUnknownLength_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AllowGet_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AllowGet on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AllowGet_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AllowGet_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AllowGet_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AllowGet_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AllowGet_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AllowGet_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AllowGet_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AllowGet_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AllowGet_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AllowGet_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AllowGet_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AllowGet_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AllowGet_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AllowGet_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AllowGet_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAllowGetValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AllowGet_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AllowPost_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AllowPost on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AllowPost_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AllowPost_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AllowPost_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AllowPost_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AllowPost_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AllowPost_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AllowPost_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AllowPost_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AllowPost_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AllowPost_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AllowPost_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AllowPost_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AllowPost_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AllowPost_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AllowPost_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAllowPostValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AllowPost_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AllowInvalid_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AllowInvalid on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AllowInvalid_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AllowInvalid_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AllowInvalid_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AllowInvalid_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AllowInvalid_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AllowInvalid_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AllowInvalid_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AllowInvalid_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AllowInvalid_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AllowInvalid_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AllowInvalid_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AllowInvalid_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AllowInvalid_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AllowInvalid_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AllowInvalid_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAllowInvalidValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AllowInvalid_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_PragmaNoCache_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for PragmaNoCache on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_PragmaNoCache_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_PragmaNoCache_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_PragmaNoCache_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_PragmaNoCache_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_PragmaNoCache_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_PragmaNoCache_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_PragmaNoCache_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_PragmaNoCache_UnitTest is fully constructed.
		@post			CWspHeaderWriter_PragmaNoCache_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_PragmaNoCache_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_PragmaNoCache_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_PragmaNoCache_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_PragmaNoCache_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_PragmaNoCache_UnitTest is fully constructed.
		@post			CWspHeaderWriter_PragmaNoCache_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iPragmaNoCacheValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_PragmaNoCache_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_PragmaParameter_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for PragmaParameter on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_PragmaParameter_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_PragmaParameter_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_PragmaParameter_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_PragmaParameter_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_PragmaParameter_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_PragmaParameter_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_PragmaParameter_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_PragmaParameter_UnitTest is fully constructed.
		@post			CWspHeaderWriter_PragmaParameter_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_PragmaParameter_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_PragmaParameter_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_PragmaParameter_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_PragmaParameter_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_PragmaParameter_UnitTest is fully constructed.
		@post			CWspHeaderWriter_PragmaParameter_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iPragmaParameterValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_PragmaParameter_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_UpgradeText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for UpgradeText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_UpgradeText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_UpgradeText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_UpgradeText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_UpgradeText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_UpgradeText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_UpgradeText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_UpgradeText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_UpgradeText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UpgradeText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_UpgradeText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_UpgradeText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_UpgradeText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_UpgradeText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_UpgradeText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UpgradeText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iUpgradeTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_UpgradeText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_UpgradeNullText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for UpgradeNullText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_UpgradeNullText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_UpgradeNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_UpgradeNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_UpgradeNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_UpgradeNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_UpgradeNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_UpgradeNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_UpgradeNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UpgradeNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_UpgradeNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_UpgradeNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_UpgradeNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_UpgradeNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_UpgradeNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UpgradeNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iUpgradeNullTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_UpgradeNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_Warning110_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for Warning110 on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_Warning110_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_Warning110_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_Warning110_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_Warning110_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_Warning110_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_Warning110_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_Warning110_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_Warning110_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Warning110_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_Warning110_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_Warning110_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_Warning110_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_Warning110_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_Warning110_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Warning110_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iWarning110Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_Warning110_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_Warning214_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for Warning214 on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_Warning214_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_Warning214_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_Warning214_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_Warning214_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_Warning214_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_Warning214_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_Warning214_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_Warning214_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Warning214_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_Warning214_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_Warning214_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_Warning214_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_Warning214_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_Warning214_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Warning214_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iWarning214Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_Warning214_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_WarningWarnValue100_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for WarningWarnValue100 on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_WarningWarnValue100_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_WarningWarnValue100_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_WarningWarnValue100_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_WarningWarnValue100_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_WarningWarnValue100_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_WarningWarnValue100_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_WarningWarnValue100_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_WarningWarnValue100_UnitTest is fully constructed.
		@post			CWspHeaderWriter_WarningWarnValue100_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_WarningWarnValue100_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_WarningWarnValue100_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_WarningWarnValue100_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_WarningWarnValue100_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_WarningWarnValue100_UnitTest is fully constructed.
		@post			CWspHeaderWriter_WarningWarnValue100_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iWarningWarnValue100Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_WarningWarnValue100_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for WarningWarnValue110NullAgent on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest is fully constructed.
		@post			CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest is fully constructed.
		@post			CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iWarningWarnValue110NullAgentValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_WarningWarnValue110NullAgent_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptConstrainedToken_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptConstrainedToken on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptConstrainedToken_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptConstrainedToken_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptConstrainedToken_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptConstrainedToken_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptConstrainedToken_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptConstrainedToken_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptConstrainedToken_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptConstrainedToken_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptConstrainedToken_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptConstrainedToken_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptConstrainedToken_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptConstrainedToken_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptConstrainedToken_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptConstrainedToken_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptConstrainedToken_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptConstrainedTokenValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptConstrainedToken_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptConstrainedText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptConstrainedText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptConstrainedText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptConstrainedText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptConstrainedText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptConstrainedText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptConstrainedText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptConstrainedText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptConstrainedText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptConstrainedText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptConstrainedText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptConstrainedText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptConstrainedText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptConstrainedText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptConstrainedText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptConstrainedText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptConstrainedText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptConstrainedTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptConstrainedText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptGeneralTokenQVal on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptGeneralTokenQValValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptGeneralTokenQVal_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptGeneralLongInt2Params on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptGeneralLongInt2ParamsValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptGeneralLongInt2Params_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptGeneralNullToken3Params on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptGeneralNullToken3ParamsValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptGeneralNullToken3Params_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_CookieVersion_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for CookieVersion on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_CookieVersion_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_CookieVersion_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_CookieVersion_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_CookieVersion_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_CookieVersion_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_CookieVersion_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_CookieVersion_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_CookieVersion_UnitTest is fully constructed.
		@post			CWspHeaderWriter_CookieVersion_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_CookieVersion_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_CookieVersion_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_CookieVersion_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_CookieVersion_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_CookieVersion_UnitTest is fully constructed.
		@post			CWspHeaderWriter_CookieVersion_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iCookieVersionValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_CookieVersion_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_CookieVersionNameVal_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for CookieVersionNameVal on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_CookieVersionNameVal_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_CookieVersionNameVal_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_CookieVersionNameVal_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_CookieVersionNameVal_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_CookieVersionNameVal_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_CookieVersionNameVal_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_CookieVersionNameVal_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_CookieVersionNameVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_CookieVersionNameVal_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_CookieVersionNameVal_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_CookieVersionNameVal_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_CookieVersionNameVal_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_CookieVersionNameVal_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_CookieVersionNameVal_UnitTest is fully constructed.
		@post			CWspHeaderWriter_CookieVersionNameVal_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iCookieVersionNameValValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_CookieVersionNameVal_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for CookieVersionNameNullValPath on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest is fully constructed.
		@post			CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest is fully constructed.
		@post			CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iCookieVersionNameNullValPathValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_CookieVersionNameNullValPath_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for CookieVaerionNameValNullPathDomain on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest is fully constructed.
		@post			CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest is fully constructed.
		@post			CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iCookieVaerionNameValNullPathDomainValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_CookieVaerionNameValNullPathDomain_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptMultiHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptMultiHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptMultiHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptMultiHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AcceptCharsetMultiHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAcceptCharsetMultiHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AcceptCharsetMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_RangeMultiHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for RangeMultiHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_RangeMultiHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_RangeMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_RangeMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_RangeMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_RangeMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_RangeMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_RangeMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_RangeMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RangeMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_RangeMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_RangeMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_RangeMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_RangeMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_RangeMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_RangeMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iRangeMultiHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_RangeMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TEMultiHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TEMultiHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TEMultiHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TEMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TEMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TEMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TEMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TEMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TEMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TEMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TEMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TEMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TEMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TEMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TEMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TEMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TEMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTEMultiHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TEMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_TrailerMultiHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for TrailerMultiHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_TrailerMultiHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_TrailerMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_TrailerMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_TrailerMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TrailerMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_TrailerMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_TrailerMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_TrailerMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TrailerMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_TrailerMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_TrailerMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_TrailerMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_TrailerMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_TrailerMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_TrailerMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iTrailerMultiHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_TrailerMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ViaMultiHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ViaMultiHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ViaMultiHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ViaMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ViaMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ViaMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ViaMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ViaMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ViaMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ViaMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ViaMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ViaMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ViaMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ViaMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ViaMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ViaMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ViaMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iViaMultiHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ViaMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_AllowMultiHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for AllowMultiHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_AllowMultiHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_AllowMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_AllowMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_AllowMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_AllowMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_AllowMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_AllowMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_AllowMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AllowMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_AllowMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_AllowMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_AllowMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_AllowMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_AllowMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_AllowMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iAllowMultiHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_AllowMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ContentEncodingMultiHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iContentEncodingMultiHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ContentEncodingMultiHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_UnknownHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for UnknownHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_UnknownHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_UnknownHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_UnknownHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_UnknownHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TETrailers_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_UnknownHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_UnknownHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_UnknownHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UnknownHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_UnknownHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_UnknownHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_UnknownHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_UnknownHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_UnknownHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UnknownHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iUnknownHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_UnknownHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_UnknownNullHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for UnknownNullHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_UnknownNullHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_UnknownNullHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_UnknownNullHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_UnknownNullHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TETrailers_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_UnknownNullHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_UnknownHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_UnknownNullHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UnknownNullHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_UnknownNullHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_UnknownNullHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_UnknownNullHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_UnknownNullHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_UnknownNullHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_UnknownNullHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iUnknownNullHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_UnknownNullHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_MultiUnknownHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for MultiUnknownHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_MultiUnknownHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_MultiUnknownHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_MultiUnknownHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_MultiUnknownHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_TETrailers_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_UnknownNullHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_MultiUnknownHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_MultiUnknownHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_MultiUnknownHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_MultiUnknownHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_MultiUnknownHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_MultiUnknownHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_MultiUnknownHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_MultiUnknownHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_MultiUnknownHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iMultiUnknownHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_MultiUnknownHeader_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_Profile_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for Profile on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_Profile_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_Profile_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_Profile_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_Profile_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_Profile_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_Profile_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_Profile_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_Profile_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Profile_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_Profile_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_Profile_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_Profile_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_Profile_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_Profile_UnitTest is fully constructed.
		@post			CWspHeaderWriter_Profile_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iProfileValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_Profile_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ProfileNullText_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ProfileNullText on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ProfileNullText_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ProfileNullText_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ProfileNullText_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ProfileNullText_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ProfileNullText_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ProfileNullText_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ProfileNullText_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ProfileNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ProfileNullText_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ProfileNullText_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ProfileNullText_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ProfileNullText_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ProfileNullText_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ProfileNullText_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ProfileNullText_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iProfileNullTextValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ProfileNullText_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ProfileMalformed_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ProfileMalformed on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ProfileMalformed_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ProfileMalformed_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ProfileMalformed_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ProfileMalformed_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ProfileMalformed_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ProfileMalformed_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ProfileMalformed_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ProfileMalformed_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ProfileMalformed_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ProfileMalformed_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ProfileMalformed_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ProfileMalformed_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ProfileMalformed_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ProfileMalformed_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ProfileMalformed_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iProfileMalformedValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ProfileMalformed_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ProfileDiffShort_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ProfileDiffShort on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ProfileDiffShort_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ProfileDiffShort_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ProfileDiffShort_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ProfileDiffShort_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ProfileDiffShort_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ProfileDiffShort_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ProfileDiffShort_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ProfileDiffShort_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ProfileDiffShort_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ProfileDiffShort_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ProfileDiffShort_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ProfileDiffShort_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ProfileDiffShort_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ProfileDiffShort_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ProfileDiffShort_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iProfileDiffShortValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ProfileDiffShort_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ProfileDiffLong_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ProfileDiffLong on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ProfileDiffLong_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ProfileDiffLong_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ProfileDiffLong_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ProfileDiffLong_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ProfileDiffLong_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ProfileDiffLong_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ProfileDiffLong_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ProfileDiffLong_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ProfileDiffLong_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ProfileDiffLong_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ProfileDiffLong_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ProfileDiffLong_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ProfileDiffLong_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ProfileDiffLong_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ProfileDiffLong_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iProfileDiffLongValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ProfileDiffLong_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for ProfileDiffMultiHeader on CWspHeaderWriter, the class under test.
 */
class CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest is fully constructed.
		@post			CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iProfileDiffMultiHeaderValidator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_ProfileDiffMultiHeader_UnitTest



// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for DefectFix_HOL595DR6_1 on CWspHeaderWriter, the class under test.
	
	Test Case:	This is a simple test that sets the current WSP version to 1.2 and tries to
				encode a header introduced in 1.3.
 */
class CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iDefectFix_HOL595DR6_1Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_DefectFix_HOL595DR6_1_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for DefectFix_HOL595DR6_2 on CWspHeaderWriter, the class under test.

	Test Case:	This test case uses the Content-Type header field with WSP v1.4 content-type
				value using v1.4 as the current encoding
 */
class CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iDefectFix_HOL595DR6_2Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_DefectFix_HOL595DR6_2_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for DefectFix_HOL595DR6_3 on CWspHeaderWriter, the class under test.

	Test Case:	This test case uses the Content-Type header field with WSP v1.4 content-type
				value using v1.2 as the current encoding
 */
class CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iDefectFix_HOL595DR6_3Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_DefectFix_HOL595DR6_3_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for DefectFix_HOL595DR6_4 on CWspHeaderWriter, the class under test.

	Test Case:	This test case uses the Content-Type header field with a value with a parameter
				value that has been replaced in v1.4 using v1.4
 */
class CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iDefectFix_HOL595DR6_4Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_DefectFix_HOL595DR6_4_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for DefectFix_HOL595DR6_5 on CWspHeaderWriter, the class under test.

	Test Case:	This test case uses the Content-Type header field with a value with a parameter
				value that has been replaced in v1.4 using v1.2
 */
class CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iDefectFix_HOL595DR6_5Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_DefectFix_HOL595DR6_5_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for DefectFix_HOL595DR6_6 on CWspHeaderWriter, the class under test.

	Test Case:	This test case uses the Content-Type header field with a value with a parameter
				value that has been replaced in v1.4 using v1.4
 */
class CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iDefectFix_HOL595DR6_6Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_DefectFix_HOL595DR6_6_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest
	@Depends CWspHeaderWriter & CUnitTest 

	Comments : Unit Test for DefectFix_HOL595DR6_7 on CWspHeaderWriter, the class under test.

	Test Case:	This test case uses the Content-Type header field with a value with a parameter
				value that was introduced in v1.3 using v1.2 as current
 */
class CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest : public CUnitTest
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
		@return			CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest* The constructed object.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest is fully constructed, and initialized.
	 */
	static CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest is fully destroyed.
	*/
	~CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest();

private:
	/**
		@fn				CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class.
		@pre 			None.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest is fully constructed.
	*/
	inline CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest is fully constructed.
		@post			CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CWspHeaderWriter class tested by this UnitTest's transitions.
	 */
	CWspHeaderWriter_UnitTestContext* iUTContext;
	TWspHeaderWriter_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TWspHeaderWriter_Ctor_TransitionValidator*	iCtorValidator;
	TWspHeaderWriter_EncodeHeaderL_TransitionValidator*	iDefectFix_HOL595DR6_7Validator;
	TWspHeaderWriter_Dtor_TransitionValidator*	iDtorValidator;
	};	// CWspHeaderWriter_DefectFix_HOL595DR6_7_UnitTest

#endif	// __WSPHEADERWRITERUNITTEST_H__
