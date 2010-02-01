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

#ifndef __WSPPANIC_H__
#define __WSPPANIC_H__

#include <e32std.h>

// @todo docing

_LIT(KWspPanicCategory, "WSP-CO-PH");
enum TWspPanic
	{
	KWspPanicDisconnectNotDone					= 1,
	KWspPanicBadClientMessageSize				= 2,
	KWspPanicBadServerMessageSize				= 3,
	KWspPanicBadMethodMOR						= 4,
	KWspPanicBadConenctionTimeout				= 5,

	KWspPanicMethodsStillOutstanding			= 6,
	KWspPanicTransportHandlerAlreadyExists		= 7,
	KWspPanicTransportHandlerDoesNotExist		= 8,
	KWspPanicNotExpectingMoreResponseData		= 9,
	KWspPanicRxDataObjectNotReset				= 10,

	KWspPanicResponseDataNotReleased			= 11,
	KWspPanicNoResponseDataReceived				= 12,
	KWspPanicMethodAlreadyActive				= 13,
	KWspPanicLDTNotSuppoted						= 14,
	KWspPanicNotExpectingMoreRequestData		= 15,

	KWspPanicBadMethodState						= 16,
	KWspPanicTxDataObjectNotReset				= 17,
	KWspPanicNoRequestDataReceived				= 18,
	KWspPanicBadMethodType						= 19,
	KWspPanicBadProxyAddressProperty			= 20,

	KWspPanicBadServiceNumberProperty			= 21,
	KWspPanicBadWspSessionTypeProperty			= 22,
	KWspPanicBadBearerProperty					= 23,
	KWspPanicBadSecurityProperty				= 24,
	KWspPanicBadPortProperty					= 25,

	KWspPanicCommDBContainsBadData				= 26,
	KWspPanicBadTrailerHeader					= 27,
	KWspPanicNoTrailerHeaders					= 28,
	KWspPanicBadBearerType						= 29,
	KWspPanicSessionNotInValidState				= 30,

	KWspPanicIllegalSessionState				= 31,
	KWspPanicNoMoreMessageSpace					= 32,
	KWspPanicTxDataInInvalidState				= 33
	};

GLREF_C void Panic(TWspPanic aPanic);

#endif //  __WSPPANIC_H__
