// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <uri8.h>
#include <e32base.h>
#include <http.h>
#include <chttpformencoder.h>
#include <ssl.h>
#include <signed.h>
#include "httpexampleclient.h"
#include "httpexampleutils.h"
 

// format for output of data/time values
_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");

// Format for hook file name and location for hooks
_LIT(KHookFormat, "Z:\\httptest\\%S.esk");
_LIT(KHookDir, "C:\\System\\Data\\");

_LIT(KEnterProx, "Enter Proxy authority");
_LIT(KEnterHook, "Enter Hook name (preface with '-' to delete)");
_LIT(KEnterSessId, "Enter Session ID (positive integer or 'none')");


/* 	If the file httpexampleclient.txt exists in the root directory of a drive
	it will be used to provide default initial values for when setting the URL, 
	proxy, etc. The file format is:
		[var] = [value]
	for example:
		PROXY = my.proxy.co.uk:5473
		URL = http://www.example.org/dir/file.html
	whitespace is ignored.
	
	Note: these are the the values set at run time, they are merely the default
	options presented to the user. In the above example the proxy is not turned
	on by default, but the string "my.proxy.co.uk:5473" is presetned as the default
	option when the use hits "p", saving them the need to have to type it in. 
	*/
// Defaults
_LIT(KDefaultFileFormat,"%c:\\httpexampleclient.txt");
_LIT(KHookString, "HOOK");
_LIT(KProxyString, "PROXY");
_LIT(KURLString, "URL");
_LIT(KSessionIdString, "SESSIONID");
_LIT(KNone, "none");
_LIT(KSchemeHttps, "https");



// File system root
_LIT(KFileSystemRoot,"C:\\");

// Standard headers used by default
_LIT8(KUserAgent, "HTTPExampleClient (1.0)");
_LIT8(KAccept, "*/*");

_LIT(KHttpExampleClientPanic, "HTTP-EC");

enum THttpExampleClientPanics
	{
	EReqBodySumitBufferNotAllocated,
	KBodyWithInvalidSize,
	KCouldntNotifyBodyDataPart
	};

// Size of buffer used when submitting request bodies
const TInt KMaxSubmitSize = 1024;
const TInt KMaxHeaderNameLen = 32;
const TInt KMaxHeaderValueLen = 128;


//
// Implementation of CHttpClient
//


// Supplied as the name of the test program to CHttpExampleUtils
_LIT(KHttpExampleClientTestName, "HttpExample");

CHttpClient::CHttpClient()
	: iReqBodySubmitBufferPtr(0,0)
	{
	// Initial timestamp is time now
	ResetTimeElapsed();
	}


CHttpClient::~CHttpClient()
	{
	iSess.Close();
	iFileServ.Close();
	delete iReqBodySubmitBuffer;
	delete iTransObs;
	delete iFormEncoder;
	delete iUtils;
	}

CHttpClient* CHttpClient::NewLC()
	{
	CHttpClient* me = new(ELeave) CHttpClient;
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CHttpClient* CHttpClient::NewL()
	{
	CHttpClient* me = NewLC();
	CleanupStack::Pop(me);
	return me;
	}

void CHttpClient::ConstructL()
	{
	iUtils = CHttpExampleUtils::NewL(KHttpExampleClientTestName);
	
	// Open the RHTTPSession
	iSess.OpenL();

	// Install this class as the callback for authentication requests
	InstallAuthenticationL(iSess);
	
	User::LeaveIfError(iFileServ.Connect());
	iTransObs = CHttpEventHandler::NewL(*iUtils);
	}


TBool CHttpClient::ClearHook(TPtrC aHook)
	{
	TInt err = KErrNone;
	if(aHook.Length()!=0)
		{
		err=iFileServ.Delete(aHook);
		}
	else if(iHookFileName.Length()>0) 
		{
		err=iFileServ.Delete(iHookFileName);
		if(err==KErrNone) 
			{
			iHookFileName.Zero();
			}
		}
	if(err == KErrNone)
		{
		User::InfoPrint(_L("Hook revmoved. Need to restart "));
		return ETrue;
		}
	return EFalse;
	}

// return true if need to quit
void CHttpClient::SetHookL(TDesC& aHook)
	{
	iHookFileName.Format(KHookFormat, &aHook);
	CFileMan* fileMan = CFileMan::NewL(iFileServ);
	CleanupStack::PushL(fileMan);
	TInt err = fileMan->Copy( iHookFileName, KHookDir, CFileMan::EOverWrite );
	if( err != KErrNone && err != KErrNotFound) 
		{
		User::Leave(err);
		}
	CleanupStack::PopAndDestroy(fileMan);
	User::InfoPrint(_L("Hook installed. Need to restart "));
	}

// to avoid code bloat
#define CHECKVAL(name)  if(var==K ##name##String)	{a##name.Copy(val);}

void CHttpClient::SetDefaults(TDes& aURL, TDes& aProxy, TDes& aHook, TDes& aSessionId)
	{
	TFileName filename;
	RFile file;
	filename.Format(KDefaultFileFormat, 'C');
	TInt err = file.Open(iFileServ, filename, EFileRead);
	if(err != KErrNone) 
		{
		filename.Format(KDefaultFileFormat, 'Z');
		err = file.Open(iFileServ, filename, EFileRead);
		}
	if(err == KErrNone) 
		{
		TFileText line;
		line.Set(file);
		TInt err = line.Read(filename);
		while(err == KErrNone || err ==KErrTooBig) 
			{
			filename.Trim();
			TInt div = filename.Locate('=');
			if(div>0) 
				{
				TInt i;
				for (i=div-1;i>0 && filename[i]==' ';i--);	// ibid
				TPtrC var = filename.Left(i+1);
				for (i=div+1;i<filename.Length() && filename[i]==' ';i++);	//ibid
				TPtrC val = filename.Right(filename.Length()-i);
				CHECKVAL(Hook)
				else CHECKVAL(Proxy)
				else CHECKVAL(URL)
				else CHECKVAL(SessionId);	
				}
			err = line.Read(filename);
			}
		}
		file.Close();
	}


void CHttpClient::StartClientL()
	{

	ResetTimeElapsed();
	TBuf<256> url;
	TBuf<256> prox;
	TBuf<256> hook;
	TBuf<16> sessid;
	SetDefaults(url, prox, hook, sessid);
	
	RStringPool strP = iSess.StringPool();

	// Repeat until user selects quit
	TBool done = EFalse;
	while (!done)
		{
		// Get user command from menu
		iUtils->Test().Console()->ClearScreen();
		_LIT(KSelectOption, " Select an option \n\n");
		iUtils->Test().Printf(KSelectOption);
		_LIT(KPossibleSelectionsText, " 1 Get \n 2 Post \n 3 Head \n 4 Trace \n 5 Toggle Verbose \n p Set Proxy\n s Set Session ID\n h Set Hook\n 6 Quit \n");
		_LIT(KPossibleSelections,"123456psh");
		TInt selection = iUtils->GetSelection(KPossibleSelectionsText, KPossibleSelections);
		RStringF method;
		iHasARequestBody = EFalse;


		// Set the method to use and ask for a url
		switch (selection)
			{
		case EGet: 
			method = strP.StringF(HTTP::EGET,RHTTPSession::GetTable());
			break;
		case EPost:
			method = strP.StringF(HTTP::EPOST,RHTTPSession::GetTable());
			iHasARequestBody = ETrue; 
			break;
		case EHead: 
			method = strP.StringF(HTTP::EHEAD,RHTTPSession::GetTable()); 
			break;
		case ETrace:
			method = strP.StringF(HTTP::ETRACE,RHTTPSession::GetTable()); 
			break;
		case EToggleVerbosity: 
			{
			TBool verbose = iTransObs->Verbose();
			iTransObs->SetVerbose(!verbose);
			if (!verbose)
				User::InfoPrint(_L("Verbosity is ON "));
			else
				User::InfoPrint(_L("Verbosity is OFF"));
			}
			break;
		case EQuit:
			done = ETrue;
			break;
		case EHook:
			iUtils->GetAnEntry(KEnterHook, hook);
			if(hook.Length() > 0) 
				{		
				if(hook[0]=='-')
					{
					hook.Replace(0,1,KHookDir);
					hook.Append(_L(".esk"));
					done = ClearHook(hook);
					}
				else 
					{
					SetHookL(hook);
					done = ETrue;
					}
				}
			break;
		case ESession:
			iUtils->GetAnEntry(KEnterSessId, sessid);
			if(sessid.Length() > 0) 
				{
				TLex number(sessid);
				TInt val;
				if(number.Val(val)==KErrNone)
					{
					iSess.ConnectionInfo().SetPropertyL(strP.StringF(HTTP::ESessionId,RHTTPSession::GetTable()), val);
					}
				else if (sessid==KNone) 
					{				
					iSess.ConnectionInfo().RemoveProperty(strP.StringF(HTTP::ESessionId,RHTTPSession::GetTable()));
					}
				}
			break;
		case EProxy:
			iUtils->GetAnEntry(KEnterProx, prox);
			if(prox.Length() > 0) 
				{
			  	  iSess.ConnectionInfo().SetPropertyL(iSess.StringPool().StringF(HTTP::EProxyUsage,RHTTPSession::GetTable()), iSess.StringPool().StringF(HTTP::EUseProxy,RHTTPSession::GetTable()));
				TBuf8<256> prox8;
				prox8.Copy(prox);
				 RStringF proxy = iSess.StringPool().OpenFStringL(prox8);
				CleanupClosePushL(proxy);
				 iSess.ConnectionInfo().SetPropertyL(iSess.StringPool().StringF(HTTP::EProxyAddress,RHTTPSession::GetTable()), proxy);
				CleanupStack::PopAndDestroy(&proxy);
				}
			else
				{
			 	iSess.ConnectionInfo().SetPropertyL(iSess.StringPool().StringF(HTTP::EProxyUsage,RHTTPSession::GetTable()), iSess.StringPool().StringF(HTTP::EDoNotUseProxy,RHTTPSession::GetTable()));
				}
			break;
		default: 
			 break;
			} 

		if (!done && (selection >= 0) && (selection <= 3))
			{
			// Get the URL
			_LIT(KEnterUrl, "Enter Url");
			iUtils->GetAnEntry(KEnterUrl, url);
			if (url.Length() > 0) // back to main menu if we still have an empty URL
				{
				// Get a filename to submit as our request body, for methods that need one
				if (iHasARequestBody)
					{
					GetRequestBodyL(method);
					}

				// Start the method off
				TBuf8<256> url8;
				url8.Copy(url);
				InvokeHttpMethodL(url8, method);
				}
			else
				{
				_LIT(KEmptyUrlErrorText, "You must supply a non empty url");
				iUtils->Test().Printf(KEmptyUrlErrorText);
				User::After(1000000);
				}
			}

		} // while

	// End of tests
	DisplayTimeElapsed();
	iUtils->PressAnyKey();
}

/** Get the post body data from the user. 
The user supplies name and value pairs. These are then encoded as added by CHTTPFormEncoder which acts as a data supplier
for this request. This is typically how a form body for a html form submission that uses POST might be constructed.
*/
void CHttpClient::GetPostBodyManuallyL()
	{
	if (iFormEncoder)
		{
		delete iFormEncoder;
		iFormEncoder = NULL;
		}

	iFormEncoder = CHTTPFormEncoder::NewL(); 
	TBuf<256> name;
	TBuf<256> value;
	TBuf8<256> name8;
	TBuf8<256> value8;
	
	_LIT(KGetPostName, "Enter Name (END to finish)");
	_LIT(KGetPostValue, "Enter Value ");
	_LIT(KEnd, "END");
	do
		{
		iUtils->GetAnEntry(KGetPostName, name);
		if (name.CompareF(KEnd) != 0)
			{
			iUtils->GetAnEntry(KGetPostValue, value);
			name8.Copy(name);
			value8.Copy(value);
			iFormEncoder->AddFieldL(name8, value8);
			}
		}
		while (name.CompareF(KEnd) != 0);
	}


/** Gets the body that you wish to submit when using a POST Method
This can be supplied as a file or can be input manually in name,value pairs
*/
void CHttpClient::GetRequestBodyL(RStringF& aMethod)
	{
	if (aMethod== iSess.StringPool().StringF(HTTP::EPOST,RHTTPSession::GetTable()))
		{
		// get a post body by file or contruct manually?
		_LIT(KConstructPostManually, "\n> Construct Post Manually?  Yes | No ");
		_LIT(KYesNo, "YyNn");
		TInt selection = iUtils->GetSelection(KConstructPostManually,KYesNo);
		if (selection < 2) // 2 is NO
			{
			iManualPost = ETrue;
			GetPostBodyManuallyL();
			return;
			}
		// else carry on as usual and get post data from a file
		}

	iManualPost = EFalse;
	_LIT(KRequestPathPrompt, "Enter path to request body file: ");
	iUtils->GetAnEntry(KRequestPathPrompt, iReqBodyFileName);
	_LIT(KRequestBodyContentType, "Enter request body content-type: ");
	iUtils->GetAnEntry(KRequestBodyContentType, iReqBodyContentType);
	iParsedFileName.Set(KFileSystemRoot,&iReqBodyFileName,NULL);

	// Check it exists and open a file handle
	if (iFileServ.IsValidName(iReqBodyFileName))
		{
		TInt err= iReqBodyFile.Open(iFileServ, iParsedFileName.FullName(), EFileRead);
		if(err!=KErrNone)
		   {
		   _LIT(KFileNotOpen, "Unable to open a file.\n");
		   iUtils->Test().Printf(KFileNotOpen);
		   User::Leave(KErrNotFound);			
		   } 
		delete iReqBodySubmitBuffer;
		iReqBodySubmitBuffer = NULL;
		iReqBodySubmitBuffer = HBufC8::NewMaxL(KMaxSubmitSize);
		iReqBodySubmitBufferPtr.Set(iReqBodySubmitBuffer->Des());
		}
	else
		{
		_LIT(KFileNameNotValid, "The specified filename is not valid!.\n");
		iUtils->Test().Printf(KFileNameNotValid);
		User::Leave(KErrBadName);
		}
	}

TBool CHttpClient::GetNextDataPart(TPtrC8& aDataPart)
	{
	__ASSERT_DEBUG(iReqBodySubmitBuffer, User::Panic(KHttpExampleClientPanic, EReqBodySumitBufferNotAllocated));
	// Read from the request body file
	iNoMoreDate = EFalse;
	TInt err = iReqBodyFile.Read(iReqBodySubmitBufferPtr);
	if (err == KErrNone)
		{
		aDataPart.Set(iReqBodySubmitBufferPtr);
		++iDataChunkCount;
		iNoMoreDate = (iReqBodySubmitBufferPtr.Length() == 0);
		}
	return iNoMoreDate;
	}

void CHttpClient::ReleaseData()
	{
	// Clear out the submit buffer
	TPtr8 buff = iReqBodySubmitBuffer->Des();
	buff.Zero();
	if (iNoMoreDate==EFalse)
		{
		// Notify HTTP of more data available immediately, since it's being read from file
		TRAPD(err, iTrans.NotifyNewRequestBodyPartL());
		if (err != KErrNone)
			User::Panic(KHttpExampleClientPanic, KCouldntNotifyBodyDataPart);
		}
	}

TInt CHttpClient::OverallDataSize()
	{
	TInt size = 0;
	TInt err = iReqBodyFile.Size(size);
	if (err < 0)
		User::Panic(KHttpExampleClientPanic,KBodyWithInvalidSize);

	return size;
	}

TInt CHttpClient::Reset()
  {
  if (iHasARequestBody)
    {
    // Reset to beginning of file
	TInt pos = 0;
    iReqBodyFile.Seek(ESeekStart, pos);
    return KErrNone;
    }
  else
    {
    return KErrNotSupported;
    }
 } 

/** Invoke the http method
This actually creates the transaction, sets the headers and body and then starts the transaction 
*/
void CHttpClient::InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod)
	{
	// Set the protocol, before the first transaction gets started
/*	if (iUseWspProtocol)
		{
		RHTTPConnectionInfo ci = iSess.ConnectionInfo();
		ci.SetPropertyL(iSess.StringPool().StringF(HTTP::EProtocol,RHTTPSession::GetTable()),
						THTTPHdrVal(iSess.StringPool().StringF(HTTP::EWSP,RHTTPSession::GetTable())));
		}*/

	iDataChunkCount = 0;
	TUriParser8 uri; 
	uri.Parse( aUri );
	HBufC* scheme = uri.DisplayFormL( EUriScheme );
	
	iTransObs->SetSecuredHttp( !scheme->CompareF( KSchemeHttps ) );
	delete scheme;
	
	iTrans = iSess.OpenTransactionL(uri, *iTransObs, aMethod);
	RHTTPHeaders hdr = iTrans.Request().GetHeaderCollection();

	// Add headers appropriate to all methods
	SetHeaderL(hdr, HTTP::EUserAgent, KUserAgent);
	SetHeaderL(hdr, HTTP::EAccept, KAccept);

	// Add headers and body data for methods that use request bodies
	if (iHasARequestBody)
		{
		// Content type header
		TBuf8<KMaxContentTypeSize> contTypeBuf;
		contTypeBuf.Copy(iReqBodyContentType);
		RStringF contTypeStr = iSess.StringPool().OpenFStringL(contTypeBuf);
		CleanupClosePushL(contTypeStr);
		THTTPHdrVal contType(contTypeStr);
		hdr.SetFieldL(iSess.StringPool().StringF(HTTP::EContentType,RHTTPSession::GetTable()), contType);
		CleanupStack::PopAndDestroy(&contTypeStr);
		
		MHTTPDataSupplier* dataSupplier = this;
		if (iManualPost)
			dataSupplier = iFormEncoder;
		iTrans.Request().SetBody(*dataSupplier);
		}
    
	/*
	//Set TimeOut Values
	RStringF sendTimeOutProp = iSess.StringPool().StringF(HTTP::ESendTimeOutValue, iSess.GetTable());
	THTTPHdrVal sendTimeOutPropValue;
	sendTimeOutPropValue.SetInt(10);
	iTrans.PropertySet().SetPropertyL(sendTimeOutProp, sendTimeOutPropValue);
 
	RStringF recvTimeOutProp = iSess.StringPool().StringF(HTTP::EReceiveTimeOutValue, iSess.GetTable());
	THTTPHdrVal recvTimeOutPropValue;
	recvTimeOutPropValue.SetInt(10);
	iTrans.PropertySet().SetPropertyL(recvTimeOutProp, recvTimeOutPropValue);
	*/

	// submit the transaction
	iTrans.SubmitL();

	// Start the scheduler, once the transaction completes or is cancelled on an error the scheduler will be
	// stopped in the event handler
	CActiveScheduler::Start();

	// all done
	iUtils->PressAnyKey();

	// close the request body file, if one was opened
	if (iHasARequestBody)
		iReqBodyFile.Close();
	}

void CHttpClient::SetHeaderL(RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue)
	{
	RStringF valStr = iSess.StringPool().OpenFStringL(aHdrValue);
	CleanupClosePushL(valStr);
	THTTPHdrVal val(valStr);
	aHeaders.SetFieldL(iSess.StringPool().StringF(aHdrField,RHTTPSession::GetTable()), val);
	CleanupStack::PopAndDestroy(&valStr); 
	}

void CHttpClient::ResetTimeElapsed()
// Resets timestamp to time now
	{
	iLastTimeStamp.UniversalTime(); 
	}


void CHttpClient::DisplayTimeElapsed()
// Calculate elapsed time since last measurement, and display
	{
	TTime timeNow;
	timeNow.UniversalTime();
	TTimeIntervalMicroSeconds elapsedMicroSec =
									timeNow.MicroSecondsFrom(iLastTimeStamp);
	iLastTimeStamp = timeNow;
	iUtils->Test().Printf(
		_L("Time elapsed since last measurement is: %d ms\n"),
		elapsedMicroSec.Int64()/1000
		);
	}


/** Called when a authenticated page is requested
Asks the user for a username and password that would be appropriate for the url that was
supplied.
*/
TBool CHttpClient::GetCredentialsL(const TUriC8& aURI, RString aRealm, 
								   RStringF aAuthenticationType,
								   RString& aUsername, 
								   RString& aPassword)

	{
	TBuf<KMaxUserEntrySize> scratch;
	TBuf8<KMaxUserEntrySize> scratch8;

	// Convert to 16 bit to display
	HBufC* uriDesBuf = HBufC::NewLC(aURI.UriDes().Length());
	TPtr uriDesPtr = uriDesBuf->Des();
	uriDesPtr.Copy(aURI.UriDes());

	HBufC* uriRealmBuf = HBufC::NewLC(aRealm.DesC().Length());
	TPtr uriRealmPtr = uriRealmBuf->Des();
	uriRealmPtr.Copy(aRealm.DesC());

	// Prompt user for credentials.
	iUtils->Test().Printf(_L("Enter credentials for URL %S, realm %S\n"), &uriDesPtr, & uriRealmPtr);

	CleanupStack::PopAndDestroy(2, uriDesBuf);

	scratch.Copy(aAuthenticationType.DesC());
	iUtils->Test().Printf(_L("Using %S authentication\n"), &scratch);
	iUtils->GetAnEntry(_L("Username (or QUIT to give up): "), scratch);
	scratch8.Copy(scratch);
	if (scratch8.CompareF(_L8("quit")))
		{
		TRAPD(err, aUsername = aRealm.Pool().OpenStringL(scratch8));
		if (!err)
			{
			iUtils->GetAnEntry(_L("Password: "), scratch);
			scratch8.Copy(scratch);
			TRAP(err, aPassword = aRealm.Pool().OpenStringL(scratch8));
			if (!err)
				return ETrue;
			}
		}
	return EFalse;
	}


//
// Implementation of class CHttpEventHandler
//

void CHttpEventHandler::ConstructL()
	{
	User::LeaveIfError(iFileServ.Connect());
	}


CHttpEventHandler::CHttpEventHandler(CHttpExampleUtils& aUtils) :
	iUtils(aUtils)
	{
	}


CHttpEventHandler::~CHttpEventHandler()
	{
	iFileServ.Close();
	}


CHttpEventHandler* CHttpEventHandler::NewLC(CHttpExampleUtils& aUtils)
	{
	CHttpEventHandler* me = new(ELeave)CHttpEventHandler(aUtils);
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}


CHttpEventHandler* CHttpEventHandler::NewL(CHttpExampleUtils& aUtils)
	{
	CHttpEventHandler* me = NewLC(aUtils);
	CleanupStack::Pop(me);
	return me;
	}


void CHttpEventHandler::SetVerbose(TBool aVerbose)
	{
	iVerbose = aVerbose;
	}


TBool CHttpEventHandler::Verbose() const
	{
	return iVerbose;
	}
	
void CHttpEventHandler::SetSecuredHttp( TBool aSecuredHttp )
	{
	iSecuredHttp = aSecuredHttp;
	}


TBool CHttpEventHandler::SecuredHttp() const
	{
	return iSecuredHttp;
	}


void CHttpEventHandler::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received. We can determine now if there is
			// going to be a response body to save.
			RHTTPResponse resp = aTransaction.Response();
			TInt status = resp.StatusCode();
			RStringF statusStr = resp.StatusText();
			TBuf<32> statusStr16;
			statusStr16.Copy(statusStr.DesC());
			iUtils.Test().Printf(_L("Status: %d (%S)\n"), status, &statusStr16);

			// Dump the headers if we're being verbose
			if (iVerbose)
			{
				DumpRespHeadersL(aTransaction);
				
				if ( iSecuredHttp )
					{
					CheckCertificatesL( aTransaction );
					}
			}

			// Determine if the body will be saved to disk
			iSavingResponseBody = EFalse;
			TBool cancelling = EFalse;
			if (resp.HasBody() && (status >= 200) && (status < 300) && (status != 204))
				{
				TInt dataSize = resp.Body()->OverallDataSize();
				if (dataSize >= 0)
					iUtils.Test().Printf(_L("Response body size is %d\n"), dataSize);
				else
					iUtils.Test().Printf(_L("Response body size is unknown\n"));

				TInt selection = iUtils.GetSelection(_L("\n> Save response to disk? Yes | No | Cancel"), _L("YyNnCc"));
				iSavingResponseBody = (selection < 2);
				cancelling = (selection == 4) || (selection == 5);
				}

			// If we're cancelling, must do it now..
			if (cancelling)
				{
				iUtils.Test().Printf(_L("\nTransaction Cancelled\n"));
				aTransaction.Close();
				CActiveScheduler::Stop();
				}
			else if (iSavingResponseBody) // If we're saving, then open a file handle for the new file
				{
				iUtils.GetAnEntry(_L("Enter filename including path to save response body"), iRespBodyFileName);
				iParsedFileName.Set(KFileSystemRoot,&iRespBodyFileName,NULL);

				// Check it exists and open a file handle
				TInt valid = iFileServ.IsValidName(iRespBodyFileName);
				if (!valid)
					{
					iUtils.Test().Printf(_L("The specified filename is not valid!.\n"));
					iSavingResponseBody = EFalse;
					}
				else
					{
					TInt err = iRespBodyFile.Replace(iFileServ,
													 iParsedFileName.FullName(),
													 EFileWrite|EFileShareExclusive);
					if (err)
						{
						iSavingResponseBody = EFalse;
						User::Leave(err);
						}
					}
				}

			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Get the body data supplier
			iRespBody = aTransaction.Response().Body();

			// Some (more) body data has been received (in the HTTP response)
			if (iVerbose)
				DumpRespBody(aTransaction);
			else
				iUtils.Test().Printf(_L("*"));

			// Append to the output file if we're saving responses
			if (iSavingResponseBody)
				{
				TPtrC8 bodyData;
				TBool lastChunk = iRespBody->GetNextDataPart(bodyData);
				iRespBodyFile.Write(bodyData);
				if (lastChunk)
					iRespBodyFile.Close();
				}

			// Done with that bit of body data
			iRespBody->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			iUtils.Test().Printf(_L("\nTransaction Complete\n"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			iUtils.Test().Printf(_L("Transaction Successful\n"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::EFailed:
			{
			iUtils.Test().Printf(_L("Transaction Failed\n"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::ERedirectedPermanently:
			{
			iUtils.Test().Printf(_L("Permanent Redirection\n"));
			} break;
		case THTTPEvent::ERedirectedTemporarily:
			{
			iUtils.Test().Printf(_L("Temporary Redirection\n"));
			} break;
		case THTTPEvent::ERedirectRequiresConfirmation:
 			{
			// 301(Moved Permanently), 302(Found) or 307(Temporary Redirect) status is received 
			// from a transaction and hence ERedirectRequiresConfirmation is sent by filter
			// client has opted to close the transaction
			iUtils.Test().Printf(_L("Redirect requires confirmation\n"));
 			aTransaction.Close();
 			CActiveScheduler::Stop();
 			} break;
	    case THTTPEvent::EReceiveTimeOut:
			{
			iUtils.Test().Printf(_L("Receive TimeOut\n"));
			}break;
		case THTTPEvent::ESendTimeOut:
			{
			iUtils.Test().Printf(_L("Send TimeOut\n"));
			}break;
		default:
			{
			iUtils.Test().Printf(_L("<unrecognised event: %d>\n"), aEvent.iStatus);
			// close off the transaction if it's an error
			if (aEvent.iStatus < 0)
				{
				aTransaction.Close();
				CActiveScheduler::Stop();
				}
			} break;
		}
	}

TInt CHttpEventHandler::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	iUtils.Test().Printf(_L("MHFRunError fired with error code %d\n"), aError);

	return KErrNone;
	}


void CHttpEventHandler::DumpRespHeadersL(RHTTPTransaction& aTrans)
	{
	RHTTPResponse resp = aTrans.Response();
	RStringPool strP = aTrans.Session().StringPool();
	RHTTPHeaders hdr = resp.GetHeaderCollection();
	THTTPHdrFieldIter it = hdr.Fields();

	TBuf<KMaxHeaderNameLen>  fieldName16;
	TBuf<KMaxHeaderValueLen> fieldVal16;

	while (it.AtEnd() == EFalse)
		{
		RStringTokenF fieldName = it();
		RStringF fieldNameStr = strP.StringF(fieldName);
		THTTPHdrVal fieldVal;
		if (hdr.GetField(fieldNameStr,0,fieldVal) == KErrNone)
			{
			const TDesC8& fieldNameDesC = fieldNameStr.DesC();
			fieldName16.Copy(fieldNameDesC.Left(KMaxHeaderNameLen));
			switch (fieldVal.Type())
				{
			case THTTPHdrVal::KTIntVal:
				iUtils.Test().Printf(_L("%S: %d\n"), &fieldName16, fieldVal.Int());
				break;
			case THTTPHdrVal::KStrFVal:
				{
				RStringF fieldValStr = strP.StringF(fieldVal.StrF());
				const TDesC8& fieldValDesC = fieldValStr.DesC();
				fieldVal16.Copy(fieldValDesC.Left(KMaxHeaderValueLen));
				iUtils.Test().Printf(_L("%S: %S\n"), &fieldName16, &fieldVal16);
				}
				break;
			case THTTPHdrVal::KStrVal:
				{
				RString fieldValStr = strP.String(fieldVal.Str());
				const TDesC8& fieldValDesC = fieldValStr.DesC();
				fieldVal16.Copy(fieldValDesC.Left(KMaxHeaderValueLen));
				iUtils.Test().Printf(_L("%S: %S\n"), &fieldName16, &fieldVal16);
				}
				break;
			case THTTPHdrVal::KDateVal:
				{
				TDateTime date = fieldVal.DateTime();
				TBuf<40> dateTimeString;
				TTime t(date);
				t.FormatL(dateTimeString,KDateFormat);

				iUtils.Test().Printf(_L("%S: %S\n"), &fieldName16, &dateTimeString);
				} 
				break;
			default:
				iUtils.Test().Printf(_L("%S: <unrecognised value type>\n"), &fieldName16);
				break;
				}

			// Display realm for WWW-Authenticate header
			RStringF wwwAuth = strP.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable());
			if (fieldNameStr == wwwAuth)
				{
				// check the auth scheme is 'basic'
				RStringF basic = strP.StringF(HTTP::EBasic,RHTTPSession::GetTable());
				RStringF realm = strP.StringF(HTTP::ERealm,RHTTPSession::GetTable());
				THTTPHdrVal realmVal;
				if ((fieldVal.StrF() == basic) && 
					(!hdr.GetParam(wwwAuth, realm, realmVal)))
					{
					RStringF realmValStr = strP.StringF(realmVal.StrF());
					fieldVal16.Copy(realmValStr.DesC());
					iUtils.Test().Printf(_L("Realm is: %S\n"), &fieldVal16);
					}
				}
			}
		++it;
		}
	}

void CHttpEventHandler::DumpRespBody(RHTTPTransaction& aTrans)
	{
	MHTTPDataSupplier* body = aTrans.Response().Body();
	TPtrC8 dataChunk;
	TBool isLast = body->GetNextDataPart(dataChunk);
	DumpIt(dataChunk);
	if (isLast)
		iUtils.Test().Printf(_L("Got last data chunk.\n"));
	}


void CHttpEventHandler::DumpIt(const TDesC8& aData)
//Do a formatted dump of binary data
	{
	// Iterate the supplied block of data in blocks of cols=80 bytes
	const TInt cols=16;
	TInt pos = 0;
	TBuf<KMaxFileName - 2> logLine;
	TBuf<KMaxFileName - 2> anEntry;
	const TInt dataLength = aData.Length();
	while (pos < dataLength)
		{
		//start-line hexadecimal( a 4 digit number)
		anEntry.Format(TRefByValue<const TDesC>_L("%04x : "), pos);
		logLine.Append(anEntry);

		// Hex output
		TInt offset;
		for (offset = 0; offset < cols; ++offset)
			{
			if (pos + offset < aData.Length())
				{
				TInt nextByte = aData[pos + offset];
				anEntry.Format(TRefByValue<const TDesC>_L("%02x "), nextByte);
				logLine.Append(anEntry);
				}
			else
				{
				//fill the remaining spaces with blanks untill the cols-th Hex number 
				anEntry.Format(TRefByValue<const TDesC>_L("   "));
				logLine.Append(anEntry);
				}
			}
			anEntry.Format(TRefByValue<const TDesC>_L(": "));
			logLine.Append(anEntry);

		// Char output
		for (offset = 0; offset < cols; ++offset)
			{
			if (pos + offset < aData.Length())
				{
				TInt nextByte = aData[pos + offset];
				if ((nextByte >= ' ') && (nextByte <= '~'))
					{
					anEntry.Format(TRefByValue<const TDesC>_L("%c"), nextByte);
					logLine.Append(anEntry);
					}
				else
					{
					anEntry.Format(TRefByValue<const TDesC>_L("."));
					logLine.Append(anEntry);
					}
				}
			else
				{
				anEntry.Format(TRefByValue<const TDesC>_L(" "));
				logLine.Append(anEntry);
				}
			}
			iUtils.Test().Printf(TRefByValue<const TDesC>_L("%S\n"), &logLine);	
			logLine.Zero();

		// Advance to next  byte segment (1 seg= cols)
		pos += cols;
		}
	}
	
	
void CHttpEventHandler::CheckCertificatesL(RHTTPTransaction& aTrans)
	{
	
	if ( aTrans.Session().ServerCert() )
		{
		iUtils.Test().Printf( _L("Session CCertificate found\n") );
		
		const CCertificate* sessTCert;
		sessTCert = aTrans.ServerCert();
		
		if( sessTCert )
			{
			TPtrC8 serialNo8(sessTCert->SerialNumber());
			TBuf<KMaxUserEntrySize> serialNo16;
			serialNo16.Copy(serialNo8);
			HBufC16* issuer(sessTCert->IssuerL());
			iUtils.Test().Printf( _L("Serial No: %S, \n"),&serialNo16 );
			iUtils.Test().Printf( _L("Issuer: %S, "),issuer );
			delete issuer;
			}
		}
	else
		{
		iUtils.Test().Printf( _L("Session CCertificate NULL\n") );
		}
		
	if ( aTrans.ServerCert() )
		{
		iUtils.Test().Printf( _L("Transaction CCertificate found\n") );
		const CCertificate* transTCert;
		transTCert = aTrans.ServerCert();
		
		if( transTCert )
			{
			TPtrC8 serialNo8(transTCert->SerialNumber());
			TBuf<KMaxUserEntrySize> serialNo16;
			serialNo16.Copy(serialNo8);
			HBufC16* issuer(transTCert->IssuerL());
			iUtils.Test().Printf( _L("Serial No: %S, \n"),&serialNo16 );
			iUtils.Test().Printf( _L("Issuer: %S, \n"),issuer );
			delete issuer;
			}
		}
	else
		{
		iUtils.Test().Printf( _L("Transaction CCertificate NULL\n") );
		}
	}


//
// Main implementation
//

LOCAL_D void TestL()
// Create a test object, invoke the tests using it and remove
	{
	// Start C32 and initalize some device drivers. This is necessary when running a test console as these won't 
	// have been started
	CHttpExampleUtils::InitCommsL();

	// create an active scheduler to use
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	// Create and start the client
	CHttpClient* httpCli = CHttpClient::NewLC();
	httpCli->StartClientL();

	CleanupStack::PopAndDestroy(2); // httpCli, scheduler
	}


GLDEF_C TInt E32Main()
// Main program - run the tests within a TRAP harness, reporting any errors that
// occur via the panic mechanism. Test for memory leaks using heap marking.
	{

	__UHEAP_MARK;
	CTrapCleanup* tc=CTrapCleanup::New();

	TRAPD(err,TestL());
	if (err!=KErrNone)
		User::Panic(_L("Test failed with error code: %i"), err);
	delete tc;
	__UHEAP_MARKEND;
	return KErrNone;
	}

