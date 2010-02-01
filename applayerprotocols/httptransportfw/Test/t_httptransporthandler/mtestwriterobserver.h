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

#ifndef __MTESTWRITEROBSERVER_H__
#define __MTESTWRITEROBSERVER_H__

#include <e32std.h>

/**	@class		MTestWriterObserver
	The MTestWriterObserver class is the observer for the test socket writer 
	object. It allows the test socket writer to notify its observer of when it
	has sent the specified data or that the output stream has been closed.
	@componentInternal
	@see		CTestSocketWriter
*/
class MTestWriterObserver
	{
public:

/**	@fn			DataSent() =0
	Notifies the observer that the data has been sent. The test socket writer 
	has sent the specified data.
	@componentInternal
*/
	virtual void DataSent() =0;

/**	@fn			WriterClosed() =0
	Notifies the observer that the writer is closed.
	@componentInternal
*/
	virtual void WriterClosed(TInt aError) =0;

	};

#endif	// __MTESTWRITEROBSERVER_H__
