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
// Contains implementation of CTestGetPropertyStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// System Includes
#include <bookmarkdatabase.h>
#include <escapeutils.h>

// User Include
#include "TestGetPropertyStep.h"

/**
Constructor. Sets the test step name
@internalTechnology
@test
*/
CTestGetPropertyStep::CTestGetPropertyStep(CTestBookmarksServer& aTestServer) : CTestBookmarksBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestGetPropertyStep);
	}

/**
Base class pure virtual.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestGetPropertyStep::doTestStepL()
	{
	TInt property;
	TPtrC expectedValue;
	if(!GetIntFromConfig(ConfigSection(), 	 KIniProperty, 		property	  ) ||
	  (!GetStringFromConfig(ConfigSection(), KIniExpectedValue,	expectedValue ) &&
	  	property != EBookmarkAuth && property != EDbVersion )
	  )
		{
		ERR_PRINTF3(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n"
					  ),&KIniProperty, &KIniExpectedValue
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		TRAPD(error, DoTestL(property, expectedValue));
		if(error != KErrNone)
			{
			ERR_PRINTF2(_L("Error occured in CTestGetPropertyStep::DoTestL"), error);
			SetTestStepResult(EFail);
			}
		}
	return TestStepResult();
	}	// doTestStepL

/**
Checks whether DB or item property is to be retreived and calls the
appropriate function
@internalTechnology
@test
@param		Enumeration indicating the property we have to get
@param		Expected value for the property
@return		None
*/
void CTestGetPropertyStep::DoTestL(const TInt& aProperty, const TPtrC& aExpectedValue)
	{
	TPtrC title;
	TPtrC typeOfItem;
	if(aProperty < KDbGeneralPropertiesStart)
		{
		if(!GetStringFromConfig(ConfigSection(), KIniTitle, 	 title		) ||
	   	   !GetStringFromConfig(ConfigSection(), KIniTypeOfItem, typeOfItem	)
	   	  )
			{
			ERR_PRINTF3(_L("%S and %S must also be provided for item property"),&KIniTypeOfItem, &KIniTitle);
			SetTestStepResult(EFail);
			}
		else
			{// Item property
			TInt error = KErrNone;
			RBkNode bkNode;
			if((error = GetBkNode(title, typeOfItem, bkNode)) != KErrNone)
				{
				ERR_PRINTF3(_L("Error occured while opening item %S : %D"), &title, error);
				SetTestStepError(error);
				}
			else
				{
				CleanupClosePushL(bkNode);
				GetAndCompareItemPropertyValueL(bkNode, aProperty, aExpectedValue);
				CleanupStack::PopAndDestroy(&bkNode);
				}
			}
		}
	else
		{// DB property
		GetAndCompareDbPropertyValueL(aProperty, aExpectedValue);
		}
	}	//DoTestL

/**
Gets and compares item property with expected value
@internalTechnology
@test
@param		Reference to the handle to the node whose property we have to get
@param		Enumeration indicating the property we have to get
@param		Expected value for the property
@return		None
*/
void CTestGetPropertyStep::GetAndCompareItemPropertyValueL(const RBkNode& aBkNode, const TInt& aProperty, const TPtrC& aExpectedValue)
	{
	TVerdict verdict = EPass;
	TInt error = KErrNone;
	TUint32 retTUint32Val;
	TBool retBoolVal;
	TPtrC retStrVal;
	switch(aProperty)
		{
		case EItemWritable:
			retBoolVal = aBkNode.IsWritable();
			verdict = CompareBools(retBoolVal, CTestBookmarksServer::GetBool(aExpectedValue));
			break;
		case EItemPublic:
			retBoolVal = aBkNode.IsPublic();
			verdict = CompareBools(retBoolVal, CTestBookmarksServer::GetBool(aExpectedValue));
			break;
		case EBookmarkIsHome:
			{
			RBkBookmark homePgBkMrk = aBkNode.OpenBookmarkL();
			retBoolVal = homePgBkMrk.IsHomePage();
			homePgBkMrk.Close();
			verdict = CompareBools(retBoolVal, CTestBookmarksServer::GetBool(aExpectedValue));
			break;
			}
		case EItemDescription:
			TRAP(error, retStrVal.Set(aBkNode.DescriptionL()));
			if(error == KErrNone)
				{
				verdict = CompareStrings(retStrVal, aExpectedValue);
				}
			break;
		case EItemParent:
			{
			error = GetItemParent(aBkNode, aExpectedValue, verdict);
			break;
			}
		case EItemTitle	:
			retStrVal.Set(aBkNode.Title());
			verdict = CompareStrings(retStrVal, aExpectedValue);
			break;
		case EBookmarkNap:
			{
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			TRAP(error, retTUint32Val = bkBkMrk.GetNapL());
			bkBkMrk.Close();
			if(error == KErrNone)
				{
				verdict = CompareTUints(retTUint32Val, CTestBookmarksServer::GetTUint32(aExpectedValue));
				}
			break;
			}
		case EBookmarkProxy:
			{
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			TRAP(error, retTUint32Val = bkBkMrk.ProxyL());
			bkBkMrk.Close();
			if(error == KErrNone)
				{
				verdict = CompareTUints(retTUint32Val, CTestBookmarksServer::GetTUint32(aExpectedValue));
				}
			break;
			}
		case EBookmarkUri:
			{
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			CleanupClosePushL(bkBkMrk);
			// Convert to 16-bit
			HBufC* hBufC = EscapeUtils::ConvertToUnicodeFromUtf8L(bkBkMrk.Uri());
			verdict = CompareStrings(hBufC->Des(), aExpectedValue);
			delete hBufC;
			CleanupStack::PopAndDestroy(&bkBkMrk);
			break;
			}
		case EBookmarkAuth:
			{
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			TRAP(error, verdict = DoGetAuthenticationL(bkBkMrk));
			bkBkMrk.Close();
			break;
			}
		case EBookmarkLastVisited:
			{
			TTime retTTimeVal;
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			TRAP(error, retTTimeVal = bkBkMrk.LastVisited());
			if(error == KErrNone)
				{
				verdict = CompareTTimes(retTTimeVal, TTime(aExpectedValue));
				}
			bkBkMrk.Close();
			break;
			}
		case EBookmarkLastModified:
			{
			TTime retTTimeVal;
			RBkBookmark bkBkMrk = aBkNode.OpenBookmarkL();
			TRAP(error, retTTimeVal = bkBkMrk.LastModifiedL());
			if(error == KErrNone)
				{
				verdict = CompareTTimes(retTTimeVal, TTime(aExpectedValue));
				}
			bkBkMrk.Close();
			break;
			}
		case EItemOwner:
			TRAP(error, retTUint32Val = aBkNode.OwnerL());
			if(error == KErrNone)
				{
				verdict = CompareTUints(retTUint32Val, CTestBookmarksServer::GetTUint32(aExpectedValue));
				}
			break;
		default:
			INFO_PRINTF2(_L("Invalid property number : %D"), aProperty);
			SetTestStepResult(EFail);
		}

	// Examine the result
	CheckErrorAndVerdict(error, verdict);
	}	// GetAndCompareItemPropertyValueL

/**
Gets and compares the item's parent with an expected value
@internalTechnology
@test
@param		Reference to the handle to the node whose parent we have to get
@param		The title of the expected parent folder
@param		Refernce to the verdict variable, that will be set by this method
@return		Error resulting out of the operation, if any. Else, KErrNone
*/
TInt CTestGetPropertyStep::GetItemParent(const RBkNode& aBkNode, const TPtrC& aExpectedValue, TVerdict& aVerdict)
	{
	RBkNode parentFolder;
	TRAPD(error, parentFolder = aBkNode.OpenParentL());
	if(error == KErrNone)
		{
		TUint32 retTUint32Val = parentFolder.Id();
		parentFolder.Close();
		RBkFolder expectedParentFolder;
		if((error = GetBkNode(aExpectedValue, KFolder(), expectedParentFolder)) != KErrNone)
			{
			ERR_PRINTF3(_L("Error occured while opening item %S : %D"), &aExpectedValue, error);
			SetTestStepError(error);
			}
		else
			{
			aVerdict = CompareTUints(retTUint32Val, expectedParentFolder.Id());
			expectedParentFolder.Close();
			}
		}
		return error;
	}

/**
Gets and compares DB property with expected value
@internalTechnology
@test
@param		Enumeration indicating the property we have to get
@param		Expected value for the property
@return		None
*/
void CTestGetPropertyStep::GetAndCompareDbPropertyValueL(const TInt& aProperty, const TPtrC& aExpectedValue)
	{
	TInt error = KErrNone;
	TVerdict verdict = EPass;
	TPtrC retStrVal;
	TUint32 retTUint32Val;
	switch(aProperty)
		{
		case EDbDefaultNap:
			retTUint32Val = iBkDb.DefaultNap();
			verdict = CompareTUints(retTUint32Val, CTestBookmarksServer::GetTUint32(aExpectedValue));
			break;
		case EDbDefaultProxy:
			retTUint32Val = iBkDb.DefaultProxy();
			verdict = CompareTUints(retTUint32Val, CTestBookmarksServer::GetTUint32(aExpectedValue));
			break;
		case EDbHome:
			{
			error = GetAndCompareHomePageBookmark(aExpectedValue, verdict);
			break;
			}
		case EDbHomePageText:
			retStrVal.Set(iBkDb.HomePageText());
			verdict = CompareStrings(retStrVal, aExpectedValue);
			break;
		case EDbSearchUri:
			{
			// Convert to 16-bit
			HBufC* hBufC = EscapeUtils::ConvertToUnicodeFromUtf8L(iBkDb.SearchUri());
			retStrVal.Set(hBufC->Des());
			verdict = CompareStrings(retStrVal, aExpectedValue);
			delete hBufC;
			break;
			}
		default:
			INFO_PRINTF2(_L("Invalid property number : %D"), aProperty);
			SetTestStepResult(EFail);
		}
	
	// Examine the result
	CheckErrorAndVerdict(error, verdict);
	}	// GetAndCompareItemPropertyValueL

/**
Gets and compares the item's homepage bookmark with an expected value
@internalTechnology
@test
@param		Descriptor indicating the expected bookmark
@param		Reference to the verdict variable, that will be set by this method
@return		Error resulting out of the operation, if any. Else, KErrNone
*/
TInt CTestGetPropertyStep::GetAndCompareHomePageBookmark(const TPtrC& aExpectedValue, TVerdict& aVerdict)
	{
	TInt error = KErrNone;
	RBkBookmark homePageBkMrk;
	TRAP(error, homePageBkMrk = iBkDb.OpenHomeL());
	if(error == KErrNone)
		{
		TUint32 retTUint32Val = homePageBkMrk.Id();
		homePageBkMrk.Close();
		RBkBookmark expectedNode;
		if((error = GetBkNode(aExpectedValue, KBookmark(), expectedNode)) != KErrNone)
			{
			ERR_PRINTF3(_L("Error occured while opening item %S : %D"), &aExpectedValue, error);
			SetTestStepError(error);
			}
		else
			{
			aVerdict = CompareTUints(retTUint32Val, expectedNode.Id());
			expectedNode.Close();
			}
		}
		return error;
	}

/**
Reads expected authentication details from ini, gets the authentication object
from the bookmark and compares its components with the expected ones
@internalTechnology
@test
@param		Reference to handle to the bookmark whose auth-params we are to get
@return		EPass or EFail based on the result of the comparison
*/
TVerdict CTestGetPropertyStep::DoGetAuthenticationL(const RBkBookmark& bkBkMrk)
	{
	TVerdict verdict = EPass;
	TPtrC name;
	TPtrC password;
	TPtrC method;
	CAuthentication::TMethod tMethod = CAuthentication::EDigest;

	if(!GetStringFromConfig(ConfigSection(), KIniAuthName, 	   name		) ||
	   !GetStringFromConfig(ConfigSection(), KIniAuthPassword, password	)
	  )
		{
		ERR_PRINTF3(_L("%S and %S must also be mentioned in ini file"),&KIniAuthName, &KIniAuthPassword);
		SetTestStepResult(EFail);
		return EFail;
		}
	else
		{
		if(GetStringFromConfig(ConfigSection(),KIniAuthMethod, method))
			{
			if(method.Compare(KMethodBasic) == KErrNone)
				{
				tMethod = CAuthentication::EBasic;
				}
			}

		// Get auth-params
		// Construct 16-bit versions of the auth-params, so that we can compare
		HBufC* retName = EscapeUtils::ConvertToUnicodeFromUtf8L(const_cast<RBkBookmark&>(bkBkMrk).AuthenticationL().Name());
		CleanupStack::PushL(retName);
		HBufC* retPassword = EscapeUtils::ConvertToUnicodeFromUtf8L(const_cast<RBkBookmark&>(bkBkMrk).AuthenticationL().Password());
		CleanupStack::PushL(retPassword);

		CAuthentication::TMethod retTMethod = const_cast<RBkBookmark&>(bkBkMrk).AuthenticationL().Method();

		// Compare and decide on the verdict
		if(CompareStrings(retName->Des(), name) == EFail)
			{
			INFO_PRINTF1(_L("The expected name and returned name did not match"));
			verdict = EFail;
			}
		if(CompareStrings(retPassword->Des(), password) == EFail)
			{
			INFO_PRINTF1(_L("The expected password and returned password did not match"));
			verdict = EFail;
			}
		if(CompareTInts(retTMethod, tMethod) == EFail)
			{
			INFO_PRINTF1(_L("The expected method and returned method did not match"));
			verdict = EFail;
			}
		CleanupStack::PopAndDestroy(2, retName);
		}
	return verdict;
	}	// DoGetAuthenticationL


