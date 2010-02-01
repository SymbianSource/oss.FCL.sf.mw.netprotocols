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

#ifndef __CINC082448_H__
#define __CINC082448_H__

#include <e32base.h>
#include <http.h>
#include <http/mhttpfiltercreationcallback.h>
#include <http/mhttpauthenticationcallback.h>
#include "HttpTestCore.h"

class RConnection;
class CHTTPTestUtils;
class CScriptFile;
/*	
Test case for defect fix CINC082448. 
*/
class CINC082448 : public CHttpTestTransBase, 
							public MHTTPTransactionCallback,
							public MHTTPFilterCreationCallback,
							public MHTTPAuthenticationCallback
	{
public:
	static CINC082448* NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	virtual ~CINC082448();

	void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;
	// method inherited from MHTTPAuthenticationCallback
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
								 RStringF aAuthenticationType,
								 RString& aUsername, 
								 RString& aPassword);							 

private:
	CINC082448(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void ConfigureSessionFiltersL(TFilterConfigurationIterator* aFilterConfigIter);

	TBool IsConnectionActive ( RConnection& aConnection );
private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	
protected:
	TInt iOpenTransactionCount;
	TInt iFailureError;
	TInt iTestNumber;
	};
	
#endif // __CINC082448_H__
