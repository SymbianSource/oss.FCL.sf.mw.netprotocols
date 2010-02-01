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
// The implementation of the transition validation classes upon the TFilterConfigurationIterator class methods.
// 
//

/**
 @file FilterConfigurationIteratorTransitionValidation.inl
*/

// Literals
_LIT(KTxtTestFailed, "<FONT color=\"red\">***** TEST FAILED *****</FONT>");
_LIT(KTxtTestPassed, "<FONT color=\"green\">***** TEST PASSED OK *****</FONT>");

// ______________________________________________________________________________
//
inline TFilterConfigurationIterator_Ctor_TransitionValidator::TFilterConfigurationIterator_Ctor_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

_LIT(KCtorFailedInvariant,
	"*** CFilterConfigurationIterator_Ctor_Transition invariant failure ***");

inline TBool TFilterConfigurationIterator_Ctor_TransitionValidator::ValidatePreConditions()
	{
	// For a c'tor this should be empty.
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	if(iUTContext.StateAccessor().InvariantTest(context.iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		iUTContext.DataLogger().LogInformation(KCtorFailedInvariant);
		return EFalse;
		}
	// Make sure object does not exist
	if( context.iFilterConfigurationIterator != NULL )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFail, "*** C'tor pre condition fail - Object already exists ***");
		iUTContext.DataLogger().LogInformation(KTxtFail);
		return EFalse;
		}
	return ETrue;
	}

inline TBool TFilterConfigurationIterator_Ctor_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// For a d'tor this should be empty.
	if(iUTContext.StateAccessor().InvariantTest(context.iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		iUTContext.DataLogger().LogInformation(KCtorFailedInvariant);
		return EFalse;
		}
	// Make sure object has been created
	if( context.iFilterConfigurationIterator == NULL )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFail, "*** C'tor post condition fail - Test Object was not created ***");
		iUTContext.DataLogger().LogInformation(KTxtFail);
		return EFalse;
		}
	return ETrue;
	}

// ______________________________________________________________________________
//
inline TFilterConfigurationIterator_Dtor_TransitionValidator::TFilterConfigurationIterator_Dtor_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

_LIT(KDtorFailedInvariant,
	"*** CFilterConfigurationIterator_Dtor_Transition invariant failure ***");

inline TBool TFilterConfigurationIterator_Dtor_TransitionValidator::ValidatePreConditions()
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// For a d'tor this should not be empty.
	if(iUTContext.StateAccessor().InvariantTest(context.iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		iUTContext.DataLogger().LogInformation(KDtorFailedInvariant);
		return EFalse;
		}
	// Test object should exists at this point
	if( context.iFilterConfigurationIterator == NULL )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** D'tor pre-condition fail - Test Object already destroyed ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	return ETrue;
	}

inline TBool TFilterConfigurationIterator_Dtor_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// For a d'tor this should be empty.
	if(iUTContext.StateAccessor().InvariantTest(context.iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		iUTContext.DataLogger().LogInformation(KDtorFailedInvariant);
		return EFalse;
		}
	// Test object should be destroyed
	if( context.iFilterConfigurationIterator != NULL )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFail, "*** D'tor post condition fail - Test object not destroyed ***");
		iUTContext.DataLogger().LogInformation(KTxtFail);
		return EFalse;
		}
	iUTContext.DataLogger().LogInformation(KTxtTestPassed);
	return ETrue;
	}

// ______________________________________________________________________________
//
inline TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator::TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

_LIT(KSuccessfulFirstFailedInvariant,
	"CFilterConfigurationIterator_SuccessfulFirst_Transition invariant failure");

inline TBool TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator::ValidatePreConditions()
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// Invariant test
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext).iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KSuccessfulFirstFailedInvariant);
		return EFalse;
		}
	// Test object should exist
	if(context.iFilterConfigurationIterator == NULL)
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** First method pre-condition fail - Test Object not created ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	return ETrue;
	}

inline TBool TFilterConfigurationIterator_SuccessfulFirst_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// Invariant test
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext).iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KSuccessfulFirstFailedInvariant);
		return EFalse;
		}
	// First method should be successful returning KErrNone
	if(context.iFirstReturn != KErrNone)
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFail, "*** Successful First method post condition fail - First method returned %d ***");
		iUTContext.DataLogger().LogInformationWithParameters(KTxtFail, context.iFirstReturn);
		return EFalse;
		}
	return ETrue;
	}

// ______________________________________________________________________________
//
inline TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator::TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

_LIT(KSuccessfulAtStartFailedInvariant,
	"CFilterConfigurationIterator_SuccessfulAtStart_Transition invariant failure");

inline TBool TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator::ValidatePreConditions()
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// Invariant test
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext).iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KSuccessfulAtStartFailedInvariant);
		return EFalse;
		}
	// Test object should exist
	if(context.iFilterConfigurationIterator == NULL)
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** AtStart method pre-condition fail - Test Object not created ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	// Call to First() should be successful
	if(context.iFirstReturn != KErrNone)
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** AtStart method pre-condition fail - First Method was not succesful ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	return ETrue;
	}

inline TBool TFilterConfigurationIterator_SuccessfulAtStart_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// Invariant test
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext).iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KSuccessfulAtStartFailedInvariant);
		return EFalse;
		}
	// Iterator should be at start
	if( context.iAtStartReturn != context.iAtStartExpected )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFail, "*** Successful AtStart method post condition fail - Method returned False ***");
		iUTContext.DataLogger().LogInformation(KTxtFail);
		return EFalse;
		}
	return ETrue;
	}

// ______________________________________________________________________________
//
inline TFilterConfigurationIterator_SuccessfulNext_TransitionValidator::TFilterConfigurationIterator_SuccessfulNext_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

_LIT(KSuccessfulNextFailedInvariant,
	"CFilterConfigurationIterator_SuccessfulNext_Transition invariant failure");

inline TBool TFilterConfigurationIterator_SuccessfulNext_TransitionValidator::ValidatePreConditions()
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// Invariant test
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext).iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KSuccessfulNextFailedInvariant);
		return EFalse;
		}
	// Test object should exist
	if(context.iFilterConfigurationIterator == NULL)
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** First method pre-condition fail - Test Object not created ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	// Call to first should be successful
	if(context.iFirstReturn != KErrNone)
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** First method pre-condition fail - First Method was not succesful ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	return ETrue;
	}

inline TBool TFilterConfigurationIterator_SuccessfulNext_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// Invariant test
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext).iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KSuccessfulNextFailedInvariant);
		return EFalse;
		}
	// Call to Next should be successful
	if(context.iNextReturn != KErrNone)
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** Next method post-condition fail - Next Method was not succesful ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	return ETrue;
	}

// ______________________________________________________________________________
//
inline TFilterConfigurationIterator_FindByDataType_TransitionValidator::TFilterConfigurationIterator_FindByDataType_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

_LIT(KFindByDataTypeFailedInvariant,
	"CFilterConfigurationIterator_FindByDataType_Transition invariant failure");

inline TBool TFilterConfigurationIterator_FindByDataType_TransitionValidator::ValidatePreConditions()
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// Invariant test
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext).iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KFindByDataTypeFailedInvariant);
		return EFalse;
		}
	// Test object should exist
	if(context.iFilterConfigurationIterator == NULL)
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** FindByDataType method pre-condition fail - Test Object not created ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	// Call to first should be successful
	if(context.iFirstReturn != KErrNone)
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** FindByDataType method pre-condition fail - First Method was not succesful ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	return ETrue;
	}

inline TBool TFilterConfigurationIterator_FindByDataType_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// Invariant test
	if(iUTContext.StateAccessor().InvariantTest(REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext).iFilterConfigurationIterator))
		{
		iUTContext.DataLogger().LogInformation(KFindByDataTypeFailedInvariant);
		return EFalse;
		}
	// Call to FindByDataType() should match expected value
	if(context.iFindByDataTypeReturn != context.iFindByDataTypeExpected )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** FindByDataType method post-condition fail - FindByDataType found filter plugin ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	return ETrue;
	}

// ______________________________________________________________________________
//
inline TFilterConfigurationIterator_SessionListProtocols_TransitionValidator::TFilterConfigurationIterator_SessionListProtocols_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

_LIT(KSessionListProtocolsFailedInvariant,
	"CFilterConfigurationIterator_SessionListProtocols_Transition invariant failure");

inline TBool TFilterConfigurationIterator_SessionListProtocols_TransitionValidator::ValidatePreConditions()
	{
	// No pre-conditions needed here
	return ETrue;
	}

inline TBool TFilterConfigurationIterator_SessionListProtocols_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	// Check that there was at least one protocol listed on the device
	CFilterConfigurationIterator_UnitTestContext& context = REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	// Invariant test
	if(context.iProtocolCount == 0)
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFailed, "*** There are no Protocol Handlers found on the device - At least one is required. ***");
		iUTContext.DataLogger().LogInformation(KTxtFailed);
		return EFalse;
		}
	iUTContext.DataLogger().LogInformation(KTxtTestPassed);
	return ETrue;
	}
