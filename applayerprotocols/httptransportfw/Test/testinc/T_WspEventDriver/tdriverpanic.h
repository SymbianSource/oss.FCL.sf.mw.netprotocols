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

#ifndef __TDRIVERPANIC_H__
#define __TDRIVERPANIC_H__

// System includes
//
#include <e32std.h>

/** Definition of class 'TDriverPanic'.  This class provides a trivial panic utility used throughout the WSP Driver.
*/
_LIT(KDriverPanicType, "WSPPHDriver");
class TDriverPanic
	{
public: // type definitions

	/** Panic codes for the WSP PH Driver
	*/
	typedef enum
		{
		EWspEventDriverNotInitialised = 1,
		EBadTHTTPEventInScript = 2,
		ENullPointer = 3,
		ECannotCreateItemInStore = 4,
		EItemNotFoundInStore = 5,
		EUnrecognisedMethod = 6,
		EInvalidInteger = 7,
		ETimeDescriptor = 8,
		EBadTHdrValTypeInScript = 9,
		EParameterMissingInScript = 10,
		EWrongPropertyType = 11,
		EUnsupportedHdrValueType = 12,
		EBadIntegerValueInScript = 13,
		EInvalidScriptFilename = 14,
		EMemoryAllocError = 15
	} TDriverPanicCode;

public: // methods

	/** Panic the Driver.  Uses the specified panic code.
	*/
	inline static void Panic(TDriverPanicCode aPanic);

	};

#include "t_wspeventdriver/TDriverPanic.inl"

#endif // __TDRIVERPANIC_H__