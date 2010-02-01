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
// Class for opening and manipulating the internet uri list.
// @internalComponent
// 
//

#ifndef __INETURILISTIMPL_H__
#define __INETURILISTIMPL_H__

#include <ineturilist.h>
#include <ineturi.h>
#include "ineturilistsession.h"


/**
CInetUriListImpl implements RInetUriList

This class performs adding, removing and updating the data in the storage. It is having 
methods to query the URI information fron the storage. 

@internalComponent
*/
NONSHARABLE_CLASS ( CInetUriListImpl ) : public CBase
	{
public:
	
	~CInetUriListImpl ();

	static CInetUriListImpl* NewL ();
	
	void AddL ( const RInetUri& aInetUri );
	void RemoveL ( const RInetUri& aInetUri );
	void UpdateL ( const RInetUri& aInetUri );	
	
	TInt Count ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType );
	
	TInt GetListType ( const TDesC8& aUri, InetUriList::TServiceType aServiceType,InetUriList::TListType& aListType );
	
	RInetUri QueryUriL ( const TDesC8& aUri, InetUriList::TServiceType aServiceType );
	
	void QueryUriL ( const TQueryArgs& aArgs, MQueryResultsCallback& aQueryCallback, MUriCustomiser* aUriCustomiser );
	void QueryTldInfoL (const TPolicyQueryArgs& aQueryArgs, TQueryResults& aResultArgs );
	
private:
	void ConstructL ();			
	CInetUriListImpl ();
	
	void RetrieveResultsL ( CUriListReadStream& aListStream, MQueryResultsCallback& aQueryCallback );
	HBufC8* FetchPolicyDataL(const TPolicyQueryArgs& aArgs);
	RInetUri UnpackL ( RReadStream& aStream );
	
	const TDesC8& DoNormalisationLC ( const TDesC8& aUri, MUriCustomiser* aUriCustomiser );
private:
	RInetUriListSession iInetUriListSession;
	};

#endif // __INETURILISTIMPL_H__

