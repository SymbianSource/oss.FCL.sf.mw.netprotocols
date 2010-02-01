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
 @file MWspHeaderCodePages.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __MWSPHEADERCODEPAGES_H__
#define __MWSPHEADERCODEPAGES_H__

// System includes
#include <e32base.h>
#include <wsp/wsptypes.h>

class MWspHeaderCodePages
/**
@class MWspHeaderCodePages
@since			7.0
The MWspHeaderCodePages API provides an interface to a set of header code
pages as defined by the WSP Specification, July 2001. This capability 
associates a page number with a page name. The Server can negotiate the
use of header code page proposed by the Client by including the page codes
(without the page name) in the return negotiation.

The interface allows the set of header code pages to be accessed one by one.
The read function provides the caller with a copy of the header code page 
information. The set can be reset to the default value of the capability (as
defined by the WSP Specification), which is an empty set. A header code page 
can be added to the set.
@publishedAll
@deprecated
*/
	{
public:	// Methods

/**
	@fn				Start() const =0
	Intended Usage	:	Ensures that the first header code page in the set is 
						available for reading.
	@since			7.0
	@return			An error code. If there are no header code pages, then the 
					value returned is KErrNotFound. If there are header code 
					pages then the value KErrNone is returned.
	@post			The subsequent call to GetNext() is applied to the first
					header code page in the set.
 */
	virtual TInt Start() const =0;

/**
	@fn				GetNext(TUint8& aPageCode, TPtrC8& aPageName) const =0
	Intended Usage	:	Gets the next header code page in the set. If there are 
						no code pages left then KErrNotFound error code is 
						returned.
	@since			7.0
	@param			aPageCode	A output parameter reference for a byte with the
								extension code page number.
	@param			aPageName	A output parameter reference for a buffer with 
								the name of the header code page.
	@return			An error code. If there are no header code pages left, then 
					the value returned is KErrNotFound. If there are header code
					pages left then the value KErrNone is returned.
	@post			The next header code page in the set is available for reading.
 */
	virtual TInt GetNext(TUint8& aPageCode, TPtrC8& aPageName) const =0;

/**
	@fn				Reset() =0
	Intended Usage	:	Resets the capability to the default. The default is 
						defined in the WSP Specification, July 2001, as being an
						empty set of code pages.
	@since			7.0
	@post			The set of header code pages is empty.
 */
	virtual void Reset() =0;

/**
	@fn				AddHeaderCodePageL(TUint8 aPageCode, const TDesC8& aPageName) =0
	Intended Usage	:	Adds a header code page to the set.
	@since			7.0
	@leave			Leave code KErrNoMemory if the code page cannot be added to 
					the set.
	@param			aPageCode	A byte with the extension code page number.
	@param			aPageName	A buffer with the name of the header code page.
	@post			If no error has occured then the header code page has been 
					added.
 */
	virtual void AddHeaderCodePageL(TUint8 aPageCode, const TDesC8& aPageName) =0;

	};

#endif	// __MWSPHEADERCODEPAGES_H__
