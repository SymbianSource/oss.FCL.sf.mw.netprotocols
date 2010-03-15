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
* Description:  Proxy filter
*
*/



// INCLUDE FILES
#include <http.h>
#include <es_sock.h>
#include <cdbstore.h>
#include <commdbconnpref.h>

// User Includes
#include "HttpFilterProxy.h"
#include "httpfiltercommonstringsext.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES
void PanicHttpFiltersProxy(TInt aErr = 0);

// CONSTANTS
const TInt KProxyOrderOffset = 20;
// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// LocalHostCheckL
// If the host is a local host, then remove the proxy property. Returns ETrue
// if the transaction request URI was for a localhost.
// -----------------------------------------------------------------------------
//
TBool LocalHostCheckL(RHTTPTransaction& aTransaction, RStringPool& aStringPool)
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
        // request is for localhost, explicitly state that this transaction
        // must not be sent to proxy.
    	RStringF proxyUsageStrF = aStringPool.StringF(HTTP::EProxyUsage, 
                                                      RHTTPSession::GetTable());
        
    	RStringF dontUseProxyStrF = aStringPool.StringF(HTTP::EDoNotUseProxy, 
                                                        RHTTPSession::GetTable());	

        aTransaction.PropertySet().RemoveProperty(proxyUsageStrF);
        aTransaction.PropertySet().SetPropertyL(proxyUsageStrF, dontUseProxyStrF);

    	//RStringF proxyAddrStrF = aStringPool.StringF(HTTP::EProxyAddress, 
        //                                             RHTTPSession::GetTable());

        //aTransaction.PropertySet().RemoveProperty(proxyAddrStrF);
        return ETrue;                                                          
        }
    return EFalse;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHttpFilterProxy::CHttpFilterProxy
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CHttpFilterProxy::CHttpFilterProxy(RHTTPSession* aSession)
{
    __ASSERT_DEBUG(aSession != NULL, PanicHttpFiltersProxy());
    iSession      = aSession;
}


// ------------------------------------------------------------------------------------------
// CHttpFilterProxy::ConstructL
// Symbian 2nd phase constructor can leave.
// ------------------------------------------------------------------------------------------
//
void CHttpFilterProxy::ConstructL()
{
	iStringPool = iSession->StringPool();
	iConnInfo = iSession->ConnectionInfo();
	
	// Added to fix panic when Proxy filter is used outside the browser initiated HTTP Framework.
	iSession->StringPool().OpenL(HttpFilterCommonStringsExt::GetTable());

    // Register the filter for submit events,
    // Adds a filter to the session's filter queue.

    iSession->FilterCollection().AddFilterL(*this,   // The filter to add
                                            THTTPEvent::ESubmit,       // The event that triggers this filter
                                            RStringF(),                // The header whose presence triggers this filter, or KNullDesC to trigger on any header
                                            KAnyStatusCode,            // The status code that triggers this filter, or KAnyStatusCode to trigger on any status code
                                            ECache - KProxyOrderOffset,// The position of the filter in the queue
                                            iStringPool.StringF(HttpFilterCommonStringsExt::EProxyFilter,RHTTPSession::GetTable()));  //The name of the filter to add

	// register for notification of KErrNotReady error codes
	// this allows us to re-start the connection if it fails
	iSession->FilterCollection().AddFilterL(*this, KErrNotReady, MHTTPFilter::ETidyUp -1, iStringPool.StringF(HttpFilterCommonStringsExt::EProxyFilter,RHTTPSession::GetTable()));
}

//---------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------
//
CHttpFilterProxy::~CHttpFilterProxy()
{
    // Cleanup strings.
    iProxyAddress.Close();
    iExceptions.Close();

/*
//  Closing RSocketServ is causing crash in SDK Applications. SDK Apps will have to close RSocketServ and RConnection after closing CHTTPSession.
//  It is not needed to close connection as soon as the CHttpConnectionManager close connection by
//  calling   CHttpConnectionManager::CloseConnection()
	if(iFilterOwnsConnection)
		{
		// Close ESOCK handles
		iConnection.Close();
		iSocketServ.Close();
		}
*/
}

// ---------------------------------------------------------------------------------------------
// CHttpFilterProxy::InstallFilterL
// Two-phased constructor. This function replaces NewL.
// ---------------------------------------------------------------------------------------------
//
CHttpFilterProxy* CHttpFilterProxy::InstallFilterL(TAny* aSession)
{
    __ASSERT_DEBUG(aSession != NULL, PanicHttpFiltersProxy());
    RHTTPSession* session = REINTERPRET_CAST(RHTTPSession*, aSession);
    CHttpFilterProxy* filter = new (ELeave) CHttpFilterProxy( session );
    CleanupStack::PushL(filter);
    filter->ConstructL();
    CleanupStack::Pop(filter);
    return filter;
}

// ---------------------------------------------------------------------------------------------
// CHttpFilterProxy::MHFLoad
// Called when the filter is being added to the session's filter queue.
// ---------------------------------------------------------------------------------------------
//
void CHttpFilterProxy::MHFLoad(RHTTPSession, THTTPFilterHandle)
{
    ++iLoadCount;
}

// ----------------------------------------------------------------------------------------------
// CHttpFilterProxy::MHFUnload
// Called when the filter is being removed from a session's filter queue.
// ----------------------------------------------------------------------------------------------
//
void CHttpFilterProxy::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle /*aFilterHandler*/)
{
    __ASSERT_DEBUG(iLoadCount >= 0, PanicHttpFiltersProxy());
    if (--iLoadCount)
    {
        return;
    }
    delete this;
}

// ------------------------------------------------------------------------------------------------
// CHttpFilterProxy::MHFRunL
// Process a transaction event.
// ------------------------------------------------------------------------------------------------
//
void CHttpFilterProxy::MHFRunL(RHTTPTransaction aTransaction,
                               const THTTPEvent& aEvent)
    {
    switch(aEvent.iStatus)
	    {
		case THTTPEvent::ESubmit:
		    {
            if (LocalHostCheckL(aTransaction, iStringPool))
                {
                return;
                }
			SetProxyL(aTransaction);
		    } 
            break;

		case KErrNotReady:
		    {
            if (LocalHostCheckL(aTransaction, iStringPool))
                {
                return;
                }
			if(iFilterOwnsConnection)
			    {
				// we must re-start the RConnection
		 		TInt err = iConnection.Start();
				if (err == KErrAlreadyExists)
				    {
					// the KErrNotReady must have come from elsewhere
					return;
				    }

				User::LeaveIfError(err);

				// re-submit the transaction
				aTransaction.Cancel();
				aTransaction.SubmitL();				
			    }
			break;
		    }

		default:
			break;
	    }

    }

// -----------------------------------------------------------------------------
// CHttpFilterProxy::MHFRunError
// Process an error that occured while processing the transaction.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterProxy::MHFRunError(TInt /*aError*/,
                                   RHTTPTransaction aTransaction,
                                   const THTTPEvent& /*aEvent*/)
{
    // Cleanup strings.
    iProxyAddress.Close();
    iExceptions.Close();
    aTransaction.Fail();
    return KErrNone;
}

// -----------------------------------------------------------------------------
// CHttpFilterProxy::MHFSessionRunL
// Process a session event.
// -----------------------------------------------------------------------------
//
void CHttpFilterProxy::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
{
}

// -----------------------------------------------------------------------------
// CHttpFilterProxy::MHFSessionRunError
// Called when MHFRunL leaves from a session event.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterProxy::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
{
    return aError;
}

// -----------------------------------------------------------------------------
// CHttpFilterProxy::SetProxyL
// Function to handle Submit events.
// Set proxy properties (EUseProxy and EProxyAddress) in order to get connected throught
// proxy when a new HTTP session will be in effect.  The Proxy data will be taken from
// the CommDb if a new connection has been used. If a proxy property has already been 
// set by a higher filter or by the client, then those settings are preferred over 
// CommsDB.
// -----------------------------------------------------------------------------
//
void CHttpFilterProxy::SetProxyL(const RHTTPTransaction aTransaction)
{
    THTTPHdrVal isNewConn, proxyAddress;

    // If connection has been disconnected the user could change Access Point, so let's
    // check if a new connection has been used.
    TBool ret = iConnInfo.Property (iStringPool.StringF(HttpFilterCommonStringsExt::EHttpNewConnFlag,
                                   HttpFilterCommonStringsExt::GetTable()), isNewConn);

    if (ret && isNewConn.Type() == THTTPHdrVal::KTIntVal && (isNewConn.Int() == 1))
    {
        iUsage = EFalse;
        // Read proxy info from the CommDB
        ReadProxyInfoL(iStringPool,
                       iUsage,
                       iProxyAddress,
                       iExceptions);
    }
    else
    {
        TBool useProxy( ETrue );
        THTTPHdrVal proxyUsageVal;

	    // Is the property for proxy usage set?
        TBool hasUsageValue = iConnInfo.Property(iStringPool.StringF(HTTP::EProxyUsage,
                            RHTTPSession::GetTable()), proxyUsageVal);
                            
        // If the property is set, is it EUseProxy
        if (hasUsageValue)
        {
        useProxy = ( proxyUsageVal.StrF().Index(RHTTPSession::GetTable()) == HTTP::EUseProxy);
        }

        // Use proxy if the property value is EUseProxy or no property is set.
        if (useProxy || !hasUsageValue)
        {
            THTTPHdrVal proxyAddressVal;
            TBool hasValue = iConnInfo.Property(iStringPool.StringF(HTTP::EProxyAddress,
                                               RHTTPSession::GetTable()), proxyAddressVal);
            if (!hasValue)
            {
                // The proxyAddress has not been set, so Read proxy info from the CommDB
                ReadProxyInfoL(iStringPool,
                               iUsage,
                               iProxyAddress,
                               iExceptions);

            }
            else
            {
                // Now get the proxy address...
                iProxyAddress = proxyAddressVal.StrF().Copy();
                iUsage = ETrue;
            }
        }
//      }
    }

    // We should not use proxy if the uri matches to one of exceptions from the exception list
    // In this case the Origing server will be used

    ExcptionsCompare(aTransaction);
    
    // Respect proxy settings already defined by client or higher HTTP filter
    THTTPHdrVal proxyUsage;  // Check if property present or not present, not the value.
    if ( !iConnInfo.Property(iStringPool.StringF(HTTP::EProxyUsage,
                            RHTTPSession::GetTable()), proxyUsage)) 
   	{ 

		// Set the proxy address and proxy Usage
		proxyAddress = THTTPHdrVal(iProxyAddress);

		iConnInfo.RemoveProperty(iStringPool.StringF(HTTP::EProxyAddress,
		                        RHTTPSession::GetTable()));
		iConnInfo.SetPropertyL(iStringPool.StringF(HTTP::EProxyAddress,
		                      RHTTPSession::GetTable()), proxyAddress);

		iConnInfo.RemoveProperty(iStringPool.StringF(HTTP::EProxyUsage,
		                        RHTTPSession::GetTable()));
		if (iUsage)
		{
		    iConnInfo.SetPropertyL(iStringPool.StringF(HTTP::EProxyUsage,
		                          RHTTPSession::GetTable()), iStringPool.StringF(HTTP::EUseProxy,
		                                  RHTTPSession::GetTable()));
		}
		else
		{
		    iConnInfo.SetPropertyL(iStringPool.StringF(HTTP::EProxyUsage,
		                          RHTTPSession::GetTable()), iStringPool.StringF(HTTP::EDoNotUseProxy,
		                                  RHTTPSession::GetTable()));
		}
    	
   	}  
    

    // Cleanup strings.
    iProxyAddress.Close();
    iExceptions.Close();
}


// -----------------------------------------------------------------------------
// CHttpFilterProxy::ReadProxyInfoL
// Purpose:	Reads the Proxy information from the comms database.
// Gives the Proxy usage, a proxy address (<proxy name>:<proxy port>),
// and proxy exceptions through the output arguments.
// -----------------------------------------------------------------------------
//
void CHttpFilterProxy::ReadProxyInfoL(const RStringPool aStringPool,
                                      TBool&      aUsage,
                                      RStringF&   aProxyAddress,
                                      RStringF&   aExceptions)

{
    // Let's find Internet Access point ID (ServiceId) in use from the RConnection associated with the HTTP framework
    THTTPHdrVal connValue;
    TUint32     serviceId;
    TUint pushCount = 0;
    RConnection* connPtr = NULL;
	THTTPHdrVal		iapId;
	TCommDbConnPref pref;
    TBool hasValue = iConnInfo.Property (aStringPool.StringF(HTTP::EHttpSocketConnection, RHTTPSession::GetTable()),
                     connValue);

    if (hasValue && connValue.Type() == THTTPHdrVal::KTIntVal)
    {
        connPtr = REINTERPRET_CAST(RConnection*, connValue.Int());
	}
	else
	{
		// Connect to ESOCK and open connection handle
		User::LeaveIfError(iSocketServ.Connect());
		User::LeaveIfError(iConnection.Open(iSocketServ));

		TBool ret = iConnInfo.Property (aStringPool.StringF(HttpFilterCommonStringsExt::EAccessPointID, HttpFilterCommonStringsExt::GetTable()),
                     iapId);
		if (ret && (iapId.Type() == THTTPHdrVal::KTIntVal) && (iapId.Int() != 0) )
			{
			 pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
			 pref.SetIapId( iapId.Int() );
			 //pref.SetBearerSet( ECommDbBearerGPRS );
			 pref.SetDirection(ECommDbConnectionDirectionOutgoing);
			 TInt err = iConnection.Start(pref);
			 if ( err != KErrNone )
				User::LeaveIfError(iConnection.Start());
			}
		else
			{
			// start the connection - note that this thread will be blocked until the connection is established.
			User::LeaveIfError(iConnection.Start());
			}

		connPtr = &iConnection;

		// set the ESOCK handles as session properties after successful connection
		iConnInfo.SetPropertyL(aStringPool.StringF(HTTP::EHttpSocketServ, RHTTPSession::GetTable()), iSocketServ.Handle());
		iConnInfo.SetPropertyL(aStringPool.StringF(HTTP::EHttpSocketConnection, RHTTPSession::GetTable()), reinterpret_cast<TInt>(&iConnection));

		iFilterOwnsConnection = ETrue;
	}

    // Retrieve the service Id  from the RConnection ; "<table name>/<field name>
    User::LeaveIfError(connPtr->GetIntSetting(_L("IAP\\IAPService"), serviceId));

    // Retrieve the service Type  from the RConnection
    HBufC* serviceType16 = HBufC::NewL(KCommsDbSvrMaxFieldLength);
    CleanupStack::PushL(serviceType16);
    pushCount++;
    TPtr serviceTypePtr16 = serviceType16->Des();
    User::LeaveIfError(connPtr->GetDesSetting(_L("IAP\\IAPServiceType"), serviceTypePtr16));

    // Open the Comms DB with the IAP style
    CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeUnspecified);
    CleanupStack::PushL(db);
    pushCount++;

    // Opens a view on records in the Proxies table with a specified range of service types and service IDs.
    // Proxies records are included that have matching service types and IDs.
    // When the use of the view object is complete, it should be popped from the cleanup stack, and deleted.
    CCommsDbTableView* pView = db->OpenViewOnProxyRecordLC(serviceId, serviceTypePtr16);

    pushCount++;
    TInt err = pView->GotoFirstRecord();
	aUsage = ETrue;
	// if there is some other DB error.
	if ( err != KErrNotFound && err != KErrNone )
		{
        User::LeaveIfError( err );
		}
    if ( err == KErrNotFound )
		{// setting to EFalse because proxy table view does not exisit, was not created during Access point creation?
		aUsage = EFalse;
		}
	else
		// no need to keep going as not Proxy will be used. 
		{
		// Read the proxy usage flag from the selected Proxy record.
		pView->ReadBoolL(TPtrC(PROXY_USE_PROXY_SERVER), aUsage);

		// COntinue only if the proxy usage flag is set
		if (aUsage)
			{
			// Read the server name specified in the selected Proxy record.
			// The proxyAddress will be build based on the serverName and the port number
			// separated with ':'
			HBufC* serverName = pView->ReadLongTextLC(TPtrC(PROXY_SERVER_NAME));
			pushCount++;
			TPtr serverNamePtr16 = serverName->Des();
			serverNamePtr16.Trim();

			// Continue only if we have a proxy name
			if (serverNamePtr16.Length() > 0 && serverNamePtr16.Ptr() != NULL)
				{
				// Read the port number specified in the selected Proxy record
				TUint32 portNumber = 8080;      // default port number
				TInt length = 1;                // add 1 for the separator ':'            
				pView->ReadUintL(TPtrC(PROXY_PORT_NUMBER), portNumber);
				// calculate the length of portNumber

				if (portNumber == 0)
				   {
				   portNumber = 8080;
				   }

				TUint32 portNumSave = portNumber;
				while (portNumSave > 0)
					{
				   portNumSave = portNumSave/10;
				   length++;
					}
				// convert number to string & add to the server name. We should get <serverName1>:<port number>
				HBufC8* proxyAddress8 = HBufC8::NewL(serverNamePtr16.Length() + length);
				CleanupStack::PushL(proxyAddress8);
				pushCount++;
				TPtr8 proxyAddressPtr8 = proxyAddress8->Des();
				proxyAddressPtr8.Copy(serverNamePtr16);
				_LIT(KSeparator, ":");
				proxyAddressPtr8.Append(KSeparator);
				proxyAddressPtr8.AppendNum((TInt)portNumber);

				// Set gateway as a service number - this is the proxy address
				aProxyAddress = aStringPool.OpenFStringL(proxyAddressPtr8);

				// Read the server name specified in the selected Proxy record.
				HBufC* exceptions16 = pView->ReadLongTextLC(TPtrC(PROXY_EXCEPTIONS));
				pushCount++;
				TPtr exceptionsPtr16 = exceptions16->Des();
				// Copy to 8-bits...
				HBufC8* exceptions8 = HBufC8::NewL(exceptionsPtr16.Length());
				CleanupStack::PushL(exceptions8);
				pushCount++;
				TPtr8 exceptionsPtr8 = exceptions8->Des();
				exceptionsPtr8.Copy(exceptionsPtr16);

				// Set gateway as a service number - this is the proxy address
				aExceptions = aStringPool.OpenFStringL(exceptionsPtr8);
				}
			else
				{
				aUsage = EFalse;
				}
			}
		}
    // All done - clean up
    CleanupStack::PopAndDestroy(pushCount); //pView, serverType16, db, serverName, proxyAddress8, exceptions8  exceptions16
}

//---------------------------------------------------------------------------------
//
//   Get exception from the list of exceptions
//   Returns ETrue if there is at least one exception in the list.
//   Returns EFalse if list of exceptions is empty or there is no exceptions in the list any more.
//---------------------------------------------------------------------------------
TBool CHttpFilterProxy::GetException(TPtr8& aException,TInt& aStartIndex)
{
    TBool done;
    TPtrC8 exceptionsDesC = iExceptions.DesC();
    TPtr8 exceptionsDes((TUint8*)exceptionsDesC.Ptr(), exceptionsDesC.Length(),exceptionsDesC.Length());

    exceptionsDes.TrimAll();
    // get part of string starting from aStartIndex. aStartIndex =0 when it is called the first time.
    TPtrC8 desPtr = exceptionsDes.Mid(aStartIndex);

    //The offset of the exception position from the beginning of the exception list
    const TChar separator(';');
    TInt position = desPtr.Locate(separator);
    if (position != KErrNotFound)
    {
        // Extracts a portion of the data starting from the beggining
        // Get the rest of the descriptor without the separator that we have found
        aException.Set((unsigned char*)desPtr.Ptr(), position, position);
        aStartIndex = position + 1;
        done = ETrue;
    }
    else
    {
        aException.Set((unsigned char*)desPtr.Ptr(), desPtr.Length(), desPtr.Length());
        done = EFalse;
    }
    return done;
}

// -----------------------------------------------------------------------------
// CHttpFilterProxy::ExcptionsCompare
// Compare each exception from the exception list against the current uri.
// -----------------------------------------------------------------------------
//
void CHttpFilterProxy::ExcptionsCompare(const RHTTPTransaction aTransaction)
{
    if (iUsage)
    {
        TBool isFound  = ETrue;
        TInt   index = 0;
        TInt   loopCount = 0;
        TPtr8 exception (NULL, 0,0);
        // Let's get uri for this transaction to comapre the current uri with an exception.
        // If the current uri has an exception for proxy set proxy usage to EDoNotUseProxy.
        RHTTPRequest req = aTransaction.Request();
        TUriC8 originalUri = req.URI();
        const TDesC8& uriHost = originalUri.UriDes();

        TPtrC8 exceptionsDes = iExceptions.DesC();
        if (exceptionsDes.Length() != 0)
        {
            // at least one exception has been found in the list of exceptions
            while (isFound)
            {
                loopCount++;                
                isFound = GetException(exception, index);
                if (exception.Compare(uriHost) == NULL)
                {
                    iUsage = EFalse;
                    isFound = EFalse;
                }
            } //while
        }
    }
}

//  End of File
