// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <es_enum.h>
#include "testexplicitsessionstep.h"

_LIT8(KProxy, "10.29.23.12:9003");

CTestExplicitSessionStep::CTestExplicitSessionStep()
	{
	SetTestStepName(KTestExplicitSessionStep); 
	}


CTestExplicitSessionStep::~CTestExplicitSessionStep()
	{
	}
	

void CTestExplicitSessionStep::StartClientL()
	{
	TPtrC uri;
	TInt iap = 0;
	
    if (!GetStringFromConfig(ConfigSection(), KIniUri, uri) ||
    	!GetIntFromConfig(ConfigSection(), KIAP, iap)
    	)
		{
		ERR_PRINTF3(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%d"
					  ),&KIniUri, iap);
		SetTestStepResult(EFail);
		return;
		}

	TBuf8<256> url8;
	url8.Copy(uri);
	TUriParser8 uriP; 
	uriP.Parse( url8 );

	iExplicitSession.OpenL(uriP, iap);
	iSession = iExplicitSession.HTTPSession();
	RStringPool strP = iSession.StringPool();
	
/*
	THTTPHdrVal val;
	RHTTPConnectionInfo cF = iSession.ConnectionInfo();
	cF.Property(strP.StringF(HTTP::EHttpSocketConnection, RHTTPSession::GetTable()), val);
	RConnection* conn = reinterpret_cast<RConnection*>(val.Int());
	TConnectionInfoBuf buf;
	conn->GetConnectionInfo(1, buf);
	TInt iapId = buf().iIapId;	
*/	
	
	RStringF method;
	iSession.ConnectionInfo().SetPropertyL(iSess.StringPool().StringF(HTTP::EProxyUsage,RHTTPSession::GetTable()), iSess.StringPool().StringF(HTTP::EUseProxy,RHTTPSession::GetTable()));
	RStringF proxy = iSession.StringPool().OpenFStringL(KProxy());
	CleanupClosePushL(proxy);
	iSession.ConnectionInfo().SetPropertyL(iSession.StringPool().StringF(HTTP::EProxyAddress,RHTTPSession::GetTable()), proxy);
	CleanupStack::PopAndDestroy(&proxy);

	
	method = strP.StringF(HTTP::EGET,RHTTPSession::GetTable());		
	TRAPD(err,InvokeHttpMethodL(url8, method));
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Teststep Failed: Leave occured in CTestExplicitSessionStep::InvokeHttpMethodL: %D\n"
					  ),err
				   );
		SetTestStepResult(EFail);
		}
	method.Close(); 
	iExplicitSession.CancelStart();
	RHTTPSession ss = iExplicitSession();
	iExplicitSession.Close();

	iExplicitSession.OpenL(uriP, iap, EFalse);
	TRequestStatus iStatus = KRequestPending;
	iExplicitSession.StartConnection(iStatus);
	iExplicitSession.Close();
	
	iExplicitSession.OpenL(uriP, iap);
	iExplicitSession.Close();
	
	TRAP(err, iExplicitSession.OpenL(uriP, KConnectionTypeDefault));
	iStatus = KRequestPending;
	iExplicitSession.StartConnection(iStatus);
	iExplicitSession.Close();
	}


void CTestExplicitSessionStep::InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod)
	{
	TUriParser8 uri; 
	uri.Parse( aUri );
	
	iTrans = iSession.OpenTransactionL(uri, *this, aMethod);
	RHTTPHeaders hdr = iTrans.Request().GetHeaderCollection();

	iTrans.SubmitL();
	CActiveScheduler::Start();
	}


void CTestExplicitSessionStep::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			RHTTPResponse resp = aTransaction.Response();
			TInt status = resp.StatusCode();
			RStringF statusStr = resp.StatusText();
			TBuf<32> statusStr16;
			statusStr16.Copy(statusStr.DesC());
			INFO_PRINTF3(_L("Status: %D (%S)\n"),status, &statusStr16);

			if (resp.HasBody() && (status >= 200) && (status < 300) && (status != 204))
				{
				TInt dataSize = resp.Body()->OverallDataSize();
				if (dataSize >= 0)
					INFO_PRINTF2(_L("Response body size is %d\n"), dataSize);
				else
					INFO_PRINTF1(_L("Response body size is unknown\n"));
				}
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			MHTTPDataSupplier* respBody;
			respBody = aTransaction.Response().Body();
			INFO_PRINTF1(_L("Receiving response body...\n"));
			
			TPtrC8 bodyData;
			respBody->GetNextDataPart(bodyData);
			respBody->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			INFO_PRINTF1(_L("\nTransaction Complete\n"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			INFO_PRINTF1(_L("Transaction Successful\n"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::EFailed:
			{
			INFO_PRINTF1(_L("Transaction Failed\n"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::ERedirectedPermanently:
			{
			INFO_PRINTF1(_L("Permanent Redirection\n"));
			} break;
		case THTTPEvent::ERedirectedTemporarily:
			{
			INFO_PRINTF1(_L("Temporary Redirection\n"));
			} break;
		case THTTPEvent::ERedirectRequiresConfirmation:
 			{
			INFO_PRINTF1(_L("Redirect requires confirmation\n"));
 			aTransaction.Close();
 			CActiveScheduler::Stop();
 			} break;
		default:
			{
			INFO_PRINTF2(_L("<unrecognised event: %D>\n"), aEvent.iStatus);
			if (aEvent.iStatus < 0)
				{
				aTransaction.Close();
				CActiveScheduler::Stop();
				}
			} break;
		}
	}


TInt CTestExplicitSessionStep::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	INFO_PRINTF2(_L("MHFRunError fired with error code %D\n"), aError);
	return KErrNone;
	}
