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


#ifndef __CHTTPTESTCHUNKEDBASE__
#define __CHTTPTESTCHUNKEDBASE__

#include "HttpTestCore.h"
#include "CHttpAccepTtestBase.h"
#include <http.h>
#include <http/mhttpauthenticationcallback.h>

#include "csrvaddrval.h"

/*	post content which requires authorisation
	and provide valid username/password (scooby/doo) */

class CHttpTestChunkedBase: public CHttpAcceptTestBase , public MHTTPAuthenticationCallback
	{
public:
	TBool GetNextDataPart(TPtrC8& aDataChunk);
	void ReleaseData();
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername,RString& aPassword);

protected:
	void OpenTestSessionL();
	void CloseTestSession();

protected:
	TBool iUseChunkedTransferEncoding;
	TInt iReqBodyChunkCount;
	TBool iLastChunk;
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};

#endif 
