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
// Author: Franco.Bellu@symbian.com
// 
//


#ifndef __TC6_H__
#define __TC6_H__

#include "HttpTestCore.h"
#include <http.h>
#include "csrvaddrval.h"

/** 
 * like TC5 a completed transaction is simulated but the an  entity header 
 * specific to ocsp is added
 */
class CHttpTestCase6: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
	//to implement in this class' member functs definitions
public:
	CHttpTestCase6(CScriptFile* aIniSettingsFile);
	//##ModelId=3A76B9DD03D9
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual ~CHttpTestCase6();
private:	
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);
	//##ModelId=3A76B9DD03CF
	virtual void DoCancel();	
	//##ModelId=3A76B9DD03CE
//	virtual const TDesC& TestName();
protected:
	//##ModelId=3A76B9DD03C7
	void CompleteOwnRequest();
private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};


#endif
