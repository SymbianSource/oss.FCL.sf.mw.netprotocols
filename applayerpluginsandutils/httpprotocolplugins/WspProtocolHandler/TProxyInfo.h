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

#ifndef __TPROXYINFO_H__
#define __TPROXYINFO_H__

#include <e32base.h>
#include <stringpool.h>
#include <wsp/wsptypes.h>

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;

/**	The TProxyInfo type contains the information that defines a connection to
	a proxy that is capable of establishing a WSP session. As a T-class the
	destructor does no cleanup. A Close() function has been supplied to do the
	cleanup - i.e. it closes the RStringF data members.
*/
//##ModelId=3C4C419F02FC
class TProxyInfo
	{
public:	// Methods

/**	@fn				operator==(const TProxyInfo& aProxyInfo) const
	Intended Usage	:	Equality operator.
	@since			7.0
	@return			A boolean value of ETrue if aProxyInfo matches this object.
*/
	//##ModelId=3C4C419F0374
	TBool operator==(const TProxyInfo& aProxyInfo) const;

/**	@fn				operator!=(const TProxyInfo& aProxyInfo) const
	Intended Usage	:	Inequality operator.
	@since			7.0
	@return			A boolean value of ETrue if aProxyInfo does not match this 
					object.
*/
	//##ModelId=3C4C419F036A
	inline TBool operator!=(const TProxyInfo& aProxyInfo) const;

/**	@fn				Close()
	Intended Usage	:	Cleans up the resource usage in this class. Closes the
						RStringFs for the proxy address and the service number.
	@since			7.0
	@post			The string members have been closed.
*/
	//##ModelId=3C4C419F0360
	inline void Close();

public:	// Attributes

	/** The proxy address. This class is not responsible for the cleanup of this
		string.
	*/
	//##ModelId=3C4C419F0358
	RStringF			iProxyAddress;

	/** The bearer type.
	*/
	//##ModelId=3C4C419F034E
	TWspBearer			iBearer;

	/** The local port number (only for IP bearer).
	*/
	//##ModelId=3C4C419F0344
	TUint16				iLocalPort;

	/** The remote port number (only for IP bearer).
	*/
	//##ModelId=3C4C419F033A
	TUint16				iRemotePort;

	/** The service number for the proxy (only for SMS bearer). This class is 
		not responsible for the cleanup of this string.
	*/
	//##ModelId=3C4C419F0326
	RStringF			iServiceCentreNumber;

	/** The Wsp Session Service type.
	*/
	//##ModelId=3C4C419F031C
	TWspSession			iSessionType;

	/** A flag indicating whether the WSP session should be secured.
	*/
	//##ModelId=3C4C419F0312
	TBool				iSecureConnection;

	};

#include "TProxyInfo.inl"

#endif	// __TPROXYINFO_H__
