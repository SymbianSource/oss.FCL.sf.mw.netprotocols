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

#ifndef __MSOCKETCONNECTORSTORE_H__
#define __MSOCKETCONNECTORSTORE_H__

#include <e32std.h>

class CSocketConnector;

class MSocketConnectorStore
/**	
	The MSocketConnectorStore API defines the callback API for the socket
	connector class (CSocketConnector) to its owner, the store.
	@internalComponent
	@see		CSocketConnector
*/
	{
public:	// methods

/**	
	Notifies the store that the socket connector has completed. The specified
	socket connector should be removed from the store. The socket connector will
	self delete after this call has completed as it has been orphaned. 
	@param		aOrphanedSocketConnector	The socket connector to be removed
											from the store.
	@panic		EInvariantFalse	The orphaned socket connector is not in the 
								store.
*/
	virtual void ConnectionCompleted(CSocketConnector& aOrphanedSocketConnector) =0;

	};

#endif	// __MSOCKETCONNECTORSTORE_H__

