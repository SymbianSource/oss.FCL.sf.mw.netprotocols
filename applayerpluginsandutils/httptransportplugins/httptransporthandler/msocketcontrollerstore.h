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

#ifndef __MSOCKETCONTROLLERSTORE_H__
#define __MSOCKETCONTROLLERSTORE_H__

#include <e32std.h>

class CSocketController;

class MSocketControllerStore
/**	
	The MSocketControllerStore API provides support for socket controller objects
	to remove themselves from the socket controller store.
	@internalComponent
*/
	{
public:	// methods

/**	
	Remove the socket controller object from the store. The socket controller
	is removed from the store - the store no longer has ownership of the socket
	controller.
	@param		aOrphanedSocketController	The socket controller to be removed 
											from the store.
	@panic		EInvariantFalse	The orphaned socket controller is not in the 
								store.
*/
	virtual void SocketControllerShutdown(CSocketController& aOrphanedSocketController) =0;

	};

#endif	// __MSOCKETCONTROLLERSTORE_H__
