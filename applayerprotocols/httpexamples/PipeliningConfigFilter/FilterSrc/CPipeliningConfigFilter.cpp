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

#include "CPipeliningConfigFilter.h"
#include "CDomainList.h"
#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <httpstringconstants.h>

const TInt KFilterPosition = MHTTPFilter::ECache+10;	// Position of the filter in filter queue
const TInt KMaxDomainList = 50;							// Maximum number of elements in domain list
_LIT8(KPipeliningConfigFilterName, "_PCF");				// Filter name

CPipeliningConfigFilter::CPipeliningConfigFilter()
	{ 
	}

CEComFilter* CPipeliningConfigFilter::InstallFilterL(TAny* aSession)
	{
	RHTTPSession* session = reinterpret_cast<RHTTPSession*>(aSession);
	CPipeliningConfigFilter* filter = new (ELeave) CPipeliningConfigFilter();
	CleanupStack::PushL(filter);
	filter->ConstructL(*session);
	CleanupStack::Pop(filter); 
	return filter;
	}

void CPipeliningConfigFilter::ConstructL(RHTTPSession aSession)
	{
	iDomainList = CDomainList::NewL(KMaxDomainList);
	iStringPool = aSession.StringPool();
	iFilterName = iStringPool.OpenFStringL(KPipeliningConfigFilterName());
	
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::ESubmit,				// Register for submit events
											RStringF(),								// Any header
											KAnyStatusCode,							// Any status code
											KFilterPosition,						// Priority of filter
											iFilterName);							// Name of filter
											
	aSession.FilterCollection().AddFilterL(*this, THTTPEvent::EGotResponseHeaders,	// Register for got response header events
											RStringF(),								// Any header
											KAnyStatusCode,							// Any status code
											KFilterPosition,						// Priority of filter
											iFilterName);							// Name of filter
	}

CPipeliningConfigFilter::~CPipeliningConfigFilter()
	{
	iFilterName.Close();
	delete iDomainList;
	}


void CPipeliningConfigFilter::MHFUnload(RHTTPSession,THTTPFilterHandle)
	{
	if (--iLoadCount)
		return;

	delete this;
	}

void CPipeliningConfigFilter::MHFLoad(RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/)
	{
	++iLoadCount;
	}

void CPipeliningConfigFilter::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	// Check that the event in a HTTP event
	if (aEvent.iUID != KHTTPUid)
		return;

	switch(aEvent.iStatus)
		{
	case THTTPEvent::ESubmit:
		{
		// Submit event
		ProcessSubmitTransactionL(aTransaction);
		}
		break;
	case THTTPEvent::EGotResponseHeaders:
		{
		// Got response headers event
		ProcessRulesL(aTransaction);
		}
		break;
	default:
		// Does not ned to do anything here.
		break;
		}
	}


TInt CPipeliningConfigFilter::MHFRunError(TInt /*aError*/, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	// If anything left, we've run out of memory or something
	// similarly catastrophic has gone wrong.
	aTransaction.Fail();
	return KErrNone;
	}

void CPipeliningConfigFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CPipeliningConfigFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}
	
void CPipeliningConfigFilter::ProcessSubmitTransactionL(RHTTPTransaction& aTransaction)
	{
	// Need to check if the domain is in the domain list, if not we must disable
	// pipelining on this transaction
	RStringF domain = iStringPool.OpenFStringL( aTransaction.Request().URI().Extract(EUriHost) );
	CleanupClosePushL(domain);
	
	if( iDomainList->IsDomainInList(domain)==KErrNotFound )
		DisablePipeliningL(aTransaction); // Not in list, disable pipelining
	
	CleanupStack::PopAndDestroy(&domain);
	}

void CPipeliningConfigFilter::DisablePipeliningL(RHTTPTransaction& aTransaction)
	{
	// Disable pipelining on the supplied transaction
	RStringF property = iStringPool.StringF(HTTP::EHttpPipelining, RHTTPSession::GetTable());
	THTTPHdrVal disablePipelining = iStringPool.StringF(HTTP::EDisablePipelining, RHTTPSession::GetTable());
	aTransaction.PropertySet().SetPropertyL(property, disablePipelining);
	}
	
void CPipeliningConfigFilter::ProcessRulesL(RHTTPTransaction& aTransaction)
	{
	// Process through the rules in nested 'if' statements and if all rules
	// are complied with, add the domain to the to domain list.

	// Rule 1: Is the transaction HTTP version 1.1
	if( IsHTTP11(aTransaction) )
		{
		// Transaction is HTTP/1.1
		// Rule 2: The transaction is not DIGEST authentication
		if( !IsDIGESTAuthenticationL(aTransaction) )
			{
			// if we get here all rules have passed so add the domain to the list
			AddDomainToListL(aTransaction);
			}
		}
	}
	
void CPipeliningConfigFilter::AddDomainToListL(RHTTPTransaction& aTransaction)
	{
	// Add the domain from the supplied transaction to the domain list,
	// need to convert from descriptor to RStringF
	TPtrC8 domainDes = aTransaction.Request().URI().Extract(EUriHost);
	RStringF domain = iStringPool.OpenFStringL(domainDes);
	CleanupClosePushL(domain);
	iDomainList->AddDomainToListL(domain); // iDomainList takes ownership of domain
	CleanupStack::Pop(&domain);
	}
	
TBool CPipeliningConfigFilter::IsHTTP11(RHTTPTransaction& aTransaction)
	{
	TInt8 versionVal = 1; // Compare the response version against this
	TVersion version = aTransaction.Response().Version();
	if( (version.iMajor==versionVal) && (version.iMinor==versionVal) )
		{
		// HTTP version is 1.1
		return ETrue;
		}
	
	// Not HTTP/1.1	
	return EFalse;
	}

TBool CPipeliningConfigFilter::IsDIGESTAuthenticationL(RHTTPTransaction& aTransaction)
	{
	const TInt authStatusCode = 401;
	if( aTransaction.Response().StatusCode() == authStatusCode )
		{
		// We have a HTTP 401 status code
		RHTTPHeaders headers = aTransaction.Response().GetHeaderCollection();
		RStringF wwwAuthenticate = iStringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable());

		// Go through all the parts and look for DIGEST
		const TInt parts = headers.FieldPartsL(wwwAuthenticate);
		for (TInt ii=0; ii<parts; ++ii)
			{
			THTTPHdrVal fieldVal;
			headers.GetField(wwwAuthenticate, ii, fieldVal);
			TInt hdrValIndex = fieldVal.StrF().Index(RHTTPSession::GetTable());
			if( hdrValIndex == HTTP::EDigest )
				{
				// Transaction is DIGEST authentication
				return ETrue;
				}
			}
		}
	
	// This is not a DIGEST authentication response
	return EFalse;
	}

