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

#include "CHttpTestChunkedBase.h"
#include <http.h>
#include "csrvaddrval.h"

void CHttpTestChunkedBase::OpenTestSessionL()
	{
	//open  a Session
	
	iSession.OpenL();
	iEngine->Utils().LogIt(_L("Session Created(Iteration 3 Post xx)"));
	iEngine->Utils().LogIt(_L("Session parameters: Default"));

	InstallAuthenticationL(iSession);

	//Get the iSession'string pool handle;
	iMyStrP = iSession.StringPool();
	//open string this testcase needs

	// Literals used in the function
	_LIT8(KWapTestNameUrl, "http://scooby:doo@WapTestName/upload/testchunked");
	// Replace the host name in the URL
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KWapTestNameUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	TUriParser8 testURI; 
	testURI.Parse(newUrlPtr8);
	
	iTransaction = iSession.OpenTransactionL(testURI, *this, iMyStrP.StringF(HTTP::EPUT,RHTTPSession::GetTable()));
	iOpenTransactionCount++;
	iEngine->Utils().LogIt(_L("Transaction Created in Session"));

	//Get a handle of the request in iTransaction
	RHTTPRequest myRequest = iTransaction.Request();
	RHTTPHeaders myHeaders = myRequest.GetHeaderCollection();
	//provide  some headers 

	THTTPHdrVal contentTypeHdr(iMyStrP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable()));
	myHeaders.SetFieldL(iMyStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable()),contentTypeHdr);
	
	TSrvAddrVal::LogUsing8BitDesL(iEngine, newUrlPtr8);
	iEngine->Utils().LogIt(_L("Method:Post"));
	iEngine->Utils().LogIt(_L("Content-Type:application/x-www-form-urlencoded "));
	CleanupStack::PopAndDestroy(newUrl8);
	myRequest.SetBody(*this);
	} 

void CHttpTestChunkedBase::CloseTestSession()
	{
	// Close strings used in this session before closing the session
	//close Transaction and session
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}

TBool CHttpTestChunkedBase::GetCredentialsL(const TUriC8& /*aURI*/, RString /*aRealm*/, RStringF /*aAuthenticationType*/, RString& /*aUsername*/,RString& /*aPassword*/)
	{
	return 0;
	}

void CHttpTestChunkedBase::ReleaseData() 
	{	
	++iReqBodyChunkCount;
	// As we only have 3 chunks do not tell the transaction that there is more 
	// data after we have released the last chunk!
	if(!iLastChunk)
		{
		TRAPD(err, iTransaction.NotifyNewRequestBodyPartL());
		if(err!=KErrNone)
			iTransaction.Fail();
		}
	}

TBool CHttpTestChunkedBase::GetNextDataPart(TPtrC8& aDataChunk)
	{
	_LIT8(KHttpPostBodyChunk1,"ABC");
	_LIT8(KHttpPostBodyChunk2,"DEF");
	_LIT8(KHttpPostBodyChunk3,"GHJ");

	TBool retVal = EFalse;
	switch (iReqBodyChunkCount)
		{
	case 0:
		{
		aDataChunk.Set(KHttpPostBodyChunk1());
		} break;
	case 1:
		{
		aDataChunk.Set(KHttpPostBodyChunk2());
		} break;
	case 2:
		{
		aDataChunk.Set(KHttpPostBodyChunk3());
		iLastChunk = ETrue;
		retVal = ETrue;
		} break;
		}
	return retVal;
	}
