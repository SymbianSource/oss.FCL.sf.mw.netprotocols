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
#include "ineturilistserver.h"
#include "urilistinterface.h"
#include "sqldbtransaction.h"


const TUint KRangeCount = 4;
const TUint8 KPolicyElementReadDeviceData =	0;
const TUint8 KPolicyElementWriteDeviceData = 1;
const TInt KOpCodeRanges [ KRangeCount ] =
	{
	CInetUriListServer::EListType, // AlwaysPass
	CInetUriListServer::EQuery,	   // ReadDeviceData: CInetUriListServer::EQuery ... 
	CInetUriListServer::EAddUri,  // WriteDeviceData: CInetUriListServer::EAddUri ... CInetUriListServer::EUpdateUri
	CInetUriListServer::ENotSupported			
	};

const TUint8 KElementsIndex [ KRangeCount ] =
	{	
	CPolicyServer::EAlwaysPass,
	KPolicyElementReadDeviceData,
	KPolicyElementWriteDeviceData,
	CPolicyServer::ENotSupported
	};
	
const CPolicyServer::TPolicyElement KPolicyElements[] = 
	{
	{_INIT_SECURITY_POLICY_C1 ( ECapabilityReadDeviceData ), CPolicyServer::EFailClient }, 
	{_INIT_SECURITY_POLICY_C1 ( ECapabilityWriteDeviceData ), CPolicyServer::EFailClient }, 
	};

const CPolicyServer::TPolicy KInetUriListServerPolicy = 
	{
	CPolicyServer::EAlwaysPass,
	KRangeCount,
	KOpCodeRanges,
	KElementsIndex,
	KPolicyElements
	};

CInetUriListServerSession::CInetUriListServerSession ()
	{
		
	}

CInetUriListServerSession::~CInetUriListServerSession ()
	{	
	iUriListStreams.Close ();
	
	( static_cast< const CInetUriListServer* >( Server() ) )->DecSessionCount();	
	}

CInetUriListServerSession* CInetUriListServerSession::NewL ()
	{
	CInetUriListServerSession* self = new ( ELeave ) CInetUriListServerSession;	
	return self;
	}

void CInetUriListServerSession::ServiceL ( const RMessage2& aMessage )
	{	
	DispatchMessageL ( aMessage );
	}

void CInetUriListServerSession::ServiceError ( const RMessage2& aMessage, TInt aError )
	{
	aMessage.Complete( aError );
	}	
	
CBufFlat* CInetUriListServerSession::ReadMessageLC ( TInt aParam, const RMessage2& aMessage )
	{
	TInt desMaxLen ( aMessage.GetDesMaxLengthL ( aParam ) ); // Maximum length of the descriptor
	
	// Read from client message buffer
	CBufFlat* buffer = CBufFlat::NewL ( desMaxLen );
	CleanupStack::PushL ( buffer );	
	buffer->ExpandL ( 0, desMaxLen );
	TPtr8 bufferPtr ( buffer->Ptr(0) );
	aMessage.ReadL ( aParam, bufferPtr );
	
	return buffer;	
	}

CUriListStream* CInetUriListServerSession::GetListStreamL ( CUriQueryFilter* aQueryFilter )
	{
	CleanupStack::PushL ( aQueryFilter );
	CUriListStream* listStream = CUriListStream::NewL ( aQueryFilter );
	CleanupStack::Pop (); // aQueryFilter 
	return listStream;	
	}

void CInetUriListServerSession::DispatchMessageL ( const RMessage2& aMessage )
	{
	TInt ret ( KErrNone );

	switch ( aMessage.Function() )	
		{	

		case CInetUriListServer::EListType:
		ret = GetListTypeL ( aMessage );
		break;
		
		case CInetUriListServer::EQuery:
		ret = QueryL ( aMessage );
		break;
		
		case CInetUriListServer::EQueryWithUri:
		ret = QueryWithUriL ( aMessage );
		break;
		
		case CInetUriListServer::EReadQueryResults:
		ret = ReadQueryResultsL ( aMessage );
		break;
		
		case CInetUriListServer::ECloseSrvStream:
		ret = CloseSrvStream ( aMessage );
		break;
		
		case CInetUriListServer::EUriCount:
		ret = UriCountL ( aMessage );
		break;
		
		case CInetUriListServer::EAddUri:
		ret = AddUriL ( aMessage );
		break;
		
		case CInetUriListServer::ERemoveUri:
		ret = RemoveUriL ( aMessage );
		break;
		
		case CInetUriListServer::EUpdateUri:
		ret = UpdateUriL ( aMessage );
		break;

		case CInetUriListServer::EQueryTldInfo:
		ret = QueryTldInfoL ( aMessage );
		break;
	
		case CInetUriListServer::ETldListType:
		ret = GetHostTypeL(aMessage);
		break;
		
		case CInetUriListServer::EPolicyLength:
		ret = PolicyDataLengthL ( aMessage );
		break;
		
		case CInetUriListServer::ENotSupported:
		ret = KErrNotFound;	
		break;				
		
		default:
		ret = KErrNotSupported;
		break;
		}
	
	if ( ret != KRequestPending )
		{
		aMessage.Complete( ret );
		}	
	}


TInt CInetUriListServerSession::AddUriL ( const RMessage2& aMessage )
	{
	CBufFlat* outBuffer = ReadMessageLC ( 0, aMessage );
	
	RBufReadStream readStream ( *outBuffer );
	CleanupClosePushL ( readStream );
	
	TInt uriId = 0;
	TInt propId = 0;
	CUriListInterface* listInterface = ( static_cast < const CInetUriListServer* > ( Server() ) )->UriListInterface ();
	listInterface->AddUriL ( readStream, uriId, propId );
	
	// Write back the uri id and property id
	TPckgBuf < TInt > uriIdBuffer ( uriId );
	TPckgBuf < TInt > propIdBuffer ( propId );
	aMessage.WriteL ( 1, uriIdBuffer );
	aMessage.WriteL ( 2, propIdBuffer );
	
	CleanupStack::PopAndDestroy ( 2 ); // outBuffer, readStream
	return KErrNone;		
	}
	
TInt CInetUriListServerSession::RemoveUriL ( const RMessage2& aMessage )
	{
	TPckgBuf < TInt > uriIdBuffer;
	aMessage.ReadL ( 0, uriIdBuffer );
	TPckgBuf < TInt > propIdBuffer;
	aMessage.ReadL ( 1, propIdBuffer );
	CUriListInterface* listInterface = ( static_cast < const CInetUriListServer* > ( Server() ) )->UriListInterface ();
	
	listInterface->RemoveUriL ( uriIdBuffer(), propIdBuffer() );
	
	return KErrNone;	
	}

TInt CInetUriListServerSession::UpdateUriL ( const RMessage2& aMessage )
	{
	TPckgBuf < TUint32 > idBuffer;
	aMessage.ReadL ( 0, idBuffer );
	TPckgBuf < InetUriList::TListType > listTypeBuffer;
	aMessage.ReadL ( 1, listTypeBuffer );
	
	HBufC8* favouriteName = HBufC8::NewLC ( aMessage.GetDesLengthL ( 2 ) );
	TPtr8 favouriteNamePtr ( favouriteName->Des () );
	
	aMessage.ReadL ( 2, favouriteNamePtr );
	
	CUriListInterface* listInterface = ( static_cast < const CInetUriListServer* > ( Server() ) )->UriListInterface ();
	
	listInterface->UpdateUriL ( idBuffer(), listTypeBuffer(), favouriteNamePtr );
	
	CleanupStack::PopAndDestroy (); // favouriteName
	return KErrNone;	
	}

TInt CInetUriListServerSession::UriCountL ( const RMessage2& aMessage )
	{
	TPckgBuf < InetUriList::TServiceType > stBuffer;
	TPckgBuf < InetUriList::TListType > ltBuffer;	
	aMessage.ReadL ( 0, stBuffer );
	aMessage.ReadL ( 1, ltBuffer );
	
	CUriListInterface* listInterface = ( static_cast < const CInetUriListServer* > ( Server() ) )->UriListInterface ();
	return listInterface->CountUriL ( stBuffer(), ltBuffer() );	
	}

TInt CInetUriListServerSession::GetListTypeL ( const RMessage2& aMessage )
	{
	HBufC8* uri = HBufC8::NewLC ( aMessage.GetDesLengthL (0) );
	TPtr8 uriPtr ( uri->Des() );
	
	aMessage.ReadL ( 0, uriPtr );
	TPckgBuf < InetUriList::TServiceType > stBuffer;
	aMessage.ReadL ( 1, stBuffer );
	
	CUriListInterface* listInterface = ( static_cast < const CInetUriListServer* > ( Server() ) )->UriListInterface ();
	
	InetUriList::TListType listType = listInterface->GetListTypeL ( uriPtr, stBuffer() ); 
	TPckgBuf < InetUriList::TListType > ltBuffer ( listType );
	
	aMessage.WriteL ( 2, ltBuffer );
	
	CleanupStack::PopAndDestroy (); // uri
	
	return KErrNone;
	}

TInt CInetUriListServerSession::QueryWithUriL ( const RMessage2& aMessage )
	{
	RBuf8 uri;
	CleanupClosePushL ( uri );
	uri.CreateL (aMessage.GetDesLengthL (0));
	TPckgBuf < TQueryArgs > queryArgs;
	aMessage.ReadL ( 0, uri );
	aMessage.ReadL ( 1, queryArgs );

	CUriListInterface* listInterface = ( static_cast < const CInetUriListServer* > ( Server() ) )->UriListInterface ();
	CUriListStream* listStream = GetListStreamL ( listInterface->QueryWithUriL ( uri, queryArgs() ) );		
	CleanupStack::PopAndDestroy (); // uri
	CleanupStack::PushL (listStream); 
	iUriListStreams.AllocL ();
	CleanupStack::Pop (); // listStream
	return iUriListStreams.Add ( listStream );
	}

/**
Fetches length of the Policy data
*/	
TInt CInetUriListServerSession::PolicyDataLengthL ( const RMessage2& aMessage )
	{
	RBuf8 uri;
	CleanupClosePushL ( uri );
	uri.CreateL (aMessage.GetDesLengthL (0));
	TPckgBuf < TPolicyQueryArgs > queryArgs;
	aMessage.ReadL ( 0, uri );
	aMessage.ReadL ( 1, queryArgs );
	CUriListInterface* listInterface = ( static_cast < const CInetUriListServer* > ( Server() ) )->UriListInterface ();
	TInt length ( listInterface->PolicyDataLengthL( uri, queryArgs() ) );
	CleanupStack::PopAndDestroy();//uri		
	return length;
	}

/**
Fetches Policy data
*/
TInt CInetUriListServerSession::QueryTldInfoL ( const RMessage2& aMessage )
	{
	RBuf8 uri;
	CleanupClosePushL ( uri );
	uri.CreateL (aMessage.GetDesLengthL (0));
	TPckgBuf < TPolicyQueryArgs > queryArgs;
	aMessage.ReadL ( 0, uri );
	aMessage.ReadL ( 1, queryArgs );
	CUriListInterface* listInterface = ( static_cast < const CInetUriListServer* > ( Server() ) )->UriListInterface ();
	HBufC8* resultBuf = listInterface->QueryTldInfoL( uri, queryArgs() );
	CleanupStack::PushL ( resultBuf );
	TInt resultLength ( resultBuf->Length() ) ;
	aMessage.Write(2,resultBuf->Des(),0);
	CleanupStack::PopAndDestroy(2);//uri, resultBuf 
	return resultLength;
	}

/**
Identifies whther Hosttype is in Black List or White List
*/
TInt CInetUriListServerSession::GetHostTypeL ( const RMessage2& aMessage )
	{
	RBuf8 uri;
	CleanupClosePushL ( uri );
	uri.CreateL (aMessage.GetDesLengthL (0));
	aMessage.ReadL ( 0, uri );
	CUriListInterface* listInterface = ( static_cast < const CInetUriListServer* > ( Server() ) )->UriListInterface ();
	TInt hostType ( listInterface->GetHostTypeL( uri ) ); 
	CleanupStack::PopAndDestroy(); //uri
	return hostType;
	}

TInt CInetUriListServerSession::QueryL ( const RMessage2& aMessage )
	{
	TPckgBuf < TQueryArgs > queryArgs;
	aMessage.ReadL ( 0, queryArgs );
	CUriListInterface* listInterface = ( static_cast < const CInetUriListServer* > ( Server() ) )->UriListInterface ();

	CUriListStream* listStream = GetListStreamL ( listInterface->QueryL ( queryArgs() ) );	
	CleanupStack::PushL (listStream); 
	iUriListStreams.AllocL ();
	CleanupStack::Pop ();	// listStream
	return iUriListStreams.Add ( listStream );
	}

TInt CInetUriListServerSession::ReadQueryResultsL ( const RMessage2& aMessage )
	{
	TInt handle = aMessage.Int0 ();
	TInt totalRecords = 0;
	CUriListStream* listStream = iUriListStreams.Find ( handle );
	if ( !listStream )
		{
		// Panic the client	
		aMessage.Panic ( KInetUriListServerName(), InetUriList::KErrInvalidStreamHandle );
		return totalRecords;
		}
	TPtrC8 dataPtr;
	totalRecords = listStream->WriteUriListL ( dataPtr );
	aMessage.WriteL ( 1, dataPtr );
	return totalRecords;
	}

TInt CInetUriListServerSession::CloseSrvStream ( const RMessage2& aMessage )
	{
	TInt handle = aMessage.Int0 ();
	iUriListStreams.Remove ( handle );
	
	return KErrNone;
	}
	
// ----------------------------------------------------------------------


CInetUriListServer::CInetUriListServer ()
: CPolicyServer ( EPriorityStandard, KInetUriListServerPolicy, ESharableSessions ),
	iSessionCount (0)
	{
		
	}

CInetUriListServer::~CInetUriListServer ()
	{
	delete iUriListInterface;
	}

CInetUriListServer* CInetUriListServer::NewL ()
	{
	CInetUriListServer* self = new ( ELeave ) CInetUriListServer;
	CleanupStack::PushL ( self );
	
	self->ConstructL ();
	
	CleanupStack::Pop ( self );
	return self;
	}

void CInetUriListServer::ConstructL ()
	{	
	StartL ( KInetUriListServerName );
	
	iUriListInterface = CUriListInterface::NewL ();
	}
	
void CInetUriListServer::IncSessionCount () const
	{
	++iSessionCount;	
	}

void CInetUriListServer::DecSessionCount () const
	{
	--iSessionCount;
	
	if ( iSessionCount == 0 )	
		{
		CActiveScheduler::Stop ();			
		}
	}

void CInetUriListServer::Error ( TInt aError )
	{
	Message().Complete ( aError );
	ReStart();
	}

CSession2* CInetUriListServer::NewSessionL( const TVersion& aVersion, const RMessage2& /* aMessage */ ) const
	{
	TVersion ver ( KInetUriListServerMajorVersion, KInetUriListServerMinorVersion, KInetUriListServerBuildVersion );
	if ( !User::QueryVersionSupported ( ver,aVersion ) )
		{
		User::Leave( KErrNotSupported );
		}

	CInetUriListServerSession* session = CInetUriListServerSession::NewL ();
	IncSessionCount ();
	
	return session;
	}

CUriListInterface* CInetUriListServer::UriListInterface () const
	{
	return iUriListInterface;		
	}
// ----------------------------------------------------------------------

CInetUriListSrvScheduler::CInetUriListSrvScheduler ()
	{
	}

CInetUriListSrvScheduler::~CInetUriListSrvScheduler()
	{
	delete iInetUriListSrv;
	}

CInetUriListSrvScheduler* CInetUriListSrvScheduler::NewL ()
	{
	CInetUriListSrvScheduler* self = new ( ELeave ) CInetUriListSrvScheduler;
	CActiveScheduler::Install ( self );
	
	// Create the inet urilist server
	self->iInetUriListSrv = CInetUriListServer::NewL ();
	
	return self;	
	}

void CInetUriListSrvScheduler::Error ( TInt aError ) const
	{	
	if ( iInetUriListSrv )	
		{
		iInetUriListSrv->Error( aError );			
		}
	}
	
// ----------------------------------------------------------------------
//
// Thread top level
// Perform all server initialisation, in particular creation of the
// scheduler and server and then run the scheduler
//

static void RunServerL()
	{
	// create and install the active scheduler we need
	CInetUriListSrvScheduler* s = CInetUriListSrvScheduler::NewL();
	CleanupStack::PushL(s);

	//
	// naming the server thread after the server helps to debug panics
	User::LeaveIfError ( RThread().RenameMe ( KInetUriListServerName() ) );

	//
	// Initialisation complete, now signal the client
	RProcess::Rendezvous ( KErrNone );
	//
	// Ready to run
	CActiveScheduler::Start ();
	//
	// Cleanup the scheduler
	CleanupStack::PopAndDestroy ( s );
	}

// Inet URI server entrypoint
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	//
	CTrapCleanup* cleanup = CTrapCleanup::New ();
	TInt r = KErrNoMemory;
	if ( cleanup )
		{
		TRAP ( r, RunServerL () );
		delete cleanup;
		}
	__UHEAP_MARKEND;
	return r;
	}

