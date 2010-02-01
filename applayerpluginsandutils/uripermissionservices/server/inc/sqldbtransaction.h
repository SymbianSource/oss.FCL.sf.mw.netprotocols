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

#ifndef __SQLDBTRANSACTION_H__
#define __SQLDBTRANSACTION_H__

#include <sqldb.h>
#include "dbaccessor.h"

/**
This class manages the SQL transactions. The SQL statements will be executed with the methods
provided in this class.
*/
class CSqlDbTransaction : 	private CBase, 
							public MDBTransaction
	{
	public:
		static MDBTransaction* NewL ( RSqlDatabase& aDb, const TDesC8& aQueryStmt );
				
		// Methods from MDBTransaction
		void BindTextL ( TInt aParamPos, const TDesC8& aParamValue );				
		void BindIntL ( TInt aParamPos, TInt aParamValue );
		TInt ExecuteL ();
		TPtrC8 ColumnTextL ( TInt aFieldPos );
		TInt ColumnIntL ( TInt aFieldPos );
		TBool Next ();
		void Release ();
	private:
		CSqlDbTransaction (  );
		~CSqlDbTransaction ();		
		void ConstructL ( RSqlDatabase& aDb, const TDesC8& aQueryStmt );				
	private:
		RSqlStatement 	iStatement;
	};

#endif // __SQLDBACCESSOR_H__
