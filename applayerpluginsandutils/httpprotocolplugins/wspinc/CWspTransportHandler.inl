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
//
#include <ecom/ecom.h>

/**
Data type name for WSP Transport Handler
@publishedAll
@released
*/
_LIT8(KDataTypeName, "wsp-transport-handler");


inline
CWspTransportHandler* CWspTransportHandler::NewL(RStringPool aStringPool,
												 MSecurityPolicy* aSecurityPolicy,
												 MWspCOSessionCallback& aSessionCB,
												 MWspProxyInfoProvider& aProxyInfoProv,
											     MWspCapabilityProvider& aCapProv,
											     MWspSessionHeadersProvider& aSessHdrProv)
	{
	// CWspTransportHandler ECOM Interface UID = 101F4482
	const TUid KUidWspTrHndInterface = {0x101F4482};

	// Set up a structure containing the instantiation parameters
	SInstantiationParams initParams;
	initParams.iStringPool		= &aStringPool;
	initParams.iSecurityPolicy	= aSecurityPolicy;
	initParams.iSessionCB		= &aSessionCB;
	initParams.iProxyInfoProv	= &aProxyInfoProv;
	initParams.iCapProv			= &aCapProv;
	initParams.iSessHdrProv		= &aSessHdrProv;

	// Set up the parameters which allow ECOM's resolver to pick the right implementation
	TEComResolverParams resParams;
	resParams.SetDataType(KDataTypeName());

	// Get the instantiation.  Allow ECOM to use the default resolver.  Pass in the init params.
	TAny* ptr = REComSession::CreateImplementationL(KUidWspTrHndInterface,
								 _FOFF(CWspTransportHandler, iDtor_ID_Key),
								 REINTERPRET_CAST(TAny*, &initParams),
								 resParams);

	return REINTERPRET_CAST(CWspTransportHandler*, ptr);
	}

inline
CWspTransportHandler::~CWspTransportHandler()
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

inline
CWspTransportHandler::CWspTransportHandler(RStringPool aStringPool,
										   MSecurityPolicy* aSecurityPolicy,
										   MWspCOSessionCallback& aSessionCB,
										   MWspProxyInfoProvider& aProxyInfoProv,
										   MWspCapabilityProvider& aCapProv,
										   MWspSessionHeadersProvider& aSessHdrProv)
	: iStringPool(aStringPool), iSecurityPolicy(aSecurityPolicy), 
	  iSessionCB(aSessionCB), iProxyInfoProv(aProxyInfoProv), iCapProv(aCapProv),
	  iSessHdrProv(aSessHdrProv)
	{
	}
