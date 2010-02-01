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


#include "ineturiimpl.h"
#include "ineturiproperties.h"
#include <uri8.h>
#include <uriutils.h>
 
CInetUriImpl* CInetUriImpl::NewL ()
	{
	CInetUriImpl* self = new ( ELeave ) CInetUriImpl ();
	CleanupStack::PushL ( self );	
	self->ConstructL ();	
	CleanupStack::Pop ();	
	return self;			
	}

CInetUriImpl::CInetUriImpl ()
: iUriId ( -1 )
	{		
	}

CInetUriImpl::~CInetUriImpl ()
	{
	delete iUri;
	iUri = NULL;
	delete iProperties;	
	iProperties = NULL;
	}

CInetUriImpl* CInetUriImpl::NewL ( const TDesC8& aUri, InetUriList::TServiceType aServiceType, InetUriList::TListType aUriType )
	{
	CInetUriImpl* self = new ( ELeave ) CInetUriImpl ();
	CleanupStack::PushL ( self );
	self->ConstructL ( aUri, aServiceType, aUriType );
	CleanupStack::Pop ();	
	return self;
	}

void CInetUriImpl::ConstructL ()
	{
	iProperties = new ( ELeave ) CInetUriProperties;
	}

void CInetUriImpl::ConstructL ( const TDesC8& aUri, InetUriList::TServiceType aServiceType, InetUriList::TListType aUriType )
	{	
	TUriParser8 uriParser;
	User::LeaveIfError ( uriParser.Parse ( aUri ) );
	// Do a syntax based normalisation
	iUri = UriUtils::NormaliseUriL ( uriParser );
	ConstructL ( aServiceType, aUriType );
	}

void CInetUriImpl::ConstructL ( InetUriList::TServiceType aServiceType, InetUriList::TListType aUriType )
	{
	iProperties = CInetUriProperties::NewL ( aServiceType, aUriType );
	}

/**
Returns the underlying CUri8 object
*/
const CUri8& CInetUriImpl::Uri () const
	{
	return *iUri;		
	}

/**
Set the URI Id
*/
void CInetUriImpl::SetUriId ( TInt aId )
	{
	iUriId = aId;		
	}

/**
Returns the URI Id
*/
TInt CInetUriImpl::UriId () const
	{
	return iUriId;	
	}

/**
Returns the properties object
*/
CInetUriProperties& CInetUriImpl::Properties () const
	{
	return *iProperties;	
	}

/**
Leaves with KErrUriReadOnly value if the URI is read-only
*/
void CInetUriImpl::LeaveIfReadOnlyL () const
	{
	if ( iProperties->Permission() == InetUriList::EReadOnly )
		{
		User::Leave ( InetUriList::KErrUriReadOnly );
		}
	}

/**
Clears the URI object by deleting the underlying CUri8 object
*/
void CInetUriImpl::Clear ()
	{
	delete iUri;
	iUri = NULL;
	iProperties->Clear ();
	}

TBool CInetUriImpl::IsDirty () const
	{
	return ( iUri == NULL );		
	}

void CInetUriImpl::SetUri ( CUri8& aUri )
	{
	iUri = &aUri;		
	}

/**
Set the URI and parses the URI
*/
void CInetUriImpl::SetUriL ( const TDesC8& aUri )
	{
	// The handle must be open and attached to a concrete inernet uri object
	__ASSERT_ALWAYS( !iUri, User::Panic( InetUriList::KInetUriListErrHandleNotOpen, InetUriList::KErrNotOpen ) );

	TUriParser8 uriParser;
	User::LeaveIfError ( uriParser.Parse ( aUri ) );
	iUri = CUri8::NewL ( uriParser );
	}

const TDesC8& CInetUriImpl::UriDes () const
	{
	// The handle must be open and attached to a concrete inernet uri object
	__ASSERT_ALWAYS( iUri, User::Panic( InetUriList::KInetUriListErrHandleNotOpen, InetUriList::KErrNotOpen ) );
	
	return iUri->Uri().UriDes();		
	}

/**
Calculates total size of the IPC data buffer
*/
TInt CInetUriImpl::Size () const
	{	
	return ( sizeof ( TInt ) * 2 + iUri->Uri ().UriDes ().Length () + iProperties->Size() );
	}
	
/**
Packs the URI object to send it via IPC
*/
void CInetUriImpl::PackL ( RWriteStream& aStream )
	{
	aStream.WriteInt32L ( iUriId );
	const TDesC8& uriDes ( iUri->Uri ().UriDes () );
	aStream.WriteInt32L ( uriDes.Length() );
	aStream.WriteL ( uriDes );

	// Pack the properties
	iProperties->PackL ( aStream );
	}

/**
Unpacks the stream and assign to respective values
*/
void CInetUriImpl::UnpackL ( RReadStream& aStream )
	{
	SetUriId ( aStream.ReadInt32L() );
	TInt uriSize = aStream.ReadInt32L ();
	HBufC8* uri = HBufC8::NewLC ( uriSize );
	TPtr8 ptr ( uri->Des () );
	aStream.ReadL ( ptr, uriSize );
	SetUriL ( ptr );
	CleanupStack::PopAndDestroy ( ); // uri

	// Unpack the properties
	iProperties->UnpackL ( aStream );
	}
