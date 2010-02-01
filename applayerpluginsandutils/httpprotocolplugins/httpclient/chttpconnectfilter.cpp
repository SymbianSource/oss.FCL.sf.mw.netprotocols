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

#include "chttpconnectfilter.h"

#include <httpstringconstants.h>
#include <httperr.h>
#include <http/rhttpheaders.h>

_LIT8(KUAProfile, "x-wap-profile");

// 'this' used in base member initializer list, The 'this' pointer being used is a base class pointer.
#pragma warning( disable : 4355 )

CHttpConnectFilter* CHttpConnectFilter::NewL(RHTTPSession aSession)
	{
	CHttpConnectFilter* self = new (ELeave) CHttpConnectFilter(aSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CHttpConnectFilter::~CHttpConnectFilter()
	{
	iConnectTransactions.Close();
	iPendingTransactions.Close();

	// If this object has been destroyed from the cleanup stack during creation
	// of the object, it might still be loaded - check. Normally the delete is 
	// initiated by the 'delete this' in MHFUnload.
	if( iLoadCount )
		{
		// As already in a destructor, MHFUnload must not delete this again.
		iLoadCount = -1;
		RStringF filterName = iStringPool.StringF(HTTP::EHttpConnectFilter, iStringTable);
		iSession.FilterCollection().RemoveFilter(filterName);
		}
	}

CHttpConnectFilter::CHttpConnectFilter(RHTTPSession aSession)
: CBase(), iSession(aSession), iStringTable(RHTTPSession::GetTable()), iStringPool(aSession.StringPool()), iCallback(*this)
	{
	}

void CHttpConnectFilter::ConstructL()
	{
	// Register the filter. 
	RStringF filterName = iStringPool.StringF(HTTP::EHttpConnectFilter, iStringTable);
	iSession.FilterCollection().AddFilterL(
										  *this, 
										  THTTPEvent::ENeedTunnel, 
										  MHTTPFilter::EProtocolHandler +20,
										  filterName
										  );
	iSession.FilterCollection().AddFilterL(
										  *this, 
										  THTTPEvent::ECancel, 
										  MHTTPFilter::EProtocolHandler +20,
										  filterName
										  );
	}

TInt CHttpConnectFilter::FindConnectRequest(RStringF aTunnelHost, RStringF aProxy)
	{
	TInt id = KErrNotFound;
	TInt index = iConnectTransactions.Count();
	RStringF tunnelName = iStringPool.StringF(HTTP::ETunnel, iStringTable);
	while( index > 0 && id == KErrNotFound )
		{
		--index;
		RHTTPTransaction trans = iConnectTransactions[index];

		THTTPHdrVal tunnelVal;
#ifdef _DEBUG
		TBool found = 
#endif
		trans.PropertySet().Property(tunnelName, tunnelVal);

		__ASSERT_DEBUG( found, User::Invariant() );

		if( aTunnelHost == tunnelVal.StrF() )
			{
			// Ok the tunnel matches - check to see if a specific proxy is being
			// used.
			THTTPHdrVal proxyVal = RStringF();
			trans.PropertySet().Property(iStringPool.StringF(HTTP::EProxyAddress, iStringTable), proxyVal);

			if( proxyVal.StrF() == aProxy )
				{
				// There already is a CONNECT request for this tunnel - no need to
				// create another.
				id = trans.Id();
				}
			}
		}
	return id;
	}

TInt CHttpConnectFilter::CreateConnectTransactionL(RStringF aTunnelHost, RStringF aProxy, RStringF aUserAgent, RStringF aProfile)
	{
	// The Request-URI for the CONNECT request is in the 'authority' form, as 
	// described in RFC2616, section 5.1.2, but need to pass the request uri as
	// a TUriC8...
	_LIT8(KSchemeString, "https://%S"); // uses more code to use RString EHTTPS than we save by adding the 5 bytes here
	TUriParser8 connectUri;
	HBufC8* uri = HBufC8::NewLC(aTunnelHost.DesC().Length()+8);
	uri->Des().AppendFormat(KSchemeString(), &aTunnelHost.DesC());
	connectUri.Parse(*uri);

	// Open a CONNECT transaction...
	RStringF connectMethod = iStringPool.StringF(HTTP::ECONNECT, iStringTable);
	iNewConnectTransaction = iSession.OpenTransactionL(
													  connectUri,
													  iCallback,
													  connectMethod
													  );
	CleanupStack::PopAndDestroy(uri);
	// As the CONNECT transaction is prepared, submitted and stored, it is 
	// possible to orphan this transaction - it will need to be closed if a 
	// leave occurs.
	iCloseConnectTransaction = ETrue;

	// Set aTunnelHost as the ETunnel property in the transaction.
	RStringF tunnelName = iStringPool.StringF(HTTP::ETunnel, iStringTable);
	THTTPHdrVal tunnelVal(aTunnelHost);
	iNewConnectTransaction.PropertySet().SetPropertyL(tunnelName, tunnelVal);

	if( aProxy.DesC().Length() > 0 )
		{
		// ok need to use a specific proxy - set the appropriate properties in 
		// the transaction.
		RHTTPPropertySet properties = iNewConnectTransaction.PropertySet();
		properties.SetPropertyL(
							   iStringPool.StringF(HTTP::EProxyUsage, iStringTable),
							   iStringPool.StringF(HTTP::EUseProxy, iStringTable)
							   );
		properties.SetPropertyL(
							   iStringPool.StringF(HTTP::EProxyAddress, iStringTable),
							   aProxy
							   );
		}

	// get new transaction headers
	RHTTPHeaders hdr = iNewConnectTransaction.Request().GetHeaderCollection();

	if(aUserAgent.DesC().Length() > 0)
		{
		// add the User-agent header
		hdr.SetFieldL(
					  iStringPool.StringF(HTTP::EUserAgent, iStringTable),
					  THTTPHdrVal(aUserAgent)
					 );
		}

	if(aProfile.DesC().Length() > 0)
		{
		// add the User Agent profile header
//Note: if mmp file defines SRCDBG the next 4 lines cause a compiler bug in thumb udeb builds
// can workaround compiler bug by removing local variable & using this line instead:
//		hdr.SetFieldL(iStringPool.OpenFStringL(KUAProfile), THTTPHdrVal(aProfile));
// but its not leave safe, but could be used for debugging if SRCDBG is really needed

		RStringF profileHeader = iStringPool.OpenFStringL(KUAProfile);
		CleanupClosePushL(profileHeader);

		hdr.SetFieldL(profileHeader, THTTPHdrVal(aProfile));

		CleanupStack::PopAndDestroy(&profileHeader);
		}

	// Submit the CONNECT request...
	iNewConnectTransaction.SubmitL();

	// Store CONNECT transaction...
	User::LeaveIfError(iConnectTransactions.Append(iNewConnectTransaction));

	return iNewConnectTransaction.Id();		
	}

void CHttpConnectFilter::CloseConnectTransaction(TInt aConnectId)
	{
	// Run through connect transaction array, remove and close the one with
	// this id.
	const TInt count = iConnectTransactions.Count();
	for( TInt index = 0; index < count; ++index )
		{
		RHTTPTransaction trans = iConnectTransactions[index];
		if( trans.Id() == aConnectId )
			{
			trans.Close();
			iConnectTransactions.Remove(index);
			return;
			}
		}
	// If no CONNECT transaction found with that id - something has gone wrong.
	User::Invariant();
	}

/*
 *	Methods from MHTTPFilterBase
 */

void CHttpConnectFilter::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch( aEvent.iStatus )
		{
	case THTTPEvent::ENeedTunnel:
		{
		// Transaction needs a tunnel - need to see if a CONNECT request has 
		// been made for this origin server via the same proxy.
		aTransaction.Cancel();

		// The transaction must have the ETunnel property - this contains the
		// host/port of where the tunnel needs to lead to.
		RHTTPPropertySet properties = aTransaction.PropertySet();
		THTTPHdrVal tunnelVal;
#ifdef _DEBUG
		TBool found = 
#endif
		properties.Property(iStringPool.StringF(HTTP::ETunnel, iStringTable), tunnelVal);

		__ASSERT_DEBUG( found, User::Invariant() );
		__ASSERT_DEBUG( tunnelVal.Type() == THTTPHdrVal::KStrFVal, User::Invariant() );

		// Also, check to see if the transaction has a specific proxy to use.
		// NOTE - no need to check HTTP::EUseProxy, only need to see if 
		// the HTTP::EProxyAddress is set/
		THTTPHdrVal proxyVal = RStringF();
		properties.Property(iStringPool.StringF(HTTP::EProxyAddress, iStringTable), proxyVal);

		// See if there already is CONNECT transaction for this tunnel host/port.
		TInt connectId = FindConnectRequest(tunnelVal.StrF(), proxyVal.StrF());

		if( connectId == KErrNotFound )
			{
			// get request headers
			RHTTPHeaders hdr = aTransaction.Request().GetHeaderCollection();

			// extract user-agent header
			THTTPHdrVal userAgent = RStringF();
			hdr.GetField(iStringPool.StringF(HTTP::EUserAgent, iStringTable), 0, userAgent);

			// extract UA profile header
			RStringF profileHeader = iStringPool.OpenFStringL(KUAProfile);
			THTTPHdrVal profile = RStringF();
			hdr.GetField(profileHeader, 0, profile);
			profileHeader.Close();

			// Create a CONNECT transaction for the host/port.
			connectId = CreateConnectTransactionL(tunnelVal.StrF(), proxyVal.StrF(), userAgent.StrF(), profile.StrF());
			}
		
		// Associate this transaction with the CONNECT transaction.
		TPendingTransaction pending = TPendingTransaction(aTransaction, connectId);
		User::LeaveIfError(iPendingTransactions.Append(pending));

		// If a new CONNECT request was created, it is now safe.
		iCloseConnectTransaction = EFalse;
		} break;
	case THTTPEvent::ECancel:
		{
		// Check if this transaction is in the pending queue
		TBool found = EFalse;
		TInt index = iPendingTransactions.Count();
		while( index > 0 && !found )
			{
			--index;
			TPendingTransaction pending = iPendingTransactions[index];

			if( pending.iPendingTransaction == aTransaction )
				{
				// This transaction was waiting for a tunnel - remove it from the
				// queue. Let the CONNECT transaction for the tunnel continue as
				// there could be other pending transactions waiting for that
				// same tunnel.
				iPendingTransactions.Remove(index);
				found = ETrue;
				}
			}
		} break;
	default:
		break;
		}
	}

void CHttpConnectFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CHttpConnectFilter::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	// Something has gone a bit wrong - does the connect transaction need to be
	// closed?
	if( iCloseConnectTransaction )
		{
		// Yep, this means that leave occured after the new connect transaction 
		// was opened.
		iNewConnectTransaction.Close();
		}

	// Send the error to the client
	if(aTransaction.SendEvent(aError, THTTPEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::ECurrentFilter)) != KErrNone )
	    {
	    aTransaction.Fail();
	    }

	return KErrNone;
	}

TInt CHttpConnectFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}

/*
 *	Methods from MHTTPFilter
 */

void CHttpConnectFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	++iLoadCount;
	}

void CHttpConnectFilter::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle)
	{
	if( --iLoadCount > 0 )
		return;

	delete this;
	}

/*
 *	Methods from MHttpConnectObserver
 */

void CHttpConnectFilter::ConnectCompleteL(TInt aConnectId)
	{
	// Close this CONNECT transaction
	CloseConnectTransaction(aConnectId);

	// Locate the pending transactions that were waiting for this tunnel.
	TInt index = iPendingTransactions.Count();
	while( index > 0 )
		{
		--index;
		TPendingTransaction pending = iPendingTransactions[index];

		if( pending.iConnectId == aConnectId )
			{
			// This transaction was waiting for the tunnel - re-submit it.
			pending.iPendingTransaction.SubmitL();

			// Remove this entry.
			iPendingTransactions.Remove(index);
			}
		}
	}

void CHttpConnectFilter::ConnectErrorL(TInt aConnectId)
	{
	// Close this CONNECT transaction
	CloseConnectTransaction(aConnectId);

	// Locate the pending transactions that were waiting for this tunnel.
	TInt index = iPendingTransactions.Count();
	while( index > 0 )
		{
		--index;
		TPendingTransaction pending = iPendingTransactions[index];

		if( pending.iConnectId == aConnectId )
			{
			// This transaction was waiting for the tunnel - send error. Pretend
			// that the error came from the protocol handler. This ensures all 
			// filters are notified.
			pending.iPendingTransaction.SendEventL(
												  KErrHttpCannotEstablishTunnel,
												  THTTPEvent::EIncoming, 
												  THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)
												  );

			// Remove this entry.
			iPendingTransactions.Remove(index);
			}
		}
	}

void CHttpConnectFilter::FailPendingTransactions(TInt aConnectId)
	{
	// No need to close the CONNECT transaction - would have closed already.

	// Locate the pending transactions that were waiting for this tunnel.
	TInt index = iPendingTransactions.Count();
	while( index > 0 )
		{
		--index;
		TPendingTransaction pending = iPendingTransactions[index];

		if( pending.iConnectId == aConnectId )
			{
			// This transaction was waiting for the tunnel - fail it!
			pending.iPendingTransaction.Fail();

			// Remove this entry.
			iPendingTransactions.Remove(index);
			}
		}

	}
