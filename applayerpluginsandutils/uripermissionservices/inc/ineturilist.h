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
// 
//

/**
 @file
 @publishedAll
 @released
*/

#ifndef __INETURILIST_H__
#define __INETURILIST_H__

#include <ineturilistdef.h>
#include <tldlistdef.h>
#include <ineturi.h>
#include <uri8.h>
#include <babitflags.h>

class CInetUriListImpl;




/**
This class defines the interface that can be implemented by an application to receive the
query results. 

@publishedAll
@released
*/
class MQueryResultsCallback
	{
	public:
	/**
	The callback function that will be called when there is at least one query result.
	
	@param aUri The Inet URI object. Ownership will be passed. The application should close the 
				object handle.
	@return ETrue - More query results, if any, should follow.
			EFalse - No more query result is required and querying will be stopped.
	*/
	virtual TBool OnQueryResultsL ( RInetUri aUri ) =0;
	};


/**
This class defines the interface that can be implemented by an application that wishes to do
protocol and scheme-based normalisation of the URI before query operation.

@publishedAll
@released
*/
class MUriCustomiser
	{
	public:
	/**
	The callback function that will be called to perform scheme and protocol-based normalisation.
	The URI will be syntax normalised before calling this function.
	
	@param aUri The syntax normalised URI.
	@return Final normalised URI, which is syntax and protocol/scheme based.
	*/
	virtual CUri8* OnUriCustomisationL ( const TUriC8& aUri ) =0;	
	};



/**
RInetUriList represents a handle to the list as a whole. Opening the handle will initiate a connection the
URI List server. This class is responsible for adding, removing, updating, or retrieving the URI and
its associated properties.

@publishedAll
@released
*/
class RInetUriList
	{
public:
	IMPORT_C RInetUriList ();

	IMPORT_C void OpenL ();
	IMPORT_C void Close ();
	
	IMPORT_C void AddL ( const RInetUri& aInetUri );
	IMPORT_C void RemoveL ( const RInetUri& aInetUri );
	IMPORT_C void UpdateL ( const RInetUri& aInetUri );
	
	IMPORT_C RInetUri OpenInetUriL ( const TDesC8& aUri, InetUriList::TServiceType aServiceType );
	IMPORT_C TInt Count ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType );
	
	IMPORT_C TInt GetListType ( const TDesC8& aUri, InetUriList::TServiceType aServiceType, InetUriList::TListType& aListType );
	
	IMPORT_C void QueryUriL ( const TQueryArgs& aArgs, MQueryResultsCallback* aQueryCallback, MUriCustomiser* aUriOptimiser =NULL );
	IMPORT_C void QueryTldInfoL ( const TPolicyQueryArgs& aQueryArgs, TQueryResults& aResultArgs );
private:
	CInetUriListImpl* iInetUriListImpl; // // The internal list object that this handle is connected to
	};

#endif //__INETURILIST_H__
