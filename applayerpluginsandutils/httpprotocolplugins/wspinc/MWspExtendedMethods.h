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
 @file MWspExtendedMethods.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPEXTENDEDMETHODS_H__
#define __MWSPEXTENDEDMETHODS_H__

// System includes
#include <e32base.h>
#include <wsp/wsptypes.h>


class MWspExtendedMethods
/**
@class MWspExtendedMethods
@since			7.0
The MWspExtendedMethods API provides an interface to a set of extended 
methods as defined by the WSP Specification, July 2001. This capability gives
a set of methods, beyond those defined in HTTP/1.1 to be used in the WSP 
Session. Each method has a PDU type, which is assigned by the Client, and 
a PDU name. The WSP Specification, July 2001, specifies for the PDU type 
that the range 0x50-0x5F indicates a method that use a Get PDU format, and
the range 0x70-7F indicates a method that use a Post PDU format.

The interface allows the set of methods to be accessed one by one. The read 
function provides the caller with a copy of the extend method information. 
The set can be reset to the default value of the capability (as defined by
the WSP Specification), which is an empty set. An extended method can be 
added to the set, the preferred addresses should be added first.
@publishedAll
@deprecated
*/
	{
public:

/** @fn				Start() const =0
	Intended Usage	:	Ensures that the first extended method in the set is 
						available for reading.
	@since			7.0
	@return			An error code. If there are no extended methods, then the 
					value returned is KErrNotFound. If there are extended methods
					then the value KErrNone is returned.
	@post			The subsequent call to GetNext() is applied to the first
					extended method in the set.
*/
	virtual TInt Start() const =0;

/** @fn				GetNext(TUint8& aPDUType, TPtrC8& aMethodName) const =0
	Intended Usage	:	Gets the next extended method in the set. If there are 
						no methods left then KErrNotFound error code is returned.
	@since			7.0
	@param			aPDUType	An output parameter reference for a byte 
								indicating the type of the PDU. The WSP 
								Specification, July 2001, specifies that the 
								range 0x50-0x5F is for methods that use a Get
								PDU format, and the range 0x70-7F is for methods
								that use a Post PDU format.
	@param			aMethodName	An output reference for a buffer with the name 
								of the method.
	@return			An error code. If there are no extended methods left, then 
					the value returned is KErrNotFound. If there are extended 
					methods left then the value KErrNone is returned.
	@post			The next extended method in the set is available for reading.
*/
	virtual TInt GetNext(TUint8& aPDUType, TPtrC8& aMethodName) const =0;

/** @fn				Reset() =0
	Intended Usage	:	Resets the capability to the default. The default is 
						defined in the WSP Specification, July 2001, as being an
						empty set of extended methods.
	@since			7.0
	@post			The set of extended methods is empty.
*/
	virtual void Reset() =0;

/** @fn				AddExtendedMethodL(TUint8 aPDUType, const TDesC8& aMethodName) =0
	Intended Usage	:	Adds an extended method to the set.
	@since			7.0
	@leave			Leave code KErrNoMemory if the method cannot be added to 
					the set.
	@param			aPDUType	A byte indicating the type of the PDU. The WSP
								Specification, July 2001, specifies that the
								range 0x50-0x5F is for methods that use a Get
								PDU format, and the range 0x70-7F is for methods
								that use a Post PDU format.
	@param			aMethodName	A buffer with the name of the method.
	@post			If no error has occurred then the extended method has been 
					added.
*/
	virtual void AddExtendedMethodL(TUint8 aPDUType, const TDesC8& aMethodName) =0;

	};

#endif	// __MWSPEXTENDEDMETHODS_H__