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

inline MSocketControllerFactory& CTcpTransportLayer::SocketControllerFactory()
/**
	Accessor for the socket controller factory.
	@internalComponent
	@return		The socket controller factory.
*/
	{
	return *this;
	}

inline MSocketConnectorStore& CTcpTransportLayer::SocketConnectorStore()
/**
	Accessor for the socket connector store.
	@internalComponent
	@return		The socket connector store.
*/
	{
	return *this;
	}

inline MSocketControllerStore& CTcpTransportLayer::SocketControllerStore()
/**
	Accessor for the socket controller store.
	@internalComponent
	@return		The socket controller store.
*/
	{
	return *this;
	}

inline MCommsInfoProvider& CTcpTransportLayer::CommsInfoProvider()
/**
	Accessor for the comms info provider.
	@internalComponent
	@return		The comms info provider.
*/
	{
	return *this;
	}
