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
 @file WspHeaderReaderTransitions.h
*/

#ifndef __WSPHEADERREADERTRANSITIONS_H__
#define __WSPHEADERREADERTRANSITIONS_H__

#include "TestUtilities.h"
#include "StateAccessor.h"
#include "LeakTestTransition.h"
#include "DataLogger.h"
#include "CWspHeaderReader.h"
#include "WspHeaderReaderStateAccessor.h"
#include "UnitTest.h"

// Forward declarations
class CHeaders;
class CHeaderField;
class RHeaderField;

#define CTransitionType CTransition

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_UnitTestContext
	@Depends CWspHeaderReader & CUnitTestContext 

	Comments : Provide all the CWspHeaderReader specific
	parameters and behaviour on the CWspHeaderReader
	test class for a transition.
 */
class CWspHeaderReader_UnitTestContext : public CUnitTestContext
	{
public:
	/**
		@fn				CWspHeaderReader_UnitTestContext(CDataLogger& aDataLogger,
														MStateAccessor& aStateAccessor,
														MTransitionObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest's Transitions.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderReader class under test.
		@pre 			None.
		@post			CWspHeaderReader_UnitTestContext is fully constructed, and initialized.
	*/
	inline CWspHeaderReader_UnitTestContext(CDataLogger& aDataLogger,
										MStateAccessor& aStateAccessor,
										MTransitionObserver& aObserver,
										const TStringTable& aStrTable);

	/**
		@fn				~CWspHeaderReader_UnitTestContext()
		Intended Usage	: Default Destructor
		@since			7.0
		@pre 			CWspHeaderReader_UnitTestContext is fully constructed.
		@post			CWspHeaderReader_UnitTestContext is fully destroyed
	*/
	virtual inline ~CWspHeaderReader_UnitTestContext();

	inline void ConstructL(TInt aHeaderName);

	TInt iHeaderToken;

	inline void SetOutputL();

	inline void SetTrapOutputL();

	inline void SetExpectedL(const TDesC8& aExpectedData);

	CHeaderCodec* iCodec;

	RStringPool iStrPool;

	const TStringTable& iStrTable;

	CHeaders* iHeader;

	CHeaderField* iHeaderField;

	RHeaderField* iTestHeader;

	HBufC8* iExpectedData;

	HBufC8* iOutputData;
	}; // CWspHeaderReader_UnitTestContext

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_NewL_Transition
	@Depends CWspHeaderReader & CTransition 

	Comments : Transition test of the CWspHeaderReader::NewL method.
 */
class CWspHeaderReader_NewL_Transition : public CTransitionType
	{
public:
	/**
		@fn				CWspHeaderReader_NewL_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CWspHeaderReader_NewL_Transition is fully constructed.
	*/
	CWspHeaderReader_NewL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CWspHeaderReader::NewL method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CWspHeaderReader::NewL)
		@since			7.0
		@return			None
		@pre 			CWspHeaderReader_NewL_Transition is fully constructed.
		@post			No change in the CWspHeaderReader_NewL_Transition apart
						from iWspHeaderReader, which may have changed state.
						(@see CWspHeaderReader::NewL post-condition) for iWspHeaderReader's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CWspHeaderReader_UnitTestContext
		@pre 			iUTContext is a valid CWspHeaderReader_UnitTestContext.
		@post			No change in the CWspHeaderReader_NewL_Transition
	*/
	inline CWspHeaderReader_UnitTestContext& Context() const;
	};	// CWspHeaderReader_NewL_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_Dtor_Transition
	@Depends CWspHeaderReader & CTransition 

	Comments : Transition test of the CWspHeaderReader::Dtor method.
 */
class CWspHeaderReader_Dtor_Transition : public CTransitionType
	{
public:
	/**
		@fn				CWspHeaderReader_Dtor_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CWspHeaderReader_Dtor_Transition is fully constructed.
	*/
	CWspHeaderReader_Dtor_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CWspHeaderReader::Dtor method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CWspHeaderReader::Dtor)
		@since			7.0
		@return			None
		@pre 			CWspHeaderReader_Dtor_Transition is fully constructed.
		@post			No change in the CWspHeaderReader_Dtor_Transition apart
						from iWspHeaderReader, which may have changed state.
						(@see CWspHeaderReader::Dtor post-condition) for iWspHeaderReader's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CWspHeaderReader_UnitTestContext
		@pre 			iUTContext is a valid CWspHeaderReader_UnitTestContext.
		@post			No change in the CWspHeaderReader_Dtor_Transition
	*/
	inline CWspHeaderReader_UnitTestContext& Context() const;
	};	// CWspHeaderReader_Dtor_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_DecodeHeaderL_Transition
	@Depends CWspHeaderReader & CTransition 

	Comments : Transition test of the CWspHeaderReader::DecodeHeaderL method.
 */
class CWspHeaderReader_DecodeHeaderL_Transition : public CTransitionType
	{
public:
	/**
		@fn				CWspHeaderReader_DecodeHeaderL_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CWspHeaderReader_DecodeHeaderL_Transition is fully constructed.
	*/
	CWspHeaderReader_DecodeHeaderL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CWspHeaderReader::DecodeHeaderL method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CWspHeaderReader::DecodeHeaderL)
		@since			7.0
		@return			None
		@pre 			CWspHeaderReader_DecodeHeaderL_Transition is fully constructed.
		@post			No change in the CWspHeaderReader_DecodeHeaderL_Transition apart
						from iWspHeaderReader, which may have changed state.
						(@see CWspHeaderReader::DecodeHeaderL post-condition) for iWspHeaderReader's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CWspHeaderReader_UnitTestContext
		@pre 			iUTContext is a valid CWspHeaderReader_UnitTestContext.
		@post			No change in the CWspHeaderReader_DecodeHeaderL_Transition
	*/
	inline CWspHeaderReader_UnitTestContext& Context() const;
	};	// CWspHeaderReader_DecodeHeaderL_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderReader_DecodeHeaderLTrap_Transition
	@Depends CWspHeaderReader & CTransition 

	Comments : Transition test of the CWspHeaderReader::DecodeHeaderLTrap method.
 */
class CWspHeaderReader_DecodeHeaderLTrap_Transition : public CTransitionType
	{
public:
	/**
		@fn				CWspHeaderReader_DecodeHeaderLTrap_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CWspHeaderReader_DecodeHeaderLTrap_Transition is fully constructed.
	*/
	CWspHeaderReader_DecodeHeaderLTrap_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CWspHeaderReader::DecodeHeaderLTrap method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CWspHeaderReader::DecodeHeaderLTrap)
		@since			7.0
		@return			None
		@pre 			CWspHeaderReader_DecodeHeaderLTrap_Transition is fully constructed.
		@post			No change in the CWspHeaderReader_DecodeHeaderLTrap_Transition apart
						from iWspHeaderReader, which may have changed state.
						(@see CWspHeaderReader::DecodeHeaderLTrap post-condition) for iWspHeaderReader's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CWspHeaderReader_UnitTestContext
		@pre 			iUTContext is a valid CWspHeaderReader_UnitTestContext.
		@post			No change in the CWspHeaderReader_DecodeHeaderLTrap_Transition
	*/
	inline CWspHeaderReader_UnitTestContext& Context() const;
	};	// CWspHeaderReader_DecodeHeaderLTrap_Transition

#include "WspHeaderReaderTransitions.inl"

#include "WspHeaderReaderUnitTestContext.inl"

#endif // __WSPHEADERREADERTRANSITIONS_H__

