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

/**
 @file MNwssWapServer.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/
#ifndef __MNWSSWAPSERVER_H__
#define __MNWSSWAPSERVER_H__

// System includes
#include <wapcli.h>

/**
	@since			7.0
	This interface provides access to the WAP Stack Server via a handle owned by the interface
	implementor.  It is used to narrow the API on the implementor for particular clients.
 */
//##ModelId=3C9B09470333
class MNwssWapServer
	{
public:	// Methods

/**Obtain access to the WAP Stack Server via the returned handle.
	@since			7.0
	@return			Reference to a WAP Stack Server session handle.  This must NOT be closed by the caller.			
 */
	//##ModelId=3C9B09470347
	virtual RWAPServ& WapStack() = 0;
	};

#endif	// __MNWSSWAPSERVER_H__
