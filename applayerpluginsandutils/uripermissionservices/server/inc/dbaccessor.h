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

#ifndef __DBACCESSOR_H__
#define __DBACCESSOR_H__


class MDBTransaction
	{
	public:
		/**
		Bind the parameter for the query statement
		*/
		virtual void BindTextL ( TInt aParamPos, const TDesC8& aParamValue ) = 0;		
		
		virtual void BindIntL ( TInt aParamPos, TInt aParamValue ) = 0;
		/**
		Execute the query and returns the record set.
		*/
		virtual TInt ExecuteL () = 0;	
		
		/**
		Get the column value by passing the column index
		*/		
		virtual TPtrC8 ColumnTextL ( TInt aFieldPos ) = 0;
		virtual TInt ColumnIntL ( TInt aFieldPos ) = 0;

		/**
		Move to the next record. Returns EFalse in the case of end of record
		*/		
		virtual TBool Next () = 0;
		
		/**
		Cleanup the object
		*/	
		virtual void Release () = 0;
	};

class MDBAccessor
	{
	public:	
		/**
		Prepare the transaction and returns a transaction object
		*/
		virtual MDBTransaction* PrepareTransactionL ( const TDesC8& aQueryStmt ) = 0;
		/**
		Function does a scalar query and returns the result value
		*/
		virtual TInt ExecuteScalarQueryL ( const TDesC8& aQueryStmt ) = 0;	
		/**
		The following methods are used for BEGIN/COMMIT/ROLLBACK transaction 
		with the database
		*/
		virtual void BeginTransactionL () = 0;
		virtual void CommitTransactionL () = 0;
		virtual void RollbackTransaction () = 0;
		
		/**
		Cleanup the object
		*/	
		virtual void Release () = 0;	
	};
	
#endif // __SQLDBACCESSOR_H__
