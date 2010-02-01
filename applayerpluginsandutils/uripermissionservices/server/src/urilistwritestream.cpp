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

#include <e32base.h>
#include <s32buf.h>
#include "urilistwritestream.h"
#include "ineturiimpl.h"
#include "ineturiproperties.h"
#include "urilist.h"
#include "ineturilistserver.h"
#include "uriqueryfilter.h"

/**
Stanadard factory construction method
*/
CUriListStream* CUriListStream::NewL ( CUriQueryFilter* aQueryFilter )
	{
	CUriListStream* self = new (ELeave) CUriListStream;
	CleanupStack::PushL ( self );
	self->ConstructL (aQueryFilter);
	CleanupStack::Pop ();
	return self;
	}

CUriListStream::CUriListStream ()
	{
		
	}

CUriListStream::~CUriListStream ()
	{
	iWriteStream.Close ();
	delete iInetUri;
	// Delete the buffer
	delete iIpcBuffer;
	delete iQueryFilter;	
	}

void CUriListStream::ConstructL ( CUriQueryFilter* aQueryFilter )
	{
	iIpcBuffer = CBufFlat::NewL ( KIpcDataSize );	
	iIpcBuffer->ExpandL ( 0, KIpcDataSize );		
	iWriteStream.Open ( *iIpcBuffer );	// Open the stream
	iInetUri = CInetUriImpl::NewL ();
	iQueryFilter = aQueryFilter;						
	}

/**
Writes the URI list to the IPC data stream. The results could contain multiple 
data records. The stream position will be set to the beginning before the write 
operation. It returns the total number of records returned via the data stream.
*/
TInt CUriListStream::WriteUriListL ( TPtrC8& aData )
	{
	ResetL ();

	MDBTransaction& dbTrans = iQueryFilter->DBTransaction();
	
	TInt totalRecords = 0;
	TBool dirty = iInetUri->IsDirty ();

	while ( !dirty || dbTrans.Next() ) 
		{
		if ( iQueryFilter->MatchRecordL () )
			{
			PopulateDataFieldsL ();
			if ( PendingWriteStreamSizeL () >= iInetUri->Size() )
				{
				iInetUri->PackL ( iWriteStream );
				iInetUri->Clear ();
				dirty = ETrue;
				++totalRecords; // Increment the total no. of records
				}
			else
				{
				break;				
				}			
			}
		}
	// Write to the buffer.
	aData.Set ( iIpcBuffer->Ptr(0) );
	
	// return with total no. of records
	return totalRecords;	
	}

/**
Resets the write stream position to the begining.
*/
void CUriListStream::ResetL ()
	{
	MStreamBuf* srcStream = iWriteStream.Sink ();
	srcStream->SeekL ( MStreamBuf::EWrite, EStreamBeginning, 0 );
	}

/**
Returns the number of bytes remaining in the stream for the write operation. 
*/
TInt CUriListStream::PendingWriteStreamSizeL ()
	{
	MStreamBuf* srcStream = iWriteStream.Sink ();	
	TStreamPos streamPos = srcStream->TellL ( MStreamBuf::EWrite );
	
	return KIpcDataSize - streamPos.Offset();
	}


/**
Reads the data from the storage and populates the CInetUriImpl object. Later this
data will be packed and send it via IPC data buffer
*/
void CUriListStream::PopulateDataFieldsL ()
	{
	MDBTransaction& dbTrans = iQueryFilter->DBTransaction();
	iInetUri->SetUriId ( dbTrans.ColumnIntL ( URILIST::EId ) );
	CUri8* uri = CreateUriL ();
	iInetUri->SetUri ( *uri );
	
	// Populate the properties
	CInetUriProperties& properties = iInetUri->Properties ();

	properties.SetPropId ( dbTrans.ColumnIntL ( URILIST::EPropId - 1 ) );
	properties.SetServiceType ( static_cast <InetUriList::TServiceType> ( dbTrans.ColumnIntL ( URILIST::EServiceType - 1 ) ) );
	properties.SetListType ( static_cast <InetUriList::TListType> ( dbTrans.ColumnIntL ( URILIST::EListType - 1 ) ) );
	properties.SetFavouriteNameL ( dbTrans.ColumnTextL ( URILIST::EFavouriteName - 1 ) );
	properties.SetPermission ( static_cast <InetUriList::TPermission> ( dbTrans.ColumnIntL ( URILIST::EPermission - 1 ) ) );
	}

/**
Creates a new URI object by setting the individual components. The individual componentes are 
read from the storage.
*/
CUri8* CUriListStream::CreateUriL ()
	{
	MDBTransaction& dbTrans = iQueryFilter->DBTransaction();
	CUri8* uri = CUri8::NewL ();
	CleanupStack::PushL ( uri );

	SetUriComponentL ( *uri, dbTrans.ColumnTextL ( URILIST::EScheme ), EUriScheme );	
	SetUriComponentL ( *uri, dbTrans.ColumnTextL ( URILIST::EHost ), EUriHost );	
	SetUriComponentL ( *uri, dbTrans.ColumnTextL ( URILIST::EPort ), EUriPort );	
	SetUriComponentL ( *uri, dbTrans.ColumnTextL ( URILIST::EUserInfo ), EUriUserinfo );	
	SetUriComponentL ( *uri, dbTrans.ColumnTextL ( URILIST::EPath ), EUriPath );			
	SetUriComponentL ( *uri, dbTrans.ColumnTextL ( URILIST::EQuery ), EUriQuery );	
	SetUriComponentL ( *uri, dbTrans.ColumnTextL ( URILIST::EFragments ), EUriFragment );	
	
	CleanupStack::Pop (); // uri
	return uri;
	}

/**
Sets the URI component.
*/
void CUriListStream::SetUriComponentL ( CUri8& aUri, const TDesC8& aUriComponent, TUriComponent aComponent )
	{
	aUri.SetComponentL ( aUriComponent, aComponent );
	}

