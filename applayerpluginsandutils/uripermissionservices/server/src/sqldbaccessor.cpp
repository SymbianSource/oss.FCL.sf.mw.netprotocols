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

#include "sqldbaccessor.h"
#include "sqldbtransaction.h"
#include "urilist.h"
#include "tldlist.h"

#if defined (TESTDBNAME)
	_LIT( KDatabaseName, "[A00026FF]URIList.dat" );
#else
	_LIT( KDatabaseName, "[20009D70]URIList.dat" );	
#endif

CSqlDbAccessor::CSqlDbAccessor ()
	{
		
	}

CSqlDbAccessor::~CSqlDbAccessor ()
	{
	iDatabase.Close ();
	}

MDBAccessor* CSqlDbAccessor::NewL ( const RStringPool& aStringPool )
	{
	CSqlDbAccessor* self = new ( ELeave ) CSqlDbAccessor ();
	CleanupStack::PushL ( self );
	self->ConstructL ( aStringPool );
	CleanupStack::Pop ();
	
	return self;	
	}

void CSqlDbAccessor::ConstructL ( const RStringPool& aStringPool )
	{
	TInt result = iDatabase.Open ( KDatabaseName() );
	if ( result == KErrNotFound )
		{
		// Database is not existing. Create one.
		CreateL ( aStringPool );			
		}
	else 
		{
		User::LeaveIfError ( result );
		}
	}

/**
Creates a new database and its schemas.
*/		
void CSqlDbAccessor::CreateL ( const RStringPool& aStringPool )
	{
	// Create the URI List database. 
	
	// First create security policy	
	RSqlSecurityPolicy securityPolicy;
	CleanupClosePushL ( securityPolicy );
	
	// Create security policies container object using a default security policy.
	TSecurityPolicy defaultPolicy ( TSecurityPolicy::EAlwaysPass );
	User::LeaveIfError (securityPolicy.Create ( defaultPolicy )); 
	
	// Set up policy to apply to database schema and assign it
	TSecurityPolicy schemaPolicy ( TSecurityPolicy::EAlwaysPass );
	User::LeaveIfError (securityPolicy.SetDbPolicy ( RSqlSecurityPolicy::ESchemaPolicy, schemaPolicy ) );
	
	// Set up policy to apply to write activity on the database and assign it
	TSecurityPolicy writePolicy ( TSecurityPolicy::EAlwaysPass );
	User::LeaveIfError (securityPolicy.SetDbPolicy ( RSqlSecurityPolicy::EWritePolicy, writePolicy ));
	
	// Set up policy to apply to write activity to the database table named "URIList" and assign it
	TSecurityPolicy readPolicy ( TSecurityPolicy::EAlwaysPass );
	User::LeaveIfError (securityPolicy.SetDbPolicy(RSqlSecurityPolicy::EReadPolicy, readPolicy));
	
	User::LeaveIfError ( iDatabase.Create ( KDatabaseName, securityPolicy ) );	
	
	CreateSchemaL ( aStringPool );
	
	CleanupStack::PopAndDestroy (); // securityPolicy
	}
 
/**
Creates the schemas for the database. We create 2 tables ( URITbl & URIPropsTbl ), 1 view based 
on those 2 tables for easy retrieval of data and the triggers to update the view. In SQLite the
view updation is done via the triggers. By default in SQLite views are read-only
*/		
void CSqlDbAccessor::CreateSchemaL ( const RStringPool& aStringPool )
	{
	RBuf8 stmtBuf;
	CleanupClosePushL ( stmtBuf );
	stmtBuf.CreateL ( KMaxDbStmtLen );

	const TDesC8& id ( aStringPool.String (URILIST::EId,URILIST::Table).DesC() );
	const TDesC8& scheme ( aStringPool.String (URILIST::EScheme,URILIST::Table).DesC() );
	const TDesC8& userInfo ( aStringPool.String (URILIST::EUserInfo,URILIST::Table).DesC() );
	const TDesC8& host ( aStringPool.String (URILIST::EHost,URILIST::Table).DesC() );
	const TDesC8& port ( aStringPool.String (URILIST::EPort,URILIST::Table).DesC() );
	const TDesC8& path ( aStringPool.String (URILIST::EPath,URILIST::Table).DesC() );
	const TDesC8& query ( aStringPool.String (URILIST::EQuery,URILIST::Table).DesC() );
	const TDesC8& fragments ( aStringPool.String (URILIST::EFragments,URILIST::Table).DesC() );
	
	const TDesC8& RefId ( aStringPool.String (URILIST::EURIRefId,URILIST::Table).DesC() );
	const TDesC8& propId ( aStringPool.String (URILIST::EPropId,URILIST::Table).DesC() );
	const TDesC8& serviceType (aStringPool.String (URILIST::EServiceType,URILIST::Table).DesC());
	const TDesC8& listType (aStringPool.String (URILIST::EListType,URILIST::Table).DesC());
	const TDesC8& permission (aStringPool.String (URILIST::EPermission,URILIST::Table).DesC());
	const TDesC8& favouriteName (aStringPool.String (URILIST::EFavouriteName,URILIST::Table).DesC());
	
	const TDesC8& KTldname ( aStringPool.String (TLDLIST::ETLDName,TLDLIST::Table).DesC() );
	const TDesC8& KTldListType ( aStringPool.String (TLDLIST::EListType,TLDLIST::Table).DesC() );
	const TDesC8& KCharacterSet ( aStringPool.String (TLDLIST::ECharacterSet,TLDLIST::Table).DesC() );

	_LIT8 ( KURITblCreateStmt, "CREATE TABLE %S ( \
								%S INTEGR, \
								%S TEXT,\
								%S TEXT, \
								%S TEXT, \
								%S TEXT, \
								%S TEXT, \
								%S TEXT, \
								%S TEXT );" );

	_LIT8 ( KURIPropTblCreateStmt, "CREATE TABLE %S ( \
									%S INTEGER, \
									%S INTEGER,\
									%S INTEGER, \
									%S INTEGER, \
									%S INTEGER, \
									%S TEXT );" );

	_LIT8 ( KTldTblCreateStmt, "CREATE TABLE %S ( \
								%S TEXT,\
								%S INTEGER,\
								%S BLOB );" );
								
	
	_LIT8 ( KViewStmt, "CREATE VIEW %S AS SELECT \
					T1.%S %S, T1.%S %S, T1.%S %S, T1.%S %S, T1.%S %S, T1.%S %S, T1.%S %S, T1.%S %S, \
					T2.%S %S, T2.%S %S, T2.%S %S, T2.%S %S, T2.%S %S \
					FROM \
					%S T1, %S T2 \
					WHERE \
					T1.%S = T2.%S;" );

	_LIT8 ( KTriggerInsertStmt, "CREATE TRIGGER %S INSTEAD OF INSERT ON %S \
							BEGIN \
							INSERT INTO %S \
							VALUES ( new.%S, new.%S, new.%S, new.%S, new.%S, new.%S, new.%S, new.%S, \
							new.%S, new.%S, new.%S, new.%S, new.%S); \
							END;" );
	

	_LIT8 ( KTriggerUpdateStmt, "CREATE TRIGGER %S INSTEAD OF UPDATE ON %S \
								BEGIN \
								UPDATE %S SET %S=new.%S, %S=new.%S \
								WHERE %S=old.%S; \
								END;" );
	_LIT8 ( KTriggerDeleteStmt, "CREATE TRIGGER %S INSTEAD OF DELETE ON %S \
								BEGIN \
								DELETE FROM %S WHERE %S=old.%S; \
								END;" );
							
	_LIT8 ( KTldTableIndexStmt, "CREATE INDEX %S ON %S ( %S, %S )" );
					
	stmtBuf.Format ( KURITblCreateStmt(), &(KUriTblName()), &id, &scheme, &userInfo, &host, &port, &path, &query, &fragments );
	ExecuteL ( stmtBuf );
	stmtBuf.Format ( KURIPropTblCreateStmt(), &(KUriPropsTblName()), &RefId, &propId, &serviceType, &listType, &permission, &favouriteName );	
	ExecuteL ( stmtBuf );
	stmtBuf.Format ( KViewStmt(), &(KViewName()), &id, &id, &scheme, &scheme, &userInfo, &userInfo, &host, &host, &port, &port,
					 &path, &path, &query, &query, &fragments, &fragments, &propId, &propId, &serviceType, &serviceType, 
					 &listType, &listType, &permission, &permission, &favouriteName, &favouriteName, &(KUriTblName()), &(KUriPropsTblName()), &id, &propId );
	ExecuteL ( stmtBuf );
	stmtBuf.Format ( KTriggerInsertStmt(), &(KTriggerInsertView()), &(KViewName()), &(KViewName()), &id, &scheme, &userInfo, &host, &port, &path, &query, &fragments, &propId, &serviceType, &listType, &permission, &favouriteName );
	ExecuteL ( stmtBuf );
	stmtBuf.Format ( KTriggerUpdateStmt(), &(KTriggerUpdateView()), &(KViewName()), &(KViewName()), &listType, &listType, &favouriteName, &favouriteName, &propId, &propId );
	ExecuteL ( stmtBuf );
	stmtBuf.Format ( KTriggerDeleteStmt(), &(KTriggerDeleteView()), &(KViewName()), &(KViewName()), &propId, &propId );		
	ExecuteL ( stmtBuf );
	
	stmtBuf.Format ( KTldTblCreateStmt(), &(KTldTblName()), &KTldname, &KTldListType, &KCharacterSet );
	ExecuteL ( stmtBuf );
	
	stmtBuf.Format ( KTldTableIndexStmt(), &(KTldTblIndex()), &(KTldTblName()), &KTldname, &KTldListType );
	ExecuteL ( stmtBuf );
	
	CleanupStack::PopAndDestroy ( ); // stmtBuf	
	}

/**
Executes the SQL statement and leave incase of error
*/
void CSqlDbAccessor::ExecuteL ( const TDesC8& aStmt )
	{
	User::LeaveIfError ( iDatabase.Exec ( aStmt ) );
	}

/**
Prepares a new SQL transaction object by providing a SQL statement.
*/
MDBTransaction* CSqlDbAccessor::PrepareTransactionL ( const TDesC8& aQueryStmt )
	{
	return CSqlDbTransaction::NewL ( iDatabase, aQueryStmt );
	}

/**
Scalar queries are queries which returns a single value upon completion. This function
executes a scalar query and returns the result
*/
TInt CSqlDbAccessor::ExecuteScalarQueryL ( const TDesC8& aQueryStmt )
	{
	TSqlScalarFullSelectQuery selectQuery ( iDatabase );
	return 	selectQuery.SelectIntL ( aQueryStmt );
	}

void CSqlDbAccessor::BeginTransactionL ()
	{
	_LIT8 ( KBeginTrans, "BEGIN TRANSACTION;" );
	ExecuteL ( KBeginTrans() );
	}

void CSqlDbAccessor::CommitTransactionL ()
	{
	_LIT8 ( KCommitTrans, "COMMIT TRANSACTION;" );
	ExecuteL ( KCommitTrans() );			
	}

void CSqlDbAccessor::RollbackTransaction ()
	{
	_LIT8 ( KRollbackTrans, "ROLLBACK TRANSACTION;" );
	TInt err = iDatabase.Exec ( KRollbackTrans() );			
	}

void CSqlDbAccessor::Release ()
	{
	delete this;			
	}
