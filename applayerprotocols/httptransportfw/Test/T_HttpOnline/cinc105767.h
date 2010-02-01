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

#ifndef __CINC105767_H__
#define __CINC105767_H__

#include <e32base.h>
#include <http.h>
#include <http/mhttpfiltercreationcallback.h>
#include <http/mhttpauthenticationcallback.h>
#include "HttpTestCore.h"

class RConnection;
class CHTTPTestUtils;
class CScriptFile;
/*	
Test case for defect fix CINC105767. 
*/
class CINC105767 : public CHttpTestTransBase, 
							public MHTTPTransactionCallback
	{
public:
	static CINC105767* NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	virtual ~CINC105767();

	void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;

private:
	CINC105767(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void CloseConnection ();
	TBool IsConnectionActive ( RConnection& aConnection );
	
	// methods inherited from MHTTPDataSupplier
	virtual TBool GetNextDataPart(TPtrC8& aDataChunk);
	void ReleaseData();
	TInt OverallDataSize();
	TInt Reset ();
private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	
protected:
	TInt iFailureError;
	TInt iTestNumber;
	TInt iExpectedError;
	};
	
#endif // __CINC105767_H__
