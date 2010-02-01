// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contains declaration of CTestBookmarksServer class and common constants
// 
//

#ifndef __TEST_BOOKMARKS_SERVER_H__
#define __TEST_BOOKMARKS_SERVER_H__

// System Includes
#include <testexecuteserverbase.h>

/*@{*/
// Literal constants for INI file field-names
_LIT(KIniDbMode, 				"dbmode"		);
_LIT(KIniTypeOfItem, 			"typeofitem"	);
_LIT(KIniRecursive, 			"recursive"		);
_LIT(KIniTitle, 				"title"			);
_LIT(KIniParent, 				"parent"		);
_LIT(KIniProperty,				"property"		);
_LIT(KIniValue,					"value"			);
_LIT(KIniExpectedValue,			"expectedvalue"	);
_LIT(KIniOperation,				"operation"		);
_LIT(KIniCategory,				"category"		);
_LIT(KIniDataType,				"datatype"		);
_LIT(KIniUid,					"uid"			);
_LIT(KIniAuthName,				"name"			);
_LIT(KIniAuthPassword,			"password"		);
_LIT(KIniAuthMethod,			"method"		);
_LIT(KIniMajor,					"major"			);
_LIT(KIniMinor,					"minor"			);
_LIT(KIniBuild,					"build"			);
/*@}*/

/*@{*/
// Literal constants denoting the DB modes
_LIT(KDbModePublic, 			"public"		);
_LIT(KDbModePrivate, 			"private"		);
_LIT(KDbModeAll, 				"all"			);
_LIT(KDbModeManager, 			"manager"		);
/*@}*/

/*@{*/
// Literal constant for the main test folder
_LIT(KMainTestFolder, 			"testbookmarkssuitemaintestfolder");
/*@}*/

/*@{*/
// Literal constants used by some test steps
_LIT(KRegisterOperation, 		"register"		);
_LIT(KDeRegisterOperation, 		"deregister"	);
_LIT(KGetOperation, 			"get"			);
_LIT(KSetOperation, 			"set"			);

_LIT(KTypeInt, 					"int"			);
_LIT(KTypeReal, 				"real"			);
_LIT(KTypeDes8, 				"des8"			);
_LIT(KTypeDes16, 				"des16"			);

_LIT(KDb, 						"db"			);
_LIT(KFolder, 					"folder"		);
_LIT(KBookmark, 				"bookmark"		);

_LIT(KRoot,						"root"			);

_LIT(KMethodBasic,				"basic"			);
_LIT(KMethodDigest,				"digest"		);

_LIT(KTrue,						"TRUE"			);
_LIT(KFalse,					"FALSE"			);
/*@}*/

const TInt KItemBoolPropertiesStart		=	0;
const TInt KItemGeneralPropertiesStart 	=	100;
const TInt KDbGeneralPropertiesStart	=	200;

/**
Enumerations denoting Boolean properties that have to be Set or Got
The property numbers in the ini file are based on these.
@internalTechnology
@test
*/
enum TItemBoolProperties
	{
	/**	Public attribute */
	EItemPublic				= KItemBoolPropertiesStart + 1,
	/**	Writable attribute */
	EItemWritable			= KItemBoolPropertiesStart + 2,
	/**	Is homepage flag */
	EBookmarkIsHome			= KItemBoolPropertiesStart + 3,
	};

/**
Enumerations denoting Other general properties that have to be Set or Got
The property numbers in the ini file are based on these.
@internalTechnology
*/
enum TItemGeneralProperties
	{
	/**	Item Description attribute */
	EItemDescription		= KItemGeneralPropertiesStart + 1,
	/**	Item Parent attribute */
	EItemParent				= KItemGeneralPropertiesStart + 2,
	/**	Item Title attribute */
	EItemTitle				= KItemGeneralPropertiesStart + 3,
	/**	Bookmark NAP attribute */
	EBookmarkNap			= KItemGeneralPropertiesStart + 4,
	/**	Item Owner attribute */
	EItemOwner				= KItemGeneralPropertiesStart + 5,
	/**	Bookmark URI attribute */
	EBookmarkUri			= KItemGeneralPropertiesStart + 6,
	/**	Bookmark Authentication attribute */
	EBookmarkAuth			= KItemGeneralPropertiesStart + 7,
	/**	Bookmark Last visited time attribute */
	EBookmarkLastVisited	= KItemGeneralPropertiesStart + 8,
	/**	Bookmark Proxy attribute */
	EBookmarkProxy			= KItemGeneralPropertiesStart + 9,
	/**	Bookmark Last modified time attribute */
	EBookmarkLastModified	= KItemGeneralPropertiesStart + 10,
	};

/**
Enumerations denoting properties of the DB that have to be Set or Got
The property numbers in the ini file are based on these.
@internalTechnology
@test
*/
enum TDbGeneralProperties
	{
	/**	DB Default NAP attribute */
	EDbDefaultNap			= KDbGeneralPropertiesStart + 1,
	/**	DB Default Proxy attribute */
	EDbDefaultProxy			= KDbGeneralPropertiesStart + 2,
	/**	DB Homepage attribute */
	EDbHome					= KDbGeneralPropertiesStart + 3,
	/**	DB Homepage text attribute */
	EDbHomePageText			= KDbGeneralPropertiesStart + 4,
	/**	DB Search URI attribute */
	EDbSearchUri			= KDbGeneralPropertiesStart + 5,
	/**	DB Version attribute */
	EDbVersion				= KDbGeneralPropertiesStart + 6,
	};

/**
The test server.
@internalTechnology
@test
*/
class CTestBookmarksServer : public CTestServer
	{
public:
	// Construction
	static	CTestBookmarksServer* 	NewL();

	// Destruction
	~CTestBookmarksServer();

	// Base class pure virtual
	virtual CTestStep* 				CreateTestStep(const TDesC& aStepName);

	const TPtrC 					ServerName();

	static TBool 					GetBool(const TPtrC& aString);
	static TUint32					GetTUint32(const TPtrC& aValue);
	static TInt 					GetTInt(const TPtrC& aValue);
	static TReal 					GetTReal(const TPtrC& aValue);

public:
	/** Array to store the titles of items created */
	RPointerArray<HBufC> 			iTitles;
	/** Array to store the ids of items created    */
	RArray<TUint> 					iIds;
	};

#endif		// __TEST_BOOKMARKS_SERVER_H__

