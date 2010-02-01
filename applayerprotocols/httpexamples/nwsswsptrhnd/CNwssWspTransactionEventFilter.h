// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CNWSSWSPTRANSACTIONEVENTFILTER_H__
#define __CNWSSWSPTRANSACTIONEVENTFILTER_H__

#include <e32base.h>
#include <wapcli.h>

#include "mnwsstransactioneventhandler.h"

class CNwssTransLookUpTable;

//##ModelId=3C9B095501DE
class CNwssWspTransactionEventFilter : public CActive, public MNwssTransactionEventHandler
	{
public:	// Methods

/**	
	Static factory c'tor.
	@since			7.0
	@param			aTransLUT			A reference to the transaction look up 
										table.
	@param			aTransEventHandler	A reference to the actual transaction 
										event handler.
*/
	//##ModelId=3C9B095502B2
	static CNwssWspTransactionEventFilter* NewL(
											   CNwssTransLookUpTable&			aTransTable, 
											   MNwssTransactionEventHandler&	aTransEventHandler,
											   TInt							aTableSize
											   );

/** 
	Destructor.
	@since			7.0
*/
	//##ModelId=3C9B095502B1
	virtual ~CNwssWspTransactionEventFilter();

/**
	Signals that a T-MethodAbort.ind needs to be sent. The aborted method 
	aTransaction is marked as being aborted in the transaction look up table.
	@since			7.0
	@param			aTransId	The ID for the transaction object that has been
								aborted.
*/
	//##ModelId=3C9B095502A9
	void SendMethodAbortIndEvent(RWSPCOTrans::TTransID aTransId);

/**
	Indicates that client has sent the S-Disconnect.req primitive. Need to ignore
	all transaction events bar T-MethodAbort.ind.
	@since			7.0
*/
	//##ModelId=3C9B095502A8
	void SessionDisconnectRequested();

/**
	Indicates that the server has sent the S-Disconnect.ind primitive.
	@since			7.0
*/
	//##ModelId=3C9B095502A7
	void SessionDisconnected();

/**
  	Resizes the transaction Id table.  If aNewSize exceeds the 
					current size, then entries are added.  If it is less than 
					the current size, then surplus entries are deleted.
  	@since			7.0
	@param			aNewSize	(in) The new table size
	@leave			System error codes, e.g. KErrNoMemory if the table can't be
					enlarged.
 */
	//##ModelId=3C9B0955029D
	void ResizeTableL(TInt aNewSize);

private: // methods inherited from MNwssTransactionEventHandler

	/** This represents the T-MethodInvoke.cnf primitive.
		@see MNwssTransactionEventHandler
	*/
	//##ModelId=3C9B09550293
	virtual void HandleMethodInvokeCnfL(RWSPCOTrans::TTransID aTransId);

	/** This represents the T-MethodResult.ind primitive.
		@see MNwssTransactionEventHandler
	*/
	//##ModelId=3C9B09550289
	virtual void HandleMethodResultIndL(RWSPCOTrans::TTransID aTransId);

	/** This represents the T-MethodAbort.ind primitive.
		@see MNwssTransactionEventHandler
	*/
	//##ModelId=3C9B0955027E
	virtual void HandleAbortIndL(RWSPCOTrans::TTransID aTransId);

private: // methods from CActive

/**Called when the active object fires, ie. its asynchronous
					request has completed
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory and internal error codes
					generated for WTLS handshake failures, see <wsperror.h>
 */
	//##ModelId=3C9B09550275
	virtual void RunL();

/**Called if RunL leaves, to allow the AO to handle the error before
					the scheduler does
	@since			7.0
	@param			aError	(in) The error code to be handled, which RunL() left with
	@return			A final error code, should aError not have been handled, which the
					active scheduler handles.
 */
	//##ModelId=3C9B0955026B
	virtual TInt RunError(TInt aError);

/**Cancellation protocol for the active object.  It cancels the
					request GetNextEvent() request it keeps outstanding with the
					WAP Stack.
	@since			7.0
 */
	//##ModelId=3C9B0955026A
	virtual void DoCancel();

private:	// Methods

/**
	C'tor.
	@since				7.0
*/
	//##ModelId=3C9B09550257
	CNwssWspTransactionEventFilter(CNwssTransLookUpTable& aTransTable, MNwssTransactionEventHandler& aTransEventHandler);

/**
	Second phase c'tor.
	@since				7.0
*/
	//##ModelId=3C9B0955024D
	void ConstructL(TInt aTableSize);

/**
	Sets this object active and then completes the request.
	@since			7.0
*/
	//##ModelId=3C9B0955024C
	void CompleteSelf();

private:	// Attributes

	/** A reference to the transaction look up table.
	*/
	//##ModelId=3C9B09550244
	CNwssTransLookUpTable&			iTransTable;

	/** A reference to the actual transaction event handler
	*/
	//##ModelId=3C9B09550230
	MNwssTransactionEventHandler&	iTransEventHandler;

	/** The index to the next available slot in the table.
	*/
	//##ModelId=3C9B09550226
	TInt							iHighWaterMark;

	/**	An array of entries for the transaction IDs of aborted methods;
	*/
	//##ModelId=3C9B0955021A
	RArray<RWSPCOTrans::TTransID>	iEntries;

	/** A flag indicating whether the session is in the Closing state.
	*/
	//##ModelId=3C9B09550209
	TBool							iSessionClosing;

	};

#endif // __CNWSSWSPTRANSACTIONEVENTFILTER_H__
