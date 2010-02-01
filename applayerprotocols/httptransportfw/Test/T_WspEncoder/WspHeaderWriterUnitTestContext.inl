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
// The context for the unit tests upon the CWspHeaderWriter class methods.
// 
//

/**
 @file WspHeaderWriterUnitTestContext.inl
*/

#include "WspHeaderWriterTransitions.h"
#include "cheaders.h"
#include "cheaderfield.h"

// System includes
#include <wspstringconstants.h>

_LIT8(KTxtTestHeader, "TestHeader");

// ______________________________________________________________________________
//
inline CWspHeaderWriter_UnitTestContext::CWspHeaderWriter_UnitTestContext(CDataLogger& aDataLogger,
												MStateAccessor& aStateAccessor,
												MTransitionObserver& aObserver,
												const TStringTable& aStrTable)
: CUnitTestContext(aDataLogger, aStateAccessor, aObserver), iStrTable(aStrTable)
	{
	//Do nothing
	}

inline CWspHeaderWriter_UnitTestContext::~CWspHeaderWriter_UnitTestContext()
	{
	delete iExpectedData;
	delete iOutputData;
	delete iTestHeader;
	delete iHeaderField;
	delete iHeader;
	delete iCodec;
	iStrPool.Close();
	}

inline void CWspHeaderWriter_UnitTestContext::ConstructL(TInt aHeaderName)
	{
	iHeaderToken = aHeaderName;
	iStrPool.OpenL(iStrTable);
	iCodec = CWspHeaderCodec::NewL(iStrPool, iStrTable);
	REINTERPRET_CAST(CWspHeaderCodec*, iCodec)->SetWspVersion(CWspHeaderCodec::EVersion1_4);
	iHeader = CHeaders::NewL(*iCodec);
	if( iHeaderToken == KErrNotFound )
		{
		RStringF testHeader = iStrPool.OpenFStringL(KTxtTestHeader());
		CleanupClosePushL(testHeader);
		iHeaderField = CHeaderField::NewL(testHeader, *iHeader);
		CleanupStack::PopAndDestroy(&testHeader);
		}
	else
		{
		iHeaderField = CHeaderField::NewL(iStrPool.StringF(aHeaderName, iStrTable), *iHeader);
		}
	iTestHeader = new(ELeave) RHeaderField(*iHeaderField);
	}

inline void CWspHeaderWriter_UnitTestContext::SetOutputL()
	{
	TPtrC8 rawData;
	iTestHeader->RawDataL(rawData);
	iOutputData = rawData.AllocL();
	}

inline void CWspHeaderWriter_UnitTestContext::SetExpectedL(const TDesC8& aExpectedData)
	{
	iExpectedData = aExpectedData.AllocL();
	}

inline void CWspHeaderWriter_UnitTestContext::SetTrapOutputL()
	{
	TPtrC8 rawData;
	TRAPD(err, iTestHeader->RawDataL(rawData));
	if(err == KErrNoMemory)
		User::Leave(err);

	TBuf8<25> convertNum;
	convertNum.Num(err);
	iOutputData = convertNum.AllocL();
	}
