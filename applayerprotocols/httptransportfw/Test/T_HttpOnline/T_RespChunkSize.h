// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined (__CTEXTMODETESTRESPCHUNKSIZE_H__)
#define __CTEXTMODETESTRESPCHUNKSIZE_H__

#include <http.h>
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif
#if !defined(__T_DUMMYSERVER_H__)
#include "T_DummyServer.h"
#endif

#include "TestScripts.h"

/**
 *  Derived test case class : test cancellation (dummy)
 */
class CTextModeTestRespChunkSize: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
	// to implement in this class' member functs definitions
public:
	static CTextModeTestRespChunkSize* NewL(TInt aDummyProxyPort, TInt aRespChunkSize, CScriptFile* aIniSettingsFile);
	virtual ~CTextModeTestRespChunkSize();
public:
	// methods inherited from MHTTPTransactionCallback
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;

	// methods inherited from MHTTPDataSupplier
	virtual TBool GetNextDataPart(TPtrC8& aDataChunk);
	void ReleaseData();
	TInt OverallDataSize();
protected:
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
private:
	CTextModeTestRespChunkSize(TInt aDummyProxyPort, TInt aRespChunkSize, CScriptFile* aIniSettingsFile);
	void ConstructL();
private:
	CDummyServer* iDummyServer;
	TInt iDummyProxyPort;
	TInt iRespChunkSize;
	TInt iFailureError;

	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */

	CScriptFile* iIniSettingsFile;
	};

#endif // !defined __CTEXTMODETESTRESPCHUNKSIZE_H__
