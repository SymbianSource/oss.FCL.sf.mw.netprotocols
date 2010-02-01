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
// Contains implementation of CTestSetPropertyStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// System Includes
#include <bookmarkdatabase.h>
#include <escapeutils.h>
#include <cauthentication.h>

// User Include
#include "TestSetPropertyStep.h"

/**
Constructor. Sets the test step name
@internalTechnology
@test
*/
CTestSetPropertyStep::CTestSetPropertyStep(CTestBookmarksServer& aTestServer) : CTestBookmarksBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestSetPropertyStep);
	}


/**
Base class pure virtual.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestSetPropertyStep::doTestStepL()
	{
	TInt property;
	TPtrC value;
	if(!GetIntFromConfig(ConfigSection(), 	 KIniProperty, property ) ||
	  (!GetStringFromConfig(ConfigSection(), KIniValue,	   value	) && property != EBookmarkAuth)
	  )
		{
		ERR_PRINTF3(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n"
					  ),&KIniProperty, &KIniValue
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		TRAPD(error, DoTestL(property, value));
		if(error != KErrNone)
			{
			ERR_PRINTF2(_L("Error occured in CTestSetPropertyStep::DoTestL"), error);
			SetTestStepResult(EFail);
			}
		}
	return TestStepResult();
	}	// doTestStepL

/**
Checks whether DB or item property is to be set and calls the appropriate
function
@internalTechnology
@test
@param		Enumeration indicating the property to be set
@param		Value to be set for the property
@return		None
*/
void CTestSetPropertyStep::DoTestL(const TInt& aProperty, const TPtrC& aValue)
	{
	TPtrC title;
	TPtrC typeOfItem;
	if(aProperty < KDbGeneralPropertiesStart)
		{// Item property
		if(!GetStringFromConfig(ConfigSection(), KIniTitle, 	 title			) ||
	   	   !GetStringFromConfig(ConfigSection(), KIniTypeOfItem, typeOfItem		)
	   	  )
			{
			ERR_PRINTF3(_L("%S and %S must also be provided for item property"),&KIniTypeOfItem, &KIniTitle);
			SetTestStepResult(EFail);
			}
		else
			{
			TInt error = KErrNone;
			RBkNode bkNode;
			if((error = GetBkNode(title, typeOfItem, bkNode)) != KErrNone)
				{
				ERR_PRINTF3(_L("Error occured while opening item %S : %D"), &title, error);
				SetTestStepError(error);
				}
			else
				{
				// Node opened, set the property
				CleanupClosePushL(bkNode);
				SetItemPropertyValueL(bkNode, aProperty, aValue);
				CleanupStack::PopAndDestroy(&bkNode);
				}
			}
		}
	else
		{// Database property
		SetDbPropertyValueL(aProperty, aValue);
		}
	}	// DoTestL

/**
Sets an item's property
@internalTechnology
@test
@param		Reference to the handle of the node whose property is to be set
@param		Enumeration indicating the property to be set
@param		Value to be set for the property
@return		None
*/
void CTestSetPropertyStep::SetItemPropertyValueL(RBkNode& aBkNode, const TInt& aProperty, const TPtrC& aValue)
	{
	INFO_PRINTF2(_L("Property length = %D"), aValue.Length());
	TInt error = KErrNone;
	switch(aProperty)
		{
		case EItemWritable:
			TRAP(error, aBkNode.SetWritableL(CTestBookmarksServer::GetBool(aValue)));
			break;
		case EItemPublic:
			TRAP(error, aBkNode.SetPublicL(CTestBookmarksServer::GetBool(aValue)));
			break;
		case EItemDescription:
			TRAP(error, aBkNode.SetDescriptionL(aValue));
			break;
		case EItemParent:
			{
			error = SetItemParent(aBkNode, aValue);
			break;
			}
		case EItemTitle:
			TRAP(error, aBkNode.SetTitleL(aValue));
			break;
		case EBookmarkNap:
			{
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			TRAP(error, bkBkMrk.SetNapL(CTestBookmarksServer::GetTUint32(aValue)));
			bkBkMrk.Close();
			break;
			}
		case EBookmarkProxy:
			{
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			TRAP(error, bkBkMrk.SetProxyL(CTestBookmarksServer::GetTUint32(aValue)));
			bkBkMrk.Close();
			break;
			}
		case EBookmarkUri:
			{
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			CleanupClosePushL(bkBkMrk);
			// Convert to 8-bit
			HBufC8* hBufC8 = EscapeUtils::ConvertFromUnicodeToUtf8L(aValue);
			TRAP(error, bkBkMrk.SetUriL(hBufC8->Des()));
			delete hBufC8;
			CleanupStack::PopAndDestroy(&bkBkMrk);
			break;
			}
		case EBookmarkAuth:
			{
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			TRAP(error, DoSetAuthenticationL(bkBkMrk));
			bkBkMrk.Close();
			break;
			}
		case EBookmarkLastVisited:
			{
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			TTime time(aValue);
			TRAP(error, bkBkMrk.SetLastVisitedL(time));
			bkBkMrk.Close();
			break;
			}
		case EBookmarkLastModified:
			{
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			TTime time(aValue);
			TRAP(error, bkBkMrk.SetLastModifiedL(time));
			bkBkMrk.Close();
			break;
			}
		case EItemOwner:
			TRAP(error, aBkNode.SetOwnerL(CTestBookmarksServer::GetTUint32(aValue)));
			break;
		default:
			INFO_PRINTF2(_L("Invalid property number : %D"), aProperty);
			SetTestStepResult(EFail);
			break;
		}

	// Examine the result
	CheckErrorAndCommit(error);
	}	// SetItemPropertyValueL

/**
Sets the item's parent to a folder with the title passed as parameter
@internalTechnology
@test
@param		Reference to the handle to the node whose parent we have to set
@param		The title of the parent folder to be set
@return		Error resulting out of the operation, if any. Else, KErrNone
*/
TInt CTestSetPropertyStep::SetItemParent(RBkNode& aBkNode, const TPtrC& aValue)
	{
	TInt error = KErrNone;
	RBkNode parentNode;
	if((error = GetBkNode(aValue, KFolder(), parentNode)) != KErrNone)
		{
		ERR_PRINTF3(_L("Error occured while opening item %S : %D"), &aValue, error);
		SetTestStepError(error);
		}
	else
		{
		RBkFolder parentFolder;
		TRAP(error, parentFolder = parentNode.OpenFolderL());
		if(error == KErrNone)
			{
			TRAP(error, aBkNode.SetParentL(parentFolder));
			parentFolder.Close();
			}
		parentNode.Close();
		}
	return error;	
	}

/**
Sets a database property
@internalTechnology
@test
@param		Enumeration indicating the property to be set
@param		Value to be set for the property
@return		None
*/
void CTestSetPropertyStep::SetDbPropertyValueL(const TInt& aProperty, const TPtrC& aValue)
	{
	TInt error = KErrNone;
	switch(aProperty)
		{
		case EDbDefaultNap:
			iBkDb.SetDefaultNap(CTestBookmarksServer::GetTUint32(aValue));
			break;
		case EDbDefaultProxy:
			iBkDb.SetDefaultProxy(CTestBookmarksServer::GetTUint32(aValue));
			break;
		case EDbHome:
			{
			error = SetHomePageBookmark(aValue);
			break;
			}
		case EDbHomePageText:
			TRAP(error, iBkDb.SetHomePageTextL(aValue));
			break;
		case EDbSearchUri:
			{
			// Convert to 8-bit
			HBufC8* hBufC8 = EscapeUtils::ConvertFromUnicodeToUtf8L(aValue);
			TRAP(error, iBkDb.SetSearchUriL(hBufC8->Des()));
			delete hBufC8;
			break;
			}
		default:
			INFO_PRINTF2(_L("Invalid property number : %D"), aProperty);
			SetTestStepResult(EFail);
			break;
		}

	// Examine the result
	CheckErrorAndCommit(error);
	}	// SetDbPropertyValueL

/**
Sets the item's homepage bookmark to a bookmark indicated by the parameter
@internalTechnology
@test
@param		Descriptor identifying the bookmark to be set as homepage
@return		Error resulting out of the operation, if any. Else, KErrNone
*/
TInt CTestSetPropertyStep::SetHomePageBookmark(const TPtrC& aValue)
	{
	TInt error = KErrNone;
	RBkNode bkNode;
	if((error = GetBkNode(aValue, KBookmark(), bkNode)) != KErrNone)
		{
		ERR_PRINTF3(_L("Error occured while opening item %S : %D"), &aValue, error);
		SetTestStepError(error);
		}
	else
		{
		CleanupClosePushL(bkNode);
		RBkBookmark homePageBkMrk = bkNode.OpenBookmarkL();
		iBkDb.SetHome(homePageBkMrk);
		homePageBkMrk.Close();
		CleanupStack::PopAndDestroy(&bkNode);
		}
	return error;	
	}
				
/**
Reads authentication detail from ini, constructs an authentication object
and sets it to the bookmark.
@internalTechnology
@test
@param		Reference to handle to the bookmark whose auth-params are to be set
@return		None
*/
void CTestSetPropertyStep::DoSetAuthenticationL(RBkBookmark& bkBkMrk)
	{
	TPtrC name;
	TPtrC password;

	if(!GetStringFromConfig(ConfigSection(), KIniAuthName, 	   name		) ||
	   !GetStringFromConfig(ConfigSection(), KIniAuthPassword, password	)
	  )
		{
		ERR_PRINTF3(_L("%S and %S must also be mentioned in ini file"),&KIniAuthName, &KIniAuthPassword);
		SetTestStepResult(EFail);
		}
	else
		{
		HBufC8* name8 = EscapeUtils::ConvertFromUnicodeToUtf8L(name);
		CleanupStack::PushL(name8);
		HBufC8* password8 = EscapeUtils::ConvertFromUnicodeToUtf8L(password);
		CleanupStack::PushL(password8);

		TPtrC method;
		CAuthentication *auth = NULL;
		// Construct authentication object
		if(!GetStringFromConfig(ConfigSection(), KIniAuthMethod, method))
			{
			// No method supplied
			INFO_PRINTF1(_L("No method supplied, calling without method argument"));
			auth = CAuthentication::NewL(name8->Des(), password8->Des());
			}
		else if(method.Compare(KMethodBasic()) == KErrNone)
			{
			auth = CAuthentication::NewL(name8->Des(), password8->Des(), CAuthentication::EBasic);
			}
		else if(method.Compare(KMethodDigest()) == KErrNone)
			{
			auth = CAuthentication::NewL(name8->Des(), password8->Des(), CAuthentication::EDigest);
			}
		else
			{
			INFO_PRINTF2(_L("Invalid method %S"), &method);
			SetTestStepResult(EFail);
			}

		if(auth != NULL)
			{
			CleanupStack::PushL(auth);
			// Set the bookmark's authentication object
			bkBkMrk.SetAuthenticationL(*auth);
			CleanupStack::PopAndDestroy(auth);
			}

		CleanupStack::PopAndDestroy(2, name8);
		}
	}	// DoSetAuthenticationL
