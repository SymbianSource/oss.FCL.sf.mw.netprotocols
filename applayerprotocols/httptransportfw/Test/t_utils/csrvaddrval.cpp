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

#include "csrvaddrval.h"
#include <uri16.h>
#include <uri8.h>
#include <uricommon.h>

// Literals used in the file
_LIT(KAddressValue,			"address");

/**
	The 8bit URL input is parsed and the value of the host name is replaced with the
	value that is retrieved from the settings.ini file.  If the value for the 
	host name does not exist in the settings.ini file then the input URL is 
	return to the calling function to be used without the values changed.  The
	calling function takes the ownership and destroys HBufC8.

	@param	aUrlToBeModified	The URL / URI that is to be parsed in which 
								the host name is to be replaced.
	@param	aIniSettingsFile	The script file that holds the sections from 
								which the value has to be retrieved
	@leave						leaves with a standard error
*/

EXPORT_C HBufC8* TSrvAddrVal::ReplaceHostNameL(const TDesC8& aUrlToBeModified, CScriptFile* aIniSettingsFile)
	{
	// Parse the Url / Uri passed to this function
	TUriParser8 uriParser8;
	uriParser8.Parse(aUrlToBeModified);

	CUri8* modifyUri8 = CUri8::NewLC(uriParser8);
	TUriC8 modifyUriC8 = modifyUri8->Uri();

	// Convert TDesC8& aUrlToBeModified to 16 bit as the function call 
	// aIniSettingsFile->ItemValue() takes a 16 bit as parameter.
	HBufC16* urlToBeModified16 = HBufC16::NewLC(KMaxUrlBufSize); 
	TPtr16 urlToBeModifiedPtr16 = urlToBeModified16->Des();
	urlToBeModifiedPtr16.Copy(aUrlToBeModified);

	// Maximum length of the host name is consider to be 256 characters.
	// Retrieve the Uri Host component value
	HBufC8* hostName8 = HBufC8::NewLC(KMaxHostNameBufSize); 
	TPtr8 hostNamePtr8 = hostName8->Des();
	hostNamePtr8.Copy(modifyUriC8.Extract(EUriHost));

	/* 
	If only a value like "WapTestIP" is passed the parser is supposed to
	use it as the host name.  But instead we have Identified that it is being
	parsed as path.  So this piece of code is to create a work around solution
	for the same
	*/
	
	if ((!modifyUriC8.IsPresent(EUriScheme)) && (!modifyUriC8.IsPresent(EUriUserinfo)) && (!modifyUriC8.IsPresent(EUriHost)))
		{
		// Retrieve the host name / IP value from settings.ini file
		hostNamePtr8.Copy(aIniSettingsFile->ItemValue(urlToBeModifiedPtr16, KAddressValue(), urlToBeModifiedPtr16));

		CleanupStack::Pop(hostName8);
		CleanupStack::PopAndDestroy(2, modifyUri8);

		return hostName8;
		}

	/* 
	If only a value like "WapTestIP:9003" is passed, the parser is supposed 
	to use it as the host name and port.  But instead we have Identified that 
	it is being parsed as scheme and path.  So this piece of code is to create 
	a work around solution for the same
	*/
	
	else if ((modifyUriC8.IsPresent(EUriScheme)) && (!modifyUriC8.IsPresent(EUriUserinfo)) && (!modifyUriC8.IsPresent(EUriHost)) && (!modifyUriC8.IsPresent(EUriPort)))
		{
		// Convert from 8 bit to 16 bit for the function ItemValue();
		urlToBeModifiedPtr16.Copy(modifyUriC8.Extract(EUriScheme));

		// Retrieve the host name / IP value from settings.ini file
		hostNamePtr8.Copy(aIniSettingsFile->ItemValue(urlToBeModifiedPtr16, KAddressValue(), urlToBeModifiedPtr16));

		// Replace the host name / IP with the value retrieved from the
		// settings.ini file
		modifyUri8->SetComponentL(hostNamePtr8, EUriScheme);
		TUriC8 newUriC8 = modifyUri8->Uri();

		// Prepare the new Url / Uri to be returned to the calling function
		HBufC8* newUrl8 = HBufC8::NewLC(KMaxUrlBufSize);
		TPtr8 newUrlPtr8 = newUrl8->Des();
		newUrlPtr8.Copy(newUriC8.UriDes());
		
		CleanupStack::Pop(newUrl8);
		// buildUrl, hostName, modifyUri
		CleanupStack::PopAndDestroy(3, modifyUri8);

		return newUrl8;
		}
	else
		{
		// Converting to 16 bit
		urlToBeModifiedPtr16.Copy(hostNamePtr8);

		// Retrieve the host name / IP value from settings.ini file
		hostNamePtr8.Copy(aIniSettingsFile->ItemValue(urlToBeModifiedPtr16, KAddressValue(), urlToBeModifiedPtr16));
		
		// Replace the host name / IP with the value retrieved from the
		// settings.ini file
		modifyUri8->SetComponentL(hostNamePtr8, EUriHost);
		TUriC8 newUriC8 = modifyUri8->Uri();
		
		// Prepare the new Url / Uri to be returned to the calling function
		HBufC8* newUrl8 = HBufC8::NewLC(KMaxUrlBufSize);
		TPtr8 newUrlPtr8 = newUrl8->Des();
		newUrlPtr8.Copy(newUriC8.UriDes());
		
		CleanupStack::Pop(newUrl8);
		// buildUrl, hostName, modifyUri
		CleanupStack::PopAndDestroy(3, modifyUri8);
	
		return newUrl8;
		}
	}

/**
	The 16bit URL input is parsed and the value of the host name is replaced with the
	value that is retrieved from the settings.ini file.  If the value for the 
	host name does not exist in the settings.ini file then the input URL is 
	return to the calling function to be used without the values changed.  The
	calling function takes the ownership and destroys HBufC8.

	@param	aUrlToBeModified	The URL / URI that is to be parsed in which 
								the host name is to be replaced.
	@param	aIniSettingsFile	The script file that holds the sections from 
								which the value has to be retrieved
	@leave						leaves with a standard error
*/

EXPORT_C HBufC16* TSrvAddrVal::ReplaceHostNameL(const TDesC16& aUrlToBeModified, CScriptFile* aIniSettingsFile)
	{
	// Parse the Url / Uri passed to this function
	TUriParser16 uriParser16;
	uriParser16.Parse(aUrlToBeModified);

	CUri16* modifyUri16 = CUri16::NewLC(uriParser16);
	TUriC16 modifyUriC16 = modifyUri16->Uri();

	// This variable is created since ItemValue
	HBufC16* hostName16 = HBufC16::NewLC(KMaxHostNameBufSize); 
	TPtr16 hostNamePtr16 = hostName16->Des();
	hostNamePtr16.Copy(modifyUriC16.Extract(EUriHost));

	// Maximum length of the host name is consider to be 256 characters.
	// This variable is used to store the retrieved Host Name value
	HBufC16* hostNameValue16 = HBufC16::NewLC(KMaxHostNameBufSize); 
	TPtr16 hostNameValuePtr16 = hostNameValue16->Des();
	hostNameValuePtr16.Copy(modifyUriC16.Extract(EUriHost));

	/* 
	If only a value like "WapTestIP" is passed the parser is supposed to
	use it as the host name.  But instead we have Identified that it is being
	parsed as path.  So this piece of code is to create a work around solution
	for the same
	*/
	
	if ((!modifyUriC16.IsPresent(EUriScheme)) && (!modifyUriC16.IsPresent(EUriUserinfo)) && (!modifyUriC16.IsPresent(EUriHost)))
		{
		// Retrieve the host name / IP value from settings.ini file
		hostNameValuePtr16.Copy(aIniSettingsFile->ItemValue(aUrlToBeModified, KAddressValue(), aUrlToBeModified));

		CleanupStack::Pop(hostNameValue16);
		CleanupStack::PopAndDestroy(2, modifyUri16);

		return hostNameValue16;
		}

	/* 
	If only a value like "WapTestIP:9003" is passed, the parser is supposed 
	to use it as the host name and port.  But instead we have Identified that 
	it is being parsed as scheme and port.  So this piece of code is to create 
	a work around solution for the same
	*/
	
	else if ((modifyUriC16.IsPresent(EUriScheme)) && (!modifyUriC16.IsPresent(EUriUserinfo)) && (!modifyUriC16.IsPresent(EUriHost)) && (!modifyUriC16.IsPresent(EUriPort)))
		{
		hostNamePtr16.Copy(modifyUriC16.Extract(EUriScheme));

		// Retrieve the host name / IP value from settings.ini file
		hostNameValuePtr16.Copy(aIniSettingsFile->ItemValue(hostNamePtr16, KAddressValue(), hostNamePtr16));

		// Replace the host name / IP with the value retrieved from the
		// settings.ini file
		modifyUri16->SetComponentL(hostNameValuePtr16, EUriScheme);
		TUriC16 newUriC16 = modifyUri16->Uri();

		// Prepare the new Url / Uri to be returned to the calling function
		HBufC16* newUrl16 = HBufC16::NewLC(KMaxUrlBufSize);
		TPtr16 newUrlPtr16 = newUrl16->Des();
		newUrlPtr16.Copy(newUriC16.UriDes());
		
		CleanupStack::Pop(newUrl16);
		CleanupStack::PopAndDestroy(3, modifyUri16); //hostName16, hostNameValue16

		return newUrl16;
		}
	else
		{
		// Retrieve the host name / IP value from settings.ini file
		hostNameValuePtr16.Copy(aIniSettingsFile->ItemValue(hostNamePtr16, KAddressValue(), hostNamePtr16));
		
		// Replace the host name / IP with the value retrieved from the
		// settings.ini file
		modifyUri16->SetComponentL(hostNamePtr16, EUriHost);
		TUriC16 newUriC16 = modifyUri16->Uri();
		
		// Prepare the new Url / Uri to be returned to the calling function
		HBufC16* newUrl16 = HBufC16::NewLC(KMaxUrlBufSize);
		TPtr16 newUrlPtr16 = newUrl16->Des();
		newUrlPtr16.Copy(newUriC16.UriDes());
		
		CleanupStack::Pop(newUrl16);
		CleanupStack::PopAndDestroy(3, modifyUri16); //hostName16, hostNameValue16
	
		return newUrl16;
		}
	}

/**
	Converts the 8 bit descriptor to 16 bit descriptor and logs it

	@param	aModify8BitDesC		The 8 bit descriptor that needs to be converted to
								16 bit descriptor.
	@param	aEngine				The engine that is used to log data.
	@leave						leaves with a standard error
*/
EXPORT_C void TSrvAddrVal::LogUsing8BitDesL(CHttpTestEngine* aEngine, const TDesC8& aModify8BitDesC)
	{
	HBufC16* modifiedBuf = HBufC16::NewLC(aModify8BitDesC.Length());
	modifiedBuf->Des().Copy(aModify8BitDesC);
	TPtr16 logValuePtr16 = modifiedBuf->Des();

	aEngine->Utils().LogIt(_L("URI: "));
	aEngine->Utils().LogIt(logValuePtr16);
	CleanupStack::PopAndDestroy(modifiedBuf);
	}
