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

#ifndef __MDRIVEROBSERVER_H__
#define __MDRIVEROBSERVER_H__

#include <e32std.h>

/**	@class		MDriverObserver
	The MDriverObserver class is the observer for the driver objects. It allows
	the drivers to notify the observer of when a test starts, an error occurs 
	and when a test completes. Also provides a logging mechanism.
	@componentInternal
	@see		CDriverBase
*/
class MDriverObserver
	{
public:	// methods

/**	@fn			NotifyError(TInt aError) =0
	Notifies the observer that an error has occured.
	@componentInternal
	@param		aError	The error code that has occured.
*/
	virtual void NotifyError(TInt aError) =0;

/**	@fn			NotifyStart() =0
	Notifies the observer that the test has started.
	@componentInternal
*/
	virtual void NotifyStart() =0;

/**	@fn			NotifyComplete() =0
	Notifies the observer that the test has completed.
	@componentInternal
*/
	virtual void NotifyComplete() =0;

/** @fn			Log(const TDesC& aComment) =0
	Logs comment to log file, but not the summary file.
	@componentInternal
	@param		aComment	The comment to be logged.
*/
	virtual void Log(const TDesC& aComment) =0;

/**	@fn			Dump(const TDesC8& aData) =0
	Dumps binary data to log file, but not the summary file.
	@componentInternal
	@param		aData	The data to be dumped.
*/
	virtual void Dump(const TDesC8& aData) =0;

	};

#endif	// __MDRIVEROBSERVER_H__
