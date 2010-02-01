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
// The implementation of the transition classes upon the CWspHeaderReader class methods.
// 
//

/**
 @file WspHeaderReaderTransitions.inl
*/

// User includes
#include "cwspheadercodec.h"

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderNewLTransition,"CWspHeaderReader_NewL_Transition");

inline CWspHeaderReader_NewL_Transition::CWspHeaderReader_NewL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KWspHeaderReaderNewLTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CWspHeaderReader_NewL_Transition::TransitMethodL()
	{
	_LIT(KWspHeaderReaderNewLTransitMethod, "CWspHeaderReader::NewL transition");
	Context().DataLogger().LogInformation(KWspHeaderReaderNewLTransitMethod);
	}

inline CWspHeaderReader_UnitTestContext& CWspHeaderReader_NewL_Transition::Context() const
	{
	return REINTERPRET_CAST(CWspHeaderReader_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderDtorTransition,"CWspHeaderReader_Dtor_Transition");

// WARNING : It is not recommended that CLeakTestTransitions
// are used on Dtor methods, so if CTransitionType is defined as CLeakTestTransition
// it is suggested that this Dtor transition's base is changed explicitly to CTransition
inline CWspHeaderReader_Dtor_Transition::CWspHeaderReader_Dtor_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KWspHeaderReaderDtorTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CWspHeaderReader_Dtor_Transition::TransitMethodL()
	{
	_LIT(KWspHeaderReaderDtorTransitMethod, "CWspHeaderReader::Dtor transition");
	Context().DataLogger().LogInformation(KWspHeaderReaderDtorTransitMethod);
	}

inline CWspHeaderReader_UnitTestContext& CWspHeaderReader_Dtor_Transition::Context() const
	{
	return REINTERPRET_CAST(CWspHeaderReader_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderDecodeHeaderLTransition,"CWspHeaderReader_DecodeHeaderL_Transition");

inline CWspHeaderReader_DecodeHeaderL_Transition::CWspHeaderReader_DecodeHeaderL_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KWspHeaderReaderDecodeHeaderLTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CWspHeaderReader_DecodeHeaderL_Transition::TransitMethodL()
	{
	_LIT(KWspHeaderReaderDecodeHeaderLTransitMethod, "CWspHeaderReader::DecodeHeaderL transition");
	Context().DataLogger().LogInformation(KWspHeaderReaderDecodeHeaderLTransitMethod);
	Context().SetOutputL();
	}

inline CWspHeaderReader_UnitTestContext& CWspHeaderReader_DecodeHeaderL_Transition::Context() const
	{
	return REINTERPRET_CAST(CWspHeaderReader_UnitTestContext&,iUTContext);
	}

// ______________________________________________________________________________
//
_LIT(KWspHeaderReaderDecodeHeaderLTrapTransition,"CWspHeaderReader_DecodeHeaderLTrap_Transition");

inline CWspHeaderReader_DecodeHeaderLTrap_Transition::CWspHeaderReader_DecodeHeaderLTrap_Transition(CUnitTestContext& aUTContext,
													TTransitionValidator& aValidator)
: CTransitionType(KWspHeaderReaderDecodeHeaderLTrapTransition, aUTContext, aValidator)
	{
	// Do nothing here.
	}

inline void CWspHeaderReader_DecodeHeaderLTrap_Transition::TransitMethodL()
	{
	_LIT(KWspHeaderReaderDecodeHeaderLTransitMethod, "CWspHeaderReader::DecodeHeaderL transition");
	Context().DataLogger().LogInformation(KWspHeaderReaderDecodeHeaderLTransitMethod);
	Context().SetTrapOutputL();
	}

inline CWspHeaderReader_UnitTestContext& CWspHeaderReader_DecodeHeaderLTrap_Transition::Context() const
	{
	return REINTERPRET_CAST(CWspHeaderReader_UnitTestContext&,iUTContext);
	}
