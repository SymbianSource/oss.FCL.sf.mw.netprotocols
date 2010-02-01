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

#include <s32mem.h>
#include "ineturilistsession.h"
#include "ineturilistserver.h"
#include "ineturiimpl.h"
#include "ineturiproperties.h"
#include "urilistreadstream.h"
#include <uri8.h>


const TInt KNumConnectRetries = 5;

_LIT( KServerCode, "INETURILISTSERVER" );

RInetUriListSession::RInetUriListSession ()
	{
	
	}
	
RInetUriListSession::~RInetUriListSession ()
	{
	Close ();
	}

/**
Connects to the ineturilist server, attempting to start if necessary. 
 */
TInt RInetUriListSession::Connect ()
	{
	TInt retry = KNumConnectRetries;
	TVersion version ( KInetUriListServerMajorVersion, 
						KInetUriListServerMinorVersion, 
						KInetUriListServerBuildVersion );
	FOREVER
		{
		TInt err = CreateSession ( KInetUriListServerName, version );

		if ( err != KErrNotFound && err != KErrServerTerminated )
			{
			return err;
			}
		// need to restart server
		if ( --retry == 0 )
			{
			return err;
			}
		err = StartServer ();
		if ( err != KErrNone && err != KErrAlreadyExists )
			{
			return err;
			}
		}
	}
	
void RInetUriListSession::Close ()
	{
	if ( Handle() )
		{
		RSessionBase::Close ();
		}
	}


TInt RInetUriListSession::StartServer ()
	{
	const TInt KIntServerType = 0x20009D70; 
	const TUidType serverUid ( KNullUid, KNullUid, TUid::Uid( KIntServerType ) );
	
	TRequestStatus started ( KRequestPending );
	TRendezvous rendezvous;
	rendezvous.iId = RThread ().Id ();		// id of this thread
	rendezvous.iStatus = &started;

	RProcess server;
	TInt err = server.Create( KServerCode,
						TPtrC ( reinterpret_cast < TText* > ( &rendezvous ),
						sizeof ( rendezvous ) / sizeof ( TText ) ), serverUid );

	if ( err != KErrNone )
		{
		return err;
		}

	server.SetPriority ( EPriorityHigh );
	
	TRequestStatus stat;
	server.Rendezvous ( stat );
	if ( stat != KRequestPending )
		{
		server.Kill ( 0 );		// abort startup
		}
	else
		{
		server.Resume ();	// logon OK - start the server
		}
	User::WaitForRequest ( stat );		// wait for start or death
	server.Close ();
	return stat.Int ();
	}

/**
Returns a buffer of size specified as argument.

*/
CBufFlat* RInetUriListSession::GetBufferLC ( TInt aSize )
	{
	CBufFlat* buffer = CBufFlat::NewL ( aSize );
	CleanupStack::PushL ( buffer );
	buffer->ExpandL ( 0, aSize );
	return buffer;
	}

/**
Sends the IPC data packet the server for adding into the storage. On return of
the IPC call the URI Id & Property ID will be updated.

@param aInetUri The URI object with its associated properties. The URI Id & property
Id will be updated on this object on return of the IPC call.
*/
void RInetUriListSession::AddL ( CInetUriImpl& aInetUri )
	{
	TInt uriSize = aInetUri.Size ();
	
	CBufFlat* inBuffer = GetBufferLC ( uriSize );
		
	RBufWriteStream writeStream ( *inBuffer );
	CleanupClosePushL ( writeStream );
	
	// Pack the data
	aInetUri.PackL ( writeStream );
	
	TPtr8 ptr ( inBuffer->Ptr (0) );		
	
	TInt uriId = 0;
	TInt propId = 0;
	
	TPckgBuf < TInt > uriIdBuffer ( uriId );
	TPckgBuf < TInt > propIdBuffer ( propId );
		
	TIpcArgs args ( &ptr, &uriIdBuffer, &propIdBuffer );
	const TInt result = SendReceive ( CInetUriListServer::EAddUri, args );
	
	User::LeaveIfError ( result );
	
	aInetUri.SetUriId ( uriIdBuffer() );
	aInetUri.Properties().SetPropId ( propIdBuffer() );
	
	CleanupStack::PopAndDestroy ( 2 ); // writeStream, inBuffer	
	}

/**
Sends the IPC data packet to the server for removing from the storage. 

@param aInetUri The URI object with its associated properties that need to be 
removed from the storage
*/
void RInetUriListSession::RemoveL ( const CInetUriImpl& aInetUri )
	{
	// Only URI Id & Properties ID need to be sent to the server which will issue a 
	// DELETE statement based on the Properties ID to the DB storage
	TPckg < TInt > uriIdBuffer ( aInetUri.UriId() );
	TPckg < TInt > propIdBuffer ( aInetUri.Properties().PropId() );
	TIpcArgs args ( &uriIdBuffer, &propIdBuffer );
	
	const TInt result = SendReceive ( CInetUriListServer::ERemoveUri, args );
	User::LeaveIfError ( result );
	}
/**
Sends the IPC data packet to the server for updating the storage. Only favouritename
and listtype will be updated.

@param aInetUri The URI object whose favouritename & listype that need updation
*/
void RInetUriListSession::UpdateL ( const CInetUriImpl& aInetUri )
	{
	// Only Favourite Name & List type can be updated. Sent the 
	// IPC args with those values and the URI ID
	TPckg < TUint32 > propIdBuffer ( aInetUri.Properties().PropId() );	
	TPckg < InetUriList::TListType > listTypeBuffer ( aInetUri.Properties().ListType() );
	
	TIpcArgs args ( &propIdBuffer, &listTypeBuffer, &( aInetUri.Properties().FavouriteName() ) );
	
	const TInt result = SendReceive ( CInetUriListServer::EUpdateUri, args );
	User::LeaveIfError ( result );
	}

/**
Counts the number of URIs present in the list for a given servicetype & listtype. The data will 
be packaged and send to the server. The count value will be returned as a return value to 
the SendReceive fn
*/
TInt RInetUriListSession::Count ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType )
	{
	TPckg < InetUriList::TServiceType > stBuffer ( aServiceType );
	TPckg < InetUriList::TListType > ltBuffer ( aListType );
	
	TIpcArgs args ( &stBuffer, &ltBuffer );
	return SendReceive ( CInetUriListServer::EUriCount, args );
	}

/**
Get the listtype for a URI, given a servicetype
*/
TInt RInetUriListSession::GetListTypeL ( const TDesC8& aUri, InetUriList::TServiceType aServiceType, InetUriList::TListType& aListType )
	{	
//	InetUriList::TListType lt;
	
	TPckg < InetUriList::TServiceType > stBuffer ( aServiceType );
	TPckgBuf < InetUriList::TListType > ltBuffer;// ( lt );
	
	TIpcArgs args ( &aUri, &stBuffer, &ltBuffer );
	
	const TInt result = SendReceive ( CInetUriListServer::EListType, args );
	if ( result != KErrNone )
		{
		User::Leave ( result );			
		}

	aListType = ltBuffer();
	
	return result;
	}

/**
Queries the URI in the list by providing query arguments. The server will setup a stream
and the handle will be returned as a return value for SendReceive fn. Using that handle
client can stream the query result.
*/
CUriListReadStream* RInetUriListSession::QueryUriL ( const TQueryArgs& aArgs )
	{
	TPckg < TQueryArgs > queryArgs ( aArgs );	
	TIpcArgs args ( &queryArgs );
	const TInt result = SendReceive ( CInetUriListServer::EQuery, args );
	User::LeaveIfError ( result );
	
	return CUriListReadStream::NewL( *this, result );
	}
	
/**
Queries the URI in the list by providing query arguments. The server will return 
the policydata by writing it back into IpcArgs. And return value for SendReceive 
fn contains the length of the aCharset filled by server.
aArgs contains Query args and aCharSet is an out parameter.
*/
TInt RInetUriListSession::QueryTldInfo ( const TPolicyQueryArgs& aArgs, TPtr8& aCharSet )
	{
	const TDesC8& inputUri = *(reinterpret_cast<TDesC8*> ( aArgs.Get ( TPolicyQueryArgs::ETldUri ) ));
	TPckg < TPolicyQueryArgs > queryArgs ( aArgs );	
	TIpcArgs args ( &inputUri, &queryArgs, &aCharSet );
	return SendReceive ( CInetUriListServer::EQueryTldInfo, args );
	}

/**
Fetch the length of Policy data
*/
TInt RInetUriListSession::PolicyDataLength ( const TPolicyQueryArgs& aArgs )
	{
	const TDesC8& inputUri = *(reinterpret_cast<TDesC8*> ( aArgs.Get ( TPolicyQueryArgs::ETldUri ) ));
	TPckg < TPolicyQueryArgs > queryArgs ( aArgs );	
	TIpcArgs args (&inputUri, &queryArgs );
	return SendReceive ( CInetUriListServer::EPolicyLength, args );	
	}
	
/**
Identifies whether the given Uri is BlackListed or Whitelisted
*/
TInt RInetUriListSession::GetHostType ( const TPolicyQueryArgs& aArgs )
	{
	const TDesC8& inputUri = *(reinterpret_cast<TDesC8*> ( aArgs.Get ( TPolicyQueryArgs::ETldUri ) ));
	TIpcArgs args (&inputUri);	
	TInt err = SendReceive ( CInetUriListServer::ETldListType, args );
	return err;
	}

/**
Queries the URI in the list by providing query arguments and a URI. The server will setup 
a stream and the handle will be returned as a return value for SendReceive fn. Using that 
handle, client can stream the query result.
*/
CUriListReadStream* RInetUriListSession::QueryUriL ( const TDesC8& aUri, const TQueryArgs& aArgs )
	{
	TPckg < TQueryArgs > queryArgs ( aArgs );	
	TIpcArgs args ( &aUri, &queryArgs );
	const TInt result = SendReceive ( CInetUriListServer::EQueryWithUri, args );
	User::LeaveIfError ( result );
	
	return CUriListReadStream::NewL( *this, result );
	}

/**
The query results stream has been setup in the server and the server stream handle is 
returned via QueryUriL call. Read the query results using that handle. 
*/		
TInt RInetUriListSession::ReadQueryResults ( TInt aHandle, TPtr8& aPtr )
	{
	TIpcArgs args ( aHandle, &aPtr );
	
	return SendReceive ( CInetUriListServer::EReadQueryResults, args );
	}
	
/**
Close the server stream handle.
*/
void RInetUriListSession::CloseSrvStream ( TInt aHandle )
	{
	TIpcArgs args ( aHandle );
	
	SendReceive ( CInetUriListServer::ECloseSrvStream, args );		
	}
