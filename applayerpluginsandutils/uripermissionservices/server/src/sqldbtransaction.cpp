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

#include "sqldbtransaction.h"

/**
Stanadard factory construction method
*/
MDBTransaction* CSqlDbTransaction::NewL ( RSqlDatabase& aDb, const TDesC8& aQueryStmt )
	{
	CSqlDbTransaction* self = new ( ELeave )CSqlDbTransaction ();
	CleanupStack::PushL ( self );
	self->ConstructL ( aDb, aQueryStmt );
	CleanupStack::Pop (); // self
	
	return self;
	}
	
CSqlDbTransaction::~CSqlDbTransaction ()
	{
	iStatement.Close ();	
	}

CSqlDbTransaction::CSqlDbTransaction ( )
	{
		
	}

void CSqlDbTransaction::ConstructL ( RSqlDatabase& aDb, const TDesC8& aQueryStmt )
	{
	iStatement.PrepareL ( aDb, aQueryStmt );
	}

/**
Binds the text value with the SQL statement
*/	
void CSqlDbTransaction::BindTextL ( TInt aParamPos, const TDesC8& aParamValue )
	{
	User::LeaveIfError ( iStatement.BindBinary ( aParamPos, aParamValue ) );			
	}

/**
Binds the int value with the SQL statement
*/	
void CSqlDbTransaction::BindIntL ( TInt aParamPos, TInt aParamValue )
	{
	User::LeaveIfError ( iStatement.BindInt ( aParamPos, aParamValue ) );
	}

/**
Executes the SQL statement. Leaves incase of an error. Otherwise returns with
the number of records changed during a INSERT, UPDATE or DELETE operation
*/
TInt CSqlDbTransaction::ExecuteL ()
	{
	TInt ret = iStatement.Exec ();
	User::LeaveIfError ( ret );
	return ret;
	}

/**
Returns the column text value from the result set, given the field position
*/	
TPtrC8 CSqlDbTransaction::ColumnTextL ( TInt aFieldPos )
	{
	return iStatement.ColumnBinaryL ( aFieldPos );
	}

/**
Returns the column integer value from the result set, given the field position
*/
TInt CSqlDbTransaction::ColumnIntL ( TInt aFieldPos )
	{
	return iStatement.ColumnInt ( aFieldPos );
	}

/**
Moves to the first or next record. Returns ETrue if there is a record otherwise returns
EFalse
*/
TBool CSqlDbTransaction::Next ()
	{
	return ( iStatement.Next() == KSqlAtRow );	
	}

void CSqlDbTransaction::Release ()
	{
	delete this;	
	}
	
