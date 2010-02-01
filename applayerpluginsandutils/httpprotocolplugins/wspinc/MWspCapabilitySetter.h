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
 @file MWspCapabilitySetter.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCAPABILITYSETTER_H__
#define __MWSPCAPABILITYSETTER_H__

// System includes
#include <e32base.h>
#include <wsp/wsptypes.h>

// Types used for arguments in this file (e.g. TWspBearer) are from the Wap namespace
using namespace Wap;

class MWspCapabilitySetter
/** 
@class MWspCapabilitySetter
@since			7.0
The MWspCapabilitySetter API provides an interface to set the values for a 
set of capabilities as defined by the WSP Specification, July 2001.
@publishedAll
@deprecated
*/
	{
public:	// Methods

/**
	@fn				Reset(TWspCapability aCapability = EAllCapabilities) =0
	Intended Usage	:	Resets the capability specified by the aCapability. The 
						value to which the capability is reset is the default
						value defined in the WSP Specification, July 2001. In 
						the case of set capabilities (e.g. Alias Address), the 
						default is to have none.
	@since			7.0
	@param			aCapability	A enum specifying which capability to reset. The
								default value is EAllCapabilities.
	@post			The specified capability is reset to its default value.
 */
	virtual void Reset(TWspCapability aCapability = EAllCapabilities) =0;

/**
	@fn				AddAliasAddressL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress) =0
	Intended Usage	:	Adds an alias address to the set. This capability indicates
						alternate addresses that can be used to access the same
						entity instance in this current session. The addresses
						are listed in preferential order, with the most preferred
						at the start.
	@since			7.0
	@leave			Leave code KErrNoMemory if the capability cannot be added to 
					the set.
	@param			aBearer		The type of bearer network which this address 
								should be used with.
	@param			aPort		The destination port number. A value of zero
								indicates that no port number has been specified.
	@param			aAddress	A buffer with the bearer address to be used. The
								format of the address is bearer dependent.
	@post			The alias address has been added to the set.
 */
	virtual void AddAliasAddressL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress) =0;

/**
	@fn				AddExtendedMethodL(TUint8 aPDUType, const TDesC8& aMethodName) =0
	Intended Usage	:		Adds an extended method to the set. This capability 
							gives a set of methods, beyond those defined in 
							HTTP/1.1. 
	@since			7.0
	@leave			Leave code KErrNoMemory if the capability cannot be added to 
					the set.
	@param			aPDUType	A byte indicating the type of the PDU. The WSP
								Specification, July 2001, specifies that the
								range 0x50-0x5F is for methods that use a Get
								PDU format, and the range 0x70-7F is for methods
								that use a Post PDU format.
	@param			aMethodName	A buffer with the name of the method.
	@post			The extended method has been added to the set.
 */
	virtual void AddExtendedMethodL(TUint8 aPDUType, const TDesC8& aMethodName) =0;

/**
	@fn				AddHeaderCodePageL(TUint8 aPageCode, const TDesC8& aPageName) =0
	Intended Usage	:	Adds an extension header code page to the set.
	@since			7.0
	@leave			Leave code KErrNoMemory if the capability cannot be added to 
					the set.
	@param			aPageCode	A byte with the extension code page number.
	@param			aPageName	A buffer with the name of the header code page.
	@post			The Header Code Page has been added to the set.
 */
	virtual void AddHeaderCodePageL(TUint8 aPageCode, const TDesC8& aPageName) =0;

/**
	@fn				SetMethodMOR(TUint8 aMOR) =0
	Intended Usage	:	Set the Method MOR. The Method MOR defines the number of
						active/outstanding method invocations at one time during 
						the session.
	@since			7.0
	@param			aMOR	The Method MOR as an 8-bit positive number.
	@post			The Method MOR has been set to the given value.
 */
	virtual void SetMethodMOR(TUint8 aMOR) =0;

/**
	@fn				SetPushMOR(TUint8 aMOR) =0
	Intended Usage	:	Set the Push MOR. The Push MOR defines the number of
						active/outstanding push invocations at one time during 
						the session.
	@since			7.0
	@param			aMOR	The Push MOR as an 8-bit positive number.
	@post			The Push MOR has been set to the given value.
 */
	virtual void SetPushMOR(TUint8 aMOR) =0;

/**
	@fn				SetProtocolOptions(TUint8 aProtocolOptions) =0
	Intended Usage	:	Set the protocol options. The Protocol Options capability
						defines the optional service facilities and features for
						the session. The presence of an element indicates that 
						the use of the specified facility or feature is enabled. 
						The features and facilities described in the WSP 
						Specification, July 2001, are specified by flags in a 
						single byte. Additional flags will be specified in 
						subsequent bytes, but this is not supported currently.
	@since			7.0
	@leave			Leave code KErrNoMemory if the capability cannot be set.
	@param			aProtocolOptions	A byte containing the Protocol Options.
	@post			The Protocol Options have been set to the given value.
 */
	virtual void SetProtocolOptions(TUint8 aProtocolOptions) =0;

/**
	@fn				SetClientSDUSize(TUint32 aSize) =0
	Intended Usage	:	Set the Client SDU size. The Client SDU size is the size
						of the largest service data unit that may be sent to the
						Client during the session. A value of zero indicates that
						there is no limit to the SDU size.
	@since			7.0
	@param			aSize	The SDU size, in bytes, as a 32-bit positive number.
	@post			The Client SDU size has been set to the given value.
 */
	virtual void SetClientSDUSize(TUint32 aSize) =0;

/**
	@fn				SetServerSDUSize(TUint32 aSize) =0
	Intended Usage	:	Set the Server SDU size. The Server SDU size is the size
						of the largest service data unit that may be sent to the
						Server during the session. A value of zero indicates that
						there is no limit to the SDU size.
	@since			7.0
	@param			aSize	The SDU size, in bytes, as a 32-bit positive number.
	@post			The Server SDU size has been set to the given value.
 */
	virtual void SetServerSDUSize(TUint32 aSize) =0;

/**
	@fn				SetClientMessageSize(TUint32 aSize) =0
	Intended Usage	:	Set the Client Message size. The Client Message size is
						the size of the largest message that may be sent to the
						Client during the session. A message may consist of 
						multiple SDUs. A value of zero indicates that there is 
						no limit to the Message size.
	@since			7.0
	@param			aSize	The message size, in bytes, as a 32-bit positive number.
	@post			The Client Message size has been set to the given value.
 */
	virtual void SetClientMessageSize(TUint32 aSize) =0;

/**
	@fn				SetServerMessageSize(TUint32 aSize) =0
	Intended Usage	:	Set the Server Message size. The Server Message size is
						the size of the largest message that may be sent to the
						Server during the session. A message may consist of 
						multiple SDUs. A value of zero indicates that there is 
						no limit to the Message size.
	@since			7.0
	@param			aSize	The message size, in bytes, as a 32-bit positive number.
	@post			The Server Message size has been set to the given value.
 */
	virtual void SetServerMessageSize(TUint32 aSize) =0;

/**
	@fn				AddUnknownCapabilityL(const TDesC8& aIdentifier, const TDesC8& aValue) =0
	Intended Usage	:	Adds an unknown capability to the set. This is a 
						capability not defined by the WSP Specification, July 
						2001. The Identifier is encoded in the same way as the
						header field name - it uses the BNF rule for Field-Name, 
						as shown in 8.4.2.6. Well-known capabilities have their
						identifier encoded using a short-integer, and unknown 
						capabilities encode their identifier using a Null-
						terminated string. Any extra capabilities must be defined
						in such a way that an empty parameter value is illegal. 
						This allows the unknown capability to be rejected.
	@since			7.0
	@leave			Leave code KErrNoMemory if the capability cannot be added to 
					the set.
	@param			aIdentifier	A buffer with the identifier for the unknown
								capability.
	@param			aValue		A buffer with the parameters for the capability.
	@post			The unknown capability has been added to the set.
 */
	virtual void AddUnknownCapabilityL(const TDesC8& aIdentifier, const TDesC8& aValue) =0;

	};

#endif	// __MWSPCAPABILITYSETTER_H__