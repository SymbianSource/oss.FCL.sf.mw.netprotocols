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
#include <http/cecomfilter.h>
#include <httperr.h>
#include <http/tfilterconfigurationiter.h>// Class signature

// Local includes
#include "CHTTPSession.h"

// Standard constructor
TFilterConfigurationIterator::TFilterConfigurationIterator(CHTTPSession* aSession)
	:	iSession(aSession),
		iFilterInfoList(aSession->iFilterInfoList),
		iCurrentFilterIndex(0),
		iFilterCount(0)
	{
	// Count the number of implicit and explicit filters
	const TInt fullListCount = iFilterInfoList.Count();
	for( TInt i=0; i<fullListCount; ++i )
		{
		switch(iFilterInfoList[i]->iCategory)
			{
			case TSessionFilterInfo::EImplicit:
			case TSessionFilterInfo::EExplicit:
				++iFilterCount;
			default:
				break;
			}
		}
	}

// Standard destructor
TFilterConfigurationIterator::~TFilterConfigurationIterator()
	{
	}

// Set current filter as the first one in the list
EXPORT_C TInt TFilterConfigurationIterator::First()
	{
	if( iFilterCount == 0 )
		return KErrNotFound;
	else
		{
		// Find the first implicit or explicit filter
		iCurrentFilterIndex = 0;
		if( iFilterInfoList[iCurrentFilterIndex]->iCategory == TSessionFilterInfo::EMandatory )
			return Next();
		}

	return KErrNone;
	}

// Set the current filter to the next filter in the list
EXPORT_C TInt TFilterConfigurationIterator::Next()
	{
	__ASSERT_ALWAYS(iFilterCount, HTTPPanic::Panic(HTTPPanic::EInvalidFilterHandle));

	// Check if the is at the end of the list
	if( iCurrentFilterIndex == iFilterCount - 1 )
		{
		return KErrNotFound;
		}
	else
		{
		// If the next filter is a mandatory filter then move on to the next filter
		if( iFilterInfoList[iCurrentFilterIndex]->iCategory == TSessionFilterInfo::EMandatory )
			return Next();
		++iCurrentFilterIndex;
		}

	return KErrNone;
	}

// Returns a boolean indicating whether the current filter is the first in the list
EXPORT_C TBool TFilterConfigurationIterator::AtStart()
	{
	__ASSERT_ALWAYS(iFilterCount, HTTPPanic::Panic(HTTPPanic::EInvalidFilterHandle));
	// Store the original list index
	TInt originalListIndex = iCurrentFilterIndex;
	// This will set the list index (iCurrentFilter) to the first filter
	First();
	if( originalListIndex == iCurrentFilterIndex )
		return ETrue;
	else
		{
		// Index has changed so revert back to the original index value
		iCurrentFilterIndex = originalListIndex;
		return EFalse;
		}
	}

// Returns a boolean indicating whether the current filter is the last in the list
EXPORT_C TBool TFilterConfigurationIterator::AtEnd()
	{
	__ASSERT_ALWAYS(iFilterCount, HTTPPanic::Panic(HTTPPanic::EInvalidFilterHandle));
	// Store the original list index
	TInt originalListIndex = iCurrentFilterIndex;

	// See if the a next filter exists
	if( Next() == KErrNotFound )
		return ETrue;
	else
		{
		// Index has changed so revert back to the original index value
		iCurrentFilterIndex = originalListIndex;
		return EFalse;
		}
	}

// Set the current filter as the first filter that matches the data type passed in
EXPORT_C TInt TFilterConfigurationIterator::FindByDataType(const TDesC8& aDataType)
	{
	// Store the original filter
	TInt originalListIndex = iCurrentFilterIndex;

	// Start from the start of the list
	if( First() == KErrNotFound )
		return KErrNotFound;
	
	do
		{
		// Loop through all the filters until a match is found
		if( iFilterInfoList[iCurrentFilterIndex]->iFilterInfo->DataType().CompareF(aDataType) == 0 )
			return KErrNone;
		} 
	while( Next() != KErrNotFound );
	// If no match then set current filter back to the original filter
	iCurrentFilterIndex = originalListIndex;
	return KErrNotFound;
	}

// Get the information on the current filter
EXPORT_C const TFilterInformation TFilterConfigurationIterator::CurrentFilterInformation() const
	{
	// Panic if the current filter has not been set using the First() method
	__ASSERT_ALWAYS(iFilterCount, HTTPPanic::Panic(HTTPPanic::EInvalidFilterHandle));

	// Determine what the filter category is
	TFilterInformation::TFilterCategory currentFilterCategory;
	if( iFilterInfoList[iCurrentFilterIndex]->iCategory == TSessionFilterInfo::EImplicit)
		currentFilterCategory = TFilterInformation::EImplicitFilter;
	else
		currentFilterCategory = TFilterInformation::EExplicitFilter;
	
	// Determine if the filter has been loaded yet
	TBool isFilterLoaded = EFalse;
	if( iFilterInfoList[iCurrentFilterIndex]->iFilterPlugin != NULL )
		isFilterLoaded = ETrue;

	// Create a current filter information class containing the details of the filter
	TFilterInformation currentFilterInformation(
		iFilterInfoList[iCurrentFilterIndex]->iFilterInfo->DisplayName(),
		iFilterInfoList[iCurrentFilterIndex]->iFilterInfo->DataType(),
		iFilterInfoList[iCurrentFilterIndex]->iFilterInfo->Version(),
		iFilterInfoList[iCurrentFilterIndex]->iFilterInfo->ImplementationUid(),
		currentFilterCategory,
		isFilterLoaded);

	return currentFilterInformation;
	}

// Install the current filter
EXPORT_C TInt TFilterConfigurationIterator::InstallCurrentFilter() const
	{
	__ASSERT_ALWAYS(iFilterCount, HTTPPanic::Panic(HTTPPanic::EInvalidFilterHandle));
	if( iFilterInfoList[iCurrentFilterIndex]->iFilterPlugin != NULL )
		return KErrAlreadyExists;
	else
		{
		// Get the UID of the current filter
		TUid filterUid = iFilterInfoList[iCurrentFilterIndex]->iFilterInfo->ImplementationUid();

		// Install the filter through ECom
		TRAPD(err, iFilterInfoList[iCurrentFilterIndex]->iFilterPlugin = 
						CEComFilter::InstallFilterL(iSession->Handle(), filterUid));
		return err;
		}
	}

// Uninstall the current filter
EXPORT_C TInt TFilterConfigurationIterator::UninstallCurrentFilter() const
	{
	__ASSERT_ALWAYS(iFilterCount, HTTPPanic::Panic(HTTPPanic::EInvalidFilterHandle));
	if( iFilterInfoList[iCurrentFilterIndex]->iFilterPlugin == NULL )
		return KErrNotFound;
	else
		{
		// Delete the implementation and the filter should uninstall itself
		delete iFilterInfoList[iCurrentFilterIndex]->iFilterPlugin;
		iFilterInfoList[iCurrentFilterIndex]->iFilterPlugin = NULL;
		return KErrNone;
		}
	}
