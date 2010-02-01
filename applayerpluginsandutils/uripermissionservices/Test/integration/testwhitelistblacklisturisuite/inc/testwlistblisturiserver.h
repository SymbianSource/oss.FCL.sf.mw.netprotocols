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
// TESTWHITELISTBLACKLISTURISERVER.H
// Contains declaration of CTestWListBListUriServer class
// @internalAll
//

#ifndef __TEST__WHITELISTBLACKLIST_URI_SERVER_H__
#define __TEST__WHITELISTBLACKLIST_URI_SERVER_H__

// System Include
// For accessing TEF classes 
#include <test/testexecuteserverbase.h>
#include <ineturilist.h>

/*@{*/
// Literal constants for INI file field-names
_LIT(KIniUri, 		     "Uri"			  );
_LIT(KIniServiceType, 	 "ServiceType"	  );
_LIT(KIniListType, 		 "ListType"	      );
_LIT(KIniFavouriteName,  "FavouriteName"  );
_LIT(KIniMatchType,  	 "MatchType"  );
_LIT(KIniExpectedRetCode,"ExpRetCode");
_LIT(KIniExpectedMatchCount,"ExpMatchCount");
_LIT(KIniExpCountOrErrCode,"ExpCountOrErrCode");
_LIT(KIniExpectedListType,"ExpListType");
_LIT(KIniExpectedUri,"ExpUri");
_LIT(KIniIsUsingTUriC,"IsUsingTUriC");
_LIT(KIniIsCapabilityTest,"IsCapabilityTest");
_LIT(KTxt, "ERROR");
_LIT(KInitldQueryType,  	 "TldQueryType"  );
/*@}*/

/**
This is the test server to test the white/black-list uri service API. 
@internalTechnology
*/
class CTestWListBListUriServer : public CTestServer
	{
public:
	// Construction
	static	CTestWListBListUriServer* 	NewL();
	// Base class pure virtual
	virtual CTestStep* 	CreateTestStep(const TDesC& aStepName);
protected:
	CTestStep* 	 CreateTestStepL(const TDesC& aStepName);
	
	const TPtrC  ServerName();

	};

#endif		// __TEST__WHITELISTBLACKLIST_URI_SERVER_H__







