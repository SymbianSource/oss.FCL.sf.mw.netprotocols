/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Iop filter
*
*/



// INCLUDE FILES
#include <http.h>
#include <es_sock.h>

// User Includes
#include "HttpFilterIop.h"
#include "httpfiltercommonstringsext.h"

#include "MIopRedirectCallback.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES
void PanicHttpFiltersIop(TInt aErr = 0);

// CONSTANTS
const TInt KIopOrderOffset = 10;

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHttpFilterIop::CHttpFilterIop
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CHttpFilterIop::CHttpFilterIop(RHTTPSession* aSession, MIopRedirectCallback* aIopRedirectCallback, TUint aIopCapabilities)
{
    __ASSERT_DEBUG(aSession != NULL, PanicHttpFiltersIop());
    iSession = aSession;
	iIopCapabilities = aIopCapabilities;
	iIopRedirectCallback = aIopRedirectCallback; //can be null for backward compatibility or iopOptionRedirectAction not used
	iLoadCount = 0;
}


// ------------------------------------------------------------------------------------------
// CHttpFilterIop::ConstructL
// Symbian 2nd phase constructor can leave.
// ------------------------------------------------------------------------------------------
//
void CHttpFilterIop::ConstructL()
{
	if (iIopCapabilities & iopOptionHostHeader)
		{
		// Register the filter for submit events,
		// Adds a filter to the session's filter queue.

		iSession->FilterCollection().AddFilterL(*this,   // The filter to add
												THTTPEvent::ESubmit,       // The event that triggers this filter
												RStringF(),                // The header whose presence triggers this filter, or KNullDesC to trigger on any header
												KAnyStatusCode,            // The status code that triggers this filter, or KAnyStatusCode to trigger on any status code
												EStatusCodeHandler - KIopOrderOffset,    // The position of the filter in the queue
												iSession->StringPool().StringF(HttpFilterCommonStringsExt::EIopFilter,
												HttpFilterCommonStringsExt::GetTable())); //The name of the filter to add
		}

	if ( (iIopCapabilities & iopOptionRedirectAction) && iIopRedirectCallback)
		{
		// 301 Found
		iSession->FilterCollection().AddFilterL(*this, // The filter to add
									THTTPEvent::EGotResponseHeaders, // The event that triggers this filter
									RStringF(),						 // The header whose presence triggers this filter, or KNullDesC to trigger on any header
									301,							// The status code that triggers this filter, or KAnyStatusCode to trigger on any status code
									EStatusCodeHandler-10,			// The position of the filter in the queue
									iSession->StringPool().StringF(HttpFilterCommonStringsExt::EIopFilter,
									HttpFilterCommonStringsExt::GetTable())); //The name of the filter to add
		// 302 Found
		iSession->FilterCollection().AddFilterL(*this, // The filter to add
									THTTPEvent::EGotResponseHeaders, // The event that triggers this filter
									RStringF(),						 // The header whose presence triggers this filter, or KNullDesC to trigger on any header
									302,							// The status code that triggers this filter, or KAnyStatusCode to trigger on any status code
									EStatusCodeHandler-10,			// The position of the filter in the queue
									iSession->StringPool().StringF(HttpFilterCommonStringsExt::EIopFilter,
									HttpFilterCommonStringsExt::GetTable())); //The name of the filter to add

		// 303 See other
		iSession->FilterCollection().AddFilterL(*this, // The filter to add
									THTTPEvent::EGotResponseHeaders, // The event that triggers this filter
									RStringF(),						 // The header whose presence triggers this filter, or KNullDesC to trigger on any header
									303,							// The status code that triggers this filter, or KAnyStatusCode to trigger on any status code
									EStatusCodeHandler-10,			// The position of the filter in the queue
									iSession->StringPool().StringF(HttpFilterCommonStringsExt::EIopFilter,
									HttpFilterCommonStringsExt::GetTable())); //The name of the filter to add
		// 307 See other
		iSession->FilterCollection().AddFilterL(*this, // The filter to add
									THTTPEvent::EGotResponseHeaders, // The event that triggers this filter
									RStringF(),						 // The header whose presence triggers this filter, or KNullDesC to trigger on any header
									307,							// The status code that triggers this filter, or KAnyStatusCode to trigger on any status code
									EStatusCodeHandler-10,			// The position of the filter in the queue
									iSession->StringPool().StringF(HttpFilterCommonStringsExt::EIopFilter,
									HttpFilterCommonStringsExt::GetTable())); //The name of the filter to add

		}

}

//---------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------
//
CHttpFilterIop::~CHttpFilterIop()
{
    // If we've been destroyed from the cleanup stack during creation
    // of the object, it might still be loaded. So check. (Normaly the
    // delete is initiated by the 'delete this' in MHFUnload)
    if (iLoadCount)
        {
        // As we're already in a destructor, MHFUnload must not delete us again
        iLoadCount = -1;
        if( NULL != iSession)  
  			{  
     		iSession->FilterCollection().RemoveFilter(iSession->StringPool().StringF( HttpFilterCommonStringsExt::EIopFilter, HttpFilterCommonStringsExt::GetTable()));  
  			}  

        }
}

// ---------------------------------------------------------------------------------------------
// CHttpFilterIop::InstallFilterL
// Two-phased constructor. This function replaces NewL.
// ---------------------------------------------------------------------------------------------
//
CHttpFilterIop* CHttpFilterIop::InstallFilterL(TAny* aParams)
{
    __ASSERT_DEBUG(aParams != NULL, PanicHttpFiltersIop());
    TIopParams* iopParams = REINTERPRET_CAST(TIopParams*, aParams);
    __ASSERT_DEBUG(iopParams->iSession != NULL, PanicHttpFiltersIop());

    CHttpFilterIop* filter = new (ELeave) CHttpFilterIop( iopParams->iSession, iopParams->iIopRedirectCallback, iopParams->iIopCapabilities );
    CleanupStack::PushL(filter);
    filter->ConstructL();
    CleanupStack::Pop(filter);
    return filter;
}

// ---------------------------------------------------------------------------------------------
// CHttpFilterIop::MHFLoad
// Called when the filter is being added to the session's filter queue.
// ---------------------------------------------------------------------------------------------
//
void CHttpFilterIop::MHFLoad(RHTTPSession, THTTPFilterHandle)
{
    ++iLoadCount;
}

// ----------------------------------------------------------------------------------------------
// CHttpFilterIop::MHFUnload
// Called when the filter is being removed from a session's filter queue.
// ----------------------------------------------------------------------------------------------
//
void CHttpFilterIop::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle /*aFilterHandler*/)
{
    __ASSERT_DEBUG(iLoadCount >= 0, PanicHttpFiltersIop());
    if (--iLoadCount)
    {
        return;
    }
    delete this;
}

// ------------------------------------------------------------------------------------------------
// CHttpFilterIop::MHFRunL
// Process a transaction event.
// ------------------------------------------------------------------------------------------------
//
void CHttpFilterIop::MHFRunL(RHTTPTransaction aTransaction,
                             const THTTPEvent& aEvent)
{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::ESubmit:
			{
			_LIT8(KPort80, "80");
			TPtrC8 port80Ptr = KPort80();

			RHTTPHeaders headers(aTransaction.Request().GetHeaderCollection());
			if (iIopCapabilities & iopOptionHostHeader)
				{
				TUriC8 uri = aTransaction.Request().URI();
				TPtrC8 hostPtr = uri.Extract(EUriHost);
				TPtrC8 portPtr = uri.Extract(EUriPort);
				if (portPtr.Length() > 0 && portPtr.Compare(port80Ptr))
					{
					hostPtr.Set(hostPtr.Ptr(), hostPtr.Length() + 1 + portPtr.Length());
					}

				headers.RemoveField(iSession->StringPool().StringF(HTTP::EHost, RHTTPSession::GetTable()));
				if (hostPtr.Length() > 0)
					{
					RStringF hostString = iSession->StringPool().OpenFStringL(hostPtr);
					CleanupClosePushL(hostString);
					headers.SetFieldL(iSession->StringPool().StringF(HTTP::EHost, RHTTPSession::GetTable()),
											 THTTPHdrVal(hostString));
					CleanupStack::PopAndDestroy(); // hostString
					}
				}
			break;
			} // case THTTPEvent::ESubmit:
		case THTTPEvent::EGotResponseHeaders:
			{
			switch (aTransaction.Response().StatusCode())
				{
				case 301:
				case 302:
				case 303:
				case 307:
					{
					if (iIopRedirectCallback) // can not be null
						HandleRedirectL(aTransaction);
					break;
					}
				default:
					break;
				} //  switch (aTransaction.Response().StatusCode())
			} // case THTTPEvent::EGotResponseHeaders:
		default:
			break;
		} // switch (aEvent.iStatus)
}

// -----------------------------------------------------------------------------
// CHttpFilterIop::MHFRunError
// Process an error that occured while processing the transaction.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterIop::MHFRunError(TInt ,
                                 RHTTPTransaction ,
                                 const THTTPEvent& )
{
//    aTransaction.Fail();
    return KErrNone;
}

// -----------------------------------------------------------------------------
// CHttpFilterIop::MHFSessionRunL
// Process a session event.
// -----------------------------------------------------------------------------
//
void CHttpFilterIop::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
{
}

// -----------------------------------------------------------------------------
// CHttpFilterIop::MHFSessionRunError
// Called when MHFRunL leaves from a session event.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterIop::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
{
    return aError;
}

// -----------------------------------------------------------------------------
// CHttpFilterIop::HandleRedirectL
// Called when received redirection status code 
// -----------------------------------------------------------------------------
//
void CHttpFilterIop::HandleRedirectL(RHTTPTransaction aTransaction)
    {

	RStringPool strPool = iSession->StringPool();
    RStringF location = strPool.StringF(HTTP::ELocation,RHTTPSession::GetTable());
    RHTTPHeaders responseHeaders(aTransaction.Response().GetHeaderCollection());
    THTTPHdrVal locationValue;

    TInt err = responseHeaders.GetField(location, 0, locationValue);
    if (err == KErrNotFound) // location not present when redirected
        return; // Redirect filter will deal with it, we care only about non http, https and relative(assumes http(s) ) schemes
    else
        {
		User::LeaveIfError(err);
        TUriParser8 uri;
        uri.Parse(locationValue.StrF().DesC());
		if (uri.IsPresent(EUriHost) && uri.IsPresent(EUriScheme)) // looking only for absolue Url path and schemes other than http(s)
			{
				const TDesC8& scheme = uri.Extract(EUriScheme);
				if (scheme.FindF(_L8("http")) == KErrNotFound) // everything but http(s)
				{
					TPtrC8 ptr(uri.UriDes());
					iIopRedirectCallback->NonHttpRedirect(ptr, aTransaction);
					aTransaction.Cancel();
				}
			}
		}
    }

//  End of File

