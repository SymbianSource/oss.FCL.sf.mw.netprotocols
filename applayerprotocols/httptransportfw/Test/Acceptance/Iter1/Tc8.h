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
// Author: Leon.Clarke@symbian.com
// 
//

#ifndef __TC8_H__
#define __TC8_H__

#include "HttpTestCore.h"
#include <http.h>
#include <http/mhttpauthenticationcallback.h>
#include "csrvaddrval.h"

/** 
 * like TC5 a completed transaction is simulated but with basic authentication
 */
class CHttpTestCase8: public CHttpTestTransBase, public MHTTPTransactionCallback, public MHTTPAuthenticationCallback
	{
	//to implement in this class' member functs definitions
public:
	CHttpTestCase8(CScriptFile* aIniSettingsFile);
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	// From MHTTPAuthenticationCallback.
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword);

	virtual ~CHttpTestCase8();
private:	
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);
	virtual void DoCancel();	
protected:
	void CompleteOwnRequest();
private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};

#endif
