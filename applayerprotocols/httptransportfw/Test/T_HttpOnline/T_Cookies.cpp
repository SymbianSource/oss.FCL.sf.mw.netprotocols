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
// defines test class: CTextModeTestCookies
// 
//

#include "T_Cookies.h"
#include "examplecookiemanager.h"
#include <http/ccookiefilterinterface.h>

// User include
#include "csrvaddrval.h"

// Literals used in the file
_LIT8(KHttpCookieSet1Url,			"http://WapTestIP/perl/cookies/1/cookieset1.pl");
_LIT8(KHttpCookieSet2Url,			"http://WapTestIP/perl/cookies/1/cookieset2.pl");
_LIT8(KHttpCookieSet3Url,			"http://WapTestIP/perl/cookies/1/extra/cookieset3.pl");
_LIT8(KHttpCookieCheck1Url,			"http://WapTestIP/perl/cookies/1/dumpform.pl");
_LIT8(KHttpCookieCheck2Url,			"http://WapTestIP/perl/cookies/1/extra/dumpform.pl");
_LIT8(KHttpCookieWrongPathUrl,		"http://WapTestIP/perl/cookies/2/wrongpath.pl");
_LIT8(KHttpCookieGoodDomainUrl,		"http://WapTestName/perl/cookies/3/gooddomain.pl");
_LIT8(KHttpCookieBadDomainUrl,		"http://WapTestName/perl/cookies/4/baddomain.pl");
_LIT8(KHttpCookieAmendDomainUrl,	"http://WapTestName/perl/cookies/5/amenddomain.pl");
_LIT8(KHttpCookieNoExpiryUrl,		"http://WapTestName/perl/cookies/6/noexpiry.pl");
_LIT8(KHttpCookie2Set1Url,			"http://WapTestIP:9000/tryit?req=cook4.txt&resp=cook3.txt");
_LIT8(KHttpCookie2PortSetUrl,		"http://WapTestIP:9000/tryit?req=portok1.txt&resp=portok2.txt");
_LIT8(KHttpCookie2BadPortSetUrl,	"http://WapTestIP:9000/tryit?req=badport1.txt&resp=badport2.txt");
_LIT8(KHttpCookieSetTwiceUrl,		"http://WapTestIP/perl/cookies/10/cookieset0.pl");

const TInt KErrCookieTestFailed = -7397;

/**
	This function is called to create a new instance of the class 
	CTextModeTestCookies

	@param aTestNumber			The test number that has to be executed
	@param aIniSettingsFile		The script file that holds the sections
								from which the value has to be retrieved
	@leave 						KErrNoMemory if there is no memory to allocate for the object
*/
CTextModeTestCookies* CTextModeTestCookies::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CTextModeTestCookies* self = new(ELeave)CTextModeTestCookies(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
	Destructor
*/
CTextModeTestCookies::~CTextModeTestCookies()
	{
	delete iCookieManager;
	iTransArray.Close();
	}

/**
	This C++ constructor is called in the NewL function and is used to 
	instantiate iTestNumber and iIniSettingsFile

	@param aTestNumber		- The test number that has to be executed
	@param aIniSettingsFile	- The script file that holds the sections
							  from which the value has to be retrieved
*/
CTextModeTestCookies::CTextModeTestCookies(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	// does nothing
	}

/**
	This constructor is called in the NewL function and used by the class to
	construct self.  Currently this has no functionality.  In future this 
	constructor can be used.
*/
void CTextModeTestCookies::ConstructL()
	{
	}

/**
	This function is called to initiate the tests.
	It may leave with any of system wide error codes.
*/
void CTextModeTestCookies::DoRunL()
	{
	_LIT8(KProtocol, "HTTP/TCP");

	// Open the HTTP session

	if (iTestNumber == 11)
		{
		// Tests fix for defect ..
		// INC036279 - RHTTPSession::OpenL() can cause multiple deletes of 
		// iImplementation.  If the callback to ConfigureSessionFiltersL() 
		// (below) leaves then calls to RHTTPSession::Close() will panic as 
		// iImplementation has already been deleted.

		TRAPD( err, iSession.OpenL(KProtocol(), this));
		if (err)
			{
			iSession.Close();
			User::Leave(err);
			}
		}
	else
		{
		iSession.OpenL(KProtocol(), this);
		}

	CleanupClosePushL(iSession);

	switch(iTestNumber)
		{
		case 1:
			DoCookieTest1L();
			break;
		case 2:
			DoCookieTest2L();
			break;
		case 3:
			DoCookieTest3L();
			break;
		case 4:
			DoCookieTest4L();
			break;
		case 5:
			DoCookieTest5L();
			break;
		case 6:
			DoCookieTest6L();
			break;
		case 7:
			DoCookieTest7L();
			break;
		case 8:
			DoCookieTest8L();
			break;
		case 9:
			DoCookieTest9L();
			break;
		case 10:
			DoCookieTest10L();
			break;
		case 11:
			break;
		case 12:
			DoCookieTest12L();
			break;
		default:
			__DEBUGGER(); // Invalid test number
		}

	// Close anything opened
	delete iCookieManager;
	iCookieManager = NULL;
	CleanupStack::PopAndDestroy(&iSession); //close iSession

	// If any transaction failed then fail the test
	User::LeaveIfError(iFailureError);
	}

/**
	If a test is failed then is function is executed to log the error code
	with which the test failed

	@param	aErr	The error code with which the test failed.
*/
TInt CTextModeTestCookies::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CTextModeTestCookies::DoCancel()
	{
	}

const TDesC& CTextModeTestCookies::TestName()
	{
	_LIT(KHeaderTestName,"CTextModeTestCookies");
	return KHeaderTestName;
	}

TInt CTextModeTestCookies::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTextModeTestCookies::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received
			iEngine->Utils().LogIt(_L("<Got response headers>"));
			if (iTestNumber == 12)
				{
				ValidateResponseL();
				}
			else
				{
				ValidateRequestL();
				}
			} break;
		case THTTPEvent::ESubmit:
			{
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Some (more) body data has been received (in the HTTP response)
			iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
			ValidateBodyL(aTransaction);
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			iEngine->Utils().LogIt(_L("<Transaction Complete>"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			// The transaction succeeded
			iEngine->Utils().LogIt(_L("<Transaction succeeded>"));
			aTransaction.Close();
			if (--iOpenTransactionCount == 0)
				{
				CActiveScheduler::Stop();
				}
			} break;
		case THTTPEvent::EFailed:
			{
			// The transaction failed so fail the test
			iEngine->Utils().LogIt(_L("<Transaction failed>"));
			aTransaction.Close();
			if (--iOpenTransactionCount == 0)
				{
				CActiveScheduler::Stop();
				}
			// If the failure error has already been set, keep it.  Otherwise
			// set it to KErrCookieTestFailed
			if(iFailureError == KErrNone)
				{
				iFailureError = KErrCookieTestFailed;
				}
			} break;
		default:
			{
			_LIT(KLogDescription, "<unrecognised event> %d");
			iEngine->Utils().LogIt(KLogDescription,aEvent.iStatus);
			if (aEvent.iStatus < 0)
				{
				iFailureError = aEvent.iStatus;
				CActiveScheduler::Stop();
				}
			} 
			break;
		}
	}

/**
	This function is called to create a new transaction array.
	It may leave with any of the system wide error codes.
	@param	aUri	The URI for which a Transaction has to be created.
*/
void CTextModeTestCookies::CreateTransArrayL(const TDesC8& aUri)
	{
	// Open the transactions for this test
	TBuf8<KMaxUrlBufSize> url;
	RStringF get = iSession.StringPool().StringF(HTTP::EGET, RHTTPSession::GetTable());
	iOpenTransactionCount = 0;

	// create the transaction and submit
	url.Zero();
	url.Format(aUri);
	TUriParser8 urlParser;
	urlParser.Parse(url);
	RHTTPTransaction tr = iSession.OpenTransactionL(urlParser, *this, get);
	CleanupClosePushL(tr);
	User::LeaveIfError(iTransArray.Append(tr));
	CleanupStack::Pop(&tr);
	tr.SubmitL();
	iOpenTransactionCount++;
	}

void CTextModeTestCookies::ConfigureSessionFiltersL(TFilterConfigurationIterator* /*aFilterConfigIter*/)
	{
	if (iTestNumber == 11)
		{
		User::Leave(KErrNone);
		}

	iCookieManager = CExampleCookieManager::NewL(iSession.StringPool());

	CCookieFilterInterface::InstallFilterL(iSession, iCookieManager);
	}

void CTextModeTestCookies::ValidateBodyL(RHTTPTransaction aTransaction)
	{
	RHTTPResponse resp = aTransaction.Response();
	MHTTPDataSupplier* body = resp.Body();
	TPtrC8 data;
	body->GetNextDataPart(data);
	body->ReleaseData();
	}

/**
	This test gets two cookies and checks if the right cookies are sent.
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::DoCookieTest1L()
	{
	HBufC8* httpCookieSet1Url = TSrvAddrVal::ReplaceHostNameL(KHttpCookieSet1Url(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieSet1Url);

	HBufC8* httpCookieSet2Url = TSrvAddrVal::ReplaceHostNameL(KHttpCookieSet2Url(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieSet2Url);

	HBufC8* httpCookieCheck1Url = TSrvAddrVal::ReplaceHostNameL(KHttpCookieCheck1Url(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieCheck1Url);

	DoGenericCookieTestL(httpCookieSet1Url, httpCookieSet2Url, httpCookieCheck1Url);

	CleanupStack::PopAndDestroy(3, httpCookieSet1Url); // and httpCookieSet2Url, httpCookieCheck1Url
	}

/**
	This test tries to get a cookie by passing the wrong path in the Url.
	It may leave with any of the system wide error codes
*/
void CTextModeTestCookies::DoCookieTest2L()
	{
	HBufC8* httpCookieWrongPathUrl = TSrvAddrVal::ReplaceHostNameL(KHttpCookieWrongPathUrl(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieWrongPathUrl);

	DoGenericCookieTestL(httpCookieWrongPathUrl, NULL, NULL);

	CleanupStack::PopAndDestroy(httpCookieWrongPathUrl);
	}

/**
	This test gets two cookies by passing a good domain (domain that is 
	existing).
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::DoCookieTest3L()
	{
	HBufC8* httpCookieGoodDomainUrl = TSrvAddrVal::ReplaceHostNameL(KHttpCookieGoodDomainUrl(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieGoodDomainUrl);

	DoGenericCookieTestL(httpCookieGoodDomainUrl, httpCookieGoodDomainUrl, NULL);

	CleanupStack::PopAndDestroy(httpCookieGoodDomainUrl);
	}

/**
	This test gets a cookie by passing a bad domain (domain that is not
	existing).
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::DoCookieTest4L()
	{
	HBufC8* httpCookieBadDomainUrl = TSrvAddrVal::ReplaceHostNameL(KHttpCookieBadDomainUrl(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieBadDomainUrl);

	DoGenericCookieTestL(httpCookieBadDomainUrl, NULL, NULL);

	CleanupStack::PopAndDestroy(httpCookieBadDomainUrl);
	}

/**
	This test gets two cookies by passing a domain that has to be amended.
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::DoCookieTest5L()
	{
	HBufC8* httpCookieAmendDomainUrl = TSrvAddrVal::ReplaceHostNameL(KHttpCookieAmendDomainUrl(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieAmendDomainUrl);

	DoGenericCookieTestL(httpCookieAmendDomainUrl, httpCookieAmendDomainUrl, NULL);

	CleanupStack::PopAndDestroy(httpCookieAmendDomainUrl);
	}

/**
	This test gets a cookie that does not expire.
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::DoCookieTest6L()
	{
	HBufC8* httpCookieNoExpiryUrl = TSrvAddrVal::ReplaceHostNameL(KHttpCookieNoExpiryUrl(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieNoExpiryUrl);

	DoGenericCookieTestL(httpCookieNoExpiryUrl, NULL, NULL);

	CleanupStack::PopAndDestroy(httpCookieNoExpiryUrl);
	}

/**
	This test gets two cookies and checks if the right cookies are sent.
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::DoCookieTest7L()
	{
	HBufC8* httpCookieSet2Url = TSrvAddrVal::ReplaceHostNameL(KHttpCookieSet2Url(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieSet2Url);

	HBufC8* httpCookieSet3Url = TSrvAddrVal::ReplaceHostNameL(KHttpCookieSet3Url(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieSet3Url);

	HBufC8* httpCookieCheck2Url = TSrvAddrVal::ReplaceHostNameL(KHttpCookieCheck2Url(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieCheck2Url);

	DoGenericCookieTestL(httpCookieSet2Url, httpCookieSet3Url, httpCookieCheck2Url);

	CleanupStack::PopAndDestroy(3, httpCookieSet2Url); // and httpCookieSet3Url, httpCookieCheck2Url
	}

/**
	This test gets two cookies.
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::DoCookieTest8L()
	{
	HBufC8* httpCookie2Set1Url = TSrvAddrVal::ReplaceHostNameL(KHttpCookie2Set1Url(), iIniSettingsFile);
	CleanupStack::PushL(httpCookie2Set1Url);

	DoGenericCookieTestL(httpCookie2Set1Url, httpCookie2Set1Url, NULL);

	CleanupStack::PopAndDestroy(httpCookie2Set1Url);
	}

/**
	This test gets a cookie while the domain name is passed with port number
	and checks if the right cookie is sent.
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::DoCookieTest9L()
	{
	HBufC8* httpCookie2PortSetUrl = TSrvAddrVal::ReplaceHostNameL(KHttpCookie2PortSetUrl(), iIniSettingsFile);
	CleanupStack::PushL(httpCookie2PortSetUrl);

	DoGenericCookieTestL(httpCookie2PortSetUrl, NULL, httpCookie2PortSetUrl);

	CleanupStack::PopAndDestroy(httpCookie2PortSetUrl);
	}

/**
	This test gets a cookie while the domain name is passed with bad port 
	number.
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::DoCookieTest10L()
	{
	HBufC8* httpCookie2BadPortSetUrl = TSrvAddrVal::ReplaceHostNameL(KHttpCookie2BadPortSetUrl(), iIniSettingsFile);
	CleanupStack::PushL(httpCookie2BadPortSetUrl);

	DoGenericCookieTestL(httpCookie2BadPortSetUrl, NULL, NULL);

	CleanupStack::PopAndDestroy(httpCookie2BadPortSetUrl);
	}

/**
	This test gets a cookie twice.
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::DoCookieTest12L()
	{
	HBufC8* httpCookieSetTwiceUrl = TSrvAddrVal::ReplaceHostNameL(KHttpCookieSetTwiceUrl(), iIniSettingsFile);
	CleanupStack::PushL(httpCookieSetTwiceUrl);

	DoGenericCookieTestL(httpCookieSetTwiceUrl, NULL, NULL);

	CleanupStack::PopAndDestroy(httpCookieSetTwiceUrl);
	}

/**
	This function is called by all the cookie tests.  This function is called
	3 times as it processes once for each parameter.  i.e., First if executes
	for aCookie1, and comes back to this function and executes for aCookie2 and
	then comes back to execute for aCookie3.
	It may leave with any of the system wide error codes.

	@param	aCookie1	The URI to get a cookie
	@param	aCookie2	The URI to get a cookie
	@param	aCookie3	The URI to check if we sent the correct cookies.
*/
void CTextModeTestCookies::DoGenericCookieTestL(const TDesC8* aCookie1, const TDesC8* aCookie2, const TDesC8* aCookie3)
	{
	if(aCookie1)
		{
		// Go to the first URI where we will get a cookie
		CreateTransArrayL(*aCookie1);
		iFailureError = KErrNone;
		CActiveScheduler::Start();
		}

	if(aCookie2)
		{
		// Go to the second URI where we will get a cookie
		CreateTransArrayL(*aCookie2);
		iFailureError = KErrNone;
		CActiveScheduler::Start();
		}

	if(aCookie3)
		{
		// Go to the third URI where we check that we sent the correct cookies
		CreateTransArrayL(*aCookie3);
		iFailureError = KErrNone;
		CActiveScheduler::Start();
		}
	}

/**
	This function is called to validate the request.

	@leave	Leaves with a standard error
*/
void CTextModeTestCookies::ValidateRequestL()
	{
	const TInt transCount = iTransArray.Count();
	RHTTPTransaction trans = iTransArray[transCount - 1];
	RHTTPRequest request = trans.Request();
	RHTTPHeaders headers = request.GetHeaderCollection();

	CheckForCookiesL(headers, iTestNumber, transCount);
	}

/**
	This function is called to validate the response.
	It may leave with any of the system wide error codes.
*/
void CTextModeTestCookies::ValidateResponseL()
	{
	const TInt transCount = iTransArray.Count();
	RHTTPTransaction trans = iTransArray[transCount - 1];
	RHTTPResponse response = trans.Response();
	RHTTPHeaders headers = response.GetHeaderCollection();

	CheckForSetCookiesL(headers, iTestNumber, transCount);
	}
/**
	This function is called to check for the exisiting cookies.
		
	@leave KErrNotFound, if the set cookies are not found
*/

void CTextModeTestCookies::CheckForSetCookiesL(RHTTPHeaders aHeaders, TInt /*aTestNumber*/, TInt /*aTransNumber*/)
	{
	RStringF cookieHeaderFieldName = iSession.StringPool().StringF(HTTP::ESetCookie, iSession.GetTable());

	const TInt numCookies = aHeaders.FieldPartsL(cookieHeaderFieldName);

	if (numCookies!=2)
		{
		User::Leave(KErrNotFound);
		}
	}

/**
	This function is called to check for the exisiting cookies.
	
	@leave KErrNotFound, if the cookies being checked for are not found
*/
void CTextModeTestCookies::CheckForCookiesL(RHTTPHeaders aHeaders, TInt aTestNumber, TInt aTransNumber)
	{
	RStringF cookieHeaderFieldName = iSession.StringPool().StringF(HTTP::ECookie, iSession.GetTable());

	const TInt numCookies = aHeaders.FieldPartsL(cookieHeaderFieldName);

	RStringF stringToMatch = StringToMatchL(aTestNumber, aTransNumber, 0);
	const TDesC8& matchDes = stringToMatch.DesC();
	CleanupClosePushL(stringToMatch);

	TBool found = EFalse;
	if(stringToMatch != RStringF())
		{
		THTTPHdrVal hdr;
		for(TInt index=0; (index<numCookies) && !found; ++index)
			{
			User::LeaveIfError(aHeaders.GetField(cookieHeaderFieldName, 0, hdr));
			const TDesC8& hdrDes = hdr.StrF().DesC();

			if(hdrDes.Find(matchDes) != KErrNotFound)
				{
				found = ETrue;
				}

			}
		if(!found)
			{
			User::Leave(KErrNotFound);
			}
		}

	CleanupStack::PopAndDestroy(&stringToMatch);
	}

/**
	This function is called to get the strings that is to be matched for each test case

	@leave	KErrCookieTestFailed, if the cookie count is more than what is expected.
*/

RStringF CTextModeTestCookies::StringToMatchL(TInt aTestNumber, TInt aTransNumber, TInt aCookieCount)
	{
	// To add to this function:
	// Add to the switch statement for the test you are writing and return a 
	// string to search for in the cookie header (a good choice is the name of
	// the cookie you expect to find).  If you do not expect to find a cookie 
	// then make that check here and leave if you want to fail the test 
	// (return RStringF() if you want to pass.
	// 

	_LIT8(KTest1Trans1String, "TEST_PARTA");
	_LIT8(KTest1Trans2String, "TEST_PARTB");
	_LIT8(KTest3String, "TEST3");
	_LIT8(KTest5String, "TEST5");
	_LIT8(KTest6String, "TEST6");
	_LIT8(KTest7Trans1String, "TEST_PARTB");
	_LIT8(KTest7Trans2String, "TEST_PARTC");
	_LIT8(KTest8String, "feroz");
	_LIT8(KTest9String, "scooby");
	_LIT8(KTest10String, "FOX");
	_LIT8(KTest11String, "BADGER");

	RStringF returnString;

	switch(aTestNumber)
		{
		case 1:
			if(aTransNumber == 2)
				{
				returnString = iSession.StringPool().OpenFStringL(KTest1Trans1String());
				}
			else if(aTransNumber == 3)
				{
				returnString = iSession.StringPool().OpenFStringL(KTest1Trans2String());
				}
			break;
		case 2:
			if(iCookieManager->CookieCount() != 0)
				{
				iFailureError = KErrCookieTestFailed;
				User::Leave(KErrCookieTestFailed);
				}
			break;
		case 3:
			if(aTransNumber == 2)
				{
				returnString = iSession.StringPool().OpenFStringL(KTest3String());
				}
			break;
		case 4:
			if(iCookieManager->CookieCount() != 0)
				{
				iFailureError = KErrCookieTestFailed;
				User::Leave(KErrCookieTestFailed);
				}
			break;
		case 5:
			if(aTransNumber == 2)
				{
				returnString = iSession.StringPool().OpenFStringL(KTest5String());
				}
			break;
		case 6:
			if(aTransNumber == 2)
				{
				returnString = iSession.StringPool().OpenFStringL(KTest6String());
				}
			break;
		case 7:
			if(aTransNumber == 2)
				{
				returnString = iSession.StringPool().OpenFStringL(KTest7Trans1String());
				}
			else if(aTransNumber == 3)
				{
				returnString = iSession.StringPool().OpenFStringL(KTest7Trans2String());
				}
			break;
		case 8:
			if(aTransNumber == 2)
				{
				returnString = iSession.StringPool().OpenFStringL(KTest8String());
				}
			break;
		case 9:
			if(aTransNumber == 3)
				{
				returnString = iSession.StringPool().OpenFStringL(KTest9String());
				}
			break;
		case 10:
			if(iCookieManager->CookieCount() != 0)
				{
				iFailureError = KErrCookieTestFailed;
				User::Leave(KErrCookieTestFailed);
				}
			break;
		case 11:
			break;
		case 12:
			if (aCookieCount == 0)
				{
				returnString = iSession.StringPool().OpenFStringL(KTest10String());
				}
			else
				{
				returnString = iSession.StringPool().OpenFStringL(KTest11String());
				}
			break;
		default:
			break;
		}

	return returnString;
	}
