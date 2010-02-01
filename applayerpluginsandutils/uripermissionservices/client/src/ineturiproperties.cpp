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

#include "ineturiproperties.h"

CInetUriProperties* CInetUriProperties::NewL ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType )
	{
	CInetUriProperties* self = new ( ELeave ) CInetUriProperties ( aServiceType, aListType );
	return self;
	}

CInetUriProperties::CInetUriProperties ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType )
: iPropId ( -1 ),
iServiceType ( aServiceType ),
iListType ( aListType ),
iPermission ( InetUriList::EReadWrite )
	{
		
	}

CInetUriProperties::CInetUriProperties ()
: iPropId ( -1 ),
iListType ( InetUriList::EBlackList ),
iPermission ( InetUriList::EReadWrite )
	{
		
	}

CInetUriProperties::~CInetUriProperties ()
	{
	delete iFavouriteName;	
	iFavouriteName = NULL;
	}

/**
Returns the servicetype of the URI
*/
InetUriList::TServiceType CInetUriProperties::ServiceType () const
	{
	return iServiceType;
	}

/**
Returns the listtype of the URI
*/
InetUriList::TListType CInetUriProperties::ListType () const
	{
	return iListType;
	}

/**
Returns the permission associated with the URI
*/
InetUriList::TPermission CInetUriProperties::Permission () const
	{
	return iPermission;	
	}

/**
Returns the favouritename associated with the URI. Returns KNullDesC8 
if no favouritename is set.
*/
const TDesC8& CInetUriProperties::FavouriteName () const
	{
	if ( iFavouriteName )
		return *iFavouriteName;
	
	return KNullDesC8();
	}

/**
Returns the Property Id.
*/
TInt CInetUriProperties::PropId () const
	{
	return iPropId;		
	}

/**
Set the servicetype for the URI
*/
void CInetUriProperties::SetServiceType ( InetUriList::TServiceType aServiceType )
	{
	iServiceType = aServiceType;	
	}

/**
Set the listtype for the URI
*/
void CInetUriProperties::SetListType ( InetUriList::TListType aListType )
	{
	iListType = aListType;
	}

/**
Set the permission for the URI
*/
void CInetUriProperties::SetPermission ( InetUriList::TPermission aPermission )
	{
	iPermission = aPermission;	
	}

/**
Set the favouritename for the URI
*/
void CInetUriProperties::SetFavouriteNameL ( const TDesC8& aFavouriteName )
	{
	delete iFavouriteName;	
	iFavouriteName = NULL;
	iFavouriteName = aFavouriteName.AllocL ();
	}

/**
Clear the properties value and sets the default

*/
void CInetUriProperties::Clear ()
	{
	delete iFavouriteName;
	iFavouriteName = NULL;
	SetPermission ( InetUriList::EReadWrite );
	}

void CInetUriProperties::SetPropId ( TInt aId )
	{
	iPropId = aId;		
	}

/**
Total size of the properties. The buffer size for the IPC data is based on this
Total size = Property id + Service type + List Type + Permission + Len of Favourite name 
+ actual Favouritename length
Property Id, Service type, List type, Permission & Len of Favourite name takes sizeof (TInt)
*/
TInt CInetUriProperties::Size ()
	{	
	TInt size = ( sizeof ( TInt ) * 5 ); // Property Id + Service Type + List Type + Permission + Length of favourite name
	
	size += ( FavouriteName().Length () );
	
	return size;			
	}

/**
Packs the properties data to set via IPC data to the server.
*/
void CInetUriProperties::PackL ( RWriteStream& aStream )
	{
	aStream.WriteInt32L ( iPropId );		
	aStream.WriteInt32L ( iServiceType );
	aStream.WriteInt32L ( iListType );
	aStream.WriteInt32L ( iPermission );
	
	TPtrC8 ptr ( FavouriteName() );
	aStream.WriteInt32L ( ptr.Length() );
	aStream.WriteL ( ptr );
	}

/**
Unpacks the properties stream and assign to the respective variables
*/
void CInetUriProperties::UnpackL ( RReadStream& aStream )
	{
	iPropId = aStream.ReadInt32L ();
	iServiceType = static_cast < InetUriList::TServiceType > ( aStream.ReadInt32L () );
	iListType = static_cast < InetUriList::TListType > ( aStream.ReadInt32L () );
	iPermission = static_cast < InetUriList::TPermission > ( aStream.ReadInt32L () );

	TInt favouriteNameLength = aStream.ReadInt32L ();

	HBufC8* favouriteName = HBufC8::NewLC ( favouriteNameLength );
	TPtr8 ptr ( favouriteName->Des () );
	aStream.ReadL ( ptr, favouriteNameLength );
	SetFavouriteNameL ( ptr );
	CleanupStack::PopAndDestroy (); // favouriteName	
	}
