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

#ifndef __INETURILISTSERVER_H__
#define __INETURILISTSERVER_H__

#include <e32base.h>
#include <e32std.h>
#include <e32cmn.h>
#include <ineturilistdef.h>
#include <tldlistdef.h>
#include "StreamObjContainer.h"
#include "urilistwritestream.h"

const TInt KIpcDataSize = 2 * 1024;

// ----------------------------------------------------
const TUint KInetUriListServerMajorVersion = 0;
const TUint KInetUriListServerMinorVersion = 1;
const TUint KInetUriListServerBuildVersion = 1;

_LIT( KInetUriListServerName,"!InetUriListServer" );

// Structures
struct TRendezvous
	{
	TThreadId iId;
	TRequestStatus* iStatus;
	};

// ----------------------------------------------------


class CUriListInterface;
class CUriQueryFilter;
	
class CInetUriListServerSession : public CSession2
	{
public:
	static CInetUriListServerSession* NewL ();
	~CInetUriListServerSession ();
	
		
private:
	// From CSession2
	void ServiceL ( const RMessage2& aMessage );
	
	void ServiceError (const RMessage2 &aMessage, TInt aError);
	
	CInetUriListServerSession ();	
	
	void DispatchMessageL ( const RMessage2& aMessage );
	CBufFlat* ReadMessageLC ( TInt aParam, const RMessage2& aMessage );
	CUriListStream* GetListStreamL ( CUriQueryFilter* aQueryFilter );
	
private:
	TInt AddUriL ( const RMessage2& aMessage );
	TInt RemoveUriL ( const RMessage2& aMessage );
	TInt UpdateUriL ( const RMessage2& aMessage );
	
	TInt UriCountL ( const RMessage2& aMessage );
	
	TInt GetListTypeL ( const RMessage2& aMessage );

	TInt QueryWithUriL ( const RMessage2& aMessage );
	TInt QueryL ( const RMessage2& aMessage );
	TInt QueryTldInfoL ( const RMessage2& aMessage );
	TInt GetHostTypeL ( const RMessage2& aMessage );
	TInt PolicyDataLengthL ( const RMessage2& aMessage );
	TInt ReadQueryResultsL ( const RMessage2& aMessage );
	TInt CloseSrvStream ( const RMessage2& aMessage );

private:
	InetUriList::TServiceType iServiceType;	
	
	RStreamObjContainer < CUriListStream > iUriListStreams;
	};

// -------------------------------------------------------------------------------

class CInetUriListServer : public CPolicyServer
	{
public:

	enum TFunctionCode
		{
		EListType,
		EUriCount,
		EQuery,
		EQueryWithUri,
		EReadQueryResults,
		ECloseSrvStream,
		EAddUri,
		ERemoveUri,
		EUpdateUri,
		EQueryTldInfo,
		ETldListType,
		EPolicyLength,
		ENotSupported		
		};	
		
	static CInetUriListServer* NewL ();
	
	~CInetUriListServer ();
	
	void IncSessionCount () const;
	void DecSessionCount () const;
	
	void Error ( TInt aError );	
	
	inline CUriListInterface* UriListInterface () const;
private:
	virtual CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage ) const;
	
	CInetUriListServer ();
	void ConstructL ();
	
	
private:
	mutable TInt iSessionCount;
	mutable CUriListInterface* iUriListInterface;
	};

// ----------------------------------------------------------------------------------

class CInetUriListSrvScheduler : public CActiveScheduler
	{
public:
	~CInetUriListSrvScheduler();
	static CInetUriListSrvScheduler* NewL();

private:
	CInetUriListSrvScheduler();
	
	// from CActiveScheduler
	virtual void Error(TInt aError) const;

private:
	CInetUriListServer* iInetUriListSrv;
	};
	
// ----------------------------------------------------------------------------------

#endif // __INETURILISTSERVER_H__
