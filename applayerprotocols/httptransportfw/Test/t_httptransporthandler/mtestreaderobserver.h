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

#ifndef __MTESTREADEROBSERVER_H__
#define __MTESTREADEROBSERVER_H__

#include <e32std.h>

/**	@class		MTestReaderObserver
	The MTestReaderObserver class is the observer for the test socket reader
	object. It allows the test socket reader to notify its observer of when it
	has received the expected data or that the input stream has been closed.
	@componentInternal
	@see		CTestSocketReader
*/
class MTestReaderObserver
	{
public:

/**	@fn			DataReceived() =0
	Notifies the observer that the data has been received. The test socket 
	reader has received the  expected data.
	@componentInternal
*/
	virtual void DataReceived() =0;

/**	@fn			ReaderClosed() =0
	Notifies the observer that the reader is closed.
	@componentInternal
*/
	virtual void ReaderClosed(TInt aError) =0;

	};

#endif	// __MTESTREADEROBSERVER_H__
