// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TRANSSTATE_H
#define TRANSSTATE_H

// Definition of enumerator used to record CTransactor state.
// Must ensure the mode enums here match CRepository::TTransactionMode
enum TTransactionState
	{
	ENoTransaction = 0,
	EReadBit = 1,
	EWriteBit = 2,
	EFailedBit = 4,
	EReadTransaction = EReadBit,					// pessimistic, locking
	EConcurrentReadWriteTransaction = EWriteBit,	// optimistic, non-serialised
	EReadWriteTransaction = EReadBit | EWriteBit,	// pessimistic, locking
	// must ensure all bits used to represent transaction mode are set in
	// EAllTransactionModeBits otherwise TransactionMode method will not be correct
	EAllTransactionModeBits = EReadBit | EWriteBit
	};

#endif // TRANSSTATE_H
