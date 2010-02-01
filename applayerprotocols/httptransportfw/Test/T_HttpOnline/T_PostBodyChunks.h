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

#if !defined (__CTEXTMODETESTPOSTBODYCHUNKS_H__)
#define __CTEXTMODETESTPOSTBODYCHUNKS_H__

#include <http.h>
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

#include "TestScripts.h"

/**
	Derived test case class : POST body sent in separate chunks

	@param	aIniSettingsFile	This holds the instance of the settings.ini 
								file.
 */
class CTextModeTestPostBodyChunks: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
public:
	static CTextModeTestPostBodyChunks* NewL(CScriptFile* aIniSettingsFile);
	virtual ~CTextModeTestPostBodyChunks();
public: 
	// methods inherited from MHTTPTransactionCallback
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	// methods inherited from MHTTPDataSupplier
	virtual TBool GetNextDataPart(TPtrC8& aDataChunk);
	void ReleaseData();
	TInt OverallDataSize();
protected:
	const TDesC& TestName();
	// methods used to run the tests.
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
private:
	class CDelayedBodyDataNotifier : public CTimer
		{
	public:
		static CDelayedBodyDataNotifier* NewL(RHTTPTransaction aTrans);
		virtual void RunL();
	protected:
		CDelayedBodyDataNotifier(RHTTPTransaction aTrans);
	private:
		RHTTPTransaction iTrans;
		};
private:
	CTextModeTestPostBodyChunks(CScriptFile* aIniSettingsFile);
	void ConstructL();
private:
	TInt iReqBodyChunkCount;
	CDelayedBodyDataNotifier* iReleaseTimer;
	TInt iFailureError;

	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};


#endif // !defined __CTEXTMODETESTPOSTBODYCHUNKS_H__
