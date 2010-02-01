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
// The unit test class definitions for the TFilterConfigurationIterator class.
// 
//

/**
 @file FilterConfigurationIteratorUnitTest.h
*/
#ifndef __FILTERCONFIGURATIONITERATORUNITTEST_H__
#define __FILTERCONFIGURATIONITERATORUNITTEST_H__

#include "TestBedDefinitions.h"
#include "FilterConfigurationIteratorStateAccessor.h"
#include "FilterConfigurationIteratorTransitionValidation.h"
#include "FilterConfigurationIteratorTransitions.h"

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_CreateAndDestroy_UnitTest
	@Depends TFilterConfigurationIterator & CUnitTest 

	Comments : Unit Test for CreateAndDestroy on TFilterConfigurationIterator, the class under test.
 */
class CFilterConfigurationIterator_CreateAndDestroy_UnitTest : public CUnitTest
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
		@return			CFilterConfigurationIterator_CreateAndDestroy_UnitTest* The constructed object.
		@pre 			None.
		@post			CFilterConfigurationIterator_CreateAndDestroy_UnitTest is fully constructed, and initialized.
	 */
	static CFilterConfigurationIterator_CreateAndDestroy_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CFilterConfigurationIterator_CreateAndDestroy_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CFilterConfigurationIterator_CreateAndDestroy_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CFilterConfigurationIterator_CreateAndDestroy_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CFilterConfigurationIterator_CreateAndDestroy_UnitTest is fully constructed.
		@post			CFilterConfigurationIterator_CreateAndDestroy_UnitTest is fully destroyed.
	*/
	~CFilterConfigurationIterator_CreateAndDestroy_UnitTest();

private:
	/**
		@fn				CFilterConfigurationIterator_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the TFilterConfigurationIterator class.
		@pre 			None.
		@post			CFilterConfigurationIterator_CreateAndDestroy_UnitTest is fully constructed.
	*/
	inline CFilterConfigurationIterator_CreateAndDestroy_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CFilterConfigurationIterator_CreateAndDestroy_UnitTest is fully constructed.
		@post			CFilterConfigurationIterator_CreateAndDestroy_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The TFilterConfigurationIterator class tested by this UnitTest's transitions.
	 */
	CFilterConfigurationIterator_UnitTestContext* iUTContext;
	TFilterConfigurationIterator_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TFilterConfigurationIterator_Ctor_TransitionValidator*	iCtorValidator;
	// TFilterConfigurationIterator_CreateAndDestroy_TransitionValidator*	iCreateAndDestroyValidator;
	TFilterConfigurationIterator_Dtor_TransitionValidator*	iDtorValidator;
	};	// CFilterConfigurationIterator_CreateAndDestroy_UnitTest


// ______________________________________________________________________________
//

/**
	@internal
	@class CFilterConfigurationIterator_SuccessfulAtStart_UnitTest
	@Depends CFilterConfigurationIterator & CUnitTest 

	Comments : Unit Test for SuccessfulAtStart on CFilterConfigurationIterator, the class under test.
 */
class CFilterConfigurationIterator_SuccessfulAtStart_UnitTest : public CUnitTest
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
		@return			CFilterConfigurationIterator_SuccessfulAtStart_UnitTest* The constructed object.
		@pre 			None.
		@post			CFilterConfigurationIterator_SuccessfulAtStart_UnitTest is fully constructed, and initialized.
	 */
	static CFilterConfigurationIterator_SuccessfulAtStart_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CFilterConfigurationIterator_SuccessfulAtStart_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CFilterConfigurationIterator_SuccessfulAtStart_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CFilterConfigurationIterator_SuccessfulAtStart_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CFilterConfigurationIterator_SuccessfulAtStart_UnitTest is fully constructed.
		@post			CFilterConfigurationIterator_SuccessfulAtStart_UnitTest is fully destroyed.
	*/
	~CFilterConfigurationIterator_SuccessfulAtStart_UnitTest();

private:
	/**
		@fn				CFilterConfigurationIterator_SuccessfulAtStart_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CFilterConfigurationIterator class.
		@pre 			None.
		@post			CFilterConfigurationIterator_SuccessfulAtStart_UnitTest is fully constructed.
	*/
	inline CFilterConfigurationIterator_SuccessfulAtStart_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CFilterConfigurationIterator_SuccessfulAtStart_UnitTest is fully constructed.
		@post			CFilterConfigurationIterator_SuccessfulAtStart_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CFilterConfigurationIterator class tested by this UnitTest's transitions.
	 */
	CFilterConfigurationIterator_UnitTestContext* iUTContext;
	TFilterConfigurationIterator_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TFilterConfigurationIterator_Ctor_TransitionValidator*	iCtorValidator;
	TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator* iSuccessfulFirstValidator;
	TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator*	iSuccessfulAtStartValidator;
	TFilterConfigurationIterator_Dtor_TransitionValidator*	iDtorValidator;
	};	// CFilterConfigurationIterator_SuccessfulAtStart_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest
	@Depends CFilterConfigurationIterator & CUnitTest 

	Comments : Unit Test for UnsuccessfulAtStart on CFilterConfigurationIterator, the class under test.
 */
class CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest : public CUnitTest
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
		@return			CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest* The constructed object.
		@pre 			None.
		@post			CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest is fully constructed, and initialized.
	 */
	static CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest is fully constructed.
		@post			CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest is fully destroyed.
	*/
	~CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest();

private:
	/**
		@fn				CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CFilterConfigurationIterator class.
		@pre 			None.
		@post			CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest is fully constructed.
	*/
	inline CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest is fully constructed.
		@post			CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CFilterConfigurationIterator class tested by this UnitTest's transitions.
	 */
	CFilterConfigurationIterator_UnitTestContext* iUTContext;
	TFilterConfigurationIterator_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TFilterConfigurationIterator_Ctor_TransitionValidator*	iCtorValidator;
	TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator* iSuccessfulFirstValidator;
	TFilterConfigurationIterator_SuccessfulNext_TransitionValidator*	iSuccessfulNextValidator;
	TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator*	iAtStartValidator;
	TFilterConfigurationIterator_Dtor_TransitionValidator*	iDtorValidator;
	};	// CFilterConfigurationIterator_UnsuccessfulAtStart_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest
	@Depends CFilterConfigurationIterator & CUnitTest 

	Comments : Unit Test for UnsuccessfulFindByDataType on CFilterConfigurationIterator, the class under test.
 */
class CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest : public CUnitTest
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
		@return			CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest* The constructed object.
		@pre 			None.
		@post			CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest is fully constructed, and initialized.
	 */
	static CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest is fully constructed.
		@post			CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest is fully destroyed.
	*/
	~CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest();

private:
	/**
		@fn				CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CFilterConfigurationIterator class.
		@pre 			None.
		@post			CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest is fully constructed.
	*/
	inline CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest is fully constructed.
		@post			CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CFilterConfigurationIterator class tested by this UnitTest's transitions.
	 */
	CFilterConfigurationIterator_UnitTestContext* iUTContext;
	TFilterConfigurationIterator_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TFilterConfigurationIterator_Ctor_TransitionValidator*	iCtorValidator;
	TFilterConfigurationIterator_FindByDataType_TransitionValidator*	iFindByDataTypeValidator;
	TFilterConfigurationIterator_Dtor_TransitionValidator*	iDtorValidator;
	};	// CFilterConfigurationIterator_UnsuccessfulFindByDataType_UnitTest

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest
	@Depends CFilterConfigurationIterator & CUnitTest 

	Comments : Unit Test for ProtocolList_DEFECT_EXT5BEGTY on CFilterConfigurationIterator, the class under test.
 */
class CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest : public CUnitTest
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
		@return			CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest* The constructed object.
		@pre 			None.
		@post			CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest is fully constructed, and initialized.
	 */
	static CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest* NewL(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				RunError(TInt aError)
		Intended Usage	: Intercept the panic caused by a RunL leave,
						to restore the CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest
						object to a sensible state.
						(called by the Active Scheduler immediately before the Panic).
		Error Condition	: @see CUnitTest::RunError().
		@since			7.0
		@return			KErrNone if cleanup successful, otherwise
						@see CUnitTest::RunError()
		@pre 			CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest is fully constructed, and initialized.
		@post			The object has been restored to a sensible state.
	 */
	inline TInt RunError(TInt aError);

	/**
		@fn				~CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest()
		Intended Usage	: Standard Destructor.
		Error Condition	: None.	
		@since			7.0
		@pre 			CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest is fully constructed.
		@post			CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest is fully destroyed.
	*/
	~CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest();

private:
	/**
		@fn				CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest(CDataLogger& aDataLogger,
														MUnitTestObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest.
		@param			aStateAccessor WhiteBox state access to the CFilterConfigurationIterator class.
		@pre 			None.
		@post			CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest is fully constructed.
	*/
	inline CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest(CDataLogger& aDataLogger,
											MUnitTestObserver& aObserver);

	/**
		@fn				void ConstructL()
		Intended Usage	: Second phase of safe two phase construction, 
		to complete the object initialisation.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, and @see CUnitTest::BaseNewL().
		@since			7.0
		@return			None 
		@pre 			CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest is fully constructed.
		@post			CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest is fully initialized.
	*/
	void ConstructL();

	/**
	The context of the Unit Test.
	i.e The CFilterConfigurationIterator class tested by this UnitTest's transitions.
	 */
	CFilterConfigurationIterator_UnitTestContext* iUTContext;
	TFilterConfigurationIterator_StateAccessor*	iStateAccessor;
	/* C'tor, d'tor, and method transition validators */
	TFilterConfigurationIterator_Ctor_TransitionValidator*	iCtorValidator;
	TFilterConfigurationIterator_SessionListProtocols_TransitionValidator*	iProtocolList_DEFECT_EXT5BEGTYValidator;
	TFilterConfigurationIterator_Dtor_TransitionValidator*	iDtorValidator;
	};	// CFilterConfigurationIterator_ProtocolList_DEFECT_EXT5BEGTY_UnitTest

#endif	// __FILTERCONFIGURATIONITERATORUNITTEST_H__
