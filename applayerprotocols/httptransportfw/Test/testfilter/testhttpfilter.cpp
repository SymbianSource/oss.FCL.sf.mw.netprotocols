// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <http/rhttpresponse.h>
#include <httperr.h>
#include <httpstringconstants.h>
#include <bautils.h>
#include <e32math.h>
#include <hash.h>
#include <e32const.h>
#include <tconvbase64.h>  //for base64 en/decoding
#include "testhttpfilter.h"
_LIT8( KTESTHTTPFilterName, "TestHttp");
_LIT8( KAuthenticationInfoStr, "Authentication-Info" );
_LIT8( KUserAgentProductToken,"3gpp-gba");
const TInt KB64KeySize = 64;
_LIT(KTestHttpFilter, "TEST HTTP FILTER ");

void PanicTestHttpFilters(TInt aErr = 0)
{
    User::Panic(KTestHttpFilter, aErr);
}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestFilter::CTestFilter
// Constructor
// -----------------------------------------------------------------------------
//
CTestFilter::CTestFilter()
		: iBootstrapCount(0),iHaveCredentials(EFalse)
{

}

// -----------------------------------------------------------------------------
// CTestFilter::InstallFilterL
// Initialize the filter and register it to sesssion's filter collection
// -----------------------------------------------------------------------------
//
CTestFilterInterface* CTestFilter::InstallFilterL(TAny* aParams)
{
	__ASSERT_DEBUG(aParams != NULL, PanicTestHttpFilters());
	RHTTPSession* session = REINTERPRET_CAST(RHTTPSession*, aParams);
	CTestFilter* filter = new (ELeave) CTestFilter();
	CleanupStack::PushL(filter);
	filter->ConstructL(*session);
	CleanupStack::Pop(filter);
	return filter;
}
	
// -----------------------------------------------------------------------------
// CTestFilter::ConstructL
// Memory and resource allocation, leaves
// -----------------------------------------------------------------------------
//
void CTestFilter::ConstructL(RHTTPSession aSession)
{
	iStringPool = aSession.StringPool();
	// register the filter
	RStringF filterName = iStringPool.OpenFStringL( KTESTHTTPFilterName );
	CleanupClosePushL( filterName );
	iStringPool = aSession.StringPool();

	iUsernameStr = iStringPool.StringF( HTTP::EUsername, RHTTPSession::GetTable() );
    iPasswordStr = iStringPool.StringF( HTTP::EPassword, RHTTPSession::GetTable() );
	iUserAgent = iStringPool.OpenFStringL( KUserAgentProductToken );
    iAuthInfo = iStringPool.OpenFStringL( KAuthenticationInfoStr );
    
    //Regsiter for THTTPEvent::ESubmit http event
    aSession.FilterCollection().AddFilterL( *this,
                                            THTTPEvent::ESubmit,
                                            RStringF(),
                                            KAnyStatusCode,
                                            EStatusCodeHandler,
                                            filterName);
   
    //Regsiter for THTTPEvent::EGotResponseHeaders http event with "Authentication-Info" header 
    //and HTTPStatus::EOk http status code,just to know if the credentials established are valid.
    aSession.FilterCollection().AddFilterL( *this,
                                            THTTPEvent::EGotResponseHeaders,
                                            iAuthInfo,
                                            HTTPStatus::EOk,
                                            EStatusCodeHandler,
                                            filterName );

    //Regsiter for THTTPEvent::EGotResponseHeaders http event with "WWW-Authenticate" header 
    //and HTTPStatus::EUnauthorized http status code
    //The priority for this is set to MHTTPFilter::EStatusCodeHandler - 1,
    //so the filter intercepts the transaction before the default 
    //http digest authentication filter(which has priority MHTTPFilter::EStatusCodeHandler).

    aSession.FilterCollection().AddFilterL( *this, 
											THTTPEvent::EGotResponseHeaders,	// Any transaction event
											iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable()),
											HTTPStatus::EUnauthorized,
											MHTTPFilter::EStatusCodeHandler - 1,	  // Priority of filter
											filterName );						      // Name of filter

	CleanupStack::PopAndDestroy( &filterName );
  
    iTimeOut =  CTimerOut::NewL(*this);
    
	}


//------------------------------------------------------------------------
// CTestFilter::~CTestFilter
// Destructor
//------------------------------------------------------------------------
//
CTestFilter::~CTestFilter()
{
	CleanupAll();			
	delete iTimeOut;
}

//------------------------------------------------------------------------
// CTestFilter::MHFLoad
//------------------------------------------------------------------------
//
void CTestFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
{
	++iLoadCount;
}

//------------------------------------------------------------------------
// CTestFilter::MHFUnload
//------------------------------------------------------------------------
//
void CTestFilter::MHFUnload(RHTTPSession , THTTPFilterHandle)
{
	 __ASSERT_DEBUG( iLoadCount >= 0, PanicTestHttpFilters() );
    
	if (--iLoadCount)
	{
		return;
	}
	delete this;
}

//------------------------------------------------------------------------
// CTestFilter::MHFRunL
// See MHTTPFilterBase::MHFRunL 
//------------------------------------------------------------------------
//
void CTestFilter::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
{
	if (aEvent.iUID != KHTTPUid) 
		return;

	switch(aEvent.iStatus)
	{
		case THTTPEvent::ESubmit:
		{
			DoSubmitL(aTransaction);
		}
		break;

		case THTTPEvent::EGotResponseHeaders:
		{
			// Get HTTP status code from header (e.g. 200)
			RHTTPResponse resp = aTransaction.Response();
			CheckHeadersL( aTransaction );
		}
		break;
		default: 
		{
		}
		break;
	}
	
}


//------------------------------------------------------------------------
// CTestFilter::MHFRunError
// See MHTTPFilterBase::MHFRunError
//------------------------------------------------------------------------
//
TInt CTestFilter::MHFRunError(TInt /*aError*/, RHTTPTransaction aTransaction, const THTTPEvent& )
{
 	TInt error = 0;
	// map aError to global error message
	// pass the errorcode forward
	THTTPEvent httpEvent(error);
	TRAP_IGNORE(aTransaction.SendEventL(httpEvent, THTTPEvent::EIncoming, THTTPFilterHandle::ECurrentFilter ));
	return KErrNone;
}

//------------------------------------------------------------------------
// CTestFilter::MHFSessionRunL
// See MHTTPFilterBase::MHFSessionRunL
//------------------------------------------------------------------------
//
void CTestFilter::MHFSessionRunL(const THTTPSessionEvent& )
{
	// do nothing
}

//------------------------------------------------------------------------
// CTestFilter::MHFSessionRunL
// See MHTTPFilterBase::MHFSessionRunL
//------------------------------------------------------------------------
//
TInt CTestFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& )
{
	// session problem
	return aError;
}
//------------------------------------------------------------------------
// CTestFilter::CheckHeadersL
// Check HTTP headers and extract MIME type
//------------------------------------------------------------------------
// 
void CTestFilter::CheckHeadersL(  RHTTPTransaction& aTrans )
{
	// read the header data and check the MIME type here	
	// check the status and body
	RHTTPResponse response = aTrans.Response();
	TInt status = response.StatusCode();
	THTTPHdrVal fieldVal;
	// check if status == 401 and realm is 3GPP then we need to bootstrap, if we are already bootstrappign this is the second call
	if(  status == HTTPStatus::EUnauthorized )
	{				
	
	   TInt headerPart=0;    	
	   THTTPHdrVal headerVal;
       RStringF wwwAuthHeader = iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable());
	   RHTTPHeaders headers(aTrans.Response().GetHeaderCollection());

	    RString realm;
	    THTTPHdrVal hdrVal;
	    if (!headers.GetParam(wwwAuthHeader, iStringPool.StringF(HTTP::ERealm,RHTTPSession::GetTable()), 
				  hdrVal, headerPart))
	    {
			realm = hdrVal;
			const TDesC8& val = realm.DesC();
	   			if (headerPart == KErrNotFound)
					return;
	            THTTPHdrVal authTypeParam;


	 				RHTTPTransactionPropertySet propSet = aTrans.PropertySet();
	 				// if we are already bootstrapping results will be retrieved when request is resubmitted
	                iBootstrapPending = ETrue;
	                
             
	                TTimeIntervalMicroSeconds32 aInterval(20000000);
	                
	                iTimeOut->Start(aInterval); //Start the http post request timer, aInterval - timeout in micro-seconds
	                
	                if(!iBootstrapWait.IsStarted())
	                    {
                        iBootstrapWait.Start();
	                    }
	                  
	                if( iHaveCredentials )
	                    {
                        RHTTPTransactionPropertySet propSet = aTrans.PropertySet();
                       
                        //user name/pass word for accessing http://replab.nrln.net/digest/ 
                        //username: dummy, password: dummy
                        TBuf8<KB64KeySize> keyBase64(_L8("Aladdin")); 
                        TBuf8<KB64KeySize> keyBase64Username(_L8("open sesame"));;
                        RString username = iStringPool.OpenStringL( keyBase64 );
                        CleanupClosePushL<RString>( username );
                        RString password = iStringPool.OpenStringL( keyBase64Username );
                       CleanupClosePushL<RString>( password );
                        propSet.SetPropertyL( iUsernameStr, username );
                        propSet.SetPropertyL( iPasswordStr, password );
                        CleanupStack::PopAndDestroy(&password);
                        CleanupStack::PopAndDestroy(&username);
                        //Cancel the transaction
                        aTrans.Cancel();
                        // Re-submit the http request with much needed credentials
                        aTrans.SubmitL(); 
                        }
	                else
	                    {
                        //Since bootstrapping failed,Do not have to resubmit the http request ?
                        return;
                        }
			
		}
	}
}

void CTestFilter::DoSubmitL( RHTTPTransaction aTransaction )
{
	RHTTPHeaders hdr = aTransaction.Request().GetHeaderCollection();
	
	RStringF fieldname = aTransaction.Session().StringPool().StringF(HTTP::EUserAgent,RHTTPSession::GetTable());
	
	TBool found = EFalse;
	TPtrC8 rawFieldData;
	
	hdr.GetRawField(fieldname, rawFieldData);
	
	found = rawFieldData.Find(iUserAgent.DesC()) != KErrNotFound;
	
	if(!found)
	    hdr.SetFieldL(fieldname, iUserAgent);
	
    if( iHaveCredentials )
        {
        TBuf8<KB64KeySize> keyBase64(_L8("Aladdin")); 
        TBuf8<KB64KeySize> keyBase64Username(_L8("open sesame"));;
        RString username = iStringPool.OpenStringL( keyBase64 );
        CleanupClosePushL<RString>( username );
        RString password = iStringPool.OpenStringL( keyBase64Username );
        CleanupClosePushL<RString>( password );
		EncodeBasicAuthL(username, password, aTransaction);
        CleanupStack::PopAndDestroy(&password);
        CleanupStack::PopAndDestroy(&username);
        	
        }
}

//-----------------------------------------------------------------------------
// CTestFilter::CleanupAll
// Cleanup all the transactions, in case a session error happens or a session
// is closed.
//-----------------------------------------------------------------------------
//
void CTestFilter::CleanupAll()
{	
	
	iUserAgent.Close();
	iAuthInfo.Close();
}

TBool CTestFilter::HandleTimerEventL(TTimerEventType /*aEventType*/)
{
    iBootstrapPending = EFalse;
    
    iHaveCredentials = ETrue;
     
    if (iBootstrapWait.IsStarted())
    	iBootstrapWait.AsyncStop();
    
    return ETrue;
}


void CTestFilter::EncodeBasicAuthL(RString& aUsername, 
										  RString& aPW, 
										  RHTTPTransaction& aTransaction)
	{
	// Standard, plain-text HTTP - Base 64 the name and password
	TBase64 codec;
	HBufC8* nameAndPW = HBufC8::NewMaxLC(aUsername.DesC().Length() + aPW.DesC().Length() + 1);
	_LIT8(KBasicAuthFormatString, "%S:%S");
	nameAndPW->Des().Format(KBasicAuthFormatString, &aUsername.DesC(), &aPW.DesC());
 
	// Conservatively allocate a buffer twice as big as the unencoded
	// buffer for the encoded string.
	HBufC8* encoded = HBufC8::NewMaxLC(nameAndPW->Des().Length() * 2);
	TPtr8 encodedPtr(encoded->Des());
	codec.Encode(*nameAndPW, encodedPtr);
	
	RString encodedString = iStringPool.OpenStringL(*encoded);
	CleanupStack::PopAndDestroy(2, nameAndPW); 

	CleanupClosePushL(encodedString);
	
	RHTTPHeaders requestHeaders(aTransaction.Request().GetHeaderCollection());
	
	requestHeaders.RemoveField(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable())); 
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
									THTTPHdrVal(iStringPool.StringF(HTTP::EBasic,RHTTPSession::GetTable())));
	
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
									THTTPHdrVal(encodedString));

	aTransaction.PropertySet().SetPropertyL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
																THTTPHdrVal(encodedString));
	
	CleanupStack::PopAndDestroy(&encodedString);
	
	}
