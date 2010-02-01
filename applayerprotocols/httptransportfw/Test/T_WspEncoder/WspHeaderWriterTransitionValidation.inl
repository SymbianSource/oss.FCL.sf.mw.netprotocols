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
// The implementation of the transition validation classes upon the CWspHeaderWriter class methods.
// 
//

/**
 @file WspHeaderWriterTransitionValidation.inl
*/

// Literals
_LIT(KTxtTestFailed, "<FONT color=\"red\">***** TEST FAILED *****</FONT>");
_LIT(KTxtTestPassed, "<FONT color=\"green\">***** TEST PASSED OK *****</FONT>");

// ______________________________________________________________________________
//
inline TWspHeaderWriter_Ctor_TransitionValidator::TWspHeaderWriter_Ctor_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

_LIT(KCtorFailedInvariant,
	"CWspHeaderWriter_Ctor_Transition invariant failure");

inline TBool TWspHeaderWriter_Ctor_TransitionValidator::ValidatePreConditions()
	{
	/*
	CWspHeaderWriter_UnitTestContext& context = REINTERPRET_CAST(CWspHeaderWriter_UnitTestContext&,iUTContext);
	if( context.iCodec != NULL )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFail, "*** C'tor pre condition fail - Object already exists ***");
		iUTContext.DataLogger().LogInformation(KTxtFail);
		return EFalse
		}
	*/
	return ETrue;
	}

inline TBool TWspHeaderWriter_Ctor_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	/*
	CWspHeaderWriter_UnitTestContext& context = REINTERPRET_CAST(CWspHeaderWriter_UnitTestContext&,iUTContext);
	if( context.iCodec == NULL )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFail, "*** C'tor post condition fail - Object not created ***");
		iUTContext.DataLogger().LogInformation(KTxtFail);
		return EFalse
		}
	*/
	//iUTContext.DataLogger().LogInformation(KTxtTestPassed);
	return ETrue;
	}

// ______________________________________________________________________________
//
inline TWspHeaderWriter_Dtor_TransitionValidator::TWspHeaderWriter_Dtor_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

_LIT(KDtorFailedInvariant,
	"CWspHeaderWriter_Dtor_Transition invariant failure");

inline TBool TWspHeaderWriter_Dtor_TransitionValidator::ValidatePreConditions()
	{
	/*
	CWspHeaderWriter_UnitTestContext& context = REINTERPRET_CAST(CWspHeaderWriter_UnitTestContext&,iUTContext);
	if( context.iCodec == NULL )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFail, "*** D'tor pre condition fail - Object not created ***");
		iUTContext.DataLogger().LogInformation(KTxtFail);
		return EFalse
		}
	*/
	return ETrue;
	}

inline TBool TWspHeaderWriter_Dtor_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	/*
	CWspHeaderWriter_UnitTestContext& context = REINTERPRET_CAST(CWspHeaderWriter_UnitTestContext&,iUTContext);
	if( context.iCodec != NULL )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFail, "*** D'tor post condition fail - Object not destroyed ***");
		iUTContext.DataLogger().LogInformation(KTxtFail);
		return EFalse
		}
	*/
	iUTContext.DataLogger().LogInformation(KTxtTestPassed);
	return ETrue;
	}

// ______________________________________________________________________________
//
inline TWspHeaderWriter_EncodeHeaderL_TransitionValidator::TWspHeaderWriter_EncodeHeaderL_TransitionValidator(CUnitTestContext& aUTContext)
:TTransitionValidator(aUTContext)
	{
	// Do nothing
	}

_LIT(KEncodeHeaderLFailedInvariant,
	"CWspHeaderWriter_EncodeHeaderL_Transition invariant failure");

inline TBool TWspHeaderWriter_EncodeHeaderL_TransitionValidator::ValidatePreConditions()
	{
	// @TODO - do a check that the header field is what we expect
	return ETrue;
	}

inline TBool TWspHeaderWriter_EncodeHeaderL_TransitionValidator::ValidatePostConditions(TTestBedAsyncState /* aAsyncState */)
	{
	CWspHeaderWriter_UnitTestContext& context = REINTERPRET_CAST(CWspHeaderWriter_UnitTestContext&,iUTContext);
	TPtrC8 headerNameDes;
	if( context.iHeaderToken != KErrNotFound )
		{
		RStringF headerName = context.iStrPool.StringF(context.iHeaderToken, context.iStrTable);
		headerNameDes.Set(headerName.DesC());
		}
	else
		{
		headerNameDes.Set(KTxtTestHeader());
		}
	_LIT8(KTxtHeaderName, "Header Name - %S");
	iUTContext.DataLogger().LogInformationWithParameters(KTxtHeaderName, &headerNameDes);
	// Do a check for the buffer sixe size maximum that can be logged is 150
	if( (context.iExpectedData->Length() < 100) && (context.iOutputData->Length() < 100) )
		{
		_LIT8(KTxtExpected, "Expected Data - %S");
		_LIT8(KTxtActual, "Actual Data - %S");
		iUTContext.DataLogger().LogInformationWithParameters(KTxtExpected, context.iExpectedData);
		iUTContext.DataLogger().LogInformationWithParameters(KTxtActual, context.iOutputData);
		}

	if( context.iExpectedData->CompareF(*(context.iOutputData)) != 0 )
		{
		iUTContext.DataLogger().LogInformation(KTxtTestFailed);
		_LIT(KTxtFail, "*** Decode Header post condition fail - Expected data not same as output ***");
		iUTContext.DataLogger().LogInformation(KTxtFail);
		return EFalse;
		}
	//iUTContext.DataLogger().LogInformation(KTxtTestPassed);
	return ETrue;
	}
