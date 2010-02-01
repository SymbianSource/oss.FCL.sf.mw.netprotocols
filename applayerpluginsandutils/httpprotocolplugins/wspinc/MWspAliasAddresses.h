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
// MWspAliasAddress.h
// 
//

/**
 @file MWspAliasAddress.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPALIASADDRESS_H__
#define __MWSPALIASADDRESS_H__

// System includes
#include <e32base.h>
#include <wsp/wsptypes.h>

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;


class MWspAliasAddresses
/**
@class MWspAliasAddress
@since			7.0
The MWspAliasAddresses API provides an interface to a set of alias addresses
as defined by the WSP Specification, July 2001. This is an informational 
capability that indicates alternate addresses that can be used to access the
same entity instance in this current session. The addresses are listed in 
preferential order, with the most preferred at the start.

The interface allows the set of addresses to be accessed one by one. The 
read function provides the caller with a copy of the alias address 
information. The set of alias addresses can be reset to the default value
of the capability (as defined by the WSP Specification), which is an empty
set. An alias address can be added to the set - the preferred addresses should
be added first.
@publishedAll
@deprecated
*/
	{
public: // Methods

/** @fn				Start() const =0
	Intended Usage	:	Ensures that the first alias address in the set is 
						available for reading.
	@since			7.0
	@return			An error code. If there are no alias addresses, then the 
					value returned is KErrNotFound. If there are alias addresses
					then the value KErrNone is returned.
	@post			The subsequent call to GetNext() is applied to the first
					alias address in the set.
*/
	virtual TInt Start() const =0;

/** @fn				GetNext(TWspBearer& aBearer, TUint16& aPort, TPtrC8& aAddress) const =0
	Intended Usage	:	Gets the next alias address in the set. If there are no 
						addresses left then KErrNotFound error code is returned.
						The addresses are listed in preferential order, with the
						most preferred at the start.
	@since			7.0
	@param			aBearer		An output parameter reference for the type of 
								bearer network which this address should be used
								with.
	@param			aPort		An output parameter reference for the destination 
								port number. A value of zero indicates that no 
								port number has been specified.
	@param			aAddress	An output parameter reference for a buffer with
								the bearer address to be used. The format of the
								address is bearer dependent.
	@return			An error code. If there are no alias addresses left, then 
					the value returned is KErrNotFound. If there are alias 
					addresses left then the value KErrNone is returned.
	@post			The next alias address in the set is available for reading.
*/
	virtual TInt GetNext(TWspBearer& aBearer, TUint16& aPort, TPtrC8& aAddress) const =0;

/** @fn				Reset() =0
	Intended Usage	:	Resets the capability to the default. The default is 
						defined in the WSP Specification, July 2001, as being an
						empty set of addresses.
	@since			7.0
	@post			The set of alias addresses is empty.
*/
	virtual void Reset() =0;

/** @fn				AddAddressL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress) =0
	Intended Usage	:	Adds an alias address to the set. The addresses should
						be added in preferential order, with the most preferred
						at the start.
	@since			7.0
	@leave			Leave code KErrNoMemory if the address cannot be added to 
					the set.
	@param			aBearer		The type of bearer network which this address 
								should be used with.
	@param			aPort		The destination port number. A value of zero
								indicates that no port number has been specified.
	@param			aAddress	A buffer with the bearer address to be used. The
								format of the address is bearer dependent.
	@post			If no error has occurred then the alias address has been added.
*/
	virtual void AddAddressL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress) =0;

	};

#endif	// __MWSPALIASADDRESS_H__