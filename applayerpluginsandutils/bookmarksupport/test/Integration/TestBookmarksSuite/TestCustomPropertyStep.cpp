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
// Contains implementation of CTestCustomPropertyStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// System Includes
#include <bookmarkdatabase.h>

// User Include
#include "TestCustomPropertyStep.h"

/**
Constructor. Sets the test step name
@internalTechnology
@test
*/
CTestCustomPropertyStep::CTestCustomPropertyStep(CTestBookmarksServer& aTestServer) : CTestBookmarksBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestCustomPropertyStep);
	}


/**
Base class pure virtual.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestCustomPropertyStep::doTestStepL()
	{
	TPtrC operation;
	TPtrC category;
	TPtrC uniqueId;
	if(!GetStringFromConfig(ConfigSection(), KIniOperation, operation ) ||
	   !GetStringFromConfig(ConfigSection(), KIniCategory, 	category  ) ||
	   !GetStringFromConfig(ConfigSection(), KIniUid,		uniqueId  )
	  )
		{
		ERR_PRINTF4(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n"
					  ),&KIniOperation, &KIniCategory, &KIniUid
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		if(operation.Compare(KRegisterOperation) == KErrNone || operation.Compare(KDeRegisterOperation) == KErrNone)
			{// Register or deregister custom property
			DoRegDeregCustomProperty(operation, category, uniqueId);
			}
		else if(operation.Compare(KSetOperation) == KErrNone || operation.Compare(KGetOperation) == KErrNone)
			{// Set or get custom property
			TRAPD(error, DoSetGetCustomPropertyL(operation, category, uniqueId));
			if(error != KErrNone)
				{
				ERR_PRINTF3(_L("Error occured while doing %S operation: %D"), operation, error);
				SetTestStepResult(EFail);
				}
			}
		else
			{
			ERR_PRINTF2(_L("Invalid operation : %S"), &operation);
			SetTestStepResult(EFail);
			}
		}
	return TestStepResult();
	}	// doTestStepL

/**
Registers or deregisters a custom property
@internalTechnology
@test
@param		Operation to be performed : set/get/register/deregister
@param		Category: db/folder/bookmark
@param		Uid of the custom property
@return		None
*/
void CTestCustomPropertyStep::DoRegDeregCustomProperty(const TPtrC& aOperation, const TPtrC& aCategory, const TPtrC& aUid)
	{
	TInt error = KErrNone;
	TPtrC dataType;
	if(aOperation.Compare(KRegisterOperation) == KErrNone)
		{
		if(!GetStringFromConfig(ConfigSection(), KIniDataType, dataType))
			{
			ERR_PRINTF2(_L("%S should be mentioned in ini for registering a property"), &KIniDataType);
			SetTestStepResult(EFail);
			}
		else
			{
			// Register property
			error = RegisterCustomProperty(aUid, aCategory, dataType);
			}
		}
	else if(aOperation.Compare(KDeRegisterOperation) == KErrNone)
		{
		// Deregister property
		error = DeRegisterCustomProperty(aUid, aCategory);
		}

	// Examine the result
	CheckErrorAndCommit(error);
	}	// DoRegDeregCustomProperty

/**
Resolves and returns the enumeration indicating the custom property type
function
@internalTechnology
@test
@param		Descriptor indicating the data type of the custom property
@return		Bookmark::TPropertyType enumeration indicating the type of property
*/
Bookmark::TPropertyType CTestCustomPropertyStep::GetPropertyType(const TPtrC& aDataType)
	{
	if(aDataType.Compare(KTypeInt) == KErrNone)
		{
		return Bookmark::EDataTypeInt;
		}
	else if(aDataType.Compare(KTypeReal) == KErrNone)
		{
		return Bookmark::EDataTypeReal;
		}
	else if(aDataType.Compare(KTypeDes8) == KErrNone)
		{
		return Bookmark::EDataTypeDes8;
		}
	else
		{
		return Bookmark::EDataTypeDes16;
		}
	}

/**
Registers the custom property
@internalTechnology
@test
@param		Descriptor indicating the UID of the property
@param		Category: db/folder/bookmark
@param		Datatype of the custom property
@return		Error code as a result of the register operation
*/
TInt CTestCustomPropertyStep::RegisterCustomProperty(const TPtrC& aUid, const TPtrC& aCategory, const TPtrC& aDataType)
	{
	// Resolve property type
	Bookmark::TPropertyType propertyType = GetPropertyType(aDataType);
	// Register property
	TInt error = KErrNone;
	if(aCategory.Compare(KDb()) == KErrNone)
		{
		TRAP(error, iBkDb.RegisterDatabasePropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), propertyType));
		}
	else if(aCategory.Compare(KFolder()) == KErrNone)
		{
		TRAP(error, iBkDb.RegisterFolderPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), propertyType));
		}
	else if(aCategory.Compare(KBookmark()) == KErrNone)
		{
		TRAP(error, iBkDb.RegisterBookmarkPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), propertyType));
		}
	else
		{
		ERR_PRINTF2(_L("Invalid category: %S"), aCategory);
		SetTestStepResult(EFail);
		}
	return error;	
	}

/**
De-registers the custom property
@internalTechnology
@test
@param		Descriptor indicating the UID of the property
@param		Category: db/folder/bookmark
@return		Error code as a result of the de-register operation
*/
TInt CTestCustomPropertyStep::DeRegisterCustomProperty(const TPtrC& aUid, const TPtrC& aCategory)
	{
	TInt error = KErrNone;
	if(aCategory.Compare(KDb()) == KErrNone)
		{
		TRAP(error, iBkDb.DeregisterDatabasePropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid))));
		}
	else if(aCategory.Compare(KFolder()) == KErrNone)
		{
		TRAP(error, iBkDb.DeregisterFolderPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid))));
		}
	else if(aCategory.Compare(KBookmark()) == KErrNone)
		{
		TRAP(error, iBkDb.DeregisterBookmarkPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid))));
		}
	else
		{
		ERR_PRINTF2(_L("Invalid category: %S"), aCategory);
		SetTestStepResult(EFail);
		}
	return error;	
	}
			
/**
Resolves whether to set or get a custom property and calls tha appropriate
function
@internalTechnology
@test
@param		Operation to be performed : set/get/register/deregister
@param		Category: db/folder/bookmark
@param		Uid of the custom property
@return		None
*/
void CTestCustomPropertyStep::DoSetGetCustomPropertyL(const TPtrC& aOperation, const TPtrC& aCategory, const TPtrC& aUid)
	{
	TPtrC dataType;
	if(!GetStringFromConfig(ConfigSection(), KIniDataType, dataType))
		{
		ERR_PRINTF2(_L("%S should be mentioned in ini for setting/getting a property"), &KIniDataType);
		SetTestStepResult(EFail);
		}
	else
		{
		if(aCategory.Compare(KDb) == KErrNone)
			{
			if(aOperation.Compare(KSetOperation) == KErrNone)
				{
				DoSetPropertyL(iBkDb, dataType, aUid);
				}
				else
				{
				DoGetPropertyL(iBkDb, dataType, aUid);
				}
			}
		else
			{
			TPtrC title;
			TPtrC typeOfItem;
			if(!GetStringFromConfig(ConfigSection(), KIniTypeOfItem, typeOfItem	) ||
	   		   !GetStringFromConfig(ConfigSection(), KIniTitle,		 title		)
	   		  )
				{
				ERR_PRINTF3(_L("%S and %S should be mentioned in ini for setting property for bookmark or a folder"), &KIniTypeOfItem, &KIniTitle);
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
					{// Node opened successfully. Can Set or Get property
					if(aOperation.Compare(KSetOperation) == KErrNone)
						{
						DoSetPropertyL(bkNode, dataType, aUid);
						}
					else
						{
						DoGetPropertyL(bkNode, dataType, aUid);
						}
					bkNode.Close();
					}
				}
			}
		}
	}	// DoSetGetCustomProperty

/**
Template that sets the custom property using SetCustomPropertyL
@internalTechnology
@test
@param		Reference to handle to the DB/Folder/Bookmark
@param		Datatype of the property : int/real/des8/des16
@param		Uid of the custom property
@return		None
*/
template <class RType>
void CTestCustomPropertyStep::DoSetPropertyL(RType &aBkWho, const TPtrC& aDataType, const TPtrC& aUid)
	{
	TPtrC value;
	if(!GetStringFromConfig(ConfigSection(), KIniValue, value))
		{
		ERR_PRINTF2(_L("%S should be mentioned in ini for setting a property"), &KIniValue);
		SetTestStepResult(EFail);
		}
	else
		{
		TInt error = KErrNone;
		INFO_PRINTF2(_L("Setting val for : %U"), CTestBookmarksServer::GetTUint32(aUid));
		if(aDataType.Compare(KTypeInt) == KErrNone)
			{
			TRAP(error, aBkWho.SetCustomPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), CTestBookmarksServer::GetTInt(value)));
			}
		else if(aDataType.Compare(KTypeReal) == KErrNone)
			{
			TRAP(error, aBkWho.SetCustomPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), CTestBookmarksServer::GetTReal(value)));
			}
		else if(aDataType.Compare(KTypeDes8) == KErrNone)
			{
			// Construct 8-bit descriptor
			HBufC8* hBufC8 = HBufC8::NewL(value.Length());
			hBufC8->Des().Copy(value);
			TRAP(error, aBkWho.SetCustomPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), hBufC8->Des()));
			delete hBufC8;
			}
		else if(aDataType.Compare(KTypeDes16) == KErrNone)
			{
			TRAP(error, aBkWho.SetCustomPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), value));
			}
		else
			{
			ERR_PRINTF2(_L("Invalid datatype: %S"), &aDataType);
			SetTestStepResult(EFail);
			}

		// Examine the result
		CheckErrorAndCommit(error);
		}
	}	// DoSetPropertyL

/**
Gets the custom property using GetCustomPropertyL and compares with
expected result
@internalTechnology
@test
@param		Reference to handle to the DB/Folder/Bookmark
@param		Datatype of the property : int/real/des8/des16
@param		Uid of the custom property
@return		None
*/
template <class RType>
void CTestCustomPropertyStep::DoGetPropertyL(RType &aBkWho, const TPtrC& aDataType, const TPtrC& aUid)
	{
	TPtrC expectedValue;
	if(!GetStringFromConfig(ConfigSection(), KIniExpectedValue, expectedValue))
		{
		ERR_PRINTF2(_L("%S should be mentioned in ini for setting a property"), &KIniExpectedValue);
		SetTestStepResult(EFail);
		}
	else
		{
		TInt error = KErrNone;
		TVerdict verdict = EPass;
		
		// Check the datatype and process accordingly
		if(aDataType.Compare(KTypeInt) == KErrNone)
			{
			TInt retVal = KErrNone;
			TRAP(error, aBkWho.GetCustomPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), retVal));
			if(error == KErrNone)
				{
				verdict = CompareTInts(retVal, CTestBookmarksServer::GetTInt(expectedValue));
				}
			}
		else if(aDataType.Compare(KTypeReal) == KErrNone)
			{
			TReal retVal = KErrNone;
			TRAP(error, aBkWho.GetCustomPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), retVal));
			if(error == KErrNone)
				{
				verdict = CompareTReals(retVal, CTestBookmarksServer::GetTReal(expectedValue));
				}
			}
		else if(aDataType.Compare(KTypeDes8) == KErrNone)
			{
			// Construct 8-bit descriptor
			HBufC8* retVal8 = HBufC8::NewL(Bookmark::KMaxDataLength);
			TPtr8 ptr8 = retVal8->Des();
			TRAP(error, aBkWho.GetCustomPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), ptr8));
			if(error == KErrNone)
				{
				CleanupStack::PushL(retVal8);
				HBufC* retVal = HBufC::NewL(ptr8.Length());
				retVal->Des().Copy(ptr8);
				verdict = CompareStrings(retVal->Des(), expectedValue);
				delete retVal;
				}
			CleanupStack::PopAndDestroy(retVal8);
			}
		else if(aDataType.Compare(KTypeDes16) == KErrNone)
			{
			HBufC* retVal = HBufC::NewL(Bookmark::KMaxDataLength);
			TPtr ptr = retVal->Des();
			TRAP(error, aBkWho.GetCustomPropertyL(TUid::Uid(CTestBookmarksServer::GetTUint32(aUid)), ptr));
			if(error == KErrNone)
				{
				verdict = CompareStrings(ptr, expectedValue);
				}
			delete retVal;
			}
		else
			{
			ERR_PRINTF2(_L("Invalid datatype: %S"), &aDataType);
			SetTestStepResult(EFail);
			}

		// Examine the result
		CheckErrorAndVerdict(error, verdict);
		}
	}	// DoGetPropertyL


