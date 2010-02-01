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
// @internalComponent
// 
//

#ifndef __INETURIIMPL_H__
#define __INETURIIMPL_H__

#include <ineturilistdef.h>
#include <s32mem.h>
#include "ineturiproperties.h"

class TUriC8;
class CUri8;


/**
CInetUriImpl implements RInetUri

This class is the place holder for the URI and its associated properties. It is having
methods to set the URI and its associated properties. Once created the URI and servicetype
cannot be modified. Also, the class is having methods to pack and unpack the data streams
that will be sent/receive via IPC

@internalComponent
*/
NONSHARABLE_CLASS ( CInetUriImpl ) : public CBase
	{
public:

	~CInetUriImpl ();
	
	static CInetUriImpl* NewL ();
	static CInetUriImpl* NewL ( const TDesC8& aUri, InetUriList::TServiceType aServiceType, InetUriList::TListType aType );
	
	CInetUriProperties& Properties () const;
	
	void SetUri ( CUri8& aUri );
	
	void SetUriL ( const TDesC8& aUri );
	const CUri8& Uri () const;
	const TDesC8& UriDes () const;
	
	void SetUriId ( TInt aId );
	TInt UriId () const;
	
	TInt Size () const;
	void PackL ( RWriteStream& aStream );
	void UnpackL ( RReadStream& aStream );
	
	void Clear ();
	TBool IsDirty () const;
	
	void LeaveIfReadOnlyL () const;
private:
	CInetUriImpl ();
	void ConstructL ();
	void ConstructL ( const TDesC8& aUri, InetUriList::TServiceType aServiceType, InetUriList::TListType aType );		
	void ConstructL ( InetUriList::TServiceType aServiceType, InetUriList::TListType aType );
	
private:
	TInt				iUriId;
	CUri8* 				iUri;
	CInetUriProperties* iProperties;
	};

#endif // __INETURIIMPL_H__
	
