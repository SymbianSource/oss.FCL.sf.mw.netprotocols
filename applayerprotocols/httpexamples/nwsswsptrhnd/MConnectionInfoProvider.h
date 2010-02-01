// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file MConnectionInfoProvider.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MCONNECTIONINFOPROVIDER_H__
#define __MCONNECTIONINFOPROVIDER_H__

// System includes
#include <wsp/mwspproxyinfoprovider.h>
#include <wsp/mwspcapabilityprovider.h>
#include <wsp/mwspsessionheadersprovider.h>


/**
	@since			7.0
	This interface defines methods to obtain further interfaces that supply connection-related
	information.  This information comes in three varieties:
	* the WAP Proxy which is to be connected to;
	* the Client's requested capabilities and the Server's negotiated capabilities;
	* the Client's session request header, and the Server's session response header.
 */
//##ModelId=3C9B094C007D
class MConnectionInfoProvider
	{
public:	// Methods

/**Obtain the client's provider of proxy information
	@since			7.0
 */
	//##ModelId=3C9B094C00A6
	virtual MWspProxyInfoProvider& ProxyInfoProvider() const = 0;

/**Obtain the client's provider of capabilities information
	@since			7.0
 */
	//##ModelId=3C9B094C00A5
	virtual MWspCapabilityProvider& CapabilityProvider() const = 0;

/**Obtain the client's provider of sesion headers
	@since			7.0
 */
	//##ModelId=3C9B094C009B
	virtual MWspSessionHeadersProvider& SessionHeadersProvider() const = 0;
	};

#endif	// __MCONNECTIONINFOPROVIDER_H__
