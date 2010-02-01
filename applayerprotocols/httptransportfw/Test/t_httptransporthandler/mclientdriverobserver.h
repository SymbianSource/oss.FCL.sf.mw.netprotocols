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

#ifndef __MCLIENTDRIVEROBSERVER_H__
#define __MCLIENTDRIVEROBSERVER_H__

#include <e32std.h>

/**	@class		MClientDriverObserver
	The MClientDriverObserver class is the observer for the client socket driver
	object. It allows the client socket driver object to notify its observer 
	that it is closed.
	@componentInternal
	@see		CClientSocketDriver
*/
class MClientDriverObserver
	{
public:	// methods

/**	@fn			ClientClosed() =0
	Notifies that the client socket driver is closed.
	@componentInternal
*/
	virtual void ClientClosed() =0;

	};

#endif	// __MCLIENTDRIVEROBSERVER_H__
