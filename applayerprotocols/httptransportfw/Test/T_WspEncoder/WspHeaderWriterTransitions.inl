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
// The implementation of the transition classes upon the CWspHeaderWriter class methods.
// 
//

/**
 @file WspHeaderWriterTransitions.inl
*/

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterNewLTransition,"CWspHeaderWriter_NewL_Transition");

inline CWspHeaderWriter_NewL_Transition::CWspHeaderWriter_NewL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KWspHeaderWriterNewLTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CWspHeaderWriter_NewL_Transition::TransitMethodL()
	{
	_LIT(KWspHeaderWriterNewLTransitMethod, "CWspHeaderWriter::NewL transition");
	Context().DataLogger().LogInformation(KWspHeaderWriterNewLTransitMethod);
	}

inline CWspHeaderWriter_UnitTestContext& CWspHeaderWriter_NewL_Transition::Context() const
	{
	return REINTERPRET_CAST(CWspHeaderWriter_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterDtorTransition,"CWspHeaderWriter_Dtor_Transition");

// WARNING : It is not recommended that CLeakTestTransitions
// are used on Dtor methods, so if CTransitionType is defined as CLeakTestTransition
// it is suggested that this Dtor transition's base is changed explicitly to CTransition
inline CWspHeaderWriter_Dtor_Transition::CWspHeaderWriter_Dtor_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KWspHeaderWriterDtorTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CWspHeaderWriter_Dtor_Transition::TransitMethodL()
	{
	_LIT(KWspHeaderWriterDtorTransitMethod, "CWspHeaderWriter::Dtor transition");
	Context().DataLogger().LogInformation(KWspHeaderWriterDtorTransitMethod);
	}

inline CWspHeaderWriter_UnitTestContext& CWspHeaderWriter_Dtor_Transition::Context() const
	{
	return REINTERPRET_CAST(CWspHeaderWriter_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterEncodeHeaderLTransition,"CWspHeaderWriter_EncodeHeaderL_Transition");

inline CWspHeaderWriter_EncodeHeaderL_Transition::CWspHeaderWriter_EncodeHeaderL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KWspHeaderWriterEncodeHeaderLTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CWspHeaderWriter_EncodeHeaderL_Transition::TransitMethodL()
	{
	_LIT(KWspHeaderWriterEncodeHeaderLTransitMethod, "CWspHeaderWriter::EncodeHeaderL transition");
	Context().DataLogger().LogInformation(KWspHeaderWriterEncodeHeaderLTransitMethod);
	Context().SetOutputL();
	}

inline CWspHeaderWriter_UnitTestContext& CWspHeaderWriter_EncodeHeaderL_Transition::Context() const
	{
	return REINTERPRET_CAST(CWspHeaderWriter_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderWriterEncodeHeaderLTrapTransition,"CWspHeaderWriter_EncodeHeaderLTrap_Transition");

inline CWspHeaderWriter_EncodeHeaderLTrap_Transition::CWspHeaderWriter_EncodeHeaderLTrap_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KWspHeaderWriterEncodeHeaderLTrapTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CWspHeaderWriter_EncodeHeaderLTrap_Transition::TransitMethodL()
	{
	_LIT(KWspHeaderWriterEncodeHeaderLTrapTransitMethod, "CWspHeaderWriter::EncodeHeaderLTrap transition");
	Context().DataLogger().LogInformation(KWspHeaderWriterEncodeHeaderLTrapTransitMethod);
	Context().SetTrapOutputL();
	}

inline CWspHeaderWriter_UnitTestContext& CWspHeaderWriter_EncodeHeaderLTrap_Transition::Context() const
	{
	return REINTERPRET_CAST(CWspHeaderWriter_UnitTestContext&,iUTContext);
	}
