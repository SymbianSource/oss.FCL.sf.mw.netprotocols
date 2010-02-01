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

#ifndef __MCONNECTIONPREFSPROVIDER_H__
#define __MCONNECTIONPREFSPROVIDER_H__

#include <e32std.h>

class RConnection;
class MSecurityPolicy;

class MConnectionPrefsProvider
/**
Interface class used as a callback observer to provide any connection 
preferences. This is used by the transport layer as callback to provide 
preferences such as allowing exsiting comms	connections to be used and 
security preferences.
@see	CHttpTransportLayer
*/
		{
public: // Methods

/**
	Callback function used by the transport layer that allows the client to 
	provide an existing	comms connection to use. The return value indicates 
	whether the client provided a connection or not. If the client does not 
	provide an existing connection, the paramters should not be set.
	@param		aSocketServerHandle	Handle to the existing socket server that
									is already connected
	@param		aConnectionPtr		Pointer to the comms connection that has 
									already been started
	@return		ETrue if an existing conection is provided or EFlase if no 
				connection is supplied.
*/
	virtual TBool SupplyCommsConnection( RConnection*& aConnectionPtr ) =0;
	virtual TBool SupplySocketServerHandle ( TInt& aSocketServerHandle ) =0;

/**
	Callback function used by the transport layer to set the comms connection 
	that has been setup back in the client, making the connection to the client.
	@param		aSocketServerHandle	Handle to the newly created socket server
	@param		aConnectionPtr		Pointer to the newly created (and connected)
									comms connection
*/
	virtual void SetCommsConnectionL( RConnection* aConnectionPtr ) =0;
	virtual void SetSocketServerHandleL ( TInt aSocketServerHandle ) =0;

/**
	Callback used by the transport layer to query a client to provide security 
	preferences.
	@param		aDialogPrompt	A boolean indicating whether clients should be 
								prompted or not.
	@param		aSecurityPolicy	The security policy provider, can be NULL if not
								provided.
*/
	virtual void GetSecurityPrefs(TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy) =0;

/**
	Callback function that indicates the socket shutdown mode. This method instructs the
	transport layer to shutdown	the socket immediately.
	@return		A boolean indicating whether the socket should be shutdown immediately or not.
*/
	virtual TBool ImmediateSocketShutdown() =0;
	
/**
	Callback function used by the transport layer to get the session ID. The result is either a non-negative integer, or an error code.
	
	@return		A negative error code or the Session ID. This returns KErrNotSupported
				if the implementation does not support session IDs
*/
	virtual TInt SessionId() =0;
	
/**
	Callback function used by the transport layer to get the maximum receive buffer size.
	@return    Returns the receive buffer size.
*/
	virtual TInt GetRecvBufferSize() = 0;

	};

#endif	// __MCONNECTIONPREFSPROVIDER_H__

