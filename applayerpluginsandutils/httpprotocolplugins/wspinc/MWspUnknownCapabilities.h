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
 @file MWspUnknownCapabilities.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPUNKNOWNCAPABILITIES_H__
#define __MWSPUNKNOWNCAPABILITIES_H__

// System includes
#include <e32base.h>
#include <wsp/wsptypes.h>

class MWspUnknownCapabilities
/**
@class MWspUnknownCapabilities
@since			7.0
The MWspUnknownCapabilities API provides an interface to a set of unknown
capabilities as defined by the WSP Specification, July 2001. The Identifier
is encoded in the same way as the header field name - it uses the BNF rule 
for Field-Name, as shown in the WSP Specification, Section 8.4.2.6. Well-
known capabilities have their identifier encoded using a short-integer, and
unknown capabilities encode their identifier using a Null-terminated string.
Any extra capabilities must be defined in such a way that an empty parameter
value is illegal. This allows the unknown capability to be rejected.

The interface allows the set of unknown capabilities to be accessed one by 
one. The read function provides the caller with a copy of the capability 
information. The set can be reset to the default value of the capability (as
defined by the WSP Specification), which is an empty set. An unknown 
capability can be added to the set.
@publishedAll
@deprecated
*/
	{
public:	// Methods

/**
	@fn				Start() const =0
	Intended Usage	:	Ensures that the first unknown capability in the set is 
						available for reading.
	@since			7.0
	@return			An error code. If there are no unknown capabilities, then 
					the value returned is KErrNotFound. If there are unknown 
					capabilities then the value KErrNone is returned.
	@post			The subsequent call to GetNext() is applied to the first
					unknown capability in the set.
 */
	virtual TInt Start() const =0;

/**
	@fn				GetNext(TPtrC8& aIdentifier, TPtrC8& aValue) const =0
	Intended Usage	:	Gets the next unknown capability in the set. If there 
						are no unknown capabilities left then KErrNotFound error code is 
						returned.
	@since			7.0
	@param			aIdentifier	An output parameter reference for a buffer with
								the identifier for the unknown capability.
	@param			aValue		An output parameter reference for a buffer with
								the parameters for the capability.
	@return			An error code. If there are no unknown capabilities left, 
					then the value returned is KErrNotFound. If there are unknown
					capabilities left then the value KErrNone is returned.
	@post			The next unknown capability in the set is available for 
					reading.
 */
	virtual TInt GetNext(TPtrC8& aIdentifier, TPtrC8& aValue) const =0;

/**
	@fn				Reset() =0
	Intended Usage	:	Resets the capability to the default. The default is 
						defined in the WSP Specification, July 2001, as being an
						empty set of code pages.
	@since			7.0
	@post			The set of unknown capabilities is empty.
 */
	virtual void Reset() =0;

/**
	@fn				AddUnknownCapabilityL(const TDesC8& aIdentifier, const TDesC8& aValue) =0
	Intended Usage	:	Adds an unknown capability to the set.
	@since			7.0
	@leave			Leave code KErrNoMemory if the capability cannot be added to 
					the set.
	@param			aIdentifier	A buffer with the identifier for the unknown
								capability.
	@param			aValue		A buffer with the parameters for the capability.
	@post			If no error has occurred then the unknown capability has been 
					added.
 */
	virtual void AddUnknownCapabilityL(const TDesC8& aIdentifier, const TDesC8& aValue) =0;

	};

#endif	// __MWSPUNKNOWNCAPABILITIES_H__
