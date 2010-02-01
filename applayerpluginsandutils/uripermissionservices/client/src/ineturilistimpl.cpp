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
// ineturilist.cpp
//
//

#include <e32base.h>
#include "ineturilistimpl.h"
#include "ineturiimpl.h"
#include <uriutils.h>

CInetUriListImpl::CInetUriListImpl ()
	{		
	}
	
CInetUriListImpl::~CInetUriListImpl ()
	{
	iInetUriListSession.Close ();
	}
/**
Standard factory construction method

*/
CInetUriListImpl* CInetUriListImpl::NewL ()
	{
	CInetUriListImpl* self = new ( ELeave ) CInetUriListImpl ();
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ( self );	
	return self;
	}
	
void CInetUriListImpl::ConstructL ()
	{
	User::LeaveIfError ( iInetUriListSession.Connect () );
	}
/**
Add a new URI to the storage. The session with the server must have been already opened.

@param aInetUri A fully constructed URI object

*/
void CInetUriListImpl::AddL ( const RInetUri& aInetUri )
	{
	// Add to the database
	iInetUriListSession.AddL ( aInetUri.Impl() );
	}

/**
Removes the URI from the storage. Function will leave if the URI object is read-only

@leave KErrUriReadOnly The URI object is read-only and cannot be removed
*/
void CInetUriListImpl::RemoveL ( const RInetUri& aInetUri )
	{		
	// Only URIs with read-write permission can be removed
	aInetUri.Impl().LeaveIfReadOnlyL ();

	// Remove from the list and database
	iInetUriListSession.RemoveL ( aInetUri.Impl() );
	}

/**
Updates the URI in the storage. Only favouritename & listtype will be updated

@param aInetUri URI object
@leave KErrUriReadOnly The URI object is read-only and cannot be modified
*/
void CInetUriListImpl::UpdateL ( const RInetUri& aInetUri )
	{		
	// Only URIs with read-write permission can be updated
	aInetUri.Impl().LeaveIfReadOnlyL ();

	// Update the list and database
	iInetUriListSession.UpdateL ( aInetUri.Impl() );
	}

/**
Counts the number of URIs in the list, given ServiceType and a ListType
*/
TInt CInetUriListImpl::Count ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType )
	{
	return 	iInetUriListSession.Count ( aServiceType, aListType );
	}

/**
Retrieves the Listtype of a give URI, given ServiceType. 

@return KErrUriNotFound The URI is not found in the list
		KErrNone The URI is found in the list and the aListType value is set.
*/
TInt CInetUriListImpl::GetListType ( const TDesC8& aUri, InetUriList::TServiceType aServiceType, InetUriList::TListType& aListType )
	{		
	TInt ret = KErrNone;
	TRAPD ( err, ret = iInetUriListSession.GetListTypeL ( DoNormalisationLC ( aUri, NULL ), aServiceType, aListType ); CleanupStack::PopAndDestroy (); );	
	if ( err != KErrNone )
		ret = err;
	return ret;
	}

/**
Queries the URI in the list, given ServiceType & URI as argument. An exact URI match
will be performed.

@param aUri URI descriptor
@param aServiceType Servicetype which the URI represents

@return A RInetUri object. Application can use this object for further operations
@leave KErrUriNotFound The URI is not found in the list for the given servicetype
*/
RInetUri CInetUriListImpl::QueryUriL ( const TDesC8& aUri, InetUriList::TServiceType aServiceType )
	{
	TQueryArgs args ( aServiceType, InetUriList::EExact );
	CUriListReadStream* uriListStream = iInetUriListSession.QueryUriL ( DoNormalisationLC ( aUri, NULL ), args );
	CleanupStack::PushL ( uriListStream );

	TInt totalRecords = uriListStream->ReadUriListL ();
	
	if ( totalRecords <= 0 )
		{
		User::Leave ( InetUriList::KErrUriNotFound );
		}

	RInetUri inetUri = UnpackL ( uriListStream->BufReadStream() );
	CleanupStack::PopAndDestroy (2); // uriListStream, object added in DoSyntaxNormalisationLC
	return inetUri;
	}

/**
Queries for the URI in the list.

@param aArgs Query argument. The ServiceType is mandatory and need to be set with the argument. 
			 Other query parameters are optional
@param aQueryCallback The callback function that will be called to return the query results 
@param aUriCustomiser The callback function that need to implement the customisation of the URI if the application wishes

@leave KErrServiceTypeNotPresent The servicetype value is not present in the query argument and is mandatory

*/
void CInetUriListImpl::QueryUriL ( const TQueryArgs& aArgs, MQueryResultsCallback& aQueryCallback, MUriCustomiser* aUriCustomiser )
	{
	// Check service type is present in the args. Without service type no query operation
	// will be performed
	if ( aArgs.Get ( TQueryArgs::EServiceType ) == KErrNotFound )
		{
		User::Leave ( InetUriList::KErrServiceTypeNotPresent );
		}
	
	CUriListReadStream* uriListStream = NULL;
	
	#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
  	if ( aArgs.Get ( TQueryArgs::EUri ) != KErrNotFound ) 
 		{
 		const TDesC8& uri = *(reinterpret_cast<TDesC8*> ( aArgs.Get ( TQueryArgs::EUri ) ));
		// Do normalisation
		TPtrC8 customisedUri ( DoNormalisationLC ( uri, aUriCustomiser ) );
		uriListStream = iInetUriListSession.QueryUriL ( customisedUri, aArgs );	
		CleanupStack::PopAndDestroy (); // objects added in DoNormalisationLC
		}
	else
		{
		uriListStream = iInetUriListSession.QueryUriL ( aArgs );	
		}	
	#else
  	if ( aArgs.IsSet ( TQueryArgs::EUri ) )
 		{
 		const TDesC8& uri = *(reinterpret_cast<TDesC8*> ( aArgs.Get ( TQueryArgs::EUri ) ));
		// Do normalisation
		TPtrC8 customisedUri ( DoNormalisationLC ( uri, aUriCustomiser ) );
		uriListStream = iInetUriListSession.QueryUriL ( customisedUri, aArgs );	
		CleanupStack::PopAndDestroy (); // objects added in DoNormalisationLC
		}
	else
		{
		uriListStream = iInetUriListSession.QueryUriL ( aArgs );	
		}	
	
	#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS

	CleanupStack::PushL ( uriListStream );
	
	RetrieveResultsL ( *uriListStream, aQueryCallback );

	CleanupStack::PopAndDestroy (); // uriListStream

	}

/**
Retrieves the query results from the server. The stream will be closed once the client has received
all the records or upon request by the application via the reqturn value of query results callback function
*/	
void CInetUriListImpl::RetrieveResultsL ( CUriListReadStream& aListStream, MQueryResultsCallback& aQueryCallback )
	{
	TBool bContinue = ETrue;
	do
		{
		TInt totalRecords = aListStream.ReadUriListL ();
		if ( totalRecords <= 0 )
			{
			bContinue = EFalse;
			}
		
		
		for ( TInt i = 1; i <= totalRecords; ++i )
			{
			// Unpack and do callback				
			RInetUri inetUri = UnpackL ( aListStream.BufReadStream() );
			if ( !aQueryCallback.OnQueryResultsL ( inetUri ) )
				{
				// Client has asked to stop the retrieval of query resuls
				// break the operation.
				bContinue = EFalse;
				break;
				}
			}
		} while ( bContinue );		
	}
	
/**
Unpacks the data stream and constructs a full URI object
*/
RInetUri CInetUriListImpl::UnpackL ( RReadStream& aStream )
	{
	CInetUriImpl* inetUri = CInetUriImpl::NewL ();
	CleanupStack::PushL ( inetUri )	;
	inetUri->UnpackL ( aStream );
	RInetUri inetUriObj;
	inetUriObj.Attach ( *inetUri );
	CleanupStack::Pop (); //inetUri
	return inetUriObj;
	}


/**
Does the syntax based normalisation. If the MUriCustomiser is set then it customises the URI, 
that is, do the protocol/scheme-based normalisation on the URI. The customisation algorithm needs 
to be implemented by the application. 
*/
const TDesC8& CInetUriListImpl::DoNormalisationLC ( const TDesC8& aUri, MUriCustomiser* aUriCustomiser )
	{
	// Do a syntax based normalisation
	TUriParser8 uriParser;
	User::LeaveIfError ( uriParser.Parse ( aUri ) );
	CUri8* normalisedUri = UriUtils::NormaliseUriL ( uriParser );	
	CleanupStack::PushL ( normalisedUri );
	if ( aUriCustomiser )
		{
		normalisedUri = aUriCustomiser->OnUriCustomisationL	( normalisedUri->Uri() );
		CleanupStack::PopAndDestroy (); // Added after UriUtils::NormaliseUriL
		CleanupStack::PushL ( normalisedUri );
		}
	return 	normalisedUri->Uri().UriDes();	
	}
	
/**
Queries on Uri either to fetch the TLD Policy data or get the List type of the given Uri.

@param aArgs TPolicyQueryArgs argument. The Uri and Querytype are mandatory. 
			 To fetch Charset, List type is Mandatory, whereas to fetch List type it is not.
@param aResultArgs is out parmeter contains either List type info or Charset info. Based on the QueryType.

@leave KErrInvalidTLD The TLD type if No policy data is available for the given TLD and Requested ListType.
*/
void CInetUriListImpl::QueryTldInfoL ( const TPolicyQueryArgs& aArgs, TQueryResults&   aResultArgs )
	{
	// Check Uri type is present in the args.
	if ( aArgs.Get ( TPolicyQueryArgs::ETldUri ) == KErrNotFound )
		{
		User::Leave ( InetUriList::KErrTldUriNotPresent );
		}
	// Query type is present in the args. 
	// Without these no query operation will be performed
	if ( aArgs.Get ( TPolicyQueryArgs::ETldQueryType ) == KErrNotFound )
		{
		User::Leave ( InetUriList::KErrTldQueryTypeNotPresent );
		}

	InetUriList::TTLDQueryType queryType (InetUriList::EPolicyListType);
	queryType = static_cast < InetUriList::TTLDQueryType > ( aArgs.Get ( TPolicyQueryArgs::ETldQueryType ) );	
		
	// Identify the Host type whether it is WhiteListed or BlackListed
	if ( queryType == InetUriList::EPolicyListType )
		{
		InetUriList::TListType lt(InetUriList::EBlackList);
		TInt result = iInetUriListSession.GetHostType(aArgs);
		User::LeaveIfError( result );				
		lt = static_cast < InetUriList::TListType > ( result );
		aResultArgs.Set(lt);
		}
	//fetch Policy data and send the results in TQueryResults
	else 	
		{
		HBufC8* results(NULL);
		results = FetchPolicyDataL ( aArgs );
		//owner ship of results with TQueryResults	
		aResultArgs.Set(results);
		}
	}

/**
Fetch Policy data
*/
HBufC8* CInetUriListImpl::FetchPolicyDataL(const TPolicyQueryArgs& aArgs )	
	{
	TInt policyLength = iInetUriListSession.PolicyDataLength( aArgs );
	//Policy file contains empty body for Requested white/Black listed data
	User::LeaveIfError ( policyLength );		
	HBufC8* policydata = HBufC8::NewL( policyLength );
	CleanupStack::PushL( policydata );
	TPtr8 ptr ( policydata->Des() ) ;
	TInt err = iInetUriListSession.QueryTldInfo( aArgs, ptr );
	CleanupStack::Pop( policydata );
	User::LeaveIfError( err );
	return policydata;
	}

