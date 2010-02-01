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
// The definitions of the transition classes upon the CWspHeaderWriter class methods.
// 
//

/**
 @file WspHeaderWriterTransitionValidation.h
*/

#ifndef __WSPHEADERWRITERTRANSITIONVALIDATION_H__
#define __WSPHEADERWRITERTRANSITIONVALIDATION_H__

#include "WspHeaderWriterTransitions.h"

// ______________________________________________________________________________
//
/**
	@internal
	@class TWspHeaderWriter_Ctor_TransitionValidator
	@Depends CWspHeaderWriter & TTransitionValidator 

	Comments : Provide all the CtorUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CWspHeaderWriter test class for a transition.
 */
class TWspHeaderWriter_Ctor_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TWspHeaderWriter_Ctor_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TWspHeaderWriter_Ctor_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the WspHeaderWriter_Ctor transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TWspHeaderWriter_Ctor_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderWriter class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the WspHeaderWriter_Ctor transition behaviour.
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
		@pre 			TWspHeaderWriter_Ctor_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderWriter class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TWspHeaderWriter_Ctor_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internal
	@class TWspHeaderWriter_Dtor_TransitionValidator
	@Depends CWspHeaderWriter & TTransitionValidator 

	Comments : Provide all the DtorUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CWspHeaderWriter test class for a transition.
 */
class TWspHeaderWriter_Dtor_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TWspHeaderWriter_Dtor_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TWspHeaderWriter_Dtor_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the WspHeaderWriter_Dtor transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TWspHeaderWriter_Dtor_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderWriter class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the WspHeaderWriter_Dtor transition behaviour.
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
		@pre 			TWspHeaderWriter_Dtor_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderWriter class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TWspHeaderWriter_Dtor_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internal
	@class TWspHeaderWriter_EncodeHeaderL_TransitionValidator
	@Depends CWspHeaderWriter & TTransitionValidator 

	Comments : Provide all the EncodeHeaderLUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CWspHeaderWriter test class for a transition.
 */
class TWspHeaderWriter_EncodeHeaderL_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TWspHeaderWriter_EncodeHeaderL_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TWspHeaderWriter_EncodeHeaderL_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the WspHeaderWriter_EncodeHeaderL transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TWspHeaderWriter_EncodeHeaderL_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderWriter class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the WspHeaderWriter_EncodeHeaderL transition behaviour.
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
		@pre 			TWspHeaderWriter_EncodeHeaderL_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderWriter class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TWspHeaderWriter_EncodeHeaderL_TransitionValidator

#include "WspHeaderWriterTransitionValidation.inl"

#endif // __WSPHEADERWRITERTRANSITIONVALIDATION_H__

