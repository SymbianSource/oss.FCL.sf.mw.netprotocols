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
 @file WspHeaderWriterTransitions.h
*/

#ifndef __WSPHEADERWRITERTRANSITIONS_H__
#define __WSPHEADERWRITERTRANSITIONS_H__

#include "TestUtilities.h"
#include "StateAccessor.h"
#include "LeakTestTransition.h"
#include "DataLogger.h"
#include "CWspHeaderWriter.h"
#include "WspHeaderWriterStateAccessor.h"
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
	@class CWspHeaderWriter_UnitTestContext
	@Depends CWspHeaderWriter & CUnitTestContext 

	Comments : Provide all the CWspHeaderWriter specific
	parameters and behaviour on the CWspHeaderWriter
	test class for a transition.
 */
class CWspHeaderWriter_UnitTestContext : public CUnitTestContext
	{
public:
	/**
		@fn				CWspHeaderWriter_UnitTestContext(CDataLogger& aDataLogger,
														MStateAccessor& aStateAccessor,
														MTransitionObserver& aObserver)
		Intended Usage	: Default constructor.
		Error Condition	: None. 
		@since			7.0
		@param			aDataLogger The output logging object.
		@param			aObserver The observer of this UnitTest's Transitions.
		@param			aStateAccessor WhiteBox state access to the CWspHeaderWriter class under test.
		@pre 			None.
		@post			CWspHeaderWriter_UnitTestContext is fully constructed, and initialized.
	*/
	inline CWspHeaderWriter_UnitTestContext(CDataLogger& aDataLogger,
										MStateAccessor& aStateAccessor,
										MTransitionObserver& aObserver,
										const TStringTable& aStrTable);

	/**
		@fn				~CWspHeaderWriter_UnitTestContext()
		Intended Usage	: Default Destructor
		@since			7.0
		@pre 			CWspHeaderWriter_UnitTestContext is fully constructed.
		@post			CWspHeaderWriter_UnitTestContext is fully destroyed
		*/
		virtual inline ~CWspHeaderWriter_UnitTestContext();

	/** The instance of the class under test */
	CWspHeaderWriter* iWspHeaderWriter;

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

	}; // CWspHeaderWriter_UnitTestContext

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_NewL_Transition
	@Depends CWspHeaderWriter & CTransition 

	Comments : Transition test of the CWspHeaderWriter::NewL method.
 */
class CWspHeaderWriter_NewL_Transition : public CTransitionType
	{
public:
	/**
		@fn				CWspHeaderWriter_NewL_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CWspHeaderWriter_NewL_Transition is fully constructed.
	*/
	CWspHeaderWriter_NewL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CWspHeaderWriter::NewL method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CWspHeaderWriter::NewL)
		@since			7.0
		@return			None
		@pre 			CWspHeaderWriter_NewL_Transition is fully constructed.
		@post			No change in the CWspHeaderWriter_NewL_Transition apart
						from iWspHeaderWriter, which may have changed state.
						(@see CWspHeaderWriter::NewL post-condition) for iWspHeaderWriter's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CWspHeaderWriter_UnitTestContext
		@pre 			iUTContext is a valid CWspHeaderWriter_UnitTestContext.
		@post			No change in the CWspHeaderWriter_NewL_Transition
	*/
	inline CWspHeaderWriter_UnitTestContext& Context() const;
	};	// CWspHeaderWriter_NewL_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_Dtor_Transition
	@Depends CWspHeaderWriter & CTransition 

	Comments : Transition test of the CWspHeaderWriter::Dtor method.
 */
class CWspHeaderWriter_Dtor_Transition : public CTransitionType
	{
public:
	/**
		@fn				CWspHeaderWriter_Dtor_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CWspHeaderWriter_Dtor_Transition is fully constructed.
	*/
	CWspHeaderWriter_Dtor_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CWspHeaderWriter::Dtor method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CWspHeaderWriter::Dtor)
		@since			7.0
		@return			None
		@pre 			CWspHeaderWriter_Dtor_Transition is fully constructed.
		@post			No change in the CWspHeaderWriter_Dtor_Transition apart
						from iWspHeaderWriter, which may have changed state.
						(@see CWspHeaderWriter::Dtor post-condition) for iWspHeaderWriter's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CWspHeaderWriter_UnitTestContext
		@pre 			iUTContext is a valid CWspHeaderWriter_UnitTestContext.
		@post			No change in the CWspHeaderWriter_Dtor_Transition
	*/
	inline CWspHeaderWriter_UnitTestContext& Context() const;
	};	// CWspHeaderWriter_Dtor_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_EncodeHeaderL_Transition
	@Depends CWspHeaderWriter & CTransition 

	Comments : Transition test of the CWspHeaderWriter::EncodeHeaderL method.
 */
class CWspHeaderWriter_EncodeHeaderL_Transition : public CTransitionType
	{
public:
	/**
		@fn				CWspHeaderWriter_EncodeHeaderL_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CWspHeaderWriter_EncodeHeaderL_Transition is fully constructed.
	*/
	CWspHeaderWriter_EncodeHeaderL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CWspHeaderWriter::EncodeHeaderL method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CWspHeaderWriter::EncodeHeaderL)
		@since			7.0
		@return			None
		@pre 			CWspHeaderWriter_EncodeHeaderL_Transition is fully constructed.
		@post			No change in the CWspHeaderWriter_EncodeHeaderL_Transition apart
						from iWspHeaderWriter, which may have changed state.
						(@see CWspHeaderWriter::EncodeHeaderL post-condition) for iWspHeaderWriter's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CWspHeaderWriter_UnitTestContext
		@pre 			iUTContext is a valid CWspHeaderWriter_UnitTestContext.
		@post			No change in the CWspHeaderWriter_EncodeHeaderL_Transition
	*/
	inline CWspHeaderWriter_UnitTestContext& Context() const;
	};	// CWspHeaderWriter_EncodeHeaderL_Transition

// ______________________________________________________________________________
//
/**
	@internal
	@class CWspHeaderWriter_EncodeHeaderLTrap_Transition
	@Depends CWspHeaderWriter & CTransition 

	Comments : Transition test of the CWspHeaderWriter::EncodeHeaderLTrap method.
 */
class CWspHeaderWriter_EncodeHeaderLTrap_Transition : public CTransitionType
	{
public:
	/**
		@fn				CWspHeaderWriter_EncodeHeaderLTrap_Transition(CUnitTestContext& aUTContext,
																	TTransitionValidator& aValidator)
		Intended Usage	: Standard c'tor method.
		Error Condition	: None.
		@since			7.0
		@param			aUTContext The context this transition is operating in.
		@param			aValidator Used for checking the pre & post conditions of the test object.
		@pre 			None.
		@post			CWspHeaderWriter_EncodeHeaderLTrap_Transition is fully constructed.
	*/
	CWspHeaderWriter_EncodeHeaderLTrap_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator);
	/**
		@fn				TransitMethodL()
		Intended Usage	: To execute the CWspHeaderWriter::EncodeHeaderLTrap method for the test harness.
		Error Condition	: Leaves with an error code.
		@exception		KErrNoMemory, (@see CWspHeaderWriter::EncodeHeaderLTrap)
		@since			7.0
		@return			None
		@pre 			CWspHeaderWriter_EncodeHeaderLTrap_Transition is fully constructed.
		@post			No change in the CWspHeaderWriter_EncodeHeaderLTrap_Transition apart
						from iWspHeaderWriter, which may have changed state.
						(@see CWspHeaderWriter::EncodeHeaderLTrap post-condition) for iWspHeaderWriter's new state.
	*/
	inline void TransitMethodL();

	/**
		@fn				Context() const
		Intended Usage	: To provide access to the unit test context cast to the correct type.
		Error Condition	: None.
		@since			7.0
		@return			The unit test context cast to a CWspHeaderWriter_UnitTestContext
		@pre 			iUTContext is a valid CWspHeaderWriter_UnitTestContext.
		@post			No change in the CWspHeaderWriter_EncodeHeaderLTrap_Transition
	*/
	inline CWspHeaderWriter_UnitTestContext& Context() const;
	};	// CWspHeaderWriter_EncodeHeaderLTrap_Transition

#include "WspHeaderWriterTransitions.inl"

#include "WspHeaderWriterUnitTestContext.inl"

#endif // __WSPHEADERWRITERTRANSITIONS_H__

