// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <ineturilist.h>
#include <ineturi.h>
#include "ineturilistimpl.h"


/**
A default RInetUriList constructor.
*/
EXPORT_C RInetUriList::RInetUriList ()
: iInetUriListImpl ( NULL )
	{
		
	}

/**
Opens a URI list object. The function connects to the URI list server. 
*/
EXPORT_C void RInetUriList::OpenL ()
	{
	if ( !iInetUriListImpl )
		{
		// List is not opened so open it.
		iInetUriListImpl = CInetUriListImpl::NewL ();	
		}	
	}

/**
Closes the URI list object.
*/
EXPORT_C void RInetUriList::Close ()
	{
	delete iInetUriListImpl;
	iInetUriListImpl = NULL;
	}

/**
Add a new URI object to the list and its associated storage. Requires WriteDeviceData capability.

@param aInetUri URI object that needs to be added.
@leave KErrPermissionDenied If the application is not having WriteDeviceData capabilities.

@publishedPartner
@released
*/
EXPORT_C void RInetUriList::AddL ( const RInetUri& aInetUri )
	{
	// The handle must be open and attached to a concrete internet URI list object
	__ASSERT_ALWAYS( iInetUriListImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );
	
	iInetUriListImpl->AddL ( aInetUri );			
	}

/**
Removes a URI object from the list and its associated storage. Requires WriteDeviceData capabilities.

@param aInetUri URI object that needs to be removed.
@leave KErrPermissionDenied If the application is not having WriteDeviceData capabilities.
@leave KErrUriReadOnly URI object is read-only and cannot be removed.

@publishedPartner
@released
*/
EXPORT_C void RInetUriList::RemoveL ( const RInetUri& aInetUri )
	{
	// The handle must be open and attached to a concrete internet URI list object
	__ASSERT_ALWAYS( iInetUriListImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );
	iInetUriListImpl->RemoveL ( aInetUri );
	}

/**
Updates the URI object properties. Only favourite name and list type can be modified. Requires WriteDeviceData 
capabilities.

@param aInetUri URI object that needs to be updated
@leave KErrPermissionDenied If the application is not having WriteDeviceData capabilities.
@leave KErrUriReadOnly URI object is read-only and cannot be modified.

@publishedPartner
@released
*/
EXPORT_C void RInetUriList::UpdateL ( const RInetUri& aInetUri )	
	{
	// The handle must be open and attached to a concrete internet URI list object
	__ASSERT_ALWAYS( iInetUriListImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );
	iInetUriListImpl->UpdateL ( aInetUri );
	}

/**
Opens a URI object, which can be used to idenitfy the properties associated with the URI. Removing or updating
the URI from the list can be done with this object. Requires ReadDeviceData capabilities.

@param aUri URI input descriptor for which the information need to be retrieved
@param aServiceType Service type of the URI

@return RInetUri type object

@leave KErrPermissionDenied The application is not having ReadDeviceData capabilities.
@leave KErrUriNotFound The URI is not present in the list for the specified service type.
*/
EXPORT_C RInetUri RInetUriList::OpenInetUriL ( const TDesC8& aUri, TServiceType aServiceType )
	{
	// The handle must be open and attached to a concrete internet URI list object
	__ASSERT_ALWAYS( iInetUriListImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );
	return iInetUriListImpl->QueryUriL ( aUri, aServiceType );
	}

/**
Returns the number of URIs present in the list based on the given service type and list type. Requires 
ReadDeviceData capabilities.

@param aServiceType Service type (EBrowser, EWapPush, EPushEMail, EVoip and so on.)
@param aListType List type (EWhiteList and EBlackList)

@return Number of URIs present in the list.
*/
EXPORT_C TInt RInetUriList::Count ( TServiceType aServiceType, TListType aListType )
	{
	// The handle must be open and attached to a concrete internet URI list object
	__ASSERT_ALWAYS( iInetUriListImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );
	return iInetUriListImpl->Count ( aServiceType, aListType );
	}

/**
Retrieves the type associated with the URI by a given service type. Requires no capabilities.

@param aUri URI descriptor for which list type need to be retrieved
@param aServiceType The servicetype associated with the URI
@param aListType Returns the list type of URI( EWhiteList or EBlackList )

@return KErrUriNotFound if the URI is not located in the list, otherwise returns KErrNone.
*/
EXPORT_C TInt RInetUriList::GetListType ( const TDesC8& aUri, TServiceType aServiceType,TListType& aListType )
	{
	// The handle must be open and attached to a concrete internet URI list object
	__ASSERT_ALWAYS( iInetUriListImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );
	return iInetUriListImpl->GetListType ( aUri, aServiceType, aListType );
	}

/**
Queries the URI on the list. Different options are available for query operation. See 
TQueryArgs. The service type needs to be set in the query arguments for all queries. If 
a URI is set and no match criteria is specified, function will take EExact match as the 
matching criteria. The application must implement the result's callback function to receive 
the query results. The results can be controlled in the callback function, that is, the 
application can stop receiving the query results by returning EFalse from the query callback.
Application can also implement URI customisation callback if it wishes to do protocol/
scheme-based normalisation before query.

@param aArgs Query arguments.
@param aQueryCallback Callback function that will be called on each query result.
@param aUriCustomiser URI customiser callback function to do protocol/scheme-based normalisation before query.

@leave KErrServiceTypeNotPresent The service type is not specified in the query argument.
*/
EXPORT_C void RInetUriList::QueryUriL ( const TQueryArgs& aArgs, MQueryResultsCallback* aQueryCallback, MUriCustomiser* aUriCustomiser /* =NULL */ )
	{
	// The handle must be open and attached to a concrete internet URI list object
	__ASSERT_ALWAYS( iInetUriListImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );
	iInetUriListImpl->QueryUriL ( aArgs, *aQueryCallback, aUriCustomiser );			
	}

/**
Queries on Uri either to fetch the TLD Policy data or get the List type of the given Uri.

@param aArgs TPolicyQueryArgs argument. The Uri and Querytype are mandatory. 
			 To fetch Charset, List type is Mandatory, whereas to fetch List type it is not.
@param aResultArgs is out parmeter contains either List type info or Charset info. Based on the QueryType.

@leave KErrInvalidTLD The TLD type if No policy data is available for the given TLD.
*/
EXPORT_C void RInetUriList::QueryTldInfoL ( const TPolicyQueryArgs& aQueryArgs, TQueryResults& aResultArgs )
	{
	// The handle must be open and attached to a concrete internet URI list object
	__ASSERT_ALWAYS( iInetUriListImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );
	iInetUriListImpl->QueryTldInfoL ( aQueryArgs, aResultArgs );			
	}

