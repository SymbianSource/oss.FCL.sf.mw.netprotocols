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
 @file MWspCapabilityViewer.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPCAPABILITYVIEWER_H__
#define __MWSPCAPABILITYVIEWER_H__

// System includes
#include <e32base.h>

// Forward declarations
class MWspAliasAddresses;
class MWspExtendedMethods;
class MWspHeaderCodePages;
class MWspUnknownCapabilities;


class MWspCapabilityViewer
/**
@class MWspCapabilityViewer
@since			7.0
The MWspCapabiltyViewer API provides an interface to view a set of capabilities
as defined by the WSP Specification, July 2001.
@publishedAll
@deprecated
*/
	{
public:	// Methods

/**
	@fn				GetAliasAddresses() const =0
	Intended Usage	:	The set of Alias Addresses. This capability indicates
						alternate addresses that can be used to access the same
						entity instance in this current session. The addresses
						are listed in preferential order, with the most preferred
						at the start.
	@since			7.0
	@return			A const reference to a MWspAliasAddresses object. 
 */
	virtual const MWspAliasAddresses& GetAliasAddresses() const =0;

/**
	@fn				GetExtendedMethods() const =0
	Intended Usage	:	The set of extended methods. This capability gives a
						set of methods, beyond those defined in HTTP/1.1. 
	@since			7.0
	@return			A const reference to a MWspExtendedMethods object. 
 */
	virtual const MWspExtendedMethods& GetExtendedMethods() const =0;

/**
	@fn				GetHeaderCodePages() const =0
	Intended Usage	:	The set of extension header code pages. 
	@since			7.0
	@return			A const reference to a MWspHeaderCodePages object. 
 */
	virtual const MWspHeaderCodePages& GetHeaderCodePages() const =0;

/**
	@fn				GetMethodMOR() const =0
	Intended Usage	:	The Method MOR defines the number of active/outstanding
						method invocations at one time during the session.
	@since			7.0
	@return			The Method MOR size as an unsigned 8-bit number.
 */
	virtual TUint8 GetMethodMOR() const =0;

/**
	@fn				GetPushMOR() const =0
	Intended Usage	:	The Push MOR defines the number of active/outstanding
						push invocations at one time during the session.
	@since			7.0
	@return			The Push MOR size as an unsigned 8-bit number.
 */
	virtual TUint8 GetPushMOR() const =0;

/**
	@fn				GetProtocolOptions() const =0
	Intended Usage	:	The Protocol Options capability defines the optional 
						service facilities and features for the session. The
						presence of an element indicates that the use of the 
						specified facility or feature is enabled. The features
						and facilities described in the WSP Specification, July
						2001, are specified by flags in a single byte. 
						Additional flags will be specified in subsequent bytes,
						but this is not supported currently.

	@since			7.0
	@return			The Protocol Options encoded within a byte.
					Options.
					
 */
	virtual TUint8 GetProtocolOptions() const =0;

/**
	@fn				GetClientSDUSize() const =0
	Intended Usage	:	The Client SDU size is the size of the largest service
						data unit that may be sent to the Client during the 
						session. A value of zero indicates that there is no limit
						to the SDU size.
	@since			7.0
	@return			The SDU size, in bytes, as an unsigned 32-bit number.
 */
	virtual TUint32 GetClientSDUSize() const =0;

/**
	@fn				GetServerSDUSize() const =0
	Intended Usage	:	The Server SDU size is the size of the largest service
						data unit that may be sent to the Server during the 
						session. A value of zero indicates that there is no limit
						to the SDU size.
	@since			7.0
	@return			The SDU size, in bytes, as an unsigned 32-bit number.
 */
	virtual TUint32 GetServerSDUSize() const =0;

/**
	@fn				GetClientMessageSize() const =0
	Intended Usage	:	The Client Message size is the size of the largest 
						message that may be sent to the Client during the 
						session. A message may consist of multiple SDUs. A 
						value of zero indicates that there is no limit to the 
						Message size.
	@since			7.0
	@return			The Message size, in bytes, as an unsigned 32-bit number.
 */
	virtual TUint32 GetClientMessageSize() const =0;

/**
	@fn				GetServerMessageSize() const =0
	Intended Usage	:	The Server Message size is the size of the largest 
						message that may be sent to the Server during the 
						session. A message may consist of multiple SDUs. A 
						value of zero indicates that there is no limit to the 
						Message size.
	@since			7.0
	@return			The Message size, in bytes, as an unsigned 32-bit number.
 */
	virtual TUint32 GetServerMessageSize() const =0;

/**
	@fn				GetUnknownCapabilities() const =0
	Intended Usage	:	A set of capabilities not defined by the WSP 
						Specification, July 2001. The Identifier is encoded in
						the same way as the header field name - it uses the BNF
						rule for Field-Name, as shown in 8.4.2.6. Well-known 
						capabilities have their identifier encoded using a short-
						integer, and unknown capabilities encode their identifier
						using a Null-terminated string. Any extra capabilities 
						must be defined in such a way that an empty parameter 
						value is illegal. This allows the unknown capability to 
						be rejected.
	@since			7.0
	@return			A const reference to a MWspUknownCapabilities object. 
 */
	virtual const MWspUnknownCapabilities& GetUnknownCapabilities() const =0;

	};

#endif	// __MWSPCAPABILITYVIEWER_H__