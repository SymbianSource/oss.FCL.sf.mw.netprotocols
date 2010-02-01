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
// The definitions of the transition classes upon the CWspHeaderReader class methods.
// 
//

/**
 @file WspHeaderReaderTransitionValidation.h
*/

#ifndef __WSPHEADERREADERTRANSITIONVALIDATION_H__
#define __WSPHEADERREADERTRANSITIONVALIDATION_H__

#include "WspHeaderReaderTransitions.h"

// ______________________________________________________________________________
//
/**
	@internal
	@class TWspHeaderReader_Ctor_TransitionValidator
	@Depends CWspHeaderReader & TTransitionValidator 

	Comments : Provide all the CtorUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CWspHeaderReader test class for a transition.
 */
class TWspHeaderReader_Ctor_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TWspHeaderReader_Ctor_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TWspHeaderReader_Ctor_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the WspHeaderReader_Ctor transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TWspHeaderReader_Ctor_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderReader class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the WspHeaderReader_Ctor transition behaviour.
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
		@pre 			TWspHeaderReader_Ctor_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderReader class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TWspHeaderReader_Ctor_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internal
	@class TWspHeaderReader_Dtor_TransitionValidator
	@Depends CWspHeaderReader & TTransitionValidator 

	Comments : Provide all the DtorUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CWspHeaderReader test class for a transition.
 */
class TWspHeaderReader_Dtor_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TWspHeaderReader_Dtor_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TWspHeaderReader_Dtor_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the WspHeaderReader_Dtor transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TWspHeaderReader_Dtor_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderReader class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the WspHeaderReader_Dtor transition behaviour.
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
		@pre 			TWspHeaderReader_Dtor_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderReader class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TWspHeaderReader_Dtor_TransitionValidator

// ______________________________________________________________________________
//
/**
	@internal
	@class TWspHeaderReader_DecodeHeaderL_TransitionValidator
	@Depends CWspHeaderReader & TTransitionValidator 

	Comments : Provide all the DecodeHeaderLUnit Test's specific
	validatation for the state of a transition before and after its execution.
	on the CWspHeaderReader test class for a transition.
 */
class TWspHeaderReader_DecodeHeaderL_TransitionValidator : public TTransitionValidator
	{
public:
	/**
		@fn				TWspHeaderReader_DecodeHeaderL_TransitionValidator(CUnitTestContext& aUTContext)
		Intended Usage	: Standard constructor
		@exception		KErrNoMemory
		@since			7.0
		@param			aUTContext The context within which this transition is executing
	 */
	inline TWspHeaderReader_DecodeHeaderL_TransitionValidator(CUnitTestContext& aUTContext);

	/**
		@fn				ValidatePreConditions()
		Intended Usage	: Checks the start state
						of the WspHeaderReader_DecodeHeaderL transition behaviour.
		Error Condition	: Invalid pre-conditions
		@since			7.0
		@return			ETrue if the pre-conditions were valid, EFalse otherwise.
		@pre 			TWspHeaderReader_DecodeHeaderL_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderReader class.
	*/
	virtual inline TBool ValidatePreConditions();

	/**
		@fn				ValidatePostConditions(TTestBedAsyncState aAsyncState)
		Intended Usage	:	Checks the start state
						of the WspHeaderReader_DecodeHeaderL transition behaviour.
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
		@pre 			TWspHeaderReader_DecodeHeaderL_TransitionValidator is fully constructed.
		@post			No change to the CWspHeaderReader class.
	*/
	virtual inline TBool ValidatePostConditions(TTestBedAsyncState aAsyncState);
	};	// TWspHeaderReader_DecodeHeaderL_TransitionValidator

#include "WspHeaderReaderTransitionValidation.inl"

#endif // __WSPHEADERREADERTRANSITIONVALIDATION_H__

