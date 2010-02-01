// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//
#include <wsp/mwspcosessioncallback.h>
#include <wsp/mwspcomethodcallback.h>
#include <wsp/mwspproxyinfoprovider.h>
#include <wsp/mwspsessionheadersprovider.h>
#include <wsp/mwspcapabilityprovider.h>
#include <wsp/mwspcapabilityviewer.h>
#include <wsp/mwspcapabilitysetter.h>
#include <wsp/mwspextendedmethods.h>
#include <wsp/mwspheadercodepages.h>
#include <wsp/mwspaliasaddresses.h>
#include <wsp/mwspunknowncapabilities.h>
#include <http/mhttpdatasupplier.h>
#include <uri8.h>

// Local includes
//
#include "tnwsswsptrhndpanic.h"
#include "testoom.h"

// Class signature
//
#include "CNwssWspTransportHandler.h"


// Constants used in this file
//


//
// Implementation of class  'CNwssWspTransportHandler'
//

CNwssWspTransportHandler* CNwssWspTransportHandler::NewL(TAny* aInstantiationParams)
	{
	CNwssWspTransportHandler* me = new(ELeave)CNwssWspTransportHandler(aInstantiationParams);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
	return me;
	}

CNwssWspTransportHandler::CNwssWspTransportHandler(TAny* aInstantiationParams)
: CWspTransportHandler(*(REINTERPRET_CAST(CWspTransportHandler::SInstantiationParams*, aInstantiationParams)->iStringPool),
					    (REINTERPRET_CAST(CWspTransportHandler::SInstantiationParams*, aInstantiationParams)->iSecurityPolicy),
					   *(REINTERPRET_CAST(CWspTransportHandler::SInstantiationParams*, aInstantiationParams)->iSessionCB),
					   *(REINTERPRET_CAST(CWspTransportHandler::SInstantiationParams*, aInstantiationParams)->iProxyInfoProv),
					   *(REINTERPRET_CAST(CWspTransportHandler::SInstantiationParams*, aInstantiationParams)->iCapProv),
					   *(REINTERPRET_CAST(CWspTransportHandler::SInstantiationParams*, aInstantiationParams)->iSessHdrProv))
	{
	}

CNwssWspTransportHandler::~CNwssWspTransportHandler()
	{
	delete iWspCOSession;
	iWapStackHnd.Close();
	}

CWspTransportHandler::TWspSupportedServices CNwssWspTransportHandler::SupportedServices() const
	{
	// report what this plug-in can support
	return CWspTransportHandler::ECOSessionService | CWspTransportHandler::ECOMethodInvocationService;
	}

void CNwssWspTransportHandler::ConstructL()
	{
	// Create a Connection-Oriented session handler
	MSecurityPolicy* secPol = (iSecurityPolicy? iSecurityPolicy : this);
	iWspCOSession = CNwssWspCOSession::NewL(iStringPool, *this, *this, *secPol, iSessionCB);

	// Connect to the WAP Stack server.  Leave if this fails.
	__TESTOOMD(stkErr, iWapStackHnd.Connect());
	User::LeaveIfError(stkErr);
	}

MWspCOSessionInvoker& CNwssWspTransportHandler::COSessionInvoker()
	{
	return *iWspCOSession;
	}

MWspCOMethodInvoker& CNwssWspTransportHandler::COTransactionInvoker()
	{
	return *iWspCOSession;
	}

MWspCOPushInvoker& CNwssWspTransportHandler::COPushInvoker()
	{
	TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ECOPushNotSupported);
	return (MWspCOPushInvoker&)(*(MWspCOSessionInvoker*)NULL);
	}

MWspCLMethodInvoker& CNwssWspTransportHandler::CLMethodInvoker()
	{
	TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EConnectionLessNotSupported);
	return (MWspCLMethodInvoker&)(*(MWspCOSessionInvoker*)NULL);
	}

MWspCLPushInvoker& CNwssWspTransportHandler::CLPushInvoker()
	{
	TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EConnectionLessNotSupported);
	return (MWspCLPushInvoker&)(*(MWspCOSessionInvoker*)NULL);
	}

RWAPServ& CNwssWspTransportHandler::WapStack()
	{
	return iWapStackHnd;
	}

MWspProxyInfoProvider& CNwssWspTransportHandler::ProxyInfoProvider() const
	{
	return iProxyInfoProv;
	}

MWspCapabilityProvider& CNwssWspTransportHandler::CapabilityProvider() const
	{
	return iCapProv;
	}

MWspSessionHeadersProvider& CNwssWspTransportHandler::SessionHeadersProvider() const
	{
	return iSessHdrProv;
	}

void
CNwssWspTransportHandler::ValidateUntrustedServerCert(TCertInfo& /*aServerCert*/,
													  TRequestStatus& aStatus) const
	{
	TRequestStatus* stat = &aStatus;
	User::RequestComplete(stat, KErrNone);
	}

void CNwssWspTransportHandler::CancelValidateUntrustedServerCert()
	{
	}

const RArray<TWtlsCipherSuite>& CNwssWspTransportHandler::GetWtlsCipherSuites()
	{
	return iDefSecPolCipherSuites;
	}

const TDesC8&
CNwssWspTransportHandler::GetTlsCipherSuites()
	{
	return KNullDesC8();
	}

const RArray<TWtlsKeyExchangeSuite>& CNwssWspTransportHandler::GetWtlsKeyExchangeSuites()
	{
	return iDefSecPolKeyExchSuite;
	}

TInt
CNwssWspTransportHandler::GetNamedPolicyProperty(RStringF /*aPropertyName*/, RString& /*aPropertyValue*/)
	{
	return KErrNotFound;
	}

void CNwssWspTransportHandler::Reserved1()
	{
	}

void CNwssWspTransportHandler::Reserved2()
	{
	}

TInt CNwssWspTransportHandler::ServerCert(TCertInfo& aCertInfo) const
	{
	return iWspCOSession->ServerCert(aCertInfo);
	}

