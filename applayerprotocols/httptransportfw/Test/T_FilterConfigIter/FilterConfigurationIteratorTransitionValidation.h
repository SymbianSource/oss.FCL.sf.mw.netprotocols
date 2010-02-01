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
 @file FilterConfigurationIteratorTransitionValidation.h
*/

#ifndef __FILTERCONFIGURATIONITERATORTRANSITIONVALIDATION_H__
#define __FILTERCONFIGURATIONITERATORTRANSITIONVALIDATION_H__

#include "FilterConfigurationIteratorTransitions.h"

// ______________________________________________________________________________
//
/**
	@internal
	@class TFilterConfigurationIterator_Ctor_TransitionValidator
	@Depends TFilterConfigurationIterator & TTransitionValidator 

	Comments : Provide all the CtorUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the TFilterConfigurationIterator test class for a transition.
 */
class TFilterConfigurationIterator_Ctor_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TFilterConfigurationIterator_Ctor_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TFilterConfigurationIterator_Ctor_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the FilterConfigurationIterator_Ctor transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_Ctor_TransitionValidator is fully constructed.
		@post			No change to the TFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the FilterConfigurationIterator_Ctor transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			7.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_Ctor_TransitionValidator is fully constructed.
		@post			No change to the TFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TFilterConfigurationIterator_Ctor_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internal
	@class TFilterConfigurationIterator_Dtor_TransitionValidator
	@Depends TFilterConfigurationIterator & TTransitionValidator 

	Comments : Provide all the DtorUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the TFilterConfigurationIterator test class for a transition.
 */
class TFilterConfigurationIterator_Dtor_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TFilterConfigurationIterator_Dtor_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TFilterConfigurationIterator_Dtor_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the FilterConfigurationIterator_Dtor transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_Dtor_TransitionValidator is fully constructed.
		@post			No change to the TFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the FilterConfigurationIterator_Dtor transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			7.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_Dtor_TransitionValidator is fully constructed.
		@post			No change to the TFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TFilterConfigurationIterator_Dtor_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internal
	@class TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator
	@Depends CFilterConfigurationIterator & TTransitionValidator 

	Comments : Provide all the SuccessfulFirstUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CFilterConfigurationIterator test class for a transition.
 */
class TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the FilterConfigurationIterator_SuccessfulFirst transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator is fully constructed.
		@post			No change to the CFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the FilterConfigurationIterator_SuccessfulFirst transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			7.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator is fully constructed.
		@post			No change to the CFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internal
	@class TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator
	@Depends CFilterConfigurationIterator & TTransitionValidator 

	Comments : Provide all the SuccessfulAtStartUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CFilterConfigurationIterator test class for a transition.
 */
class TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the FilterConfigurationIterator_SuccessfulAtStart transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator is fully constructed.
		@post			No change to the CFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the FilterConfigurationIterator_SuccessfulAtStart transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			7.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator is fully constructed.
		@post			No change to the CFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internal
	@class TFilterConfigurationIterator_SuccessfulNext_TransitionValidator
	@Depends CFilterConfigurationIterator & TTransitionValidator 

	Comments : Provide all the SuccessfulNextUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CFilterConfigurationIterator test class for a transition.
 */
class TFilterConfigurationIterator_SuccessfulNext_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TFilterConfigurationIterator_SuccessfulNext_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TFilterConfigurationIterator_SuccessfulNext_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the FilterConfigurationIterator_SuccessfulNext transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_SuccessfulNext_TransitionValidator is fully constructed.
		@post			No change to the CFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the FilterConfigurationIterator_SuccessfulNext transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			7.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_SuccessfulNext_TransitionValidator is fully constructed.
		@post			No change to the CFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TFilterConfigurationIterator_SuccessfulNext_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internal
	@class TFilterConfigurationIterator_FindByDataType_TransitionValidator
	@Depends CFilterConfigurationIterator & TTransitionValidator 

	Comments : Provide all the FindByDataTypeUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CFilterConfigurationIterator test class for a transition.
 */
class TFilterConfigurationIterator_FindByDataType_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TFilterConfigurationIterator_FindByDataType_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TFilterConfigurationIterator_FindByDataType_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the FilterConfigurationIterator_FindByDataType transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_FindByDataType_TransitionValidator is fully constructed.
		@post			No change to the CFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the FilterConfigurationIterator_FindByDataType transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			7.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_FindByDataType_TransitionValidator is fully constructed.
		@post			No change to the CFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TFilterConfigurationIterator_FindByDataType_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internal
	@class TFilterConfigurationIterator_SessionListProtocols_TransitionValidator
	@Depends TFilterConfigurationIterator & TTransitionValidator 

	Comments : Provide all the SessionListProtocolsUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the TFilterConfigurationIterator test class for a transition.
 */
class TFilterConfigurationIterator_SessionListProtocols_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TFilterConfigurationIterator_SessionListProtocols_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TFilterConfigurationIterator_SessionListProtocols_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the FilterConfigurationIterator_SessionListProtocols transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_SessionListProtocols_TransitionValidator is fully constructed.
		@post			No change to the TFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the FilterConfigurationIterator_SessionListProtocols transition behaviour.
						When overriding, if the transition calls an asynchronous function
						ValidatePostConditions will be called twice. Firstly, after the
						asynchronous function has been called and, secondly, after the 
						asynchronous request has completed. The parameter aAsyncState can
						be used to distinguish between these two cases.
		Error Condition	: Invalid post-conditions.
		@since			7.0
		@param			aAsyncState EAsyncCalled if the async function has been just been called,
						EAsyncCompleted if the function has completed.
		@return			ETrue if the post-conditions were valid, EFalse otherwise.
		@pre 			TFilterConfigurationIterator_SessionListProtocols_TransitionValidator is fully constructed.
		@post			No change to the TFilterConfigurationIterator class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TFilterConfigurationIterator_SessionListProtocols_TransitionValidator

#include "FilterConfigurationIteratorTransitionValidation.inl"

#endif // __FILTERCONFIGURATIONITERATORTRANSITIONVALIDATION_H__

