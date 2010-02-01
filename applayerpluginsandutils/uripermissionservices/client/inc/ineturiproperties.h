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

#ifndef __INETURIPROPERTIES_H__
#define __INETURIPROPERTIES_H__

#include <ineturilistdef.h>
#include <s32mem.h>

/**
This class holds all the associated properties CInetUriListImpl. It is having methods to
pack and unpack the properties data that will be sent/receive via IPC.

@internalComponent
*/
NONSHARABLE_CLASS ( CInetUriProperties ) : public CBase
	{
	public:
	static CInetUriProperties* NewL ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType );
	
	CInetUriProperties ();
	~CInetUriProperties ();
	
	
	InetUriList::TServiceType ServiceType () const;
	InetUriList::TListType ListType () const;
	InetUriList::TPermission Permission () const;
	const TDesC8& FavouriteName () const;
	TInt PropId () const;
	
	void SetServiceType ( InetUriList::TServiceType aServiceType );
	void SetListType ( InetUriList::TListType aListType );
	void SetPermission ( InetUriList::TPermission aPermission );
	void SetFavouriteNameL ( const TDesC8& aFavouriteName );
	void SetPropId ( TInt aId );
	
	TInt Size ();
	void PackL ( RWriteStream& aStream );
	void UnpackL ( RReadStream& aStream );
	
	void Clear ();
	
	private:
	CInetUriProperties ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType );

	private:
	TInt						iPropId;
	InetUriList::TServiceType 	iServiceType;
	InetUriList::TListType 		iListType;
	InetUriList::TPermission	iPermission;
	HBufC8* 					iFavouriteName;				
	};

#endif // __INETURIPROPERTIES_H__
