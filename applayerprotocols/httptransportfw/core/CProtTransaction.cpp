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
//

// System includes
#include <e32std.h>
#include <http/framework/ctxdata.h>
#include <http/framework/crxdata.h>
#include <httperr.h>
#include <http/framework/cprottransaction.h>// Class signature


// Local includes


// Destructor - cleans up and releases resources to the system
EXPORT_C
CProtTransaction::~CProtTransaction()
	{
	delete iTxData;
	delete iRxData;
	}

/** Obtain the API-level transaction to which this object corresponds 
	@return a RHTTPTransaction object which represents the transaction to which this object corresponds.

*/
EXPORT_C
RHTTPTransaction CProtTransaction::Transaction() const
	{
	return iTrans;
	}

EXPORT_C
void CProtTransaction::ResetRxData()
	{
	delete iRxData;
	iRxData = NULL;
	}

EXPORT_C
void CProtTransaction::ResetTxData()
	{
	delete iTxData;
	iTxData = NULL;
	}

/** Obtain the Transmit Data portion of the transaction
	@return reference to CTxData object that contains transmit Data portion of the transaction.

*/
EXPORT_C
CTxData& CProtTransaction::TxData() const
	{
	return *iTxData;
	}

/** Obtain the Received Data portion of the transaction 
	@return reference to CRxData object that contains received Data portion of the transaction.

*/
EXPORT_C
CRxData& CProtTransaction::RxData() const
	{
	return *iRxData;
	}

// Default constructor
CProtTransaction::CProtTransaction()
	{
	// does nothing
	}

/** Normal constructor
*/
EXPORT_C
CProtTransaction::CProtTransaction(RHTTPTransaction aTrans)
: iTrans(aTrans), iTransactionState(EPending)
	{
	// does nothing
	}

CProtTransaction::TTransactionState CProtTransaction::TransactionState() const
	{
	return iTransactionState;
	}

void CProtTransaction::SetTransactionState(TTransactionState aState)
	{
	iTransactionState = aState;
	}
