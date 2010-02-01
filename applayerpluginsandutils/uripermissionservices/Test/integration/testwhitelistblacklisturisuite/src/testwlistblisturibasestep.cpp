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
// Contains implementation of CTestWListBListUriBaseStep class
// @internalAll
// 
//

// User Include
#include "testwlistblisturibasestep.h"

/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestWListBListUriBaseStep::CTestWListBListUriBaseStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestWListBListUriBaseStep);
	}

/**
Destructor: Closes the database handle
@internalTechnology
@test
*/
CTestWListBListUriBaseStep::~CTestWListBListUriBaseStep()
	{
	iUriDb.Close();
	delete iSched;
	iSched = NULL;
	}
	
/**
Base class virtual doTestStepPreambleL(): Opens connection with the URI database.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of opening the DB.
*/
TVerdict CTestWListBListUriBaseStep::doTestStepPreambleL()
	{
	// is this required?
	iSched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(iSched);
	
	TRAPD(error, iUriDb.OpenL());
	if(error != KErrNone)
		{
		ERR_PRINTF2(_L("Error occured while opening[creating session] database : %D\n"), error);
		//SetTestStepError(error); // is this required?
		}
	else
		{
		INFO_PRINTF1(_L("Database opened & session created successfully\n"));
		}
	return TestStepResult();
	}	// doTestPreambleL

/**
Base class pure virtual doTestStepL(): Just returns the test step result.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestWListBListUriBaseStep::doTestStepL()
	{
	return TestStepResult();
	}	// doTestStepL
	
/*
Creates data member iUriProps (RInetUri) object.
It also convert 16 bit string to 8 bit
@internalTechnology
@test
@param aUri  				A non-modifiable object of TUriC8
@param aListType  			List type of given URI
@param aServiceType  		Service type of given URI
@return		None
*/
void CTestWListBListUriBaseStep::CreateL(const TDesC& aUri, const TInt aListType, const TInt aServiceType, const TDesC& aFavName)
	{
	HBufC8* uriBuf = HBufC8::NewLC(aUri.Length());
	TPtr8 uri8Bit = uriBuf->Des();
	uri8Bit.Copy(aUri);
	INFO_PRINTF1(_L("Creating URI properties object\n"));
	iUriProps.CreateL(uri8Bit,(InetUriList::TServiceType)aServiceType,(InetUriList::TListType)aListType);
	INFO_PRINTF1(_L("Created: URI properties object successfully\n"));
	CleanupStack::PopAndDestroy(); // uriBuf
	
	if(aFavName.Compare(KNullDesC) != 0)
		{
		// Setting favName to iUriProps object
		HBufC8* favNameBuf = HBufC8::NewLC(aFavName.Length());
		TPtr8 favName8Bit = favNameBuf->Des();
		favName8Bit.Copy(aFavName);
		iUriProps.SetFavouriteNameL(favName8Bit);
		INFO_PRINTF1(_L("Favourite name of URI has been set successfully\n"));
		CleanupStack::PopAndDestroy(); // favNameBuf
		}
	}

/*
Fetch URI handle from the database
It also convert 16 bit string to 8 bit
@internalTechnology
@test
@param aUri  				A non-modifiable object of TUriC8
@param aListType  			List type of given URI
@param aServiceType  		Service type of given URI
@return		None
*/
void CTestWListBListUriBaseStep::FetchL(const TDesC& aUri, const TInt aServiceType, const TDesC& aFavName)
	{
	HBufC8* uriBuf = HBufC8::NewLC(aUri.Length());
	TPtr8 uri8Bit = uriBuf->Des();
	uri8Bit.Copy(aUri);
	INFO_PRINTF1(_L("Fetching/opening URI properties/handle\n"));
	iUriProps = iUriDb.OpenInetUriL(uri8Bit,(InetUriList::TServiceType)aServiceType);
	INFO_PRINTF1(_L("Fetched: URI properties/handle successfully\n"));
	CleanupStack::PopAndDestroy(); // uriBuf
	
	if(aFavName.Compare(KNullDesC) != 0)
		{
		// Setting favName to iUriProps object
		HBufC8* favNameBuf = HBufC8::NewLC(aFavName.Length());
		TPtr8 favName8Bit = favNameBuf->Des();
		favName8Bit.Copy(aFavName);
		iUriProps.SetFavouriteNameL(favName8Bit);
		INFO_PRINTF1(_L("Favourite name of URI has been set successfully\n"));
		CleanupStack::PopAndDestroy(); // favNameBuf
		}	
	}

/*
The call-back function that will be called when there is atleast one query results.
@internalTechnology
@test
@param aUri  				Uri which comes as a result of query
@return		ETrue - More query results, if any, should follow.
			EFalse - No more query results is required and querying will be stopped.
*/
TBool CTestWListBListUriBaseStep::OnQueryResultsL ( RInetUri aUri )
	{
	iQueryResults.Append ( aUri );
	return ETrue;
	}

/*
Release memory of all the found URI
@test
@internalTechnology
*/
void CTestWListBListUriBaseStep::ClearUriArray ()
	{	
	for ( TInt i = 0; i < iQueryResults.Count(); ++i )
		{
		iQueryResults[i].Close ();
		}
	iQueryResults.Reset ();
	iQueryResults.Close ();
	}

/*
Count of found URI
@test
@internalTechnology
@return count
*/		
TInt CTestWListBListUriBaseStep::Count ()
	{
	return iQueryResults.Count ();	
	}


/*
The call-back function that will be called to perform scheme and protocol based normalisation.
The URI will be syntax normalised before calling this function.
@test
@internalTechnology	
@param aUri The syntax normalised URI.
@return Final URI which is syntax and protocol/scheme based normalised.
*/
CUri8* CTestWListBListUriBaseStep::OnUriCustomisationL ( const TUriC8& aUri )
	{
	return SchemeOrProNormaliseUriL(aUri);
	}

/*
This is just for testing the functionality of White/BlackListUriService.
Actual implementation will be provided by client.
@test
@internalTechnology	
@param aUri URI required for Scheme and Protocol based normalization.
@return Final URI which is syntax and protocol/scheme based normalised.
*/	
CUri8* CTestWListBListUriBaseStep::SchemeOrProNormaliseUriL(const TUriC8& aUri)
	{
	_LIT8(KHttp,"http");
	_LIT8(KEmpty,"");
	_LIT8(KEighty,"80");	
	
	CUri8* normalisedUri = CUri8::NewL(aUri);
	CleanupStack::PushL(normalisedUri);
	
	if(aUri.IsPresent(EUriScheme))
		{
		HBufC8* extractScheme = HBufC8::NewLC(aUri.Extract(EUriScheme).Length());
		TPtr8 schemeComponent = extractScheme->Des();
		schemeComponent.Append(aUri.Extract(EUriScheme));
		//scheme is HTTP
		if(schemeComponent.CompareF(KHttp()) == 0)
			{
			HBufC8* port = HBufC8::NewLC(aUri.Extract(EUriPort).Length());
			TPtr8 portComponent = port->Des();
			//Port is 80
			if(portComponent.CompareF(KEighty()) == 0 || portComponent.CompareF(KEmpty()) == 0)
				{
				//remove the port component
				normalisedUri->RemoveComponentL(EUriPort);
				}
			CleanupStack::PopAndDestroy(port);
			
			//Path is empty
			HBufC8* pathBuf = HBufC8::NewLC(aUri.Extract(EUriPath).Length());
			TPtr8 pathComponent = pathBuf->Des();
			pathComponent.Append(aUri.Extract(EUriPath));
			if(pathComponent.CompareF(KEmpty()) == 0 )
				{
				//set path as '/'
				_LIT8(KSlash,"/");
				normalisedUri->SetComponentL(KSlash, EUriPath);	
				}
			CleanupStack::PopAndDestroy(pathBuf);
			}
		CleanupStack::PopAndDestroy(extractScheme);
		}
	CleanupStack::Pop(normalisedUri);
	return normalisedUri;
	}
	



