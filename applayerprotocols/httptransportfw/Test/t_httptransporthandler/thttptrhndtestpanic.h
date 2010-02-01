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

#ifndef __THTTPTRHNDTESTPANIC_H__
#define __THTTPTRHNDTESTPANIC_H__

const TInt KHttpTrHndTestErrBase = -9999;

/**	Enumeration of the http transport handler test harness specific error codes.
	@componentInternal
*/
enum
	{
	KHttpTrHndTestErrUnexpectedCmd = KHttpTrHndTestErrBase
	};

/**	@class			THttpTrHndTestPanic
	The THttpTrHndTestPanic class provides the panic function and codes for 
	the http transport handler test harness.
	@componentInternal
*/
class THttpTrHndTestPanic
	{
public:	// enums

/**	Panic codes for the http transport handler test harness.
	@componentInternal
*/
	enum THttpTrHndTestPanicCode
		{
		EEmptyCmdStack					= 0,
		EBadCommandId					= 1,
		EBadSocketReaderState			= 2,
		EBadSocketWriterState			= 3,
		EReceivedBadData				= 4,
		EReceivedPipelinedData			= 5
		};

public:	// methods

	inline static void Panic(THttpTrHndTestPanicCode aPanic);

	};

#include "thttptrhndtestpanic.inl"

#endif	// __THTTPTRHNDTESTPANIC_H__
