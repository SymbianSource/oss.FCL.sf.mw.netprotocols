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

#include <ineturilistdef.h>
#include <tldlistdef.h>
#include "urilistinterface.h"
#include "sqldbaccessor.h"
#include "ineturiimpl.h"
#include "urilistwritestream.h"
#include "urilist.h"
#include "urilistinitializer.h"
#include "tldlistinitializer.h"
#include "uriqueryfilter.h"
#include "tldproperties.h"
#include <delimitedpath8.h>
#include "tldlist.h"
#include <swi/sisregistrysession.h>
#include <swi/sisregistryentry.h>
#include <bautils.h> 
#include <uriutils.h>

const TInt KMaxLength = 1024;
const TInt pkgId(0x2000A471);
_LIT(KRomDirectory, "z");
_LIT(KUriListFile, ":\\private\\20009D70\\ineturilist.xml");
#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
_LIT(KPolicyFile, ":\\private\\20009D70\\tldpolicy.xml");
#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS

CUriListInterface::CUriListInterface ()
	{
		
	}

CUriListInterface::~CUriListInterface ()
	{
	if ( iDbAccessor )
		{
		iDbAccessor->Release ();	
		}
	iStringPool.Close ();
	}

CUriListInterface* CUriListInterface::NewL ()
	{
	CUriListInterface* self = new ( ELeave ) CUriListInterface ();
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ( self );
	
	return self;
	}

	
void CUriListInterface::ConstructL ()
	{
	iStringPool.OpenL ( URILIST::Table );
	iStringPool.OpenL ( TLDLIST::Table );
	iDbAccessor = CSqlDbAccessor::NewL ( iStringPool );
	InitializeDatabaseL ();
	}

/**
Add a new URI and its associated properties into the storage. 

@param aUriStream A stream to read the CInetUriImpl object.
@param aUriId URI Id value that will be returned to the caller
@param aPropId Property Id value that will be returned to the caller

*/
void CUriListInterface::AddUriL ( RReadStream& aUriStream, TInt& aUriId, TInt& aPropId )
	{	
	CInetUriImpl* inetUri = CInetUriImpl::NewL ();
	CleanupStack::PushL ( inetUri );
	inetUri->UnpackL ( aUriStream );
	InsertL ( *inetUri );
	aUriId = inetUri->UriId ();
	aPropId = inetUri->Properties().PropId ();
	CleanupStack::PopAndDestroy ( inetUri );	
	}

/**
Removes the properties from the URIPropertiesTbl. The function removes the URI from the 
URITbl table if it is not having a dependency. See the table structure

@param aUriId URI Id value 
@param aPropId Property Id value
*/
void CUriListInterface::RemoveUriL ( TInt aUriId, TInt aPropId )
	{
	iDbAccessor->BeginTransactionL ();
	// This is not to delete the object rather to do a rollback operation incase of a failure
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::RollbackTrans, iDbAccessor ) );
	
	// Delete from properties table
	DeleteL ( KUriPropsTblName(), URILIST::EPropId, aPropId );

	// The URI also need to be removed if it is not marked for another service type.
	// Check for dependency. 
	if ( CheckDependencyL( aUriId ) )
		{
		// No dependency. The URI also need to be removed			
		DeleteL ( KUriTblName(), URILIST::EId, aUriId );
		}	
	
	// Delete the same from view
	DeleteL ( KViewName(), URILIST::EPropId, aPropId );
	iDbAccessor->CommitTransactionL ();
	CleanupStack::Pop (); // iDbAccessor
	}

/**
Updates the listtype and favouritename. Only, these 2 properties can be modified.

@param aUriPropsId Property ID which uniquely identifies the row in the URIPropertiesTbl
@param aListType New Listtype 
@param aFavouriteName New favourite name for the URI
*/
void CUriListInterface::UpdateUriL ( TInt aUriPropsId, InetUriList::TListType aListType, const TDesC8& aFavouriteName )
	{	
	iDbAccessor->BeginTransactionL ();
	// This is not to delete the object rather to do a rollback operation incase of a failure
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::RollbackTrans, iDbAccessor ) );

	// Only update List Type & Favourite Name. Other properties are not allowed to update.
	if ( UpdateL ( KUriPropsTblName(), aUriPropsId, aListType, aFavouriteName ) == 0 )
		User::Leave ( InetUriList::KErrUriNotFound );

	User::LeaveIfError ( UpdateL ( KViewName(), aUriPropsId, aListType, aFavouriteName ) );
	
	iDbAccessor->CommitTransactionL ();
	CleanupStack::Pop (); // iDbAccessor	
	}

/**
Inserts the information into URITbl, URIPropertiesTbl & the URI view. Checks whether
the URI is already present in the storage and applies the blacklist/whitelist rule
before insertion. 

@param aInetUri URI information object. On return the new property id and URI id will be set.
*/
void CUriListInterface::InsertL ( CInetUriImpl& aInetUri )
	{
	TInt result = KErrNone;
	const TUriC8& uri ( aInetUri.Uri().Uri() );
	MDBTransaction* dbTrans = DoSearchExactUriLC ( uri, aInetUri.Properties().ServiceType() );
	if ( dbTrans->Next() )
		{		
		// URI already exists for the requested service type. 
		result = InetUriList::KErrUriAlreadyExists;
		}	
	User::LeaveIfError ( result );
	CleanupStack::PopAndDestroy (); // dbTrans
	
	// Now the URI can be added.
	iDbAccessor->BeginTransactionL ();
	// This is not to delete the object rather to do a rollback operation incase of a failure
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::RollbackTrans, iDbAccessor ) );

	TInt uriId = 0;
	if ( InsertIntoUriTblL ( aInetUri.Uri().Uri(), uriId ) == 0 )
		User::Leave ( InetUriList::KErrUriAddFailed );
		
	aInetUri.SetUriId ( uriId );
	
	// Now add the properties	
	TInt propId = 0;
	if ( InsertIntoUriPropsTblL ( aInetUri.UriId(), aInetUri.Properties(), propId ) == 0 )
		User::Leave ( InetUriList::KErrUriAddFailed );	
	aInetUri.Properties().SetPropId( propId );
	User::LeaveIfError ( InsertIntoViewL ( aInetUri ) );
	iDbAccessor->CommitTransactionL ();			
	CleanupStack::Pop (); // iDbAccessor	
	}

/**
Inserts the information into TLDTbl. Checks whether
the URI is already present in the storage and applies the blacklist/whitelist rule
before insertion. 

@param aTldPolicydata Tld information object.
*/
void CUriListInterface::InsertTldPolicyDataL ( CTldProperties& aTldPolicydata )
	{
	//First check whether such row is available
	//if yes dont insert.
	// Now the TLD Policy data can be added.
	iDbAccessor->BeginTransactionL ();
	// This is not to delete the object rather to do a rollback operation incase of a failure
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::RollbackTrans, iDbAccessor ) );
	//Inserts unique row 
	if( !IsDuplicateEntryL( aTldPolicydata )   )
		{
		InsertIntoTldTblL ( aTldPolicydata );
		}

	iDbAccessor->CommitTransactionL ();			
	CleanupStack::Pop (); // iDbAccessor	
	}
	
/**
Inserts the data into the URITbl. 

@param aUri URI object
@param [out] aUriId The URI will be filled upon successful insertion. This ID
	uniquelly indentifies the URI.

@return Returns the number of rows inserted. Normally 1. Otherwise returns with
		system-wide error code.
*/
TInt CUriListInterface::InsertIntoUriTblL ( const TUriC8& aUri, TInt& aUriId )
	{
	const TDesC8& id ( iStringPool.String (URILIST::EId,URILIST::Table).DesC() );
	aUriId = GetMaxIdValueL ( KUriTblName(), id ) + 1;
	_LIT8 ( KUriTblInsertStmt, "INSERT INTO %S VALUES (:V1, :V2, :V3, :V4, :V5, :V6, :V7, :V8)" );
	
	RBuf8 sqlStmt;
	sqlStmt.CreateL ( KMaxDbStmtLen );
	CleanupClosePushL ( sqlStmt );
	sqlStmt.Format ( KUriTblInsertStmt(), &(KUriTblName()) );
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( sqlStmt );
	CleanupStack::PopAndDestroy (); // sqlStmt
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	// We are having 8 parameters for this query. First bind the query with parameter position 0 
	// and so on
	dbTrans->BindIntL ( URILIST::EId, aUriId ); 
	dbTrans->BindTextL ( URILIST::EScheme, aUri.IsPresent(EUriScheme) ? aUri.Extract ( EUriScheme ) : KNullDesC8() );
	dbTrans->BindTextL ( URILIST::EUserInfo, aUri.IsPresent(EUriUserinfo) ? aUri.Extract ( EUriUserinfo ) : KNullDesC8() );
	dbTrans->BindTextL ( URILIST::EHost, aUri.IsPresent(EUriHost) ? aUri.Extract ( EUriHost ) : KNullDesC8() ); 
	dbTrans->BindTextL ( URILIST::EPort, aUri.IsPresent(EUriPort) ? aUri.Extract ( EUriPort ) : KNullDesC8() );
	dbTrans->BindTextL ( URILIST::EPath, aUri.IsPresent(EUriPath) ? aUri.Extract ( EUriPath ) : KNullDesC8() ); 
	dbTrans->BindTextL ( URILIST::EQuery, aUri.IsPresent(EUriQuery) ? aUri.Extract ( EUriQuery ) : KNullDesC8() );  
	dbTrans->BindTextL ( URILIST::EFragments, aUri.IsPresent(EUriFragment) ? aUri.Extract ( EUriFragment ) : KNullDesC8() );  				

	TInt result = dbTrans->ExecuteL ();
	CleanupStack::PopAndDestroy (); //dbTrans
	return result;
	}

/**
Inserts the data into the TLDTbl. 

@return Returns the number of rows inserted. Normally 1. Otherwise returns with
		system-wide error code.
*/
TInt CUriListInterface::InsertIntoTldTblL ( const CTldProperties& aTldProps )
	{
	_LIT8 ( KTldTblInsertStmt, "INSERT INTO %S VALUES ( :V1, :V2, :V3 )" );
	RBuf8 sqlStmt;
	sqlStmt.CreateL ( KMaxDbStmtLen );
	CleanupClosePushL ( sqlStmt );
	sqlStmt.Format ( KTldTblInsertStmt(), &(KTldTblName()) );
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( sqlStmt );
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	dbTrans->BindTextL ( TLDLIST::ETLDName, aTldProps.TldName() );
	TInt lt = static_cast<TInt> ( aTldProps.ListType() ) ;
	dbTrans->BindIntL ( TLDLIST::EListType, lt );
	dbTrans->BindTextL ( TLDLIST::ECharacterSet, aTldProps.CharacterSet() );
	TInt result = dbTrans->ExecuteL ();
	CleanupStack::PopAndDestroy (2); // sqlStmt, dbTrans
	return result;
	}
	
/**
Checks whether the duplicate entry in the TLDTbl. 

@return ETrue if already such row exist else retrns EFalse
*/
TBool CUriListInterface::IsDuplicateEntryL ( const CTldProperties& aTldProps )
	{
	_LIT8 ( KTldTblSearchStmt, "SELECT * FROM %S WHERE %S=:V1 and %S=:V2" );
	const TDesC8& KTldName (iStringPool.String (TLDLIST::ETLDName,TLDLIST::Table).DesC());
	const TDesC8& KTldListType (iStringPool.String (TLDLIST::EListType,TLDLIST::Table).DesC());
	RBuf8 sqlStmt;
	sqlStmt.CreateL ( KMaxDbStmtLen );
	CleanupClosePushL ( sqlStmt );
	sqlStmt.Format ( KTldTblSearchStmt(), &(KTldTblName()), &KTldName, &KTldListType  );
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( sqlStmt );
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	dbTrans->BindTextL ( TLDLIST::ETLDName, aTldProps.TldName() );
	TInt lt = static_cast<TInt> ( aTldProps.ListType() ) ;
	dbTrans->BindIntL ( TLDLIST::EListType, lt );
	TBool result( dbTrans->Next () );
	CleanupStack::PopAndDestroy (2); // sqlStmt, dbTrans
	return result;
	}

/**
Inserts the data into the URIPropertiesTbl. 

@param aUri URI object
@param [out] aUriId The URI will be filled upon successful insertion. This ID
	uniquelly indentifies the URI.

@return Returns the number of rows inserted. Normally 1. Otherwise returns with
		system-wide error code.
*/
TInt CUriListInterface::InsertIntoUriPropsTblL ( TInt aUriId, const CInetUriProperties& aUriProps, TInt& aPropId )
	{
	const TInt KMaxLen = 128;
	const TDesC8& propId ( iStringPool.String (URILIST::EPropId,URILIST::Table).DesC() );
	aPropId = GetMaxIdValueL ( KUriPropsTblName(), propId ) + 1;
	_LIT8 ( KInsertStmt, "INSERT INTO %S VALUES (%d, %d, %d, %d, %d, :V1)");
	RBuf8 queryBuf;
	CleanupClosePushL (queryBuf);
	queryBuf.CreateL ( KMaxLen );
	queryBuf.AppendFormat ( KInsertStmt, &(KUriPropsTblName()), aUriId, aPropId, aUriProps.ServiceType(), aUriProps.ListType(), aUriProps.Permission() );
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( queryBuf );
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	dbTrans->BindTextL ( 0, aUriProps.FavouriteName () ); // Bind with parameter index 0
	TInt result = dbTrans->ExecuteL ();
	CleanupStack::PopAndDestroy (2); // queryBuf, dbTrans
	
	return result;
	}

/**
Insert the data into the view. The view insertion will happen via a trigger. 
The SQLite views are read-only and cannot do any INSERT/DELETE/UPDATE operation directly.

@param aInetUri The URI information

@return KErrNone for success else return any of the system wide error code.

Note: The view insertion execution will not give the number of rows actualy 
inserted. It returns KErrNone
*/
TInt CUriListInterface::InsertIntoViewL ( const CInetUriImpl& aInetUri )
	{
	_LIT8 ( KUriViewInsertStmt, "INSERT INTO %S \
								VALUES (:V1, :V2, :V3, :V4, :V5, :V6, :V7, :V8, \
								:V9, :V10, :V11, :V12, :V13)" );
	
	RBuf8 sqlStmt;
	sqlStmt.CreateL ( KMaxDbStmtLen );
	CleanupClosePushL ( sqlStmt );
	sqlStmt.Format ( KUriViewInsertStmt(), &(KViewName()));
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( sqlStmt );
	CleanupStack::PopAndDestroy (); //sqlStmt
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	// We are having 8 parameters for this query. First bind the query with parameter position 0 
	// and so on
	dbTrans->BindIntL ( URILIST::EId, aInetUri.UriId() ); 
	TUriC8 uri ( aInetUri.Uri().Uri() );
	dbTrans->BindTextL ( URILIST::EScheme, uri.IsPresent(EUriScheme) ? uri.Extract ( EUriScheme ) : KNullDesC8() );
	dbTrans->BindTextL ( URILIST::EUserInfo, uri.IsPresent(EUriUserinfo) ? uri.Extract ( EUriUserinfo ) : KNullDesC8() );
	dbTrans->BindTextL ( URILIST::EHost, uri.IsPresent(EUriHost) ? uri.Extract ( EUriHost ) : KNullDesC8() ); 
	dbTrans->BindTextL ( URILIST::EPort, uri.IsPresent(EUriPort) ? uri.Extract ( EUriPort ) : KNullDesC8() );
	dbTrans->BindTextL ( URILIST::EPath, uri.IsPresent(EUriPath) ? uri.Extract ( EUriPath ) : KNullDesC8() ); 
	dbTrans->BindTextL ( URILIST::EQuery, uri.IsPresent(EUriQuery) ? uri.Extract ( EUriQuery ) : KNullDesC8() );  
	dbTrans->BindTextL ( URILIST::EFragments, uri.IsPresent(EUriFragment) ? uri.Extract ( EUriFragment ) : KNullDesC8() );  

	CInetUriProperties& properties = aInetUri.Properties ();
	dbTrans->BindIntL ( URILIST::EPropId - 1, properties.PropId() );
	dbTrans->BindIntL ( URILIST::EServiceType - 1, properties.ServiceType() );
	dbTrans->BindIntL ( URILIST::EListType - 1, properties.ListType() );
	dbTrans->BindIntL ( URILIST::EPermission - 1, properties.Permission() );
	dbTrans->BindTextL ( URILIST::EFavouriteName-1, properties.FavouriteName() );

	TInt result = dbTrans->ExecuteL ();
	CleanupStack::PopAndDestroy (); //dbTrans	
	return result;	
	}


/**
Updates the URI properties

@param aSchemaName Schema name - Could be table or view name
@param aPropId Property ID 
@param aListType new List type
@param aFavouritename new favourite name.
*/
TInt CUriListInterface::UpdateL ( const TDesC8& aSchemaName, TInt aPropId, InetUriList::TListType aListType, const TDesC8& aFavouriteName )
	{
	const TDesC8& propId ( iStringPool.String (URILIST::EPropId,URILIST::Table).DesC() );
	const TDesC8& listType (iStringPool.String (URILIST::EListType,URILIST::Table).DesC());
	const TDesC8& favouriteName (iStringPool.String (URILIST::EFavouriteName,URILIST::Table).DesC());
	
	// Only update List Type & Favourite Name. Other properties are not allowed to update.
	_LIT8 ( KUpdateStmt, "UPDATE %S SET %S=%d, %S=:V1 where %S=%d" );
	const TInt KMaxLen = 128;
	RBuf8 queryBuf;
	CleanupClosePushL ( queryBuf );
	queryBuf.CreateL ( KMaxLen );
	queryBuf.AppendFormat ( KUpdateStmt(), &aSchemaName, &listType, aListType, &favouriteName, &propId, aPropId );
	
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( queryBuf );
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	dbTrans->BindTextL ( 0, aFavouriteName );
	TInt result = dbTrans->ExecuteL ();
	CleanupStack::PopAndDestroy (2); // dbTrans, queryBuf
	return result;
	}

/**
Counts the number of URIs present in the storage for a given service type & list type
*/
TInt CUriListInterface::CountUriL ( InetUriList::TServiceType aServiceType, InetUriList::TListType aListType )
	{

	TQueryArgs args ( aServiceType, aListType );
	CUriQueryFilter* queryFilter = QueryL ( args );
	CleanupStack::PushL ( queryFilter );
	TInt count = queryFilter->MatchRecordCountL();
	CleanupStack::PopAndDestroy (); // queryFilter
	return count;
	}

/**
Returns the List type associated with the URI and a servicetype
*/
InetUriList::TListType CUriListInterface::GetListTypeL ( const TDesC8& aUri, InetUriList::TServiceType aSt )
	{
	TUriParser8 uriParser;
	User::LeaveIfError ( uriParser.Parse ( aUri ) );
	
	MDBTransaction* dbTrans = DoSearchExactUriLC ( uriParser, aSt );
	
	if ( !dbTrans->Next() )
		{		
		User::LeaveIfError ( InetUriList::KErrUriNotFound );
		}	
	InetUriList::TListType lt = static_cast < InetUriList::TListType > ( dbTrans->ColumnIntL ( URILIST::EListType - 1 ) );
	CleanupStack::PopAndDestroy (); // dbTrans
	return lt;
	}

/**
Delete the information from the given schema

@param aSchemaName Table or view name
@param aFieldPos The actual string index in the string table
@param aid ID

@return 
*/
void CUriListInterface::DeleteL ( const TDesC8& aSchemaName, TInt aFieldPos, TInt aId )
	{
	const TInt KMaxLen = 64;
	_LIT8 ( KDeleteStmt, "DELETE FROM %S where %S=%d" );
	
	const TDesC8& fieldName ( iStringPool.String (aFieldPos,URILIST::Table).DesC() );
	
	RBuf8 queryBuf;
	CleanupClosePushL ( queryBuf );
	queryBuf.CreateL ( KMaxLen );
	queryBuf.AppendFormat ( KDeleteStmt(), &aSchemaName, &fieldName, aId );

	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( queryBuf );
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	dbTrans->ExecuteL ();
	CleanupStack::PopAndDestroy ( 2 ); // queryBuf, dbTrans	
	}

/**
The URI can be added for different servicetypes. Check if it is exist in the table.

*/
TBool CUriListInterface::CheckDependencyL ( TInt aId )
	{
	const TDesC8& id ( iStringPool.String (URILIST::EId,URILIST::Table).DesC() );	
	const TInt KMaxLen = 64;
	_LIT8 ( KQueryStmt, "SELECT * FROM %S where %S=%d" );
	RBuf8 queryBuf;
	CleanupClosePushL ( queryBuf );
	queryBuf.CreateL ( KMaxLen );
	queryBuf.AppendFormat ( KQueryStmt(), &(KUriTblName()), &id, aId );
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( queryBuf );
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	TBool result = dbTrans->Next();
	CleanupStack::PopAndDestroy (2); // queryBuf, dbTrans
	return result;
	}


/**
Get the Maximum Id value from the table.

*/
TInt CUriListInterface::GetMaxIdValueL ( const TDesC8& aTblName, const TDesC8& aFieldName )
	{
	const TInt KMaxLen = 64;
	_LIT8 ( KMaxValueQuery, "SELECT MAX(%S) from %S" );
	RBuf8 queryBuf;
	CleanupClosePushL ( queryBuf );
	queryBuf.CreateL ( KMaxLen );
	queryBuf.AppendFormat ( KMaxValueQuery, &aFieldName, &aTblName );
	TInt result = iDbAccessor->ExecuteScalarQueryL ( queryBuf );
	CleanupStack::PopAndDestroy (); //queryBuf 
	return result;
	}

/**
Destroys the transaction object
*/
void CUriListInterface::DestroyTransObj ( TAny* aPtr )
	{
	MDBTransaction* trans =	(MDBTransaction*)aPtr;
	trans->Release();
	}
/**
Rollback the transaction when a leave happens in insert/delete/update operation
*/
void CUriListInterface::RollbackTrans ( TAny* aPtr )
	{
	MDBAccessor* accessor = (MDBAccessor*)aPtr;
	accessor->RollbackTransaction();
	}

/**
Query for the URI based on service type and listtype. The servicetype and listtype 
will be extracted from aQueryArgs
*/
CUriQueryFilter* CUriListInterface::QueryL ( const TQueryArgs& aQueryArgs )
	{
	const TDesC8& serviceType (iStringPool.String (URILIST::EServiceType,URILIST::Table).DesC());
	const TDesC8& listType (iStringPool.String (URILIST::EListType,URILIST::Table).DesC());

	RBuf8 queryBuf; 
	InitViewQueryLC (queryBuf);
	
	TInt st = aQueryArgs.Get ( TQueryArgs::EServiceType );
	TInt lt = aQueryArgs.Get ( TQueryArgs::EListType );

	if ( lt == KErrNotFound )
		{
		_LIT8 (KFmtStr, "%S=%d" );
		queryBuf.AppendFormat ( KFmtStr, &serviceType, st );

		}
	else
		{
		_LIT8 (KFmtStr, "%S=%d and %S=%d" );
		queryBuf.AppendFormat ( KFmtStr, &serviceType, st, &listType, lt );
		}

	
	// Execute the query
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( queryBuf );
	CleanupStack::PopAndDestroy (); //queryBuf
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	CUriQueryFilter* queryFilter = CUriQueryFilter::NewL ( dbTrans );
	CleanupStack::Pop (); // dbTrans
	
	return queryFilter;	// return the queryFilter
	}

/**
Fetches the length of the policy data if exists. 
*/
TInt CUriListInterface::PolicyDataLengthL ( const TDesC8& aUri, const TPolicyQueryArgs& aQueryArgs )
	{
	CUriQueryFilter* queryFilter = QueryWithTldL ( aUri, aQueryArgs );
	CleanupStack::PushL(queryFilter);
	MDBTransaction& dbTrans = queryFilter->DBTransaction();
	TInt resultsLength(0);
	if ( dbTrans.Next() && queryFilter->MatchRecordL() )
		{
		resultsLength = dbTrans.ColumnTextL ( TLDLIST::ECharacterSet ).Length();
		}
	else
		{
		//Requested Policydata Not available for this TLD
		CleanupStack::PopAndDestroy(queryFilter);
		User::Leave(InetUriList::KErrInvalidTLD);
		}
	CleanupStack::PopAndDestroy(queryFilter);
	return resultsLength;
	}

/**
Fetches the host type of given uri. 
*/
InetUriList::TListType CUriListInterface::GetHostTypeL (const TDesC8& aUri )
	{
	TBool blackListed( EFalse );
	TInt err(KErrNone);
	TRAP(err, blackListed = IsBlackListedUriL(aUri) );
	if( err == InetUriList::KErrInvalidTLD )
		{
		//No Blacklisted policy data avaialble
		TBool whiteListed( EFalse );
		whiteListed = IsWhiteListedUriL(aUri);
		blackListed = whiteListed? EFalse : ETrue;
		}
	//Bundle output Args with "blackListed" flag
	return blackListed ? InetUriList::EBlackList : InetUriList::EWhiteList ;
	}

HBufC8* CUriListInterface::QueryTldInfoL(const TDesC8& aUri, const TPolicyQueryArgs& aQueryArgs )
	{
	HBufC8* charSet(NULL);
	CUriQueryFilter* queryFilter = QueryWithTldL ( aUri, aQueryArgs );
	CleanupStack::PushL(queryFilter);
	MDBTransaction& dbTrans = queryFilter->DBTransaction();
	if ( dbTrans.Next() && queryFilter->MatchRecordL() )
		{
		charSet = dbTrans.ColumnTextL ( TLDLIST::ECharacterSet ).AllocL();
		}
	else
		{
		//Requested Policydata Not available for this TLD
		CleanupStack::PopAndDestroy(queryFilter);
		User::Leave(InetUriList::KErrInvalidTLD);
		}
	CleanupStack::PopAndDestroy(queryFilter);
	return charSet;
	}
	
/**
Query for the Tld based on listtype. The listtype will be extracted from aQueryArgs
*/
CUriQueryFilter* CUriListInterface::QueryWithTldL (const TDesC8& aUri,  const TPolicyQueryArgs& aQueryArgs )
	{
	//Etract Tld
	HBufC8* tld = ExtractTldL(aUri);
	CleanupStack::PushL(tld); 
	//execute Query Tld
	CUriQueryFilter* queryFilter = DoQueryWithTldL ( tld->Des(), aQueryArgs );
	CleanupStack::PopAndDestroy(); //tld
	return queryFilter;
	}

/**
Query for the Tld based on listtype. The listtype will be extracted from aQueryArgs
*/
CUriQueryFilter* CUriListInterface::DoQueryWithTldL ( const TDesC8& aUri, const TPolicyQueryArgs& aQueryArgs )
	{
	InetUriList::TTLDQueryType matchType(InetUriList::EPolicyListType);
	#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
	if ( aQueryArgs.Get ( TPolicyQueryArgs::ETldQueryType ) != KErrNotFound)
		{
		matchType = static_cast < InetUriList::TTLDQueryType > ( aQueryArgs.Get ( TPolicyQueryArgs::ETldQueryType ) );	
		}
	#else
	if ( aQueryArgs.IsSet ( TPolicyQueryArgs::ETldQueryType ) )
		{
		matchType = static_cast < InetUriList::TTLDQueryType > ( aQueryArgs.Get ( TPolicyQueryArgs::ETldQueryType ) );	
		}
	#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
	__ASSERT_ALWAYS( ( matchType == InetUriList::EPolicyListType || matchType == InetUriList::EPolicyCharSet ), User::Invariant() );
	
	const TDesC8& tldName (iStringPool.String (TLDLIST::ETLDName,TLDLIST::Table).DesC());
	const TDesC8& listType (iStringPool.String (TLDLIST::EListType,TLDLIST::Table).DesC());
	
	RBuf8 queryBuf; 
	InitTldQueryLC (queryBuf);
	//Do not change the order of this query. Index is created on tldname and list type
	_LIT8(KSql, "( %S=:V1 and  %S=:V2 ) or ( %S=:V3 and %S=:V2 )" );
	queryBuf.AppendFormat( KSql, &tldName, &listType, &tldName, &listType );

	//Prepare the query
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( queryBuf );
	CleanupStack::PopAndDestroy (); //queryBuf
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	TInt lt = aQueryArgs.Get ( TPolicyQueryArgs::ETldListType );
	RBuf8 upperCaseUri;
	CleanupClosePushL(upperCaseUri);
	upperCaseUri.CreateL(aUri);
	upperCaseUri.UpperCase();
	dbTrans->BindTextL(0, aUri);
	dbTrans->BindIntL(1, lt);
	dbTrans->BindTextL(2, upperCaseUri );
	
	// Execute the query
	CUriQueryFilter* queryFilter = CTldPolicyDataFilter::NewL ( dbTrans );
	CleanupStack::PopAndDestroy (); //upperCaseUri
	CleanupStack::Pop (); // dbTrans
	return queryFilter;	// return the queryFilter
	}
	
/**
Query the list with the URI and other specified parameters in the aQueryArgs
*/
CUriQueryFilter* CUriListInterface::QueryWithUriL ( const TDesC8& aUri, const TQueryArgs& aQueryArgs )
	{
	TUriParser8 uriParser;
	User::LeaveIfError ( uriParser.Parse ( aUri ) );

	MDBTransaction* dbTrans = NULL;
	CUriQueryFilter* queryFilter = NULL;

	TInt serviceType = aQueryArgs.Get ( TQueryArgs::EServiceType );
	TInt listType = aQueryArgs.Get ( TQueryArgs::EListType );	
	InetUriList::TURIMatch matchType = 	InetUriList::EExact;
	#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
	if ( aQueryArgs.Get ( TQueryArgs::EURIMatch ) != KErrNotFound)
		matchType = static_cast < InetUriList::TURIMatch > ( aQueryArgs.Get ( TQueryArgs::EURIMatch ) );
	#else
	if ( aQueryArgs.IsSet ( TQueryArgs::EURIMatch ) )
		matchType = static_cast < InetUriList::TURIMatch > ( aQueryArgs.Get ( TQueryArgs::EURIMatch ) );
	#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS
	switch ( matchType )
		{
		case InetUriList::EExact:
		dbTrans = DoSearchExactUriLC ( uriParser, serviceType, listType );
		queryFilter = CUriQueryFilter::NewL ( dbTrans );
		break;	

		case InetUriList::EDomain:
		dbTrans = DoSearchUriDomainLC ( uriParser, serviceType, listType );
		queryFilter = CUriDomainFilter::NewL ( dbTrans, uriParser.Extract( EUriHost ) );
		break;

		case InetUriList::EExactPath:
		dbTrans = DoSearchUriPathLC (uriParser, matchType, serviceType, listType );
		queryFilter = CUriQueryFilter::NewL ( dbTrans );
		break;

		case InetUriList::EPartialPrefixPath:
		case InetUriList::EPartialSuffixPath:
		dbTrans = DoSearchUriPathLC (uriParser, matchType, serviceType, listType );
		queryFilter = CUriPathFilter::NewL ( dbTrans, matchType, uriParser.Extract ( EUriPath ) );
		break;
		default:
		User::Invariant ();			
		}
	CleanupStack::Pop (); // dbTrans
	return queryFilter;
	}

/**
Do a exact match in the list for the given uri and servicetype. All URI components
will be matched
*/
MDBTransaction* CUriListInterface::DoSearchExactUriLC ( const TUriC8& aUri, TInt aServiceType, TInt aListType /* = KErrNotFound */ )
	{
	const TDesC8& serviceType (iStringPool.String (URILIST::EServiceType,URILIST::Table).DesC());
	const TDesC8& listType (iStringPool.String (URILIST::EListType,URILIST::Table).DesC());

	RBuf8 queryBuf;
	InitViewQueryLC(queryBuf);
	
	AppendFieldName ( queryBuf, URILIST::EScheme, aUri, EUriScheme );
	AppendFieldName ( queryBuf, URILIST::EUserInfo, aUri, EUriUserinfo );
	AppendFieldName ( queryBuf, URILIST::EHost, aUri, EUriHost );	
	AppendFieldName ( queryBuf, URILIST::EPort, aUri, EUriPort );	
	AppendFieldName ( queryBuf, URILIST::EPath, aUri, EUriPath );	
	AppendFieldName ( queryBuf, URILIST::EQuery, aUri, EUriQuery );	
	AppendFieldName ( queryBuf, URILIST::EFragments, aUri, EUriFragment );	
	if ( aListType == KErrNotFound )	
		{
		_LIT8 ( KStr, "%S=%d" );
		queryBuf.AppendFormat( KStr(), &serviceType, aServiceType );
		}		
	else
		{
		_LIT8 ( KStrWithListType, "%S=%d and %S=%d" );
		queryBuf.AppendFormat( KStrWithListType(), &serviceType, aServiceType, &listType, aListType );			
		}
	TInt bindPos = 0;

	// Prepare the transaction.
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( queryBuf );
	CleanupStack::PopAndDestroy ();  // queryBuf
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	
	if ( aUri.IsPresent(EUriScheme) && aUri.Extract(EUriScheme).Length() )
		dbTrans->BindTextL ( bindPos++, aUri.Extract(EUriScheme) );
	if ( aUri.IsPresent(EUriUserinfo) && aUri.Extract(EUriUserinfo).Length() )
		dbTrans->BindTextL ( bindPos++, aUri.Extract(EUriUserinfo) );
	if ( aUri.IsPresent(EUriHost) && aUri.Extract(EUriHost).Length() )
		dbTrans->BindTextL ( bindPos++, aUri.Extract(EUriHost) );
	if ( aUri.IsPresent(EUriPort) && aUri.Extract(EUriPort).Length() )
		dbTrans->BindTextL ( bindPos++, aUri.Extract(EUriPort) );
	if ( aUri.IsPresent(EUriPath) && aUri.Extract(EUriPath).Length() )
		dbTrans->BindTextL ( bindPos++, aUri.Extract(EUriPath) );
	if ( aUri.IsPresent(EUriQuery) && aUri.Extract(EUriQuery).Length() )
		dbTrans->BindTextL ( bindPos++, aUri.Extract(EUriQuery) );
	if ( aUri.IsPresent(EUriFragment) && aUri.Extract(EUriFragment).Length() )
		dbTrans->BindTextL ( bindPos++, aUri.Extract(EUriFragment) );
	
	return dbTrans;			
	}

/**
Do a path match. Path match can be EExactPath, EPartialSuffixPath or EPartialPrefixPath.
Only path and scheme components of the URI is considered for this query 

Actual filtering of the query results will be done CUriPathFilter class while sending the results.
*/
MDBTransaction* CUriListInterface::DoSearchUriPathLC ( const TUriC8& aUri, InetUriList::TURIMatch aMatchType, TInt aServiceType, TInt aListType /* = KErrNotFound */ )
	{
	const TDesC8& serviceType (iStringPool.String (URILIST::EServiceType,URILIST::Table).DesC());
	const TDesC8& listType (iStringPool.String (URILIST::EListType,URILIST::Table).DesC());
	const TDesC8& path ( iStringPool.String (URILIST::EPath,URILIST::Table).DesC() );
	const TDesC8& scheme ( iStringPool.String (URILIST::EScheme,URILIST::Table).DesC() );

	RBuf8 queryBuf;
	InitViewQueryLC(queryBuf);
	
	if ( aMatchType == InetUriList::EExactPath && aListType == KErrNotFound )	
		{
		// exact path without listtype
		_LIT8 ( KFmtStr, "%S=? and %S=%d and %S=?" );
		queryBuf.AppendFormat ( KFmtStr(), &scheme, &serviceType, aServiceType, &path );
		}
	else if ( aMatchType == InetUriList::EExactPath )
		{
		// exact path with listtype			
		_LIT8 ( KFmtStr, "%S=? and %S=%d and %S=%d and %S=?" );
		queryBuf.AppendFormat ( KFmtStr(), &scheme, &serviceType, aServiceType, &listType, aListType, &path );		
		}
	else if ( aListType == KErrNotFound )
		{
		// partial path withoutlisttype
		_LIT8 ( KFmtStr, "%S=? and %S=%d" );	
		queryBuf.AppendFormat ( KFmtStr(), &scheme, &serviceType, aServiceType );
		}
	else
		{
		// partial path with listtype
		_LIT8 ( KFmtStr, "%S=? and %S=%d and %S=%d" );	
		queryBuf.AppendFormat ( KFmtStr(), &scheme, &serviceType, aServiceType, &listType, aListType );			
		}
	
	// Prepare the transaction.
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( queryBuf );
	CleanupStack::PopAndDestroy ();  // queryBuf
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	dbTrans->BindTextL ( 0, aUri.Extract ( EUriScheme ) ); // Bind the scheme
	if ( aMatchType == InetUriList::EExactPath )
		dbTrans->BindTextL ( 1, aUri.Extract ( EUriPath ) ); // Bind the path
	return dbTrans;
	}

/**
Do a domain search. Actual domain query ersults will be done in the CUriDomainFilter class
*/
MDBTransaction* CUriListInterface::DoSearchUriDomainLC ( const TUriC8& aUri, TInt aServiceType, TInt aListType /* = KErrNotFound */ )
	{
	const TDesC8& serviceType (iStringPool.String (URILIST::EServiceType,URILIST::Table).DesC());
	const TDesC8& listType (iStringPool.String (URILIST::EListType,URILIST::Table).DesC());
	const TDesC8& scheme ( iStringPool.String (URILIST::EScheme,URILIST::Table).DesC() );
	RBuf8 queryBuf;
	InitViewQueryLC(queryBuf);
	
	if ( aListType == KErrNotFound )	
		{
		_LIT8 ( KStr, "%S=? and %S=%d" );
		queryBuf.AppendFormat( KStr(), &scheme, &serviceType, aServiceType );
		}		
	else
		{
		_LIT8 ( KStrWithListType, "%S=? and %S=%d and %S=%d" );
		queryBuf.AppendFormat( KStrWithListType(), &scheme, &serviceType, aServiceType, &listType, aListType );			
		}
	// Prepare the transaction.
	MDBTransaction* dbTrans = iDbAccessor->PrepareTransactionL ( queryBuf );
	CleanupStack::PopAndDestroy ();  // queryBuf
	CleanupStack::PushL ( TCleanupItem ( CUriListInterface::DestroyTransObj, dbTrans ) );
	dbTrans->BindTextL ( 0, aUri.Extract ( EUriScheme ) ); // Bind the scheme

	return dbTrans;				
	}


void CUriListInterface::InitViewQueryLC (  RBuf8& aBuf )
	{
	_LIT8 ( KSelectStmt, "SELECT * FROM %S where ");
	CleanupClosePushL ( aBuf );
	aBuf.CreateL ( KMaxDbStmtLen );
	aBuf.AppendFormat ( KSelectStmt(), &(KViewName()) );
	}
	
void CUriListInterface::InitTldQueryLC (  RBuf8& aBuf )
	{
	_LIT8 ( KSelectStmt, "SELECT * FROM %S where ");
	CleanupClosePushL ( aBuf );
	aBuf.CreateL ( KMaxDbStmtLen );
	aBuf.AppendFormat ( KSelectStmt(), &(KTldTblName()) );
	}

void CUriListInterface::AppendFieldName ( RBuf8& aBuf, TInt aTableIndex, const TUriC8& aUri, TUriComponent aComponent )
	{
	_LIT8 ( KNormalStr, "%S=? and " );
	_LIT8 ( KIsNullStr, "%S ISNULL and " );
	
	const TPtrC8 value ( aUri.IsPresent ( aComponent ) ? aUri.Extract ( aComponent ) : KNullDesC8() );
	if ( value.Length() > 0 )
		{
		aBuf.AppendFormat ( KNormalStr(), &(GetFieldName(aTableIndex)) );
		}
	else
		{
		aBuf.AppendFormat ( KIsNullStr(), &(GetFieldName(aTableIndex)) );
		}		
	}

const TDesC8& CUriListInterface::GetFieldName ( TInt aTableIndex )
	{
	return iStringPool.String ( aTableIndex,URILIST::Table ).DesC();	
	}

void CUriListInterface::BindTextL ( MDBTransaction& aDbTrans, TInt aBindPos, const TUriC8& aUri, TUriComponent aComponent )
	{
	TPtrC8 valuePtr ( KNullDesC8() );
	if ( aUri.IsPresent (aComponent) )
		valuePtr.Set( aUri.Extract ( aComponent ) );
	
	aDbTrans.BindTextL ( aBindPos, (valuePtr.Length() == 0) ? KNullDesC8() : valuePtr );
	}

/**
Initializes the database(if it is empty) from the pre-configured XML file. 

*/
void CUriListInterface::InitializeDatabaseL ()
	{
	const TDesC8& id ( iStringPool.String (URILIST::EId,URILIST::Table).DesC() );
	if ( GetMaxIdValueL ( KViewName(), id ) != 0 )
		{
		// Database already created and thre are some values already added.
		//Update with Policyfile installed, after rebooting the server.
		UpgradePolicyDatabaseL();
		return;
		}
	// Otherwise go and read the XML file, if exists and populate the data fields
	TBuf<KMaxLength> uriListFilePath;
	uriListFilePath.FillZ();
	//if uriListFilePath is Null then SIS file is not installed
	GetInstalledDirectoryL( TUid::Uid(pkgId), KUriListFile(), uriListFilePath );
	CUriListInitializer* listInitializer = CUriListInitializer::NewL ( *this );
	CleanupStack::PushL ( listInitializer );
	TInt err( KErrNone );
	if(uriListFilePath.CompareF(KNullDesC) != 0)
		{
		//File ineturilist.xml exist in external path.
		TRAP ( err, listInitializer->ParseDocumentL ( uriListFilePath ) );	
		}
	CleanupStack::PopAndDestroy (); // listInitializer
	if ( err != KErrNotFound && err != KErrPathNotFound ) 
		{
		// File found and some other error. leave
		User::LeaveIfError ( err );
		}
	//initialise Policy data
	InitializePolicyDatabaseL();
	}


/**
Initializes the database to default policy data if it is available in ROM.
If a valid upgraded policy file exist then, database will be updated to later version.
*/
void CUriListInterface::InitializePolicyDatabaseL ()
	{
	//Install the file with ROM Policy data if ROM file exists	
	_LIT( KDefaultFilePath, "z:\\private\\20009d70\\tldpolicy.xml");
	if( FileExist( KDefaultFilePath() ) )
		{
		CTldListInitializer* tldlistInitializer = CTldListInitializer::NewL ( *this );
		CleanupStack::PushL ( tldlistInitializer );
		TRAPD ( policyfileerr, tldlistInitializer->ParseDocumentL ( KDefaultFilePath() ) );
		CleanupStack::PopAndDestroy ();	//tldlistInitializer
		if ( policyfileerr != KErrNotFound && policyfileerr != KErrPathNotFound ) // File found and some other error. leave
			{
			User::LeaveIfError ( policyfileerr );	
			}
		}

	UpgradePolicyDatabaseL();
	}

/**
Database will be updated with the Policy file installed.
*/
void CUriListInterface::UpgradePolicyDatabaseL()
	{
	//Upgrade DB if Policy file is installed
	//Get the Installed file path
	_LIT(KTldPolicyFile, "c:\\private\\20009d70\\tldpolicy.xml");
	if(!FileExist( KTldPolicyFile() ))
		{
		return;	
		}
	//Upgrade DB
	CTldListInitializer* tldListUpgrader = CTldListInitializer::NewL ( *this );
	CleanupStack::PushL ( tldListUpgrader );	
	TRAPD ( fileerr, tldListUpgrader->ParseDocumentL ( KTldPolicyFile() ) );
	CleanupStack::PopAndDestroy ();//tldListUpgrader
	if ( fileerr != KErrNotFound && fileerr != KErrPathNotFound ) // File found and some other error. leave
		{
		User::LeaveIfError ( fileerr );	
		}
	}

/**
Get the external file path installed out of ROM.
out parameter -- aFilePath
*/
void CUriListInterface::GetInstalledDirectoryL(const TUid aUid, const TDesC& aFile, TDes& aFilePath )
	{
	__ASSERT_DEBUG( aFile.CompareF( KNullDesC ) !=0, User::Invariant( ) );
	Swi::RSisRegistrySession session;
	if(session.Connect( ) != KErrNone)
		{
		//Sis Not Installed
		session.Close();
		return;
		}
	CleanupClosePushL( session );
	TBool installed = session.IsInstalledL( aUid );
	Swi::RSisRegistryEntry  registryEntry;
    TInt error = registryEntry.Open( session, aUid );
    CleanupClosePushL( registryEntry );
    TBool available = EFalse;
   	if ( error == KErrNone )
	   	{
		RPointerArray<HBufC> files;
		// get list of files
		TRAPD( err, registryEntry.FilesL( files ) );
		if (err != KErrNone)
			{
			files.ResetAndDestroy();
			CleanupStack::PopAndDestroy(2);	// &registryEntry, &session
			User::Leave(err);
			}
		TInt filesCount = files.Count();
		for (TInt index = 0; index < filesCount; index++)
			{
			TBuf<KMaxLength> fileNameBuf(KNullDesC);
			fileNameBuf.Copy(files[index]->Des());
			TInt pos = fileNameBuf.FindF(aFile);
			if( pos >= 0 && !available && fileNameBuf.Left(pos).CompareF(KRomDirectory)!=0 )
				{
				aFilePath.Copy(fileNameBuf);
				available = ETrue;
				break;
				}
			}
		files.ResetAndDestroy();
		}
	CleanupStack::PopAndDestroy(2);	// &registryEntry, &session	
	}

/**
Checks if the specified file exists.

@param aFileName File to check
@return ETrue if the file exists, otherwise EFalse
*/
TBool CUriListInterface::FileExist(const TDesC& aFilePath)
	{
	RFs iFs;
	TBool result( EFalse );
	TInt error = iFs.Connect();
	if( !error )
		{
		result = BaflUtils::FileExists( iFs, aFilePath );
		}
	iFs.Close();
	return result;
	}

/**
return ETrue if Uri is BlackListed, else return EFalse
Leaves with KErrNotSupported if No Policydata is available
*/
TBool CUriListInterface::IsBlackListedUriL( const TDesC8& aUri )
	{
	TBool blackListed(EFalse);
	TPolicyQueryArgs blackListArgs ( InetUriList::EBlackList, InetUriList::EPolicyCharSet );
	CUriQueryFilter* queryFilter = QueryWithTldL ( aUri, blackListArgs );
	CleanupStack::PushL(queryFilter);
	MDBTransaction& dbTrans = queryFilter->DBTransaction();
	if ( dbTrans.Next() && queryFilter->MatchRecordL() )
		{
		const TDesC8& charSet = dbTrans.ColumnTextL ( TLDLIST::ECharacterSet );
		//Policy file contains empty body for Requested white/Black listed data
		__ASSERT_ALWAYS( charSet.Length() > 0, User::Invariant() );
		RArray<TChar> policyCharList;
		CleanupClosePushL(policyCharList);
		//Tokenize the results
		TokenizeStringL( charSet, policyCharList);
		//Get the host and check whether any of these char is in PolicyCharList
		HBufC8* host = ExtractHostL(aUri);
		CleanupStack::PushL(host);
		blackListed = IsBlackListedHost( host->Des(), policyCharList ); 
		CleanupStack::PopAndDestroy(2);	//host, PolicyCharList
		}
	else
		{
		//Requested Policydata Not available for this TLD
		CleanupStack::PopAndDestroy(queryFilter);
		User::Leave(InetUriList::KErrInvalidTLD);
		}
	CleanupStack::PopAndDestroy(queryFilter);
	return blackListed;
	}

/**
return ETrue if Uri is WhiteListed, else return EFalse
Leaves with KErrNotSupported if No Policydata is available
*/
TBool CUriListInterface::IsWhiteListedUriL( const TDesC8& aUri )
	{
	TBool whiteListed (EFalse);
	TPolicyQueryArgs whiteListArgs ( InetUriList::EWhiteList, InetUriList::EPolicyCharSet );
	CUriQueryFilter* queryFilter = QueryWithTldL ( aUri, whiteListArgs );
	CleanupStack::PushL(queryFilter);
	MDBTransaction& dbTrans = queryFilter->DBTransaction();
	if ( dbTrans.Next() && queryFilter->MatchRecordL() )
		{
		const TDesC8& charSet = dbTrans.ColumnTextL ( TLDLIST::ECharacterSet );
		//Policy file contains empty body for Requested white/Black listed data
		__ASSERT_ALWAYS( charSet.Length() > 0, User::Invariant() );
		RArray<TChar> policyCharList;
		CleanupClosePushL(policyCharList);
		//Tokenize the results
		TokenizeStringL(charSet, policyCharList);
		//Get the host and check whether any of these char is in PolicyCharList
		HBufC8* host = ExtractHostL(aUri);
		CleanupStack::PushL(host);
		whiteListed = IsWhiteListedHostL( host->Des(), policyCharList ); 
		CleanupStack::PopAndDestroy(2);	// PolicyCharList,host
		}
	else
		{
		//Requested Policydata Not available for this TLD
		CleanupStack::PopAndDestroy(queryFilter);
		User::Leave(InetUriList::KErrInvalidTLD);
		}
	CleanupStack::PopAndDestroy(queryFilter);
	return whiteListed;
	}

/**
returns ETrue if aHost is white listed else returns EFalse
*/
TBool CUriListInterface::IsWhiteListedHostL( const TDesC8& aHost, RArray<TChar>& aPolicyList )
	{
	//Every character in the host must be in the list
	RArray<TChar> hostCharList;
	CleanupClosePushL(hostCharList);
	StringToCharListL(aHost, hostCharList);
	TBool whiteList(ETrue);
	for(TInt index(0); index < hostCharList.Count(); index++ )
		{
		if(!IsNextCharInList(hostCharList[index], aPolicyList))	
			{
			whiteList = EFalse;
			break;
			}
		}
	CleanupStack::PopAndDestroy();//hostCharList
	return whiteList;
	}

/**
Returns ETrue if aChar is in aPolicyList else returns EFalse
*/
TBool CUriListInterface::IsNextCharInList(TChar aChar, RArray<TChar>& aPolicyList)
	{
	TBool found(EFalse);
	for (TInt index(0); index < aPolicyList.Count(); index++)
		{
		if( aPolicyList[index] == aChar )	
			{
			found =  ETrue;	
			break;
			}
		}
	return found;
	}

/**
Tokenizes the policydata into characters list
*/
void CUriListInterface::TokenizeStringL(const TDesC8& aString, RArray<TChar>& aList, TChar aSeparator)
	{
	TLex8 lexer(aString);
	while(!lexer.Eos())
		{
		TChar charcter = lexer.Get();
		if ( charcter != aSeparator && charcter != '\n' &&  charcter != '\t' )
			{
			aList.AppendL(charcter);	
			}
		}
	}

/**
Tokenizes the Host into characters list
*/
void CUriListInterface::StringToCharListL(const TDesC8& aString, RArray<TChar>& aList, TChar aSeparator)
	{
	//Here sepearator is '.'
	TokenizeStringL( aString, aList, aSeparator );
	}

/**
returns ETrue if aHost is Black listed else returns EFalse
*/
TBool CUriListInterface::IsBlackListedHost( const TDesC8& aHost, RArray<TChar>& aList )
	{
	TBool result( EFalse );
	for(TInt index(0); index < aList.Count(); index++ )	
		{
		if ( aHost.Locate(aList[index]) >= 0	)
			{
			result = ETrue;
			break;			
			}
		}
	return result;
	}

/**
Extracts the TLD from the given uri.
*/
HBufC8* CUriListInterface::ExtractTldL( const TDesC8& aUri )
	{
	HBufC8* tld = ExtractHostL( aUri );
	CleanupStack::PushL( tld );
	TChar dot('.');
	TInt pos = tld->LocateReverse( dot );
	__ASSERT_DEBUG( ( pos >= 0 ) , User::Invariant() );
	HBufC8* tldPtr = tld->Mid( pos + 1 ).AllocL(); //Excluding dot '.'
	CleanupStack::PopAndDestroy( tld );
	return tldPtr;
	}
	
/**
Extracts the Host from the given uri.
*/
HBufC8* CUriListInterface::ExtractHostL( const TDesC8& aUri )
	{
	TUriParser8 tldUri1;
	RBuf8 tld;
	RBuf8 customisedUri;
	HBufC8* tldPtr;
	User::LeaveIfError ( tldUri1.Parse ( aUri ) );
	TBool isSchemePresent = tldUri1.IsPresent( EUriScheme );
	CleanupClosePushL ( tld );
	CleanupClosePushL ( customisedUri );
	if (!isSchemePresent)
	{
        _LIT8(KHttp, "http://");
	    RBuf8 urirbuf;
	    urirbuf.CleanupClosePushL();
	    urirbuf.CreateL(KHttp,aUri.Length()+7);
	    urirbuf.Append(aUri);
	    customisedUri.CreateL( DoNormalisationLC ( urirbuf ) );
	    TUriParser8 tldUri2;
	    User::LeaveIfError ( tldUri2.Parse ( customisedUri ) );
	    __ASSERT_ALWAYS(tldUri2.IsPresent( EUriHost ), User::Invariant());
	    tld.CreateL( tldUri2.Extract(EUriHost) );
	    tldPtr = tld.AllocL();
	    CleanupStack::PopAndDestroy(2); //calls aRBuf.Close()and DoNormalisationLC pop
	    
	 }
	else 
	{   
        customisedUri.CreateL( DoNormalisationLC ( aUri ) );
        User::LeaveIfError ( tldUri1.Parse ( customisedUri ) );
        __ASSERT_ALWAYS(tldUri1.IsPresent( EUriHost ), User::Invariant());
        tld.CreateL( tldUri1.Extract(EUriHost) );
        tldPtr = tld.AllocL();
        CleanupStack::PopAndDestroy ();//objects added in DoNormalisationLC
    }
	CleanupStack::PopAndDestroy (); //customisedUri
	CleanupStack::PopAndDestroy (); //tld
	 return tldPtr;
	}	

/**
Does the syntax based normalisation. If the MUriCustomiser is set then it customises the URI, 
that is, do the protocol/scheme-based normalisation on the URI. The customisation algorithm needs 
to be implemented by the application. 
*/
const TDesC8& CUriListInterface::DoNormalisationLC ( const TDesC8& aUri )
	{
	// Do a syntax based normalisation
	TUriParser8 uriParser;
	User::LeaveIfError ( uriParser.Parse ( aUri ) );
	CUri8* normalisedUri = UriUtils::NormaliseUriL ( uriParser );	
	CleanupStack::PushL ( normalisedUri );
	return 	normalisedUri->Uri().UriDes();	
	}

