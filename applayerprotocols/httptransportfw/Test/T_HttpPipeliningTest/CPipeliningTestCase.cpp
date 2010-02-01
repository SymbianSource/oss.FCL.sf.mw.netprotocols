// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include "CPipeliningTestCase.h"


CPipeliningTestCase::CPipeliningTestCase()
	{
	}

CPipeliningTestCase::~CPipeliningTestCase()
	{
	}

TInt CPipeliningTestCase::TransactionCount(TInt /*aConnectionIndex*/) const
	{
	return TotalTransactionCount();
	}

TInt CPipeliningTestCase::ConnectionCount() const
	{
	return 0;
	}

TInt CPipeliningTestCase::RealExpectedConnectionCount() const
	{
	return ConnectionCount();
	}

TBool CPipeliningTestCase::BatchTransactions() const
	{
	return ETrue;
	}	

TInt CPipeliningTestCase::ExpectedError(RHTTPTransaction /*aTrans*/)
	{
	return KErrNone;
	}

TBool CPipeliningTestCase::EnableBatching() const
	{
	return EFalse;
	}
	
void CPipeliningTestCase::ProcessHeadersL(RHTTPTransaction /*aTrans*/)
	{
	}

TBool CPipeliningTestCase::TransportHandlerPriority() const
	{
	return EFalse;
	}
	

