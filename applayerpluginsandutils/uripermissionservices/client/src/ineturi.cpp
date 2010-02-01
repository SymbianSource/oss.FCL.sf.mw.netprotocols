// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "ineturiimpl.h"
#include "ineturiproperties.h"

/**
A default RInetUri constructor.

*/
EXPORT_C RInetUri::RInetUri ()
: iInetUriImpl ( NULL )
	{
		
	}

/**
Creates and opens a new URI object handle, given a URI and its associated service type and list type. The URI will 
be syntax normalised before creation. All the URIs will be created with read-write permission. The permission cannot
be changed. The service type cannot be modified later.

@param aUri URI descriptor object
@param aServiceType The service type associated with the URI. For possible values, see InetUriList::TServiceType
@param aListType The list type associated with the URI

*/
EXPORT_C void RInetUri::CreateL ( const TDesC8& aUri, TServiceType aServiceType, TListType aListType )
	{
	if ( iInetUriImpl )	
		User::LeaveIfError ( KErrUriAlreadyExists );
	
	iInetUriImpl = CInetUriImpl::NewL ( aUri, aServiceType, aListType );
	}

/**
Closes the RInetUri handle.
*/
EXPORT_C void RInetUri::Close ()
	{
	delete iInetUriImpl;	
	iInetUriImpl = NULL;
	}

/**
Returns the service type associated with the URI. 

@publishedPartner
@released

@return service type
*/
EXPORT_C TServiceType RInetUri::ServiceType () const
	{
	// The handle must be open and attached to a concrete inernet uri object
	__ASSERT_ALWAYS( iInetUriImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );

	return iInetUriImpl->Properties().ServiceType();		
	}

/**
Returns the list type associated with the URI.

@publishedPartner
@released

@return list type of the URI
*/
EXPORT_C TListType RInetUri::ListType () const
	{
	// The handle must be open and attached to a concrete inernet uri object
	__ASSERT_ALWAYS( iInetUriImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );

	return iInetUriImpl->Properties().ListType ();	
	}

/**
Returns the permission associated with the URI, that is, read-only or read-write.

@publishedPartner
@released

@return list type
*/
EXPORT_C TPermission RInetUri::Permission () const
	{
	// The handle must be open and attached to a concrete inernet uri object
	__ASSERT_ALWAYS( iInetUriImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );

	return iInetUriImpl->Properties ().Permission ();	
	}

/**
Returns the favourite name of the URI. Returns KNullDesC8 if no favourite name is set.

@publishedPartner
@released

@return Favourite name of URI
*/
EXPORT_C const TDesC8& RInetUri::FavouriteName () const
	{
	// The handle must be open and attached to a concrete inernet URI object
	__ASSERT_ALWAYS( iInetUriImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );

	return iInetUriImpl->Properties ().FavouriteName ();
	}

/**
Returns the CUri8 type object.

@publishedPartner
@released

@return Reference to URI
*/
EXPORT_C const CUri8& RInetUri::Uri () const
	{
	// The handle must be open and attached to a concrete inernet URI object
	__ASSERT_ALWAYS( iInetUriImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );

	return iInetUriImpl->Uri ();
	}

/**
Set the new list type for the URI.

@param aListType New list type to be associated with the URI

@publishedPartner
@released
*/
EXPORT_C void RInetUri::SetListType ( TListType aListType )
	{
	// The handle must be open and attached to a concrete inernet URI object
	__ASSERT_ALWAYS( iInetUriImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );

	iInetUriImpl->Properties ().SetListType ( aListType );	
	}
	
/**
Set the favourite name associated with the URI.

@param aFavouriteName Favourite name of the URI

@publishedPartner
@released
*/
EXPORT_C void RInetUri::SetFavouriteNameL ( const TDesC8& aFavouriteName )
	{
	// The handle must be open and attached to a concrete inernet URI object
	__ASSERT_ALWAYS( iInetUriImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );

	iInetUriImpl->Properties ().SetFavouriteNameL ( aFavouriteName );
	}
	
/**
Not intended for external use.

@internalComponent
*/
CInetUriImpl& RInetUri::Impl () const
	{
	return *iInetUriImpl;
	}

/**
Not intended for external use.

@internalComponent
*/
void RInetUri::Attach ( CInetUriImpl& aInetUri )
	{
	// The handle must not be opened
	__ASSERT_ALWAYS( !iInetUriImpl, User::Panic( KInetUriListErrHandleNotOpen, KErrNotOpen ) );
	iInetUriImpl = &aInetUri;	
	}
