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
 @file CNwssWspCOEventDispatcher.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__CNWSSWSPCOEVENTDISPATCHER_H__
#define __CNWSSWSPCOEVENTDISPATCHER_H__

// System includes
#include <e32base.h>
#include <wapcli.h>


// Forward declarations
class MNwssSessionEventHandler;
class MNwssTransactionEventHandler;
class MNwssOomHandler;

/**
	@since			7.0
	This class acts as a hub for the receipt of Connection-Oriented WSP session and
	transaction events.  It is an Active Object, and each time the method ::Start()
	is called, it requests an event from the NWSS WAP Stack.  Upon completion of
	the request, the incoming event is examined, and its type (session or
	transaction) is determined. A call-back method appropriate to the event type is
	called on either the MNwssTransactionEventHandler or MNwssSessionEventHandler
	interfaces that 'observes' the object.  In the case of transaction events, the
	NWSS WAP Stack's transaction handle (a RWSPCOTrans object) is passed through
	along with the event.
 */

//##ModelId=3BBD8CBA0288
class CNwssWspCOEventDispatcher : public CActive
	{
public: // methods

/** 
    Factory construction of CNwssWspCOEventDispatcher.
	@since			7.0
	@param			aWspSession		(in) The caller's Wap Stack CO WSP session handle.
	@param			aSessEventHnd	(in) An interface to be used for notifications
										 of received session events.
	@param			aTransEventHnd	(in) An interface to be used for notifications
										 of received transaction events.
	@param			aOutOfMemoryHnd	(in) An interface to be used in the event of OOM occurring
										 whilst the session or transaction event handlers were
										 handling a WAP stack event.
	@return			The newly constructed CNwssWspCOEventDispatcher instance.
	@leave			System error codes, e.g. KErrNoMemory.
 */
	//##ModelId=3C4C49B90178
	static CNwssWspCOEventDispatcher* NewL(RWSPCOConn& aWspSession,
										   MNwssSessionEventHandler& aSessEventHnd,
										   MNwssTransactionEventHandler& aTransEventHnd,
										   MNwssOomHandler& aOutOfMemoryHnd);

/**
	Destructor.
	@since			7.0
 */
	//##ModelId=3C4C49B900E2
	virtual ~CNwssWspCOEventDispatcher();

/**
	Start waiting for stack events.  The dispatcher goes active,
					and will repeatedly request events from the WAP Stack, until
					it is subsequently cancelled.
	@since			7.0
 */
	//##ModelId=3C4C49B9007D
	void Start();

private: // methods from CActive

/** 
		Called when the active object fires, ie. its asynchronous
					request has completed
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory and internal error codes
					generated for WTLS handshake failures, see <wsperror.h>
 */
	//##ModelId=3C4C49B803BB
	virtual void RunL();

/**
		Called if RunL leaves, to allow the AO to handle the error before
					the scheduler does
	@since			7.0
	@param			aError	(in) The error code to be handled, which RunL() left with
	@return			A final error code, should aError not have been handled, which the
					active scheduler handles.
 */
	//##ModelId=3C4C49B8025D
	virtual TInt RunError(TInt aError);

/**
		Cancellation protocol for the active object.  It cancels the
					request GetNextEvent() request it keeps outstanding with the
					WAP Stack.
	@since			7.0
 */
	//##ModelId=3C4C49B801B2
	virtual void DoCancel();

private: // methods

/**
	Normal constructor.  Keeps references to the supplied arguments.
	@since			7.0
	@param			aWspSession		(in) The caller's Wap Stack CO WSP session handle.
	@param			aSessEventHnd	(in) An interface to be used for notifications
										 of received session events.
	@param			aTransEventHnd	(in) An interface to be used for notifications
										 of received transaction events.
	@param			aOutOfMemoryHnd	(in) An interface to be used in the event of OOM occurring
										 whilst the session or transaction event handlers were
										 handling a WAP stack event.  
 */
	//##ModelId=3C4C49B70324
	CNwssWspCOEventDispatcher(RWSPCOConn& aWspSession,
							  MNwssSessionEventHandler& aSessEventHnd,
							  MNwssTransactionEventHandler& aTransEventHnd,
							  MNwssOomHandler& aOutOfMemoryHnd);

private: // attributes

	/** The WSP Session handle.  This is not owned, opened or closed in this class.
	*/
	//##ModelId=3C4C49B702AD
	RWSPCOConn& iWspSession;

	/** Handler for session events received at the dispatcher.
	*/
	//##ModelId=3C4C49B702A1
	MNwssSessionEventHandler& iSessEventHnd;

	/** Handler for transaction events received at the dispatcher.
	*/
	//##ModelId=3C4C49B7028D
	MNwssTransactionEventHandler& iTransEventHnd;

	/** Handler for out-of-memory situations.
	*/
	//##ModelId=3C9B09460197
	MNwssOomHandler& iOutOfMemoryHnd;

	/** The event that has occured
	*/
	//##ModelId=3C4C49B70279
	RWSPCOConn::TEvent iWspEvent;

	/** The transaction handle on which transaction events occur
	*/
	//##ModelId=3C4C49B70215
	RWSPCOTrans iTransaction;

	/** The ID of the last transaction on which an event was received
	*/
	//##ModelId=3C9B09460183
	RWSPCOTrans::TTransID iTrId;
	};


#endif // __CNWSSWSPCOEVENTDISPATCHER_H__
