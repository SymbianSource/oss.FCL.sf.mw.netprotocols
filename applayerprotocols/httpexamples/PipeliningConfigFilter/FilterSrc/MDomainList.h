// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MDOMAINLIST_H__
#define __MDOMAINLIST_H__

#include <e32base.h>
#include <stringpool.h>

// This interface class defines the API required for implementing a domain list
// for use with the Pipelining Config Filter in the HTTP Transport Framework.
class MDomainList
	{
public:	
	// This method must search the list implementation for the supplied RStringF
	// aDomain. This should return the index of the position of the domain in
	// the list or KErrNotFound if it does not exist.
	virtual TInt IsDomainInList(const RStringF& aDomain) const = 0;
	
	// This method must add the domain specified in aDomain to the list
	// implementation. This method must take ownership of the supplied RStringF
	// and is responsible for closing the string when it is no longer required.
	// This method should also not allow any duplicates to be added to the list.
	virtual void AddDomainToListL(RStringF aDomain) = 0;
	
	// This method must return the count of domains elements in the list.
	virtual TInt Count() const = 0;
	};
	
#endif // __MDOMAINLIST_H__
