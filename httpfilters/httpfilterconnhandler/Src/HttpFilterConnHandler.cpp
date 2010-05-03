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
* Description:  ConnHandler filter
*
*/



// INCLUDE FILES
#include <http.h>
#include <es_sock.h>

#include <ApEngineConsts.h>  // defines bearer types
#include "HttpFilterConnHandler.h"

#include "HttpFilterConnHandlerObserverPS.h"
#include <PSVariables.h>  // Publish & Subscribe

#include "httpfiltercommonstringsext.h"
#include "mconnectioncallback.h"

// uncomment for logging
//#include <flogger.h>
//_LIT(KDir, "connFilter");
//_LIT(KFile, "filtererr.txt");

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES
void PanicHttpFiltersConnHandler(TInt aErr = 0);

// CONSTANTS
const TInt KConnHandlerOrderOffset = 10;

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// LocalHostCheckL
// If the host is a local host. Returns ETrue
// if the transaction request URI was for a localhost.
// -----------------------------------------------------------------------------
//
TBool LocalHostCheckL(RHTTPTransaction& aTransaction)
    {
    _LIT8(KLoopbackIPv4Url, "http://127.0.0.1"); 
    
    RHTTPRequest request = aTransaction.Request();
    TUriC8 uri = request.URI();

    TUriParser8 parserLoopBack;
    parserLoopBack.Parse(KLoopbackIPv4Url());
    TInt match = parserLoopBack.Compare(uri, EUriHost);

    if (KErrNone != match)
        {
        _LIT8(KLocalHostUrl, "http://localhost"); 

        // try another compare - compare against the "localhost".
        TUriParser8 parserLocalHost;
        parserLocalHost.Parse(KLocalHostUrl());
        match = parserLocalHost.Compare(uri, EUriHost);

        if (KErrNone == match) 
            {
            _LIT8(KLoopbackIPv4, "127.0.0.1"); 

            // "localhost" resolves to "::1", manually, replace with "127.0.0.1"
            CUri8* newUri = CUri8::NewLC(uri);
            newUri->SetComponentL(KLoopbackIPv4(), EUriHost);
            request.SetURIL(newUri->Uri());
            CleanupStack::PopAndDestroy(newUri);
            }
        }
              
    if (KErrNone == match)
        {
        return ETrue;                                                          
        }
    return EFalse;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHttpFilterConnHandler::CHttpFilterConnHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CHttpFilterConnHandler::CHttpFilterConnHandler(RHTTPSession* aSession, MConnectionCallback* aConnCallback):
        iConnCallback (aConnCallback)
{
    __ASSERT_DEBUG(aConnCallback != NULL, PanicHttpFiltersConnHandler());
    __ASSERT_DEBUG(aSession != NULL, PanicHttpFiltersConnHandler());
    iSession      = aSession;
}


// ------------------------------------------------------------------------------------------
// CHttpFilterConnHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// ------------------------------------------------------------------------------------------
//
void CHttpFilterConnHandler::ConstructL()
{
    // Register the filter for THTTPSessionEvent::EConnect events,
    // Adds a filter to the session's filter queue.
    iSession->FilterCollection().AddFilterL(*this,   // The filter to add
                                            THTTPSessionEvent::EConnect,       // The event that triggers this filter
                                            RStringF(),   // The header whose presence triggers this filter, or KNullDesC to trigger on any header
                                            KAnyStatusCode,  // The status code that triggers this filter, or KAnyStatusCode to trigger on any status code
                                            ECache - KConnHandlerOrderOffset,            // The position of the filter in the queue
                                            iSession->StringPool().StringF(HttpFilterCommonStringsExt::EConnHandlerFilter,
                                                                           HttpFilterCommonStringsExt::GetTable()));    //The name of the filter to add

    // Register the filter for submit events,
    // Adds a filter to the session's filter queue.
    iSession->FilterCollection().AddFilterL(*this,   // The filter to add
                                            THTTPEvent::ESubmit,       // The event that triggers this filter
                                            RStringF(), // The header whose presence triggers this filter, or KNullDesC to trigger on any header
                                            KAnyStatusCode, // The status code that triggers this filter, or KAnyStatusCode to trigger on any status code
                                            ECache - KConnHandlerOrderOffset,            // The position of the filter in the queue
                                            iSession->StringPool().StringF(HttpFilterCommonStringsExt::EConnHandlerFilter,
                                                                           HttpFilterCommonStringsExt::GetTable())); //The name of the filter to add

	// Create an instance of the CHttpFilterConnHandlerObserver in order to handle "No coverage" situation
    iObserver = CHttpFilterConnHandlerObserver::NewL(iConnCallback);
}

//---------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------
//
CHttpFilterConnHandler::~CHttpFilterConnHandler()
{
    delete iObserver;
}

// ---------------------------------------------------------------------------------------------
// CHttpFilterConnHandler::InstallFilterL
// Two-phased constructor. This function replaces NewL.
// ---------------------------------------------------------------------------------------------
//
CHttpFilterConnHandler* CHttpFilterConnHandler::InstallFilterL(TAny* aParams)
{
    __ASSERT_DEBUG(aParams != NULL, PanicHttpFiltersConnHandler());
    TConnParams* connParams = REINTERPRET_CAST(TConnParams*, aParams);
    __ASSERT_DEBUG(connParams->iSession != NULL, PanicHttpFiltersConnHandler());
    __ASSERT_DEBUG(connParams->iConnCallback != NULL, PanicHttpFiltersConnHandler());

    CHttpFilterConnHandler* filter = new (ELeave) CHttpFilterConnHandler( connParams->iSession, connParams->iConnCallback );
    CleanupStack::PushL(filter);
    filter->ConstructL();
    CleanupStack::Pop(filter);
    return filter;
}

// ---------------------------------------------------------------------------------------------
// CHttpFilterConnHandler::MHFLoad
// Called when the filter is being added to the session's filter queue.
// ---------------------------------------------------------------------------------------------
//
void CHttpFilterConnHandler::MHFLoad(RHTTPSession, THTTPFilterHandle)
{
    ++iLoadCount;
}

// ----------------------------------------------------------------------------------------------
// CHttpFilterConnHandler::MHFUnload
// Called when the filter is being removed from a session's filter queue.
// ----------------------------------------------------------------------------------------------
//
void CHttpFilterConnHandler::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle /*aFilterHandler*/)
{
    __ASSERT_DEBUG(iLoadCount >= 0, PanicHttpFiltersConnHandler());
    if (--iLoadCount)
    {
        return;
    }
    delete this;
}

// ------------------------------------------------------------------------------------------------
// CHttpFilterConnHandler::MHFRunL
// Process a transaction event.
// ------------------------------------------------------------------------------------------------
//
void CHttpFilterConnHandler::MHFRunL(RHTTPTransaction aTransaction,
                                     const THTTPEvent& aEvent)
{
    TInt state = 0;
    TInt gprsState = 0;
    TInt wcdmaState = 0;
    TApBearerType bearerType;

    if (aEvent.iStatus == THTTPEvent::ESubmit)
    {
        THTTPHdrVal isNewConn;
        RHTTPConnectionInfo	connInfo = iSession->ConnectionInfo();
        RStringPool strPool = aTransaction.Session().StringPool();
        TBool ret = connInfo.Property (strPool.StringF(HttpFilterCommonStringsExt::EHttpNewConnFlag,
                                       HttpFilterCommonStringsExt::GetTable()), isNewConn);

        if ( LocalHostCheckL(aTransaction) && !( ret && isNewConn.Type() == THTTPHdrVal::KTIntVal ) )
            {
            return;
            }

        THTTPHdrVal callback;
        RHTTPTransactionPropertySet propSet = aTransaction.PropertySet();
        RStringF callbackStr = strPool.StringF( HttpFilterCommonStringsExt::EConnectionCallback, 
            HttpFilterCommonStringsExt::GetTable() );

        MConnectionCallback* callbackPtr = NULL;
    
        // this is a transaction, already forwarded to download manager
        if( propSet.Property( callbackStr, callback ) )
        {
            callbackPtr = REINTERPRET_CAST( MConnectionCallback*, callback.Int() );
        }        
        // make sure it is not null
        callbackPtr = callbackPtr ? callbackPtr : iConnCallback;

        CreateConnectionL( &bearerType, callbackPtr );

        iObserver->SetBearerTypeAndUid(bearerType);
        // check if "No coverage" situation exists for the GPRS bearer
        if (bearerType == EApBearerTypeGPRS)
        {
            // we sent request, so change Observer state to  EActiveReady
            iObserver->GetObserverState(state);
            if (state == EIdle)
            {
              iObserver->GetStateL(KPSUidGprsStatusValue, gprsState);
			  iObserver->GetStateL(KPSUidWcdmaStatusValue, wcdmaState);
              if (gprsState == EPSGprsSuspend && wcdmaState == EPSWcdmaSuspend)
              {
                // user sent request from "No coverage" area, so all outstanding transactions will 
                // be canceled in this case and user will be notified.
                  callbackPtr->CoverageEvent(EErrNetUnreach);
              }
              else
              {
                  iObserver->SetObserverState(EActiveReady);         
              }
            }  
        }
        else if(bearerType == EApBearerTypeCDMA) 
        {
            // we sent request, so change Observer state to  EActiveReady
            iObserver->GetObserverState(state);
            if (state == EIdle)
            {
              iObserver->GetStateL(KPSUidWcdmaStatusValue, wcdmaState);
			  iObserver->GetStateL(KPSUidGprsStatusValue, gprsState);
              if (wcdmaState == EPSWcdmaSuspend  && gprsState == EPSGprsSuspend) //this should not meet all the time.
              {
                // user sent request from "No coverage" area, so all outstanding transactions will 
                // be canceled in this case and user will be notified.  
                  callbackPtr->CoverageEvent(EErrNetUnreach);
              }
              else
              {
                  iObserver->SetObserverState(EActiveReady);
              }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandler::MHFRunError
// Process an error that occured while processing the transaction.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterConnHandler::MHFRunError(TInt ,
        RHTTPTransaction aTransaction,
        const THTTPEvent& )
{
    aTransaction.Cancel();
    return KErrNone;
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandler::MHFSessionRunL
// Process a session event.
// -----------------------------------------------------------------------------
//
void CHttpFilterConnHandler::MHFSessionRunL(const THTTPSessionEvent& aEvent)
{
    TApBearerType bearerType;

    if (aEvent.iStatus == THTTPSessionEvent::EConnect)
    {
        CreateConnectionL(&bearerType, iConnCallback );
    }
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandler::MHFSessionRunError
// Called when MHFRunL leaves from a session event.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterConnHandler::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
{
    return aError;
}

// -----------------------------------------------------------------------------
// CHttpFilterConnHandler::GetObserver
// Access method
// -----------------------------------------------------------------------------
//
void CHttpFilterConnHandler::GetObserver(CHttpFilterConnHandlerObserver*& aObserver)
{
    aObserver = iObserver;
}


// -----------------------------------------------------------------------------
// CHttpFilterConnHandler::CreateConnection
// Function to handle Submit events.
// Set proxy properties (EUseProxy and EProxyAddress) in order to get connected throught
// proxy when a new HTTP session will be in effect.  The Proxy data will be taken from
// the CommDb if a new connection has been used.
// -----------------------------------------------------------------------------
//
void CHttpFilterConnHandler::CreateConnectionL(
    TApBearerType* bearerType,
    MConnectionCallback* aCallbackPtr )
{
    RStringPool stringPool = iSession->StringPool();
    RHTTPConnectionInfo	connInfo = iSession->ConnectionInfo();

    // pointer to RConnection typecasted as TInt
    TInt connectionPtr = 0;
    // handle to the socket server
    TInt sockSvrHandle = 0;
    // flag of a new connection
    TBool newConn = ETrue;

    TInt error = aCallbackPtr->CreateConnection(&connectionPtr, &sockSvrHandle, &newConn, bearerType);

    // Possible causes to leave are: KErrCommsLineFail (NW_STAT_CONN_FAILED), KErrCancel (NW_STAT_CANCELLED)
    if (error != KErrNone)
    {
        User::LeaveIfError( error );
    }

    // Remove the properties if they were set before, before setting them again
    connInfo.RemoveProperty(stringPool.StringF(HTTP::EHttpSocketConnection,
                            RHTTPSession::GetTable()));
    connInfo.RemoveProperty(stringPool.StringF(HTTP::EHttpSocketServ,
                            RHTTPSession::GetTable()));
    connInfo.RemoveProperty(stringPool.StringF(HttpFilterCommonStringsExt::EHttpNewConnFlag,
                            HttpFilterCommonStringsExt::GetTable()));


    // Add the properties only if there is valid connection and socket server handler
    if (connectionPtr != NULL && sockSvrHandle != 0)
    {
        connInfo.SetPropertyL(stringPool.StringF(HTTP::EHttpSocketConnection,
                              RHTTPSession::GetTable()), THTTPHdrVal(connectionPtr));
        // Set socket server handle
        connInfo.SetPropertyL(stringPool.StringF(HTTP::EHttpSocketServ,
                              RHTTPSession::GetTable()), THTTPHdrVal((TInt)sockSvrHandle));
        // Set aNewConn flag
        connInfo.SetPropertyL(stringPool.StringF(HttpFilterCommonStringsExt::EHttpNewConnFlag,
                              HttpFilterCommonStringsExt::GetTable()), THTTPHdrVal((TBool)newConn));
    }
}

//  End of File

