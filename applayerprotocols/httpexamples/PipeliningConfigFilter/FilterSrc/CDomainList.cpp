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

#include "CDomainList.h"

// Maximum number of stored domains in list
const TUint KMaxDomainList = 50;

CDomainList* CDomainList::NewL(TInt aListSize)
	{
	CDomainList* self = new(ELeave) CDomainList(aListSize);
	return self;
	}

CDomainList::CDomainList(TInt aListSize)
	:iListSize(aListSize)
	{
	// If a list size is passed in as zero, use the default size
	if( iListSize<=0 )
		iListSize = KMaxDomainList;
	}
		
CDomainList::~CDomainList()
	{
	// Need to close all the RStringFs that are stored in domain list array
	for( TInt ii=0; ii<iDomainList.Count(); ++ii )
		{
		iDomainList[ii].Close();
		}
		
	// Close the array
	iDomainList.Reset();
	iDomainList.Close();
	}
	
TInt CDomainList::IsDomainInList(const RStringF& aDomain) const
	{
	// Do a reverse search through the domain list for the supplied domain
	// as its more likely that the most recently added domains will be
	// required most often
	for( TInt ii=Count()-1; ii>=0; --ii)
		{
		if( iDomainList[ii] == aDomain )
			return ii; // Found the domain, return its index
		}
	
	return KErrNotFound;
	}

void CDomainList::AddDomainToListL(RStringF aDomain)
	{
	const TInt domainPosition = IsDomainInList(aDomain); // Is domain already in list?
	const TInt domainCount = Count(); // How big is the domain list
	
	// First we check the domain we are adding is not already at the bottom
	// of the list and if we have any domains at all in the list
	// If the list is empty we want to simply add the domain.
	// If the domain being added is already the last one in the list then
	// we dont need to do anything.
	if( !(domainCount == (domainPosition+1) && domainCount!=0) )
		{
		// If the domain already exists in list remove it from its current
		// postion in the list so that it can get added to the bottom of the
		// list
		if( domainPosition != KErrNotFound )
			{
			iDomainList[domainPosition].Close(); // Owned by list, so close first
			iDomainList.Remove(domainPosition);
			}
		// If the domain is not already in the list, ensure that the list does
		// not exceed the specified maximum size. If the list is at its maximum,
		// we want to remove the first domain so that this domain can be
		// appended to the bottom of the list
		else if( domainCount >= iListSize )
			{
			iDomainList[0].Close(); // Close the RStringF as its owned by the list
			iDomainList.Remove(0);
			}
			
		// Add the domain to the bottom of the list
		User::LeaveIfError(iDomainList.Append(aDomain));
		}
	else
		{
		// The domain is required to be added to the list so close the
		// RStringF passed in
		aDomain.Close();
		}
	}
	
TInt CDomainList::Count() const
	{
	return iDomainList.Count();
	}
