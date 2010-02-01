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

/**
 @file
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__MHTTPTRANSACTIONCALLBACK_H__
#define __MHTTPTRANSACTIONCALLBACK_H__

// System includes
#include <e32std.h>
#include <http/rhttptransaction.h>
#include <http/thttpevent.h>


//##ModelId=3C4C1886022B
class MHTTPTransactionCallback
/** 
The per-transaction callback for receiving HTTP events.
@publishedAll
@released
*/
	{
public:	// Methods

	/** Called when the filter's registration conditions are satisfied for events that
		occur on a transaction.
		Note that this function is not allowed to leave if called with
		certain events. @see THTTPEvent
		@param aTransaction The transaction that the event has occurred on.
		@param aEvent The event that has occurred.
		@leave Any Leaves must be handled by the appropriate MHFRunError.
	*/
	//##ModelId=3C4C1886024C
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent) =0;

	/** Called when RunL leaves from a transaction event. This works in the same
		way as CActve::RunError; return KErrNone if you have handled the error.
		If you don't completely handle the error, a panic will occur.
		@param aError The leave code that RunL left with.
		@param aTransaction The transaction that was being processed.
		@param aEvent The Event that was being processed.
		@return KErrNone if the error has been cancelled or the code
		of the continuing error otherwise.
	*/
	//##ModelId=3C4C1886023F
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent) =0;

	}; 

#endif //	__MHTTPTRANSACTIONCALLBACK_H__
