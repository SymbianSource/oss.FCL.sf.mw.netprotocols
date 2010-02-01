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
// The definitions of the transition classes upon the TFilterConfigurationIterator class methods.
// 
//

/**
 @file FilterConfigurationIteratorTransitions.h
*/

#ifndef __FILTERCONFIGURATIONITERATORTRANSITIONS_H__
#define __FILTERCONFIGURATIONITERATORTRANSITIONS_H__

#include "TestUtilities.h"
#include "StateAccessor.h"
#include "LeakTestTransition.h"
#include "DataLogger.h"
#include <http/tfilterconfigurationiter.h>
#include "FilterConfigurationIteratorStateAccessor.h"
#include "UnitTest.h"

// System includes
#include <http/rhttpsession.h>
#include <http/mhttpfiltercreationcallback.h>
#include <http/tfilterinformation.h>

// Literals
_LIT8(KTxtNoExist, "HTTP/NOTEXIST");
_LIT8(KTxtHttpImplicit, "HTTP/IMPLICIT");

#define CTransitionType CTransition

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_UnitTestContext
	@Depends TFilterConfigurationIterator & CUnitTestContext 

	Comments : Provide all the TFilterConfigurationIterator specific
	parameters and behaviour on the TFilterConfigurationIterator
	test class for a transition.
 */
class CFilterConfigurationIterator_UnitTestContext : public CUnitTestContext,
													 public MHTTPFilterCreationCallback
	{
public:
	/**
		@fn				CFilterConfigurationIterator_UnitTestContext(CDataLogger& aDataLogger,
														MStateAccessor& aStateAccessor,
														MTransitionObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest's Transitions.
		@param			aStateAccessor WhiteBox state access to the TFilterConfigurationIterator class under test.
		@pre 			None.
		@post			CFilterConfigurationIterator_UnitTestContext is fully constructed, and initialized.
	*/
	inline CFilterConfigurationIterator_UnitTestContext(CDataLogger& aDataLogger,
										MStateAccessor& aStateAccessor,
										MTransitionObserver& aObserver);

	/**
		@fn				~CFilterConfigurationIterator_UnitTestContext()
		Intended Usage	: Default Destructor
		@since			7.0
		@pre 			CFilterConfigurationIterator_UnitTestContext is fully constructed.
		@post			CFilterConfigurationIterator_UnitTestContext is fully destroyed
		*/
		virtual inline ~CFilterConfigurationIterator_UnitTestContext();

	inline void CreateFilterIterL();

	inline void DeleteFilterIter();

	inline void ConfigureSessionFiltersL(TFilterConfigurationIterator* aFilterConfigIter);

	enum TTestDataType
		{
		ENotExist,
		EHttpImplicit
		};

	inline const TDesC8& GetTestFilterName(TTestDataType aTestFilterName);

	/** The instance of the class under test */
	TFilterConfigurationIterator* iFilterConfigurationIterator;

	// Transport session
	RHTTPSession iTFSession;

	// Stores return value of First() method
	TInt iFirstReturn;

	// Stores return value of AtStart() method
	TBool iAtStartReturn;
	// Stores the expected return value of AtStart()
	TBool iAtStartExpected;

	// Stores return value of Next() method
	TInt iNextReturn;

	// Stores the filter information returned by the iterator
	TFilterInformation* iFilterInfo;

	// Stores the return value of FindByDataType() method
	TInt iFindByDataTypeReturn;
	// Stores the expected return value of FindByDataType() method
	TInt iFindByDataTypeExpected;

	// Descriptor pointer array for protocol list
	RPointerArray<HBufC8> iProtocolArray;
	TInt iProtocolCount;

	}; // CFilterConfigurationIterator_UnitTestContext

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_NewL_Transition
	@Depends TFilterConfigurationIterator & CTransition 

	Comments : Transition test of the TFilterConfigurationIterator::NewL method.
 */
class CFilterConfigurationIterator_NewL_Transition : public CTransitionType
	{
public:
	/**
		@fn				CFilterConfigurationIterator_NewL_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CFilterConfigurationIterator_NewL_Transition is fully constructed.
	*/
	CFilterConfigurationIterator_NewL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the TFilterConfigurationIterator::NewL method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see TFilterConfigurationIterator::NewL)
		@since			7.0
		@return			None
		@pre 			CFilterConfigurationIterator_NewL_Transition is fully constructed.
		@post			No change in the CFilterConfigurationIterator_NewL_Transition apart
						from iFilterConfigurationIterator, which may have changed state.
						(@see TFilterConfigurationIterator::NewL post-condition) for iFilterConfigurationIterator's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CFilterConfigurationIterator_UnitTestContext
		@pre 			iUTContext is a valid CFilterConfigurationIterator_UnitTestContext.
		@post			No change in the CFilterConfigurationIterator_NewL_Transition
	*/
	inline CFilterConfigurationIterator_UnitTestContext& Context() const;
	};	// CFilterConfigurationIterator_NewL_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_Dtor_Transition
	@Depends TFilterConfigurationIterator & CTransition 

	Comments : Transition test of the TFilterConfigurationIterator::Dtor method.
 */
class CFilterConfigurationIterator_Dtor_Transition : public CTransitionType
	{
public:
	/**
		@fn				CFilterConfigurationIterator_Dtor_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CFilterConfigurationIterator_Dtor_Transition is fully constructed.
	*/
	CFilterConfigurationIterator_Dtor_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the TFilterConfigurationIterator::Dtor method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see TFilterConfigurationIterator::Dtor)
		@since			7.0
		@return			None
		@pre 			CFilterConfigurationIterator_Dtor_Transition is fully constructed.
		@post			No change in the CFilterConfigurationIterator_Dtor_Transition apart
						from iFilterConfigurationIterator, which may have changed state.
						(@see TFilterConfigurationIterator::Dtor post-condition) for iFilterConfigurationIterator's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CFilterConfigurationIterator_UnitTestContext
		@pre 			iUTContext is a valid CFilterConfigurationIterator_UnitTestContext.
		@post			No change in the CFilterConfigurationIterator_Dtor_Transition
	*/
	inline CFilterConfigurationIterator_UnitTestContext& Context() const;
	};	// CFilterConfigurationIterator_Dtor_Transition


// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_First_Transition
	@Depends CFilterConfigurationIterator & CTransition 

	Comments : Transition test of the CFilterConfigurationIterator::First method.
 */
class CFilterConfigurationIterator_First_Transition : public CTransitionType
	{
public:
	/**
		@fn				CFilterConfigurationIterator_First_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CFilterConfigurationIterator_First_Transition is fully constructed.
	*/
	CFilterConfigurationIterator_First_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CFilterConfigurationIterator::First method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CFilterConfigurationIterator::First)
		@since			7.0
		@return			None
		@pre 			CFilterConfigurationIterator_First_Transition is fully constructed.
		@post			No change in the CFilterConfigurationIterator_First_Transition apart
						from iFilterConfigurationIterator, which may have changed state.
						(@see CFilterConfigurationIterator::First post-condition) for iFilterConfigurationIterator's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CFilterConfigurationIterator_UnitTestContext
		@pre 			iUTContext is a valid CFilterConfigurationIterator_UnitTestContext.
		@post			No change in the CFilterConfigurationIterator_First_Transition
	*/
	inline CFilterConfigurationIterator_UnitTestContext& Context() const;
	};	// CFilterConfigurationIterator_First_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_AtStart_Transition
	@Depends CFilterConfigurationIterator & CTransition 

	Comments : Transition test of the CFilterConfigurationIterator::AtStart method.
 */
class CFilterConfigurationIterator_AtStart_Transition : public CTransitionType
	{
public:
	/**
		@fn				CFilterConfigurationIterator_AtStart_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CFilterConfigurationIterator_AtStart_Transition is fully constructed.
	*/
	CFilterConfigurationIterator_AtStart_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CFilterConfigurationIterator::AtStart method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CFilterConfigurationIterator::AtStart)
		@since			7.0
		@return			None
		@pre 			CFilterConfigurationIterator_AtStart_Transition is fully constructed.
		@post			No change in the CFilterConfigurationIterator_AtStart_Transition apart
						from iFilterConfigurationIterator, which may have changed state.
						(@see CFilterConfigurationIterator::AtStart post-condition) for iFilterConfigurationIterator's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CFilterConfigurationIterator_UnitTestContext
		@pre 			iUTContext is a valid CFilterConfigurationIterator_UnitTestContext.
		@post			No change in the CFilterConfigurationIterator_AtStart_Transition
	*/
	inline CFilterConfigurationIterator_UnitTestContext& Context() const;
	};	// CFilterConfigurationIterator_AtStart_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_Next_Transition
	@Depends CFilterConfigurationIterator & CTransition 

	Comments : Transition test of the CFilterConfigurationIterator::Next method.
 */
class CFilterConfigurationIterator_Next_Transition : public CTransitionType
	{
public:
	/**
		@fn				CFilterConfigurationIterator_Next_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CFilterConfigurationIterator_Next_Transition is fully constructed.
	*/
	CFilterConfigurationIterator_Next_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CFilterConfigurationIterator::Next method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CFilterConfigurationIterator::Next)
		@since			7.0
		@return			None
		@pre 			CFilterConfigurationIterator_Next_Transition is fully constructed.
		@post			No change in the CFilterConfigurationIterator_Next_Transition apart
						from iFilterConfigurationIterator, which may have changed state.
						(@see CFilterConfigurationIterator::Next post-condition) for iFilterConfigurationIterator's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CFilterConfigurationIterator_UnitTestContext
		@pre 			iUTContext is a valid CFilterConfigurationIterator_UnitTestContext.
		@post			No change in the CFilterConfigurationIterator_Next_Transition
	*/
	inline CFilterConfigurationIterator_UnitTestContext& Context() const;
	};	// CFilterConfigurationIterator_Next_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_CurrentFilterInformation_Transition
	@Depends CFilterConfigurationIterator & CTransition 

	Comments : Transition test of the CFilterConfigurationIterator::CurrentFilterInformation method.
 */
class CFilterConfigurationIterator_CurrentFilterInformation_Transition : public CTransitionType
	{
public:
	/**
		@fn				CFilterConfigurationIterator_CurrentFilterInformation_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CFilterConfigurationIterator_CurrentFilterInformation_Transition is fully constructed.
	*/
	CFilterConfigurationIterator_CurrentFilterInformation_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CFilterConfigurationIterator::CurrentFilterInformation method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CFilterConfigurationIterator::CurrentFilterInformation)
		@since			7.0
		@return			None
		@pre 			CFilterConfigurationIterator_CurrentFilterInformation_Transition is fully constructed.
		@post			No change in the CFilterConfigurationIterator_CurrentFilterInformation_Transition apart
						from iFilterConfigurationIterator, which may have changed state.
						(@see CFilterConfigurationIterator::CurrentFilterInformation post-condition) for iFilterConfigurationIterator's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CFilterConfigurationIterator_UnitTestContext
		@pre 			iUTContext is a valid CFilterConfigurationIterator_UnitTestContext.
		@post			No change in the CFilterConfigurationIterator_CurrentFilterInformation_Transition
	*/
	inline CFilterConfigurationIterator_UnitTestContext& Context() const;
	};	// CFilterConfigurationIterator_CurrentFilterInformation_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_FindByDataType_Transition
	@Depends CFilterConfigurationIterator & CTransition 

	Comments : Transition test of the CFilterConfigurationIterator::FindByDataType method.
 */
class CFilterConfigurationIterator_FindByDataType_Transition : public CTransitionType
	{
public:
	/**
		@fn				CFilterConfigurationIterator_FindByDataType_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CFilterConfigurationIterator_FindByDataType_Transition is fully constructed.
	*/
	CFilterConfigurationIterator_FindByDataType_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CFilterConfigurationIterator::FindByDataType method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CFilterConfigurationIterator::FindByDataType)
		@since			7.0
		@return			None
		@pre 			CFilterConfigurationIterator_FindByDataType_Transition is fully constructed.
		@post			No change in the CFilterConfigurationIterator_FindByDataType_Transition apart
						from iFilterConfigurationIterator, which may have changed state.
						(@see CFilterConfigurationIterator::FindByDataType post-condition) for iFilterConfigurationIterator's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CFilterConfigurationIterator_UnitTestContext
		@pre 			iUTContext is a valid CFilterConfigurationIterator_UnitTestContext.
		@post			No change in the CFilterConfigurationIterator_FindByDataType_Transition
	*/
	inline CFilterConfigurationIterator_UnitTestContext& Context() const;
	};	// CFilterConfigurationIterator_FindByDataType_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CFilterConfigurationIterator_ListProtocols_Transition
	@Depends CFilterConfigurationIterator & CTransition 

	Comments : Transition test of the CFilterConfigurationIterator::ListProtocols method.
 */
class CFilterConfigurationIterator_ListProtocols_Transition : public CTransition
	{
public:
	/**
		@fn				CFilterConfigurationIterator_ListProtocols_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CFilterConfigurationIterator_ListProtocols_Transition is fully constructed.
	*/
	CFilterConfigurationIterator_ListProtocols_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CFilterConfigurationIterator::ListProtocols method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CFilterConfigurationIterator::ListProtocols)
		@since			7.0
		@return			None
		@pre 			CFilterConfigurationIterator_ListProtocols_Transition is fully constructed.
		@post			No change in the CFilterConfigurationIterator_ListProtocols_Transition apart
						from iFilterConfigurationIterator, which may have changed state.
						(@see CFilterConfigurationIterator::ListProtocols post-condition) for iFilterConfigurationIterator's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CFilterConfigurationIterator_UnitTestContext
		@pre 			iUTContext is a valid CFilterConfigurationIterator_UnitTestContext.
		@post			No change in the CFilterConfigurationIterator_ListProtocols_Transition
	*/
	inline CFilterConfigurationIterator_UnitTestContext& Context() const;
	};	// CFilterConfigurationIterator_ListProtocols_Transition

#include "FilterConfigurationIteratorTransitions.inl"

#include "FilterConfigurationIteratorUnitTestContext.inl"

#endif // __FILTERCONFIGURATIONITERATORTRANSITIONS_H__

