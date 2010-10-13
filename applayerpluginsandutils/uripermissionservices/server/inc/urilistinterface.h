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

#ifndef __URILISTINTERFACE_H__
#define __URILISTINTERFACE_H__
#include <s32mem.h>
#include <uri8.h>
#include <stringpool.h>
#include <ineturilistdef.h>
#include <tldlistdef.h>
#include "dbaccessor.h"

class CInetUriImpl;
class CTldProperties;
class CInetUriProperties;
class CUriListStream;
class CUriQueryFilter;

/**
This class is the interface between the server session class and the data storage. The methods 
provided in this class will make calls to the data storage class for the operations requested 
by the client.
*/
class CUriListInterface : public CBase
	{
	public:
	
	static CUriListInterface* NewL ();		
	~CUriListInterface ();

	void AddUriL ( RReadStream& aUriStream, TInt& aUriId, TInt& aPropId );
	void RemoveUriL ( TInt aUriId, TInt aPropId );
	void UpdateUriL ( TInt aUriId, InetUriList::TListType aListType, const TDesC8& aFavouriteName );
	
	TInt CountUriL ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType );
	
	InetUriList::TListType GetListTypeL ( const TDesC8& aUri, InetUriList::TServiceType aSt );

	CUriQueryFilter* QueryL ( const TQueryArgs& aQueryArgs );
	CUriQueryFilter* QueryWithUriL ( const TDesC8& aUri, const TQueryArgs& aQueryArgs );
	
	InetUriList::TListType GetHostTypeL ( const TDesC8& aUri );
	
	HBufC8* QueryTldInfoL(const TDesC8& aUri, const TPolicyQueryArgs& aQueryArgs );
	
	TInt PolicyDataLengthL ( const TDesC8& aUri, const TPolicyQueryArgs& aQueryArgs );
	
	void InsertL ( CInetUriImpl& aInetUri );	
	void InsertTldPolicyDataL ( CTldProperties& aTldPolicydata );	
	
	private:	
	CUriListInterface ();
	void ConstructL ();
	
	TInt UpdateL ( const TDesC8& aSchemaName, TInt aUriId, InetUriList::TListType aListType, const TDesC8& aFavouriteName );
	void DeleteL ( const TDesC8& aSchemaName, TInt aFieldPos, TInt aId );
	
	HBufC8* ExtractHostL( const TDesC8& aUri );
	HBufC8* ExtractTldL( const TDesC8& aUri );
	const TDesC8& DoNormalisationLC ( const TDesC8& aUri );

	TBool CheckDependencyL ( TInt aId );
	TBool FileExist(const TDesC& aFilePath);
	TBool IsDuplicateEntryL ( const CTldProperties& aTldProps );

	TInt InsertIntoUriTblL ( const TUriC8& aUri, TInt& aUriId );
	TInt InsertIntoUriPropsTblL ( TInt aUriId, const CInetUriProperties& aUriProps, TInt& aPropId );
	TInt InsertIntoViewL ( const CInetUriImpl& aInetUri );
	TInt InsertIntoTldTblL ( const CTldProperties& aTldProps );
	
	TInt GetMaxIdValueL ( const TDesC8& aTblName, const TDesC8& aFieldName );
		
	MDBTransaction* DoSearchExactUriLC ( const TUriC8& aUri, TInt aServiceType, TInt aListType = KErrNotFound );
	MDBTransaction* DoSearchUriPathLC ( const TUriC8& aUri, InetUriList::TURIMatch aMatch, TInt aServiceType, TInt aListType = KErrNotFound );
	MDBTransaction* DoSearchUriDomainLC ( const TUriC8& aUri, TInt aServiceType, TInt aListType = KErrNotFound );
	
	CUriQueryFilter* QueryWithTldL ( const TDesC8& aUri,const TPolicyQueryArgs& aQueryArgs );
	CUriQueryFilter* DoQueryWithTldL ( const TDesC8& aUri, const TPolicyQueryArgs& aQueryArgs );
	
	static void DestroyTransObj ( TAny* aPtr );
	static void RollbackTrans ( TAny* aPtr );
	
	const TDesC8& GetFieldName ( TInt aTableIndex );
	void GetInstalledDirectoryL( const TUid aUid, const TDesC& aFile, TDes& aFilePath );
	
	void InitViewQueryLC ( RBuf8& aBuf );
	void InitTldQueryLC (  RBuf8& aBuf );
	void AppendFieldName ( RBuf8& aBuf, TInt aTableIndex, const TUriC8& aUri, TUriComponent aComponent );
	void BindTextL ( MDBTransaction& aDbTrans, TInt aBindPos, const TUriC8& aUri, TUriComponent aComponent );
	
	TBool IsWhiteListedUriL( const TDesC8& aUri );
	TBool IsBlackListedUriL( const TDesC8& aUri );
	TBool IsBlackListedHost( const TDesC8& aHost, RArray<TChar>& aList );
	TBool IsNextCharInList(TChar aChar, RArray<TChar>& aPolicyList);
	TBool IsWhiteListedHostL( const TDesC8& aHost, RArray<TChar>& aPolicyList );
	void  StringToCharListL(const TDesC8& aString, RArray<TChar>& aList, TChar aSeparator = '.');
	void  TokenizeStringL(const TDesC8& aString, RArray<TChar>& aList, TChar aSeparator = ' ');

	void InitializeDatabaseL ();
	void InitializePolicyDatabaseL ();
	void UpgradePolicyDatabaseL();	
	
	private:
	MDBAccessor* 	iDbAccessor;
	RStringPool		iStringPool;
	};

#endif // __URILISTINTERFACE_H__
