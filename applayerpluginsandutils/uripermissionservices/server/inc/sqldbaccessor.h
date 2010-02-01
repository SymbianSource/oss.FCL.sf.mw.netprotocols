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

#ifndef __SQLDBACCESSOR_H__
#define __SQLDBACCESSOR_H__

#include <sqldb.h>
#include <stringpool.h>
#include "dbaccessor.h"

const TInt KMaxDbStmtLen = 1024;

_LIT8 (KUriTblName, "URITbl");
_LIT8 (KUriPropsTblName, "URIPropertiesTbl");
_LIT8 (KViewName, "URIView");
_LIT8 (KTriggerInsertView, "ViewInsert");
_LIT8 (KTriggerUpdateView, "ViewUpdate");
_LIT8 (KTriggerDeleteView, "ViewDelete");
_LIT8 (KTldTblName, "TLDTbl");
_LIT8 (KTldTblIndex, "TldIndex");

/**
This class implements the operation that will be performed on the SQL storage. This class
inherits from MDBAccessor class. It manages the SQL operations like creating tables, view and 
triggers. 
*/
class CSqlDbAccessor : private CBase, public MDBAccessor
	{
	public:
		static MDBAccessor* NewL ( const RStringPool& aStringPool );					
		// Methods from MDBAccessor
		MDBTransaction* PrepareTransactionL ( const TDesC8& aQueryStmt );
		TInt ExecuteScalarQueryL ( const TDesC8& aQueryStmt );
		void ExecuteL ( const TDesC8& aStmt );		
		void BeginTransactionL ();
		void CommitTransactionL ();
		void RollbackTransaction ();
		void Release ();
	private:
		CSqlDbAccessor ();
		~CSqlDbAccessor ();
		
		void ConstructL ( const RStringPool& aStringPool );
		
		void InitializeL ();
		void CreateL ( const RStringPool& aStringPool );
		void CreateSchemaL ( const RStringPool& aStringPool );
	
		void CreateTableL ( const RStringPool& aStringPool );
		void CreateViewL ();
		void CreateTriggerL ();				
	private:
		RSqlDatabase 	iDatabase;
	};

#endif // __SQLDBACCESSOR_H__
