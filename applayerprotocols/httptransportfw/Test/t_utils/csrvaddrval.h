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

/**
 @file csrvaddrval.h
 @internalTechnology
*/

#ifndef __CSRVADDRVAL_H__
#define __CSRVADDRVAL_H__

// User includes
#include "TestScripts.h"
#include "HttpTestCore.h"

// Constants used in the test harness.
// These constants define the maximum length of the host name and Url / Uri
#define KMaxHostNameBufSize		256
#define KMaxUrlBufSize			1024

/**
	Class definition which has functions to replace the value of the server IP
	/ Name in a Url / Uri by retrieving the value from settings.ini file.  The 
	settings.ini file has the details of the servers existing in the test 
	environment (internal URIs or IP addresses).
*/
class TSrvAddrVal
	{
public:
	// This function is called from the test harnesses to replace the host name
	// in the Url/Uri that is going to be used by the test case.
	IMPORT_C static HBufC8* ReplaceHostNameL(const TDesC8& aUrlToBeModified, CScriptFile* aIniSettingsFile);
	IMPORT_C static HBufC16* ReplaceHostNameL(const TDesC16& aUrlToBeModified, CScriptFile* aIniSettingsFile);
	// This functions converts a 8 bit descriptor to 16 bit descriptor and logs it
	// to the log file.  This function is written since LogIt() function takes a 
	// 16 bit descriptor as input and it cannot be overloaded due to BC issue.
	IMPORT_C static void LogUsing8BitDesL(CHttpTestEngine* aEngine, const TDesC8& aModify8BitDesC);
	};

#endif // __CSRVADDRVAL_H__
