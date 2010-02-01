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
// The context for the unit tests upon the TFilterConfigurationIterator class methods.
// 
//

/**
 @file FilterConfigurationIteratorUnitTestContext.inl
*/

// User includes
#include "FilterConfigurationIteratorTransitions.h"

// System includes
#include <http/tfilterinformation.h>

// ______________________________________________________________________________
//
inline CFilterConfigurationIterator_UnitTestContext::CFilterConfigurationIterator_UnitTestContext(CDataLogger& aDataLogger,
												MStateAccessor& aStateAccessor,
												MTransitionObserver& aObserver)
: CUnitTestContext(aDataLogger, aStateAccessor, aObserver)
	{
	//Do nothing
	}

inline CFilterConfigurationIterator_UnitTestContext::~CFilterConfigurationIterator_UnitTestContext()
	{
	iProtocolArray.ResetAndDestroy();
	iProtocolArray.Close();
	if(iFilterConfigurationIterator!=NULL)
		{
		DeleteFilterIter();
		}
	delete iFilterInfo;
	}

inline void CFilterConfigurationIterator_UnitTestContext::CreateFilterIterL()
	{
	_LIT8(KTxtProtocol, "HTTP/TCP");
	iTFSession.OpenL(KTxtProtocol(), this);
	}

inline void CFilterConfigurationIterator_UnitTestContext::DeleteFilterIter()
	{
	iTFSession.Close();
	iFilterConfigurationIterator = NULL;
	}

inline void CFilterConfigurationIterator_UnitTestContext::ConfigureSessionFiltersL(TFilterConfigurationIterator* aFilterConfigIter)
	{
	iFilterConfigurationIterator = aFilterConfigIter;
	}

inline const TDesC8& CFilterConfigurationIterator_UnitTestContext::GetTestFilterName(TTestDataType aTestDataType)
	{
	switch (aTestDataType)
		{
		case ENotExist:
			{
			return KTxtNoExist();
			} // break;
		case EHttpImplicit:
			{
			return KTxtHttpImplicit();
			} // break;
		default:
			{
			return KNullDesC8();
			} // break;
		}
	}
