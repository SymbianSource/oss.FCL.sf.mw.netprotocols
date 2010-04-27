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
// Class for storing URI and its properties.
// 
//

/**
 @file
 @publishedAll
 @released
*/

#ifndef __INETURI_H__
#define __INETURI_H__

#include <ineturilistdef.h>

class CInetUriImpl;
class CInetUriProperties;
class CUri8;

using namespace InetUriList;

/**
RInetUri represents a handle to the URI and its associated URI properties. A URI must always be associated with 
a service type and a list type. Service type defines the purpose for which the URI is added (for example, 
Browser, WAP Push, Push EMail, and so on), and the list type defines the permission associated with the URI 
(for example, whitelist or blacklist).

@publishedAll
@released
*/
class RInetUri
	{
public:	
	friend class CInetUriListImpl;
	
	IMPORT_C RInetUri ();

	IMPORT_C void CreateL ( const TDesC8& aUri,  TServiceType aServiceType, TListType aListType );
	IMPORT_C void Close ();
	
	IMPORT_C TServiceType ServiceType () const;
	IMPORT_C TPermission Permission () const;
	IMPORT_C TListType ListType () const;	
	IMPORT_C const TDesC8& FavouriteName () const;
	IMPORT_C const CUri8& Uri () const;
		
	IMPORT_C void SetListType ( TListType aListType );
	IMPORT_C void SetFavouriteNameL ( const TDesC8& aFavouriteName );	
	
private:
	// Used internally. Not intended for external use
	CInetUriImpl& Impl () const;
	void Attach ( CInetUriImpl& aInetUri );
private:
	CInetUriImpl* iInetUriImpl; // // The internal URI object that this handle is connected to
	};

#endif //__INETURI_H__
