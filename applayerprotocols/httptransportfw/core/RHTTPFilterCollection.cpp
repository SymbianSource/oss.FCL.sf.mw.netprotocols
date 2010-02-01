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

// System includes
#include <e32std.h>
#include <http/thttpfilterregistration.h>
#include <http/thttpfilteriterator.h>
#include <httperr.h>
#include <http/rhttpfiltercollection.h>// Class signature


// Local includes
#include "CHTTPSession.h"


/** Adds a filter to the session's filter queue.
	Intended Usage: This variant takes no status code or header. It is intended for filters
	that are only interested in events. For example session events.
	@pre The session is open and no transactions have been created yet
	@param aFilter The filter to add
	@param aEvent The event that triggers this filter (or all)
	@param aPosition The position of the filter in the
	queue. Standard values are documented in the documentation for
	the supplied filters, and are based on the standard defines in
	MHTTPFilter::TPositions
	@param aName  the name of the Filter to add
	@see See MHTTPFilter::TPositions */
EXPORT_C void RHTTPFilterCollection::AddFilterL(MHTTPFilter& aFilter, 
											   THTTPEvent aEvent,
											   TInt aPosition,
											   RStringF aName)
	{
	AddFilterL(aFilter, aEvent, RStringF(), KAnyStatusCode, aPosition, aName);
	}

/** Adds a filter to the session's filter queue.  

	Note: This function only adds filters; it doesn't replace or
	otherwise affect any pre-existing filters.

	@pre The session is open and no transactions have been created yet.
	@param aFilter The filter to add
	@param aEvent The event that triggers this filter (or all)
	@param aHeader The header whose presence triggers this filter
	(or KNullDesC for any) 
	@param aStatusCode The status code that triggers this filter
	(or -1 for all) 
	@param aPosition The position of the filter in the
	queue. Standard values are documented in the documentation for
	the supplied filters, and are based on the standard defines in
	MHTTPFilter::TPositions
	@param aName  the name of the Filter to add
	@see See MHTTPFilter::TPositions 
*/
EXPORT_C void RHTTPFilterCollection::AddFilterL(MHTTPFilter& aFilter,
												THTTPEvent aEvent,
												RStringF aHeader,
												TInt aStatusCode, 
												TInt aPosition,
												RStringF aName)
	{
	// You're only allowed to add filters if there aren't any transactions.
	__ASSERT_DEBUG(iImplementation->CanAddFilters(),HTTPPanic::Panic(HTTPPanic::EAddingFilterWithOutstandingTransactions));
	// You're only allowed to add filters between the protocol handler
	// and the client
	__ASSERT_DEBUG(aPosition >= MHTTPFilter::EProtocolHandler &&
				   aPosition <= MHTTPFilter::EClient,
				   HTTPPanic::Panic(HTTPPanic::EFilterInvalidPosition));

	// Copy the strings for use by this filter.
	aHeader.Copy();
	aName.Copy();
	
	//In case Leave happens when inserting filter in array that has reference of above variables,
	//their reference count needs to be decremented. So push onto CleanupStack.
	CleanupClosePushL(aHeader);
	CleanupClosePushL(aName);	
	
	THTTPFilterRegistration r(aFilter, aEvent, aHeader, aStatusCode, 
							  aPosition, aName, 
							  iImplementation->NextFilterHandle());
	User::LeaveIfError(iImplementation->FilterQueue().InsertInSignedKeyOrderAllowRepeats(r));
	
	CleanupStack::Pop(2); // aName, aHeader
	
	THTTPFilterHandle h;
	h.iValue = r.iHandle;
	aFilter.MHFLoad(Session(), h);
	}

/** @return ETrue if filters can currently be added or removed.
	Filters can only be added/removed if there are no transactions
	in existence on the session.
*/

EXPORT_C TBool RHTTPFilterCollection::CanChangeFilters() const
	{
	return iImplementation->CanAddFilters();
	}

/** Removes all filter registrations of the specified name.
	
	@param aFilter Filter name to remove.
 */
EXPORT_C void RHTTPFilterCollection::RemoveFilter(RStringF aFilter)
	{
	// Iterate through the filters and remove all the ones with the
	// given name. (We do this backwards, so that indexes in the
	// unsearched part of the array aren't affected by deletions)
	RArray<THTTPFilterRegistration>& filterQueue = 
		iImplementation->FilterQueue();
	TInt i = filterQueue.Count() -1;
	RStringPool strP = Session().StringPool();
	for (;i >= 0; --i)
		{
		if (filterQueue[i].iName == aFilter)
			{
			THTTPFilterHandle h;
			h.iValue = filterQueue[i].iHandle;
			filterQueue[i].iFilter->MHFUnload(Session(), h);
			strP.StringF(filterQueue[i].iHeader).Close();
			strP.StringF(filterQueue[i].iName).Close();
			filterQueue.Remove(i);
			}
		}
	}

/** An iterator to look at all filter registrations of a
	particular name. By default it will find all filters.
	
	@param aName The name of the filter to find. (All filters will
	be found if the default parameter is used) 
	@return A HTTP filter iterator.
*/

EXPORT_C THTTPFilterIterator RHTTPFilterCollection::Query(RStringF aName
														  /*= RStringF()*/)
	{
	THTTPFilterIterator r;
	r.iImplementation = iImplementation;
	r.iName = aName;
	r.First();
	return r;
	}

