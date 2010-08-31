// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MCOMMSINFOPROVIDER_H__
#define __MCOMMSINFOPROVIDER_H__

#include <e32std.h>
#include <in_sock.h>

class MSecurityPolicy;

class MCommsInfoProvider
/** 
	The MCommsInfoProvider API provides accessors to a connected socket server
	session and the protocol description for a particular protocol.
	@internalComponent
*/
	{
public:	// methods

/**	
	Accessor to the socket server session.
	@return		A reference to the connected socket server session.
*/
	virtual RSocketServ& SocketServer() =0;

/**
	Accessor to the protocol description.
	@return		A reference to the protocol description.
*/
	virtual TProtocolDesc& ProtocolDescription() =0;

/**
	Accessor to the comms connection.
	@return		A reference to the connected comms connection.
*/
	virtual RConnection& Connection() =0;

/**	
	Accessor to the security preferences.
	@param		aDialogPrompt Boolean indicating whether a secure dialog prompt should be used.
	@param		aSecurityPolicy The security policy, can be NULL if not provided
*/
	virtual void SecurityPreferences(TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy) =0;

/**	
	Indicates whether the connection is owned by the transport layer or not.
	@return		A boolean indicating whether the connection is owned.
*/
	virtual TBool OwnsConnection() =0;	
	
	/**
	Returns the protocal layer session ID. 
	
	@return		A negative error code or the Session ID. This returns KErrNotSupported
				if the implementation does not support session IDs
*/
	virtual TInt SessionId() =0;

	/**
	Returns ETrue if there is a connection, otherwise returns EFalse				
	*/
	virtual TBool HasConnection () =0;
	
	/**
	Starts a default RConnection
	*/
	virtual void StartDefaultCommsConnectionL () =0;
	
	/**
	 Returns the RHostResolver if available in cache.
	 */
	virtual void HostResolverFromCache(RHostResolver& aResolver) =0;
	
	/**
	 Add the RHostResolver to cache. If the adding fails then the RHostResolver will be
	 closed
	 */
	virtual void AddToHostResolverCache(RHostResolver& aResolver) =0;
	};

#endif	// __MCOMMSINFOPROVIDER_H__
