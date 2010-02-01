// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MTESTOBSERVER_H_
#define MTESTOBSERVER_H_

#include <e32std.h>

/**	@class		MTestObserver
	The MTestObserver class is the observer for the driver objects. It allows
	the drivers to notify the observer of when a test starts, an error occurs 
	and when a test completes. Also provides a logging mechanism.
	@componentInternal
	@see		CDriverBase
*/
class MTestObserver
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
	};
#endif /* MTESTOBSERVER_H_ */
