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
 @file TFilterConfigurationIter.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __TFILTERCONFIGURATIONITER_H__
#define __TFILTERCONFIGURATIONITER_H__

// System includes
#include <e32std.h>

// User includes
#include <http/tfilterinformation.h>

// Forward declarations
class CHTTPSession;
class TSessionFilterInfo;
class CFilterConfigurationIterator_UnitTestContext;


//##ModelId=3C4C0F45009C
class TFilterConfigurationIterator
/**
This class can only be created and destroyed by the session. It is supplied to the
client, if the client wishes to configure the filter that are installed or install
filters that must be installed explicitly. The API of the class allows the client to
navigate, install, uninstall and query the filter list. The filters visible in this
list are only the implicit and explicit filters declared as plugins. This iterator MUST
be initialised using the First() method and the return error code checked before any
other operations are called.
@publishedAll
@released
*/
	{
public: // Methods
	/**
	@fn				First()
	Intended Usage:	This method sets the current filter to the first filter in the list.
					This method must be used initially to set the
					current filter and the returned error code must be noted prior to
					using any of the methods that query or modify the current filter.
	@return			KErrNone if the first filter is found, KErrNotFound if the filter 
					can't be found or no filters exist
	@pre 			Object is constructed
	@post			Current filter is set to the first filter in the list
	*/
	//##ModelId=3C4C0F45010C
	IMPORT_C TInt First();

	/**
	@fn				Next()
	Intended Usage:	This method sets the current filter to the next filter in the list.
	@return			KErrNone if the next filter is found, KErrNotFound if the filter 
					can't be found, or no filters exist, or the current filter is at the
					end of the list.
	@pre 			Object is constructed
	@post			Current filter is set to the next filter in the list
	*/
	//##ModelId=3C4C0F45010B
	IMPORT_C TInt Next();

	/**
	@fn				AtStart()
	Intended Usage: This method indicates whether the current filter is the first filter in
					the list. This method will panic if the iterator has not been initialised
					using the First() method.
	@return			ETrue if the current filter is the first in the list
	@pre 			The iterator has been initialised using the First() method
	*/
	//##ModelId=3C4C0F45010A
	IMPORT_C TBool AtStart();

	/**
	@fn				AtEnd()
	Intended Usage: This method indicates whether the current filter is the last filter in
					the list. This method will panic if the iterator has not been initialised
					using the First() method.
	@return			ETrue if the current filter is the last in the list
	@pre 			The iterator has been initialised using the First() method
	*/
	//##ModelId=3C4C0F450101
	IMPORT_C TBool AtEnd();

	/**
	@fn				FindByDataType(const TDesC8& aDataType)
	Intended Usage:	This method find the first filter in the list that has a matching data
					type that is passed in. The method will set the current filter to the
					filter that matches the data type or if not found the current filter
					will not change. This method will always begin its search
					from the beginning of the list.
	@param			aDataType An 8-bit descriptor containing the data type to match the 
					filter plug-in with
	@return			KErrNone is the filter plug-in is found, KErrNotFound if not found
	@pre 			Object is constructed
	@post			The current filter is set to the filter that matches the data type
	*/
	//##ModelId=3C4C0F4500F8
	IMPORT_C TInt FindByDataType(const TDesC8& aDataType);

	/**
	@fn				CurrentFilterInformation() const
	Intended Usage:	This method will return the information of the current filter. The
					information is provided in a TFilterInformation structure containing the
					following information:
					1) Display name
					2) Data type
					3) Version
					4) UID
					5) Category
					6) Install status
					This method will panic if the iterator has not been initialised using the
					First() method
	@return			A struct containing the current filter information
	@pre 			The iterator has been initialised using the First() method
	@see			TFilterInformation
	*/
	//##ModelId=3C4C0F4500F7
	IMPORT_C const TFilterInformation CurrentFilterInformation() const;

	/**
	@fn				InstallCurrentFilter() const
	Intended Usage:	This method will install the current filter. This method will panic if a
					current filter is not set.
	@return			KErrNone if the filter installed, KErrAlreadyExists if the filter has
					already been installed, KErrNotFound if the filter cannot be found,
					KErrNotConnected if the ECom plug-in architecture is not connected,
					KErrNoMemory is the filter is not installed due to low memory, or any
					other system-wide error codes.
	@pre 			A current filter has been set and is not already installed
	@post			The filter is installed
	*/
	//##ModelId=3C4C0F4500F6
	IMPORT_C TInt InstallCurrentFilter() const;

	/**
	@fn				UninstallCurrentFilter() const
	Intended Usage:	This method will uninstall the current filter. This method will panic if
					a current filter is not set.
	@return			KErrNone is the filter uninstalled correctly, KErrNotFound if the filter
					has not already been installed
	@pre 			A current filter has been set and has already been installed
	@post			The filter is uninstalled
	*/
	//##ModelId=3C4C0F4500EF
	IMPORT_C TInt UninstallCurrentFilter() const;

private: // Methods
	// Friend class
	friend class CHTTPSession;

	// Friend class for testing purposes
	friend class CFilterConfigurationIterator_UnitTestContext;

	// Standard default constructor, this is private as it is designed so that only
	// the CHTTPSession object that create it.
	//##ModelId=3C4C0F4500ED
	TFilterConfigurationIterator(CHTTPSession* aSession);

	// Standard destructor, this is private as it is designed so that only the
	// CHTTPSession object can delete it.
	//##ModelId=3C4C0F4500EC
	~TFilterConfigurationIterator();

private: // Attributes
	// Pointer to the session implementation. This is not owned.
	//##ModelId=3C4C0F4500DA
	CHTTPSession* iSession;

	// Pointer array containing all the information for all the filter plugins
	// discovered by ECom. The list is stored and owned by the session. This is
	// not owned.
	//##ModelId=3C4C0F4500D0
	RPointerArray<TSessionFilterInfo>& iFilterInfoList;

	// Value conatining the index of the current filter in the list
	//##ModelId=3C4C0F4500C6
	TInt iCurrentFilterIndex;

	// Value containing the number filters in the list
	//##ModelId=3C4C0F4500BC
	TInt iFilterCount;
	};

#endif // __TFILTERCONFIGURATIONITER_H__
