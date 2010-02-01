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

#ifndef __MSOCKETDRIVEROBSERVER_H__
#define __MSOCKETDRIVEROBSERVER_H__

#include <e32std.h>

/**	@class		MSocketDriverObserver
	The MSocketDriverObserver class is the observer for the socket driver object.
	It allows the socket driver to notify its observer that it is closed.
	@componentInternal
*/
class MSocketDriverObserver
	{
public:

/**	@fn			SocketClosed() =0
	Notifies the socket driver observer that the socket driver is closed.
	@componentInternal
*/
	virtual void SocketClosed() =0;

	};

#endif	// __MSOCKETDRIVEROBSERVER_H__
