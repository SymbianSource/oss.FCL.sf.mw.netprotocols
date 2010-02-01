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

#ifndef __CNWSSWSPSESSIONEVENTFILTER_H__
#define __CNWSSWSPSESSIONEVENTFILTER_H__

#include <e32base.h>

#include "mnwsssessioneventhandler.h"

class CNwssTransLookUpTable;
class MNwssTransactionEventHandler;

//##ModelId=3C9B094C023F
class CNwssWspSessionEventFilter : public CActive, public MNwssSessionEventHandler
	{
public:	// Methods

/**
	Static factory c'tor.
	@since			7.0
	@param			aTransLUT				A reference to the transaction lookup
											table.
	@param			aSessionEventHandler	A reference to the actual session
											event handler.
	@param			aTransEventHandler		A reference to the actual transaction 
											event handler.
*/
	//##ModelId=3C9B094C02B8
	static CNwssWspSessionEventFilter* NewL(
										   CNwssTransLookUpTable&			aTransTable, 
										   MNwssSessionEventHandler&		aSessionEventHandler,
										   MNwssTransactionEventHandler&	aTransEventHandler
										   );

/**Destructor.
	@since			7.0
*/
	//##ModelId=3C9B094C02B1
	virtual ~CNwssWspSessionEventFilter();

private: // methods inherited from MNwssSessionEventHandler

/**	@see			MNwssSessionEventHandler
 */
	//##ModelId=3C9B094C02B0
	virtual void HandleDisconnectIndL();

/**	
	@see			MNwssSessionEventHandler
 */
	//##ModelId=3C9B094C02AF
	virtual void HandleSuspendIndL();

/**	
	@see			MNwssSessionEventHandler
 */
	//##ModelId=3C9B094C02AE
	virtual void HandleResumeCnfL();

/**	
	@see			MNwssSessionEventHandler
 */
	//##ModelId=3C9B094C02A5
	virtual void HandleConnectCnfL();

/**	
	@see			MNwssSessionEventHandler
 */
	//##ModelId=3C9B094C02A4
	virtual void HandleRedirectIndL();

/**	
	@see			MNwssSessionEventHandler
 */
	//##ModelId=3C9B094C029C
	virtual void HandleExceptionIndL();

private: // methods from CActive

/** 
	Called when the active object fires, ie. its asynchronous
					request has completed
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory and internal error codes
					generated for WTLS handshake failures, see <wsperror.h>
 */
	//##ModelId=3C9B094C029B
	virtual void RunL();

/**Called if RunL leaves, to allow the AO to handle the error before
					the scheduler does
	@since			7.0
	@param			aError	(in) The error code to be handled, which RunL() left with
	@return			A final error code, should aError not have been handled, which the
					active scheduler handles.
 */
	//##ModelId=3C9B094C0293
	virtual TInt RunError(TInt aError);

/**Cancellation protocol for the active object.  It cancels the
					request GetNextEvent() request it keeps outstanding with the
					WAP Stack.
	@since			7.0
 */
	//##ModelId=3C9B094C0292
	virtual void DoCancel();

private:	// Methods

/**
	C'tor.
	@since				7.0
*/
	//##ModelId=3C9B094C0287
	CNwssWspSessionEventFilter(
							  CNwssTransLookUpTable&			aTransTable, 
							  MNwssSessionEventHandler&			aSessionEventHandler,
							  MNwssTransactionEventHandler&		aTransEventHandler
							  );


/**
	Sets this object active and then completes the request.
	@since			7.0
*/
	//##ModelId=3C9B094C0286
	void CompleteSelf();

private:	// Attributes

	/** A reference to the transaction look up table.
	*/
	//##ModelId=3C9B094C027C
	CNwssTransLookUpTable&			iTransTable;

	/** A reference to the actual session event handler
	*/
	//##ModelId=3C9B094C026A
	MNwssSessionEventHandler&		iSessionEventHandler;

	/** A reference to the actual transaction event handler/
	*/
	//##ModelId=3C9B094C0262
	MNwssTransactionEventHandler&	iTransEventHandler;

	};

#endif // __CNWSSWSPSESSIONEVENTFILTER_H__
