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
// The implementation of the transition classes upon the TFilterConfigurationIterator class methods.
// 
//

/**
 @file FilterConfigurationIteratorTransitions.inl
*/

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorNewLTransition,"CFilterConfigurationIterator_NewL_Transition");

inline CFilterConfigurationIterator_NewL_Transition::CFilterConfigurationIterator_NewL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KFilterConfigurationIteratorNewLTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CFilterConfigurationIterator_NewL_Transition::TransitMethodL()
	{
	_LIT(KFilterConfigurationIteratorNewLTransitMethod, "CFilterConfigurationIterator::NewL transition");
	Context().DataLogger().LogInformation(KFilterConfigurationIteratorNewLTransitMethod);
	Context().CreateFilterIterL();
	}

inline CFilterConfigurationIterator_UnitTestContext& CFilterConfigurationIterator_NewL_Transition::Context() const
	{
	return REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorDtorTransition,"CFilterConfigurationIterator_Dtor_Transition");

// WARNING : It is not recommended that CLeakTestTransitions
// are used on Dtor methods, so if CTransitionType is defined as CLeakTestTransition
// it is suggested that this Dtor transition's base is changed explicitly to CTransition
inline CFilterConfigurationIterator_Dtor_Transition::CFilterConfigurationIterator_Dtor_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KFilterConfigurationIteratorDtorTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CFilterConfigurationIterator_Dtor_Transition::TransitMethodL()
	{
	_LIT(KFilterConfigurationIteratorDtorTransitMethod, "CFilterConfigurationIterator::Dtor transition");
	Context().DataLogger().LogInformation(KFilterConfigurationIteratorDtorTransitMethod);
	Context().DeleteFilterIter();
	}

inline CFilterConfigurationIterator_UnitTestContext& CFilterConfigurationIterator_Dtor_Transition::Context() const
	{
	return REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorFirstTransition,"CFilterConfigurationIterator_First_Transition");

inline CFilterConfigurationIterator_First_Transition::CFilterConfigurationIterator_First_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KFilterConfigurationIteratorFirstTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CFilterConfigurationIterator_First_Transition::TransitMethodL()
	{
	_LIT(KFilterConfigurationIteratorFirstTransitMethod, "CFilterConfigurationIterator::First transition");
	Context().DataLogger().LogInformation(KFilterConfigurationIteratorFirstTransitMethod);
	Context().iFirstReturn = Context().iFilterConfigurationIterator->First();
	}

inline CFilterConfigurationIterator_UnitTestContext& CFilterConfigurationIterator_First_Transition::Context() const
	{
	return REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorAtStartTransition,"CFilterConfigurationIterator_AtStart_Transition");

inline CFilterConfigurationIterator_AtStart_Transition::CFilterConfigurationIterator_AtStart_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KFilterConfigurationIteratorAtStartTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CFilterConfigurationIterator_AtStart_Transition::TransitMethodL()
	{
	_LIT(KFilterConfigurationIteratorAtStartTransitMethod, "CFilterConfigurationIterator::AtStart transition");
	Context().DataLogger().LogInformation(KFilterConfigurationIteratorAtStartTransitMethod);
	Context().iAtStartReturn = Context().iFilterConfigurationIterator->AtStart();
	}

inline CFilterConfigurationIterator_UnitTestContext& CFilterConfigurationIterator_AtStart_Transition::Context() const
	{
	return REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorNextTransition,"CFilterConfigurationIterator_Next_Transition");

inline CFilterConfigurationIterator_Next_Transition::CFilterConfigurationIterator_Next_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KFilterConfigurationIteratorNextTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CFilterConfigurationIterator_Next_Transition::TransitMethodL()
	{
	_LIT(KFilterConfigurationIteratorNextTransitMethod, "CFilterConfigurationIterator::Next transition");
	Context().DataLogger().LogInformation(KFilterConfigurationIteratorNextTransitMethod);
	Context().iNextReturn = Context().iFilterConfigurationIterator->Next();
	}

inline CFilterConfigurationIterator_UnitTestContext& CFilterConfigurationIterator_Next_Transition::Context() const
	{
	return REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorCurrentFilterInformationTransition,"CFilterConfigurationIterator_CurrentFilterInformation_Transition");

inline CFilterConfigurationIterator_CurrentFilterInformation_Transition::CFilterConfigurationIterator_CurrentFilterInformation_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KFilterConfigurationIteratorCurrentFilterInformationTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CFilterConfigurationIterator_CurrentFilterInformation_Transition::TransitMethodL()
	{
	_LIT(KFilterConfigurationIteratorCurrentFilterInformationTransitMethod, "CFilterConfigurationIterator::CurrentFilterInformation transition");
	Context().DataLogger().LogInformation(KFilterConfigurationIteratorCurrentFilterInformationTransitMethod);
	TFilterInformation filterInfo = Context().iFilterConfigurationIterator->CurrentFilterInformation();
	Context().iFilterInfo = new(ELeave) TFilterInformation(filterInfo);
	}

inline CFilterConfigurationIterator_UnitTestContext& CFilterConfigurationIterator_CurrentFilterInformation_Transition::Context() const
	{
	return REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorFindByDataTypeTransition,"CFilterConfigurationIterator_FindByDataType_Transition");

inline CFilterConfigurationIterator_FindByDataType_Transition::CFilterConfigurationIterator_FindByDataType_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KFilterConfigurationIteratorFindByDataTypeTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CFilterConfigurationIterator_FindByDataType_Transition::TransitMethodL()
	{
	_LIT(KFilterConfigurationIteratorFindByDataTypeTransitMethod, "CFilterConfigurationIterator::FindByDataType transition");
	Context().DataLogger().LogInformation(KFilterConfigurationIteratorFindByDataTypeTransitMethod);
	Context().iFindByDataTypeReturn = Context().iFilterConfigurationIterator->
		FindByDataType(Context().GetTestFilterName(CFilterConfigurationIterator_UnitTestContext::ENotExist));
	}

inline CFilterConfigurationIterator_UnitTestContext& CFilterConfigurationIterator_FindByDataType_Transition::Context() const
	{
	return REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KFilterConfigurationIteratorListProtocolsTransition,"CFilterConfigurationIterator_ListProtocols_Transition");

inline CFilterConfigurationIterator_ListProtocols_Transition::CFilterConfigurationIterator_ListProtocols_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransition(KFilterConfigurationIteratorListProtocolsTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CFilterConfigurationIterator_ListProtocols_Transition::TransitMethodL()
	{
	// This transition is a defect fix test for a memory leak in ListAvailableProtocolsL() on RHTTPSession
	TInt ii = 0;
	// OOM Loop
	FOREVER
		{
		__UHEAP_MARK;
		__UHEAP_FAILNEXT(ii);

		// Logging
		_LIT(KSessionListAvailableProtocolsTransitMethod, "RHTTPSession::ListAvailableProtocols transition");
		Context().DataLogger().LogInformation(KSessionListAvailableProtocolsTransitMethod);
	
		// Call method under test
		TRAPD(err,Context().iTFSession.ListAvailableProtocolsL(Context().iProtocolArray));
		Context().iProtocolCount = Context().iProtocolArray.Count();

		// If the method runs successfully items will have been appended to array so cleanup
		if(err==KErrNone)
			Context().iProtocolArray.ResetAndDestroy();

		__UHEAP_MARKEND;
		User::Heap().Check();
		__UHEAP_RESET;

		// Break out if the test passes successfully; allow only memory failure or test failure codes to proceed.
		if (err == KErrNone)
			break;
		else if (err != KErrNoMemory)
			User::Leave(err);

		// Next iteration
		++ii;
		}
	_LIT(KTransitionRunSuccess, "CLeakTestTransition::TransitMethodL() successful completion on iteration %d.");
	Context().DataLogger().LogInformationWithParameters(KTransitionRunSuccess, ii);
	}

inline CFilterConfigurationIterator_UnitTestContext& CFilterConfigurationIterator_ListProtocols_Transition::Context() const
	{
	return REINTERPRET_CAST(CFilterConfigurationIterator_UnitTestContext&,iUTContext);
	}
