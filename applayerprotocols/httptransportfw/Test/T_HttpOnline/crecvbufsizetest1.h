// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef	__CRECVBUFSIZETEST1_H__
#define	__CRECVBUFSIZETEST1_H__

#include <http.h>
#include "HttpTestCore.h"
#include <http/mhttptransactioncallback.h>


// User Include
#include "TestScripts.h"

class CRecvBufSizeTest1: public CHttpTestTransBase, public MHTTPTransactionCallback
						
	{
public:
	static CRecvBufSizeTest1* NewL(TInt aTestNumber, 
	CScriptFile* aIniSettingsFile);
	virtual ~CRecvBufSizeTest1();
	// to implement in this class' member function definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;

protected:
	CRecvBufSizeTest1(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();



private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	
	TInt iFailureError;
	
protected:
	TInt iTestNumber;
	};

#endif // __CRecvBufSizeTest1_H__
