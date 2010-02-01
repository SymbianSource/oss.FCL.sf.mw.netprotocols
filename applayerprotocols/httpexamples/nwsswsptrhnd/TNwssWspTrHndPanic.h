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

/**
 @file TNwssWspTrHndPanic.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/
#ifndef __TNWSSWSPTRHNDPANIC_H__
#define __TNWSSWSPTRHNDPANIC_H__

// System includes
#include <e32std.h>

// Constants used in this file
_LIT(KStubPanicType, "NWSSWSPTH");


/**
	@since			7.0
	This class provides a trivial panic utility used throughout the WSP Transport Handler.
 */
//##ModelId=3C4C46CE0250
class TNwssWspTrHndPanic
	{
public: // type definitions

	/** Panic codes for the WSP TH, NWSS stack binding
	*/
	typedef enum
		{
		EServiceNotSupported = 1,
		ETransLUTOverflow = 2,
		EUnknownMethod = 3,
		EURIOverflow = 4,
		EWrongSessionType = 5,
		EWrongBearerType = 6,
		ECOPushNotSupported = 7,
		EConnectionLessNotSupported = 8,
		EUnknownWspEvent = 9,
		EEventOnUnknownTransaction = 10,
		EInvalidTransactionInLUT = 11,
		EFailedToGetSessionData = 12,
		EFailedToGetTransactionData = 13,
		EInternalCapCodecError = 14,
		ECapabilityOverrun = 15,
		EFailedToAcknowledgeTrans = 16,
		EFailedToReleaseTrans = 17,
		ELargeDataTransferNotSupported = 18,
		EFailedToAbortTrans = 19,
		ETransLUTLeakage = 20,
		EConnectedUponDestruction = 21,
		EFailedToOpenWspSession = 22,
		// panics that derive from specific NWSS wap stack error codes
		EBearerError         = 23,
		EPortAlreadyBound    = 24,
		ECannotOpenPort      = 25,
		ETooManyConnections  = 26,
		EBadConnection       = 27,
		EBufferTooSmall      = 28,
		EInvalidSession      = 29,
		EInvalidTransaction  = 30,
		EParameterError      = 31,
		ESessionNotConnected = 32,
		ENotSupported        = 33,
		EInvalidState        = 34,
		ESessionClosed       = 35,
		EMRUExceeded         = 36,
		EInvalidBufferSize   = 37,
		EDataNotAvailable    = 38,
		EErrorTimeout        = 39,
		//
		EMORExceeded		 = 40,
		EDeferedMethodFailed = 41,
		//
		EInsufficientCryptoStrength = 42,
		//
		EUnknownPanic = 999
		} TNwssWspTrHndPanicCode;

public: // methods

/**Panic the stub.  Uses the specified panic code.
	@since			7.0
	@param			aPanic		(in) The code with which the client is panicked.
 */
	//##ModelId=3C4C46CE0264
	inline static void Panic(TNwssWspTrHndPanicCode aPanic);

	};


// Include inline method definitions
#include "TNwssWspTrHndPanic.inl"

#endif // __TNWSSWSPTRHNDPANIC_H__
