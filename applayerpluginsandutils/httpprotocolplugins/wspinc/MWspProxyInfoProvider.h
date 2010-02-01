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
 @file MWspProxyInfoProvider.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPPROXYINFOPROVIDER_H__
#define __MWSPPROXYINFOPROVIDER_H__

// System includes
#include <e32base.h>
#include <wsp/wsptypes.h>

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;


class MWspProxyInfoProvider
/**
@class MWspProxyInfoProvider
@since			7.0
The MWspProxyInfoProvider API provides an interface to the bearer connection
information that has been specified by the Client. 
@publishedAll
@deprecated
*/
	{
public:	// Methods

/** @fn				ProxyAddress() const =0
	Intended Usage	:	Identifies the peer with which the WSP session is to be
						established. The format of the this data is dependent on
						the type of bearer to be used in the connection. In the
						case of an SMS bearer, the Proxy peer is identified by a
						telephone number.
	@since			7.0
	@return			A const reference to a buffer containing the proxy address. 
*/	  
	virtual const TDesC8& ProxyAddress() const =0;

/** @fn				RemotePort() const =0
	Intended Usage	:	Identifies the port on the remote peer with which the 
						connection for the WSP session should be made. This is
						only relevant for certain bearers, e.g. IP bearer. A
						value of zero is invalid and is used to indicate that
						either a port has not been specified or it is not
						relevant to the chosen bearer
	@since			7.0
	@return			A word specifying the remote port number.
*/	  
	virtual TUint16 RemotePort() const =0;

/** @fn				LocalPort() const =0
	Intended Usage	:	Identifies the port on the local peer. A value of
						zero is invalid and is used to indicate that either
						a port has not been specified or it is not relevant
						to the chosen bearer
	@since			7.0
	@return			A word specifying the local port number
*/	  
	virtual TUint16 LocalPort() const =0;

/** @fn				Bearer() const =0
	Intended Usage	:	Identifies the type of bearer network which the Server
						address should be used with.
	@since			7.0
	@return			An enum specifying the type of bearer network.
*/	  
	virtual TWspBearer Bearer() const =0;

/**	@fn				ServiceCentreNumber() const =0
	Intended Usage	:	The telephone number for the SMS service centre. This is
						only relavent for an SMS bearer.
	@since			7.0
	@return			A reference to a buffer containing the telephone number of
					the service centre.
*/
	virtual const TDesC8& ServiceCentreNumber() const =0;

/**	@fn				WspSessionService() const =0
	Intended Usage	:	The type WSP Session Service set in the proxy info. This
						can be either ConnectionOriented or Connectionless.
	@since			7.0
	@return			An enum specifying the Wsp Session Service type
*/
	virtual TWspSession WspSessionService() const =0;

/** @fn				SecureConnection() const =0
	Intended Usage	:	Identifies the security to be used in the WSP session.
	@since			7.0
	@return			A flag that indicates whether the WSP session is to be secured
					with WTLS or not.
*/	  
	virtual TBool SecureConnection() const =0;
	};

#endif //	__MWSPPROXYINFOPROVIDER_H__
