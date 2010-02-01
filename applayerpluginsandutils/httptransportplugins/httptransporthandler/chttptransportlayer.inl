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

#include <ecom/ecom.h>

inline
THttpTransportConstructionParams::THttpTransportConstructionParams(MConnectionPrefsProvider& aConnectionPrefsProvider)
:iConnectionPrefsProvider(aConnectionPrefsProvider)
/**
	Constructor.
	@param		aConnectionPrefsProvider	Reference to a comms connection 
											callback provider.
*/
	{
	iPriority= EFalse;
	}

inline
CHttpTransportLayer* CHttpTransportLayer::NewL(const TDesC8& aTransportType, THttpTransportConstructionParams& aHttpTransportParams)
/**	
	The factory contructor. Uses ECOM to find the appropriate implementation.
	@param		aTransportType The underlying transport protocol, e.g. tcp.
	@param		aHttpTransportParams Required construction parameters
	@return		A pointer to a fully constructed object.
*/
	{
	// CHttpTransportLayer ECOM Interface UID = 101F55EC
	const TUid KUidHttpTransportLayer = {0x101F55EC};

	// Set resolving parameters
	TEComResolverParams resolverParams;
	resolverParams.SetDataType(aTransportType);
	resolverParams.SetWildcardMatch(ETrue);

	// Get the instantiation.  Allow ECOM to use the default resolver.
	TAny* ptr = REComSession::CreateImplementationL(
												   KUidHttpTransportLayer,
												   _FOFF(CHttpTransportLayer, iDtor_ID_Key),
												   &aHttpTransportParams,
												   resolverParams
												   );

	return REINTERPRET_CAST(CHttpTransportLayer*, ptr);
	}

inline
CHttpTransportLayer::~CHttpTransportLayer()
/**
	Destructor.
*/
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

inline
CHttpTransportLayer::CHttpTransportLayer()
: CBase()
/**	
	Constructor.
*/
	{
	}

inline
MSocketFactory& CHttpTransportLayer::SocketFactory()
/**
	Accessor to the socket factory object.
	@return		A reference to the socket factory object.
*/
	{
	return *this;
	}

