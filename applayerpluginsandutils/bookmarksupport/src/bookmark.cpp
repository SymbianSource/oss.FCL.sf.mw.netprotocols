// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <bookmark.h>
#include "bkmrk.h"
#include "bkmrkfolder.h"

/**
RBkBookmark constructor
@publishedPartner
@released
*/
EXPORT_C RBkBookmark::RBkBookmark() : RBkNode()
	{
	}

/**
Method to query whether this bookmark is the home page

@return ETrue if this bookmark is the home page
@publishedPartner
@released
*/
EXPORT_C TBool RBkBookmark::IsHomePage()
	{
	return Bookmark()->IsHomePage();
	}

/**
Returns the last visited time for this bookmark
@return The time this bookmark was last visited
@publishedPartner
@released
*/
EXPORT_C TTime RBkBookmark::LastVisited() const
	{
	return Bookmark()->LastVisited();
	}

/**
Sets the last visited time for this bookmark to the time specified.
This function is intended to only be used for managing bookmarks, and will leave if not called in Management mode.
Use RBkBookmark::UpdateVisited() to set the last-modified time to the current time.

@param aTime The new visited time
@leave KErrPermissionDenied This function will leave when the bookmark database is not opened as
Bookmark::EVisibilityManager.
@publishedPartner
@released
@see RBkBookmark::UpdateVisited
*/
EXPORT_C void RBkBookmark::SetLastVisitedL(TTime aTime)
	{
	CBookmark* bookmark = Bookmark ();
	bookmark->LeaveIfNotInManagerModeL ();
	bookmark->SetLastVisited ( aTime );	
	}
	
/**
Sets the last visited time to be the current time

@publishedPartner
@released
*/
EXPORT_C void RBkBookmark::UpdateVisited()
	{
	Bookmark()->UpdateVisited();
	}

/**
Returns the assigned authentication object. This object is based on the bookmark's URI

@return A reference to the bookmark's authentication object.
@publishedPartner
@released
*/
EXPORT_C const CAuthentication& RBkBookmark::AuthenticationL()
	{
	CBkmrkExtendedProperties& properties = Bookmark()->BkmrkExtendedPropertiesL();
	return properties.AuthenticationL();
	}

/**
Set the bookmark's authentication object.

@param aAuthentication Reference to a new authentication object
@publishedPartner
@released
*/
EXPORT_C void RBkBookmark::SetAuthenticationL(const CAuthentication& aAuthentication)
	{
	CBkmrkExtendedProperties& properties = Bookmark()->BkmrkExtendedPropertiesL();
	properties.SetAuthenticationL(aAuthentication);
	}

/**
Method for getting the bookmark's Uri

@return Descriptor containing the Uri.
@publishedPartner
@released
*/
EXPORT_C const TDesC8& RBkBookmark::Uri() const
	{
	return Bookmark()->Uri();
	}
/**
Method for setting the bookmark's Uri

@param aUri Descriptor containing the new Uri
@publishedPartner
@released
*/
EXPORT_C void RBkBookmark::SetUriL(const TDesC8& aUri)
	{
	Bookmark()->SetUriL(aUri);
	}

/**
Method for getting the bookmark's proxy. The proxy will be set to the default proxy when
CBookmark is first created
@return Id for identifying the proxy entry in the commdb
@publishedPartner
@released
*/
EXPORT_C TUint32 RBkBookmark::ProxyL() const
	{
	CBkmrkExtendedProperties& properties = Bookmark()->BkmrkExtendedPropertiesL();
	return properties.Proxy();
	}

/**
Method for setting the bookmark's proxy.

@param aProxy Id for identifying the proxy entry in the commdb
@publishedPartner
@released
*/
EXPORT_C void RBkBookmark::SetProxyL(TUint32 aProxy)
	{
	CBkmrkExtendedProperties& properties = Bookmark()->BkmrkExtendedPropertiesL();
	properties.SetProxyL(aProxy);
	}

/**
Method for getting the bookmark's nap.

@return Id for identifying the nap entry in the commdb
@publishedPartner
@released
*/
EXPORT_C TUint32 RBkBookmark::GetNapL() const
	{
	CBkmrkExtendedProperties& properties = Bookmark()->BkmrkExtendedPropertiesL();
	return properties.Nap();
	}

/**
Method for setting the bookmark's nap.

@param aNap Id for identifying the nap entry in the commdb
@publishedPartner
@released
*/
EXPORT_C void RBkBookmark::SetNapL(TUint32 aNap)
	{
	CBkmrkExtendedProperties& properties = Bookmark()->BkmrkExtendedPropertiesL();
	properties.SetNapL(aNap);
	}

/**
This method is an internal utility method for converting the objects item pointer
to the correct type. It is not intended for external use.

@internalComponent
*/
CBookmark* RBkBookmark::Bookmark() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return static_cast<CBookmark*>(iItem);
	}


