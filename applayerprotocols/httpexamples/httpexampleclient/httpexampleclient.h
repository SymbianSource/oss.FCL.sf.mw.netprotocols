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
// HTTPEXAMPLECLIENT is a simple text based http client. 
// It is intended as an example and introduction to the HTTP Client API's available in 
// The Symbian Platform Release 6.2
// Users should have a brief understanding of HTTP and the different types of requests and responses you can get
// from servers. They should also be familiar with setting up a communications database (cdbv2.dat) with suitable settings
// for modems/NTRas and ISP's.
// 
//

#include <e32test.h>
#include <f32file.h>
#include <http/mhttpauthenticationcallback.h>

// Maximum size of buffer to hold content-type data
//
const TInt KMaxContentTypeSize = 64;


//
// Definition of CHttpClient
//
class CHttpEventHandler;
class CHttpExampleUtils;


/** The example http client. Intended as a simple example of an http client. It is a simple text console app that allows 
	basic http methods to be tested. It allows data for the post method to be supplied via a file or built up dynamically.
	It also allows responses to be saved to a file

	It is derived from MHTTPAuthenticationCallback to allow access to servers that require a user to submit a username and
	password to access protected sections of that server.

*/
class CHttpClient : public CBase, public MHTTPDataSupplier, 
					public MHTTPAuthenticationCallback
	{
public:
	virtual ~CHttpClient();
	static CHttpClient* NewLC();
	static CHttpClient* NewL();
	void StartClientL();
// methods inherited from MHTTPDataSupplier
	virtual TBool GetNextDataPart(TPtrC8& aDataPart);
	virtual void ReleaseData();
	virtual TInt OverallDataSize();
	virtual TInt Reset();

// methods inherited from MHTTPAuthenticationCallback
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
								 RStringF aAuthenticationType,
								 RString& aUsername, 
								 RString& aPassword);

protected:
	CHttpClient();
	void ConstructL();
private:

	enum TMenuItems
		{
		EGet,
		EPost,
		EHead,
		ETrace,
		EToggleVerbosity,
		EQuit,
		EProxy,
		ESession,
		EHook
		};

	void ResetTimeElapsed();
	void DisplayTimeElapsed();
	//
	void InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod);
	void SetHeaderL(RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue);
	void GetRequestBodyL(RStringF& aMethod);
	void GetPostBodyManuallyL();
	void SetHookL(TDesC& aHook);
	TBool ClearHook(TPtrC aHook = KNullDesC());
	void SetDefaults(TDes& aURL, TDes& aProxy, TDes& aHook, TDes& aSessionId);
	
private:
	TTime iLastTimeStamp;
	TInt iDataChunkCount;
	RHTTPSession iSess;
	RHTTPTransaction iTrans;
	TBool iHasARequestBody;
	RFs iFileServ;
	RFile iReqBodyFile;
	TFileName iReqBodyFileName;
	TParse iParsedFileName;
	TBuf<KMaxContentTypeSize> iReqBodyContentType;
	CHttpEventHandler* iTransObs;
	HBufC8* iReqBodySubmitBuffer;
	TPtr8 iReqBodySubmitBufferPtr;
	CHTTPFormEncoder* iFormEncoder;		// used for manual post submissions
	TBool iManualPost;
	CHttpExampleUtils* iUtils;
	TBool iNoMoreDate;
	TFileName iHookFileName;
	};



/** Handles all events for the active transaction. Just displays and logs events that occur.
*/
class CHttpEventHandler : public CBase, public MHTTPTransactionCallback
	{
public:
	virtual ~CHttpEventHandler();
	static CHttpEventHandler* NewLC(CHttpExampleUtils& aUtils);
	static CHttpEventHandler* NewL(CHttpExampleUtils& aUtils);
	void SetVerbose(TBool aVerbose);
	TBool Verbose() const;
	void CheckCertificatesL(RHTTPTransaction& aTrans);
	void SetSecuredHttp(TBool aSecuredHttp);
	TBool SecuredHttp() const;
	//
	// methods from MHTTPTransactionCallback
	//
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

protected:
	CHttpEventHandler(CHttpExampleUtils& aUtils);
	void ConstructL();
private:
	void DumpRespHeadersL(RHTTPTransaction& aTrans);
	void DumpRespBody(RHTTPTransaction& aTrans);
	void DumpIt(const TDesC8& aData);
	TBool iVerbose;
	TBool iSavingResponseBody;
	RFs iFileServ;
	RFile iRespBodyFile;
	TFileName iRespBodyFileName;
	TParse iParsedFileName;
	MHTTPDataSupplier* iRespBody;
	CHttpExampleUtils& iUtils; // not owned
	TBool iSecuredHttp;
	};

