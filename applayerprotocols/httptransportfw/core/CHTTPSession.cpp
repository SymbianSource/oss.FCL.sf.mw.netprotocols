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

// System includes
#include <e32std.h>
#include <ecom/ecom.h>
#include <http/thttpfilterregistration.h>
#include <http/mhttptransactioncallback.h>
#include <http/mhttpsessioneventcallback.h>
#include <http/rhttpheaders.h>
#include <http/cecomfilter.h>
#include <http/tfilterconfigurationiter.h>
#include <httpstringconstants.h>
//For Proxy Info Reading
#include <cdblen.h>
#include <cdbcols.h>
#include <commsdat.h>
#include <comms-infras/dbaccess.h>
#include <metadatabase.h>
#include <commsdattypesv1_1.h>
#include <commsdattypeinfov1_1.h>


// Local includes
#include "CTransaction.h"
#include "CHTTPManager.h"
#include "CHeaderField.h"
#include "CHeaders.h"
 

// Class signature
#include "CHTTPSession.h"

// Literals
_LIT8(KTxtDefaultProtocol, "HTTP/TCP");
_LIT8(KTxtCoreFilterProtocol, "TFCORE");

void TDummyTransactionCallback::MHFRunL(RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	}

TInt TDummyTransactionCallback::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

void TClientFilter::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	CTransaction::Implementation(aTransaction)->Callback().
		MHFRunL(aTransaction, aEvent);
	}

TInt TClientFilter::MHFRunError(TInt aError, RHTTPTransaction aTransaction,
								const THTTPEvent& aEvent)
	{
	return CTransaction::Implementation(aTransaction)->Callback().
		MHFRunError(aError, aTransaction, aEvent);
	}


void TClientFilter::MHFSessionRunL(const THTTPSessionEvent& aEvent)
	{
	if (iSessionEventCallback)
		iSessionEventCallback->MHFSessionRunL(aEvent);
	}


TInt TClientFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent)
	{
	if (iSessionEventCallback)
		return iSessionEventCallback->MHFSessionRunError(aError, aEvent);
	return aError;
	}

void TClientFilter::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/)
	{
	}


void TClientFilter::MHFLoad(RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/)
	{
	}


CHTTPSession::CHTTPSession()
	{
	}

void CHTTPSession::ConstructL(const TDesC8& aProtocol, MHTTPFilterCreationCallback* aSessionCallback)
	{
	THTTPHdrVal v;
	iConnectionInfo = CHeaderFieldPart::NewL(v);
	iManager = CHTTPManager::CreateOrFindL();

	// Set the session closing flag session property. This property is a pointer to the
	// TBool flag (iSessionClosing) used as a flag to indicate when the session is closing.
	TInt sessionClosingFlagPtr = reinterpret_cast<TInt>(&iSessionClosing);
	ConnectionInfo().SetPropertyL( (iManager->StringPool()).StringF(HTTP::ESessionClosing, RHTTPSession::GetTable() ), THTTPHdrVal (sessionClosingFlagPtr) );

	// Create the protocol handler
	CreateProtocolHandlerL(aProtocol);

	// Build filter lists and install mandatory and implicit filters
	InitializeFiltersL(aProtocol);
	
	// If the client requires a callback to be able to configure the filters, call it now
	if( aSessionCallback != NULL )
		{
		iFilterConfigIter = new(ELeave) TFilterConfigurationIterator(this);
		}
	}

void CHTTPSession::InitializeFiltersL(const TDesC8& aProtocol)
	{
	__START_PERFORMANCE_LOGGER();
	const TChar protocolSeparatorChar = '/';
	// Get list of ECOM plugins that match the interface UID
	REComSession::ListImplementationsL(KUidFilterPluginInterface, iEComFilterInfoArray);

	// Locate the protocol separator and return with errorcode if its not found as its invalid
	const TInt clientProtSeparator = aProtocol.Locate(protocolSeparatorChar);
	if( clientProtSeparator == KErrNotFound )
		User::Leave(KErrNotFound);

	// Store the protocol provided by the client
	TPtrC8 clientProtocol(aProtocol.Left(clientProtSeparator));

	// Loop through all the filter plugins found by ECom
	const TInt filterCount = iEComFilterInfoArray.Count();
	for(TInt ii=0; ii<filterCount; ++ii)
		{
		// Get the descriptor with the filter datatype
		TPtrC8 filterProtocol(iEComFilterInfoArray[ii]->DataType());

		// Find the protocol in the data type, if thats not found then find the TFCORE data type
		TInt protocolDataTypeStart = filterProtocol.FindF(clientProtocol);
		TInt protocolLength = clientProtocol.Length();
		if( protocolDataTypeStart == KErrNotFound )
			{
			protocolDataTypeStart = filterProtocol.FindF(KTxtCoreFilterProtocol());
			protocolLength = KTxtCoreFilterProtocol().Length();
			}

		// Build table of plugins of core and specified protocol filters
		if( protocolDataTypeStart > KErrNotFound )
			{
			// Set the offset for the beginning of the filter type and check that the seperator exists
			TInt offset = protocolDataTypeStart + protocolLength;
			if( filterProtocol[offset] != STATIC_CAST(TUint8, protocolSeparatorChar) )
				User::Leave(KErrNotFound);
			// Increment the offset for the seperator
			++offset;

			// Ensure that there is data after the seperator
			if( filterProtocol.Length() <= offset )
				User::Leave(KErrNotFound);

			// Create a TSessionFilterInfo with the filter plugin data and append to client array
			TSessionFilterInfo* filterInfo = new(ELeave) TSessionFilterInfo;
			CleanupStack::PushL(filterInfo);

			// Set the filter properties
			filterInfo->iFilterInfo = iEComFilterInfoArray[ii];
			// Get the category of the filter from the filter data type
			const TUint KTxtExplicitChar = '-';
			const TUint KTxtMandatoryChar = '+';
			TChar filterSymbol(filterProtocol[offset]);
			if( filterSymbol==KTxtExplicitChar)
				{
				filterInfo->iCategory = TSessionFilterInfo::EExplicit;
				filterInfo->iFilterPlugin = NULL;
				}
			else if( filterSymbol==KTxtMandatoryChar )
				{
				filterInfo->iCategory = TSessionFilterInfo::EMandatory;
				// Install the filter
				// This should leave as the filter is mandatory
				filterInfo->iFilterPlugin = CEComFilter::InstallFilterL(this->Handle(), iEComFilterInfoArray[ii]->ImplementationUid());
				}
			else
				{
				filterInfo->iCategory = TSessionFilterInfo::EImplicit;
				// Install the filter
				// This is trapped if it leaves as the transport framework should be able to plough on without it
				// But leave if the error is KErrNoMemory.
				TRAPD ( err, filterInfo->iFilterPlugin = CEComFilter::InstallFilterL(this->Handle(), iEComFilterInfoArray[ii]->ImplementationUid()) );
				if ( err == KErrNoMemory )
					{
					User::Leave ( err );
					}
				}

			// Add the filter information to the list
			User::LeaveIfError(iFilterInfoList.Append(filterInfo));
			CleanupStack::Pop(filterInfo);
			}
		}
	__END_PERFORMANCE_LOGGER(_L(",CHTTPSession::InitializeFiltersL()"));
	}

CHTTPSession* CHTTPSession::NewL()
	{
	return NewL(KTxtDefaultProtocol(), NULL);
	}

CHTTPSession* CHTTPSession::NewL(const TDesC8& aProtocol)
	{
	return NewL(aProtocol, NULL);
	}

CHTTPSession* CHTTPSession::NewL(const TDesC8& aProtocol, MHTTPFilterCreationCallback* aSessionCallback)
	{
	CHTTPSession* self = new (ELeave) CHTTPSession;
	CleanupStack::PushL(self);
	self->ConstructL(aProtocol, aSessionCallback);
	CleanupStack::Pop(self);
	return self;
	}

CHTTPSession::~CHTTPSession()
	{
	iSessionClosing = ETrue;

	while (iTransactions.Count())
		iTransactions[iTransactions.Count() - 1]->Close();
	iTransactions.Close();

	// We need a RHTTPSession to give to the filters, so fake one up
	RHTTPSession s;
	s.iImplementation = this;

	// If there isn't a manager we must have failed to construct
	// the session so there can't be any filters. So it's safe not
	// to close them.
	if (iManager)
		{
		RStringPool strP = iManager->StringPool();
		for (TInt i = 0; i < iFilterQueue.Count(); i++)
			{
			THTTPFilterHandle h;
			h.iValue = iFilterQueue[i].iHandle;
			iFilterQueue[i].iFilter->MHFUnload(s, h);
			strP.StringF(iFilterQueue[i].iHeader).Close();
			strP.StringF(iFilterQueue[i].iName).Close();
			}
		iFilterQueue.Close();
		}

	delete iProtocolHandler;
	delete iConnectionInfo;
	delete iRequestSessionHeaders;
	delete iResponseSessionHeaders;
	delete iFilterConfigIter;

	// Flush and close the filter arrays
	iFilterInfoList.ResetAndDestroy();
	iEComFilterInfoArray.ResetAndDestroy();

	// No longer need the manager... the last session to close causes the manager
	// (and string pool) to be deleted.
	if (iManager)
		iManager->Release();
	
	// force ECOM to release all - final clean-up 
	REComSession::FinalClose();
	}

TFilterConfigurationIterator* CHTTPSession::FilterConfigIterator() const
	{
	return iFilterConfigIter;
	}

void CHTTPSession::ListAvailableProtocolsL(RPointerArray<HBufC8>& aProtocolArray)
	{
	// Delete existing data from protocol array
	aProtocolArray.ResetAndDestroy();

	// Query ECom for available protocols, need to use TCleanupItem as array is not cleaned
	// up by ECom if a leave occurs
	RImplInfoPtrArray availableEComProtocols;
	TCleanupItem protocolArray(CleanListImplementationArray, &availableEComProtocols);
	CleanupStack::PushL(protocolArray);
	REComSession::ListImplementationsL(KUidProtocolHandlerPluginInterface, availableEComProtocols);

	TInt error = KErrNone;
	for( TInt i=0; i<availableEComProtocols.Count(); ++i )
		{
		// Add the protocol as a heap based descriptor to the client descriptor array
		HBufC8* protocol = availableEComProtocols[i]->DataType().Alloc();
		if( protocol != NULL )
			{
			error = aProtocolArray.Append(protocol);
			if( error != KErrNone )
				{
				delete protocol;
				break;
				}
			}
		else
			{
			error = KErrNoMemory; // as protocol did not get allocated
			break;
			}
		}

	// Clean up ECom implementation array
	CleanupStack::PopAndDestroy(); // availableEComProtocols as TCleanUpItem
	if(error!=KErrNone)
		{
		// If an error occurs, cleanup passed in array and leave
		aProtocolArray.ResetAndDestroy();
		User::Leave(error);
		}
	}

void CHTTPSession::CleanListImplementationArray(TAny* aArray)
	{
	RImplInfoPtrArray* array = REINTERPRET_CAST(RImplInfoPtrArray*, aArray);
	array->ResetAndDestroy();
	}

void CHTTPSession::CreateProtocolHandlerL(const TDesC8& aProtocol)
	{
	__START_PERFORMANCE_LOGGER();
	if (!iProtocolHandler)
		{
		RHTTPSession r;
		r.iImplementation = this;

		// Create the correct protocol using the protocol passed descriptor passed in
		iProtocolHandler = CProtocolHandler::NewL(aProtocol, r);
		
		// Add the protocol handler as a filter
		r.FilterCollection().AddFilterL(*iProtocolHandler, THTTPEvent(THTTPEvent::EAll),
										RStringF(), KAnyStatusCode, 
										MHTTPFilter::EProtocolHandler, 
										iManager->StringPool().
										StringF(HTTP::EProtocolHandler,RHTTPSession::GetTable()));

		// Install the 'client filter', a filter that does the job of
		// passing messages to the client.
		r.FilterCollection().AddFilterL(iClientFilter, 
										THTTPEvent(THTTPEvent::EAll, KHTTPMatchAnyEventUid),
										RStringF(), KAnyStatusCode, 
										MHTTPFilter::EClient, 
										iManager->StringPool().
										StringF(HTTP::EClient,RHTTPSession::GetTable()));
		}
	__END_PERFORMANCE_LOGGER(_L(",CHTTPSession::CreateProtocolHandlerL()"));
	}


void CHTTPSession::AddTransactionL(CTransaction* aTransaction)
	{
	User::LeaveIfError(iTransactions.InsertInUnsignedKeyOrder(aTransaction));
	}

void CHTTPSession::RemoveTransaction(CTransaction* aTransaction)
	{
	// Check it actualy exists! Otherwise it's hard for the
	// transaction to know if it should remove itself when the
	// destructor is called from a leave during the transaction's
	// NewL)
	TInt index = iTransactions.FindInUnsignedKeyOrder(aTransaction);
	if (index != KErrNotFound)
		iTransactions.Remove(index);
	}


void CHTTPSession::SetSessionEventCallback(MHTTPSessionEventCallback* aSessionEventCallback)
	{
	iClientFilter.SetSessionEventCallback(aSessionEventCallback);
	}

void CHTTPSession::SendSessionEventL(THTTPSessionEvent aStatus, THTTPSessionEvent::TDirection aDirection, 
									THTTPFilterHandle aStart)
	{
	if (!iSessionEventTransaction)
		{
		RStringPool strP = iManager->StringPool();
		RStringF dummy = strP.StringF(HTTP::EGET,RHTTPSession::GetTable());
		_LIT8(KDummyUrl, "http://www.symbian.com");
		TUriParser8 dummyURI; 
		dummyURI.Parse(KDummyUrl);
		iSessionEventTransaction = CTransaction::NewL(iDummyTransactionCallback, dummyURI, *this, dummy);
		}

	iSessionEventTransaction->SendEventL(aStatus, aDirection, aStart);
	}

void CHTTPSession::FailSessionEvent(THTTPFilterHandle aStart)
	{
	if (iSessionEventTransaction)
		{
		iSessionEventTransaction->Fail(aStart);
		}
	}

/** Indicates that transactions should stop blocking (and if
    transaction has blocked, they should be awakened) */
void CHTTPSession::Unblock()
	{
	if (iBlockingState == EHasBlocked)
		{
		for (TInt i = 0; i < iTransactions.Count(); i++)
			iTransactions[i]->Unblock();
		}
	iBlockingState = ENormal;
	}



RHTTPHeaders CHTTPSession::RequestSessionHeadersL()
	{
	// Create the session headers if necessary. 
	if (iRequestSessionHeaders == NULL)
		{
		iRequestSessionHeaders = CHeaders::NewL(*iProtocolHandler->Codec());
		}

	return iRequestSessionHeaders->Handle();
	}

RHTTPHeaders CHTTPSession::ResponseSessionHeadersL()
	{
	// Create the session headers if necessary. 
	if (iResponseSessionHeaders == NULL)
		{
		iResponseSessionHeaders = CHeaders::NewL(*iProtocolHandler->Codec());
		}

	return iResponseSessionHeaders->Handle();
	}


void CHTTPSession::SetupDefaultProxyFromCommsDatL()
	{		
	TInt32 defaultIapId = 0;
	TUint32 serviceId = 0;
	TBuf<KCommsDbSvrMaxFieldLength> serviceType;	
	
	// Create CommmsDat session using latest version of commsdat
	CMDBSession* dbSession = CMDBSession::NewLC(CMDBSession::LatestVersion());
	
	//Read Default IAP inorder to get its service Id and type.	
	//Default IAP is the one in "connection preferences" table and with a "rank" of 1.
	CCDConnectionPrefsRecord *connPrefRecord = static_cast<CCDConnectionPrefsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdConnectionPrefsRecord));
   CleanupStack::PushL(connPrefRecord);   

    // Set the direction of connection
   connPrefRecord->iDirection = ECommDbConnectionDirectionOutgoing;
   connPrefRecord->iRanking = 1;
   
    if(!connPrefRecord->FindL(*dbSession))
        {
       User::Leave(KErrNotFound);      
       }           
    defaultIapId = connPrefRecord->iDefaultIAP;			
   //Done with Connection Preference table, destroy it.
	CleanupStack::PopAndDestroy	(connPrefRecord);
	
	//Got the default IAP id which was its record id in IAP table.
	//Get the Service ID and Serivce type of this IAP.
	CCDIAPRecord* defaultIapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	CleanupStack::PushL(defaultIapRecord);
		//Set the ID.
	defaultIapRecord->SetRecordId(defaultIapId);
	//Load the record.
	defaultIapRecord->LoadL(*dbSession);
	//read its serice id and service type.	
	serviceId = defaultIapRecord->iService;		
	serviceType.Copy(defaultIapRecord->iServiceType);
	CleanupStack::PopAndDestroy(defaultIapRecord);	
	
	// From this serviceId and serviceType get the proxy record		
	// Create the CMDBRecordSet for the search
	
	CMDBRecordSet<CCDProxiesRecord>* pProxiesRecordSet = new (ELeave)CMDBRecordSet<CCDProxiesRecord>(KCDTIdProxiesRecord);
	CleanupStack::PushL(pProxiesRecordSet);

	CCDProxiesRecord* primingProxyRecord = static_cast <CCDProxiesRecord*> (CCDRecordBase::RecordFactoryL(KCDTIdProxiesRecord));
	CleanupStack::PushL(primingProxyRecord);
	// build priming record for the search
	primingProxyRecord->iServiceType.SetMaxLengthL(serviceType.Length());
	primingProxyRecord->iServiceType = serviceType;
	
	primingProxyRecord->iService = serviceId;
	primingProxyRecord->iUseProxyServer = ETrue;
	
	pProxiesRecordSet->iRecords.AppendL(primingProxyRecord);
	CleanupStack::Pop(primingProxyRecord);
	
	// Search for the priming record
	HBufC8* proxyServer = NULL;
	
	if (pProxiesRecordSet->FindL(*dbSession))	
		{
		// Proxy is located so copy settings to aProxyServer
		TPtrC serverName(static_cast <CCDProxiesRecord*> (pProxiesRecordSet->iRecords[0])->iServerName);
		// Create the 8-bit version allowing extra characters for port number
		
		const TInt32 KMaxLenColonNumberStr = (1 + 10);  // 1 Char for ':' and 10 Chars for TUint32 string
		proxyServer = HBufC8::NewLC(serverName.Length() + KMaxLenColonNumberStr);
		
		TPtr8 ptr(proxyServer->Des());
		ptr.Copy(serverName);

		ptr.Append(':');
		ptr.AppendNum(static_cast <CCDProxiesRecord*> (pProxiesRecordSet->iRecords[0])->iPortNumber);
		}
			
	if (proxyServer)
		{
		if ((*proxyServer).Length() > 0)
			{//Create properties for current session to use the default proxy settings.
			RHTTPConnectionInfo	connInfo = this->ConnectionInfo();
			RStringPool stringPool = this->Manager().StringPool();
			RStringF address = stringPool.OpenFStringL(*proxyServer);
       	CleanupClosePushL(address);
	       	connInfo.SetPropertyL(stringPool.StringF(HTTP::EProxyUsage, RHTTPSession::GetTable()),stringPool.StringF(HTTP::EUseProxy, RHTTPSession::GetTable()));
	   		connInfo.SetPropertyL(stringPool.StringF(HTTP::EProxyAddress, RHTTPSession::GetTable()),THTTPHdrVal(address));
			CleanupStack::PopAndDestroy(&address);
			}
		CleanupStack::PopAndDestroy(proxyServer);
		}
	 CleanupStack::PopAndDestroy(pProxiesRecordSet);
	 CleanupStack::PopAndDestroy(dbSession);	
	}

void CHTTPSession::SetupHttpDataOptimiser(MHttpDataOptimiser& aHttpOptimiser)
 	{
 	iHttpDataOptimiser = &aHttpOptimiser;
   	}
   
MHttpDataOptimiser* CHTTPSession::HttpDataOptimiser()
 	{
 	return iHttpDataOptimiser;
 	}

