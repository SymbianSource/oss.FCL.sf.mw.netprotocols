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

#ifndef __MSERVERDRIVEROBSERVER_H__
#define __MSERVERDRIVEROBSERVER_H__

#include <e32std.h>

/**	@class		MServerDriverObserver
	The MServerDriverObserver class is the observer for the server socket driver
	object. It allows the server socket driver object to notify its observer 
	that it is closed.
	@componentInternal
	@see		CServerSocketDriver
*/
class MServerDriverObserver
	{
public:	// methods

/**	@fn			ServerClosed() =0
	Notifies that the server socket driver is closed.
	@componentInternal
*/
	virtual void ServerClosed() =0;

	};

#endif	// __MSERVERDRIVEROBSERVER_H__
